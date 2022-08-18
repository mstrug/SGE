/*
 ============================================================================
 Name		: sgesecuritytest.cpp
 Author	  : Michal Strug
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : Exe source file
 ============================================================================
 */

//  Include Files  

#include "sgesecuritytest.h"
#include <e32base.h>
#include <e32std.h>
#include <e32cons.h>			// Console
#include <f32file.h> 
#include "sgesecurity.h"

//  Constants

_LIT(KTextConsoleTitle, "Console");
_LIT(KTextFailed, " failed, leave code = %d");
_LIT(KTextPressAnyKey, " [press any key]\n");

//  Global Variables

LOCAL_D CConsoleBase* console; // write all messages to this


//  Local Functions

LOCAL_C void MainL()
    {
    console->Write(_L("Test log:\n"));

    RBuf8 data;

    _LIT( KFile, "c:\\private\\EF4af310\\drm\\data\\cookie.dat" );

    SGESecurity::GetDrmFileL( data, KFile() );

    RFs fs;
    fs.Connect();
    RFile file;
    file.Replace( fs, _L("c:\\test_out.dat"), EFileWrite );
    file.Write( data );
    file.Close();
    fs.Close();

    ASSERT( data.Length() == 1024 );

    TUint32 crc = 0;
    Mem::Crc32( crc, data.Ptr(), data.Length() );

    ASSERT( crc == 1257420081 );

    console->Printf( _L("\nsize: %d  crc: %d\n"), data.Length(), crc );

    data.Close();

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

