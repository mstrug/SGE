/*
 ============================================================================
 Name		: VgFont.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CVgFont implementation
 ============================================================================
 */

#include "VgFont.h"

const TInt KDefaultLen = 128;


CVgFont::CVgFont( TInt aGlyphCount ) : iStringBufferLen( KDefaultLen ), iLetters( ( aGlyphCount == 0 ? 34 : aGlyphCount ) )
    {
    }

CVgFont::~CVgFont()
    {
	iLetters.Close();
	iLettersWidths.Close();
	User::Free( iStringBuffer );
	vgDestroyFont( iVgFont );
    }

CVgFont* CVgFont::NewLC( VGImage aImage, TInt aGlyphCount )
	{
	CVgFont* self = new (ELeave) CVgFont( aGlyphCount );
	CleanupStack::PushL(self);
	self->ConstructL( aImage, aGlyphCount );
	return self;
	}

CVgFont* CVgFont::NewL( VGImage aImage, TInt aGlyphCount )
	{
	CVgFont* self = CVgFont::NewLC( aImage, aGlyphCount );
	CleanupStack::Pop(); // self;
	return self;
	}

void CVgFont::ConstructL( VGImage aImage, TInt aGlyphCount )
	{
	iVgFont = vgCreateFont( aGlyphCount );
	if ( iVgFont == VG_INVALID_HANDLE )
		{
		User::Leave( KErrArgument );
		}
	iFontImage = aImage;
	iStringBuffer = (VGuint*) User::Alloc( iStringBufferLen * sizeof( VGuint ) );
	}

void CVgFont::SetStringBuffer( const TDesC& aText )
	{
	if ( iStringBufferLen < aText.Length() )
		{
		iStringBufferLen = aText.Length() * sizeof( VGuint );
		iStringBuffer = (VGuint*) User::ReAlloc( iStringBuffer, iStringBufferLen, RAllocator::EAllowMoveOnShrink );
		}
	iStringBufferTextLen = 0;
	for ( TInt i = 0; i < aText.Length(); i++ )
		{
		if ( iLetters.Find( aText[ i ] ) != KErrNotFound )
			{
			iStringBuffer[ iStringBufferTextLen++ ] = aText[ i ];
			}
		}
	}

void CVgFont::SetStringBuffer( const TDesC& aText, TVgRect aRect )
    {
    if ( iStringBufferLen < aText.Length() )
        {
        iStringBufferLen = aText.Length() * sizeof( VGuint );
        iStringBuffer = (VGuint*) User::ReAlloc( iStringBuffer, iStringBufferLen, RAllocator::EAllowMoveOnShrink );
        }
    iStringBufferTextLen = 0;
    TInt rw = aRect.Width();
    for ( TInt i = 0, w = 0; i < aText.Length(); i++ )
        {
        TInt idx = iLetters.Find( aText[ i ] );
        if ( idx != KErrNotFound )
            {
            w += iLettersWidths[ idx ];
            if ( w > rw ) break;
            iStringBuffer[ iStringBufferTextLen++ ] = aText[ i ];
            }
        }
    }

void CVgFont::AddGlyph( TChar aLetter, TRect aGlyphRect, TPoint aGlyphsOrigin, TSize aGlyphsEscapement )
	{
	VGfloat origin[ 2 ] = { aGlyphsOrigin.iX, aGlyphsOrigin.iY };
	VGfloat esc[ 2 ] = { aGlyphsEscapement.iWidth, aGlyphsEscapement.iHeight };
	VGint x = aGlyphRect.iTl.iX;
	VGint y = aGlyphRect.iTl.iY;
	VGint w = aGlyphRect.Width();
	VGint h = aGlyphRect.Height();

	iAveragedGlyphSize.iHeight = ( iAveragedGlyphSize.iHeight + h ) / 2; 
	iAveragedGlyphSize.iWidth = ( iAveragedGlyphSize.iWidth + w ) / 2; 
	
	VGImage tmpImage = vgChildImage(iFontImage, x, y, w, h );
	vgSetGlyphToImage( iVgFont, aLetter, tmpImage, origin, esc );
	vgDestroyImage( tmpImage );
	iLetters.Append( aLetter );
	iLettersWidths.Append( aGlyphsEscapement.iWidth );
	}

