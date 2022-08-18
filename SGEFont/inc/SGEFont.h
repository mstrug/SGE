/*
 ============================================================================
 Name		: SGEFont.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEFont declaration
 ============================================================================
 */

#ifndef SGEFONT_H
#define SGEFONT_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

// FORWARD DECLARATIONS
class CBitmapContext;

// CLASS DECLARATION
enum TSGEFontAlignHorizontal
{
    ESGEFontAlignHorizontalLeft,
    ESGEFontAlignHorizontalCenter,
    ESGEFontAlignHorizontalRight,
    ESGEFontAlignHorizontalJustify
};

enum TSGEFontAlignVertical
{
    ESGEFontAlignVerticalTop,
    ESGEFontAlignVerticalCenter,
    ESGEFontAlignVerticalBottom
};

/**
 *  CSGEFont
 * 
 */
class CSGEFont : public CBase
    {
public:

    /**
     * Destructor.
     */
    IMPORT_C ~CSGEFont();
    
    virtual TInt GetTextWidth( const TDesC& aText ) = 0;

    virtual TInt GetMaxHeight() = 0;

    /*
     * Draws text on specified coordinates
     */    
    virtual void DrawText( CBitmapContext &aGc, TPoint aPosition, const TDesC& aText ) = 0;

    /*
     * Draws text centered in specified rectangle
     */    
    virtual void DrawText( CBitmapContext &aGc, TRect aRect, const TDesC& aText, 
                           TSGEFontAlignHorizontal aHorizontalAlign = ESGEFontAlignHorizontalCenter,
                           TSGEFontAlignVertical aVerticalAlign = ESGEFontAlignVerticalCenter ) = 0;

    /*
     * Draws text which is inside draw rect on specified coordinates.
     */    
    IMPORT_C virtual void DrawText( CBitmapContext &aGc, TPoint aPosition, const TDesC& aText, TRect aDrawRect );

    /*
     * Draws text which is inside draw rect centered in specified rectangle
     */    
    IMPORT_C virtual void DrawText( CBitmapContext &aGc, TRect aRect, const TDesC& aText, TRect aDrawRect, 
                                    TSGEFontAlignHorizontal aHorizontalAlign = ESGEFontAlignHorizontalCenter,
                                    TSGEFontAlignVertical aVerticalAlign = ESGEFontAlignVerticalCenter );
    
    IMPORT_C void SetLettersHorizontalStep( TInt aHorizontalStep );

    IMPORT_C void SetLettersVerticalStep( TInt aVerticalStep );

    IMPORT_C void SetDivideLines( TBool aDivideLines );

protected:

    /**
     * Constructor for performing 1st stage construction
     */
    IMPORT_C CSGEFont();

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    IMPORT_C void BaseConstructL();

protected: // data

    TSize iLettersStep;

    TBool iDivideLines;
    
    };

#endif // SGEFONT_H
