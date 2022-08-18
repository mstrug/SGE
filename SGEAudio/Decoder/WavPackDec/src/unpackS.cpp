/*
 * unpackS.cpp
 *
 *  Created on: 2009-09-18
 *      Author: Michal Strug
 *   Copyright: Copyright (c) 2022 Michal Strug. All rights reserved.
 */

#include "wavpack_local.h"


#define FAST_DECODE

#define LOSSY_MUTE


int unpack_init(WavpackContext *wpc)
{
    WavpackStream *wps = wpc->streams [wpc->current_stream];
    uchar *blockptr, *block2ptr;
    WavpackMetadata wpmd;

    wps->mute_error = FALSE;
    wps->crc = wps->crc_x = 0xffffffff;
    CLEAR (wps->wvbits);
    CLEAR (wps->wvcbits);
    CLEAR (wps->wvxbits);
    CLEAR (wps->decorr_passes);
    CLEAR (wps->dc);
    CLEAR (wps->w);

    if (!(wps->wphdr.flags & MONO_FLAG) && wpc->config.num_channels && wps->wphdr.block_samples &&
        (wpc->reduced_channels == 1 || wpc->config.num_channels == 1)) {
            wps->mute_error = TRUE;
            return FALSE;
    }

    if ((wps->wphdr.flags & UNKNOWN_FLAGS) || (wps->wphdr.flags & MONO_DATA) == MONO_DATA) {
        wps->mute_error = TRUE;
        return FALSE;
    }

    blockptr = wps->blockbuff + sizeof (WavpackHeader);

    while (read_metadata_buff (&wpmd, wps->blockbuff, &blockptr))
        if (!process_metadata (wpc, &wpmd)) {
            wps->mute_error = TRUE;
            return FALSE;
        }

    if (wps->wphdr.block_samples && wpc->wvc_flag && wps->block2buff) {
        block2ptr = wps->block2buff + sizeof (WavpackHeader);

        while (read_metadata_buff (&wpmd, wps->block2buff, &block2ptr))
            if (!process_metadata (wpc, &wpmd)) {
                wps->mute_error = TRUE;
                return FALSE;
            }
    }

    if (wps->wphdr.block_samples && !bs_is_open (&wps->wvbits)) {
        if (bs_is_open (&wps->wvcbits))
        	{
        	Mem::Copy( wpc->error_message, "can't unpack correction files alone!\0", 37 );
        	}

        wps->mute_error = TRUE;
        return FALSE;
    }

    if (wps->wphdr.block_samples && !bs_is_open (&wps->wvxbits)) {
        if ((wps->wphdr.flags & INT32_DATA) && wps->int32_sent_bits)
            wpc->lossy_blocks = TRUE;

        if ((wps->wphdr.flags & FLOAT_DATA) &&
            wps->float_flags & (FLOAT_EXCEPTIONS | FLOAT_ZEROS_SENT | FLOAT_SHIFT_SENT | FLOAT_SHIFT_SAME))
                wpc->lossy_blocks = TRUE;
    }

    if (wps->wphdr.block_samples)
        wps->sample_index = wps->wphdr.block_index;

    return TRUE;
}


int init_wv_bitstream (WavpackStream *wps, WavpackMetadata *wpmd)
{
    if (!wpmd->byte_length)
        return FALSE;

    bs_open_read (&wps->wvbits, wpmd->data, (uchar *) wpmd->data + wpmd->byte_length);
    return TRUE;
}

int init_wvc_bitstream (WavpackStream *wps, WavpackMetadata *wpmd)
{
    if (!wpmd->byte_length)
        return FALSE;

    bs_open_read (&wps->wvcbits, wpmd->data, (uchar *) wpmd->data + wpmd->byte_length);
    return TRUE;
}

int init_wvx_bitstream (WavpackStream *wps, WavpackMetadata *wpmd)
{
    uchar *cp = (uchar*) wpmd->data;

    if (wpmd->byte_length <= 4)
        return FALSE;

    wps->crc_wvx = *cp++;
    wps->crc_wvx |= (int32_t) *cp++ << 8;
    wps->crc_wvx |= (int32_t) *cp++ << 16;
    wps->crc_wvx |= (int32_t) *cp++ << 24;

    bs_open_read (&wps->wvxbits, cp, (uchar *) wpmd->data + wpmd->byte_length);
    return TRUE;
}

int read_decorr_terms (WavpackStream *wps, WavpackMetadata *wpmd)
{
    int termcnt = wpmd->byte_length;
    uchar *byteptr = (uchar*) wpmd->data;
    struct decorr_pass *dpp;

    if (termcnt > MAX_NTERMS)
        return FALSE;

    wps->num_terms = termcnt;

    for (dpp = wps->decorr_passes + termcnt - 1; termcnt--; dpp--) {
        dpp->term = (int)(*byteptr & 0x1f) - 5;
        dpp->delta = (*byteptr++ >> 5) & 0x7;

        if (!dpp->term || dpp->term < -3 || (dpp->term > MAX_TERM && dpp->term < 17) || dpp->term > 18)
            return FALSE;
    }

    return TRUE;
}


int read_decorr_weights (WavpackStream *wps, WavpackMetadata *wpmd)
{
    int termcnt = wpmd->byte_length, tcount;
    char *byteptr = (char*) wpmd->data;
    struct decorr_pass *dpp;

    if (!(wps->wphdr.flags & MONO_DATA))
        termcnt /= 2;

    if (termcnt > wps->num_terms)
        return FALSE;

    for (tcount = wps->num_terms, dpp = wps->decorr_passes; tcount--; dpp++)
        dpp->weight_A = dpp->weight_B = 0;

    while (--dpp >= wps->decorr_passes && termcnt--) {
        dpp->weight_A = restore_weight (*byteptr++);

        if (!(wps->wphdr.flags & MONO_DATA))
            dpp->weight_B = restore_weight (*byteptr++);
    }

    return TRUE;
}




