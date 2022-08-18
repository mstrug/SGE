/*
 * wputilsS.cpp
 *
 *  Created on: 2009-09-18
 *      Author: Michal Strug
 *   Copyright: Copyright (c) 2022 Michal Strug. All rights reserved.
 */

#include "wavpack_local.h"
#include "wavpack_version.h"


//#define stricmp(x,y) strcasecmp(x,y)


static void free_streams (WavpackContext *wpc);



static const uint32_t sample_rates [] = { 6000, 8000, 9600, 11025, 12000, 16000, 22050,
    24000, 32000, 44100, 48000, 64000, 88200, 96000, 192000 };




#ifndef NO_TAGS
static int load_tag (WavpackContext *wpc);
static int valid_tag (M_Tag *m_tag);
static void free_tag (M_Tag *m_tag);
#endif




#if !defined(NO_UNPACK) || defined(INFO_ONLY)

static uint32_t read_next_header (WavpackStreamReader *reader, void *id, WavpackHeader *wphdr);
static uint32_t seek_final_index (WavpackStreamReader *reader, void *id);
static int read_wvc_block (WavpackContext *wpc);



#ifndef NO_USE_FSTREAMS

static int32_t read_bytes (void *id, void *data, int32_t bcount)
{
//    return (int32_t) fread (data, 1, bcount, (FILE*) id);
	// FILE CHANGED
	RFile *file = (RFile*) id;
	TPtr8 ptr( (uchar*)data, bcount );
	return ( file->Read( ptr, (TInt) bcount ) == KErrNone ? bcount : 0 );
}

static uint32_t get_pos (void *id)
{
	//return ftell ((FILE*) id);
	// FILE CHANGED
	RFile *file = (RFile*) id;
	TInt pos = 0;
	file->Seek( ESeekAddress, pos );
	return pos;
}

static int set_pos_abs (void *id, uint32_t pos)
{
//    return fseek (id, pos, SEEK_SET);
	// FILE CHANGED
	RFile *file = (RFile*) id;
	TInt pos2 = pos;
	return file->Seek( ESeekStart, pos2 );
}

static int set_pos_rel (void *id, int32_t delta, int mode)
{
//    return fseek (id, delta, mode);
	// FILE CHANGED
	RFile *file = (RFile*) id;
	TInt pos2 = delta;
	return file->Seek( (TSeek)mode, pos2 );
}

static int push_back_byte (void *id, int c)
{
//    return ungetc (c, id);
	// FILE CHANGED
	RFile *file = (RFile*) id;
	TBuf8<1> buf;
	buf.Append( TChar(c) );
	file->Write( buf );		// TODO: ungetc returns EOF in case of error
	return c;
}

static uint32_t get_length (void *id)
{
//    FILE *file = id;
//    struct stat statbuf;
//
//    if (!file || fstat (fileno (file), &statbuf) || !(statbuf.st_mode & S_IFREG))
//        return 0;
//
//    return statbuf.st_size;
	// FILE CHANGED
	RFile *file = (RFile*) id;
	TInt size = 0;
	file->Size( size );
	return size;
}

static int can_seek (void *id)
{
//    FILE *file = id;
//    struct stat statbuf;
//
//    return file && !fstat (fileno (file), &statbuf) && (statbuf.st_mode & S_IFREG);
	
	// FILE CHANGED
	RFile *file = (RFile*) id;
	TInt size = 0;
	file->Size( size );
	TInt pos;
	file->Seek( ESeekAddress, pos );
	return ( pos < size );
	
}

static int32_t write_bytes (void *id, void *data, int32_t bcount)
{
//    return (int32_t) fwrite (data, 1, bcount, (FILE*) id);

	// FILE CHANGED
	RFile *file = (RFile*) id;

	TPtr8 ptr( (uchar*)data, bcount );
	return ( file->Write( ptr, bcount ) == KErrNone ? bcount : 0 );
}

static WavpackStreamReader freader = {
    read_bytes, get_pos, set_pos_abs, set_pos_rel, push_back_byte, get_length, can_seek,
    write_bytes
};


WavpackContext* WavpackOpenFileInput( RFs aFs, const TDesC& aInFilename, TInt aFlags, int norm_offset)
{
 //   char *file_mode = (char*) ( (flags & OPEN_EDIT_TAGS) ? "r+b" : "rb" );
    RFile *wv_id = new RFile, *wvc_id = NULL;
    WavpackContext *wpc;

//    if (*infilename == '-') {
//        wv_id = stdin;
//#if defined(WIN32)
//        _setmode (fileno (stdin), O_BINARY);
//#endif
//    }
//    else 
//    	if ((wv_id = fopen (infilename, file_mode)) == NULL) {
//        strcpy (error, (flags & OPEN_EDIT_TAGS) ? "can't open file for editing" : "can't open file");
//        return NULL;
//    }
 
    TInt err = wv_id->Open( aFs, aInFilename, EFileRead );
    if ( err != KErrNone )
    	{
    	return NULL;
    	}
    
//    if (wv_id != stdin && (flags & OPEN_WVC)) 
//    {
//        char *in2filename = malloc (strlen (infilename) + 10);
//
//        strcpy (in2filename, infilename);
//        strcat (in2filename, "c");
//        wvc_id = fopen (in2filename, "rb");
//        free (in2filename);
//    }
//    else
//        wvc_id = NULL;

    wpc = WavpackOpenFileInputEx ( &freader, wv_id, NULL, NULL, aFlags, norm_offset);
//    wpc = WavpackOpenFileInputEx ( &freader, wv_id, wvc_id, error, flags, norm_offset);

    if (!wpc) 
    	{
		wv_id->Close();
		delete wv_id;
//            fclose (wv_id);

//        if (wvc_id)
//            fclose (wvc_id);
    }
    else
        wpc->close_files = TRUE;

    return wpc;
}


#endif


