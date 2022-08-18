/*
 ============================================================================
 Name		: SGEGuiControlFbs.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEGuiControlFbs implementation
 ============================================================================
 */

#include "SGEGuiControlFbs.h"
#include <SGEBitmap/SGEBitmapItem.h>


// -----------------------------------------------------------------------------
// CSGEGuiControlFbs::CSGEGuiControlFbs()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEGuiControlFbs::CSGEGuiControlFbs( TUint aControlId, TRect aControlRect )
    : CSGEGuiControlBase( aControlId, aControlRect )
    {
    // No implementation required
    }

// -----------------------------------------------------------------------------
// CSGEGuiControlFbs::~CSGEGuiControlFbs()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEGuiControlFbs::~CSGEGuiControlFbs()
    {
    iControlText.Close();
    }

// -----------------------------------------------------------------------------
// CSGEGuiControlFbs::NewL()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEGuiControlFbs* CSGEGuiControlFbs::NewLC( TUint aControlId, TRect aControlRect )
    {
    CSGEGuiControlFbs *self = new ( ELeave ) CSGEGuiControlFbs( aControlId, aControlRect );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CSGEGuiControlFbs::NewL()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEGuiControlFbs* CSGEGuiControlFbs::NewL( TUint aControlId, TRect aControlRect )
    {
    CSGEGuiControlFbs* self = CSGEGuiControlFbs::NewLC( aControlId, aControlRect );
    CleanupStack::Pop(); // self;
    return self;
    }

// -----------------------------------------------------------------------------
// CSGEGuiControlFbs::ConstructL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEGuiControlFbs::ConstructL()
    {
    BaseConstructL();
    }

// -----------------------------------------------------------------------------
// CSGEGuiControlFbs::SetBitmaps()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEGuiControlFbs::SetBitmaps( CSGEBitmapItem *aBitmapStateNormal,
                                             CSGEBitmapItem *aBitmapStatePushed, 
                                             CSGEBitmapItem *aBitmapStateFocused )
    {
    iBitmaps[ ENormal ] = aBitmapStateNormal;
    iBitmaps[ EPushed ] = aBitmapStatePushed;
    iBitmaps[ EFocused ] = aBitmapStateFocused;
    iNeedsRedraw = ETrue;
    }

// -----------------------------------------------------------------------------
// CSGEGuiControlFbs::SetFonts()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEGuiControlFbs::SetFonts( CSGEFont *aFontNormal, CSGEFont *aFontPushed, CSGEFont *aFontFocused )
    {
    iFonts[ ENormal ] = aFontNormal;
    iFonts[ EPushed ] = aFontPushed;
    iFonts[ EFocused ] = aFontFocused;
    iNeedsRedraw = ETrue;
    }

// -----------------------------------------------------------------------------
// CSGEGuiControlFbs::SetTextL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEGuiControlFbs::SetTextL( const TDesC& aText )
    {
    iControlText.Close();
    iControlText.CreateL( aText );
    iNeedsRedraw = ETrue;
    }

// -----------------------------------------------------------------------------
// CSGEGuiControlFbs::SetTextMargins()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEGuiControlFbs::SetTextMargins( TInt aLeft, TInt aTop, TInt aRight, TInt aBottom )
    {
    iTextMargin.iTl.SetXY( aLeft, aTop );
    iTextMargin.iBr.SetXY( aRight, aBottom );
    iNeedsRedraw = ETrue;
    }
    
// -----------------------------------------------------------------------------
// CSGEGuiControlFbs::SetTextAlign()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEGuiControlFbs::SetTextAlign( TSGEFontAlignHorizontal aHorizontal, TSGEFontAlignVertical aVertical )
    {
    iTextAlignHorizontal = aHorizontal;
    iTextAlignVertical = aVertical;
    iNeedsRedraw = ETrue;
    }

// -----------------------------------------------------------------------------
// CSGEGuiControlFbs::BitBlt()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEGuiControlFbs::BitBlt( CBitmapContext *aGc, CSGEBitmapItem *aBitmapItem, const TRect aDrawRect )
    {
    if ( aBitmapItem )
        {
        TRect r( aDrawRect.iTl - iControlRect.iTl, aDrawRect.Size() );
        if ( aBitmapItem->BitmapMask() )
            {
            aGc->BitBltMasked( aDrawRect.iTl, aBitmapItem->Bitmap(), r,
                               aBitmapItem->BitmapMask(), EFalse );
            }
        else
            {
            aGc->BitBlt( aDrawRect.iTl, aBitmapItem->Bitmap(), r );
            }
        }
    }

// -----------------------------------------------------------------------------
// CSGEGuiControlFbs::DrawText()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEGuiControlFbs::DrawText( CBitmapContext* aGc, const TRect aDrawRect )
    {
    if ( iFonts[ iControlState ] && iControlText.Length() > 0 )
        {
        TRect r( iControlRect );
        r.iTl += iTextMargin.iTl;
        r.iBr -= iTextMargin.iBr;
        if ( r.Intersects( aDrawRect ) )
            {
            iFonts[ iControlState ]->DrawText( *aGc, r, iControlText, aDrawRect,
                                               iTextAlignHorizontal, iTextAlignVertical );
            }
        }
    }

// -----------------------------------------------------------------------------
// CSGEGuiControlFbs::Draw()
// -----------------------------------------------------------------------------
//
void CSGEGuiControlFbs::Draw( CBitmapContext* aGc, const TRect aDrawRect )
    {
    iNeedsRedraw = EFalse;
    if ( iHidden || !aDrawRect.Intersects( iControlRect ) )
        {
        return;
        }
    
    TRect drawRect( aDrawRect );
    drawRect.Intersection( iControlRect );
    
    ASSERT( iControlState == ENormal || iControlState == EPushed || iControlState == EFocused );
    BitBlt( aGc, iBitmaps[ iControlState ], drawRect );
    
    DrawText( aGc, aDrawRect );
    }



// End of file


