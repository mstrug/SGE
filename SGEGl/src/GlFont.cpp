/*
 ============================================================================
 Name		: GlFont.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : TGlFont implementation
 ============================================================================
 */

#include "GlFont.h"
#include <SGEGl/GlTexture.h>
#include <SGEGl/GlObjectRect.h>
#include <SGEGl/GlUtils.h>
#include <SGEGl/GlPrimitive.h>
#include <SGEGl/GlVertexArray.h>
#include <gles/gl.h>


CGlFont::TLetterMargins::TLetterMargins() : iLeft( 0 ), iRight( 0 )
	{
	}
TBool CGlFont::TLetterMargins::IsZero()
	{
	return ( iLeft == 0 && iRight == 0 );
	}




CGlFont::CGlFont( TGlTexture& aFontTexture, TSize aLetterSize, TText aOffset, TGlFontTextureDirection aTextureDirection )
	: iFontTexture( aFontTexture ), iLetterSize( aLetterSize ), iCharacterOffset( aOffset ), iTextureDirection( aTextureDirection )
	{
	}

CGlFont::~CGlFont()
	{
	iLettersMargins.Close();
	delete iRect;
	}

CGlFont* CGlFont::NewL( TGlTexture& aFontTexture, TSize aLetterSize, TText aOffset, TGlFontTextureDirection aTextureDirection )
	{
	CGlFont* self = new (ELeave) CGlFont( aFontTexture, aLetterSize, aOffset, aTextureDirection );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

void CGlFont::ConstructL()
	{
	iRect = CGlObjectRect::NewL();
	iRect->Primitive().VertexArray( 0 )->ColorArrayEnabled( EFalse );

	iCharsPerRow = iFontTexture.Size().iWidth / iLetterSize.iWidth; // horizontal direction (liczba znakow w 1 wierszu)
	iCharsPerCol = iFontTexture.Size().iHeight / iLetterSize.iHeight; // vertical direction (liczba znakow w 1 kolumnie)

	if ( iCharsPerRow == 0 ) iCharsPerRow++;
	if ( iCharsPerCol == 0 ) iCharsPerCol++;

	for ( TInt i = 0; i < iCharsPerRow * iCharsPerCol; i++ )
		{
		iLettersMargins.AppendL( TLetterMargins() );
		}
	}

TInt CGlFont::GetTextWidth( const TDesC& aText )
	{
	TInt ret = 0;
	for ( TInt i = 0; i < aText.Length(); i++ )
		{
		TInt idx = aText[ i ] - iCharacterOffset;

		if ( aText[ i ] == '\r' || aText[ i ] == '\n' )
			{
			break;
			}

		if ( idx >= 0 && idx < iLettersMargins.Count() && !iLettersMargins[ idx ].IsZero() )
			{
			ret += iLetterSize.iWidth - iLettersMargins[ idx ].iLeft - iLettersMargins[ idx ].iRight + iCharSpacing;
			}
		else  if ( idx >= 0 )
			{
			ret += iLetterSize.iWidth + iCharSpacing;
			}
		}
	return ret;
	}

TInt CGlFont::GetMaxHeight()
	{
	return iLetterSize.iHeight;
	}

TInt CGlFont::LineSpacing() const
	{
	return iLineSpacing;
	}

void CGlFont::SetLineSpacing( TInt aValue )
	{
	iLineSpacing = aValue;
	}

TInt CGlFont::CharSpacing() const
	{
	return iCharSpacing;
	}

void CGlFont::SetCharSpacing( TInt aValue )
	{
	iCharSpacing = aValue;
	}

TSize CGlFont::LetterSize() const
	{
	return iLetterSize;
	}

void CGlFont::AddLetterMargins( TText aLetter, TInt aLeftPixels, TInt aRightPixels )
	{
	TInt idx = aLetter - iCharacterOffset;
	if ( idx >= 0 && idx < iLettersMargins.Count() )
		{
		iLettersMargins[ idx ].iLeft = aLeftPixels;
		iLettersMargins[ idx ].iRight = aRightPixels;
		}
	}

TRect CGlFont::GetCharRect( TInt aIdx )
	{
	TRect letterRect;

	if ( iTextureDirection == EGlFontDirectionVertical )
		{
		letterRect.iTl.iX = iLetterSize.iWidth * ( aIdx / iCharsPerCol );
		letterRect.iTl.iY = iLetterSize.iHeight * ( aIdx % iCharsPerCol );
		letterRect.iBr.iX = letterRect.iTl.iX + iLetterSize.iWidth;
		letterRect.iBr.iY = letterRect.iTl.iY + iLetterSize.iHeight;
		}
	else // EGlFontDirectionHorizontal
		{
		letterRect.iTl.iX = iLetterSize.iWidth * ( aIdx % iCharsPerRow );
		letterRect.iTl.iY = iLetterSize.iHeight * ( aIdx / iCharsPerRow );
		letterRect.iBr.iX = letterRect.iTl.iX + iLetterSize.iWidth;
		letterRect.iBr.iY = letterRect.iTl.iY + iLetterSize.iHeight;
		}

	letterRect.iTl.iX += ( iLettersMargins[ aIdx ].iLeft > 0 ? iLettersMargins[ aIdx ].iLeft : 0 );
	letterRect.iBr.iX -= ( iLettersMargins[ aIdx ].iRight > 0 ? iLettersMargins[ aIdx ].iRight : 0 );

	return letterRect;
	}

void CGlFont::DrawText( TPoint aPosition, const TDesC& aText )
	{
	glPushMatrix();
	GlUtils::Translate2D( aPosition );

	for ( TInt i = 0, j = 0; i < aText.Length(); i++ )
		{
		TInt idx = aText[ i ] - iCharacterOffset;

		if ( aText[ i ] == '\r' || aText[ i ] == '\n' )
			{
			glTranslatef( -j, -iLetterSize.iHeight - iLineSpacing, 0 );
			j = 0;
			}

		if ( idx < 0 || idx >= iLettersMargins.Count() )
			{
			continue;
			}
	
		TRect letterRect = GetCharRect( idx );

		if ( iLettersMargins[ idx ].iLeft < 0 )
			{
			glTranslatef( -iLettersMargins[ idx ].iLeft, 0, 0 );
			j += -iLettersMargins[ idx ].iLeft;
			}

		glPushMatrix();
		glScalef( letterRect.Width(), letterRect.Height(), 0 );

		iFontTexture.Use( letterRect );

		( i == 0 ? iRect->Draw() : iRect->Redraw() );

		glPopMatrix();

		glTranslatef( letterRect.Width(), 0, 0 );
		j += letterRect.Width();

		if ( iLettersMargins[ idx ].iRight < 0 )
			{
			glTranslatef( -iLettersMargins[ idx ].iRight, 0, 0 );
			j += -iLettersMargins[ idx ].iRight;
			}
		if ( iCharSpacing )
			{
			glTranslatef( iCharSpacing, 0, 0 );
			j += iCharSpacing;
			}
		}

	iFontTexture.Discard();
	glPopMatrix();
	}

void CGlFont::DrawText( CBitmapContext& /*aGc*/, TPoint aPosition, const TDesC& aText )
    {
    DrawText( aPosition, aText );
    }

void CGlFont::DrawText( CBitmapContext& /*aGc*/, TRect /*aRect*/, const TDesC& /*aText*/, 
               TSGEFontAlignHorizontal /*aHorizontalAlign*/,
               TSGEFontAlignVertical /*aVerticalAlign*/ )
    {
    // not supported
    }


void CGlFont::DrawChar( TText aChar, TBool aDrawOrRedrawTexture, TBool aDiscardTexture )
    {
	TInt idx = aChar - iCharacterOffset;

	if ( idx < 0 || idx >= iLettersMargins.Count() )
		{
		return;
		}
	
	TRect letterRect = GetCharRect( idx );

	glPushMatrix();
	glScalef( letterRect.Width(), letterRect.Height(), 0 );

	iFontTexture.Use( letterRect );
	if ( aDrawOrRedrawTexture )
		{
		iRect->Draw();
		}
	else
		{
		iRect->Redraw();
		}

	glPopMatrix();

	if ( aDiscardTexture )
		{
		iFontTexture.Discard();
		}
    }

