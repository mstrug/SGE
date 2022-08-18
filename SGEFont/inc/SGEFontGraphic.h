/*
 ============================================================================
 Name		: SGEFontGraphic.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEFontGraphic declaration
 ============================================================================
 */

#ifndef SGEFONTGRAPHIC_H
#define SGEFONTGRAPHIC_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <e32hashtab.h>
#include <SGEFont/SGEFont.h>
#include <SGEBitmap/SGEBitmapUtils.h>

// FORWARD DECLARATIONS
class CSGEBitmapItem;
class CSGEFile;
class CFbsBitmap;

// CLASS DECLARATION

const TInt KDefaultFirstCharOffset = 0x20; // space
const TInt KDefaultLettersAnalyzerThreshold = 15;

/**
 *  TSGEFontGraphicLetterMargins
 * 
 */
class TSGEFontGraphicLetterMargins
    {
public:
    TInt16 iLeft;
    TInt16 iRight;
    };

/**
 *  CSGEFontGraphic
 * 
 */
NONSHARABLE_CLASS( CSGEFontGraphic ) : public CSGEFont
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    IMPORT_C ~CSGEFontGraphic();

    /*
     * Loads specified file asynchronously
     * @param aFontFileName image file with graphics fonts definition
     * @param aLetterSize size of the letter
     * @param aOffset offset of the first character in the ascii code
     * @param aLettersAnalyzerThreshold threshold value used when analyzing font
     *        and calculating characters margins, if 0 then analyzer is not used.
     */
    IMPORT_C static CSGEFontGraphic* NewL( const TDesC& aFontFileName, TSize aLetterSize, TChar aOffset = KDefaultFirstCharOffset, TInt aLettersAnalyzerThreshold = KDefaultLettersAnalyzerThreshold );

    /*
     * Loads specified file asynchronously
     * @param aSGEFile SGEFile in which the font is
     * @param aFontFileName image file with graphics fonts definition
     * @param aLetterSize size of the letter
     * @param aOffset offset of the first character in the ascii code
     * @param aLettersAnalyzerThreshold threshold value used when analyzing font
     *        and calculating characters margins, if 0 then analyzer is not used.
     */
    IMPORT_C static CSGEFontGraphic* NewL( CSGEFile &aSGEFile, const TDesC& aFontFileName, TSize aLetterSize, TChar aOffset = KDefaultFirstCharOffset, TInt aUseLettersAnalyzer = KDefaultLettersAnalyzerThreshold );

    /*
     * @param aBitmapItem reference to the bitmap item which contains font image
     * @param aLetterSize size of the letter
     * @param aOffset offset of the first character in the ascii code
     * @param aLettersAnalyzerThreshold threshold value used when analyzing font
     *        and calculating characters margins, if 0 then analyzer is not used.
     */
    IMPORT_C static CSGEFontGraphic* NewL( CSGEBitmapItem &aBitmapItem, TSize aLetterSize, TChar aOffset = KDefaultFirstCharOffset, TInt aUseLettersAnalyzer = KDefaultLettersAnalyzerThreshold );

    IMPORT_C void SetInvertMask( TBool aInvertMask );

    IMPORT_C void AddCharacterMap( TChar aCharacterCode, TChar aNewCharacterCode );
    
    IMPORT_C void TransformCurves( TSGEBitmapFunctionCurves aFunction );
    
    IMPORT_C void SetUseLettersAnalyzer( TBool aUse );

    IMPORT_C TSGEFontGraphicLetterMargins* LetterMargins( TChar aLetter );

    IMPORT_C CSGEFontGraphic* DuplicateL();

    IMPORT_C CSGEFontGraphic* DuplicateL( CSGEBitmapItem &aNewBitmap );

public: // from base class

    TInt GetTextWidth( const TDesC& aText );

    TInt GetMaxHeight();

    void DrawText( CBitmapContext &aGc, TPoint aPosition, const TDesC& aText );

    void DrawText( CBitmapContext &aGc, TRect aRect, const TDesC& aText, 
                   TSGEFontAlignHorizontal aHorizontalAlign = ESGEFontAlignHorizontalCenter,
                   TSGEFontAlignVertical aVerticalAlign = ESGEFontAlignVerticalCenter );

    IMPORT_C void DrawText( CBitmapContext &aGc, TPoint aPosition, const TDesC& aText, TRect aDrawRect );

    IMPORT_C void DrawText( CBitmapContext &aGc, TRect aRect, const TDesC& aText, TRect aDrawRect, 
                            TSGEFontAlignHorizontal aHorizontalAlign = ESGEFontAlignHorizontalCenter,
                            TSGEFontAlignVertical aVerticalAlign = ESGEFontAlignVerticalCenter );
private:

    /**
     * Constructor for performing 1st stage construction
     */
    CSGEFontGraphic( TSize aLetterSize, TChar aOffset, TBool aUseLettersAnalyzer );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL( const TDesC& aFontFileName );
    
    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL( CSGEFile &aSGEFile, const TDesC& aFontFileName );
    
    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL( CSGEBitmapItem &aBitmapItem );
    
    TInt CalculateHorizontalPosition( const TDesC& aText, TRect aRect, TSGEFontAlignHorizontal aHorizontalAlign );

    TInt CalculateVerticalPosition( const TDesC& aText, TRect aRect, TSGEFontAlignVertical aVerticalAlign );

    TRect CalculateLetterRect( TChar aLetter );
    
    void AnalyzeLettersL();
    
    void AnalyzeLetter( TChar aLetter );
    
    TInt CountLines( const TDesC& aText );

    TInt LongestLineWidth( const TDesC& aText );

private: // data

    TSize iLetterSize;

    TChar iCharacterOffset;

    TBool iBitmapsOwned;

    TBool iInvertMask;

    CFbsBitmap *iBitmap;

    CFbsBitmap *iBitmapMask;
    
    RHashMap< TUint, TUint > iCharactersMap;

    RArray< TSGEFontGraphicLetterMargins > iLettersMargins;
    
    TBool iUseLettersAnalyzer;
    
    TInt iLettersAnalyzerThreshold;
    };

#endif // SGEFONTGRAPHIC_H