WavpackContext *WavpackOpenFileInputEx (WavpackStreamReader *reader, void* wv_id, void*, char*, int flags, int norm_offset)
{
    WavpackContext *wpc = new WavpackContext; 
    // malloc (sizeof (WavpackContext));
    WavpackStream *wps;
    int num_blocks = 0;
    //uchar first_byte;
    uint32_t bcount;

    if (!wpc) 
	{
//        strcpy (error, "can't allocate memory");
        return NULL;
    }

    CLEAR (*wpc);
    wpc->wv_in = wv_id;
//    wpc->wvc_in = wvc_id;
    wpc->reader = reader;
    wpc->total_samples = (uint32_t) -1;
    wpc->norm_offset = norm_offset;
    wpc->open_flags = flags;

    wpc->filelen = wpc->reader->get_length (wpc->wv_in);

#ifndef NO_TAGS
    if ((flags & (OPEN_TAGS | OPEN_EDIT_TAGS)) && wpc->reader->can_seek (wpc->wv_in)) {
        load_tag (wpc);
        wpc->reader->set_pos_abs (wpc->wv_in, 0);
    }
#endif

#ifndef VER4_ONLY
    if (wpc->reader->read_bytes (wpc->wv_in, &first_byte, 1) != 1) {
//        strcpy (error, "can't read all of WavPack file!");
        return WavpackCloseFile (wpc);
    }

    wpc->reader->push_back_byte (wpc->wv_in, first_byte);

//    if (first_byte == 'R')
//        return open_file3 (wpc, error);
#endif

//    wpc->streams [0] = wps = malloc (sizeof (WavpackStream));
    wpc->streams [0] = wps = new WavpackStream;
    wpc->num_streams = 1;
    CLEAR (*wps);

    while (!wps->wphdr.block_samples) {

        wpc->filepos = wpc->reader->get_pos (wpc->wv_in);
        bcount = read_next_header (wpc->reader, wpc->wv_in, &wps->wphdr);

        if (bcount == (uint32_t) -1 ||
            (!wps->wphdr.block_samples && num_blocks++ > 16)) {
//                strcpy (error, "not compatible with this version of WavPack file!");
                return WavpackCloseFile (wpc);
        }

        wpc->filepos += bcount;
        wps->blockbuff = new TUint8[wps->wphdr.ckSize + 8 ];
//        wps->blockbuff = malloc (wps->wphdr.ckSize + 8);
        memcpy (wps->blockbuff, &wps->wphdr, 32);

        if (wpc->reader->read_bytes (wpc->wv_in, wps->blockbuff + 32, wps->wphdr.ckSize - 24) != wps->wphdr.ckSize - 24) {
//            strcpy (error, "can't read all of WavPack file!");
            return WavpackCloseFile (wpc);
        }

        wps->init_done = FALSE;

        if (wps->wphdr.block_samples && !(flags & OPEN_STREAMING)) {
            if (wps->wphdr.block_index || wps->wphdr.total_samples == (uint32_t) -1) {
                wpc->initial_index = wps->wphdr.block_index;
                wps->wphdr.block_index = 0;

                if (wpc->reader->can_seek (wpc->wv_in)) {
                    uint32_t pos_save = wpc->reader->get_pos (wpc->wv_in);
                    uint32_t final_index = seek_final_index (wpc->reader, wpc->wv_in);

                    if (final_index != (uint32_t) -1)
                        wpc->total_samples = final_index - wpc->initial_index;

                    wpc->reader->set_pos_abs (wpc->wv_in, pos_save);
                }
            }
            else
                wpc->total_samples = wps->wphdr.total_samples;
        }

        if (wpc->wvc_in && wps->wphdr.block_samples && (wps->wphdr.flags & HYBRID_FLAG)) {
            wpc->file2len = wpc->reader->get_length (wpc->wvc_in);
            wpc->wvc_flag = TRUE;
        }

        if (wpc->wvc_flag && !read_wvc_block (wpc)) {
//            strcpy (error, "not compatible with this version of correction file!");
            return WavpackCloseFile (wpc);
        }

        if (!wps->init_done && !unpack_init (wpc)) {
//            strcpy (error, wpc->error_message [0] ? wpc->error_message :
//                "not compatible with this version of WavPack file!");

            return WavpackCloseFile (wpc);
        }

        wps->init_done = TRUE;
    }

    wpc->config.flags &= ~0xff;
    wpc->config.flags |= wps->wphdr.flags & 0xff;
    wpc->config.bytes_per_sample = (wps->wphdr.flags & BYTES_STORED) + 1;
    wpc->config.float_norm_exp = wps->float_norm_exp;

    wpc->config.bits_per_sample = (wpc->config.bytes_per_sample * 8) -
        ((wps->wphdr.flags & SHIFT_MASK) >> SHIFT_LSB);

    if (!wpc->config.sample_rate) {
        if (!wps || !wps->wphdr.block_samples || (wps->wphdr.flags & SRATE_MASK) == SRATE_MASK)
            wpc->config.sample_rate = 44100;
        else
            wpc->config.sample_rate = sample_rates [(wps->wphdr.flags & SRATE_MASK) >> SRATE_LSB];
    }

    if (!wpc->config.num_channels) {
        wpc->config.num_channels = (wps->wphdr.flags & MONO_FLAG) ? 1 : 2;
        wpc->config.channel_mask = 0x5 - wpc->config.num_channels;
    }

    if ((flags & OPEN_2CH_MAX) && !(wps->wphdr.flags & FINAL_BLOCK))
        wpc->reduced_channels = (wps->wphdr.flags & MONO_FLAG) ? 1 : 2;

    return wpc;
}


int WavpackGetMode (WavpackContext *wpc)
{
    int mode = 0;

    if (wpc) {
        if (wpc->config.flags & CONFIG_HYBRID_FLAG)
            mode |= MODE_HYBRID;
        else if (!(wpc->config.flags & CONFIG_LOSSY_MODE))
            mode |= MODE_LOSSLESS;

        if (wpc->wvc_flag)
            mode |= (MODE_LOSSLESS | MODE_WVC);

        if (wpc->lossy_blocks)
            mode &= ~MODE_LOSSLESS;

        if (wpc->config.flags & CONFIG_FLOAT_DATA)
            mode |= MODE_FLOAT;

        if (wpc->config.flags & (CONFIG_HIGH_FLAG | CONFIG_VERY_HIGH_FLAG)) {
            mode |= MODE_HIGH;

            if ((wpc->config.flags & CONFIG_VERY_HIGH_FLAG) ||
                (wpc->streams [0] && wpc->streams [0]->wphdr.version < 0x405))
                    mode |= MODE_VERY_HIGH;
        }

        if (wpc->config.flags & CONFIG_FAST_FLAG)
            mode |= MODE_FAST;

        if (wpc->config.flags & CONFIG_EXTRA_MODE)
            mode |= (MODE_EXTRA | (wpc->config.xmode << 12));

        if (wpc->config.flags & CONFIG_CREATE_EXE)
            mode |= MODE_SFX;

        if (wpc->config.flags & CONFIG_MD5_CHECKSUM)
            mode |= MODE_MD5;

        if ((wpc->config.flags & CONFIG_HYBRID_FLAG) && (wpc->config.flags & CONFIG_DYNAMIC_SHAPING) &&
            wpc->streams [0] && wpc->streams [0]->wphdr.version >= 0x407)
                mode |= MODE_DNS;

#ifndef NO_TAGS
        if (valid_tag (&wpc->m_tag)) {
            mode |= MODE_VALID_TAG;

            if (valid_tag (&wpc->m_tag) == 'A')
                mode |= MODE_APETAG;
        }
#endif
    }

    return mode;
}



int WavpackGetVersion (WavpackContext *wpc)
{
    if (wpc) {
#ifndef VER4_ONLY
        if (wpc->stream3)
            return get_version3 (wpc);
#endif
        return 4;
    }

    return 0;
}

#endif



char *WavpackGetErrorMessage (WavpackContext *wpc)
{
    return wpc->error_message;
}


#ifndef NO_UNPACK




