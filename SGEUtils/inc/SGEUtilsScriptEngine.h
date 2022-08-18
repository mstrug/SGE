/*
 ============================================================================
  Name        : SGEUtilsScriptEngine.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#ifndef __SGEUTILSSCRIPTENGINE_H__
#define __SGEUTILSSCRIPTENGINE_H__

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <e32hashtab.h> 
#include <f32file.h> 
#include <SGEUtils/SGEUtilsScriptEngineTag.h>


// FORWARD DECLARATIONS
class CSGEFile;

// CLASS DECLARATION

/**
 * TSGEUtilsScriptEngineTagDefinition
 *
 */
class TSGEUtilsScriptEngineTagDefinition
	{
public:
	TPtrC8 iName; // case insensitive

	TUint32 iId;
	};

typedef TSGEUtilsScriptEngineTagDefinition TSGEUtilsScriptEngineGroupDefinition;


/**
 * MSGEUtilsScriptEngineClient
 *
 */
class MSGEUtilsScriptEngineClient
	{
public:

	virtual void ScriptEnginePrepare() { };

	// RArray is closed by the Script Engine
	virtual TInt ScriptEngineGroup( TUint32 aGroupId, RArray< TSGEUtilsScriptEngineTag > aValue ) = 0;

	virtual void ScriptEngineCleanup() { };

	};

/**
 * MSGEUtilsScriptEngineObserver
 *
 */
class MSGEUtilsScriptEngineObserver
	{
public:

	virtual void ScriptEngineError( TInt aError ) = 0;

	virtual void ScriptEngineErrorClient( MSGEUtilsScriptEngineClient* aClient, TInt aError ) = 0;

	virtual void ScriptEngineParsingFinished() = 0;

	};

/**
 * CSGEUtilsScriptEngine
 *
 */
class CSGEUtilsScriptEngine : public CBase
	{
public: // Constructors and destructor

	/**
	 * Destructor.
	 */
	IMPORT_C ~CSGEUtilsScriptEngine();

	/**
	 * Two-phased constructor. Observer used only for async parser.
	 */
	IMPORT_C static CSGEUtilsScriptEngine* NewL( MSGEUtilsScriptEngineObserver* aObserver = NULL );

	/**
	 * Two-phased constructor. Observer used only for async parser.
	 */
	IMPORT_C static CSGEUtilsScriptEngine* NewLC( MSGEUtilsScriptEngineObserver* aObserver = NULL );

	IMPORT_C TInt RegisterClient( MSGEUtilsScriptEngineClient& aClient, const TSGEUtilsScriptEngineGroupDefinition* aGroupDefinition, const TSGEUtilsScriptEngineTagDefinition* aTagDefinition );

	IMPORT_C TInt RegisterClient( MSGEUtilsScriptEngineClient& aClient, RArray< TSGEUtilsScriptEngineGroupDefinition > aGroupDefinition, RArray< TSGEUtilsScriptEngineTagDefinition > aTagDefinition );

	IMPORT_C TInt UnregisterClient( MSGEUtilsScriptEngineClient& aClient );

	IMPORT_C TInt UnregisterAllClients();

	IMPORT_C void ParseFileL( TFileName aFileName, TBool aAsyncParse = EFalse );

	IMPORT_C void ParseFileL( CSGEFile& aSGEFile, TFileName aFileName, TBool aAsyncParse = EFalse );

	IMPORT_C void ParseBufferL( const TDesC8& aBuffer, TBool aAsyncParse = EFalse );
	
	IMPORT_C static TInt FindTag( RArray< TSGEUtilsScriptEngineTag >& aValues, TUint32 aTagId );

private: // construction

	CSGEUtilsScriptEngine( MSGEUtilsScriptEngineObserver* aObserver );

	void ConstructL();

	static TUint32 TPtrC8Hash( TPtrC8 const &aBuf );
	
	static TBool TPtrC8Ident( TPtrC8 const &aL, TPtrC8 const &aR );

	void ParseStartL();
	
	TInt Parse();

	static TInt AsyncParse( TAny *aParam ); 

	TInt ParseDocument( TLex8& aLex );

	TInt ParseComment( TLex8& aLex );

	TInt ParseGroupName( TLex8& aLex );

	TInt ParseGroupContent( TLex8& aLex );

	void TrimPostfixSpaces( TPtrC8& aPtr );

private: // data

	MSGEUtilsScriptEngineObserver* iObserver;

	RFs iFs;

	RBuf8 iData;

	TBool iAsyncParse;

	TBool iParseStarted;

	CAsyncCallBack* iAsyncCallback; // owned

	struct SHashValue
		{
		TUint32 iId;
		TUint32 iClientIdx;
		};

	struct SRegisteredClient
		{
		MSGEUtilsScriptEngineClient& iClient;
		RHashMap< TPtrC8, TUint32 > iRegisteredTags;
		};

	RArray< SRegisteredClient > iRegisteredClients;

	RHashMap< TPtrC8, SHashValue > iRegisteredGroups;

	enum TParserState
		{
		EParserUnknown,
		EParserDocument,
		EParserComment,
		EParserGroupName,
		EParserGroupContent,
		EParserGroupEnd,
		EParserTagName,
		EParserTagValue
		};

	TParserState iParserState;

	TLex8 iParserLex;

	TUint32 iParserCurrentGroupId;

	TInt iParserCurrentClientIdx;

	RArray< TSGEUtilsScriptEngineTag > iParserCurrentGroupTags;

	};

#endif // __SGEUTILSSCRIPTENGINE_H__
