/*
 ============================================================================
 Name		: SGEAudioFileDecoderWAV.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEAudioFileDecoderWAV implementation
 ============================================================================
 */

#include "SGEAudioFileDecoderWAV.h"

CSGEAudioFileDecoderWAV::CSGEAudioFileDecoderWAV( MSGEAudioFileDecoderObserver &aObserver, RBuf8 &aData )
    : CSGEAudioFileDecoder( aObserver, aData )
    {
    // No implementation required
    }

CSGEAudioFileDecoderWAV::~CSGEAudioFileDecoderWAV()
    {
    }

CSGEAudioFileDecoderWAV* CSGEAudioFileDecoderWAV::NewLC( MSGEAudioFileDecoderObserver &aObserver, RBuf8 &aData )
    {
    CSGEAudioFileDecoderWAV *self = new (ELeave) CSGEAudioFileDecoderWAV( aObserver, aData );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

CSGEAudioFileDecoderWAV* CSGEAudioFileDecoderWAV::NewL( MSGEAudioFileDecoderObserver &aObserver, RBuf8 &aData )
    {
    CSGEAudioFileDecoderWAV *self = CSGEAudioFileDecoderWAV::NewLC( aObserver, aData );
    CleanupStack::Pop(); // self;
    return self;
    }

void CSGEAudioFileDecoderWAV::ConstructL()
    {
    BaseConstructL();

    TInt idxRiff = iData.Find( _L8("RIFF") );
    TInt idxWave = iData.Find( _L8("WAVE") );
    if ( ! ( idxRiff == 0 && idxWave == 8 ) )
        {
        User::Leave( KErrArgument );
        }
    }

void CSGEAudioFileDecoderWAV::StartDecodingL( const TSGEAudioFile &aAudioFile, const TSGEAudioConfig& /*aAudioConfig*/ )
    {
    SWaveFmt waveFmt = { 0 };
    
    TInt idxFmt = iData.FindF( _L8("fmt ") );
    if ( idxFmt > KErrNotFound )
        {
        TInt idxFmtSize = idxFmt + 4; 
        TUint32 fmtSize = *( (TUint32*) ( iData.Ptr() + idxFmtSize ) );
        if ( fmtSize >= sizeof( SWaveFmt ) )
            {
            waveFmt = *( (SWaveFmt*) ( iData.Ptr() + idxFmtSize + 4 ) );
            if ( waveFmt.iCompressionCode != 1 && waveFmt.iCompressionCode != 2 )
                {   // supporting only 16 and 8 bits per sample
                User::Leave( KErrNotSupported );
                }
            if ( waveFmt.iNumberOfChannels > 2 )
                {   // supporting only mono and stereo
                User::Leave( KErrNotSupported );
                }
            }
        else
            {
            User::Leave( KErrNotSupported );
            }
        }
    else
        {
        User::Leave( KErrNotSupported );
        }
    
    TInt idxData = iData.FindF( _L8("data") );
    if ( idxData > KErrNotFound )
        {
        TInt idxDataSize = idxData + 4; 
        iData.Delete( 0, idxDataSize );
        TUint32 dataSize = *((TUint32*)iData.Ptr());
        iData.Delete( 0, 4 );
        iData.Delete( dataSize, iData.Length() - dataSize );

        const TInt KBitsPerByte = 8;
        CSGEAudioSample *audioSample = CSGEAudioSample::NewL( aAudioFile.iId, 
                                                              waveFmt.iSignificantBitsPerSample / KBitsPerByte, 
                                                              dataSize );
        audioSample->iBps = waveFmt.iSignificantBitsPerSample;
        audioSample->iChannels = waveFmt.iNumberOfChannels;
        audioSample->iFrequency = waveFmt.iSampleRate;
        audioSample->iAudioGroup = aAudioFile.iAudioGroup;
        
        Mem::Copy( audioSample->iData, iData.Ptr(), iData.Size() );
        
        iObserver.AudioFileDecodingFinished( audioSample, KErrNone );
        }
    else
        {
        User::Leave( KErrNotSupported );
        }
    }

