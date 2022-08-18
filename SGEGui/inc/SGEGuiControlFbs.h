/*
 ============================================================================
 Name		: SGEGuiControlFbs.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEGuiControlFbs declaration
 ============================================================================
 */

#ifndef SGEGUICONTROLFBS_H
#define SGEGUICONTROLFBS_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <SGEGui/SGEGuiControlBase.h>
#include <SGEEngine/SGEObjectInterfaces.h>
#include <SGEFont/SGEFont.h>

// FORWARD DECLARATIONS
class CSGEBitmapItem;

// CLASS DECLARATION

const TInt KGuiControlStateBitmaps = 3;

/**
 *  CSGEGuiControlFbs
 * 
 */
class CSGEGuiControlFbs : public CSGEGuiControlBase
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    IMPORT_C ~CSGEGuiControlFbs();

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CSGEGuiControlFbs* NewL( TUint aControlId, TRect aControlRect );

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CSGEGuiControlFbs* NewLC( TUint aControlId, TRect aControlRect );

    
    IMPORT_C void SetBitmaps( CSGEBitmapItem *aBitmapStateNormal,
                              CSGEBitmapItem *aBitmapStatePushed, 
                              CSGEBitmapItem *aBitmapStateFocused );
    
    IMPORT_C void SetFonts( CSGEFont *aFontNormal, CSGEFont *aFontPushed, CSGEFont *aFontFocused );
    
    IMPORT_C void SetTextL( const TDesC& aText );
    
    IMPORT_C void SetTextMargins( TInt aLeft, TInt aTop, TInt aRight, TInt aBottom );
    
    IMPORT_C void SetTextAlign( TSGEFontAlignHorizontal aHorizontal, TSGEFontAlignVertical aVertical );
    
protected:

    /**
     * Constructor for performing 1st stage construction
     */
    IMPORT_C CSGEGuiControlFbs( TUint aControlId, TRect aControlRect );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    IMPORT_C void ConstructL();

    IMPORT_C void BitBlt( CBitmapContext *aGc, CSGEBitmapItem *aBitmapItem, const TRect aDrawRect );

    IMPORT_C void DrawText( CBitmapContext* aGc, const TRect aDrawRect );
    
public: // from MSGEDrawableObject
    
    virtual void Draw( CBitmapContext* aGc, const TRect aDrawRect );

protected: // data

    TFixedArray< CSGEBitmapItem*, KGuiControlStateBitmaps > iBitmaps;
    
    TFixedArray< CSGEFont*, KGuiControlStateBitmaps > iFonts;
 
    RBuf iControlText;
    
    TRect iTextMargin;
    
    TSGEFontAlignHorizontal iTextAlignHorizontal;
    
    TSGEFontAlignVertical iTextAlignVertical;
    
    };

#endif // SGEGUICONTROLFBS_H
