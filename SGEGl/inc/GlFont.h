/*
 ============================================================================
 Name		: GlFont.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : TGlFont declaration
 ============================================================================
 */

#ifndef GLFONT_H
#define GLFONT_H


// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <gles/gl.h>
#include <SGEFont/SGEFont.h>
#include <SGEGl/GlTexture.h>

// CLASS DECLARATION

class CGlObjectRect;


const TText KDefaultFirstCharOffset = 0x20; // space


/**
 *  TGlFontTextureDirection
 *
 */
enum TGlFontTextureDirection
    {
    EGlFontDirectionVertical,
    EGlFontDirectionHorizontal
    };

/**
 *  CGlFont
 *
 */
class CGlFont : public CBase //CSGEFont
    {
public:

	virtual ~CGlFont();

	static CGlFont* NewL( TGlTexture& aFontTexture, TSize aLetterSize, TText aOffset = KDefaultFirstCharOffset, TGlFontTextureDirection aTextureDirection = EGlFontDirectionVertical );

	TInt LineSpacing() const;

	void SetLineSpacing( TInt aValue );

	TInt CharSpacing() const;

	void SetCharSpacing( TInt aValue );

private:

	CGlFont( TGlTexture& aFontTexture, TSize aLetterSize, TText aOffset, TGlFontTextureDirection aTextureDirection );

	void ConstructL();

	TRect GetCharRect( TInt aIdx );

public: // from base class

    TInt GetTextWidth( const TDesC& aText );

    TInt GetMaxHeight();

    TSize LetterSize() const;

    void DrawText( TPoint aPosition, const TDesC& aText );

    void DrawText( CBitmapContext &aGc, TPoint aPosition, const TDesC& aText );
    
    void DrawText( CBitmapContext &aGc, TRect aRect, const TDesC& aText, 
                   TSGEFontAlignHorizontal aHorizontalAlign = ESGEFontAlignHorizontalCenter,
                   TSGEFontAlignVertical aVerticalAlign = ESGEFontAlignVerticalCenter );
    
    void AddLetterMargins( TText aLetter, TInt aLeftPixels, TInt aRightPixels );

    // draws character, aDrawOrRedrawTexture if set to ETrue calls draw on rect object
    void DrawChar( TText aChar, TBool aDrawOrRedrawTexture = ETrue, TBool aDiscardTexture = ETrue );

private: // data

	TGlTexture& iFontTexture;

	TSize iLetterSize;

	TText iCharacterOffset;

	class TLetterMargins
		{
	public:
		TLetterMargins();
		TBool IsZero();

		TInt16 iLeft;
		TInt16 iRight;
		};

	RArray< TLetterMargins > iLettersMargins;

	CGlObjectRect* iRect; // owned

	TGlFontTextureDirection iTextureDirection;

	TInt iCharsPerRow;
	TInt iCharsPerCol;

	TInt iCharSpacing;
	TInt iLineSpacing;
    };


#endif // GLFONT_H