void CVgFont::AddGlyphs( TChar aLetterStart, TChar aLetterEnd, TSize aImageSize, TSize aGlyphSize, TSize aGlyphsCount, TPoint aGlyphsOrigin, TSize aGlyphsEscapement, TPoint aImageOffset )
	{
	if ( aGlyphsEscapement == TSize() )
		{
		aGlyphsEscapement.iWidth = aGlyphSize.iWidth;
		}

	for ( TUint i = aLetterStart, x = 0, y = 0; i <= aLetterEnd; i++, y += aGlyphSize.iHeight )
		{
		if ( y >= aGlyphsCount.iHeight * aGlyphSize.iHeight ) 
			{
			x += aGlyphSize.iWidth;
			y = 0;
			}
		//TInt x1 = x + aGlyphsOrigin.iX;		
		//TInt y1 = y - aGlyphsOrigin.iY;		
		//TPoint tl( x1, s.iHeight - ( y1 + aGlyphsEscapement.iHeight ) );
		//TPoint br( x1 + aGlyphsEscapement.iWidth, s.iHeight - y1 );

		TPoint tl( x + aImageOffset.iX, aImageSize.iHeight - ( y + aImageOffset.iY + aGlyphSize.iHeight ) );
		TPoint br( x + aImageOffset.iX + aGlyphSize.iWidth, aImageSize.iHeight - ( y + aImageOffset.iY ) );
		AddGlyph( i, TRect( tl, br ), aGlyphsOrigin, aGlyphsEscapement );
		}
	iAveragedGlyphSize = aGlyphSize; 
	iGlyphsEscapement = aGlyphsEscapement;
	}

TInt CVgFont::AddGlyphs( TChar aLetterStart, TChar aLetterEnd, TSize aImageSize, TSize aGlyphSize, TSize aGlyphsCount, 
						 TPoint aGlyphsOrigin, const RArray<TSize>& aGlyphsEscapement, TPoint aImageOffset )
	{
	for ( TUint i = aLetterStart, x = 0, y = 0, j = 0; i <= aLetterEnd; i++, y += aGlyphSize.iHeight, j++ )
		{
		if ( aGlyphsEscapement.Count() <= j )
			{
			return KErrNotFound;
			}
		if ( y >= aGlyphsCount.iHeight * aGlyphSize.iHeight ) 
			{
			x += aGlyphSize.iWidth;
			y = 0;
			}
		TPoint tl( x + aImageOffset.iX, aImageSize.iHeight - ( y + aImageOffset.iY + aGlyphSize.iHeight ) );
		TPoint br( x + aImageOffset.iX + aGlyphSize.iWidth, aImageSize.iHeight - ( y + aImageOffset.iY ) );
		
		AddGlyph( i, TRect( tl, br ), aGlyphsOrigin, aGlyphsEscapement[ j ] );
		}
	iAveragedGlyphSize = aGlyphSize; 
	//iGlyphsEscapement = aGlyphsEscapement;
	return KErrNone;
	}

void CVgFont::AddSpace( TPoint aGlyphsOrigin, TSize aGlyphsEscapement )
	{
	VGfloat origin[ 2 ] = { aGlyphsOrigin.iX, aGlyphsOrigin.iY };
	VGfloat esc[ 2 ] = { aGlyphsEscapement.iWidth, aGlyphsEscapement.iHeight };
	vgSetGlyphToImage( iVgFont, ' ', VG_INVALID_HANDLE, origin, esc );
	iLetters.Append( ' ' );
    iLettersWidths.Append( aGlyphsEscapement.iWidth );
	}

void CVgFont::RemoveGlyph( TChar aLetter )
	{
	vgClearGlyph( iVgFont, aLetter );
	TInt idx = iLetters.Find( aLetter );
    if ( idx != KErrNotFound )
        {
        iLetters.Remove( idx );
        iLettersWidths.Remove( idx );
        }
	}

void CVgFont::DrawGlyph( TVgPoint aPoint, TChar aLetter )
	{
	VGfloat origin[ 2 ] = { aPoint.iX, aPoint.iY };
	vgSetfv( VG_GLYPH_ORIGIN, 2, origin );   
	vgDrawGlyph( iVgFont, aLetter, VG_FILL_PATH, VG_FALSE );
	}

