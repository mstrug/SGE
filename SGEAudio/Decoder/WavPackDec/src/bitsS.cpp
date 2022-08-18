/*
 * bitsS.cpp
 *
 *  Created on: 2009-09-18
 *      Author: Michal Strug
 *   Copyright: Copyright (c) 2022 Michal Strug. All rights reserved.
 */


#include "wavpack_local.h"


#if !defined(NO_UNPACK) || defined(INFO_ONLY)


static void bs_read (Bitstream *bs);


void bs_open_read (Bitstream *bs, void *buffer_start, void *buffer_end)
{
    bs->error = bs->sr = bs->bc = 0;
    bs->ptr = (bs->buf = (uchar*) buffer_start) - 1;
    bs->end = (uchar*) buffer_end;
    bs->wrap = bs_read;
}


static void bs_read (Bitstream *bs)
{
    bs->ptr = bs->buf - 1;
    bs->error = 1;
}


uint32_t bs_close_read (Bitstream *bs)
{
    uint32_t bytes_read;

    if (bs->bc < sizeof (*(bs->ptr)) * 8)
        bs->ptr++;

    bytes_read = (uint32_t)(bs->ptr - bs->buf) * sizeof (*(bs->ptr));

    if (!(bytes_read & 1))
        ++bytes_read;

    CLEAR (*bs);
    return bytes_read;
}

#endif




#ifndef NO_PACK


static void bs_write (Bitstream *bs);

void bs_open_write (Bitstream *bs, void *buffer_start, void *buffer_end)
{
    bs->error = bs->sr = bs->bc = 0;
    bs->ptr = bs->buf = (uchar*)buffer_start;
    bs->end = (uchar*)buffer_end;
    bs->wrap = bs_write;
}

static void bs_write (Bitstream *bs)
{
    bs->ptr = bs->buf;
    bs->error = 1;
}


uint32_t bs_close_write (Bitstream *bs)
{
    uint32_t bytes_written;

    if (bs->error)
        return (uint32_t) -1;

    while (1) {
        while (bs->bc)
            putbit_1 (bs);

        bytes_written = (uint32_t)(bs->ptr - bs->buf) * sizeof (*(bs->ptr));

        if (bytes_written & 1) {
            putbit_1 (bs);
        }
        else
            break;
    };

    CLEAR (*bs);
    return bytes_written;
}

#endif


void little_endian_to_native (void *data, char *format)
{
    uchar *cp = (uchar *) data;
    int32_t temp;

    // SGE PORT: 
    // problems on Samsung SGH-L870
    // on arm Symbian phones all is little_endian encoding so no conversion is needen
    return;
    
    while (*format) {
        switch (*format) {
            case 'L':
            	{
                temp = cp [0] + ((int32_t) cp [1] << 8) + ((int32_t) cp [2] << 16) + ((int32_t) cp [3] << 24);
                int32_t *cpTemp = (int32_t *) cp;
                *cpTemp = temp;
                cp += 4;
            	}
                break;

            case 'S':
            	{
                temp = cp [0] + (cp [1] << 8);
                * (short *) cp = (short) temp;
                cp += 2;
            	}
                break;

            default:
// TODO:                if (isdigit (*format))
//                    cp += *format - '0';

                break;
        }

        format++;
    }
}

void native_to_little_endian (void *data, char *format)
{
    uchar *cp = (uchar *) data;
    int32_t temp;

    while (*format) {
        switch (*format) {
            case 'L':
                temp = * (int32_t *) cp;
                *cp++ = (uchar) temp;
                *cp++ = (uchar) (temp >> 8);
                *cp++ = (uchar) (temp >> 16);
                *cp++ = (uchar) (temp >> 24);
                break;

            case 'S':
                temp = * (short *) cp;
                *cp++ = (uchar) temp;
                *cp++ = (uchar) (temp >> 8);
                break;

            default:
// TODO:               if (isdigit (*format))
//                    cp += *format - '0';

                break;
        }

        format++;
    }
}


