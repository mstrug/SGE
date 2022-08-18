/*
 ============================================================================
 Name		: CSGEAudioWavPackDecoder.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEWavPakDecoder declaration
 ============================================================================
 */

#ifndef SGEAUDIOWAVPACKDECODER_H
#define SGEAUDIOWAVPACKDECODER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <f32file.h>

// CLASS DECLARATION

/**
 *  CSGEAudioWavPackDecoder
 * 
 */
class CSGEAudioWavPackDecoder : public CBase
	{
public:
	
	class TWaveInfo
		{
	public:
		TInt iNumberOfChannels;
		TInt iSampleRate;
		TInt iBitsPerSample;
		};
	
public:

	IMPORT_C static TBool DecodeFileL( RFs aFs, const TDesC &aFileName, RBuf8 &aOutputData, 
									   TWaveInfo &aDataInfo );

	IMPORT_C static TBool DecodeFileL( RBuf8 &aInputData, RBuf8 &aOutputData, TWaveInfo &aDataInfo );
	
	};

#endif // SGEAUDIOWAVPACKDECODER_H
