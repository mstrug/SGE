/*
 ============================================================================
 Name		: SGEAudioProviderInternal.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEAudioProviderInternal implementation
 ============================================================================
 */

#include "SGEAudioProviderInternal.h"
#include "SGEAudioIPCServer.h"


CSGEAudioProviderInternal::CSGEAudioProviderInternal( MSGEAudioProviderObserver *aObserver )
    : iObserver( aObserver )
    {
    }

CSGEAudioProviderInternal::~CSGEAudioProviderInternal()
    {
    delete iInitializationAO;
    delete iTrackEventsAO;
    iClient.Close();
    delete iEngine;
    delete iSimpleEngine;
    
    CSGEAudioIPCServer::CloseServer();
    }

CSGEAudioProviderInternal* CSGEAudioProviderInternal::NewLC( MSGEAudioProviderObserver *aObserver, 
                                                             TSGEAudioConfig aConfig )
    {
    CSGEAudioProviderInternal *self = new (ELeave) CSGEAudioProviderInternal( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL( aConfig );
    return self;
    }

CSGEAudioProviderInternal* CSGEAudioProviderInternal::NewL( MSGEAudioProviderObserver *aObserver, 
                                                            TSGEAudioConfig aConfig )
    {
    CSGEAudioProviderInternal *self = CSGEAudioProviderInternal::NewLC( aObserver, aConfig );
    CleanupStack::Pop(); // self;
    return self;
    }

void CSGEAudioProviderInternal::ConstructL( TSGEAudioConfig aConfig )
    {
    if ( aConfig.iMultithreeded )
        {
        if ( aConfig.iSubsystem == ESGEAudioSubsystemSimple )
            {
            User::Leave( KErrNotSupported );
            }
        
        iInitializationAO = new (ELeave) CSGEAudioGPAO( *this );
        
        iTrackEventsAO = new (ELeave) CSGEAudioGPAO( *this );

        CSGEAudioIPCServer::CreateServerL( aConfig );
        
        User::LeaveIfError( iClient.Connect() );
        
        iClient.SubscribeForTrackEvents( iTrackEventsAO->iStatus );
        iTrackEventsAO->SetActive();
        }
    else
        {
        if ( aConfig.iSubsystem == ESGEAudioSubsystemSimple )
            {
            iSimpleEngine = CSGEAudioSimpleEngine::NewL( *this, aConfig ); 
            }
        else
            {
            iEngine = CSGEAudioEngine::NewL( *this, aConfig );
            }
        }
    }

void CSGEAudioProviderInternal::AudioEngineFileLoaded( TInt aSoundSampleId, TInt aError )
    {
    if ( iObserver )
        {
        iObserver->AudioFileLoaded( aSoundSampleId, aError );
        }
    }

void CSGEAudioProviderInternal::AudioEngineAllFilesLoaded()
    {
    if ( iObserver )
        {
        iObserver->AudioAllFilesLoaded();
        }
    }

void CSGEAudioProviderInternal::AudioEngineSoundTrackPlayStarted( TUint aTrackId )
    {
    if ( iObserver )
        {
        iObserver->AudioSoundTrackPlayStarted( aTrackId );
        }
    }

void CSGEAudioProviderInternal::AudioEngineSoundTrackPlayFinished( TUint aTrackId )
    {
    if ( iObserver )
        {
        iObserver->AudioSoundTrackPlayFinished( aTrackId );
        }
    }

void CSGEAudioProviderInternal::AudioEngineStreamCreated( TInt aError )
    {
    if ( iObserver )
        {
        iObserver->AudioStreamCreated( aError );
        }
    }

void CSGEAudioProviderInternal::AudioEngineStreamError( TInt aError )
    {
    if ( iObserver )
        {
        iObserver->AudioStreamError( aError );
        }
    }

void CSGEAudioProviderInternal::GPAONotifierCancel( CSGEAudioGPAO *aGPAO )
    {
    ASSERT( aGPAO == iInitializationAO || aGPAO == iTrackEventsAO );
    if ( aGPAO == iInitializationAO )
        {
        iClient.CancelPendingRequests();
        }
    else if ( aGPAO == iTrackEventsAO )
        {
        iClient.CancelPendingRequests();
        }
    }

void CSGEAudioProviderInternal::GPAONotifierL( CSGEAudioGPAO *aGPAO, TInt aStatus )
    {
    ASSERT( aGPAO == iInitializationAO || aGPAO == iTrackEventsAO );
    if ( iObserver )
        {
        if ( aGPAO == iInitializationAO )
            {
            if ( iClient.InitializationEventType() == RSGEAudioIPCClient::EFileLoaded )
                {
                if ( aStatus >= KErrNone )
                    {
                    iObserver->AudioFileLoaded( iClient.SampleId(), KErrNone );
                    }
                else
                    {
                    iObserver->AudioFileLoaded( iClient.SampleId(), aStatus );
                    }

                
                iClient.SubscribeForInitializationEvents( iInitializationAO->iStatus );
                iInitializationAO->SetActive();
                }
            else if ( iClient.InitializationEventType() == RSGEAudioIPCClient::EAllFilesLoaded )
                {
                iObserver->AudioAllFilesLoaded();
                }
            else if ( iClient.InitializationEventType() == RSGEAudioIPCClient::EStreamCreated )
                {
                iObserver->AudioStreamCreated( aStatus );
                
                iClient.SubscribeForInitializationEvents( iInitializationAO->iStatus );
                iInitializationAO->SetActive();
                }
            else if ( iClient.InitializationEventType() == RSGEAudioIPCClient::EStreamError )
                {
                iObserver->AudioStreamError( aStatus );
                
                iClient.SubscribeForInitializationEvents( iInitializationAO->iStatus );
                iInitializationAO->SetActive();
                }
            }
        else if ( aGPAO == iTrackEventsAO )
            {
            if ( iClient.SoundTrackEventType() == RSGEAudioIPCClient::EPlayFinished )
                {
                iObserver->AudioSoundTrackPlayFinished( aStatus );

                iClient.SubscribeForTrackEvents( iTrackEventsAO->iStatus );
                iTrackEventsAO->SetActive();
                }
            else if ( iClient.SoundTrackEventType() == RSGEAudioIPCClient::EPlayStarted )
                {
                iObserver->AudioSoundTrackPlayStarted( aStatus );

                iClient.SubscribeForTrackEvents( iTrackEventsAO->iStatus );
                iTrackEventsAO->SetActive();
                }
            }
        }
    }

TInt CSGEAudioProviderInternal::GPAONotifierError( CSGEAudioGPAO* /*aGPAO*/, TInt /*aError*/ )
    {
    return KErrNone;
    }

