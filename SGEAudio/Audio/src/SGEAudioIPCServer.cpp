/*
 ============================================================================
 Name		: SGEAudioIPCServer.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEAudioIPCServer implementation
 ============================================================================
 */

#include "SGEAudioIPCServer.h"
#include "SGEAudioIPCCommon.h"
#include "SGEAudioIPCClient.h"


CSGEAudioIPCServer::CSGEAudioIPCServer() : CServer2( EPriorityStandard )
    {
    // No implementation required
    }

CSGEAudioIPCServer::~CSGEAudioIPCServer()
    {
    delete iAudioEngine;
    }

void CSGEAudioIPCServer::CreateServerL( TSGEAudioConfig aAudioConfig,
                                        TThreadPriority aThreadPriority, 
                                        TInt aStackSize, 
                                        TInt aHeapMinSize )
    {
    // check if server is not started
    RSGEAudioIPCClient client;
    TInt err = client.Connect();
    if ( err == KErrNone )
        {
        client.Close();
        return;
        }
    else if ( err != KErrNotFound )
        {
        User::Leave( err );
        }
    client.Close();
    
    // start server
    RThread thr;
    if ( aHeapMinSize > (TInt) aAudioConfig.iHeapMaxSize )
        {
        User::Leave( KErrArgument );
        }
    User::LeaveIfError( thr.Create( KSGEAudioIPCServerThreadName(), ThreadFunction, aStackSize, 
                                    aHeapMinSize, aAudioConfig.iHeapMaxSize, (TAny*)&aAudioConfig ) );
    thr.SetPriority( aThreadPriority );

    RSemaphore semaphore;
    TInt errSem = semaphore.CreateGlobal( KSGEAudioIPCServerSemaphoreName, 0 );
    
    thr.Resume();
    thr.Close();
    
    if ( errSem == KErrNone )
        {
        const TInt KSemaphoreTimeout = 1000000; // 1 sec
        semaphore.Wait( KSemaphoreTimeout );
        semaphore.Close();
        }
    }

TInt CSGEAudioIPCServer::CloseServer()
    {
    RSGEAudioIPCClient client;
    TInt err = client.Connect();
    if ( err != KErrNone )
        {
        return err;
        }
    err = client.CloseServer();
    client.Close();
    return err;
    }

CSGEAudioIPCServer* CSGEAudioIPCServer::NewLC( TSGEAudioConfig aAudioConfig )
    {
    CSGEAudioIPCServer *self = new (ELeave) CSGEAudioIPCServer();
    CleanupStack::PushL( self );
    self->ConstructL( aAudioConfig );
    return self;
    }

CSGEAudioIPCServer* CSGEAudioIPCServer::NewL( TSGEAudioConfig aAudioConfig )
    {
    CSGEAudioIPCServer *self = CSGEAudioIPCServer::NewLC( aAudioConfig );
    CleanupStack::Pop(); // self;
    return self;
    }

void CSGEAudioIPCServer::ConstructL( TSGEAudioConfig aAudioConfig )
    {
    iAudioEngine = CSGEAudioEngine::NewL( *this, aAudioConfig );
    }

TInt CSGEAudioIPCServer::ThreadFunction( TAny* aParam )
    {
    CTrapCleanup* cleanupStack = CTrapCleanup::New();
    if ( !( cleanupStack ) )
        {
        PanicServer( KErrGeneral );
        }

    TSGEAudioConfig *config = (TSGEAudioConfig*)aParam;
    
    TRAPD( err, ThreadFunctionL( *config ) );
    if ( err != KErrNone )
        {
        PanicServer( KErrGeneral );
        }

    delete cleanupStack;
    cleanupStack = NULL;
    
    return KErrNone;    
    }

TInt CSGEAudioIPCServer::ThreadFunctionL( TSGEAudioConfig aAudioConfig )
    {
    CActiveScheduler* activeScheduler = new ( ELeave ) CActiveScheduler;
    CleanupStack::PushL( activeScheduler );
    CActiveScheduler::Install( activeScheduler );

    CSGEAudioIPCServer* server = CSGEAudioIPCServer::NewLC( aAudioConfig );
    server->StartL( KSGEAudioIPCServerName() );

    RSemaphore semaphore;
    if ( semaphore.OpenGlobal( KSGEAudioIPCServerSemaphoreName ) == KErrNone )
        {
        semaphore.Signal();
        semaphore.Close();
        }

    CActiveScheduler::Start();

    CleanupStack::PopAndDestroy( server );
    CleanupStack::PopAndDestroy( activeScheduler );
    
    return KErrNone;
    }