int read_decorr_samples (WavpackStream *wps, WavpackMetadata *wpmd)
{
    uchar *byteptr = (uchar*) wpmd->data;
    uchar *endptr = byteptr + wpmd->byte_length;
    struct decorr_pass *dpp;
    int tcount;

    for (tcount = wps->num_terms, dpp = wps->decorr_passes; tcount--; dpp++) {
        CLEAR (dpp->samples_A);
        CLEAR (dpp->samples_B);
    }

    if (wps->wphdr.version == 0x402 && (wps->wphdr.flags & HYBRID_FLAG)) {
        if (byteptr + (wps->wphdr.flags & MONO_DATA ? 2 : 4) > endptr)
            return FALSE;

        wps->dc.error [0] = exp2s ((short)(byteptr [0] + (byteptr [1] << 8)));
        byteptr += 2;

        if (!(wps->wphdr.flags & MONO_DATA)) {
            wps->dc.error [1] = exp2s ((short)(byteptr [0] + (byteptr [1] << 8)));
            byteptr += 2;
        }
    }

    while (dpp-- > wps->decorr_passes && byteptr < endptr)
        if (dpp->term > MAX_TERM) {
            if (byteptr + (wps->wphdr.flags & MONO_DATA ? 4 : 8) > endptr)
                return FALSE;

            dpp->samples_A [0] = exp2s ((short)(byteptr [0] + (byteptr [1] << 8)));
            dpp->samples_A [1] = exp2s ((short)(byteptr [2] + (byteptr [3] << 8)));
            byteptr += 4;

            if (!(wps->wphdr.flags & MONO_DATA)) {
                dpp->samples_B [0] = exp2s ((short)(byteptr [0] + (byteptr [1] << 8)));
                dpp->samples_B [1] = exp2s ((short)(byteptr [2] + (byteptr [3] << 8)));
                byteptr += 4;
            }
        }
        else if (dpp->term < 0) {
            if (byteptr + 4 > endptr)
                return FALSE;

            dpp->samples_A [0] = exp2s ((short)(byteptr [0] + (byteptr [1] << 8)));
            dpp->samples_B [0] = exp2s ((short)(byteptr [2] + (byteptr [3] << 8)));
            byteptr += 4;
        }
        else {
            int m = 0, cnt = dpp->term;

            while (cnt--) {
                if (byteptr + (wps->wphdr.flags & MONO_DATA ? 2 : 4) > endptr)
                    return FALSE;

                dpp->samples_A [m] = exp2s ((short)(byteptr [0] + (byteptr [1] << 8)));
                byteptr += 2;

                if (!(wps->wphdr.flags & MONO_DATA)) {
                    dpp->samples_B [m] = exp2s ((short)(byteptr [0] + (byteptr [1] << 8)));
                    byteptr += 2;
                }

                m++;
            }
        }

    return byteptr == endptr;
}



int read_shaping_info (WavpackStream *wps, WavpackMetadata *wpmd)
{
    if (wpmd->byte_length == 2) {
        char *byteptr = (char*) wpmd->data;

        wps->dc.shaping_acc [0] = (int32_t) restore_weight (*byteptr++) << 16;
        wps->dc.shaping_acc [1] = (int32_t) restore_weight (*byteptr++) << 16;
        return TRUE;
    }
    else if (wpmd->byte_length >= (wps->wphdr.flags & MONO_DATA ? 4 : 8)) {
        uchar *byteptr = (uchar*) wpmd->data;

        wps->dc.error [0] = exp2s ((short)(byteptr [0] + (byteptr [1] << 8)));
        wps->dc.shaping_acc [0] = exp2s ((short)(byteptr [2] + (byteptr [3] << 8)));
        byteptr += 4;

        if (!(wps->wphdr.flags & MONO_DATA)) {
            wps->dc.error [1] = exp2s ((short)(byteptr [0] + (byteptr [1] << 8)));
            wps->dc.shaping_acc [1] = exp2s ((short)(byteptr [2] + (byteptr [3] << 8)));
            byteptr += 4;
        }

        if (wpmd->byte_length == (wps->wphdr.flags & MONO_DATA ? 6 : 12)) {
            wps->dc.shaping_delta [0] = exp2s ((short)(byteptr [0] + (byteptr [1] << 8)));

            if (!(wps->wphdr.flags & MONO_DATA))
                wps->dc.shaping_delta [1] = exp2s ((short)(byteptr [2] + (byteptr [3] << 8)));
        }

        return TRUE;
    }

    return FALSE;
}



int read_int32_info (WavpackStream *wps, WavpackMetadata *wpmd)
{
    int bytecnt = wpmd->byte_length;
    char *byteptr = (char*) wpmd->data;

    if (bytecnt != 4)
        return FALSE;

    wps->int32_sent_bits = *byteptr++;
    wps->int32_zeros = *byteptr++;
    wps->int32_ones = *byteptr++;
    wps->int32_dups = *byteptr;

    return TRUE;
}



int read_channel_info (WavpackContext *wpc, WavpackMetadata *wpmd)
{
    int bytecnt = wpmd->byte_length, shift = 0;
    uchar *byteptr = (uchar*) wpmd->data;
    uint32_t mask = 0;

    if (!bytecnt || bytecnt > 5)
        return FALSE;

    if (!wpc->config.num_channels) {
        wpc->config.num_channels = *byteptr++;

        while (--bytecnt) {
            mask |= (uint32_t) *byteptr++ << shift;
            shift += 8;
        }

        wpc->config.channel_mask = mask;
    }

    return TRUE;
}


int read_config_info (WavpackContext *wpc, WavpackMetadata *wpmd)
{
    int bytecnt = wpmd->byte_length;
    uchar *byteptr = (uchar*)wpmd->data;

    if (bytecnt >= 3) {
        wpc->config.flags &= 0xff;
        wpc->config.flags |= (int32_t) *byteptr++ << 8;
        wpc->config.flags |= (int32_t) *byteptr++ << 16;
        wpc->config.flags |= (int32_t) *byteptr++ << 24;

        if (bytecnt >= 4 && (wpc->config.flags & CONFIG_EXTRA_MODE))
            wpc->config.xmode = *byteptr;
    }

    return TRUE;
}



int read_sample_rate (WavpackContext *wpc, WavpackMetadata *wpmd)
{
    int bytecnt = wpmd->byte_length;
    uchar *byteptr = (uchar*) wpmd->data;

    if (bytecnt == 3) {
        wpc->config.sample_rate = (int32_t) *byteptr++;
        wpc->config.sample_rate |= (int32_t) *byteptr++ << 8;
        wpc->config.sample_rate |= (int32_t) *byteptr++ << 16;
    }

    return TRUE;
}