uint32_t WavpackUnpackSamples (WavpackContext *wpc, int32_t *buffer, uint32_t samples)
{
    WavpackStream *wps = wpc->streams [wpc->current_stream = 0];
    uint32_t bcount, samples_unpacked = 0, samples_to_unpack;
    int num_channels = wpc->config.num_channels;
    int file_done = FALSE;

#ifndef VER4_ONLY
    if (wpc->stream3)
        return unpack_samples3 (wpc, buffer, samples);
#endif

    while (samples) {
        if (!wps->wphdr.block_samples || !(wps->wphdr.flags & INITIAL_BLOCK) ||
            wps->sample_index >= wps->wphdr.block_index + wps->wphdr.block_samples) {

                if (wpc->wrapper_bytes >= MAX_WRAPPER_BYTES)
                    break;

                free_streams (wpc);
                wpc->filepos = wpc->reader->get_pos (wpc->wv_in);
                bcount = read_next_header (wpc->reader, wpc->wv_in, &wps->wphdr);

                if (bcount == (uint32_t) -1)
                    break;

                if (wpc->open_flags & OPEN_STREAMING)
                    wps->wphdr.block_index = wps->sample_index = 0;
                else
                    wps->wphdr.block_index -= wpc->initial_index;

                wpc->filepos += bcount;
                wps->blockbuff = new TUint8[ wps->wphdr.ckSize + 8 ];
//                wps->blockbuff = malloc (wps->wphdr.ckSize + 8);
                memcpy (wps->blockbuff, &wps->wphdr, 32);

                if (wpc->reader->read_bytes (wpc->wv_in, wps->blockbuff + 32, wps->wphdr.ckSize - 24) !=
                    wps->wphdr.ckSize - 24) {
                        strcpy (wpc->error_message, "can't read all of last block!");
                        wps->wphdr.block_samples = 0;
                        wps->wphdr.ckSize = 24;
                        break;
                }

                wps->init_done = FALSE;

                if (wps->wphdr.block_samples && wps->sample_index != wps->wphdr.block_index)
                    wpc->crc_errors++;

                if (wps->wphdr.block_samples && wpc->wvc_flag)
                    read_wvc_block (wpc);

                if (!wps->wphdr.block_samples) {
                    if (!wps->init_done && !unpack_init (wpc))
                        wpc->crc_errors++;

                    wps->init_done = TRUE;
                }
        }

        if (!wps->wphdr.block_samples || !(wps->wphdr.flags & INITIAL_BLOCK) ||
            wps->sample_index >= wps->wphdr.block_index + wps->wphdr.block_samples)
                continue;

        if (wps->sample_index < wps->wphdr.block_index) {
            samples_to_unpack = wps->wphdr.block_index - wps->sample_index;

            if (samples_to_unpack > 262144) {
                strcpy (wpc->error_message, "discontinuity found, aborting file!");
                wps->wphdr.block_samples = 0;
                wps->wphdr.ckSize = 24;
                break;
            }

            if (samples_to_unpack > samples)
                samples_to_unpack = samples;

            wps->sample_index += samples_to_unpack;
            samples_unpacked += samples_to_unpack;
            samples -= samples_to_unpack;

            if (wpc->reduced_channels)
                samples_to_unpack *= wpc->reduced_channels;
            else
                samples_to_unpack *= num_channels;

            while (samples_to_unpack--)
                *buffer++ = 0;

            continue;
        }

        samples_to_unpack = wps->wphdr.block_index + wps->wphdr.block_samples - wps->sample_index;

        if (samples_to_unpack > samples)
            samples_to_unpack = samples;

        if (!wps->init_done && !unpack_init (wpc))
            wpc->crc_errors++;

        wps->init_done = TRUE;

        if (!wpc->reduced_channels && !(wps->wphdr.flags & FINAL_BLOCK)) 
        	{
        	int32_t *temp_buffer = (int32_t*)new TUint8[ samples_to_unpack * 8 ], *src, *dst;
//        	int32_t *temp_buffer = malloc (samples_to_unpack * 8), *src, *dst;
            int offset = 0;
            uint32_t samcnt;

            while (1) {
                if (wpc->current_stream == wpc->num_streams) 
                	{
                    wps = wpc->streams [wpc->num_streams++] = new WavpackStream;
//                    wps = wpc->streams [wpc->num_streams++] = malloc (sizeof (WavpackStream));
                    CLEAR (*wps);
                    bcount = read_next_header (wpc->reader, wpc->wv_in, &wps->wphdr);

                    if (bcount == (uint32_t) -1) {
                        wpc->streams [0]->wphdr.block_samples = 0;
                        wpc->streams [0]->wphdr.ckSize = 24;
                        file_done = TRUE;
                        break;
                    }

                    if (wpc->open_flags & OPEN_STREAMING)
                        wps->wphdr.block_index = wps->sample_index = 0;
                    else
                        wps->wphdr.block_index -= wpc->initial_index;

                    wps->blockbuff = malloc (wps->wphdr.ckSize + 8);
//                    wps->blockbuff = malloc (wps->wphdr.ckSize + 8);
                    memcpy (wps->blockbuff, &wps->wphdr, 32);

                    if (wpc->reader->read_bytes (wpc->wv_in, wps->blockbuff + 32, wps->wphdr.ckSize - 24) !=
                        wps->wphdr.ckSize - 24) {
                            wpc->streams [0]->wphdr.block_samples = 0;
                            wpc->streams [0]->wphdr.ckSize = 24;
                            file_done = TRUE;
                            break;
                    }

                    wps->init_done = FALSE;

                    if (wpc->wvc_flag)
                        read_wvc_block (wpc);

                    if (!wps->init_done && !unpack_init (wpc))
                        wpc->crc_errors++;

                    wps->init_done = TRUE;
                }
                else
                    wps = wpc->streams [wpc->current_stream];

                unpack_samples (wpc, src = temp_buffer, samples_to_unpack);
                samcnt = samples_to_unpack;
                dst = buffer + offset;

                if (wps->wphdr.flags & MONO_FLAG) {
                    while (samcnt--) {
                        dst [0] = *src++;
                        dst += num_channels;
                    }

                    offset++;
                }
                else if (offset == num_channels - 1) {
                    while (samcnt--) {
                        dst [0] = src [0];
                        dst += num_channels;
                        src += 2;
                    }

                    wpc->crc_errors++;
                    offset++;
                }
                else {
                    while (samcnt--) {
                        dst [0] = *src++;
                        dst [1] = *src++;
                        dst += num_channels;
                    }

                    offset += 2;
                }

                if ((wps->wphdr.flags & FINAL_BLOCK) || wpc->num_streams == MAX_STREAMS || offset == num_channels)
                    break;
                else
                    wpc->current_stream++;
            }

            wps = wpc->streams [wpc->current_stream = 0];
            free (temp_buffer);
        }
        else
            unpack_samples (wpc, buffer, samples_to_unpack);

        if (!(wps->wphdr.flags & FINAL_BLOCK) && wpc->num_streams == MAX_STREAMS) {
            strcpy (wpc->error_message, "too many channels!");
            break;
        }

        if (file_done) {
            strcpy (wpc->error_message, "can't read all of last block!");
            break;
        }

        if (wpc->reduced_channels)
            buffer += samples_to_unpack * wpc->reduced_channels;
        else
            buffer += samples_to_unpack * num_channels;

        samples_unpacked += samples_to_unpack;
        samples -= samples_to_unpack;

        if (wps->sample_index == wps->wphdr.block_index + wps->wphdr.block_samples) {
            if (check_crc_error (wpc) && wps->blockbuff) {

                if (wpc->reader->can_seek (wpc->wv_in)) {
                    int32_t rseek = ((WavpackHeader *) wps->blockbuff)->ckSize / 3;
                    wpc->reader->set_pos_rel (wpc->wv_in, (rseek > 16384) ? -16384 : -rseek, SEEK_CUR);
                }

                if (wpc->wvc_flag && wps->block2buff && wpc->reader->can_seek (wpc->wvc_in)) {
                    int32_t rseek = ((WavpackHeader *) wps->block2buff)->ckSize / 3;
                    wpc->reader->set_pos_rel (wpc->wvc_in, (rseek > 16384) ? -16384 : -rseek, SEEK_CUR);
                }

                wpc->crc_errors++;
            }
        }

        if (wpc->total_samples != (uint32_t) -1 && wps->sample_index == wpc->total_samples)
            break;
    }

    return samples_unpacked;
}



#ifndef NO_SEEKING




static uint32_t find_sample (WavpackContext *wpc, void *infile, uint32_t header_pos, uint32_t sample);



