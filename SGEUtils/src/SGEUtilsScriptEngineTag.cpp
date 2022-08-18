/*
 ============================================================================
  Name        : SGEUtilsScriptEngineTag.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

// INCLUDES
#include "SGEUtilsScriptEngineTag.h"



EXPORT_C TSGEUtilsScriptEngineTag::TSGEUtilsScriptEngineTag( TInt aTagId, TPtrC8 aValue ) : iTagId( aTagId ), iValue( aValue )
	{
	}

EXPORT_C TUint32 TSGEUtilsScriptEngineTag::TagId() const
	{
	return iTagId;
	}

EXPORT_C TBool TSGEUtilsScriptEngineTag::IsValid()
	{
	return iTagId > 0 && iValue.Length() >= 0;
	}

EXPORT_C TBool TSGEUtilsScriptEngineTag::IsEmpty()
	{
	return iValue.Length() == 0;
	}

EXPORT_C TUint32 TSGEUtilsScriptEngineTag::ValueAsTUint32()
	{
	TLex8 lex( iValue );
	TUint32 val;
	lex.SkipSpace();

	if ( iValue.Length() > 2 && iValue.Ptr()[ 0 ] == '0' && ( iValue.Ptr()[ 1 ] == 'x' || iValue.Ptr()[ 1 ] == 'X' ) )
		{
	    lex.Inc( 2 );
		if ( lex.Val( val, EHex ) == KErrNone )
			{
			return val;
			}
		}
	else if ( lex.Val( val, EDecimal ) == KErrNone )
		{
		return val;
		}

	return 0;
	}

EXPORT_C TInt32 TSGEUtilsScriptEngineTag::ValueAsTInt32()
	{
	TLex8 lex( iValue );
	TInt64 val;
	lex.SkipSpace();
	
    if ( iValue.Length() > 3 && iValue.Ptr()[ 0 ] == '-' && iValue.Ptr()[ 1 ] == '0' && ( iValue.Ptr()[ 2 ] == 'x' || iValue.Ptr()[ 2 ] == 'X' ) )
        {
        lex.Inc( 3 );
        if ( lex.Val( val, EHex ) == KErrNone )
            {
            return -(TInt32)val;
            }
        }
    else if ( iValue.Length() > 2 && iValue.Ptr()[ 0 ] == '0' && ( iValue.Ptr()[ 1 ] == 'x' || iValue.Ptr()[ 1 ] == 'X' ) )
        {
        lex.Inc( 2 );
        if ( lex.Val( val, EHex ) == KErrNone )
            {
            return (TInt32)val;
            }
        }
    else if ( lex.Val( val, EDecimal ) == KErrNone )
        {
        return (TInt32)val;
        }

	return 0;
	}

EXPORT_C TReal TSGEUtilsScriptEngineTag::ValueAsTReal()
	{
	TLex8 lex( iValue );
	TReal val;
	lex.SkipSpace();

	if ( lex.Val( val, '.' ) == KErrNone )
		{
		return val;
		}

	return 0;
	}

EXPORT_C TBool TSGEUtilsScriptEngineTag::ValueAsTBool()
	{
	return ( iValue.Length() == 1 && iValue[ 0 ] != '0' );
	}

EXPORT_C TPoint TSGEUtilsScriptEngineTag::ValueAsTPoint()
	{
	TInt c1 = iValue.Locate( ',' );
	TInt32 val1, val2;

	if ( c1 == KErrNotFound )
		{
		return TPoint();
		}

	TLex8 lex( iValue.Left( c1 ) );
	lex.SkipSpace();

	if ( lex.Val( val1 ) != KErrNone )
		{
		return TPoint();
		}

	lex.Assign( iValue.Right( iValue.Length() - c1 - 1 ) );
	lex.SkipSpace();

	if ( lex.Val( val2 ) != KErrNone )
		{
		return TPoint();
		}

	return TPoint( val1, val2 );
	}

EXPORT_C TSize TSGEUtilsScriptEngineTag::ValueAsTSize()
	{
	TPoint p = ValueAsTPoint();
	return TSize( p.iX, p.iY );
	}

EXPORT_C TRect TSGEUtilsScriptEngineTag::ValueAsTRect()
	{
	TInt32 val[ 4 ];

	TPtrC8 ptr( iValue );
	for ( TInt i = 0; i < 3; i++ )
		{
		TInt idx = ptr.Locate( ',' );
		if ( idx == KErrNotFound )
			{
			return TRect();
			}

		TLex8 lex( ptr.Left( idx ) );
		lex.SkipSpace();

		if ( lex.Val( val[ i ] ) != KErrNone )
			{
			return TRect();
			}
		
		ptr.Set( ptr.Right( ptr.Length() - idx - 1 ) );
		}

	TLex8 lex( ptr );
	lex.SkipSpace();

	if ( lex.Val( val[ 3 ] ) != KErrNone )
		{
		return TRect();
		}

	return TRect( TPoint( val[ 0 ], val[ 1 ] ), TSize( val[ 2 ], val[ 3 ] ) );
	}

EXPORT_C TRgb TSGEUtilsScriptEngineTag::ValueAsTRgb()
	{
	TInt32 val[ 4 ], i;

	TPtrC8 ptr( iValue );
	for ( i = 0; i < 3; i++ )
		{
		TInt idx = ptr.Locate( ',' );
		if ( idx == KErrNotFound )
			{
			if ( i == 2 )
				{ // RGB specifield only
				val[ 3 ] = 255;
				break;
				}
			return TRgb( 0xffffff );
			}

		TLex8 lex( ptr.Left( idx ) );
		lex.SkipSpace();
		
		if ( lex.Val( val[ i ] ) != KErrNone )
			{
			return TRgb( 0xffffff );
			}
		
		ptr.Set( ptr.Right( ptr.Length() - idx - 1 ) );
		}

	TLex8 lex( ptr );
	lex.SkipSpace();

	if ( lex.Val( val[ i ] ) != KErrNone )
		{
		return TRgb( 0xffffff );
		}

	return TRgb( val[ 0 ], val[ 1 ], val[ 2 ], val[ 3 ] );
	}

EXPORT_C TBool TSGEUtilsScriptEngineTag::ValueAsTUint32List( RArray< TUint32 > &aArray )
	{
	TPtrC8 ptr( iValue );
	TUint32 val;

	for ( TInt i = 0; i < 3; i++ )
		{
		TInt idx = ptr.Locate( ',' );
		if ( idx == KErrNotFound )
			{
			break;
			}

		TLex8 lex( ptr.Left( idx ) );
		lex.SkipSpace();
		
		if ( lex.Val( val, EDecimal ) != KErrNone )
			{
			break;
			}

		aArray.Append( val );
		
		ptr.Set( ptr.Right( ptr.Length() - idx - 1 ) );
		}

	TLex8 lex( ptr );
	lex.SkipSpace();

	if ( lex.Val( val, EDecimal ) != KErrNone )
		{
		return EFalse;
		}

	aArray.Append( val );

	return ETrue;
	}

EXPORT_C TPtrC8 TSGEUtilsScriptEngineTag::Value()
	{
	return iValue;
	}

EXPORT_C void TSGEUtilsScriptEngineTag::ValueAsRBufLC( RBuf& aBuf )
	{
	aBuf.CreateL( iValue.Length() );
	aBuf.Copy( iValue );
	aBuf.CleanupClosePushL();
	}


