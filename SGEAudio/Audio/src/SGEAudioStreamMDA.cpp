/*
 ============================================================================
 Name		: SGEAudioStreamMDA.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEAudioStreamMDA implementation
 ============================================================================
 */

#include "SGEAudioStreamMDA.h"
//#include "logger.h"


CSGEAudioStreamMDA::CSGEAudioStreamMDA( MSGEAudioStreamObserver &aObserver, TSGEAudioConfig &aConfig )
    : CSGEAudioStreamBase( aObserver, aConfig )
    {
    // No implementation required
    }

CSGEAudioStreamMDA::~CSGEAudioStreamMDA()
    {
    /*if ( iMdaAudioOutputStream )
        {
        iMdaAudioOutputStream->Stop();
        }*/
    delete iMdaAudioOutputStream;
    }

CSGEAudioStreamMDA* CSGEAudioStreamMDA::NewLC( MSGEAudioStreamObserver &aObserver, TSGEAudioConfig &aConfig )
    {
    CSGEAudioStreamMDA *self = new (ELeave) CSGEAudioStreamMDA( aObserver, aConfig );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

CSGEAudioStreamMDA* CSGEAudioStreamMDA::NewL( MSGEAudioStreamObserver &aObserver, TSGEAudioConfig &aConfig )
    {
    CSGEAudioStreamMDA *self = CSGEAudioStreamMDA::NewLC( aObserver, aConfig );
    CleanupStack::Pop(); // self;
    return self;
    }

void CSGEAudioStreamMDA::ConstructL()
    {
    BaseConstructL();
    
    iMdaAudioOutputStream = CMdaAudioOutputStream::NewL( *this, 
                                                         EMdaPriorityMax, 
                                                         EMdaPriorityPreferenceTime );     // EMdaPriorityPreferenceTimeAndQuality
    iMdaAudioOutputStream->Open( NULL );
    }

void CSGEAudioStreamMDA::ConfigureAudioL()
    {
    iMdaAudioOutputStream->SetDataTypeL( iConfig.FourCC() );

    iMdaAudioOutputStream->SetAudioPropertiesL( iConfig.SampleRateForSubsystem(), 
                                                iConfig.ChannelsForSubsystem() );

    const TInt KPercent100 = 100;
    TInt maxvol = iMdaAudioOutputStream->MaxVolume();
    iMdaAudioOutputStream->SetVolume( maxvol * iConfig.iMaxVolumePercent / KPercent100 );

    iMdaAudioOutputStream->WriteL( iBufferPtr );    // play silence
    }

void CSGEAudioStreamMDA::UpdateNow()
    {
    TRAP_IGNORE( WriteToStreamL() );
    }

TInt CSGEAudioStreamMDA::MaxVolume()
    {
    return iMdaAudioOutputStream->MaxVolume();
    }

void CSGEAudioStreamMDA::WriteToStreamL()
    {
    MixData();
    
    GetNextData();
    
    iMdaAudioOutputStream->WriteL( iBufferPtr );
    }

void CSGEAudioStreamMDA::MaoscOpenComplete( TInt aError )
    {
    if ( aError == KErrNone )
        {
        TRAP( aError, ConfigureAudioL() );
        }
    
    iObserver.AudioStreamCreated( aError );
    }

void CSGEAudioStreamMDA::MaoscBufferCopied( TInt /*aError*/, const TDesC8& /*aBuffer*/ )
    {
    TRAP_IGNORE( WriteToStreamL() );
    }

void CSGEAudioStreamMDA::MaoscPlayComplete( TInt aError )
    {
    if ( aError == KErrAccessDenied || aError == KErrDied )
        {   // phone call
        //        Logger::LogStrNum( _L("c:\\data\\audio.txt"), _L("Retry "), iConfig.CalculateSoundLatency() );
        aError = KErrNone;
        }

    if ( aError == KErrNone )
        {       
        // playback terminated normally after a call to Stop()
        }
    else if ( aError == KErrUnderflow )
        {
        TRAP_IGNORE( WriteToStreamL() );
        // the end of the sound data has been reached or
        // data hasn’t been supplied to the stream fast enough
        }
    else
        {
        // Other errors, handle here
        iObserver.AudioStreamError( aError );
        }
    }