int WavpackSeekSample (WavpackContext *wpc, uint32_t sample)
{
    WavpackStream *wps = wpc->streams [wpc->current_stream = 0];
    uint32_t bcount, samples_to_skip;
    int32_t *buffer;

    if (wpc->total_samples == (uint32_t) -1 || sample >= wpc->total_samples ||
        !wpc->reader->can_seek (wpc->wv_in) || (wpc->open_flags & OPEN_STREAMING) ||
        (wpc->wvc_flag && !wpc->reader->can_seek (wpc->wvc_in)))
            return FALSE;

#ifndef VER4_ONLY
    if (wpc->stream3)
        return seek_sample3 (wpc, sample);
#endif

    if (!wps->wphdr.block_samples || !(wps->wphdr.flags & INITIAL_BLOCK) || sample < wps->wphdr.block_index ||
        sample >= wps->wphdr.block_index + wps->wphdr.block_samples) {

            free_streams (wpc);
            wpc->filepos = find_sample (wpc, wpc->wv_in, wpc->filepos, sample);

            if (wpc->filepos == (uint32_t) -1)
                return FALSE;

            if (wpc->wvc_flag) {
                wpc->file2pos = find_sample (wpc, wpc->wvc_in, 0, sample);

                if (wpc->file2pos == (uint32_t) -1)
                    return FALSE;
            }
    }

    if (!wps->blockbuff) {
        wpc->reader->set_pos_abs (wpc->wv_in, wpc->filepos);
        wpc->reader->read_bytes (wpc->wv_in, &wps->wphdr, sizeof (WavpackHeader));
        little_endian_to_native (&wps->wphdr, WavpackHeaderFormat);
        wps->wphdr.block_index -= wpc->initial_index;
        wps->blockbuff = malloc (wps->wphdr.ckSize + 8);
        memcpy (wps->blockbuff, &wps->wphdr, sizeof (WavpackHeader));

        if (wpc->reader->read_bytes (wpc->wv_in, wps->blockbuff + sizeof (WavpackHeader), wps->wphdr.ckSize - 24) !=
            wps->wphdr.ckSize - 24) {
                free_streams (wpc);
                return FALSE;
        }

        wps->init_done = FALSE;

        if (wpc->wvc_flag) {
            wpc->reader->set_pos_abs (wpc->wvc_in, wpc->file2pos);
            wpc->reader->read_bytes (wpc->wvc_in, &wps->wphdr, sizeof (WavpackHeader));
            little_endian_to_native (&wps->wphdr, WavpackHeaderFormat);
            wps->wphdr.block_index -= wpc->initial_index;
            wps->block2buff = malloc (wps->wphdr.ckSize + 8);
            memcpy (wps->block2buff, &wps->wphdr, sizeof (WavpackHeader));

            if (wpc->reader->read_bytes (wpc->wvc_in, wps->block2buff + sizeof (WavpackHeader), wps->wphdr.ckSize - 24) !=
                wps->wphdr.ckSize - 24) {
                    free_streams (wpc);
                    return FALSE;
            }
        }

        if (!wps->init_done && !unpack_init (wpc)) {
            free_streams (wpc);
            return FALSE;
        }

        wps->init_done = TRUE;
    }

    while (!wpc->reduced_channels && !(wps->wphdr.flags & FINAL_BLOCK)) {
        if (++wpc->current_stream == wpc->num_streams) {

            if (wpc->num_streams == MAX_STREAMS) {
                free_streams (wpc);
                return FALSE;
            }

            wps = wpc->streams [wpc->num_streams++] = new WavpackStream;
//            wps = wpc->streams [wpc->num_streams++] = malloc (sizeof (WavpackStream));
            CLEAR (*wps);
            bcount = read_next_header (wpc->reader, wpc->wv_in, &wps->wphdr);

            if (bcount == (uint32_t) -1) {
                free_streams (wpc);
                return FALSE;
            }

            wps->blockbuff = malloc (wps->wphdr.ckSize + 8);
            memcpy (wps->blockbuff, &wps->wphdr, 32);

            if (wpc->reader->read_bytes (wpc->wv_in, wps->blockbuff + 32, wps->wphdr.ckSize - 24) !=
                wps->wphdr.ckSize - 24) {
                    free_streams (wpc);
                    return FALSE;
            }

            wps->init_done = FALSE;

            if (wpc->wvc_flag && !read_wvc_block (wpc)) {
                free_streams (wpc);
                return FALSE;
            }

            if (!wps->init_done && !unpack_init (wpc)) {
                free_streams (wpc);
                return FALSE;
            }

            wps->init_done = TRUE;
        }
        else
            wps = wpc->streams [wpc->current_stream];
    }

    if (sample < wps->sample_index) {
        for (wpc->current_stream = 0; wpc->current_stream < wpc->num_streams; wpc->current_stream++)
            if (!unpack_init (wpc))
                return FALSE;
            else
                wpc->streams [wpc->current_stream]->init_done = TRUE;
    }

    samples_to_skip = sample - wps->sample_index;

    if (samples_to_skip > 131072) {
        free_streams (wpc);
        return FALSE;
    }

    if (samples_to_skip) {
        buffer = (long*)malloc (samples_to_skip * 8);

        for (wpc->current_stream = 0; wpc->current_stream < wpc->num_streams; wpc->current_stream++)
            unpack_samples (wpc, buffer, samples_to_skip);

        free (buffer);
    }

    wpc->current_stream = 0;
    return TRUE;
}

#endif


#ifndef NO_TAGS



int WavpackGetNumTagItems (WavpackContext *wpc)
{
    int i = 0;

    while (WavpackGetTagItemIndexed (wpc, i, NULL, 0))
        ++i;

    return i;
}



static int get_ape_tag_item (M_Tag *m_tag, const char *item, char *value, int size);
static int get_id3_tag_item (M_Tag *m_tag, const char *item, char *value, int size);

int WavpackGetTagItem (WavpackContext *wpc, const char *item, char *value, int size)
{
    M_Tag *m_tag = &wpc->m_tag;

    if (value && size)
        *value = 0;

    if (m_tag->ape_tag_hdr.ID [0] == 'A')
        return get_ape_tag_item (m_tag, item, value, size);
    else if (m_tag->id3_tag.tag_id [0] == 'T')
        return get_id3_tag_item (m_tag, item, value, size);
    else
        return 0;
}

static int get_ape_tag_item (M_Tag *m_tag, const char *item, char *value, int size)
{
    char *p = m_tag->ape_tag_data;
    char *q = p + m_tag->ape_tag_hdr.length - sizeof (APE_Tag_Hdr);
    int i;

    for (i = 0; i < m_tag->ape_tag_hdr.item_count && q - p > 8; ++i) {
        int vsize, flags, isize;

        vsize = * (int32_t *) p; p += 4;
        flags = * (int32_t *) p; p += 4;
        isize = (int) strnlen ( (TUint8*)p, q - p);

        little_endian_to_native (&vsize, "L");
        little_endian_to_native (&flags, "L");

        if (vsize < 0 || vsize > m_tag->ape_tag_hdr.length || p + isize + vsize + 1 > q)
            break;

        if (isize && vsize && !stricmp ( (TUint8*)item, (TUint8*)p) && !(flags & 6)) {

            if (!value || !size)
                return vsize;

            if (vsize < size) {
                memcpy (value, p + isize + 1, vsize);
                value [vsize] = 0;
                return vsize;
            }
            else if (size >= 4) {
                memcpy (value, p + isize + 1, size - 1);
                value [size - 4] = value [size - 3] = value [size - 2] = '.';
                value [size - 1] = 0;
                return size - 1;
            }
            else
                return 0;
        }
        else
            p += isize + vsize + 1;
    }

    return 0;
}

static void tagcpy (char *dest, char *src, int tag_size);

static int get_id3_tag_item (M_Tag *m_tag, const char *item, char *value, int size)
{
    char lvalue [64];
    int len;

    lvalue [0] = 0;

//    if (!stricmp (item, "title"))
//        tagcpy (lvalue, m_tag->id3_tag.title, sizeof (m_tag->id3_tag.title));
//    else if (!stricmp (item, "artist"))
//        tagcpy (lvalue, m_tag->id3_tag.artist, sizeof (m_tag->id3_tag.artist));
//    else if (!stricmp (item, "album"))
//        tagcpy (lvalue, m_tag->id3_tag.album, sizeof (m_tag->id3_tag.album));
//    else if (!stricmp (item, "year"))
//        tagcpy (lvalue, m_tag->id3_tag.year, sizeof (m_tag->id3_tag.year));
//    else if (!stricmp (item, "comment"))
//        tagcpy (lvalue, m_tag->id3_tag.comment, sizeof (m_tag->id3_tag.comment));
//    else if (!stricmp (item, "track") && m_tag->id3_tag.comment [29] && !m_tag->id3_tag.comment [28])
//        sprintf (lvalue, "%d", m_tag->id3_tag.comment [29]);
//    else
//        return 0;
//
//    len = (int) strlen (lvalue);
//
//    if (!value || !size)
//        return len;
//
//    if (len < size) {
//        strcpy (value, lvalue);
//        return len;
//    }
//    else if (size >= 4) {
//        strncpy (value, lvalue, size - 1);
//        value [size - 4] = value [size - 3] = value [size - 2] = '.';
//        value [size - 1] = 0;
//        return size - 1;
//    }
//    else
        return 0;
}




static int get_ape_tag_item_indexed (M_Tag *m_tag, int index, char *item, int size);
static int get_id3_tag_item_indexed (M_Tag *m_tag, int index, char *item, int size);

