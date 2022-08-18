/*
 ============================================================================
 Name		: secookie.cpp
 Author	  : Michal Strug
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : Exe source file
 ============================================================================
 */

//  Include Files  

#include "secookie.h"
#include <e32base.h>
#include <e32std.h>
#include <e32cons.h>			// Console
#include <f32file.h> 
#include <random.h> 			// random.lib 


//  Constants

_LIT(KTextConsoleTitle, "Console");
_LIT(KTextFailed, " failed, leave code = %d");
_LIT(KTextPressAnyKey, " [press any key]\n");

//  Global Variables

LOCAL_D CConsoleBase* console; // write all messages to this



//  Local Functions

LOCAL_C void MainL()
    {
    console->Write(_L("SGE Security Cookie Creator\n v0.1\n\n"));

    RFs fs;
    User::LeaveIfError( fs.Connect() );
    RFile file;
    if ( file.Replace( fs, _L("c:\\data\\secookie.dat"), EFileWrite ) != KErrNone )
        {
        fs.Close();
        console->Write(_L("File error\n"));
        return;
        }

    RBuf8 buf;
    buf.CreateMaxL( 1024 );

    TRandom::Random( buf );

    TUint32 crc = 0;
    Mem::Crc32( crc, buf.Ptr(), buf.Length() );

    file.Write( buf );
    file.Close();

    file.Replace( fs, _L("c:\\data\\secookie.txt"), EFileWrite );
    TBuf8<64> txt;
    txt.Copy( _L8("crc32: ") );
    txt.AppendNum( crc );
    file.Write( txt );
    file.Close();

    fs.Close();

    buf.Close();

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

