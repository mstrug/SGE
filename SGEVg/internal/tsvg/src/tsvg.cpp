/*
 ============================================================================
 Name		: tsvg.cpp
 Author	  : Michal Strug
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : Exe source file
 ============================================================================
 */

//  Include Files  

#include "tsvg.h"
#include <e32base.h>
#include <e32std.h>
#include <e32cons.h>			// Console
#include "vgsvgimage.h"

//  Constants

_LIT(KTextConsoleTitle, "Console");
_LIT(KTextFailed, " failed, leave code = %d");
_LIT(KTextPressAnyKey, " [press any key]\n");

//  Global Variables

//LOCAL_D 
CConsoleBase* console; // write all messages to this

_LIT8(KSvgFile0, "<?xml version=\"1.0\" standalone=\"no\"?>\r\n<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"\r\n\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\r\n<svg width=\"4cm\" height=\"4cm\" viewBox=\"0 0 400 400\"\r\n     xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\r\n  <title>Example triangle01- simple example of a 'path'</title>\r\n  <desc>A path that draws a triangle</desc>\r\n  <rect x=\"1\" y=\"1\" width=\"398\" height=\"398\" fill=\"none\" stroke=\"blue\" />\r\n  <path d=\"M 100 100 L 300 100 L 200 300 z\" fill=\"red\" stroke=\"blue\" stroke-width=\"3\" />\r\n</svg>\r\n" );
_LIT8(KSvgFile1, "<?xml version=\"1.0\" standalone=\"no\"?>\r\n<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"\r\n\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\r\n<svg width=\"4cm\" height=\"4cm\" viewBox=\"0 0 400 400\"\r\n     xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\r\n  <title>Example triangle01- simple example of a 'path'</title>\r\n  <desc>A path that draws a triangle</desc>\r\n  <rect x=\"1\" y=\"1\" width=\"398\" height=\"398\" fill=\"none\" stroke=\"blue\" />\r\n  <path d=\" M100 100 L300,100 L 200 , 300 z \" fill=\"red\" stroke=\"blue\" stroke-width=\"3\" />\r\n</svg>\r\n" );
_LIT8(KSvgFile2, "<?xml version=\"1.0\" standalone=\"no\"?>\r\n<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"\r\n\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\r\n<svg width=\"37.5pt\" height=\"37.5pt\" viewBox=\"0 0 37.5 37.5\"\r\n     xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\r\n  <title>Example triangle01- simple example of a 'path'</title>\r\n  <desc>A path that draws a triangle</desc>\r\n  <rect x=\"0.751\" y=\"0.75\" width=\"36\" height=\"36\" fill=\"#bf6200\" fill-opacity=\"0.654902\" stroke=\"none\" />\r\n  <path d=\"M 19.5,27 L 8.25,27 L 12.75,18 L 8.25,6.75 L 21.75,6.75 L 19.5,27 Z\" stroke=\"#ffffff\" stroke-opacity=\"0.772549\" stroke-width=\"0.75\" />\r\n</svg>\r\n" );


//  Local Functions

LOCAL_C void MainL()
	{
	//
	// add your program code here, example code below
	//
	console->Write(_L("Hello, world!\n"));
	
	CVgSvgImage* img = CVgSvgImage::NewL( KSvgFile2() );
	
	delete img;
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

