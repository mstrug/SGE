/*
 ============================================================================
 Name		: SGEFont.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEFont implementation
 ============================================================================
 */

#include "SGEFont.h"

EXPORT_C CSGEFont::CSGEFont()
    {
    }

EXPORT_C CSGEFont::~CSGEFont()
    {
    }

EXPORT_C void CSGEFont::BaseConstructL()
    {
    }

EXPORT_C void CSGEFont::DrawText( CBitmapContext &aGc, TPoint aPosition, const TDesC& aText, TRect aDrawRect )
    {
    TRect r( aPosition, TSize( GetTextWidth( aText ), GetMaxHeight() ) );
    if ( aDrawRect.Intersects( r ) )
        {
        DrawText( aGc, aPosition, aText );
        }
    }

EXPORT_C void CSGEFont::DrawText( CBitmapContext &aGc, TRect aRect, const TDesC& aText, TRect aDrawRect, 
                                  TSGEFontAlignHorizontal aHorizontalAlign, TSGEFontAlignVertical aVerticalAlign )
    {
    if ( aDrawRect.Intersects( aRect ) )
        {
        DrawText( aGc, aRect, aText, aHorizontalAlign, aVerticalAlign );
        }
    }

EXPORT_C void CSGEFont::SetLettersHorizontalStep( TInt aHorizontalStep )
    {
    iLettersStep.iWidth = aHorizontalStep;
    }

EXPORT_C void CSGEFont::SetLettersVerticalStep( TInt aVerticalStep )
    {
    iLettersStep.iWidth = aVerticalStep;
    }

EXPORT_C void CSGEFont::SetDivideLines( TBool aDivideLines )
    {
    iDivideLines = aDivideLines;
    }


// end of file
