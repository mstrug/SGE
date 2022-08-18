/*
 ============================================================================
 Name		: SGEStringFileItem.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEStringFileItem implementation
 ============================================================================
 */

#include "SGEStringFileItem.h"
#include <BaRsc.h>
#include <BaRsc2.h>


CSGEStringFileItem::CSGEStringFileItem( TInt aResourceId ) : iResourceId( aResourceId )
    {
    }

CSGEStringFileItem::~CSGEStringFileItem()
    {
    delete iBuffer;
    }

CSGEStringFileItem* CSGEStringFileItem::NewL( CResourceFile &aFile, TInt aResourceId )
    {
    CSGEStringFileItem *self = new (ELeave) CSGEStringFileItem( aResourceId );
    CleanupStack::PushL( self );
    self->ConstructL( aFile );
    CleanupStack::Pop( self );
    return self;
    }

CSGEStringFileItem* CSGEStringFileItem::NewL( RResourceFile &aFile, TInt aResourceId )
    {
    CSGEStringFileItem *self = new (ELeave) CSGEStringFileItem( aResourceId );
    CleanupStack::PushL( self );
    self->ConstructL( aFile );
    CleanupStack::Pop( self );
    return self;
    }

template<class T> void CSGEStringFileItem::ConstructL( T &aFile )
    {
    iBuffer = aFile.AllocReadL( iResourceId );
    iResourceReader.SetBuffer( iBuffer );
    iString.Set( iResourceReader.ReadTPtrC() );
    }

TInt CSGEStringFileItem::ResourceId() const
    {
    return iResourceId;
    }

TPtrC CSGEStringFileItem::String() const
    {
    return iString;
    }


// end of file
