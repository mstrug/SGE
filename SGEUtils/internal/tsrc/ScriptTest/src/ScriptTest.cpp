/*
 ============================================================================
 Name		: ScriptTest.cpp
 Author	  : Michal Strug
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : Exe source file
 ============================================================================
 */

//  Include Files  

#include "ScriptTest.h"
#include "SGEUtilsScriptEngineTag.h"
#include "SGEUtilsScriptEngine.h"
#include <e32base.h>
#include <e32std.h>
#include <e32cons.h>			// Console
#include <gdi.h>

//  Constants

_LIT(KTextConsoleTitle, "Console");
_LIT(KTextFailed, " failed, leave code = %d");
_LIT(KTextPressAnyKey, " [press any key]\n");

//  Global Variables

LOCAL_D CConsoleBase* console; // write all messages to this
LOCAL_D TInt testNumber;



const TSGEUtilsScriptEngineGroupDefinition tabg1[ 2 ] = { { _L8("group1"), 1 }, { KNullDesC8(), 0 } };
const TSGEUtilsScriptEngineTagDefinition tabt1[ 3 ] = { { _L8( "tag1" ), 1 }, { _L8( "tag2" ), 2 }, { KNullDesC8(), 0 } };

const TSGEUtilsScriptEngineGroupDefinition tabg2[ 3 ] = { { _L8("group1"), 1 }, { _L8("group2"), 2 }, { KNullDesC8(), 0 } };
const TSGEUtilsScriptEngineTagDefinition tabt2[ 5 ] = { { _L8( "tag1" ), 1 }, { _L8( "tag2" ), 2 }, { _L8( "tag3" ), 3 }, { _L8( "tag4" ), 4 }, { KNullDesC8(), 0 } };

const TSGEUtilsScriptEngineGroupDefinition tabg3[ 3 ] = { { _L8("group1"), 1 }, { _L8("group2"), 2 }, { KNullDesC8(), 0 } };
const TSGEUtilsScriptEngineTagDefinition tabt3[ 10 ] = { { _L8( "tag1" ), 1 }, { _L8( "tag2" ), 2 }, { _L8( "tag3" ), 3 }, { _L8( "tag4" ), 4 }, { _L8( "tag5" ), 5 }, { _L8( "tag6" ), 6 }, { _L8( "tag7" ), 7 }, { _L8( "tag8" ), 8 }, { _L8( "tag9" ), 999 }, { KNullDesC8(), 0 } };



class CEngineObserver: public MSGEUtilsScriptEngineObserver
	{
public:
	
	void ScriptEngineError( TInt aError )
		{
		console->Printf( _L("ScriptEngineError: %d\n"), aError);
		}

	void ScriptEngineErrorClient( MSGEUtilsScriptEngineClient* aClient, TInt aError )
		{
		console->Printf( _L("ScriptEngineErrorClient: %d\n"), aError);
		}

	void ScriptEngineParsingFinished()
		{
		console->Printf( _L("ScriptEngineParsingFinished\n") );
		CActiveScheduler::Stop();
		}
	};

class CTestObserver1: public MSGEUtilsScriptEngineClient
	{
	void ScriptEnginePrepare()
		{
		}
	void ScriptEngineCleanup()
		{
		}

	TInt ScriptEngineGroup( TUint32 aGroupId, RArray< TSGEUtilsScriptEngineTag > aValue )
		{
		ASSERT( testNumber == 1 );
		
		console->Printf( _L("TC[%d][obs1] Found group: %d  tags: %d\n"), testNumber, aGroupId, aValue.Count());
		
		ASSERT( aGroupId == 1 );
		ASSERT( aValue.Count() == 2 );
		ASSERT( aValue[ 0 ].TagId() == 1 );
		ASSERT( aValue[ 1 ].TagId() == 2 );
		ASSERT( aValue[ 0 ].Value().Compare( _L8("value1")) == 0 );
		ASSERT( aValue[ 1 ].Value().Compare( _L8("value2")) == 0 );
		
		for ( TInt i = 0; i < aValue.Count(); i++ )
			{
		    RBuf buf;
            TPtrC8 val = aValue[ i ].Value();
		    buf.Create( val.Length() + 1 );
            if ( val.Length() > 0 ) buf.Copy( val );
            else buf.Append( _L(" ") );
		    buf.Append( 0 );
			console->Printf( _L("     tag: %d  val: %S\n"), aValue[ i ].TagId(), buf.Ptr() );
			buf.Close();
			}		
		console->Printf( _L("\n") );
		
		return KErrNone;
		}
	};