int WavpackGetTagItemIndexed (WavpackContext *wpc, int index, char *item, int size)
{
    M_Tag *m_tag = &wpc->m_tag;

    if (item && size)
        *item = 0;

    if (m_tag->ape_tag_hdr.ID [0] == 'A')
        return get_ape_tag_item_indexed (m_tag, index, item, size);
    else if (m_tag->id3_tag.tag_id [0] == 'T')
        return get_id3_tag_item_indexed (m_tag, index, item, size);
    else
        return 0;
}

static int get_ape_tag_item_indexed (M_Tag *m_tag, int index, char *item, int size)
{
    char *p = m_tag->ape_tag_data;
    char *q = p + m_tag->ape_tag_hdr.length - sizeof (APE_Tag_Hdr);
    int i;

    for (i = 0; i < m_tag->ape_tag_hdr.item_count && index >= 0 && q - p > 8; ++i) {
        int vsize, flags, isize;

        vsize = * (int32_t *) p; p += 4;
        flags = * (int32_t *) p; p += 4;
        isize = (int) strnlen ( (TUint8*)p, q - p);

        little_endian_to_native (&vsize, "L");
        little_endian_to_native (&flags, "L");

        if (vsize < 0 || vsize > m_tag->ape_tag_hdr.length || p + isize + vsize + 1 > q)
            break;

        if (isize && vsize && !(flags & 6) && !index--) {

            if (!item || !size)
                return isize;

            if (isize < size) {
                memcpy (item, p, isize);
                item [isize] = 0;
                return isize;
            }
            else if (size >= 4) {
                memcpy (item, p, size - 1);
                item [size - 4] = item [size - 3] = item [size - 2] = '.';
                item [size - 1] = 0;
                return size - 1;
            }
            else
                return 0;
        }
        else
            p += isize + vsize + 1;
    }

    return 0;
}

static int tagdata (char *src, int tag_size);

static int get_id3_tag_item_indexed (M_Tag *m_tag, int index, char *item, int size)
{
//    char lvalue [16];
//    int len;
//
//    lvalue [0] = 0;
//
//    if (tagdata (m_tag->id3_tag.title, sizeof (m_tag->id3_tag.title)) && !index--)
//        strcpy (lvalue, "Title");
//    else if (tagdata (m_tag->id3_tag.artist, sizeof (m_tag->id3_tag.artist)) && !index--)
//        strcpy (lvalue, "Artist");
//    else if (tagdata (m_tag->id3_tag.album, sizeof (m_tag->id3_tag.album)) && !index--)
//        strcpy (lvalue, "Album");
//    else if (tagdata (m_tag->id3_tag.year, sizeof (m_tag->id3_tag.year)) && !index--)
//        strcpy (lvalue, "Year");
//    else if (tagdata (m_tag->id3_tag.comment, sizeof (m_tag->id3_tag.comment)) && !index--)
//        strcpy (lvalue, "Comment");
//    else if (m_tag->id3_tag.comment [29] && !m_tag->id3_tag.comment [28] && !index--)
//        strcpy (lvalue, "Track");
//    else
//        return 0;
//
//    len = (int) strlen (lvalue);
//
//    if (!item || !size)
//        return len;
//
//    if (len < size) {
//        strcpy (item, lvalue);
//        return len;
//    }
//    else if (size >= 4) {
//        strncpy (item, lvalue, size - 1);
//        item [size - 4] = item [size - 3] = item [size - 2] = '.';
//        item [size - 1] = 0;
//        return size - 1;
//    }
//    else
        return 0;
}

#endif

#endif



// REMOVED ROUTINES FOR WRITING




uint32_t WavpackGetNumSamples (WavpackContext *wpc)
{
    return wpc ? wpc->total_samples : (uint32_t) -1;
}




uint32_t WavpackGetSampleIndex (WavpackContext *wpc)
{
    if (wpc) {
#if !defined(VER4_ONLY) && !defined(NO_UNPACK)
        if (wpc->stream3)
            return get_sample_index3 (wpc);
        else if (wpc->streams [0])
            return wpc->streams [0]->sample_index;
#else
        if (wpc->streams [0])
            return wpc->streams [0]->sample_index;
#endif
    }

    return (uint32_t) -1;
}

int WavpackGetNumErrors (WavpackContext *wpc)
{
    return wpc ? wpc->crc_errors : 0;
}


int WavpackLossyBlocks (WavpackContext *wpc)
{
    return wpc ? wpc->lossy_blocks : 0;
}


double WavpackGetProgress (WavpackContext *wpc)
{
    if (wpc && wpc->total_samples != (uint32_t) -1 && wpc->total_samples != 0)
        return (double) WavpackGetSampleIndex (wpc) / wpc->total_samples;
    else
        return -1.0;
}



uint32_t WavpackGetFileSize (WavpackContext *wpc)
{
    return wpc ? wpc->filelen + wpc->file2len : 0;
}



double WavpackGetRatio (WavpackContext *wpc)
{
    if (wpc && wpc->total_samples != (uint32_t) -1 && wpc->filelen) {
        double output_size = (double) wpc->total_samples * wpc->config.num_channels *
            wpc->config.bytes_per_sample;
        double input_size = (double) wpc->filelen + wpc->file2len;

        if (output_size >= 1.0 && input_size >= 1.0)
            return input_size / output_size;
    }

    return 0.0;
}



double WavpackGetAverageBitrate (WavpackContext *wpc, int count_wvc)
{
    if (wpc && wpc->total_samples != (uint32_t) -1 && wpc->filelen) {
        double output_time = (double) wpc->total_samples / wpc->config.sample_rate;
        double input_size = (double) wpc->filelen + (count_wvc ? wpc->file2len : 0);

        if (output_time >= 0.1 && input_size >= 1.0)
            return input_size * 8.0 / output_time;
    }

    return 0.0;
}




#ifndef NO_UNPACK



double WavpackGetInstantBitrate (WavpackContext *wpc)
{
    if (wpc->stream3)
        return WavpackGetAverageBitrate (wpc, TRUE);

    if (wpc && wpc->streams [0] && wpc->streams [0]->wphdr.block_samples) {
        double output_time = (double) wpc->streams [0]->wphdr.block_samples / wpc->config.sample_rate;
        double input_size = 0;
        int si;

        for (si = 0; si < wpc->num_streams; ++si) {
            if (wpc->streams [si]->blockbuff)
                input_size += ((WavpackHeader *) wpc->streams [si]->blockbuff)->ckSize;

            if (wpc->streams [si]->block2buff)
                input_size += ((WavpackHeader *) wpc->streams [si]->block2buff)->ckSize;
        }

        if (output_time > 0.0 && input_size >= 1.0)
            return input_size * 8.0 / output_time;
    }

    return 0.0;
}

#endif



WavpackContext *WavpackCloseFile (WavpackContext *wpc)
{
    free_streams (wpc);

    if (wpc->streams [0])
        free (wpc->streams [0]);

#if !defined(VER4_ONLY) && !defined(NO_UNPACK)
//    if (wpc->stream3)	// TODO?
//        free_stream3 (wpc);
#endif

#if !defined(NO_UNPACK) || defined(INFO_ONLY)
    if (wpc->close_files) {
#ifndef NO_USE_FSTREAMS
        if (wpc->wv_in != NULL)
        	{
        	RFile *f = (RFile*) wpc->wv_in;
        	f->Close();
        	delete wpc->wv_in;
        	}
//            fclose (wpc->wv_in);

        if (wpc->wvc_in != NULL)
        	{
        	RFile *f = (RFile*) wpc->wvc_in;
        	f->Close();
        	delete wpc->wvc_in;
        	}
//            fclose (wpc->wvc_in);
#endif
    }

    WavpackFreeWrapper (wpc);
#endif

#ifndef NO_TAGS
    free_tag (&wpc->m_tag);
#endif

    free (wpc);

    return NULL;
}



uint32_t WavpackGetSampleRate (WavpackContext *wpc)
{
    return wpc ? wpc->config.sample_rate : 44100;
}



int WavpackGetNumChannels (WavpackContext *wpc)
{
    return wpc ? wpc->config.num_channels : 2;
}




