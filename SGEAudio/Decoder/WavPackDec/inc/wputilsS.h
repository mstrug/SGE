/*
 * wputilsS.h
 *
 *  Created on: 2009-09-18
 *      Author: Michal Strug
 *   Copyright: Copyright (c) 2022 Michal Strug. All rights reserved.
 */

#ifndef WPUTILSS_H_
#define WPUTILSS_H_

#include "wavpack_local.h"

WavpackContext* WavpackOpenFileInput( RFs aFs, const TDesC& aInFilename, TInt aFlags, int norm_offset);

WavpackContext *WavpackCloseFile (WavpackContext *wpc);

uint32_t WavpackUnpackSamples (WavpackContext *wpc, int32_t *buffer, uint32_t samples);

int WavpackSeekSample (WavpackContext *wpc, uint32_t sample);

uint32_t WavpackGetSampleIndex (WavpackContext *wpc);

uint32_t WavpackGetNumSamples (WavpackContext *wpc);

double WavpackGetInstantBitrate (WavpackContext *wpc);

double WavpackGetAverageBitrate (WavpackContext *wpc, int count_wvc);

double WavpackGetRatio (WavpackContext *wpc);

uint32_t WavpackGetFileSize (WavpackContext *wpc);

double WavpackGetProgress (WavpackContext *wpc);

int WavpackLossyBlocks (WavpackContext *wpc);

int WavpackGetNumErrors (WavpackContext *wpc);

int WavpackGetReducedChannels (WavpackContext *wpc);

int WavpackGetBitsPerSample (WavpackContext *wpc);

int WavpackGetBytesPerSample (WavpackContext *wpc);

int WavpackGetFloatNormExp (WavpackContext *wpc);

int WavpackGetChannelMask (WavpackContext *wpc);

int WavpackGetNumChannels (WavpackContext *wpc);

uint32_t WavpackGetSampleRate (WavpackContext *wpc);

uint32_t WavpackGetWrapperBytes (WavpackContext *wpc);

uchar *WavpackGetWrapperData (WavpackContext *wpc);

void WavpackFreeWrapper (WavpackContext *wpc);

uchar* WavpackFormatSamples ( int bps, uchar *dst, int32_t *src, uint32_t samcnt );
// old: static uchar *format_samples (int bps, uchar *dst, int32_t *src, uint32_t samcnt)

#endif /* WPUTILSS_H_ */