void CVgFont::DrawText( TVgPoint aPoint, const TDesC& aText, TBool aDivideOnNewLineCharacter )
	{
	VGfloat origin[ 2 ] = { aPoint.iX, aPoint.iY };
	vgSetfv( VG_GLYPH_ORIGIN, 2, origin );
	
	if ( aDivideOnNewLineCharacter && aText.Locate( '\n' ) != KErrNotFound )
	    {
	    TInt len = aText.Length();
	    for ( TInt i = 0, j = 0; i < len; i++ )
	        {
	        if ( aText[ i ] == '\n' || i == len - 1 )
	            {
	            if ( i == len - 1 ) i++;
	            TPtrC str( aText.Mid( j, i - j ) );
	            j = i + 1;
	            
	            SetStringBuffer( str );

	            vgDrawGlyphs( iVgFont, iStringBufferTextLen, iStringBuffer, NULL, NULL, VG_FILL_PATH, VG_FALSE );
	            
	            origin[ 1 ] -= iAveragedGlyphSize.iHeight;
	            vgSetfv( VG_GLYPH_ORIGIN, 2, origin );
	            }
	        }
	    }
	else
	    {
	    SetStringBuffer( aText );

	    vgDrawGlyphs( iVgFont, iStringBufferTextLen, iStringBuffer, NULL, NULL, VG_FILL_PATH, VG_FALSE );
	    }
	}

void CVgFont::DrawText( TVgRect aRect, const TDesC& aText, TSGEFontAlignHorizontal aHorizontalAlign, TSGEFontAlignVertical aVerticalAlign, TBool aTruncate )
	{
	VGfloat origin[ 2 ] = { 0, -10000 };
	vgSetfv( VG_GLYPH_ORIGIN, 2, origin );
	if ( aTruncate && !aRect.IsEmpty() )
	    {
        SetStringBuffer( aText, aRect );
	    }
	else
	    {
	    SetStringBuffer( aText );
	    }
	vgDrawGlyphs( iVgFont, iStringBufferTextLen, iStringBuffer, NULL, NULL, 0, VG_FALSE );
	vgGetfv( VG_GLYPH_ORIGIN, 2, origin );
	
	VGfloat x = aRect.iTl.iX;
	VGfloat y = aRect.iBr.iY;
	
	switch ( aHorizontalAlign )
		{
		case ESGEFontAlignHorizontalLeft:
			break;
		case ESGEFontAlignHorizontalJustify:
		case ESGEFontAlignHorizontalCenter:
				x += ( aRect.Width() - origin[ 0 ] ) / 2;
			break;
		case ESGEFontAlignHorizontalRight:
				x += aRect.Width() - origin[ 0 ];
			break;
		}
	
	switch ( aVerticalAlign )
		{
		case ESGEFontAlignVerticalTop:
				y += aRect.Height() - iAveragedGlyphSize.iHeight;
			break;
		case ESGEFontAlignVerticalCenter:
				y += ( aRect.Height() - iAveragedGlyphSize.iHeight ) / 2;
			break;
		case ESGEFontAlignVerticalBottom:
			break;
		}
	
	origin[ 0 ] = (TInt)x;
	origin[ 1 ] = (TInt)y;
	vgSetfv( VG_GLYPH_ORIGIN, 2, origin );

	vgDrawGlyphs( iVgFont, iStringBufferTextLen, iStringBuffer, NULL, NULL, VG_FILL_PATH, VG_FALSE );
	}

TInt CVgFont::GetTextWidth( const TDesC& aText )
	{
	VGfloat origin[ 2 ] = { 0, -10000 };
	vgSetfv( VG_GLYPH_ORIGIN, 2, origin );   

	SetStringBuffer( aText );

	vgDrawGlyphs( iVgFont, aText.Length(), iStringBuffer, NULL, NULL, 0, VG_FALSE );

	vgGetfv( VG_GLYPH_ORIGIN, 2, origin );
	
	return origin[ 0 ];
	}

TSize CVgFont::GlyphSize() const
	{
	return iAveragedGlyphSize;
	}

TSize CVgFont::GlyphsEscapement() const
	{
	return iGlyphsEscapement;
	}



