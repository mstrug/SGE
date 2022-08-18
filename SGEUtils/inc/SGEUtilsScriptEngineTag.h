/*
 ============================================================================
  Name        : SGEUtilsScriptEngineTag.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#ifndef __SGEUTILSSCRIPTENGINETAG_H__
#define __SGEUTILSSCRIPTENGINETAG_H__

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <e32hashtab.h> 
#include <GDI.H> 


// FORWARD DECLARATIONS

// CLASS DECLARATION


/**
 * TSGEUtilsScriptEngineTag
 *
 */
class TSGEUtilsScriptEngineTag
	{

public:

	IMPORT_C TSGEUtilsScriptEngineTag( TInt aTagId, TPtrC8 aValue );

	IMPORT_C TUint32 TagId() const;

	IMPORT_C TBool IsValid();

	IMPORT_C TBool IsEmpty();

	IMPORT_C TUint32 ValueAsTUint32();

	IMPORT_C TInt32 ValueAsTInt32();

	/**
	 * To save real number use following real format:
	 *  TRealFormat rf( 10, 4 );
     *  rf.iPoint = '.';
     *  rf.iTriLen = 0;
	 */
	IMPORT_C TReal ValueAsTReal();

	IMPORT_C TBool ValueAsTBool();

	IMPORT_C TPoint ValueAsTPoint();

	IMPORT_C TSize ValueAsTSize();

	IMPORT_C TRect ValueAsTRect();

	IMPORT_C TRgb ValueAsTRgb();

	IMPORT_C TBool ValueAsTUint32List( RArray< TUint32 > &aArray );

	IMPORT_C TPtrC8 Value();

	IMPORT_C void ValueAsRBufLC( RBuf& aBuf ); // creates copy

private: // data

	TUint32 iTagId;

	TPtrC8 iValue;

	};


#endif // __SGEUTILSSCRIPTENGINETAG_H__
