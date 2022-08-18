/*
 ============================================================================
 Name		: SGEFontProvider.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEFontProvider implementation
 ============================================================================
 */

#include "SGEFontProvider.h"
#include "SGEFontFamily.h"
#include "SGEFont.h"


CSGEFontProvider::CSGEFontProvider()
    {
    }

EXPORT_C CSGEFontProvider::~CSGEFontProvider()
    {
    iFontFamilies.ResetAndDestroy();
    }

EXPORT_C CSGEFontProvider* CSGEFontProvider::NewLC()
    {
    CSGEFontProvider *self = new (ELeave) CSGEFontProvider();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

EXPORT_C CSGEFontProvider* CSGEFontProvider::NewL()
    {
    CSGEFontProvider *self = CSGEFontProvider::NewLC();
    CleanupStack::Pop(); // self;
    return self;
    }

void CSGEFontProvider::ConstructL()
    {
    }

EXPORT_C void CSGEFontProvider::AddFontL( CSGEFont *aFont, TUint aFontFamilyId, TUint aFontSize )
    {
    ASSERT( aFont );

    CleanupStack::PushL( aFont );   

    CSGEFontFamily *ff = iFontFamilies.Find( aFontFamilyId );

    if ( !ff )
        {
        ff = AddFontFamilyL( aFontFamilyId );
        }
    
    CleanupStack::Pop( aFont );

    ff->AddFontL( aFont, aFontSize );
    }

EXPORT_C void CSGEFontProvider::AddFontLD( CSGEFont *aFont, TUint aFontFamilyId, TUint aFontSize )
    {
    CleanupStack::PushL( aFont );
    AddFontL( aFont, aFontFamilyId, aFontSize );
    CleanupStack::Pop( aFont );
    }

EXPORT_C CSGEFont* CSGEFontProvider::GetFont( TUint aFontFamilyId, TUint aFontSize ) const
    {
    const CSGEFontFamily *ff = iFontFamilies.Find( aFontFamilyId );

    if ( ff )
        {
        return ff->GetFont( aFontSize );
        }
    else    
        {
        return NULL;
        }
    }

EXPORT_C CSGEFontFamily* CSGEFontProvider::GetFontFamily( TUint aFontFamilyId ) const
    {
    return const_cast<CSGEFontFamily*>( iFontFamilies.Find( aFontFamilyId ) );
    }

EXPORT_C void CSGEFontProvider::RemoveFontFamily( TUint aFontFamilyId )
    {
    iFontFamilies.Remove( &aFontFamilyId );
    }

EXPORT_C void CSGEFontProvider::RemoveFontFamilyAll()
    {
    iFontFamilies.ResetAndDestroy();
    }

CSGEFontFamily* CSGEFontProvider::AddFontFamilyL( TUint aFontFamilyId )
    {
    if ( iFontFamilies.Find( aFontFamilyId ) )
        {
        User::Leave( KErrAlreadyExists );
        }

    TUint *id = new (ELeave) TUint;
    CleanupStack::PushL( id );
    *id = aFontFamilyId;

    CSGEFontFamily *ff = new (ELeave) CSGEFontFamily();
    CleanupStack::PushL( ff );

    iFontFamilies.InsertL( id, ff );

    CleanupStack::Pop( ff );
    CleanupStack::Pop( id );

    return ff;
    }


// end of file