int read_wrapper_data (WavpackContext *wpc, WavpackMetadata *wpmd)
{
    if ((wpc->open_flags & OPEN_WRAPPER) && wpc->wrapper_bytes < MAX_WRAPPER_BYTES) 
    	{
    	wpc->wrapper_data = (uchar*)realloc (wpc->wrapper_data, wpc->wrapper_bytes + wpmd->byte_length);
        
        memcpy (wpc->wrapper_data + wpc->wrapper_bytes, wpmd->data, wpmd->byte_length);
        wpc->wrapper_bytes += wpmd->byte_length;
    }

    return TRUE;
}




#ifndef NO_UNPACK


static void decorr_stereo_pass (struct decorr_pass *dpp, int32_t *buffer, int32_t sample_count);
static void decorr_stereo_pass_i (struct decorr_pass *dpp, int32_t *buffer, int32_t sample_count);
static void decorr_stereo_pass_1717 (struct decorr_pass *dpp, int32_t *buffer, int32_t sample_count);
static void decorr_stereo_pass_1718 (struct decorr_pass *dpp, int32_t *buffer, int32_t sample_count);
static void decorr_stereo_pass_1818 (struct decorr_pass *dpp, int32_t *buffer, int32_t sample_count);
static void decorr_stereo_pass_nn (struct decorr_pass *dpp, int32_t *buffer, int32_t sample_count);
static void fixup_samples (WavpackContext *wpc, int32_t *buffer, uint32_t sample_count);

