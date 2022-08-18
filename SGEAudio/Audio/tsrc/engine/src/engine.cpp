/*
 ============================================================================
 Name		: engine.cpp
 Author	  : Michal Strug
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : Exe source file
 ============================================================================
 */

//  Include Files  

#include "engine.h"
#include <e32base.h>
#include <e32std.h>
#include <e32cons.h>			// Console
#include "SGEAudioConfig.h"
#include "SGEAudioMixerMono.h"
#include "SGEAudioSoundTrack.h"
#include "SGEAudioSample.h"
#include "SGEAudioEngine.h"
#include <SGEAudio/SGEAudioProvider.h>

//  Constants

_LIT(KTextConsoleTitle, "Console");
_LIT(KTextFailed, " failed, leave code = %d");
_LIT(KTextPressAnyKey, " [press any key]\n");

//  Global Variables

LOCAL_D CConsoleBase* console; // write all messages to this

class CTestClass : public CBase, MSGEAudioEngineObserver, MSGEAudioProviderObserver
    {
public:

    void RunTestL()
        {
//        console->Write(_L("Test 1 start\n"));
//        Test1L();
//        console->Write(_L("Test 1 end\n"));
//
//        console->Write(_L("Test 2 start\n"));
//        Test2L();
//        console->Write(_L("Test 2 end\n"));

        console->Write(_L("Test 3 start\n"));
        Test3L();
        console->Write(_L("Test 3 end\n"));
        }

    
    CTestClass()
        {
        }
    
    ~CTestClass()
        {
        delete iEngine;
        delete iAudioProvider;
        }

private:
    
    void Test1L()
        {
        iConfig.iBufferSize = 512;
        iConfig.iSampleRate = ESGEAudioSampleRate16000Hz;
        iConfig.iChannels = ESGEAudioChannelsMono;
        iConfig.iBufferDataType = ESGEAudioBufferDataTypePCM8;
        iConfig.iSubsystem = ESGEAudioSubsystemMDA;
        
        iEngine = CSGEAudioEngine::NewL( *this, iConfig );
        
        iEngine->AddFileToLoad( TSGEAudioFile( _L("c:\\test1.wav"), 1, ESGEAudioGroupSound, ESGEAudioFileTypeWAV ) );
        
        iEngine->StartLoadingFilesL();
        
        CActiveScheduler::Start();
        
        delete iEngine;
        iEngine = NULL;

        }
    
    void Test2L()
        {
        iConfig.iBufferSize = 512;
        iConfig.iSampleRate = ESGEAudioSampleRate16000Hz;
        iConfig.iChannels = ESGEAudioChannelsMono;
        iConfig.iBufferDataType = ESGEAudioBufferDataTypePCM8;
        iConfig.iSubsystem = ESGEAudioSubsystemMDA;
        iConfig.iMultithreeded = ETrue;
    
        iAudioProvider = CSGEAudioProvider::NewL( this, iConfig );
        
        iAudioProvider->LoadFileL( TSGEAudioFile( _L("c:\\test1.wav"), 1, ESGEAudioGroupSound, ESGEAudioFileTypeWAV ) );
        
        CActiveScheduler::Start();
        
        delete iAudioProvider;
        iAudioProvider = NULL;
        }
    
    void Test3L()
        {
        iConfig.iBufferSize = 512;
        iConfig.iSampleRate = ESGEAudioSampleRate16000Hz;
        iConfig.iChannels = ESGEAudioChannelsMono;
        iConfig.iBufferDataType = ESGEAudioBufferDataTypePCM8;
        iConfig.iSubsystem = ESGEAudioSubsystemSimple;
        iConfig.iMultithreeded = EFalse;
    
        iAudioProvider = CSGEAudioProvider::NewL( this, iConfig );
        
        iAudioProvider->LoadFileL( TSGEAudioFile( _L("c:\\test1.wav"), 1, ESGEAudioGroupSound, ESGEAudioFileTypeWAV ) );
        
        CActiveScheduler::Start();
        
        delete iAudioProvider;
        iAudioProvider = NULL;
        }
    
private: // from MSGEAudioEngineObserver
    
    void AudioEngineFileLoaded( TInt aSoundSampleId, TInt aError )
        {
        console->Write(_L("Test log: AudioEngineFileLoaded\n"));
        }  
    
    void AudioEngineAllFilesLoaded()
        {
        console->Write(_L("Test log: AudioEngineAllFilesLoaded\n"));
        
        iEngine->CreateAudioStreamL();
        }
    
    void AudioEngineSoundTrackPlayStarted( TUint aTrackId )
        {
        console->Write(_L("Test log: AudioEngineSoundTrackPlayStarted\n"));
        }
    
    void AudioEngineSoundTrackPlayFinished( TUint aTrackId )
        {
        console->Write(_L("Test log: AudioEngineSoundTrackPlayFinished\n"));
        
        CActiveScheduler::Stop();
        }
    
    void AudioEngineStreamCreated( TInt aError )
        {
        console->Write(_L("Test log: AudioEngineStreamCreated\n"));
        
        iEngine->Play( 1, (TSGEAudioRepeat) 3 );
        }
    
    void AudioEngineStreamError( TInt aError )
        {
        console->Write(_L("Test log: AudioEngineStreamError\n"));
        
        CActiveScheduler::Stop();
        }

private: // from MSGEAudioProviderObserver
    
    void AudioFileLoaded( TInt aSoundSampleId, TInt aError )
        {
        console->Write(_L("Test log: AudioFileLoaded\n"));
        }
    
    void AudioAllFilesLoaded()
        {
        console->Write(_L("Test log: AudioAllFilesLoaded\n"));
        
        iAudioProvider->CreateAudioStreamL();
        }

    void AudioSoundTrackPlayStarted( TUint aTrackId )
        {
        console->Write(_L("Test log: AudioSoundTrackPlayStarted\n"));
        
        TSGEAudioTrackState state = iAudioProvider->TrackState( 0 );
        if ( state.iPaused )
            {
            console->Write(_L("Test log: paused\n"));
            }
        }

    void AudioSoundTrackPlayFinished( TUint aTrackId )
        {
        console->Write(_L("Test log: AudioSoundTrackPlayFinished\n"));

        cnt++;
        iAudioProvider->Play( 1, (TSGEAudioRepeat) 1 );
        
        if ( cnt == 2 )
        CActiveScheduler::Stop();
        }

    void AudioStreamCreated( TInt aError )
        {
        console->Write(_L("Test log: AudioStreamCreated\n"));
        
        iAudioProvider->Play( 1, (TSGEAudioRepeat) 1 );
        }

    void AudioStreamError( TInt aError ) 
        {
        console->Write(_L("Test log: AudioStreamError\n"));
        
        CActiveScheduler::Stop();
        }
    
private: // data
    
    TSGEAudioConfig iConfig;
    
    CSGEAudioEngine *iEngine;
    
    TInt iTestPass;
    
    CSGEAudioProvider *iAudioProvider;
    
    TInt cnt;
    };