void CSGEAudioIPCServer::PanicServer( TInt aPanic )
    {
    User::Panic( KSGEAudioIPCServerPanic(), aPanic );
    }

CSession2* CSGEAudioIPCServer::NewSessionL( const TVersion &aVersion, const RMessage2& /*aMessage*/ ) const
    {
    if ( aVersion.iMajor != KSGEAudioIPCServerVersionMajor ||
         aVersion.iMinor != KSGEAudioIPCServerVersionMinor ||
         aVersion.iBuild != KSGEAudioIPCServerVersionBuild )
        {
        User::Leave( KErrNotSupported );
        }
    return CSGEAudioIPCSession::NewL( *const_cast< CSGEAudioIPCServer* >( this ), *iAudioEngine );
    }

void CSGEAudioIPCServer::SessionClosed()
    {
    iSessionIter.SetToFirst();
    TInt cnt = 0;
    while ( iSessionIter++ )
        {
        cnt++;
        }
    
    if ( cnt - 1 <= 0 ) // one session which calls this function
        {
        iAudioEngine->DestroyAudioStream();
        if ( iCloseRequested )
            {
            CActiveScheduler::Stop();
            }
        }
    }

void CSGEAudioIPCServer::SessionRequestedToCloseServer()
    {
    iCloseRequested = ETrue;
    }

void CSGEAudioIPCServer::AudioEngineFileLoaded( TInt aSoundSampleId, TInt aError )
    {
    iSessionIter.SetToFirst();
    CSGEAudioIPCSession* session = (CSGEAudioIPCSession*)iSessionIter++;
    
    while( session )
        {
        session->AudioEngineFileLoaded( aSoundSampleId, aError );
        session = (CSGEAudioIPCSession*)iSessionIter++;
        }
    }

void CSGEAudioIPCServer::AudioEngineAllFilesLoaded()
    {
    iSessionIter.SetToFirst();
    CSGEAudioIPCSession* session = (CSGEAudioIPCSession*)iSessionIter++;
    
    while( session )
        {
        session->AudioEngineAllFilesLoaded();
        session = (CSGEAudioIPCSession*)iSessionIter++;
        }
    }

void CSGEAudioIPCServer::AudioEngineSoundTrackPlayStarted( TUint aTrackId )
    {
    iSessionIter.SetToFirst();
    CSGEAudioIPCSession* session = (CSGEAudioIPCSession*)iSessionIter++;
    
    while( session )
        {
        session->AudioEngineSoundTrackPlayStarted( aTrackId );
        session = (CSGEAudioIPCSession*)iSessionIter++;
        }
    }

void CSGEAudioIPCServer::AudioEngineSoundTrackPlayFinished( TUint aTrackId )
    {
    iSessionIter.SetToFirst();
    CSGEAudioIPCSession* session = (CSGEAudioIPCSession*)iSessionIter++;
    
    while( session )
        {
        session->AudioEngineSoundTrackPlayFinished( aTrackId );
        session = (CSGEAudioIPCSession*)iSessionIter++;
        }
    }

void CSGEAudioIPCServer::AudioEngineStreamCreated( TInt aError )
    {
    iSessionIter.SetToFirst();
    CSGEAudioIPCSession* session = (CSGEAudioIPCSession*)iSessionIter++;
    
    while( session )
        {
        session->AudioEngineStreamCreated( aError );
        session = (CSGEAudioIPCSession*)iSessionIter++;
        }
    }

void CSGEAudioIPCServer::AudioEngineStreamError( TInt aError )
    {
    iSessionIter.SetToFirst();
    CSGEAudioIPCSession* session = (CSGEAudioIPCSession*)iSessionIter++;
    
    while( session )
        {
        session->AudioEngineStreamError( aError );
        session = (CSGEAudioIPCSession*)iSessionIter++;
        }
    }