int WavpackGetChannelMask (WavpackContext *wpc)
{
    return wpc ? wpc->config.channel_mask : 0;
}




int WavpackGetFloatNormExp (WavpackContext *wpc)
{
    return wpc->config.float_norm_exp;
}



int WavpackGetBitsPerSample (WavpackContext *wpc)
{
    return wpc ? wpc->config.bits_per_sample : 16;
}



int WavpackGetBytesPerSample (WavpackContext *wpc)
{
    return wpc ? wpc->config.bytes_per_sample : 2;
}

#if !defined(NO_UNPACK) || defined(INFO_ONLY)



int WavpackGetReducedChannels (WavpackContext *wpc)
{
    if (wpc)
        return wpc->reduced_channels ? wpc->reduced_channels : wpc->config.num_channels;
    else
        return 2;
}



uint32_t WavpackGetWrapperBytes (WavpackContext *wpc)
{
    return wpc ? wpc->wrapper_bytes : 0;
}

uchar *WavpackGetWrapperData (WavpackContext *wpc)
{
    return wpc ? wpc->wrapper_data : NULL;
}

void WavpackFreeWrapper (WavpackContext *wpc)
{
    if (wpc && wpc->wrapper_data) {
        free (wpc->wrapper_data);
        wpc->wrapper_data = NULL;
        wpc->wrapper_bytes = 0;
    }
}




static void seek_riff_trailer (WavpackContext *wpc);

void WavpackSeekTrailingWrapper (WavpackContext *wpc)
{
    if ((wpc->open_flags & OPEN_WRAPPER) &&
        wpc->reader->can_seek (wpc->wv_in) && !wpc->stream3) {
            uint32_t pos_save = wpc->reader->get_pos (wpc->wv_in);

            seek_riff_trailer (wpc);
            wpc->reader->set_pos_abs (wpc->wv_in, pos_save);
    }
}




static int seek_md5 (WavpackStreamReader *reader, void *id, uchar data [16]);

int WavpackGetMD5Sum (WavpackContext *wpc, uchar data [16])
{
    if (wpc->config.flags & CONFIG_MD5_CHECKSUM) {
        if (wpc->config.md5_read) {
            memcpy (data, wpc->config.md5_checksum, 16);
            return TRUE;
        }
        else if (wpc->reader->can_seek (wpc->wv_in)) {
            uint32_t pos_save = wpc->reader->get_pos (wpc->wv_in);

            wpc->config.md5_read = seek_md5 (wpc->reader, wpc->wv_in, wpc->config.md5_checksum);
            wpc->reader->set_pos_abs (wpc->wv_in, pos_save);

            if (wpc->config.md5_read) {
                memcpy (data, wpc->config.md5_checksum, 16);
                return TRUE;
            }
            else
                return FALSE;
        }
    }

    return FALSE;
}

#endif



static void free_streams (WavpackContext *wpc)
{
    int si = wpc->num_streams;

    while (si--) {
        if (wpc->streams [si]->blockbuff) {
            free (wpc->streams [si]->blockbuff);
            wpc->streams [si]->blockbuff = NULL;
        }

        if (wpc->streams [si]->block2buff) {
            free (wpc->streams [si]->block2buff);
            wpc->streams [si]->block2buff = NULL;
        }

        if (wpc->streams [si]->sample_buffer) {
            free (wpc->streams [si]->sample_buffer);
            wpc->streams [si]->sample_buffer = NULL;
        }

        if (wpc->streams [si]->dc.shaping_data) {
            free (wpc->streams [si]->dc.shaping_data);
            wpc->streams [si]->dc.shaping_data = NULL;
        }

        if (si) {
            wpc->num_streams--;
            free (wpc->streams [si]);
            wpc->streams [si] = NULL;
        }
    }

    wpc->current_stream = 0;
}




#ifndef NO_TAGS



static int valid_tag (M_Tag *m_tag)
{
    if (m_tag->ape_tag_hdr.ID [0] == 'A')
        return 'A';
    else if (m_tag->id3_tag.tag_id [0] == 'T')
        return 'T';
    else
        return 0;
}

// Free the data for any APEv2 tag that was allocated.

static void free_tag (M_Tag *m_tag)
{
    if (m_tag->ape_tag_data) {
        free (m_tag->ape_tag_data);
        m_tag->ape_tag_data = NULL;
    }
}

#endif



#if !defined(NO_UNPACK) || defined(INFO_ONLY)



static uint32_t read_next_header (WavpackStreamReader *reader, void *id, WavpackHeader *wphdr)
{
    unsigned char buffer [sizeof (*wphdr)], *sp = buffer + sizeof (*wphdr), *ep = sp;
    uint32_t bytes_skipped = 0;
    int bleft;

    while (1) {
        if (sp < ep) {
            bleft = (int)(ep - sp);
            memcpy (buffer, sp, bleft);
        }
        else
            bleft = 0;

        if (reader->read_bytes (id, buffer + bleft, sizeof (*wphdr) - bleft) != sizeof (*wphdr) - bleft)
            return -1;

        sp = buffer;

        if (*sp++ == 'w' && *sp == 'v' && *++sp == 'p' && *++sp == 'k' &&
            !(*++sp & 1) && sp [2] < 16 && !sp [3] && (sp [2] || sp [1] || *sp > 24) && sp [5] == 4 &&
            sp [4] >= (MIN_STREAM_VERS & 0xff) && sp [4] <= (MAX_STREAM_VERS & 0xff) && sp [18] < 3 && !sp [19]) {
                memcpy (wphdr, buffer, sizeof (*wphdr));
                little_endian_to_native (wphdr, WavpackHeaderFormat);
                return bytes_skipped;
            }

        while (sp < ep && *sp != 'w')
            sp++;

        if ((bytes_skipped += (uint32_t)(sp - buffer)) > 1024 * 1024)
            return -1;
    }
}



static uint32_t seek_final_index (WavpackStreamReader *reader, void *id)
{
    uint32_t result = (uint32_t) -1, bcount;
    WavpackHeader wphdr;
    uchar *tempbuff;

    if (reader->get_length (id) > 1200000L)
        reader->set_pos_rel (id, -1048576L, SEEK_END);
    else
        reader->set_pos_abs (id, 0);

    while (1) {
        bcount = read_next_header (reader, id, &wphdr);

        if (bcount == (uint32_t) -1)
            return result;

        tempbuff = malloc (wphdr.ckSize + 8);
        memcpy (tempbuff, &wphdr, 32);

        if (reader->read_bytes (id, tempbuff + 32, wphdr.ckSize - 24) != wphdr.ckSize - 24) {
            free (tempbuff);
            return result;
        }

        free (tempbuff);

        if (wphdr.block_samples && (wphdr.flags & FINAL_BLOCK))
            result = wphdr.block_index + wphdr.block_samples;
    }
}

static int seek_md5 (WavpackStreamReader *reader, void *id, uchar data [16])
{
    uchar meta_id, c1, c2;
    uint32_t bcount, meta_bc;
    WavpackHeader wphdr;

    if (reader->get_length (id) > 1200000L)
        reader->set_pos_rel (id, -1048576L, SEEK_END);

    while (1) {
        bcount = read_next_header (reader, id, &wphdr);

        if (bcount == (uint32_t) -1)
            return FALSE;

        bcount = wphdr.ckSize - sizeof (WavpackHeader) + 8;

        while (bcount >= 2) {
            if (reader->read_bytes (id, &meta_id, 1) != 1 ||
                reader->read_bytes (id, &c1, 1) != 1)
                    return FALSE;

            meta_bc = c1 << 1;
            bcount -= 2;

            if (meta_id & ID_LARGE) {
                if (bcount < 2 || reader->read_bytes (id, &c1, 1) != 1 ||
                    reader->read_bytes (id, &c2, 1) != 1)
                        return FALSE;

                meta_bc += ((uint32_t) c1 << 9) + ((uint32_t) c2 << 17);
                bcount -= 2;
            }

            if (meta_id == ID_MD5_CHECKSUM)
                return (meta_bc == 16 && bcount >= 16 &&
                    reader->read_bytes (id, data, 16) == 16);

            reader->set_pos_rel (id, meta_bc, SEEK_CUR);
            bcount -= meta_bc;
        }
    }
}

