/*
 ============================================================================
  Name        : SGEUtilsScriptEngine.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

// INCLUDES
#include "SGEUtilsScriptEngine.h"
#include <e32debug.h>
#include <SGEFile/SGEFile.h>


CSGEUtilsScriptEngine::CSGEUtilsScriptEngine( MSGEUtilsScriptEngineObserver* aObserver ) : iObserver( aObserver ), iRegisteredGroups( &TPtrC8Hash, &TPtrC8Ident )
	{
	}

EXPORT_C CSGEUtilsScriptEngine::~CSGEUtilsScriptEngine()
	{
	if ( iAsyncCallback )
		{
		iAsyncCallback->Cancel();
		delete iAsyncCallback;
		}
	iFs.Close();
	iData.Close();
	iRegisteredGroups.Close();
	iParserCurrentGroupTags.Close();
	
	for ( TInt i = 0; i < iRegisteredClients.Count(); i++ )
		{
		iRegisteredClients[ i ].iRegisteredTags.Close();
		}	
	iRegisteredClients.Close();
	}

EXPORT_C CSGEUtilsScriptEngine* CSGEUtilsScriptEngine::NewL( MSGEUtilsScriptEngineObserver* aObserver )
	{
	CSGEUtilsScriptEngine* self = CSGEUtilsScriptEngine::NewLC( aObserver );
	CleanupStack::Pop(); // self;
	return self;
	}

EXPORT_C CSGEUtilsScriptEngine* CSGEUtilsScriptEngine::NewLC( MSGEUtilsScriptEngineObserver* aObserver )
	{
	CSGEUtilsScriptEngine* self = new ( ELeave ) CSGEUtilsScriptEngine( aObserver );
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

void CSGEUtilsScriptEngine::ConstructL()
	{
	User::LeaveIfError( iFs.Connect() );
	iAsyncCallback = new (ELeave) CAsyncCallBack( CActive::EPriorityStandard );
	}

TUint32 CSGEUtilsScriptEngine::TPtrC8Hash( TPtrC8 const &aBuf )
	{
	return DefaultHash::Des8(aBuf);
	}
TBool CSGEUtilsScriptEngine::TPtrC8Ident( TPtrC8 const &aL, TPtrC8 const &aR )
	{
	return DefaultIdentity::Des8(aL, aR);
	} 

EXPORT_C TInt CSGEUtilsScriptEngine::RegisterClient( MSGEUtilsScriptEngineClient& aClient, const TSGEUtilsScriptEngineGroupDefinition* aGroupDefinition, const TSGEUtilsScriptEngineTagDefinition* aTagDefinition )
	{
	TInt i;
	TInt idx = KErrNotFound;
	for ( i = 0; i < iRegisteredClients.Count(); i++ )
		{
		if ( &iRegisteredClients[ i ].iClient == &aClient )
			{
			idx = i;
			break;
			}
		}
	if ( idx == KErrNotFound )
		{
		idx = iRegisteredClients.Count();
		SRegisteredClient src = { aClient, RHashMap< TPtrC8, TUint32 >( &TPtrC8Hash, &TPtrC8Ident ) };
		TInt err = iRegisteredClients.Append( src ); 
		if ( err != KErrNone )
			{
			return err;
			}
		}
	i = 0;
	if ( aGroupDefinition )
		{
		while ( aGroupDefinition[ i ].iId != 0 )
			{
			SHashValue s = { aGroupDefinition[ i ].iId, idx };
			TInt err = iRegisteredGroups.Insert( aGroupDefinition[ i ].iName, s );
			if ( err != KErrNone )
				{
				return err;
				}
			i++;
			}
		}
	i = 0;
	if ( aTagDefinition )
		{
		while ( aTagDefinition[ i ].iId != 0 )
			{
			TInt err = iRegisteredClients[ idx ].iRegisteredTags.Insert( aTagDefinition[ i ].iName, aTagDefinition[ i ].iId );
			if ( err != KErrNone )
				{
				return err;
				}
			i++;
			}
		}
	return KErrNone;
	}

EXPORT_C TInt CSGEUtilsScriptEngine::RegisterClient( MSGEUtilsScriptEngineClient& aClient, RArray< TSGEUtilsScriptEngineGroupDefinition > aGroupDefinition, RArray< TSGEUtilsScriptEngineTagDefinition > aTagDefinition )
	{
	TInt idx = KErrNotFound;
	for ( TInt i = 0; i < iRegisteredClients.Count(); i++ )
		{
		if ( &iRegisteredClients[ i ].iClient == &aClient )
			{
			idx = i;
			break;
			}
		}
	if ( idx == KErrNotFound )
		{
		idx = iRegisteredClients.Count();
		SRegisteredClient src = { aClient, RHashMap< TPtrC8, TUint32 >( &TPtrC8Hash, &TPtrC8Ident ) };
		TInt err = iRegisteredClients.Append( src ); 
		if ( err != KErrNone )
			{
			return err;
			}
		}
	
	for ( TInt i = 0; i < aGroupDefinition.Count(); i++ )
			{
			SHashValue s = { aGroupDefinition[ i ].iId, idx };
			TInt err = iRegisteredGroups.Insert( aGroupDefinition[ i ].iName, s );
			if ( err != KErrNone )
				{
				aGroupDefinition.Close();
				return err;
				}
			}

	for ( TInt i = 0; i < aTagDefinition.Count(); i++ )
		{
		TInt err = iRegisteredClients[ idx ].iRegisteredTags.Insert( aTagDefinition[ i ].iName, aTagDefinition[ i ].iId );
		if ( err != KErrNone )
			{
			aTagDefinition.Close();
			return err;
			}
		}
	return KErrNone;
	}
	
EXPORT_C TInt CSGEUtilsScriptEngine::UnregisterClient( MSGEUtilsScriptEngineClient& aClient )
	{
	for ( TInt i = 0; i < iRegisteredClients.Count(); i++ )
		{
		if ( &iRegisteredClients[ i ].iClient == &aClient )
			{
			iRegisteredClients[ i ].iRegisteredTags.Close();
			iRegisteredClients.Remove( i );
			
			//RHashMap< TPtrC8, SHashValue >::TIter iter;
			THashMapIter< TPtrC8, SHashValue > iterator( iRegisteredGroups );
			iterator.Reset();
			// todo: iterator.NextValue();
			SHashValue *val = iterator.CurrentValue();
			while ( val )
				{
				if ( val->iClientIdx == i )
					{
					iterator.RemoveCurrent();
					}
				iterator.NextKey();
				val = iterator.CurrentValue();
				}
			
			return KErrNone;
			}
		}	
	return KErrNotFound;
	}

EXPORT_C TInt CSGEUtilsScriptEngine::UnregisterAllClients()
	{
	for ( TInt i = 0; i < iRegisteredClients.Count(); i++ )
		{
		iRegisteredClients[ i ].iRegisteredTags.Close();
		}	
	iRegisteredClients.Reset();
	iRegisteredGroups.Close();
	return KErrNone;
	}

EXPORT_C TInt CSGEUtilsScriptEngine::FindTag( RArray< TSGEUtilsScriptEngineTag >& aValues, TUint32 aTagId )
	{
	for ( TInt i = 0; i < aValues.Count(); i++ )
		{
		if ( aValues[ i ].TagId() == aTagId )
			{
			return  i;
			}
		}
	return KErrNotFound;
	}

void CSGEUtilsScriptEngine::ParseFileL( TFileName aFileName, TBool aAsyncParse )
	{
	if ( iParseStarted || ( !iObserver && aAsyncParse ) )
		{
		User::Leave( KErrNotReady );
		}
	iAsyncParse = aAsyncParse;
	
	RFile file;
	User::LeaveIfError( file.Open( iFs, aFileName, EFileRead ) );
	CleanupClosePushL( file );

	TInt size = 0;
	User::LeaveIfError( file.Size( size ) );

	iData.Close();
	User::LeaveIfError( iData.Create( size ) );

	User::LeaveIfError( file.Read( iData ) );

	ParseStartL();

	CleanupStack::PopAndDestroy( &file );
	}

EXPORT_C void CSGEUtilsScriptEngine::ParseFileL( CSGEFile& aSGEFile, TFileName aFileName, TBool aAsyncParse )
	{
	if ( iParseStarted || ( !iObserver && aAsyncParse ) )
		{
		User::Leave( KErrNotReady );
		}
	iAsyncParse = aAsyncParse;

	TInt size;
	RReadStream file = aSGEFile.OpenFileL( aFileName, &size );
	file.PushL();
	iData.Close();
	iData.CreateL( size );
	file.ReadL( iData, size );
	CleanupStack::PopAndDestroy( &file );

	ParseStartL();
	}

EXPORT_C void CSGEUtilsScriptEngine::ParseBufferL( const TDesC8& aBuffer, TBool aAsyncParse )
	{
	if ( iParseStarted || ( !iObserver && aAsyncParse ) )
		{
		User::Leave( KErrNotReady );
		}
	iAsyncParse = aAsyncParse;

	iData.Close();
	iData.CreateL( aBuffer );

	ParseStartL();	
	}

void CSGEUtilsScriptEngine::ParseStartL()
	{
	if ( iParseStarted || ( !iObserver && iAsyncParse ) )
		{
		User::Leave( KErrNotReady );
		}

	iParserLex.Assign( iData );

	for ( TInt i = 0; i < iRegisteredClients.Count(); i++ )
		{
		iRegisteredClients[ i ].iClient.ScriptEnginePrepare();
		}	

	iParseStarted = ETrue;
	if ( iAsyncParse )
		{
		iParserState = EParserUnknown;
		if ( iAsyncCallback->IsActive() )
			{
			User::Leave( KErrNotReady ); // @todo: change error
			}
		TCallBack cb( AsyncParse, (TAny*) this );
		iAsyncCallback->Set( cb );
		iAsyncCallback->CallBack();
		}
	else
		{
		iParserState = EParserUnknown;
		TInt err = Parse();
		iParseStarted = EFalse;

		for ( TInt i = 0; i < iRegisteredClients.Count(); i++ )
			{
			iRegisteredClients[ i ].iClient.ScriptEngineCleanup();
			}

		User::LeaveIfError( err );
		}
	}

void CSGEUtilsScriptEngine::TrimPostfixSpaces( TPtrC8& aPtr )
	{
	for ( TInt i = aPtr.Length() - 1; i >= 0; i-- )
		{
		if ( aPtr[ i ] != ' ' && aPtr[ i ] != '\t' )
			{
			if( i == aPtr.Length() - 1 )
				{
				break;
				}
			aPtr.Set( aPtr.Ptr(), i + 1 );
			break;
			}
		}
	}

TInt CSGEUtilsScriptEngine::ParseDocument( TLex8& aLex )
	{
	while ( !aLex.Eos() )
		{
		aLex.SkipSpace();

		switch ( aLex.Peek() )
			{
			case '#':
				iParserState = EParserComment;
				return KErrNone;
			case '[':
				iParserState = EParserGroupName;
				return KErrNone;
			case '\r':
			case '\n':
				aLex.Get();
				break;
			default:
				return KErrGeneral;
			}
		}
	return KErrNone;
	}

TInt CSGEUtilsScriptEngine::ParseComment( TLex8& aLex )
	{
	aLex.Mark();
	while ( aLex.Get() != '\n' && !aLex.Eos() ) ;
	RDebug::RawPrint( aLex.MarkedToken() );
	iParserState = EParserDocument;
	return KErrNone;
	}

TInt CSGEUtilsScriptEngine::ParseGroupName( TLex8& aLex )
	{
	aLex.Get(); // [
	aLex.SkipSpace();
	aLex.Mark();
	while ( aLex.Get() != ']' && !aLex.Eos() ) ;
	aLex.UnGet(); // ]
	TPtrC8 groupName( aLex.MarkedToken() );

	TrimPostfixSpaces( groupName );

	SHashValue* val = iRegisteredGroups.Find( groupName );
	if ( val )
		{
		iParserCurrentGroupId = val->iId;
		iParserCurrentClientIdx = val->iClientIdx;

		RDebug::Printf( "Found group id: %d   client idx: %d", iParserCurrentGroupId, iParserCurrentClientIdx );
		}

	aLex.Get(); // ]
	aLex.SkipSpace();
	iParserState = EParserGroupContent;
	return KErrNone;
	}

TInt CSGEUtilsScriptEngine::ParseGroupContent( TLex8& aLex )
	{
	while ( !aLex.Eos() )
		{
		aLex.SkipSpace();

		TChar ch = aLex.Peek();
		if ( ch == 0 )
			{
			iParserState = EParserGroupEnd;
			return KErrNone;
			}
		else if ( ch == '[' )
			{ // next group
			iParserState = EParserGroupEnd;
			return KErrNone;
			}
		else if ( ch == '#' )
			{ // comment
			TParserState tmp = iParserState;
			ParseComment( aLex );
			iParserState = tmp;
			}
		else
			{
			iParserState = EParserTagName;

			aLex.SkipSpace();
			aLex.Mark();
			while ( aLex.Get() != '=' && !aLex.Eos() ) ;
			aLex.UnGet();
			TPtrC8 tagName( aLex.MarkedToken() );

			TrimPostfixSpaces( tagName );

			TUint32* val = iRegisteredClients[ iParserCurrentClientIdx ].iRegisteredTags.Find( tagName );
			if ( val )
				{
				RDebug::Printf( "Found tag id: %d ", *val );
				}

			aLex.Get(); // = 

			iParserState = EParserTagValue;

			aLex.Mark();
			while ( aLex.Get() == ' ' && !aLex.Eos() ) aLex.Mark();
			while ( aLex.Get() != '\n' && !aLex.Eos() ) ;
			if ( !aLex.Eos() )
			    {
			    aLex.UnGet(); // \n
			    aLex.UnGet(); // \r ?
			    if ( aLex.Peek() != '\r' )
			        {
			        aLex.Get();
			        }
			    }

			if ( val )
				{
				TPtrC8 tagValue( aLex.MarkedToken() ); // todo: save

				TrimPostfixSpaces( tagValue );

				RDebug::RawPrint( tagValue );

				TSGEUtilsScriptEngineTag tag( *val, tagValue );

				iParserCurrentGroupTags.Append( tag );
				}

			aLex.SkipSpace();

			iParserState = EParserGroupContent;

			return KErrNone;
			}		
		}

	return KErrNone;
	}

TInt CSGEUtilsScriptEngine::Parse()
	{
	while ( !iParserLex.Eos() )
		{
		RDebug::Printf( "ParseBuffer: %d ", iParserState );
		
		switch ( iParserState )
			{
			case EParserUnknown:
					iParserState = EParserDocument;
					iParserCurrentGroupId = 0;
					iParserCurrentClientIdx = KErrNotFound;
					iParserCurrentGroupTags.Reset();
				break;
			case EParserDocument:
					ParseDocument( iParserLex );
				break;
			case EParserComment:
					ParseComment( iParserLex );
				break;
			case EParserGroupName:
					ParseGroupName( iParserLex );
				break;
			case EParserGroupContent:
					ParseGroupContent( iParserLex );
				break;
			case EParserGroupEnd:
					iParserState = EParserDocument;

					if ( iParserCurrentGroupId && iParserCurrentClientIdx > KErrNotFound )
						{
						TInt err = iRegisteredClients[ iParserCurrentClientIdx ].iClient.ScriptEngineGroup( iParserCurrentGroupId, iParserCurrentGroupTags );
						if ( err != KErrNone && iObserver )
							{
							iObserver->ScriptEngineErrorClient( &iRegisteredClients[ iParserCurrentClientIdx ].iClient, err );
							}
						}

					iParserCurrentGroupId = 0;
					iParserCurrentClientIdx = KErrNotFound;
					iParserCurrentGroupTags.Reset();
					
					if ( iAsyncParse )
						{
						if ( iAsyncCallback->IsActive() )
							{
							// @todo: error
							}
						TCallBack cb( AsyncParse, (TAny*) this );
						iAsyncCallback->Set( cb );
						iAsyncCallback->CallBack();
						return KErrNone;
						}					
				break;
			case EParserTagName:
			case EParserTagValue:
					return KErrGeneral;
				break;
			}
		}

	if ( iParserCurrentGroupId && iParserCurrentClientIdx > KErrNotFound )
		{
		TInt err = iRegisteredClients[ iParserCurrentClientIdx ].iClient.ScriptEngineGroup( iParserCurrentGroupId, iParserCurrentGroupTags );
		if ( err != KErrNone && iObserver )
			{
			iObserver->ScriptEngineErrorClient( &iRegisteredClients[ iParserCurrentClientIdx ].iClient, err );
			}
		}
	iParserCurrentGroupId = 0;
	iParserCurrentClientIdx = KErrNotFound;
	iParserCurrentGroupTags.Reset();

	if ( iParserLex.Eos() && iAsyncParse )
		{
		iParseStarted = EFalse;
		iData.Close();

		for ( TInt i = 0; i < iRegisteredClients.Count(); i++ )
			{
			iRegisteredClients[ i ].iClient.ScriptEngineCleanup();
			}

		if ( iObserver )
			{
			iObserver->ScriptEngineParsingFinished();
			}
		}
	else if ( !iAsyncParse )
		{
		iParseStarted = EFalse;
		iData.Close();
		}

	return KErrNone;
	}

TInt CSGEUtilsScriptEngine::AsyncParse( TAny *aParam )
	{
	CSGEUtilsScriptEngine* self = (CSGEUtilsScriptEngine*) aParam;
	TRAPD( err, self->Parse() );
	if ( err && self->iObserver )
		{
		self->iObserver->ScriptEngineError( err );
		}
	return KErrNone;
	}

