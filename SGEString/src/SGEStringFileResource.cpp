/*
 ============================================================================
 Name		: SGEStringFileResource.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEStringFileResource implementation
 ============================================================================
 */

#include "SGEStringFileResource.h"
#include "SGEStringFileItem.h"


CSGEStringFileResource::CSGEStringFileResource( RFs &aFs ) : iFs( aFs )
    {
    // No implementation required
    }

CSGEStringFileResource::~CSGEStringFileResource()
    {
    iFile.Close();
    }

CSGEStringFileResource* CSGEStringFileResource::NewLC( RFs &aFS, const TDesC& aFileName )
    {
    CSGEStringFileResource *self = new (ELeave) CSGEStringFileResource( aFS );
    CleanupStack::PushL( self );
    self->ConstructL( aFileName );
    return self;
    }

CSGEStringFileResource* CSGEStringFileResource::NewL( RFs &aFS, const TDesC& aFileName )
    {
    CSGEStringFileResource *self = CSGEStringFileResource::NewLC( aFS, aFileName );
    CleanupStack::Pop(); // self;
    return self;
    }

void CSGEStringFileResource::ConstructL( const TDesC& aFileName )
    {
    iFile.OpenL( iFs, aFileName );
    }

TPtrC CSGEStringFileResource::GetStringL( TInt aResourceId )
    {
    CSGEStringFileItem *item = iStringItems.Find( aResourceId );
    
    if ( !item )
        {
        item = CSGEStringFileItem::NewL( iFile, aResourceId );
        CleanupStack::PushL( item );
        
        TInt *id = new (ELeave) TInt;
        *id = aResourceId;
        CleanupStack::PushL( id );
        
        iStringItems.InsertL( id, item );
        
        CleanupStack::Pop( id );
        CleanupStack::Pop( item );
        }
    
    return item->String();
    }

TPtrC CSGEStringFileResource::GetString( TInt aResourceId )
    {
    CSGEStringFileItem *item = iStringItems.Find( aResourceId );
    
    if ( item )
        {
        return item->String();
        }
    else
        {
        return KNullDesC();
        }
    }


// end of file
