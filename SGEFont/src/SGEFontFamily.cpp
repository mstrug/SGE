/*
 ============================================================================
 Name		: SGEFontFamily.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEFontFamily implementation
 ============================================================================
 */

#include "SGEFontFamily.h"
#include "SGEFont.h"


CSGEFontFamily::CSGEFontFamily()
    {
    }

CSGEFontFamily::~CSGEFontFamily()
    {
    iFonts.ResetAndDestroy();
    }

void CSGEFontFamily::AddFontL( CSGEFont *aFont, TUint aFontSize )
    {
    ASSERT( aFont );

    CleanupStack::PushL( aFont );

    if ( iFonts.Find( aFontSize ) )
        {
        User::Leave( KErrAlreadyExists );
        }

    TUint *size = new (ELeave) TUint;
    *size = aFontSize;
    CleanupStack::PushL( size );

    iFonts.InsertL( size, aFont );

    CleanupStack::Pop( size );
    CleanupStack::Pop( aFont );
    }

EXPORT_C void CSGEFontFamily::RemoveFont( TUint aFontSize )
    {
    iFonts.Remove( &aFontSize );
    }

void CSGEFontFamily::RemoveFontAll()
    {
    iFonts.ResetAndDestroy();
    }

EXPORT_C CSGEFont* CSGEFontFamily::GetFont( TUint aFontSize ) const
    {
    return const_cast<CSGEFont*>( iFonts.Find( aFontSize ) );
    }


// end of file
