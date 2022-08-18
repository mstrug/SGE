/*
 ============================================================================
 Name		: wavPakDec.cpp
 Author	  : Michal Strug
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : Exe source file
 ============================================================================
 */

//  Include Files  

#include "wavPackDec.h"
#include <e32base.h>
#include <e32std.h>
#include <e32cons.h>			// Console


#include <SGE/SGEAudio/SGEAudioWavPackDecoder.h>


//  Constants

_LIT(KTextConsoleTitle, "Console");
_LIT(KTextFailed, " failed, leave code = %d");
_LIT(KTextPressAnyKey, " [press any key]\n");

//  Global Variables

LOCAL_D CConsoleBase* console; // write all messages to this


//  Local Functions


void Test1L()
{
	RFs fs;
	fs.Connect();
	RBuf8 data;
	CSGEAudioWavPackDecoder::TWaveInfo info;
	if ( CSGEAudioWavPackDecoder::DecodeFileL( fs, _L("c:\\wavPackTest.wv"), data, info ) )
		{
		RFile file;
		file.Replace( fs, _L("c:\\wavPackTest.out"), EFileWrite );
		file.Write( data );
		file.Close();
		data.Close();
		}
	fs.Close();
}

void Test2L()
{
	RFs fs;
	fs.Connect();
	RFile file;
	file.Open( fs, _L("c:\\wavPackTest.wv"), EFileRead );
	TInt size;
	file.Size( size );
	RBuf8 dataIn;
	dataIn.Create( size );
	file.Read( dataIn );
	file.Close();
	
	RBuf8 data;
	CSGEAudioWavPackDecoder::TWaveInfo info;
	if ( CSGEAudioWavPackDecoder::DecodeFileL( dataIn, data, info ) )
		{
		RFile file;
		file.Replace( fs, _L("c:\\wavPackTest.out"), EFileWrite );
		file.Write( data );
		file.Close();
		data.Close();
		}
	dataIn.Close();
	fs.Close();
}

LOCAL_C void MainL()
	{
	//
	// add your program code here, example code below
	//
	console->Write( _L("SGEAudioWavPackDecoder testing\n"));

	Test1L();
	Test2L();

	console->Write( _L("ok\n"));
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
		return createError;

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