int32_t unpack_samples (WavpackContext *wpc, int32_t *buffer, uint32_t sample_count)
{
    WavpackStream *wps = wpc->streams [wpc->current_stream];
    uint32_t flags = wps->wphdr.flags, crc = wps->crc, i;
    int32_t mute_limit = (1L << ((flags & MAG_MASK) >> MAG_LSB)) + 2;
    int32_t correction [2], read_word, *bptr;
    struct decorr_pass *dpp;
    int tcount, m = 0;

    if (wps->sample_index + sample_count > wps->wphdr.block_index + wps->wphdr.block_samples)
        sample_count = wps->wphdr.block_index + wps->wphdr.block_samples - wps->sample_index;

    if (wps->mute_error) {
        if (wpc->reduced_channels == 1 || wpc->config.num_channels == 1 || (flags & MONO_FLAG))
            memset (buffer, 0, sample_count * 4);
        else
            memset (buffer, 0, sample_count * 8);

        wps->sample_index += sample_count;
        return sample_count;
    }

    if ((flags & HYBRID_FLAG) && !wps->block2buff)
        mute_limit *= 2;

    //////////////// handle lossless or hybrid lossy mono data /////////////////

    if (!wps->block2buff && (flags & MONO_DATA)) {
        int32_t *eptr = buffer + sample_count;

        if (flags & HYBRID_FLAG) {
            i = sample_count;

            for (bptr = buffer; bptr < eptr;)
                if ((*bptr++ = get_word (wps, 0, NULL)) == WORD_EOF) {
                    i = (uint32_t)(bptr - buffer);
                    break;
                }
        }
        else
            i = get_words_lossless (wps, buffer, sample_count);

        for (bptr = buffer; bptr < eptr;) {
            read_word = *bptr;

            for (tcount = wps->num_terms, dpp = wps->decorr_passes; tcount--; dpp++) {
                int32_t sam, temp;
                int k;

                if (dpp->term > MAX_TERM) {
                    if (dpp->term & 1)
                        sam = 2 * dpp->samples_A [0] - dpp->samples_A [1];
                    else
                        sam = dpp->samples_A [0] + ((dpp->samples_A [0] - dpp->samples_A [1]) >> 1);

                    dpp->samples_A [1] = dpp->samples_A [0];
                    k = 0;
                }
                else {
                    sam = dpp->samples_A [m];
                    k = (m + dpp->term) & (MAX_TERM - 1);
                }

                temp = apply_weight (dpp->weight_A, sam) + read_word;
                update_weight (dpp->weight_A, dpp->delta, sam, read_word);
                dpp->samples_A [k] = read_word = temp;
            }

            if (labs (read_word) > mute_limit) {
                i = (uint32_t)(bptr - buffer);
                break;
            }

            m = (m + 1) & (MAX_TERM - 1);
            crc += (crc << 1) + (*bptr++ = read_word);
        }
    }

    /////////////// handle lossless or hybrid lossy stereo data ///////////////

    else if (!wps->block2buff && !(flags & MONO_DATA)) {
        int32_t *eptr = buffer + (sample_count * 2);

        if (flags & HYBRID_FLAG) {
            i = sample_count;

            for (bptr = buffer; bptr < eptr; bptr += 2)
                if ((bptr [0] = get_word (wps, 0, NULL)) == WORD_EOF ||
                    (bptr [1] = get_word (wps, 1, NULL)) == WORD_EOF) {
                        i = (uint32_t)(bptr - buffer) / 2;
                        break;
                }
        }
        else
            i = get_words_lossless (wps, buffer, sample_count);

#ifdef FAST_DECODE
        for (tcount = wps->num_terms, dpp = wps->decorr_passes; tcount--; dpp++)
            if (((flags & MAG_MASK) >> MAG_LSB) >= 16)
                decorr_stereo_pass (dpp, buffer, sample_count);
            else if (tcount && dpp [0].term == 17 && dpp [1].term == 17) {
                decorr_stereo_pass_1717 (dpp, buffer, sample_count);
                tcount--;
                dpp++;
            }
            else if (tcount && dpp [0].term == 17 && dpp [1].term == 18) {
                decorr_stereo_pass_1718 (dpp, buffer, sample_count);
                tcount--;
                dpp++;
            }
            else if (tcount && dpp [0].term == 18 && dpp [1].term == 18) {
                decorr_stereo_pass_1818 (dpp, buffer, sample_count);
                tcount--;
                dpp++;
            }
            else if (tcount && dpp [0].term >= 1 && dpp [0].term <= 7 &&
                               dpp [1].term >= 1 && dpp [1].term <= 7) {
                decorr_stereo_pass_nn (dpp, buffer, sample_count);
                tcount--;
                dpp++;
            }
            else
                decorr_stereo_pass_i (dpp, buffer, sample_count);
#else
        for (tcount = wps->num_terms, dpp = wps->decorr_passes; tcount--; dpp++)
            decorr_stereo_pass (dpp, buffer, sample_count);
#endif

        if (flags & JOINT_STEREO)
            for (bptr = buffer; bptr < eptr; bptr += 2) {
                bptr [0] += (bptr [1] -= (bptr [0] >> 1));
                crc += (crc << 3) + (bptr [0] << 1) + bptr [0] + bptr [1];
            }
        else
            for (bptr = buffer; bptr < eptr; bptr += 2)
                crc += (crc << 3) + (bptr [0] << 1) + bptr [0] + bptr [1];

        for (bptr = buffer; bptr < eptr; bptr += 16)
            if (labs (bptr [0]) > mute_limit || labs (bptr [1]) > mute_limit) {
                i = (uint32_t)(bptr - buffer) / 2;
                break;
            }

        m = sample_count & (MAX_TERM - 1);
    }

    /////////////////// handle hybrid lossless mono data ////////////////////

    else if ((flags & HYBRID_FLAG) && (flags & MONO_DATA))
        for (bptr = buffer, i = 0; i < sample_count; ++i) {

            if ((read_word = get_word (wps, 0, correction)) == WORD_EOF)
                break;

            for (tcount = wps->num_terms, dpp = wps->decorr_passes; tcount--; dpp++) {
                int32_t sam, temp;
                int k;

                if (dpp->term > MAX_TERM) {
                    if (dpp->term & 1)
                        sam = 2 * dpp->samples_A [0] - dpp->samples_A [1];
                    else
                        sam = (3 * dpp->samples_A [0] - dpp->samples_A [1]) >> 1;

                    dpp->samples_A [1] = dpp->samples_A [0];
                    k = 0;
                }
                else {
                    sam = dpp->samples_A [m];
                    k = (m + dpp->term) & (MAX_TERM - 1);
                }

                temp = apply_weight (dpp->weight_A, sam) + read_word;
                update_weight (dpp->weight_A, dpp->delta, sam, read_word);
                dpp->samples_A [k] = read_word = temp;
            }

            m = (m + 1) & (MAX_TERM - 1);

            if (flags & HYBRID_SHAPE) {
                int shaping_weight = (wps->dc.shaping_acc [0] += wps->dc.shaping_delta [0]) >> 16;
                int32_t temp = -apply_weight (shaping_weight, wps->dc.error [0]);

                if ((flags & NEW_SHAPING) && shaping_weight < 0 && temp) {
                    if (temp == wps->dc.error [0])
                        temp = (temp < 0) ? temp + 1 : temp - 1;

                    wps->dc.error [0] = temp - correction [0];
                }
                else
                    wps->dc.error [0] = -correction [0];

                read_word += correction [0] - temp;
            }
            else
                read_word += correction [0];

            crc += (crc << 1) + read_word;

#ifdef LOSSY_MUTE
            if (labs (read_word) > mute_limit)
                break;
#endif
            *bptr++ = read_word;
        }

    //////////////////// handle hybrid lossless stereo data ///////////////////

    else if (wps->block2buff && !(flags & MONO_DATA))
        for (bptr = buffer, i = 0; i < sample_count; ++i) {
            int32_t left, right, left2, right2;
            int32_t left_c = 0, right_c = 0;

            if ((left = get_word (wps, 0, correction)) == WORD_EOF ||
                (right = get_word (wps, 1, correction + 1)) == WORD_EOF)
                    break;

            if (flags & CROSS_DECORR) {
                left_c = left + correction [0];
                right_c = right + correction [1];

                for (tcount = wps->num_terms, dpp = wps->decorr_passes; tcount--; dpp++) {
                    int32_t sam_A, sam_B;

                    if (dpp->term > 0) {
                        if (dpp->term > MAX_TERM) {
                            if (dpp->term & 1) {
                                sam_A = 2 * dpp->samples_A [0] - dpp->samples_A [1];
                                sam_B = 2 * dpp->samples_B [0] - dpp->samples_B [1];
                            }
                            else {
                                sam_A = (3 * dpp->samples_A [0] - dpp->samples_A [1]) >> 1;
                                sam_B = (3 * dpp->samples_B [0] - dpp->samples_B [1]) >> 1;
                            }
                        }
                        else {
                            sam_A = dpp->samples_A [m];
                            sam_B = dpp->samples_B [m];
                        }

                        left_c += apply_weight (dpp->weight_A, sam_A);
                        right_c += apply_weight (dpp->weight_B, sam_B);
                    }
                    else if (dpp->term == -1) {
                        left_c += apply_weight (dpp->weight_A, dpp->samples_A [0]);
                        right_c += apply_weight (dpp->weight_B, left_c);
                    }
                    else {
                        right_c += apply_weight (dpp->weight_B, dpp->samples_B [0]);

                        if (dpp->term == -3)
                            left_c += apply_weight (dpp->weight_A, dpp->samples_A [0]);
                        else
                            left_c += apply_weight (dpp->weight_A, right_c);
                    }
                }

                if (flags & JOINT_STEREO)
                    left_c += (right_c -= (left_c >> 1));
            }

            for (tcount = wps->num_terms, dpp = wps->decorr_passes; tcount--; dpp++) {
                int32_t sam_A, sam_B;

                if (dpp->term > 0) {
                    int k;

                    if (dpp->term > MAX_TERM) {
                        if (dpp->term & 1) {
                            sam_A = 2 * dpp->samples_A [0] - dpp->samples_A [1];
                            sam_B = 2 * dpp->samples_B [0] - dpp->samples_B [1];
                        }
                        else {
                            sam_A = (3 * dpp->samples_A [0] - dpp->samples_A [1]) >> 1;
                            sam_B = (3 * dpp->samples_B [0] - dpp->samples_B [1]) >> 1;
                        }

                        dpp->samples_A [1] = dpp->samples_A [0];
                        dpp->samples_B [1] = dpp->samples_B [0];
                        k = 0;
                    }
                    else {
                        sam_A = dpp->samples_A [m];
                        sam_B = dpp->samples_B [m];
                        k = (m + dpp->term) & (MAX_TERM - 1);
                    }

                    left2 = apply_weight (dpp->weight_A, sam_A) + left;
                    right2 = apply_weight (dpp->weight_B, sam_B) + right;

                    update_weight (dpp->weight_A, dpp->delta, sam_A, left);
                    update_weight (dpp->weight_B, dpp->delta, sam_B, right);

                    dpp->samples_A [k] = left = left2;
                    dpp->samples_B [k] = right = right2;
                }
                else if (dpp->term == -1) {
                    left2 = left + apply_weight (dpp->weight_A, dpp->samples_A [0]);
                    update_weight_clip (dpp->weight_A, dpp->delta, dpp->samples_A [0], left);
                    left = left2;
                    right2 = right + apply_weight (dpp->weight_B, left2);
                    update_weight_clip (dpp->weight_B, dpp->delta, left2, right);
                    dpp->samples_A [0] = right = right2;
                }
                else {
                    right2 = right + apply_weight (dpp->weight_B, dpp->samples_B [0]);
                    update_weight_clip (dpp->weight_B, dpp->delta, dpp->samples_B [0], right);
                    right = right2;

                    if (dpp->term == -3) {
                        right2 = dpp->samples_A [0];
                        dpp->samples_A [0] = right;
                    }

                    left2 = left + apply_weight (dpp->weight_A, right2);
                    update_weight_clip (dpp->weight_A, dpp->delta, right2, left);
                    dpp->samples_B [0] = left = left2;
                }
            }

            m = (m + 1) & (MAX_TERM - 1);

            if (!(flags & CROSS_DECORR)) {
                left_c = left + correction [0];
                right_c = right + correction [1];

                if (flags & JOINT_STEREO)
                    left_c += (right_c -= (left_c >> 1));
            }

            if (flags & JOINT_STEREO)
                left += (right -= (left >> 1));

            if (flags & HYBRID_SHAPE) {
                int shaping_weight;
                int32_t temp;

                correction [0] = left_c - left;
                shaping_weight = (wps->dc.shaping_acc [0] += wps->dc.shaping_delta [0]) >> 16;
                temp = -apply_weight (shaping_weight, wps->dc.error [0]);

                if ((flags & NEW_SHAPING) && shaping_weight < 0 && temp) {
                    if (temp == wps->dc.error [0])
                        temp = (temp < 0) ? temp + 1 : temp - 1;

                    wps->dc.error [0] = temp - correction [0];
                }
                else
                    wps->dc.error [0] = -correction [0];

                left = left_c - temp;
                correction [1] = right_c - right;
                shaping_weight = (wps->dc.shaping_acc [1] += wps->dc.shaping_delta [1]) >> 16;
                temp = -apply_weight (shaping_weight, wps->dc.error [1]);

                if ((flags & NEW_SHAPING) && shaping_weight < 0 && temp) {
                    if (temp == wps->dc.error [1])
                        temp = (temp < 0) ? temp + 1 : temp - 1;

                    wps->dc.error [1] = temp - correction [1];
                }
                else
                    wps->dc.error [1] = -correction [1];

                right = right_c - temp;
            }
            else {
                left = left_c;
                right = right_c;
            }

#ifdef LOSSY_MUTE
            if (labs (left) > mute_limit || labs (right) > mute_limit)
                break;
#endif
            crc += (crc << 3) + (left << 1) + left + right;
            *bptr++ = left;
            *bptr++ = right;
        }
    else
        i = 0;  /* this line can't execute, but suppresses compiler warning */

    if (i != sample_count) {
        memset (buffer, 0, sample_count * (flags & MONO_FLAG ? 4 : 8));
        wps->mute_error = TRUE;
        i = sample_count;

        if (bs_is_open (&wps->wvxbits))
            bs_close_read (&wps->wvxbits);
    }

    if (m)
        for (tcount = wps->num_terms, dpp = wps->decorr_passes; tcount--; dpp++)
            if (dpp->term > 0 && dpp->term <= MAX_TERM) {
                int32_t temp_A [MAX_TERM], temp_B [MAX_TERM];
                int k;

                memcpy (temp_A, dpp->samples_A, sizeof (dpp->samples_A));
                memcpy (temp_B, dpp->samples_B, sizeof (dpp->samples_B));

                for (k = 0; k < MAX_TERM; k++) {
                    dpp->samples_A [k] = temp_A [m];
                    dpp->samples_B [k] = temp_B [m];
                    m = (m + 1) & (MAX_TERM - 1);
                }
            }

    fixup_samples (wpc, buffer, i);

    if ((flags & FLOAT_DATA) && (wpc->open_flags & OPEN_NORMALIZE))
        WavpackFloatNormalize (buffer, (flags & MONO_DATA) ? i : i * 2,
            127 - wps->float_norm_exp + wpc->norm_offset);

    if (flags & FALSE_STEREO) {
        int32_t *dptr = buffer + i * 2;
        int32_t *sptr = buffer + i;
        int32_t c = i;

        while (c--) {
            *--dptr = *--sptr;
            *--dptr = *sptr;
        }
    }

    wps->sample_index += i;
    wps->crc = crc;

    return i;
}