class CTestObserver2: public MSGEUtilsScriptEngineClient
	{
public:
	void ScriptEnginePrepare()
		{
		}
	void ScriptEngineCleanup()
		{
		}

	TInt ScriptEngineGroup( TUint32 aGroupId, RArray< TSGEUtilsScriptEngineTag > aValue )
		{
		ASSERT( testNumber == 2 );

		console->Printf( _L("Found group: %d\n"), aGroupId);
		
		console->Printf( _L("TC[%d][obs1] Found group: %d  tags: %d\n"), testNumber, aGroupId, aValue.Count());
		
		ASSERT( aGroupId == 1 || aGroupId == 2 );
		
		if ( aGroupId == 1 )
			{
			ASSERT( aValue.Count() == 3 );
			ASSERT( aValue[ 0 ].TagId() == 1 );
			ASSERT( aValue[ 1 ].TagId() == 2 );
			ASSERT( aValue[ 2 ].TagId() == 3 );
			ASSERT( aValue[ 0 ].Value().Compare( _L8("value1")) == 0 );
			ASSERT( aValue[ 1 ].Value().Compare( _L8("value2")) == 0 );
			ASSERT( aValue[ 2 ].Value().Compare( _L8("value3")) == 0 );
			}
		else if ( aGroupId == 2 )
			{
			ASSERT( aValue.Count() == 4 );
			ASSERT( aValue[ 0 ].TagId() == 1 );
			ASSERT( aValue[ 1 ].TagId() == 2 );
			ASSERT( aValue[ 2 ].TagId() == 3 );
			ASSERT( aValue[ 3 ].TagId() == 4 );
			ASSERT( aValue[ 0 ].Value().Compare( _L8("value1")) == 0 );
			ASSERT( aValue[ 1 ].Value().Compare( _L8("value2")) == 0 );
			ASSERT( aValue[ 2 ].Value().Compare( _L8("")) == 0 );
			ASSERT( aValue[ 2 ].IsEmpty() );
			ASSERT( aValue[ 3 ].Value().Compare( _L8("v,.%5#@$alue 4")) == 0 );
			}
		
		for ( TInt i = 0; i < aValue.Count(); i++ )
			{
            RBuf buf;
            TPtrC8 val = aValue[ i ].Value();
            buf.Create( val.Length() + 2 );
            if ( val.Length() > 0 ) buf.Copy( val );
            else buf.Append( _L(" ") );
            buf.Append( 0 );
            console->Printf( _L("     tag: %d  val: %S\n"), aValue[ i ].TagId(), buf.Ptr() );
            buf.Close();
			}		
		console->Printf( _L("\n") );		
		
		return KErrNone;
		}
	};



