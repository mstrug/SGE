/*
 ============================================================================
 Name		: CSGEAudioWavPackDecoder.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEWavPakDecoder implementation
 ============================================================================
 */

#include "SGEAudioWavPackDecoder.h"
#include "wputilsS.h"
#include <BAUTILS.H>


EXPORT_C TBool CSGEAudioWavPackDecoder::DecodeFileL( RFs aFs, const TDesC &aFileName, RBuf8 &aOutputData, 
		CSGEAudioWavPackDecoder::TWaveInfo &aDataInfo )
	{
	WavpackContext *wpc = WavpackOpenFileInput( aFs, aFileName, 0, 0);
	if ( !wpc )
		{
		return EFalse;
		}

	uint32_t samples = WavpackGetNumSamples( wpc );
	uint32_t chanles = WavpackGetNumChannels( wpc );
	uint32_t bps = WavpackGetBytesPerSample( wpc );
	TInt size = chanles * samples * 4;	// 4096L * num_channels * 4
	TUint8* buffer = new TUint8[ size ];
	
	if ( !buffer )
		{
		WavpackCloseFile( wpc );
		return EFalse;
		}
	
	uint32_t ret = WavpackUnpackSamples( wpc, (long*)buffer, samples);	
		
	TInt sizeOut = ret * chanles;
	TUint8* bufferOut = new TUint8[ sizeOut  ];
	
	if ( !bufferOut )
		{
		delete [] buffer;
		WavpackCloseFile( wpc );
		return EFalse;
		}
	
	if ( ret )
		{
		WavpackFormatSamples( bps, (uchar*)bufferOut, (long*)buffer, sizeOut  );
		}
	else
		{
		delete [] buffer;
		WavpackCloseFile( wpc );
		return EFalse;
		}

	aOutputData.Assign( (TUint8*) bufferOut, sizeOut, sizeOut );

	aDataInfo.iBitsPerSample = WavpackGetBitsPerSample( wpc );
	aDataInfo.iNumberOfChannels = WavpackGetNumChannels( wpc );
	aDataInfo.iSampleRate = WavpackGetSampleRate( wpc );

	WavpackCloseFile( wpc );

	delete [] buffer;
	
	return ETrue;
	}


EXPORT_C TBool CSGEAudioWavPackDecoder::DecodeFileL( RBuf8 &aInputData, RBuf8 &aOutputData, 
		CSGEAudioWavPackDecoder::TWaveInfo &aDataInfo )
	{	// todo: add support to the WavpackOpenFileInput to read from buffer
	RFs fs;
	if ( fs.Connect() != KErrNone )
		{
		return EFalse;
		}
	
	// create temporary file
	_LIT( KTempFileDir, "d:\\system\\temp\\" );
	BaflUtils::EnsurePathExistsL( fs, KTempFileDir() );
	RFile temp;
	TFileName fn;
	if ( temp.Temp( fs, KTempFileDir(), fn, EFileWrite ) != KErrNone )
		{
		fs.Close();
		return EFalse;
		}
	temp.Write( aInputData );
	temp.Close();
	
	WavpackContext *wpc = WavpackOpenFileInput( fs, fn, 0, 0);
	if ( !wpc )
		{
		fs.Delete( fn );
		return EFalse;
		}

	uint32_t samples = WavpackGetNumSamples( wpc );
	uint32_t chanles = WavpackGetNumChannels( wpc );
	uint32_t bps = WavpackGetBytesPerSample( wpc );
	TInt size = chanles * samples * 4;	// 4096L * num_channels * 4
	TUint8* buffer = new TUint8[ size ];
	
	if ( !buffer )
		{
		WavpackCloseFile( wpc );
		fs.Delete( fn );
		return EFalse;
		}
	
	uint32_t ret = WavpackUnpackSamples( wpc, (long*)buffer, samples);	
		
	TInt sizeOut = ret * chanles;
	TUint8* bufferOut = new TUint8[ sizeOut  ];
	
	if ( !bufferOut )
		{
		delete [] buffer;
		WavpackCloseFile( wpc );
		fs.Delete( fn );
		return EFalse;
		}
	
	if ( ret )
		{
		WavpackFormatSamples( bps, (uchar*)bufferOut, (long*)buffer, sizeOut  );
		}
	else
		{
		delete [] buffer;
		WavpackCloseFile( wpc );
		fs.Delete( fn );
		return EFalse;
		}

	aOutputData.Assign( (TUint8*) bufferOut, sizeOut, sizeOut );

	aDataInfo.iBitsPerSample = WavpackGetBitsPerSample( wpc );
	aDataInfo.iNumberOfChannels = WavpackGetNumChannels( wpc );
	aDataInfo.iSampleRate = WavpackGetSampleRate( wpc );
	
	WavpackCloseFile( wpc );
	fs.Delete( fn );
	fs.Close();
	
	delete [] buffer;
	
	return ETrue;
	}


