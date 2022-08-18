/*
 ============================================================================
 Name		: SGEAudioFileDecoderMP3.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEAudioFileDecoderMP3 implementation
 ============================================================================
 */

#include "SGEAudioFileDecoderMP3.h"
#include <mmf/plugin/mmfcodecimplementationuids.hrh>


const TInt KMP3toWAVSizePrescaler = 16;


CSGEAudioFileDecoderMP3::CSGEAudioFileDecoderMP3( MSGEAudioFileDecoderObserver &aObserver, RBuf8 &aData )
    : CSGEAudioFileDecoder( aObserver, aData )
    {
    // No implementation required
    }

CSGEAudioFileDecoderMP3::~CSGEAudioFileDecoderMP3()
    {
    delete iCodec;
    }

CSGEAudioFileDecoderMP3* CSGEAudioFileDecoderMP3::NewLC( MSGEAudioFileDecoderObserver &aObserver, RBuf8 &aData )
    {
    CSGEAudioFileDecoderMP3 *self = new (ELeave) CSGEAudioFileDecoderMP3( aObserver, aData );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

CSGEAudioFileDecoderMP3* CSGEAudioFileDecoderMP3::NewL( MSGEAudioFileDecoderObserver &aObserver, RBuf8 &aData )
    {
    CSGEAudioFileDecoderMP3 *self = CSGEAudioFileDecoderMP3::NewLC( aObserver, aData );
    CleanupStack::Pop(); // self;
    return self;
    }

void CSGEAudioFileDecoderMP3::ConstructL()
    {
    BaseConstructL();

    iCodec = CMMFCodec::NewL( TFourCC( KMMFFourCCCodeMP3 ), TFourCC( KMMFFourCCCodePCM16 ) );
    }

void CSGEAudioFileDecoderMP3::StartDecodingL( const TSGEAudioFile &aAudioFile, const TSGEAudioConfig &aAudioConfig )
    {
    const TInt KOneByte = 8;
    TInt dataSize = aAudioConfig.SizeOfBufferDataType();
    CSGEAudioSample *audioSample = CSGEAudioSample::NewLC( aAudioFile.iId, dataSize, iData.Length() * KMP3toWAVSizePrescaler / dataSize );
    TPtr8 dataPtr( (TUint8*) audioSample->iData, audioSample->DataSize() );
    
    // Configure codec 
    RArray<TInt> configParams;
    configParams.Append(2 - aAudioConfig.iChannels); // stereo to mono switch (TRUE(1) or default FALSE(0))
    configParams.Append(0); // decode left or right channel (1 left, 2 right, 0 default all)
    configParams.Append(1); // decimation factor (2, 4 or default 1)  [input rate / output rate = M]
    configParams.Append(1); // error concealment level (0 none, default 1)
    configParams.Append(dataSize * KOneByte); // resolution of PCM samples (default 16 bits)
    configParams.Append(aAudioConfig.iSampleRate); //Output sampling frequency
    TUid codecId = TUid::Uid( KUidMmfCodecAudioSettings );
    TDesC8& codecData = REINTERPRET_CAST(TDesC8&, configParams);
    iCodec->ConfigureL(codecId, codecData);
    configParams.Close(); 
    
    CMMFPtrBuffer* bufIn = CMMFPtrBuffer::NewL( (const TPtr8&) iData );
    CleanupStack::PushL( bufIn );    
    CMMFPtrBuffer* bufOut = CMMFPtrBuffer::NewL( (const TPtr8&) dataPtr );
    CleanupStack::PushL( bufOut );
        
    TCodecProcessResult res = iCodec->ProcessL( *bufIn, *bufOut );
    
    CleanupStack::PopAndDestroy( bufOut );
    CleanupStack::PopAndDestroy( bufIn );
    
    if ( res.iStatus == TCodecProcessResult::EProcessComplete || 
         res.iStatus == TCodecProcessResult::EEndOfData )
        {
        audioSample->iBps = aAudioConfig.iChannels * dataSize * KOneByte;
        audioSample->iChannels = aAudioConfig.iChannels;
        audioSample->iFrequency = aAudioConfig.iSampleRate;
        audioSample->iAudioGroup = aAudioFile.iAudioGroup;
        
        audioSample->AdoptBufferSizeL( res.iDstBytesAdded );
        
        CleanupStack::Pop( audioSample );
        
        iObserver.AudioFileDecodingFinished( audioSample, KErrNone );
        }
    else if ( res.iStatus == TCodecProcessResult::EProcessIncomplete )
        {
        CleanupStack::PopAndDestroy( audioSample );
        User::Leave( KErrUnderflow );
        }
    else
        {
        CleanupStack::PopAndDestroy( audioSample );
        User::Leave( KErrGeneral ); // res.iStatus
        }
    }

