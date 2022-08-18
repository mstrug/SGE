/*
 ============================================================================
 Name		: SGEFontSystem.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEFontSystem implementation
 ============================================================================
 */

#include "SGEFontSystem.h"
#include <EIKENV.H>
#include <GDI.H>


CSGEFontSystem::CSGEFontSystem()
    {
    // No implementation required
    }

EXPORT_C CSGEFontSystem::~CSGEFontSystem()
    {
    }

EXPORT_C CSGEFontSystem* CSGEFontSystem::NewL( const TDesC &aTypefaceName, TInt aHeight, TBool aItalic, TBool aBold )
    {
    CSGEFontSystem *self = new (ELeave) CSGEFontSystem();
    CleanupStack::PushL( self );
    self->ConstructL( aTypefaceName, aHeight, aBold, aItalic );
    CleanupStack::Pop( self );
    return self;
    }

void CSGEFontSystem::ConstructL( const TDesC &aTypefaceName, TInt aHeight, TBool aBold, TBool aItalic )
    {
    TFontSpec fontSpec( aTypefaceName, aHeight );
    fontSpec.iFontStyle.SetPosture( aItalic ? EPostureItalic : EPostureUpright );
    fontSpec.iFontStyle.SetStrokeWeight( aBold ? EStrokeWeightBold : EStrokeWeightNormal );

    User::LeaveIfError( CEikonEnv::Static()->ScreenDevice()->GetNearestFontToDesignHeightInPixels( iFont, fontSpec ) );
    }


EXPORT_C CSGEFontSystem* CSGEFontSystem::NewL( const TDesC &aTypefaceName, TInt aHeight, TFontStyle aFontStyle )
    {
    CSGEFontSystem *self = new (ELeave) CSGEFontSystem();
    CleanupStack::PushL( self );
    self->ConstructL( aTypefaceName, aHeight, aFontStyle );
    CleanupStack::Pop( self );
    return self;
    }

void CSGEFontSystem::ConstructL( const TDesC &aTypefaceName, TInt aHeight, TFontStyle aFontStyle )
    {
    TFontSpec fontSpec( aTypefaceName, aHeight );
    fontSpec.iFontStyle = aFontStyle;

    User::LeaveIfError( CEikonEnv::Static()->ScreenDevice()->GetNearestFontToDesignHeightInPixels( iFont, fontSpec ) );
    }

EXPORT_C CSGEFontSystem* CSGEFontSystem::NewL( const TLogicalFont &aLogicalFont )
    {
    CSGEFontSystem *self = new (ELeave) CSGEFontSystem();
    CleanupStack::PushL( self );
    self->ConstructL( aLogicalFont );
    CleanupStack::Pop( self );
    return self;
    }

void CSGEFontSystem::ConstructL( const TLogicalFont &aLogicalFont )
    {
    iFont = const_cast< CFont* >( CEikonEnv::Static()->Font( aLogicalFont ) );
    }

EXPORT_C CSGEFontSystem* CSGEFontSystem::NewL( TSGEFontSystemStandard aStandardFont )
    {
    CSGEFontSystem *self = new (ELeave) CSGEFontSystem();
    CleanupStack::PushL( self );
    self->ConstructL( aStandardFont );
    CleanupStack::Pop( self );
    return self;
    }

void CSGEFontSystem::ConstructL( TSGEFontSystemStandard aStandardFont )
    {
    switch ( aStandardFont )
        {
        case ESGEFontSystemStandardAnnotation:
                iFont = const_cast< CFont* >( CEikonEnv::Static()->AnnotationFont() );
            break;
        case ESGEFontSystemStandardTitle:
                iFont = const_cast< CFont* >( CEikonEnv::Static()->TitleFont() );
            break;
        case ESGEFontSystemStandardLegend:
                iFont = const_cast< CFont* >( CEikonEnv::Static()->LegendFont() );
            break;
        case ESGEFontSystemStandardSymbol:
                iFont = const_cast< CFont* >( CEikonEnv::Static()->SymbolFont() );
            break;
        case ESGEFontSystemStandardDense:
                iFont = const_cast< CFont* >( CEikonEnv::Static()->DenseFont() );
            break;
        default:
                User::Leave( KErrNotSupported );
        }
    }

EXPORT_C const CFont* CSGEFontSystem::Font() const
    {
    return iFont;
    }

TInt CSGEFontSystem::GetTextWidth( const TDesC& aText )
    {
    return iFont->TextWidthInPixels( aText );
    }

TInt CSGEFontSystem::GetMaxHeight()
    {
    return iFont->FontMaxHeight();
    }

void CSGEFontSystem::DrawText( CBitmapContext &aGc, TPoint aPosition, const TDesC& aText )
    {
    aGc.UseFont( iFont );
    aGc.DrawText( aText, aPosition );
    aGc.DiscardFont();
    }

void CSGEFontSystem::DrawText( CBitmapContext &aGc, TRect aRect, const TDesC& aText, 
                               TSGEFontAlignHorizontal aHorizontalAlign,
                               TSGEFontAlignVertical aVerticalAlign )
    {
    aGc.UseFont( iFont );
    CGraphicsContext::TTextAlign alg;
    switch ( aHorizontalAlign )
        {
        case ESGEFontAlignHorizontalCenter:
                alg = CGraphicsContext::ECenter;
            break;
        case ESGEFontAlignHorizontalRight:
                alg = CGraphicsContext::ERight;
            break;
        default:
                alg = CGraphicsContext::ELeft;
        }
    
    TInt offset;
    switch ( aVerticalAlign )
        {
        case ESGEFontAlignVerticalTop:
                offset = GetMaxHeight();
            break;
        case ESGEFontAlignVerticalBottom:
                offset = aRect.Height() - iFont->DescentInPixels();
            break;
        default:
                offset = aRect.Height() / 2 + GetMaxHeight() / 2;
        }
    
    aGc.DrawText( aText, aRect, offset, alg );
    aGc.DiscardFont();
    }

// end of file