static void seek_riff_trailer (WavpackContext *wpc)
{
    WavpackStreamReader *reader = wpc->reader;
    void *id = wpc->wv_in;
    uchar meta_id, c1, c2;
    uint32_t bcount, meta_bc;
    WavpackHeader wphdr;

    if (reader->get_length (id) > 1200000L)
        reader->set_pos_rel (id, -1048576L, SEEK_END);

    while (1) {
        bcount = read_next_header (reader, id, &wphdr);

        if (bcount == (uint32_t) -1)
            return;

        bcount = wphdr.ckSize - sizeof (WavpackHeader) + 8;

        while (bcount >= 2) {
            if (reader->read_bytes (id, &meta_id, 1) != 1 ||
                reader->read_bytes (id, &c1, 1) != 1)
                    return;

            meta_bc = c1 << 1;
            bcount -= 2;

            if (meta_id & ID_LARGE) {
                if (bcount < 2 || reader->read_bytes (id, &c1, 1) != 1 ||
                    reader->read_bytes (id, &c2, 1) != 1)
                        return;

                meta_bc += ((uint32_t) c1 << 9) + ((uint32_t) c2 << 17);
                bcount -= 2;
            }

            if ((meta_id & ID_UNIQUE) == ID_RIFF_TRAILER) 
			{
//				void* tmp = (void*) new TUint8[ wpc->wrapper_bytes + meta_bc ];
//				CLEAR( tmp );
//				memmove( tmp, wpc->wrapper_data, wpc->wrapper_bytes );
                wpc->wrapper_data = (uchar*)realloc (wpc->wrapper_data, wpc->wrapper_bytes + meta_bc);

                if (reader->read_bytes (id, wpc->wrapper_data + wpc->wrapper_bytes, meta_bc) == meta_bc)
                    wpc->wrapper_bytes += meta_bc;
                else
                    return;
            }
            else
                reader->set_pos_rel (id, meta_bc, SEEK_CUR);

            bcount -= meta_bc;
        }
    }
}



static int match_wvc_header (WavpackHeader *wv_hdr, WavpackHeader *wvc_hdr)
{
    if (wv_hdr->block_index == wvc_hdr->block_index &&
        wv_hdr->block_samples == wvc_hdr->block_samples) {
            int wvi = 0, wvci = 0;

            if (wv_hdr->flags == wvc_hdr->flags)
                return 0;

            if (wv_hdr->flags & INITIAL_BLOCK)
                wvi -= 1;

            if (wv_hdr->flags & FINAL_BLOCK)
                wvi += 1;

            if (wvc_hdr->flags & INITIAL_BLOCK)
                wvci -= 1;

            if (wvc_hdr->flags & FINAL_BLOCK)
                wvci += 1;

            return (wvci - wvi < 0) ? 1 : -1;
        }

    if ((int32_t)(wvc_hdr->block_index - wv_hdr->block_index) < 0)
        return 1;
    else
        return -1;
}




static int read_wvc_block (WavpackContext *wpc)
{
    WavpackStream *wps = wpc->streams [wpc->current_stream];
    uint32_t bcount, file2pos;
    WavpackHeader wphdr;
    int compare_result;

    while (1) {
        file2pos = wpc->reader->get_pos (wpc->wvc_in);
        bcount = read_next_header (wpc->reader, wpc->wvc_in, &wphdr);

        if (bcount == (uint32_t) -1) {
            wps->wvc_skip = TRUE;
            wpc->crc_errors++;
            return FALSE;
        }

        if (wpc->open_flags & OPEN_STREAMING)
            wphdr.block_index = wps->sample_index = 0;
        else
            wphdr.block_index -= wpc->initial_index;

        if (wphdr.flags & INITIAL_BLOCK)
            wpc->file2pos = file2pos + bcount;

        compare_result = match_wvc_header (&wps->wphdr, &wphdr);

        if (!compare_result) {
            wps->block2buff = malloc (wphdr.ckSize + 8);
            memcpy (wps->block2buff, &wphdr, 32);

            if (wpc->reader->read_bytes (wpc->wvc_in, wps->block2buff + 32, wphdr.ckSize - 24) !=
                wphdr.ckSize - 24 || (wphdr.flags & UNKNOWN_FLAGS)) {
                    free (wps->block2buff);
                    wps->block2buff = NULL;
                    wps->wvc_skip = TRUE;
                    wpc->crc_errors++;
                    return FALSE;
            }

            wps->wvc_skip = FALSE;
            memcpy (&wps->wphdr, &wphdr, 32);
            return TRUE;
        }
        else if (compare_result == -1) {
            wps->wvc_skip = TRUE;
            wpc->reader->set_pos_rel (wpc->wvc_in, -32, SEEK_CUR);
            wpc->crc_errors++;
            return TRUE;
        }
    }
}



#ifndef NO_SEEKING



#define BUFSIZE 4096

static uint32_t find_header (WavpackStreamReader *reader, void *id, uint32_t filepos, WavpackHeader *wphdr)
{
    unsigned char *buffer = malloc (BUFSIZE), *sp = buffer, *ep = buffer;

    if (filepos != (uint32_t) -1 && reader->set_pos_abs (id, filepos)) {
        free (buffer);
        return -1;
    }

    while (1) {
        int bleft;

        if (sp < ep) {
            bleft = (int)(ep - sp);
            memcpy (buffer, sp, bleft);
            ep -= (sp - buffer);
            sp = buffer;
        }
        else {
            if (sp > ep)
                if (reader->set_pos_rel (id, (int32_t)(sp - ep), SEEK_CUR)) {
                    free (buffer);
                    return -1;
                }

            sp = ep = buffer;
            bleft = 0;
        }

        ep += reader->read_bytes (id, ep, BUFSIZE - bleft);

        if (ep - sp < 32) {
            free (buffer);
            return -1;
        }

        while (sp + 32 <= ep)
            if (*sp++ == 'w' && *sp == 'v' && *++sp == 'p' && *++sp == 'k' &&
                !(*++sp & 1) && sp [2] < 16 && !sp [3] && (sp [2] || sp [1] || *sp > 24) && sp [5] == 4 &&
                sp [4] >= (MIN_STREAM_VERS & 0xff) && sp [4] <= (MAX_STREAM_VERS & 0xff) && sp [18] < 3 && !sp [19]) {
                    memcpy (wphdr, sp - 4, sizeof (*wphdr));
                    little_endian_to_native (wphdr, WavpackHeaderFormat);

                    if (wphdr->block_samples && (wphdr->flags & INITIAL_BLOCK)) {
                        free (buffer);
                        return reader->get_pos (id) - (ep - sp + 4);
                    }

                    if (wphdr->ckSize > 1024)
                        sp += wphdr->ckSize - 1024;
            }
    }
}


static uint32_t find_sample (WavpackContext *wpc, void *infile, uint32_t header_pos, uint32_t sample)
{
    WavpackStream *wps = wpc->streams [wpc->current_stream];
    uint32_t file_pos1 = 0, file_pos2 = wpc->reader->get_length (infile);
    uint32_t sample_pos1 = 0, sample_pos2 = wpc->total_samples;
    double ratio = 0.96;
    int file_skip = 0;

    if (sample >= wpc->total_samples)
        return -1;

    if (header_pos && wps->wphdr.block_samples) {
        if (wps->wphdr.block_index > sample) {
            sample_pos2 = wps->wphdr.block_index;
            file_pos2 = header_pos;
        }
        else if (wps->wphdr.block_index + wps->wphdr.block_samples <= sample) {
            sample_pos1 = wps->wphdr.block_index;
            file_pos1 = header_pos;
        }
        else
            return header_pos;
    }

    while (1) {
        double bytes_per_sample;
        uint32_t seek_pos;

        bytes_per_sample = file_pos2 - file_pos1;
        bytes_per_sample /= sample_pos2 - sample_pos1;
        seek_pos = file_pos1 + (file_skip ? 32 : 0);
        seek_pos += (uint32_t)(bytes_per_sample * (sample - sample_pos1) * ratio);
        seek_pos = find_header (wpc->reader, infile, seek_pos, &wps->wphdr);

        if (seek_pos != (uint32_t) -1)
            wps->wphdr.block_index -= wpc->initial_index;

        if (seek_pos == (uint32_t) -1 || seek_pos >= file_pos2) {
            if (ratio > 0.0) {
                if ((ratio -= 0.24) < 0.0)
                    ratio = 0.0;
            }
            else
                return -1;
        }
        else if (wps->wphdr.block_index > sample) {
            sample_pos2 = wps->wphdr.block_index;
            file_pos2 = seek_pos;
        }
        else if (wps->wphdr.block_index + wps->wphdr.block_samples <= sample) {

            if (seek_pos == file_pos1)
                file_skip = 1;
            else {
                sample_pos1 = wps->wphdr.block_index;
                file_pos1 = seek_pos;
            }
        }
        else
            return seek_pos;
    }
}

