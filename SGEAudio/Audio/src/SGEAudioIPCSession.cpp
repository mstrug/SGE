/*
 ============================================================================
 Name		: SGEAudioIPCSession.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEAudioIPCSession implementation
 ============================================================================
 */

#include "SGEAudioIPCSession.h"
#include "SGEAudioIPCCommon.h"


CSGEAudioIPCSession::CSGEAudioIPCSession( MSGEAudioIPCSessionObserver &aObserver, 
                                          CSGEAudioEngine &aAudioEngine )
    : iObserver( aObserver ), iAudioEngine( aAudioEngine )
    {
    // No implementation required
    }

CSGEAudioIPCSession::~CSGEAudioIPCSession()
    {
    iTrackEventsQueue.Close();
    iObserver.SessionClosed();
    }

CSGEAudioIPCSession* CSGEAudioIPCSession::NewLC( MSGEAudioIPCSessionObserver &aObserver, 
                                                 CSGEAudioEngine &aAudioEngine )
    {
    CSGEAudioIPCSession *self = new (ELeave) CSGEAudioIPCSession( aObserver, aAudioEngine );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

CSGEAudioIPCSession* CSGEAudioIPCSession::NewL( MSGEAudioIPCSessionObserver &aObserver, 
                                                CSGEAudioEngine &aAudioEngine )
    {
    CSGEAudioIPCSession *self = CSGEAudioIPCSession::NewLC( aObserver, aAudioEngine );
    CleanupStack::Pop(); // self;
    return self;
    }

void CSGEAudioIPCSession::ConstructL()
    {
    }

void CSGEAudioIPCSession::ServiceL( const RMessage2 &aMessage )
    {
    switch ( aMessage.Function() )
        {
        case ESGEAudioIPCCloseServer:
                aMessage.Complete( KErrNone );
                iObserver.SessionRequestedToCloseServer();
            break;
        case ESGEAudioIPCCancelPendingRequests:
                EventCancelPendingRequestsL( aMessage );
                aMessage.Complete( KErrNone );
            break;
        case ESGEAudioIPCAddAudioFromBuffer:
                EventAddAudioFromBufferL( aMessage );
                aMessage.Complete( KErrNone );
            break;
        case ESGEAudioIPCAddFileToLoad:
                EventAddFileToLoadL( aMessage );
                aMessage.Complete( KErrNone );
            break;
        case ESGEAudioIPCAddFileToLoadFromSGEFile:
                EventAddFileToLoadFromSGEFileL( aMessage );
                aMessage.Complete( KErrNone );
            break;
        case ESGEAudioIPCRemoveAudioSample:
                EventRemoveAudioSampleL( aMessage );
                aMessage.Complete( KErrNone );
            break;
        case ESGEAudioIPCRemoveAudioSampleAll:
                EventRemoveAudioSampleAllL( aMessage );
                aMessage.Complete( KErrNone );
            break;
        case ESGEAudioIPCStartLoadingFiles: /* async */
                EventStartLoadingFilesL( aMessage );
            break;
        case ESGEAudioIPCCreateAudioStream: /* async */
                EventCreateAudioStreamL( aMessage );
            break;
        case ESGEAudioIPCDestroyAudioStream:
                EventDestroyAudioStreamL( aMessage );
                aMessage.Complete( KErrNone );
            break;
        case ESGEAudioIPCCompactTracks:
                EventCompactTracksL( aMessage );
                aMessage.Complete( KErrNone );
            break;
        case ESGEAudioIPCTrackState:
                EventTrackStateL( aMessage );
                aMessage.Complete( KErrNone );
            break;
        case ESGEAudioIPCTrackCount: /* complete in function */
                EventTrackCountL( aMessage );
            break;
        case ESGEAudioIPCPlay: /* complete in function */
                EventPlayL( aMessage );
            break;
        case ESGEAudioIPCStop:
                EventStopL( aMessage );
                aMessage.Complete( KErrNone );
            break;
        case ESGEAudioIPCPause:
                EventPauseL( aMessage );
                aMessage.Complete( KErrNone );
            break;
        case ESGEAudioIPCResume:
                EventResumeL( aMessage );
                aMessage.Complete( KErrNone );
            break;
        case ESGEAudioIPCMute:
                EventMuteL( aMessage );
                aMessage.Complete( KErrNone );
            break;
        case ESGEAudioIPCUnmute:
                EventUnmuteL( aMessage );
                aMessage.Complete( KErrNone );
            break;
        case ESGEAudioIPCStopAll:
                EventStopAllL( aMessage );
                aMessage.Complete( KErrNone );
            break;
        case ESGEAudioIPCPauseAll:
                EventPauseAllL( aMessage );
                aMessage.Complete( KErrNone );
            break;
        case ESGEAudioIPCResumeAll:
                EventResumeAllL( aMessage );
                aMessage.Complete( KErrNone );
            break;
        case ESGEAudioIPCMuteAll:
                EventMuteAllL( aMessage );
                aMessage.Complete( KErrNone );
            break;
        case ESGEAudioIPCUnmuteAll:
                EventUnmuteAllL( aMessage );
                aMessage.Complete( KErrNone );
            break;
        case ESGEAudioIPCSetVolume:
                EventSetVolumeL( aMessage );
                aMessage.Complete( KErrNone );
            break;
        case ESGEAudioIPCSetVolumeAll:
                EventSetVolumeAllL( aMessage );
                aMessage.Complete( KErrNone );
            break;
        case ESGEAudioIPCSubscribeForInitializationEvents: /* async */
                EventSubscribeForInitializationEventsL( aMessage );
            break;
        case ESGEAudioIPCSubscribeForTrackEvents: /* async */
                EventSubscribeForTrackEventsL( aMessage );
            break;
        default:
                aMessage.Panic( KSGEAudioIPCClientPanic(), KErrNotSupported );
            break;
        }
    }

void CSGEAudioIPCSession::EventCancelPendingRequestsL( const RMessage2& /*aMessage*/ )
    {
    if ( !iMessageInitialization.IsNull() )
        {
        iMessageInitialization.Complete( KErrCancel );
        }
    if ( !iMessageTrackEvents.IsNull() )
        {
        iMessageTrackEvents.Complete( KErrCancel );
        }
    iAudioEngine.Cancel();
    }

void CSGEAudioIPCSession::EventAddAudioFromBufferL( const RMessage2 &aMessage )
    {
    const TAny *data = aMessage.Ptr0();
    TSGEAudioIPCAddAudioFromBufferParams params;
    TPckg< TSGEAudioIPCAddAudioFromBufferParams > pckgParams( params );
    aMessage.Read( 1, pckgParams );
    iAudioEngine.AddAudioFromBufferL( data, pckgParams().iId, pckgParams().iBufferDataTypeSize,
                                      pckgParams().iDataLength, pckgParams().iFrequency,
                                      pckgParams().iChannels, (TSGEAudioGroup) pckgParams().iAudioGroup );
    }

void CSGEAudioIPCSession::EventAddFileToLoadL( const RMessage2 &aMessage )
    {
    TSGEAudioFile file( KNullDesC(), 0, ESGEAudioGroupAny, ESGEAudioFileTypeWAV );
    TPckg< TSGEAudioFile > pckgFile( file );
    aMessage.Read( 0, pckgFile );
    iAudioEngine.AddFileToLoad( pckgFile() );
    }

void CSGEAudioIPCSession::EventAddFileToLoadFromSGEFileL( const RMessage2 &aMessage )
    {
    TFileName *fn = (TFileName*) aMessage.Ptr0();
    TSGEAudioFile file( KNullDesC(), 0, ESGEAudioGroupAny, ESGEAudioFileTypeWAV );
    TPckg< TSGEAudioFile > pckgFile( file );
    aMessage.Read( 1, pckgFile );
    iAudioEngine.AddFileToLoad( *fn, pckgFile() );
    }

void CSGEAudioIPCSession::EventRemoveAudioSampleL( const RMessage2 &aMessage )
    {
    TUint sampleId = aMessage.Int0();
    iAudioEngine.RemoveAudioSample( sampleId );
    }

void CSGEAudioIPCSession::EventRemoveAudioSampleAllL( const RMessage2& /*aMessage*/ )
    {
    iAudioEngine.RemoveAudioSampleAll();
    }

void CSGEAudioIPCSession::EventStartLoadingFilesL( const RMessage2 &aMessage )
    {
    if ( !iMessageInitialization.IsNull() )
        {
        User::Leave( KErrServerBusy );
        }
    iAudioEngine.StartLoadingFilesL();
    iMessageInitialization = aMessage;
    }

void CSGEAudioIPCSession::EventCreateAudioStreamL( const RMessage2 &aMessage )
    {
    if ( !iMessageInitialization.IsNull() )
        {
        User::Leave( KErrServerBusy );
        }
    iAudioEngine.CreateAudioStreamL();
    iMessageInitialization = aMessage;
    }

void CSGEAudioIPCSession::EventDestroyAudioStreamL( const RMessage2& /*aMessage*/ )
    {
    iAudioEngine.DestroyAudioStream();
    }

void CSGEAudioIPCSession::EventCompactTracksL( const RMessage2 &aMessage )
    {
    TInt trackCount = aMessage.Int0();
    iAudioEngine.CompactTracks( trackCount );
    }

void CSGEAudioIPCSession::EventTrackStateL( const RMessage2 &aMessage )
    {
    TUint trackId = aMessage.Int0();
    TSGEAudioTrackState state = iAudioEngine.TrackState( trackId );
    TPckg< TSGEAudioTrackState > statePckg( state );
    User::LeaveIfError( aMessage.Write( 1, statePckg ) );
    }

void CSGEAudioIPCSession::EventTrackCountL( const RMessage2 &aMessage )
    {
    aMessage.Complete( iAudioEngine.TrackCount() );    
    }

void CSGEAudioIPCSession::EventPlayL( const RMessage2 &aMessage )
    {
    TUint sampleId = aMessage.Int0();
    TInt repeat = aMessage.Int1();
    aMessage.Complete( iAudioEngine.Play( sampleId, repeat ) );
    }

void CSGEAudioIPCSession::EventStopL( const RMessage2 &aMessage )
    {
    TUint trackId = aMessage.Int0();
    iAudioEngine.Stop( trackId );
    }

void CSGEAudioIPCSession::EventPauseL( const RMessage2 &aMessage )
    {
    TUint trackId = aMessage.Int0();
    iAudioEngine.Pause( trackId );
    }

void CSGEAudioIPCSession::EventResumeL( const RMessage2 &aMessage )
    {
    TUint trackId = aMessage.Int0();
    iAudioEngine.Resume( trackId );
    }

void CSGEAudioIPCSession::EventMuteL( const RMessage2 &aMessage )
    {
    TUint trackId = aMessage.Int0();
    iAudioEngine.Mute( trackId );
    }

void CSGEAudioIPCSession::EventUnmuteL( const RMessage2 &aMessage )
    {
    TUint trackId = aMessage.Int0();
    iAudioEngine.Unmute( trackId );
    }

void CSGEAudioIPCSession::EventStopAllL( const RMessage2& /*aMessage*/ )
    {
    iAudioEngine.StopAll();
    }

void CSGEAudioIPCSession::EventPauseAllL( const RMessage2& /*aMessage*/ )
    {
    iAudioEngine.PauseAll();
    }

void CSGEAudioIPCSession::EventResumeAllL( const RMessage2& /*aMessage*/ )
    {
    iAudioEngine.ResumeAll();
    }

void CSGEAudioIPCSession::EventMuteAllL( const RMessage2& /*aMessage*/ )
    {
    iAudioEngine.MuteAll();
    }

void CSGEAudioIPCSession::EventUnmuteAllL( const RMessage2& /*aMessage*/ )
    {
    iAudioEngine.UnmuteAll();
    }

void CSGEAudioIPCSession::EventSetVolumeL( const RMessage2 &aMessage )
    {
    TUint trackId = aMessage.Int0();
    TInt volume = aMessage.Int1();
    iAudioEngine.SetVolume( trackId, volume );
    }

void CSGEAudioIPCSession::EventSetVolumeAllL( const RMessage2 &aMessage )
    {
    TInt volume = aMessage.Int0();
    iAudioEngine.SetVolumeAll( volume );
    }

void CSGEAudioIPCSession::EventSubscribeForInitializationEventsL( const RMessage2 &aMessage )
    {
    if ( !iMessageInitialization.IsNull() )
        {
        User::Leave( KErrServerBusy );
        }
    iMessageInitialization = aMessage;
    if ( iAllFilesLoadedNotificationQueued )
        {
        AudioEngineAllFilesLoaded();
        }
    }

void CSGEAudioIPCSession::EventSubscribeForTrackEventsL( const RMessage2 &aMessage )
    {
    if ( !iMessageTrackEvents.IsNull() )
        {
        User::Leave( KErrServerBusy );
        }
    iMessageTrackEvents = aMessage;
    if ( iTrackEventsQueue.Count() > 0 )
        {
        TInt ev = iTrackEventsQueue[ 0 ].iType;
        TPckg< TInt > event( ev );
        TInt err = iMessageTrackEvents.Write( 0, event, 0 );
        if ( err != KErrNone )
            {
            iMessageTrackEvents.Complete( err );
            }
        else 
            {
            iMessageTrackEvents.Complete( iTrackEventsQueue[ 0 ].iTrackid );
            iTrackEventsQueue.Remove( 0 );
            }
        }
    }

void CSGEAudioIPCSession::AudioEngineFileLoaded( TInt aSoundSampleId, TInt aError )
    {
    if ( !iMessageInitialization.IsNull() )
        {
        TInt ev = ESGEAudioIPCInitializationEventFileLoaded;
        TPckg< TInt > event( ev );
        TInt err = iMessageInitialization.Write( 0, event );
        if ( err != KErrNone )
            {
            iMessageInitialization.Complete( err );
            return;
            }
        TPckg< TInt > sample( aSoundSampleId );
        err = iMessageInitialization.Write( 1, sample );
        if ( err != KErrNone )
            {
            iMessageInitialization.Complete( err );
            return;
            }
        
        else if ( aError < KErrNone )
            {
            iMessageInitialization.Complete( aError );
            }
        else
            {
            iMessageInitialization.Complete( aSoundSampleId );
            }
        }
    }

void CSGEAudioIPCSession::AudioEngineAllFilesLoaded()
    {
    if ( !iMessageInitialization.IsNull() )
        {
        TInt ev = ESGEAudioIPCInitializationEventAllFilesLoaded;
        TPckg< TInt > event( ev );
        TInt err = iMessageInitialization.Write( 0, event, 0 );
        if ( err != KErrNone )
            {
            iMessageInitialization.Complete( err );
            }
        else
            {
            iMessageInitialization.Complete( KErrNone );
            }
        iAllFilesLoadedNotificationQueued = EFalse;
        }
    else
        {
        iAllFilesLoadedNotificationQueued = ETrue;
        }
    }

void CSGEAudioIPCSession::AudioEngineSoundTrackPlayStarted( TUint aTrackId )
    {
    if ( !iMessageTrackEvents.IsNull() )
        {
        TInt ev = ESGEAudioIPCSoundTrackEventPlayStarted;
        TPckg< TInt > event( ev );
        TInt err = iMessageTrackEvents.Write( 0, event, 0 );
        if ( err != KErrNone )
            {
            iMessageTrackEvents.Complete( err );
            }
        else 
            {
            iMessageTrackEvents.Complete( aTrackId );
            }
        }
    else
        {
        STrackEventQueueElement event = { ESGEAudioIPCSoundTrackEventPlayStarted, aTrackId };
        iTrackEventsQueue.Append( event );
        }
    }

void CSGEAudioIPCSession::AudioEngineSoundTrackPlayFinished( TUint aTrackId )
    {
    if ( !iMessageTrackEvents.IsNull() )
        {
        TInt ev = ESGEAudioIPCSoundTrackEventPlayFinished;
        TPckg< TInt > event( ev );
        TInt err = iMessageTrackEvents.Write( 0, event, 0 );
        if ( err != KErrNone )
            {
            iMessageTrackEvents.Complete( err );
            }
        else
            {
            iMessageTrackEvents.Complete( aTrackId );
            }
        }
    else
        {
        STrackEventQueueElement event = { ESGEAudioIPCSoundTrackEventPlayFinished, aTrackId };
        iTrackEventsQueue.Append( event );
        }
    }

void CSGEAudioIPCSession::AudioEngineStreamCreated( TInt aError )
    {
    if ( !iMessageInitialization.IsNull() )
        {
        TInt ev = ESGEAudioIPCInitializationEventStreamCreated;
        TPckg< TInt > event( ev );
        TInt err = iMessageInitialization.Write( 0, event, 0 );
        if ( err != KErrNone )
            {
            iMessageInitialization.Complete( err );
            }
        else
            {
            iMessageInitialization.Complete( aError );
            }
        }
    }

void CSGEAudioIPCSession::AudioEngineStreamError( TInt aError )
    {
    if ( !iMessageInitialization.IsNull() )
        {
        TInt ev = ESGEAudioIPCInitializationEventStreamError;
        TPckg< TInt > event( ev );
        TInt err = iMessageInitialization.Write( 0, event, 0 );
        if ( err != KErrNone )
            {
            iMessageInitialization.Complete( err );
            }
        else
            {
            iMessageInitialization.Complete( aError );
            }
        }
    }

