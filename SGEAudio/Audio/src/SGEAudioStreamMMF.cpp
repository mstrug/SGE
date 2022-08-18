/*
 ============================================================================
 Name		: SGEAudioStreamMMF.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEAudioStreamMMF implementation
 ============================================================================
 */

#include "SGEAudioStreamMMF.h"


        
CSGEAudioStreamMMF::CSGEAudioStreamMMF( MSGEAudioStreamObserver &aObserver, TSGEAudioConfig &aConfig )
    : CSGEAudioStreamBase( aObserver, aConfig )
    {
    // No implementation required
    }

CSGEAudioStreamMMF::~CSGEAudioStreamMMF()
    {
    /*if ( iMMFDevSound )
        {
        iMMFDevSound->Stop();
        }*/
    delete iMMFDevSound;
    }

CSGEAudioStreamMMF* CSGEAudioStreamMMF::NewLC( MSGEAudioStreamObserver &aObserver, TSGEAudioConfig &aConfig )
    {
    CSGEAudioStreamMMF *self = new (ELeave) CSGEAudioStreamMMF( aObserver, aConfig );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

CSGEAudioStreamMMF* CSGEAudioStreamMMF::NewL( MSGEAudioStreamObserver &aObserver, TSGEAudioConfig &aConfig )
    {
    CSGEAudioStreamMMF *self = CSGEAudioStreamMMF::NewLC( aObserver, aConfig );
    CleanupStack::Pop(); // self;
    return self;
    }

void CSGEAudioStreamMMF::ConstructL()
    {
    BaseConstructL();
    
    iMMFDevSound = CMMFDevSound::NewL();

    iMMFDevSound->InitializeL( *this, iConfig.FourCC(), EMMFStatePlaying );
    }

void CSGEAudioStreamMMF::ConfigureAudioL()
    {
    TMMFCapabilities caps;
    caps.iBufferSize = iConfig.iBufferSize;
//    caps.iBufferSize = iConfig.BufferSizeInBytes(); // todo: or iConfig.iBufferSize
    caps.iChannels = iConfig.ChannelsForSubsystem();
    caps.iRate = iConfig.SampleRateForSubsystem();
    caps.iEncoding = iConfig.BufferDataTypeForSubsystem();
    
    iMMFDevSound->SetConfigL( caps );
    
    const TInt KPercent100 = 100;
    TInt maxvol = iMMFDevSound->MaxVolume();
    iMMFDevSound->SetVolume( maxvol * iConfig.iMaxVolumePercent / KPercent100 );

    TMMFPrioritySettings priority;
    priority.iPriority = 100;
    priority.iPref = EMdaPriorityPreferenceTime;
    iMMFDevSound->SetPrioritySettings( priority );
    
    iMMFDevSound->PlayInitL();
    }

void CSGEAudioStreamMMF::Retry()
    {
    if ( !IsActive() )
        {
        iTickCount = User::NTickCount();
        MixData(); // this gets next audio data (also sends play start/stop events)
        After( iConfig.CalculateSoundLatency() );
        }
    }

void CSGEAudioStreamMMF::UpdateNow()
    {    
    }

TInt CSGEAudioStreamMMF::MaxVolume()
    {
    return iMMFDevSound->MaxVolume();    
    }

void CSGEAudioStreamMMF::InitializeComplete( TInt aError )
    {
    if ( aError == KErrNone )
        {
        TRAP( aError, ConfigureAudioL() );
        }
    
    iObserver.AudioStreamCreated( aError );
    }

void CSGEAudioStreamMMF::ToneFinished( TInt /*aError*/ )
    {
    // No implementation required
    }

void CSGEAudioStreamMMF::BufferToBeFilled( CMMFBuffer *aBuffer )
    {
    CMMFDataBuffer *ptrBuf = reinterpret_cast<CMMFDataBuffer*>( aBuffer );
    
    MixData();

    GetNextData();
    
    ptrBuf->Data().Copy( iBufferPtr );
    
    iMMFDevSound->PlayData();
    }

void CSGEAudioStreamMMF::RunL()
    {
    TUint32 ticks = User::NTickCount() - iTickCount;
    for ( TUint32 i = 0; i <= ticks * 1000 / iConfig.CalculateSoundLatency(); i++ )
        {
        MixData(); // this gets next audio data (also sends play start/stop events)
        }
    
    iMMFDevSound->PlayInitL();
    }

TInt CSGEAudioStreamMMF::RunError( TInt /*aError*/ )
    {
    Retry();
    
    return KErrNone;
    }

void CSGEAudioStreamMMF::PlayError( TInt aError )
    {
    
    if ( aError == KErrAccessDenied || aError == KErrDied || aError == KErrInUse )
        {   // handling phone call during dev sound
        iMMFDevSound->Stop();
        Retry();
        return;
        }

    if ( aError == KErrNone )
        {       
        // playback terminated normally after a call to Stop()
        }
    else if ( aError == KErrUnderflow )
        {
        iMMFDevSound->PlayData();
        // the end of the sound data has been reached or
        // data hasn’t been supplied to the stream fast enough
        }
    else
        {
        iObserver.AudioStreamError( aError );
        }
    }

void CSGEAudioStreamMMF::BufferToBeEmptied( CMMFBuffer* /*aBuffer*/ )
    {
    ASSERT( EFalse );
    }

void CSGEAudioStreamMMF::RecordError( TInt /*aError*/ )
    {
    // No implementation required
    }

void CSGEAudioStreamMMF::ConvertError( TInt /*aError*/ )
    {
    // No implementation required
    }

void CSGEAudioStreamMMF::DeviceMessage( TUid /*aMessageType*/, const TDesC8& /*aMsg*/ )
    {
    // No implementation required
    }


