/*
 ============================================================================
 Name		: VgFont.h
 Author	  : Michal Strug
 Version	 : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CVgFont declaration
 ============================================================================
 */

#ifndef VGFONT_H
#define VGFONT_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <vg/openvg.h>
#include <SGE/SGEVg/VgImage.h>
#include <SGE/SGEVg/VgPrimitives.h>
#include <SGE/SGEFont/SGEFont.h>


/**
 *  CVgFont
 *  If using from SGEApplication destory CVgFont objects before application 
 *  destructor is called (for example in Cleanup function). 
 *  
 *  glyph image - image of a letter (whole image is draw for a glyph)
 *  glyph origin - point where letter left bottom corner starts
 *  glyph escapement - size of which a VG_GLYPH_ORIGIN will be moved  
 */
class CVgFont : public CBase
    {

public:
    
	~CVgFont();
	
	static CVgFont* NewL( VGImage aImage, TInt aGlyphCount ); 

	static CVgFont* NewLC( VGImage aImage, TInt aGlyphCount );
	
	void AddGlyph( TChar aLetter, TRect aGlyphRect, TPoint aGlyphsOrigin, TSize aGlyphsEscapement );
	
	// by column
	void AddGlyphs( TChar aLetterStart, TChar aLetterEnd, TSize aImageSize, TSize aGlyphSize, TSize aGlyphsCount, 
	                TPoint aGlyphsOrigin = TPoint(), TSize aGlyphsEscapement = TSize(), TPoint aImageOffset = TPoint() );

	TInt AddGlyphs( TChar aLetterStart, TChar aLetterEnd, TSize aImageSize, TSize aGlyphSize, TSize aGlyphsCount, 
	                TPoint aGlyphsOrigin, const RArray<TSize>& aGlyphsEscapement, TPoint aImageOffset = TPoint() );
	
	void AddSpace( TPoint aGlyphsOrigin, TSize aGlyphsEscapement );

	void RemoveGlyph( TChar aLetter );

	void DrawGlyph( TVgPoint aPoint, TChar aLetter );

	void DrawText( TVgPoint aPoint, const TDesC& aText, TBool aDivideOnNewLineCharacter = EFalse );

	void DrawText( TVgRect aRect, const TDesC& aText, 
					TSGEFontAlignHorizontal aHorizontalAlign = ESGEFontAlignHorizontalCenter,
					TSGEFontAlignVertical aVerticalAlign = ESGEFontAlignVerticalCenter, TBool aTruncate = EFalse );
	
    TInt GetTextWidth( const TDesC& aText );

    TSize GlyphSize() const;
    
    TSize GlyphsEscapement() const;
    
private:
	
	CVgFont( TInt aGlyphCount );
	
	void ConstructL( VGImage aImage, TInt aGlyphCount );
	
    void SetStringBuffer( const TDesC& aText );

    void SetStringBuffer( const TDesC& aText, TVgRect aRect );

private:
    
	VGFont iVgFont;
    
	VGImage iFontImage;
	
	VGuint* iStringBuffer;
	
	TInt iStringBufferLen;
	   
    TInt iStringBufferTextLen;
    
	TSize iAveragedGlyphSize;
	
	TSize iGlyphsEscapement;

    RArray< TInt > iLetters;
    RArray< TInt > iLettersWidths;
	
    };

#endif // VGFONT_H
