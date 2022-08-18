/*
 ============================================================================
 Name		: wav.cpp
 Author	  : Michal Strug
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : Exe source file
 ============================================================================
 */

//  Include Files  

#include "wav.h"
#include <e32base.h>
#include <e32std.h>
#include <e32cons.h>			// Console
#include "SGEAudioFileLoader.h"
#include "SGEAudioFileDecoderWAV.h"

//  Constants

_LIT(KTextConsoleTitle, "Console");
_LIT(KTextFailed, " failed, leave code = %d");
_LIT(KTextPressAnyKey, " [press any key]\n");

//  Global Variables

LOCAL_D CConsoleBase* console; // write all messages to this

class CTestClass : public CBase, public MSGEAudioFileLoaderObserver
    {
public:

    void RunTestL()
        {
        console->Write(_L("Test 1 start\n"));
        Test1L();
        console->Write(_L("Test 1 end\n"));

        console->Write(_L("Test 2 start\n"));
        Test2L();
        console->Write(_L("Test 2 end\n"));
        }

    
    CTestClass()
        {
        iFs.Connect();
        }
    
    ~CTestClass()
        {
        delete iFileLoader;
        iFs.Close();
        }

private:
    
    void Test1L()
        {
        TSGEAudioConfig cfg;
        iFileLoader = CSGEAudioFileLoader::NewL( *this, iFs, cfg );
        
        ASSERT( iFileLoader );
        console->Write(_L("Test 1: created\n"));
        
        TInt err = iFileLoader->LoadFile( TSGEAudioFile( _L("c:\\test0.wav"), 1, ESGEAudioGroupAny, ESGEAudioFileTypeWAV ) );
        ASSERT( err == KErrNone );

        err = iFileLoader->LoadFile( TSGEAudioFile( _L("c:\\non_existing_file.wav"), 1, ESGEAudioGroupAny, ESGEAudioFileTypeWAV ) );
        ASSERT( err == KErrNone );

        console->Write(_L("Test 1: files added\n"));
        
        iTestPass = 1;
        iFileLoader->StartLoadingFilesL();

        console->Write(_L("Test 1: loading files started\n"));
        
        CActiveScheduler::Start();
        
        delete iFileLoader;
        iFileLoader = NULL;

        console->Write(_L("Test 1: deleted\n"));
        }
    
    void Test2L()
        {
        TSGEAudioConfig cfg;
        iFileLoader = CSGEAudioFileLoader::NewL( *this, iFs, cfg );
        
        ASSERT( iFileLoader );
        console->Write(_L("Test 2: created\n"));
        
        TInt err = iFileLoader->LoadFile( TSGEAudioFile( _L("c:\\test0.wav"), 1, ESGEAudioGroupAny, ESGEAudioFileTypeWAV ) );
        ASSERT( err == KErrNone );

        err = iFileLoader->LoadFile( TSGEAudioFile( _L("c:\\test0.wav"), 1, ESGEAudioGroupAny, ESGEAudioFileTypeWAV ) );
        ASSERT( err == KErrNone );

        console->Write(_L("Test 2: files added\n"));
        
        iTestPass = 3;
        iFileLoader->StartLoadingFilesL();

        console->Write(_L("Test 2: loading files started\n"));
        
        CActiveScheduler::Start();
        
        delete iFileLoader;
        iFileLoader = NULL;

        console->Write(_L("Test 2: deleted\n"));
        }
    
private: // from MSGEAudioFileLoaderObserver

    void AudioFileLoaded( CSGEAudioSample* aSoundSample, TUint aSampleId, TInt aError )
        {
        console->Write(_L("Test  : AudioFileLoaded called\n"));
        if ( iTestPass == 1 )
            {
            ASSERT( aError == KErrNone );
            ASSERT( aSoundSample );
            delete aSoundSample;
            
            iTestPass = 2;
            console->Write(_L("Test 1: pass 1\n"));
            }
        else if ( iTestPass == 2 )
            {
            ASSERT( aError == KErrNotFound );
            ASSERT( !aSoundSample );
            CActiveScheduler::Stop();

            console->Write(_L("Test 1: pass 2\n"));
            }
        else if ( iTestPass == 3 )
            {
            ASSERT( aError == KErrNone );
            ASSERT( aSoundSample );
            delete aSoundSample;

            console->Write(_L("Test 2: pass 3\n"));
            }
        else
            {
            ASSERT( EFalse );
            }
        }

    void AudioFileListEmpty()
        {
        console->Write(_L("Test  : AudioFileListEmpty called\n"));
        CActiveScheduler::Stop();
        }

private: // data
    
    RFs iFs;
    
    CSGEAudioFileLoader *iFileLoader;
    
    TInt iTestPass;
    
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