#endif







#ifndef NO_TAGS



static int load_tag (WavpackContext *wpc)
{
    int ape_tag_length, ape_tag_items;
    M_Tag *m_tag = &wpc->m_tag;

    CLEAR (*m_tag);

    while (1) {

        // attempt to find an APEv2 tag either at end-of-file or before a ID3v1 tag we found

        if (m_tag->id3_tag.tag_id [0] == 'T')
            wpc->reader->set_pos_rel (wpc->wv_in, -(int32_t)(sizeof (APE_Tag_Hdr) + sizeof (ID3_Tag)), SEEK_END);
        else
            wpc->reader->set_pos_rel (wpc->wv_in, -(int32_t)sizeof (APE_Tag_Hdr), SEEK_END);

        if (wpc->reader->read_bytes (wpc->wv_in, &m_tag->ape_tag_hdr, sizeof (APE_Tag_Hdr)) == sizeof (APE_Tag_Hdr) &&
            !strncmp ( (TInt8*) m_tag->ape_tag_hdr.ID, (TInt8*)"APETAGEX", 8)) {

                little_endian_to_native (&m_tag->ape_tag_hdr, APE_Tag_Hdr_Format);

                if (m_tag->ape_tag_hdr.version == 2000 && m_tag->ape_tag_hdr.item_count &&
                    m_tag->ape_tag_hdr.length > sizeof (m_tag->ape_tag_hdr) &&
                    m_tag->ape_tag_hdr.length < (1024 * 1024) &&
                    (m_tag->ape_tag_data = new char[ m_tag->ape_tag_hdr.length ]) != NULL) 
                	{
//                    (m_tag->ape_tag_data = malloc (m_tag->ape_tag_hdr.length)) != NULL) {

                        ape_tag_items = m_tag->ape_tag_hdr.item_count;
                        ape_tag_length = m_tag->ape_tag_hdr.length;

                        if (m_tag->id3_tag.tag_id [0] == 'T')
                            m_tag->tag_file_pos = -(int32_t)sizeof (ID3_Tag);
                        else
                            m_tag->tag_file_pos = 0;

                        m_tag->tag_file_pos -= ape_tag_length + sizeof (APE_Tag_Hdr);
                        wpc->reader->set_pos_rel (wpc->wv_in, m_tag->tag_file_pos, SEEK_END);
                        memset (m_tag->ape_tag_data, 0, ape_tag_length);

                        if (wpc->reader->read_bytes (wpc->wv_in, &m_tag->ape_tag_hdr, sizeof (APE_Tag_Hdr)) !=
                            sizeof (APE_Tag_Hdr) || strncmp ((TInt8*)m_tag->ape_tag_hdr.ID, (TInt8*)"APETAGEX", 8)) {
                                free (m_tag->ape_tag_data);
                                CLEAR (*m_tag);
                                return FALSE;       // something's wrong...
                        }

                        little_endian_to_native (&m_tag->ape_tag_hdr, APE_Tag_Hdr_Format);

                        if (m_tag->ape_tag_hdr.version != 2000 || m_tag->ape_tag_hdr.item_count != ape_tag_items ||
                            m_tag->ape_tag_hdr.length != ape_tag_length) {
                                free (m_tag->ape_tag_data);
                                CLEAR (*m_tag);
                                return FALSE;       // something's wrong...
                        }

                        if (wpc->reader->read_bytes (wpc->wv_in, m_tag->ape_tag_data,
                            ape_tag_length - sizeof (APE_Tag_Hdr)) != ape_tag_length - sizeof (APE_Tag_Hdr)) {
                                free (m_tag->ape_tag_data);
                                CLEAR (*m_tag);
                                return FALSE;       // something's wrong...
                        }
                        else {
                            CLEAR (m_tag->id3_tag); // ignore ID3v1 tag if we found APEv2 tag
                            return TRUE;
                        }
                }
        }

        if (m_tag->id3_tag.tag_id [0] == 'T') {     // settle for the ID3v1 tag that we found
            CLEAR (m_tag->ape_tag_hdr);
            return TRUE;
        }

        // look for ID3v1 tag if APEv2 tag not found during first pass

        m_tag->tag_file_pos = -(int32_t)sizeof (ID3_Tag);
        wpc->reader->set_pos_rel (wpc->wv_in, m_tag->tag_file_pos, SEEK_END);

        if (wpc->reader->read_bytes (wpc->wv_in, &m_tag->id3_tag, sizeof (ID3_Tag)) != sizeof (ID3_Tag) ||
            strncmp ((TInt8*)m_tag->id3_tag.tag_id, (TInt8*)"TAG", 3)) {
                CLEAR (*m_tag);
                return FALSE;       // neither type of tag found
        }
    }
}




static void tagcpy (char *dest, char *src, int tag_size)
{
    char *s1 = src, *s2 = src + tag_size - 1;

    if (*s2 && !s2 [-1])
        s2--;

    while (s1 <= s2)
        if (*s1 == ' ')
            ++s1;
        else if (!*s2 || *s2 == ' ')
            --s2;
        else
            break;

    while (*s1 && s1 <= s2)
        *dest++ = *s1++;

    *dest = 0;
}

static int tagdata (char *src, int tag_size)
{
    char *s1 = src, *s2 = src + tag_size - 1;

    if (*s2 && !s2 [-1])
        s2--;

    while (s1 <= s2)
        if (*s1 == ' ')
            ++s1;
        else if (!*s2 || *s2 == ' ')
            --s2;
        else
            break;

    return (*s1 && s1 <= s2);
}

#endif


#endif


void WavpackLittleEndianToNative (void *data, char *format)
{
    little_endian_to_native (data, format);
}

void WavpackNativeToLittleEndian (void *data, char *format)
{
    native_to_little_endian (data, format);
}

uint32_t WavpackGetLibraryVersion (void)
{
    return (LIBWAVPACK_MAJOR<<16)
          |(LIBWAVPACK_MINOR<<8)
          |(LIBWAVPACK_MICRO<<0);
}

const char *WavpackGetLibraryVersionString (void)
{
    return LIBWAVPACK_VERSION_STRING;
}


uchar* WavpackFormatSamples ( int bps, uchar *dst, int32_t *src, uint32_t samcnt )
{
    int32_t temp;

    switch (bps) {

        case 1:
            while (samcnt--)
                *dst++ = *src++ + 128;

            break;

        case 2:
            while (samcnt--) {
                *dst++ = (uchar) (temp = *src++);
                *dst++ = (uchar) (temp >> 8);
            }

            break;

        case 3:
            while (samcnt--) {
                *dst++ = (uchar) (temp = *src++);
                *dst++ = (uchar) (temp >> 8);
                *dst++ = (uchar) (temp >> 16);
            }

            break;

        case 4:
            while (samcnt--) {
                *dst++ = (uchar) (temp = *src++);
                *dst++ = (uchar) (temp >> 8);
                *dst++ = (uchar) (temp >> 16);
                *dst++ = (uchar) (temp >> 24);
            }

            break;
    }

    return dst;
}
