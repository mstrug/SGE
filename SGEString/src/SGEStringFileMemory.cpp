/*
 ============================================================================
 Name		: SGEStringFileMemory.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEStringFileMemory implementation
 ============================================================================
 */

#include "SGEStringFileMemory.h"
#include "SGEStringFileItem.h"
#include <SGEFile/SGEFile.h>


CSGEStringFileMemory::CSGEStringFileMemory( RFs &aFs ) : iFs( aFs )
    {
    // No implementation required
    }

CSGEStringFileMemory::~CSGEStringFileMemory()
    {
    delete iFile;
    iFileData.Close();
    }

CSGEStringFileMemory* CSGEStringFileMemory::NewLC( RFs &aFS, CSGEFile &aSGEFile, const TDesC& aFileName )
    {
    CSGEStringFileMemory *self = new (ELeave) CSGEStringFileMemory( aFS );
    CleanupStack::PushL( self );
    self->ConstructL( aSGEFile, aFileName );
    return self;
    }

CSGEStringFileMemory* CSGEStringFileMemory::NewL( RFs &aFS, CSGEFile &aSGEFile, const TDesC& aFileName )
    {
    CSGEStringFileMemory *self = CSGEStringFileMemory::NewLC( aFS, aSGEFile, aFileName );
    CleanupStack::Pop(); // self;
    return self;
    }

void CSGEStringFileMemory::ConstructL( CSGEFile &aSGEFile, const TDesC& aFileName )
    {
    TInt size;
    RReadStream file = aSGEFile.OpenFileL( aFileName, &size );
    file.PushL();
    iFileData.CreateL( size );
    file.ReadL( iFileData, size );
    CleanupStack::PopAndDestroy( &file );

    iFile = CResourceFile::NewL( iFileData );
    }

TPtrC CSGEStringFileMemory::GetStringL( TInt aResourceId )
    {
    CSGEStringFileItem *item = iStringItems.Find( aResourceId );
    
    if ( !item )
        {
        item = CSGEStringFileItem::NewL( *iFile, aResourceId );
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

TPtrC CSGEStringFileMemory::GetString( TInt aResourceId )
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