class CTestObserver3: public MSGEUtilsScriptEngineClient
	{
public:
	void ScriptEnginePrepare()
		{
		}
	void ScriptEngineCleanup()
		{
		}

	TInt ScriptEngineGroup( TUint32 aGroupId, RArray< TSGEUtilsScriptEngineTag > aValue )
		{
		ASSERT( testNumber == 3 );

		console->Printf( _L("Found group: %d\n"), aGroupId);
		
		console->Printf( _L("TC[%d][obs1] Found group: %d  tags: %d\n"), testNumber, aGroupId, aValue.Count());
		
		ASSERT( aGroupId == 1 || aGroupId == 2 );
		
		if ( aGroupId == 1 )
			{
			ASSERT( aValue.Count() == 10 );
			ASSERT( aValue[ 0 ].TagId() == 1 );
			ASSERT( aValue[ 1 ].TagId() == 2 );
			ASSERT( aValue[ 2 ].TagId() == 3 );
			ASSERT( aValue[ 3 ].TagId() == 4 );
			ASSERT( aValue[ 4 ].TagId() == 5 );
			ASSERT( aValue[ 5 ].TagId() == 6 );
			ASSERT( aValue[ 6 ].TagId() == 7 );
			ASSERT( aValue[ 7 ].TagId() == 8 );
            ASSERT( aValue[ 8 ].TagId() == 8 );
            ASSERT( aValue[ 9 ].TagId() == 999 );
			
			ASSERT( aValue[ 0 ].ValueAsTUint32() == 123 );
			ASSERT( aValue[ 1 ].ValueAsTInt32() == -123 );
			ASSERT( aValue[ 2 ].ValueAsTPoint() == TPoint( 1, 2 ) );
			ASSERT( aValue[ 3 ].ValueAsTSize() == TSize( 1024, 512 ) );
			ASSERT( aValue[ 4 ].ValueAsTRect() == TRect( TPoint( 8,12 ), TSize( 100, 200 ) ) );
			ASSERT( !aValue[ 5 ].ValueAsTBool() );
			ASSERT( aValue[ 6 ].ValueAsTBool() );
			ASSERT( aValue[ 7 ].ValueAsTRgb() == TRgb( 255, 0, 255, 255 ) );
            ASSERT( aValue[ 8 ].ValueAsTRgb() == TRgb( 32, 255, 0, 128 ) );
            ASSERT( aValue[ 9 ].ValueAsTReal() == 1.42 );
			}
		else if ( aGroupId == 2 )
			{
			ASSERT( aValue.Count() == 3 );
			ASSERT( aValue[ 0 ].TagId() == 1 );
			ASSERT( aValue[ 1 ].TagId() == 1 );
			ASSERT( aValue[ 2 ].TagId() == 1 );

			ASSERT( aValue[ 0 ].Value().Compare( _L8("value1")) == 0 );
			ASSERT( aValue[ 1 ].Value().Compare( _L8("value2")) == 0 );
			ASSERT( aValue[ 2 ].Value().Compare( _L8("value2")) == 0 );
			}
		
		for ( TInt i = 0; i < aValue.Count(); i++ )
			{
            RBuf buf;
            TPtrC8 val = aValue[ i ].Value();
            buf.Create( val.Length() + 1 );
            if ( val.Length() > 0 ) buf.Copy( val );
            else buf.Append( _L(" ") );
            buf.Append( 0 );
            console->Printf( _L("     tag: %d  val: %S\n"), aValue[ i ].TagId(), buf.Ptr() );
            buf.Close();
			}		
		console->Printf( _L("\n") );		
		
		return KErrNone;
		}
	};


//  Local Functions


LOCAL_C void MainL()
	{
	//
	// add your program code here, example code below
	//
	console->Write(_L("Hello, world!\n"));
	
	CEngineObserver eobs;
	CTestObserver1 obs1;
	CTestObserver2 obs2;
	CTestObserver3 obs3;
	
	CSGEUtilsScriptEngine* se = CSGEUtilsScriptEngine::NewLC( &eobs );

	// test 1
	TInt err = se->RegisterClient( obs1, tabg1, tabt1 );
	testNumber = 1;
	se->ParseFileL( _L("c:\\data\\script_1.sgs") );
	se->ParseFileL( _L("c:\\data\\script_1.sgs"), ETrue );
	CActiveScheduler::Start();
	err = se->UnregisterClient( obs1 );
	
	// test 2
	err = se->RegisterClient( obs2, tabg2, tabt2 );
	testNumber = 2;
	se->ParseFileL( _L("c:\\data\\script_2.sgs") );
	se->ParseFileL( _L("c:\\data\\script_2.sgs"), ETrue );
	CActiveScheduler::Start();
	err = se->UnregisterClient( obs2 );	
	
	// test 3
	err = se->RegisterClient( obs3, tabg3, tabt3 );
	testNumber = 3;
	se->ParseFileL( _L("c:\\data\\script_3.sgs") );
	se->ParseFileL( _L("c:\\data\\script_3.sgs"), ETrue );
	CActiveScheduler::Start();
	err = se->UnregisterClient( obs3 );	
	
	CleanupStack::PopAndDestroy( se );
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
	console->Getch();

	delete console;
	delete cleanup;
	__UHEAP_MARKEND;
	return KErrNone;
	}