//  Local Functions

LOCAL_C void MainL()
    {
    console->Write(_L("Test log:\n"));
    
    CTestClass *test = new (ELeave) CTestClass();
    test->RunTestL();
    
    delete test;

    console->Write(_L("\nAll ok\n"));
    }

LOCAL_C void DoStartL()
    {
    // Create active scheduler (to run active objects)
    CActiveScheduler* scheduler = new (ELeave) CActiveScheduler();
    CleanupStack::PushL(scheduler);
    CActiveScheduler::Install(scheduler);

    MainL();

    // Delete active scheduler
    CleanupStack::PopAndDestroy(scheduler);
    }

//  Global Functions

GLDEF_C TInt E32Main()
    {
    // Create cleanup stack
    __UHEAP_MARK;
    CTrapCleanup* cleanup = CTrapCleanup::New();

    // Create output console
    TRAPD(createError, console = Console::NewL(KTextConsoleTitle, TSize(
            KConsFullScreen, KConsFullScreen)));
    if (createError)
        {
        delete cleanup;
        return createError;
        }

    // Run application code inside TRAP harness, wait keypress when terminated
    TRAPD(mainError, DoStartL());
    if (mainError)
        console->Printf(KTextFailed, mainError);
    console->Printf(KTextPressAnyKey);
    console->Getch();

    delete console;
    delete cleanup;
    __UHEAP_MARKEND;
    return KErrNone;
    }