static void decorr_stereo_pass (struct decorr_pass *dpp, int32_t *buffer, int32_t sample_count)
{
    int32_t *bptr, *eptr = buffer + (sample_count * 2);
    int m, k;

    switch (dpp->term) {
        case 17:
            for (bptr = buffer; bptr < eptr; bptr += 2) {
                int32_t sam, tmp;

                sam = 2 * dpp->samples_A [0] - dpp->samples_A [1];
                dpp->samples_A [1] = dpp->samples_A [0];
                bptr [0] = dpp->samples_A [0] = apply_weight (dpp->weight_A, sam) + (tmp = bptr [0]);
                update_weight (dpp->weight_A, dpp->delta, sam, tmp);

                sam = 2 * dpp->samples_B [0] - dpp->samples_B [1];
                dpp->samples_B [1] = dpp->samples_B [0];
                bptr [1] = dpp->samples_B [0] = apply_weight (dpp->weight_B, sam) + (tmp = bptr [1]);
                update_weight (dpp->weight_B, dpp->delta, sam, tmp);
            }

            break;

        case 18:
            for (bptr = buffer; bptr < eptr; bptr += 2) {
                int32_t sam, tmp;

                sam = dpp->samples_A [0] + ((dpp->samples_A [0] - dpp->samples_A [1]) >> 1);
                dpp->samples_A [1] = dpp->samples_A [0];
                bptr [0] = dpp->samples_A [0] = apply_weight (dpp->weight_A, sam) + (tmp = bptr [0]);
                update_weight (dpp->weight_A, dpp->delta, sam, tmp);

                sam = dpp->samples_B [0] + ((dpp->samples_B [0] - dpp->samples_B [1]) >> 1);
                dpp->samples_B [1] = dpp->samples_B [0];
                bptr [1] = dpp->samples_B [0] = apply_weight (dpp->weight_B, sam) + (tmp = bptr [1]);
                update_weight (dpp->weight_B, dpp->delta, sam, tmp);
            }

            break;

        default:
            for (m = 0, k = dpp->term & (MAX_TERM - 1), bptr = buffer; bptr < eptr; bptr += 2) {
                int32_t sam;

                sam = dpp->samples_A [m];
                dpp->samples_A [k] = apply_weight (dpp->weight_A, sam) + bptr [0];
                update_weight (dpp->weight_A, dpp->delta, sam, bptr [0]);
                bptr [0] = dpp->samples_A [k];

                sam = dpp->samples_B [m];
                dpp->samples_B [k] = apply_weight (dpp->weight_B, sam) + bptr [1];
                update_weight (dpp->weight_B, dpp->delta, sam, bptr [1]);
                bptr [1] = dpp->samples_B [k];

                m = (m + 1) & (MAX_TERM - 1);
                k = (k + 1) & (MAX_TERM - 1);
            }

            break;

        case -1:
            for (bptr = buffer; bptr < eptr; bptr += 2) {
                int32_t sam;

                sam = bptr [0] + apply_weight (dpp->weight_A, dpp->samples_A [0]);
                update_weight_clip (dpp->weight_A, dpp->delta, dpp->samples_A [0], bptr [0]);
                bptr [0] = sam;
                dpp->samples_A [0] = bptr [1] + apply_weight (dpp->weight_B, sam);
                update_weight_clip (dpp->weight_B, dpp->delta, sam, bptr [1]);
                bptr [1] = dpp->samples_A [0];
            }

            break;

        case -2:
            for (bptr = buffer; bptr < eptr; bptr += 2) {
                int32_t sam;

                sam = bptr [1] + apply_weight (dpp->weight_B, dpp->samples_B [0]);
                update_weight_clip (dpp->weight_B, dpp->delta, dpp->samples_B [0], bptr [1]);
                bptr [1] = sam;
                dpp->samples_B [0] = bptr [0] + apply_weight (dpp->weight_A, sam);
                update_weight_clip (dpp->weight_A, dpp->delta, sam, bptr [0]);
                bptr [0] = dpp->samples_B [0];
            }

            break;

        case -3:
            for (bptr = buffer; bptr < eptr; bptr += 2) {
                int32_t sam_A, sam_B;

                sam_A = bptr [0] + apply_weight (dpp->weight_A, dpp->samples_A [0]);
                update_weight_clip (dpp->weight_A, dpp->delta, dpp->samples_A [0], bptr [0]);
                sam_B = bptr [1] + apply_weight (dpp->weight_B, dpp->samples_B [0]);
                update_weight_clip (dpp->weight_B, dpp->delta, dpp->samples_B [0], bptr [1]);
                bptr [0] = dpp->samples_B [0] = sam_A;
                bptr [1] = dpp->samples_A [0] = sam_B;
            }

            break;
    }
}


