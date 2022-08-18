/*
 ============================================================================
 Name		: SGEBitmapTransparent.cpp
 Author	  : Michal Strug
 Version	 : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEBitmapTransparent implementation
 ============================================================================
 */

#include "SGEBitmapTransparent.h"
#include "SGEBitmapItem.h"
#include <BITSTD.H>
#include <BITDEV.H>


// -----------------------------------------------------------------------------
// CSGEBitmapTransparent::CSGEBitmapTransparent()
// -----------------------------------------------------------------------------
//
CSGEBitmapTransparent::CSGEBitmapTransparent()
    {
    // No implementation required
    }

// -----------------------------------------------------------------------------
// CSGEBitmapTransparent::~CSGEBitmapTransparent()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEBitmapTransparent::~CSGEBitmapTransparent()
    {
    delete iBitmapGc;
    delete iBitmapDevice;    
    delete iBitmap;
    
    delete iBitmapMaskGc;
    delete iBitmapMaskDevice;
    delete iBitmapMask;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapTransparent::NewLC()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEBitmapTransparent* CSGEBitmapTransparent::NewLC( TSize aSize, TRgb aColor, TUint8 aTransparency )
    {
    CSGEBitmapTransparent *self = new (ELeave) CSGEBitmapTransparent();
    CleanupStack::PushL( self );
    self->ConstructL( aSize, aColor, aTransparency );
    return self;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapTransparent::NewL()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEBitmapTransparent* CSGEBitmapTransparent::NewL( TSize aSize, TRgb aColor, TUint8 aTransparency )
    {
    CSGEBitmapTransparent *self = CSGEBitmapTransparent::NewLC( aSize, aColor, aTransparency );
    CleanupStack::Pop(); // self;
    return self;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapTransparent::ConstructL()
// -----------------------------------------------------------------------------
//
void CSGEBitmapTransparent::ConstructL( TSize aSize, TRgb aColor, TUint8 aTransparency )
    {
    iBitmap = new (ELeave) CFbsBitmap;
    User::LeaveIfError( iBitmap->Create( aSize, EColor16M ) );
    iBitmapDevice = CFbsBitmapDevice::NewL( iBitmap );
    User::LeaveIfError( iBitmapDevice->CreateContext( iBitmapGc ) );
    iBitmapGc->SetBrushColor( aColor );
    iBitmapGc->SetBrushStyle( CFbsBitGc::ESolidBrush );
    iBitmapGc->SetPenStyle( CFbsBitGc::ENullPen );
    iBitmapGc->DrawRect( TRect( TPoint(), iBitmap->SizeInPixels() ) );

    iBitmapMask = new(ELeave) CFbsBitmap;
    User::LeaveIfError( iBitmapMask->Create( aSize, EGray256 ) );
    iBitmapMaskDevice = CFbsBitmapDevice::NewL( iBitmapMask );
    User::LeaveIfError( iBitmapMaskDevice->CreateContext( iBitmapMaskGc ) );
    iBitmapMaskGc->SetBrushColor( TRgb::Gray256( aTransparency ) );
    iBitmapMaskGc->SetBrushStyle( CFbsBitGc::ESolidBrush );
    iBitmapMaskGc->SetPenStyle( CFbsBitGc::ENullPen );
    iBitmapMaskGc->DrawRect( TRect( TPoint(), iBitmapMask->SizeInPixels() ) );
    }

// -----------------------------------------------------------------------------
// CSGEBitmapTransparent::BitBlt()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEBitmapTransparent::BitBlt( CBitmapContext *aGc, TRect aRect )
    {
    aGc->BitBltMasked( aRect.iTl, iBitmap, aRect.Size(), iBitmapMask, EFalse );
    }

// -----------------------------------------------------------------------------
// CSGEBitmapTransparent::BitBlt()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEBitmapTransparent::BitBlt( CBitmapContext *aGc, TPoint aPoint, TRect aRect, TBool aInvertMask )
    {
    aGc->BitBltMasked( aPoint, iBitmap, aRect, iBitmapMask, aInvertMask );
    }

// -----------------------------------------------------------------------------
// CSGEBitmapTransparent::BitmapGc()
// -----------------------------------------------------------------------------
//
EXPORT_C CFbsBitGc* CSGEBitmapTransparent::BitmapGc() const
    {
    return iBitmapGc;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapTransparent::BitmapMaskGc()
// -----------------------------------------------------------------------------
//
EXPORT_C CFbsBitGc* CSGEBitmapTransparent::BitmapMaskGc() const
    {
    return iBitmapMaskGc;
    }   

// -----------------------------------------------------------------------------
// CSGEBitmapTransparent::CopyBitmapToMask()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEBitmapTransparent::CopyBitmapToMask()
    {
    iBitmapMaskGc->BitBlt( TPoint(), *iBitmapGc );
    }

// -----------------------------------------------------------------------------
// CSGEBitmapTransparent::CopyBitmapToMask()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEBitmapItem* CSGEBitmapTransparent::TransformToBitmapItemLD( TUint aBitmapId )
    {
    CSGEBitmapItem *item = new (ELeave) CSGEBitmapItem( iBitmap, iBitmapMask, aBitmapId );
    iBitmap = NULL;
    iBitmapMask = NULL;
    delete this;
    return item;
    }


// end of file
