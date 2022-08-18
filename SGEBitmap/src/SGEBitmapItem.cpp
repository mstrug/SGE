/*
 ============================================================================
  Name        : SGEBitmapItem.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#include "SGEBitmapItem.h"
#include <FBS.H>


// -----------------------------------------------------------------------------
// CSGEBitmapItem::CSGEBitmapItem()
// -----------------------------------------------------------------------------
//
CSGEBitmapItem::CSGEBitmapItem( TUint aBitmapId ) : iBitmapId( aBitmapId ) 
    {    
    ASSERT( aBitmapId ); // bitmap ID should be greather than 0
    }

// -----------------------------------------------------------------------------
// CSGEBitmapItem::CSGEEngine()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEBitmapItem::CSGEBitmapItem( CFbsBitmap *aBitmap, CFbsBitmap *aBitmapMask, TUint aBitmapId )
    : iBitmap( aBitmap ), iBitmapMask( aBitmapMask ), iBitmapId( aBitmapId )
    {
    ASSERT( aBitmap );
    ASSERT( aBitmapMask );
    ASSERT( aBitmapId );
    }

// -----------------------------------------------------------------------------
// CSGEBitmapItem::CSGEEngine()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEBitmapItem::~CSGEBitmapItem()
    {
    delete iBitmap;
    delete iBitmapMask;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapItem::NewL()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEBitmapItem* CSGEBitmapItem::NewL( TUint aBitmapId, TSize aSize, 
                                               TDisplayMode aMode, TDisplayMode aMaskMode )
    {
    CSGEBitmapItem *self = CSGEBitmapItem::NewLC( aBitmapId, aSize, aMode, aMaskMode );
    CleanupStack::Pop(); // self;
    return self;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapItem::NewLC()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEBitmapItem* CSGEBitmapItem::NewLC( TUint aBitmapId, TSize aSize, 
                                                TDisplayMode aMode, TDisplayMode aMaskMode )
    {
    CSGEBitmapItem *self = new (ELeave) CSGEBitmapItem( aBitmapId );
    CleanupStack::PushL( self );
    self->ConstructL( aSize, aMode, aMaskMode );
    return self;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapItem::ConstructL()
// -----------------------------------------------------------------------------
//
void CSGEBitmapItem::ConstructL( TSize aSize, TDisplayMode aMode, TDisplayMode aMaskMode )
    {
    iBitmap = new (ELeave) CFbsBitmap();
    User::LeaveIfError( iBitmap->Create( aSize, aMode ) );
    if ( aMaskMode != ENone )
        {
        iBitmapMask = new(ELeave) CFbsBitmap();
        User::LeaveIfError( iBitmapMask->Create( aSize, aMaskMode ) );
        }
    }

// -----------------------------------------------------------------------------
// CSGEBitmapItem::DuplicateL()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEBitmapItem* CSGEBitmapItem::DuplicateL() const
    {
    CSGEBitmapItem *bitmapItem = new (ELeave) CSGEBitmapItem( iBitmapId );
    bitmapItem->iBitmap = SGEBitmapUtils::DuplicateFbsBitmapL( *iBitmap );
    if ( iBitmapMask )
        {
        bitmapItem->iBitmapMask = SGEBitmapUtils::DuplicateFbsBitmapL( *iBitmapMask );
        }
    return bitmapItem;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapItem::DuplicateL()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEBitmapItem* CSGEBitmapItem::DuplicateL( TUint aNewBitmapId ) const
    {
    ASSERT( aNewBitmapId );
    CSGEBitmapItem *bitmapItem = DuplicateL();
    bitmapItem->iBitmapId = aNewBitmapId;
    return bitmapItem;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapItem::Bitmap()
// -----------------------------------------------------------------------------
//
EXPORT_C CFbsBitmap* CSGEBitmapItem::Bitmap() const
    {
    return iBitmap;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapItem::BitmapMask()
// -----------------------------------------------------------------------------
//
EXPORT_C CFbsBitmap* CSGEBitmapItem::BitmapMask() const
    {
    return iBitmapMask;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapItem::Id()
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CSGEBitmapItem::Id() const
    {
    return iBitmapId;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapItem::TransformCurves()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEBitmapItem::TransformCurves( TSGEBitmapFunctionCurves aFunction )
    {
    SGEBitmapUtils::TransformCurves( aFunction, *iBitmap );
    }

// -----------------------------------------------------------------------------
// CSGEBitmapItem::TransformCurvesMask()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEBitmapItem::TransformCurvesMask( TSGEBitmapFunctionCurves aFunction )
    {
    if ( iBitmapMask )
        {
        SGEBitmapUtils::TransformCurves( aFunction, *iBitmapMask );
        }
    }

// -----------------------------------------------------------------------------
// CSGEBitmapItem::CreateBitmapMaskL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEBitmapItem::CreateBitmapMaskL( TDisplayMode aMaskMode )
    {
    if ( !iBitmapMask )
        {
        iBitmapMask = new(ELeave) CFbsBitmap();
        User::LeaveIfError( iBitmapMask->Create( iBitmap->SizeInPixels(), aMaskMode ) );
        }
    }


// End of file