#ifdef FAST_DECODE

static void decorr_stereo_pass_i (struct decorr_pass *dpp, int32_t *buffer, int32_t sample_count)
{
    int32_t *bptr, *eptr = buffer + (sample_count * 2);
    int m, k;

    switch (dpp->term) {
        case 17:
            for (bptr = buffer; bptr < eptr; bptr += 2) {
                int32_t sam, tmp;

                sam = 2 * dpp->samples_A [0] - dpp->samples_A [1];
                dpp->samples_A [1] = dpp->samples_A [0];
                bptr [0] = dpp->samples_A [0] = apply_weight_i (dpp->weight_A, sam) + (tmp = bptr [0]);
                update_weight (dpp->weight_A, dpp->delta, sam, tmp);

                sam = 2 * dpp->samples_B [0] - dpp->samples_B [1];
                dpp->samples_B [1] = dpp->samples_B [0];
                bptr [1] = dpp->samples_B [0] = apply_weight_i (dpp->weight_B, sam) + (tmp = bptr [1]);
                update_weight (dpp->weight_B, dpp->delta, sam, tmp);
            }

            break;

        case 18:
            for (bptr = buffer; bptr < eptr; bptr += 2) {
                int32_t sam, tmp;

                sam = dpp->samples_A [0] + ((dpp->samples_A [0] - dpp->samples_A [1]) >> 1);
                dpp->samples_A [1] = dpp->samples_A [0];
                bptr [0] = dpp->samples_A [0] = apply_weight_i (dpp->weight_A, sam) + (tmp = bptr [0]);
                update_weight (dpp->weight_A, dpp->delta, sam, tmp);

                sam = dpp->samples_B [0] + ((dpp->samples_B [0] - dpp->samples_B [1]) >> 1);
                dpp->samples_B [1] = dpp->samples_B [0];
                bptr [1] = dpp->samples_B [0] = apply_weight_i (dpp->weight_B, sam) + (tmp = bptr [1]);
                update_weight (dpp->weight_B, dpp->delta, sam, tmp);
            }

            break;

        default:
            for (m = 0, k = dpp->term & (MAX_TERM - 1), bptr = buffer; bptr < eptr; bptr += 2) {
                int32_t sam;

                sam = dpp->samples_A [m];
                dpp->samples_A [k] = apply_weight_i (dpp->weight_A, sam) + bptr [0];
                update_weight (dpp->weight_A, dpp->delta, sam, bptr [0]);
                bptr [0] = dpp->samples_A [k];

                sam = dpp->samples_B [m];
                dpp->samples_B [k] = apply_weight_i (dpp->weight_B, sam) + bptr [1];
                update_weight (dpp->weight_B, dpp->delta, sam, bptr [1]);
                bptr [1] = dpp->samples_B [k];

                m = (m + 1) & (MAX_TERM - 1);
                k = (k + 1) & (MAX_TERM - 1);
            }

            break;

        case -1:
            for (bptr = buffer; bptr < eptr; bptr += 2) {
                int32_t sam;

                sam = bptr [0] + apply_weight_i (dpp->weight_A, dpp->samples_A [0]);
                update_weight_clip (dpp->weight_A, dpp->delta, dpp->samples_A [0], bptr [0]);
                bptr [0] = sam;
                dpp->samples_A [0] = bptr [1] + apply_weight_i (dpp->weight_B, sam);
                update_weight_clip (dpp->weight_B, dpp->delta, sam, bptr [1]);
                bptr [1] = dpp->samples_A [0];
            }

            break;

        case -2:
            for (bptr = buffer; bptr < eptr; bptr += 2) {
                int32_t sam;

                sam = bptr [1] + apply_weight_i (dpp->weight_B, dpp->samples_B [0]);
                update_weight_clip (dpp->weight_B, dpp->delta, dpp->samples_B [0], bptr [1]);
                bptr [1] = sam;
                dpp->samples_B [0] = bptr [0] + apply_weight_i (dpp->weight_A, sam);
                update_weight_clip (dpp->weight_A, dpp->delta, sam, bptr [0]);
                bptr [0] = dpp->samples_B [0];
            }

            break;

        case -3:
            for (bptr = buffer; bptr < eptr; bptr += 2) {
                int32_t sam_A, sam_B;

                sam_A = bptr [0] + apply_weight_i (dpp->weight_A, dpp->samples_A [0]);
                update_weight_clip (dpp->weight_A, dpp->delta, dpp->samples_A [0], bptr [0]);
                sam_B = bptr [1] + apply_weight_i (dpp->weight_B, dpp->samples_B [0]);
                update_weight_clip (dpp->weight_B, dpp->delta, dpp->samples_B [0], bptr [1]);
                bptr [0] = dpp->samples_B [0] = sam_A;
                bptr [1] = dpp->samples_A [0] = sam_B;
            }

            break;
    }
}



