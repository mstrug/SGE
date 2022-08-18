/*
 ============================================================================
 Name		: SGEFontSystem.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEFontSystem declaration
 ============================================================================
 */

#ifndef SGEFONTSYSTEM_H
#define SGEFONTSYSTEM_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <GULFONT.H>
#include <SGEFont/SGEFont.h>

// CLASS DECLARATION

/**
 *  CSGEFontSystem
 * 
 */
NONSHARABLE_CLASS( CSGEFontSystem ) : public CSGEFont
    {
public:

    enum TSGEFontSystemStandard
        {
        ESGEFontSystemStandardAnnotation,
        ESGEFontSystemStandardTitle,
        ESGEFontSystemStandardLegend,
        ESGEFontSystemStandardSymbol,
        ESGEFontSystemStandardDense
        };

public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    IMPORT_C ~CSGEFontSystem();

    /**
     * Two-phased constructors.
     */
    
    IMPORT_C static CSGEFontSystem* NewL( const TDesC &aTypefaceName, TInt aHeight, TBool aBold = EFalse, TBool aItalic = EFalse );

    IMPORT_C static CSGEFontSystem* NewL( const TDesC &aTypefaceName, TInt aHeight, TFontStyle aFontStyle );

    IMPORT_C static CSGEFontSystem* NewL( const TLogicalFont &aLogicalFont );

    IMPORT_C static CSGEFontSystem* NewL( TSGEFontSystemStandard aStandardFont );
    
    IMPORT_C const CFont* Font() const;
    
public: // from base class

    TInt GetTextWidth( const TDesC& aText );

    TInt GetMaxHeight();

    void DrawText( CBitmapContext &aGc, TPoint aPosition, const TDesC& aText );

    void DrawText( CBitmapContext &aGc, TRect aRect, const TDesC& aText, 
                   TSGEFontAlignHorizontal aHorizontalAlign = ESGEFontAlignHorizontalCenter,
                   TSGEFontAlignVertical aVerticalAlign = ESGEFontAlignVerticalCenter );

private:

    /**
     * Constructor for performing 1st stage construction
     */
    CSGEFontSystem();

    /**
     * EPOC default constructora for performing 2nd stage construction
     */

    void ConstructL( const TDesC &aTypefaceName, TInt aHeight, TBool aBold, TBool aItalic );

    void ConstructL( const TDesC &aTypefaceName, TInt aHeight, TFontStyle aFontStyle );

    void ConstructL( const TLogicalFont &aLogicalFont );

    void ConstructL( TSGEFontSystemStandard aStandardFont );
    
private: // data

    CFont *iFont;
    
    };

#endif // SGEFONTSYSTEM_H
