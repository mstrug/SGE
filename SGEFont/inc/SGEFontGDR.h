/*
 ============================================================================
 Name		: SGEFontGDR.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEFontGDR declaration
 ============================================================================
 */

#ifndef SGEFONTGDR_H
#define SGEFONTGDR_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <SGEFont/SGEFont.h>

// FORWARD DECLARATIONS
class CSGEFile;
class CFont;

// CLASS DECLARATION

_LIT( KSGEFontGDRDefaultDestinationPath, "c:\\system\\temp\\" );

/**
 *  CSGEFontGDR
 * 
 */
NONSHARABLE_CLASS( CSGEFontGDR ) : public CSGEFont
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    IMPORT_C ~CSGEFontGDR();

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CSGEFontGDR* NewL( const TDesC& aFontFileName, 
                                       const TDesC& aDestinationPath = KSGEFontGDRDefaultDestinationPath() );

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CSGEFontGDR* NewL( CSGEFile &aSGEFile, const TDesC& aFontFileName, 
                                       const TDesC& aDestinationPath = KSGEFontGDRDefaultDestinationPath() );
    
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
    CSGEFontGDR();

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL( const TDesC& aFontFileName, const TDesC& aDestinationPath );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL( CSGEFile &aSGEFile, const TDesC& aFontFileName, const TDesC& aDestinationPath );

    void CreateFileNameL( const TDesC& aFontFileName, const TDesC& aDestinationPath );

    void LoadFontL();

    void CleanupFont();
    
private: // data
    
    CFont *iFont;
    
    RBuf iFontFile;
    
    TInt iFontId;
    
    };

#endif // SGEFONTGDR_H