static void decorr_stereo_pass_1718 (struct decorr_pass *dpp, int32_t *buffer, int32_t sample_count)
{
    int32_t *bptr, *eptr = buffer + (sample_count * 2);

    for (bptr = buffer; bptr < eptr; bptr += 2) {
        int32_t sam;

        sam = 2 * dpp->samples_A [0] - dpp->samples_A [1];
        dpp->samples_A [1] = dpp->samples_A [0];
        dpp->samples_A [0] = apply_weight_i (dpp->weight_A, sam) + bptr [0];
        update_weight (dpp->weight_A, dpp->delta, sam, bptr [0]);

        sam = (dpp+1)->samples_A [0] + (((dpp+1)->samples_A [0] - (dpp+1)->samples_A [1]) >> 1);
        (dpp+1)->samples_A [1] = (dpp+1)->samples_A [0];
        bptr [0] = (dpp+1)->samples_A [0] = apply_weight_i ((dpp+1)->weight_A, sam) + dpp->samples_A [0];
        update_weight ((dpp+1)->weight_A, (dpp+1)->delta, sam, dpp->samples_A [0]);

        sam = 2 * dpp->samples_B [0] - dpp->samples_B [1];
        dpp->samples_B [1] = dpp->samples_B [0];
        dpp->samples_B [0] = apply_weight_i (dpp->weight_B, sam) + bptr [1];
        update_weight (dpp->weight_B, dpp->delta, sam, bptr [1]);

        sam = (dpp+1)->samples_B [0] + (((dpp+1)->samples_B [0] - (dpp+1)->samples_B [1]) >> 1);
        (dpp+1)->samples_B [1] = (dpp+1)->samples_B [0];
        bptr [1] = (dpp+1)->samples_B [0] = apply_weight_i ((dpp+1)->weight_B, sam) + dpp->samples_B [0];
        update_weight ((dpp+1)->weight_B, (dpp+1)->delta, sam, dpp->samples_B [0]);
    }
}

static void decorr_stereo_pass_1717 (struct decorr_pass *dpp, int32_t *buffer, int32_t sample_count)
{
    int32_t *bptr, *eptr = buffer + (sample_count * 2);

    for (bptr = buffer; bptr < eptr; bptr += 2) {
        int32_t sam;

        sam = 2 * dpp->samples_A [0] - dpp->samples_A [1];
        dpp->samples_A [1] = dpp->samples_A [0];
        dpp->samples_A [0] = apply_weight_i (dpp->weight_A, sam) + bptr [0];
        update_weight (dpp->weight_A, dpp->delta, sam, bptr [0]);

        sam = 2 * (dpp+1)->samples_A [0] - (dpp+1)->samples_A [1];
        (dpp+1)->samples_A [1] = (dpp+1)->samples_A [0];
        bptr [0] = (dpp+1)->samples_A [0] = apply_weight_i ((dpp+1)->weight_A, sam) + dpp->samples_A [0];
        update_weight ((dpp+1)->weight_A, (dpp+1)->delta, sam, dpp->samples_A [0]);

        sam = 2 * dpp->samples_B [0] - dpp->samples_B [1];
        dpp->samples_B [1] = dpp->samples_B [0];
        dpp->samples_B [0] = apply_weight_i (dpp->weight_B, sam) + bptr [1];
        update_weight (dpp->weight_B, dpp->delta, sam, bptr [1]);

        sam = 2 * (dpp+1)->samples_B [0] - (dpp+1)->samples_B [1];
        (dpp+1)->samples_B [1] = (dpp+1)->samples_B [0];
        bptr [1] = (dpp+1)->samples_B [0] = apply_weight_i ((dpp+1)->weight_B, sam) + dpp->samples_B [0];
        update_weight ((dpp+1)->weight_B, (dpp+1)->delta, sam, dpp->samples_B [0]);
    }
}

static void decorr_stereo_pass_1818 (struct decorr_pass *dpp, int32_t *buffer, int32_t sample_count)
{
    int32_t *bptr, *eptr = buffer + (sample_count * 2);

    for (bptr = buffer; bptr < eptr; bptr += 2) {
        int32_t sam;

        sam = dpp->samples_A [0] + ((dpp->samples_A [0] - dpp->samples_A [1]) >> 1);
        dpp->samples_A [1] = dpp->samples_A [0];
        dpp->samples_A [0] = apply_weight_i (dpp->weight_A, sam) + bptr [0];
        update_weight (dpp->weight_A, dpp->delta, sam, bptr [0]);

        sam = (dpp+1)->samples_A [0] + (((dpp+1)->samples_A [0] - (dpp+1)->samples_A [1]) >> 1);
        (dpp+1)->samples_A [1] = (dpp+1)->samples_A [0];
        bptr [0] = (dpp+1)->samples_A [0] = apply_weight_i ((dpp+1)->weight_A, sam) + dpp->samples_A [0];
        update_weight ((dpp+1)->weight_A, (dpp+1)->delta, sam, dpp->samples_A [0]);

        sam = dpp->samples_B [0] + ((dpp->samples_B [0] - dpp->samples_B [1]) >> 1);
        dpp->samples_B [1] = dpp->samples_B [0];
        dpp->samples_B [0] = apply_weight_i (dpp->weight_B, sam) + bptr [1];
        update_weight (dpp->weight_B, dpp->delta, sam, bptr [1]);

        sam = (dpp+1)->samples_B [0] + (((dpp+1)->samples_B [0] - (dpp+1)->samples_B [1]) >> 1);
        (dpp+1)->samples_B [1] = (dpp+1)->samples_B [0];
        bptr [1] = (dpp+1)->samples_B [0] = apply_weight_i ((dpp+1)->weight_B, sam) + dpp->samples_B [0];
        update_weight ((dpp+1)->weight_B, (dpp+1)->delta, sam, dpp->samples_B [0]);
    }
}

