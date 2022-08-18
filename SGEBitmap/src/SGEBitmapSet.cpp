/*
 ============================================================================
 Name		: SGEBitmapSet.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEBitmapSet implementation
 ============================================================================
 */

#include "SGEBitmapSet.h"
#include "SGEBitmapItem.h"
#include <FBS.H>


// -----------------------------------------------------------------------------
// CSGEBitmapSet::CSGEBitmapSet()
// -----------------------------------------------------------------------------
//
CSGEBitmapSet::CSGEBitmapSet( TUint aBitmapSetId ) : iBitmapSetId( aBitmapSetId )
    {
    }

// -----------------------------------------------------------------------------
// CSGEBitmapSet::~CSGEBitmapSet()
// -----------------------------------------------------------------------------
//
CSGEBitmapSet::~CSGEBitmapSet()
    {
    iItems.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CSGEBitmapSet::Items()
// -----------------------------------------------------------------------------
//
RPointerArray< CSGEBitmapItem >& CSGEBitmapSet::Items()
    {
    return iItems;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapSet::Id()
// -----------------------------------------------------------------------------
//
TUint CSGEBitmapSet::Id() const
    {
    return iBitmapSetId;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapSet::GetBitmapItem()
// -----------------------------------------------------------------------------
//
CSGEBitmapItem* CSGEBitmapSet::GetBitmapItem( TUint aBitmapId ) const
    {
    TInt idx = iItems.Find( aBitmapId, ItemsCompare );
    if ( idx > KErrNotFound )
        {
        return iItems[ idx ];
        }
//    for ( TInt i = 0; i < iItems.Count(); i++ )
//        {
//        if ( iItems[ i ]->Id() == aBitmapId )
//            {
//            return iItems[ i ];
//            }
//        }
    return NULL;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapSet::RemoveBitmapItem()
// -----------------------------------------------------------------------------
//
void CSGEBitmapSet::RemoveBitmapItem( TUint aBitmapId )
    {
    for ( TInt i = 0; i < iItems.Count(); i++ )
        {
        if ( iItems[ i ]->Id() == aBitmapId )
            {
            iItems.Remove( i );
            return;
            }
        }
    }

// -----------------------------------------------------------------------------
// CSGEBitmapSet::ExternalizeL()
// -----------------------------------------------------------------------------
//
void CSGEBitmapSet::ExternalizeL( RWriteStream& aStream ) const
    {
    aStream.WriteUint32L( iBitmapSetId );
    aStream.WriteUint16L( iItems.Count() );
    
    for ( TInt i = 0; i < iItems.Count(); i++ )
        {
        aStream.WriteUint32L( iItems[ i ]->Id() );
        TUint8 cnt = ( iItems[ i ]->Bitmap() ? 0x1 : 0 ) | ( iItems[ i ]->BitmapMask() ? 0x2 : 0 );
        aStream.WriteUint8L( cnt );
        if ( iItems[ i ]->Bitmap() )
            {
            aStream << *iItems[ i ]->Bitmap();
            }
        if ( iItems[ i ]->BitmapMask() )
            {
            aStream << *iItems[ i ]->BitmapMask();
            }
        }
    }

// -----------------------------------------------------------------------------
// CSGEBitmapSet::InternalizeL()
// -----------------------------------------------------------------------------
//
void CSGEBitmapSet::InternalizeL( RReadStream& aStream )
    {
    iBitmapSetId = aStream.ReadUint32L();
    
    TInt itemsCnt = aStream.ReadUint16L();
    
    for ( TInt i = 0; i < itemsCnt; i++ )
        {
        TUint itemId = aStream.ReadUint32L();
        
        if ( iItems.Find( itemId, ItemsCompare ) > KErrNotFound )
            {
            User::Leave( KErrAlreadyExists );
            }
        
        TUint8 cnt = aStream.ReadUint8L();
        
        CFbsBitmap *bmp = NULL; 
        CFbsBitmap *bmpMask = NULL; 
        
        if ( cnt & 0x1 )
            {
            bmp = new (ELeave) CFbsBitmap();
            CleanupStack::PushL( bmp );
            aStream >> *bmp;
            CleanupStack::Pop( bmp );
            }
        if ( cnt & 0x2 )
            {
            CleanupStack::PushL( bmp );
            bmpMask = new (ELeave) CFbsBitmap();
            CleanupStack::PushL( bmpMask );
            aStream >> *bmpMask;
            CleanupStack::Pop( bmpMask );
            CleanupStack::Pop( bmp );
            }
        CleanupStack::PushL( bmp );
        CleanupStack::PushL( bmpMask );
        
        CSGEBitmapItem *item = new (ELeave) CSGEBitmapItem( bmp, bmpMask, itemId );
        
        CleanupStack::Pop( bmpMask );
        CleanupStack::Pop( bmp );
        
        CleanupStack::PushL( item );
        
        iItems.AppendL( item );
        
        CleanupStack::Pop( item );
        }
    }

// -----------------------------------------------------------------------------
// CSGEBitmapSet::ItemsCompare()
// -----------------------------------------------------------------------------
//
TBool CSGEBitmapSet::ItemsCompare( const TUint *aId, const CSGEBitmapItem &aItem )
    {
    return ( aItem.Id() == *aId );
    }


// end of file        
