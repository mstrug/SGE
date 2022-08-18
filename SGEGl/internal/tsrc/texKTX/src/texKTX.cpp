/*
 ============================================================================
 Name		: texKTX.cpp
 Author	  : 
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : Exe source file
 ============================================================================
 */

//  Include Files  

#include "texKTX.h"
#include <e32base.h>
#include <e32std.h>
#include <e32cons.h>			// Console
#include <sysutil.h> 
#include <SGEGl/GlTextureManager.h> 

//  Constants

_LIT(KTextConsoleTitle, "Console");
_LIT(KTextFailed, " failed, leave code = %d");
_LIT(KTextPressAnyKey, " [press any key]\n");

//  Global Variables

LOCAL_D CConsoleBase* console; // write all messages to this
RFile file;


class CMyTex : public CBase, MGlTextureManagerObserver
	{
public:
	static CMyTex* CreateL()
		{
		CMyTex *self = new (ELeave) CMyTex();
		self->iManager = CGlTextureManager::NewL( self );
		return self;
		}

	~CMyTex()
		{
		delete iManager;
		}

	void PrepareL()
		{
		iManager->AddImageToLoadL( _L("c:\\data\\tex1.ktx"), 1 );
		iManager->AddImageToLoadL( _L("c:\\data\\tex2.ktx"), 2 );
		iManager->AddImageToLoadL( _L("c:\\data\\tex3.png"), 3 );
		}

	void StartL()
		{
		iManager->StartLoadingImagesL();
		}

	void TextureCreated( TUint aTextureId, TInt aError )
		{
		TBuf8<100> buf;
		buf.Copy( _L8("TextureCreated  ") );
		buf.AppendNum( aTextureId );
		buf.Append( _L8("  ") );
		buf.AppendNum( aError );
		buf.Append( _L8("  \r\n") );
		file.Write( buf );
		console->Printf( _L("TextureCreated: %u %d\n"), aTextureId, aError  );
		}

	void TexturesCreatedAll()
		{
		file.Write( _L8("TexturesCreatedAll\r\n") );
		console->Printf( _L("TextureCreatedAll\n") );

		CActiveScheduler::Stop();
		}
	
private:

	CGlTextureManager* iManager;

	};

//  Local Functions

LOCAL_C void TestL()
	{
	CMyTex* tex = CMyTex::CreateL();

	file.Write( _L8("Created\r\n") );
	console->Write(_L("Created\n"));

	tex->PrepareL();

	file.Write( _L8("Prepared\r\n") );
	console->Write(_L("Prepared\n"));

	tex->StartL();

	file.Write( _L8("Started\r\n") );
	console->Write(_L("Started\n"));

	CActiveScheduler::Start();

	file.Write( _L8("Stopped\r\n") );
	console->Write(_L("Stopped\n"));

	delete tex;
	}


LOCAL_C void MainL()
	{
	//
	// add your program code here, example code below
	//

	RFs fs;
	fs.Connect();
	file.Replace( fs, _L("c:\\data\\tex.log"), EFileWrite );	

	console->Write(_L("texKTX:\n"));
	file.Write( _L8("texKTX:\r\n") );

	TRAPD( err, TestL() );
	if ( err )
		{
		TBuf8<100> buf;
		buf.Copy( _L8("TestL error  ") );
		buf.AppendNum( err );
		buf.Append( _L8(" \r\n") );
		file.Write( buf );
		}

	file.Write( _L8("End\r\n") );
	file.Close();
	fs.Close();
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