static void decorr_stereo_pass_nn (struct decorr_pass *dpp, int32_t *buffer, int32_t sample_count)
{
    int32_t *bptr, *eptr = buffer + (sample_count * 2);
    int m, k, j;

    m = 0;
    k = dpp->term & (MAX_TERM - 1);
    j = (dpp+1)->term & (MAX_TERM - 1);

    for (bptr = buffer; bptr < eptr; bptr += 2) {
        int32_t tmp;

        dpp->samples_A [k] = apply_weight_i (dpp->weight_A, dpp->samples_A [m]) + (tmp = bptr [0]);
        update_weight (dpp->weight_A, dpp->delta, dpp->samples_A [m], tmp);

        bptr [0] = (dpp+1)->samples_A [j] = apply_weight_i ((dpp+1)->weight_A, (dpp+1)->samples_A [m]) + (tmp = dpp->samples_A [k]);
        update_weight ((dpp+1)->weight_A, (dpp+1)->delta, (dpp+1)->samples_A [m], tmp);

        dpp->samples_B [k] = apply_weight_i (dpp->weight_B, dpp->samples_B [m]) + (tmp = bptr [1]);
        update_weight (dpp->weight_B, dpp->delta, dpp->samples_B [m], tmp);

        bptr [1] = (dpp+1)->samples_B [j] = apply_weight_i ((dpp+1)->weight_B, (dpp+1)->samples_B [m]) + (tmp = dpp->samples_B [k]);
        update_weight ((dpp+1)->weight_B, (dpp+1)->delta, (dpp+1)->samples_B [m], tmp);

        m = (m + 1) & (MAX_TERM - 1);
        k = (k + 1) & (MAX_TERM - 1);
        j = (j + 1) & (MAX_TERM - 1);
    }
}

#endif



static void fixup_samples (WavpackContext *wpc, int32_t *buffer, uint32_t sample_count)
{
    WavpackStream *wps = wpc->streams [wpc->current_stream];
    uint32_t flags = wps->wphdr.flags;
    int lossy_flag = (flags & HYBRID_FLAG) && !wps->block2buff;
    int shift = (flags & SHIFT_MASK) >> SHIFT_LSB;

    if (flags & FLOAT_DATA) {
        float_values (wps, buffer, (flags & MONO_DATA) ? sample_count : sample_count * 2);
        return;
    }

    if (flags & INT32_DATA) {
        uint32_t count = (flags & MONO_DATA) ? sample_count : sample_count * 2;
        int sent_bits = wps->int32_sent_bits, zeros = wps->int32_zeros;
        int ones = wps->int32_ones, dups = wps->int32_dups;
        uint32_t data, mask = (1 << sent_bits) - 1;
        int32_t *dptr = buffer;

        if (bs_is_open (&wps->wvxbits)) {
            uint32_t crc = wps->crc_x;

            while (count--) {
//              if (sent_bits) {
                    getbits (&data, sent_bits, &wps->wvxbits);
                    *dptr = (*dptr << sent_bits) | (data & mask);
//              }

                if (zeros)
                    *dptr <<= zeros;
                else if (ones)
                    *dptr = ((*dptr + 1) << ones) - 1;
                else if (dups)
                    *dptr = ((*dptr + (*dptr & 1)) << dups) - (*dptr & 1);

                crc = crc * 9 + (*dptr & 0xffff) * 3 + ((*dptr >> 16) & 0xffff);
                dptr++;
            }

            wps->crc_x = crc;
        }
        else if (!sent_bits && (zeros + ones + dups)) {
            while (lossy_flag && (flags & BYTES_STORED) == 3 && shift < 8) {
                if (zeros)
                    zeros--;
                else if (ones)
                    ones--;
                else if (dups)
                    dups--;
                else
                    break;

                shift++;
            }

            while (count--) {
                if (zeros)
                    *dptr <<= zeros;
                else if (ones)
                    *dptr = ((*dptr + 1) << ones) - 1;
                else if (dups)
                    *dptr = ((*dptr + (*dptr & 1)) << dups) - (*dptr & 1);

                dptr++;
            }
        }
        else
            shift += zeros + sent_bits + ones + dups;
    }

    if (lossy_flag) {
        int32_t min_value, max_value, min_shifted, max_shifted;

        switch (flags & BYTES_STORED) {
            case 0:
                min_shifted = (min_value = -128 >> shift) << shift;
                max_shifted = (max_value = 127 >> shift) << shift;
                break;

            case 1:
                min_shifted = (min_value = -32768 >> shift) << shift;
                max_shifted = (max_value = 32767 >> shift) << shift;
                break;

            case 2:
                min_shifted = (min_value = -8388608 >> shift) << shift;
                max_shifted = (max_value = 8388607 >> shift) << shift;
                break;

            case 3: default:    /* "default" suppresses compiler warning */
                min_shifted = (min_value = (int32_t) 0x80000000 >> shift) << shift;
                max_shifted = (max_value = (int32_t) 0x7fffffff >> shift) << shift;
                break;
        }

        if (!(flags & MONO_DATA))
            sample_count *= 2;

        while (sample_count--) {
            if (*buffer < min_value)
                *buffer++ = min_shifted;
            else if (*buffer > max_value)
                *buffer++ = max_shifted;
            else
                *buffer++ <<= shift;
        }
    }
    else if (shift) {
        if (!(flags & MONO_DATA))
            sample_count *= 2;

        while (sample_count--)
            *buffer++ <<= shift;
    }
}


int check_crc_error (WavpackContext *wpc)
{
    int result = 0, stream;

    for (stream = 0; stream < wpc->num_streams; stream++) {
        WavpackStream *wps = wpc->streams [stream];

        if (wps->crc != wps->wphdr.crc)
            ++result;
        else if (bs_is_open (&wps->wvxbits) && wps->crc_x != wps->crc_wvx)
            ++result;
    }

    return result;
}

#endif


