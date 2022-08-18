/*
 ============================================================================
 Name		: SGEFontGDR.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEFontGDR implementation
 ============================================================================
 */

#include "SGEFontGDR.h"
#include <BAUTILS.H>
#include <W32STD.H>
#include <EIKENV.H>
#include <SGEFile/SGEFile.h>


CSGEFontGDR::CSGEFontGDR()
    {
    // No implementation required
    }

EXPORT_C CSGEFontGDR::~CSGEFontGDR()
    {
    CleanupFont();
    iFontFile.Close();
    }

EXPORT_C CSGEFontGDR* CSGEFontGDR::NewL( const TDesC& aFontFileName, const TDesC& aDestinationPath )
    {
    CSGEFontGDR *self = new (ELeave) CSGEFontGDR();
    CleanupStack::PushL( self );
    self->ConstructL( aFontFileName, aDestinationPath );
    CleanupStack::Pop( self );
    return self;
    }

EXPORT_C CSGEFontGDR* CSGEFontGDR::NewL( CSGEFile &aSGEFile, const TDesC& aFontFileName, const TDesC& aDestinationPath )
    {
    CSGEFontGDR *self = new (ELeave) CSGEFontGDR();
    CleanupStack::PushL( self );
    self->ConstructL( aSGEFile, aFontFileName, aDestinationPath );
    CleanupStack::Pop( self );
    return self;
    }

void CSGEFontGDR::ConstructL( const TDesC& aFontFileName, const TDesC& aDestinationPath )
    {
    CreateFileNameL( aFontFileName, aDestinationPath );
    
    RFs fs;
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );
    
    BaflUtils::EnsurePathExistsL( fs, aDestinationPath );
    
    User::LeaveIfError( BaflUtils::CopyFile( fs, aFontFileName, iFontFile ) );
    
    CleanupStack::PopAndDestroy( &fs );
    
    LoadFontL();
    }

void CSGEFontGDR::ConstructL( CSGEFile &aSGEFile, const TDesC& aFontFileName, const TDesC& aDestinationPath )
    {
    CreateFileNameL( aFontFileName, aDestinationPath );

    aSGEFile.ExtractFileL( aFontFileName, iFontFile );
    
    LoadFontL();
    }

void CSGEFontGDR::CreateFileNameL( const TDesC& aFontFileName, const TDesC& aDestinationPath )
    {
    TParse parse;
    TInt err = parse.Set( aFontFileName, NULL, NULL );

    const TUint KBackslash = '\\';
    RBuf tmpName;
    
    if ( err == KErrBadName && aFontFileName.Length() > 0 && aFontFileName[ 0 ] != KBackslash )
        {
        tmpName.CreateL( aFontFileName.Length() + 1 );
        tmpName.Append( TChar( KBackslash ) );
        tmpName.Append( aFontFileName );
        err = parse.Set( tmpName, NULL, NULL );
        if ( err )
            {
            tmpName.Close();
            User::Leave( err );
            }
        }
    else if ( err != KErrNone )
        {
        User::Leave( err );
        }

    TPtrC name = parse.NameAndExt();

    tmpName.CleanupClosePushL();
    
    iFontFile.CreateL( aDestinationPath.Length() + name.Length() );
    iFontFile.Copy( aDestinationPath );
    iFontFile.Append( name );
    
    CleanupStack::PopAndDestroy( &tmpName );
    }

void CSGEFontGDR::LoadFontL()
    {
    CWsScreenDevice *dev = CEikonEnv::Static()->ScreenDevice();
    
    User::LeaveIfError( dev->AddFile( iFontFile, iFontId ) );
    
    TAlgStyle alg;
    TInt err = dev->GetFontById( iFont, TUid::Uid( iFontId ), alg );
    
    if ( err != KErrNone )
        {
        CleanupFont();
        User::Leave( err );
        }
    }

void CSGEFontGDR::CleanupFont()
    {
    CWsScreenDevice *dev = CEikonEnv::Static()->ScreenDevice();
    dev->ReleaseFont( iFont );
    dev->RemoveFile( iFontId );

    RFs fs;
    if ( fs.Connect() == KErrNone )
        {
        fs.Delete( iFontFile );
        fs.Close();
        }
    }

EXPORT_C const CFont* CSGEFontGDR::Font() const
    {
    return iFont;
    }

TInt CSGEFontGDR::GetTextWidth( const TDesC& aText )
    {
    return iFont->TextWidthInPixels( aText );
    }

TInt CSGEFontGDR::GetMaxHeight()
    {
    return iFont->FontMaxHeight();
    }

void CSGEFontGDR::DrawText( CBitmapContext &aGc, TPoint aPosition, const TDesC& aText )
    {
    aGc.UseFont( iFont );
    aGc.DrawText( aText, aPosition );
    aGc.DiscardFont();
    }

void CSGEFontGDR::DrawText( CBitmapContext &aGc, TRect aRect, const TDesC& aText, 
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
