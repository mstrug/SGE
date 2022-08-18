/*
 ============================================================================
 Name		: SGEFontGraphic.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEFontGraphic implementation
 ============================================================================
 */

#include "SGEFontGraphic.h"
#include <SGEBitmap/SGEBitmapItem.h>
#include <SGEFile/SGEFile.h>
#include <f32file.h>
#include <S32STRM.H>
#include <ImageConversion.h>
#include <GDI.H>


const TInt KLettersMarginsSize = 64;


CSGEFontGraphic::CSGEFontGraphic( TSize aLetterSize, TChar aOffset, TInt aLettersAnalyzerThreshold )
    : iLetterSize( aLetterSize ), iCharacterOffset( aOffset ), iLettersMargins( KLettersMarginsSize ),
      iUseLettersAnalyzer( aLettersAnalyzerThreshold != 0 ), iLettersAnalyzerThreshold( aLettersAnalyzerThreshold )
    {
    }

EXPORT_C CSGEFontGraphic::~CSGEFontGraphic()
    {
    if ( iBitmapsOwned )
        {
        delete iBitmap;
        delete iBitmapMask;
        }
    iCharactersMap.Close();
    iLettersMargins.Close();
    }

EXPORT_C CSGEFontGraphic* CSGEFontGraphic::NewL( const TDesC& aFontFileName, TSize aLetterSize, TChar aOffset, TInt aLettersAnalyzerThreshold )
    {
    CSGEFontGraphic *self = new (ELeave) CSGEFontGraphic( aLetterSize, aOffset, aLettersAnalyzerThreshold );
    CleanupStack::PushL( self );
    self->ConstructL( aFontFileName );
    CleanupStack::Pop( self );
    return self;
    }

EXPORT_C CSGEFontGraphic* CSGEFontGraphic::NewL( CSGEFile &aSGEFile, const TDesC& aFontFileName, TSize aLetterSize, TChar aOffset, TInt aLettersAnalyzerThreshold )
    {
    CSGEFontGraphic *self = new (ELeave) CSGEFontGraphic( aLetterSize, aOffset, aLettersAnalyzerThreshold );
    CleanupStack::PushL( self );
    self->ConstructL( aSGEFile, aFontFileName );
    CleanupStack::Pop( self );
    return self;
    }

EXPORT_C CSGEFontGraphic* CSGEFontGraphic::NewL( CSGEBitmapItem &aBitmapItem, TSize aLetterSize, TChar aOffset, TInt aLettersAnalyzerThreshold )
    {
    CSGEFontGraphic *self = new (ELeave) CSGEFontGraphic( aLetterSize, aOffset, aLettersAnalyzerThreshold );
    CleanupStack::PushL( self );
    self->ConstructL( aBitmapItem );
    CleanupStack::Pop( self );
    return self;
    }

void CSGEFontGraphic::ConstructL( const TDesC& aFontFileName )
    {
    BaseConstructL();

    iLettersMargins.ReserveL( KLettersMarginsSize );
    
    iBitmapsOwned = ETrue;

    RFs fs;
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );
 
    CImageDecoder::TOptions decodingOptions = (CImageDecoder::TOptions) ( CImageDecoder::EOptionNoDither |
                                                                          CImageDecoder::EPreferFastDecode |
                                                                          CImageDecoder::EOptionIgnoreExifMetaData |
                                                                          CImageDecoder::EOptionAlwaysThread );

    CImageDecoder *decoder = CImageDecoder::FileNewL( fs, aFontFileName, decodingOptions );
    CleanupStack::PushL( decoder );

    if ( decoder->FrameInfo().iFlags & decoder->FrameInfo().ETransparencyPossible )
        {
        if ( decoder->FrameInfo().iFlags & decoder->FrameInfo().EAlphaChannel )
            {
            iBitmap = new (ELeave) CFbsBitmap();
            User::LeaveIfError( iBitmap->Create( decoder->FrameInfo().iOverallSizeInPixels, 
                                                 decoder->FrameInfo().iFrameDisplayMode ) );

            iBitmapMask = new (ELeave) CFbsBitmap();
            User::LeaveIfError( iBitmapMask->Create( decoder->FrameInfo().iOverallSizeInPixels, EGray256 ) );
            }
        else
            {
            iBitmap = new (ELeave) CFbsBitmap();
            User::LeaveIfError( iBitmap->Create( decoder->FrameInfo().iOverallSizeInPixels, 
                                                 decoder->FrameInfo().iFrameDisplayMode ) );

            iBitmapMask = new (ELeave) CFbsBitmap();
            User::LeaveIfError( iBitmapMask->Create( decoder->FrameInfo().iOverallSizeInPixels, EGray2 ) );
            }

        TRequestStatus status = KRequestPending;
        decoder->Convert( &status, *iBitmap );
        User::WaitForRequest( status );
        User::LeaveIfError( status.Int() );

        status = KRequestPending;
        decoder->Convert( &status, *iBitmapMask );
        User::WaitForRequest( status );
        User::LeaveIfError( status.Int() );
        }
    else
        {
        iBitmap = new (ELeave) CFbsBitmap();
        User::LeaveIfError( iBitmap->Create( decoder->FrameInfo().iOverallSizeInPixels, 
                                             decoder->FrameInfo().iFrameDisplayMode ) );

        TRequestStatus status = KRequestPending;
        decoder->Convert( &status, *iBitmap );
        User::WaitForRequest( status );
        User::LeaveIfError( status.Int() );
        }

    CleanupStack::PopAndDestroy( decoder );
    CleanupStack::PopAndDestroy( &fs );

    if ( iUseLettersAnalyzer )
        {
        AnalyzeLettersL();
        }
    }

void CSGEFontGraphic::ConstructL( CSGEFile &aSGEFile, const TDesC& aFontFileName )
    {
    BaseConstructL();

    iLettersMargins.ReserveL( KLettersMarginsSize );

    iBitmapsOwned = ETrue;

    RFs fs;
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );

    RBuf8 fileData;
    TInt size;
    
    RReadStream file = aSGEFile.OpenFileL( aFontFileName, &size );
    file.PushL();
    fileData.CreateL( size );
    file.Pop();
    CleanupClosePushL( fileData );
    file.PushL();
    file.ReadL( fileData, size );
    CleanupStack::PopAndDestroy( &file );

    CImageDecoder::TOptions decodingOptions = (CImageDecoder::TOptions) ( CImageDecoder::EOptionNoDither |
                                                                          CImageDecoder::EPreferFastDecode |
                                                                          CImageDecoder::EOptionIgnoreExifMetaData |
                                                                          CImageDecoder::EOptionAlwaysThread );

    CImageDecoder *decoder = CImageDecoder::DataNewL( fs, fileData, decodingOptions );
    CleanupStack::PushL( decoder );

    if ( decoder->FrameInfo().iFlags & decoder->FrameInfo().ETransparencyPossible )
        {
        if ( decoder->FrameInfo().iFlags & decoder->FrameInfo().EAlphaChannel )
            {
            iBitmap = new (ELeave) CFbsBitmap();
            User::LeaveIfError( iBitmap->Create( decoder->FrameInfo().iOverallSizeInPixels, 
                                                 decoder->FrameInfo().iFrameDisplayMode ) );

            iBitmapMask = new (ELeave) CFbsBitmap();
            User::LeaveIfError( iBitmapMask->Create( decoder->FrameInfo().iOverallSizeInPixels, EGray256 ) );
            }
        else
            {
            iBitmap = new (ELeave) CFbsBitmap();
            User::LeaveIfError( iBitmap->Create( decoder->FrameInfo().iOverallSizeInPixels, 
                                                 decoder->FrameInfo().iFrameDisplayMode ) );

            iBitmapMask = new (ELeave) CFbsBitmap();
            User::LeaveIfError( iBitmapMask->Create( decoder->FrameInfo().iOverallSizeInPixels, EGray2 ) );
            }

        TRequestStatus status = KRequestPending;
        decoder->Convert( &status, *iBitmap );
        User::WaitForRequest( status );
        User::LeaveIfError( status.Int() );

        status = KRequestPending;
        decoder->Convert( &status, *iBitmapMask );
        User::WaitForRequest( status );
        User::LeaveIfError( status.Int() );
        }
    else
        {
        iBitmap = new (ELeave) CFbsBitmap();
        User::LeaveIfError( iBitmap->Create( decoder->FrameInfo().iOverallSizeInPixels, 
                                             decoder->FrameInfo().iFrameDisplayMode ) );

        TRequestStatus status = KRequestPending;
        decoder->Convert( &status, *iBitmap );
        User::WaitForRequest( status );
        User::LeaveIfError( status.Int() );
        }

    CleanupStack::PopAndDestroy( decoder );
    CleanupStack::PopAndDestroy( &fileData );
    CleanupStack::PopAndDestroy( &fs );
    
    if ( iUseLettersAnalyzer )
        {
        AnalyzeLettersL();
        }
    }

void CSGEFontGraphic::ConstructL( CSGEBitmapItem &aBitmapItem )
    {
    BaseConstructL();

    iLettersMargins.ReserveL( KLettersMarginsSize );

    iBitmap = aBitmapItem.Bitmap();
    iBitmapMask = aBitmapItem.BitmapMask();
    
    ASSERT( iBitmap->SizeInPixels().iWidth % iLetterSize.iWidth == 0 );
    
    if ( iUseLettersAnalyzer )
        {
        AnalyzeLettersL();
        }
    }

EXPORT_C void CSGEFontGraphic::SetInvertMask( TBool aInvertMask )
    {
    iInvertMask = aInvertMask;
    }

EXPORT_C void CSGEFontGraphic::AddCharacterMap( TChar aCharacterCode, TChar aNewCharacterCode )
    {
    iCharactersMap.Insert( aCharacterCode, aNewCharacterCode );
    }

EXPORT_C void CSGEFontGraphic::TransformCurves( TSGEBitmapFunctionCurves aFunction )
    {
    ASSERT( iBitmap );
    
    SGEBitmapUtils::TransformCurves( aFunction, *iBitmap );
    }

EXPORT_C void CSGEFontGraphic::SetUseLettersAnalyzer( TBool aUse )
    {
    iUseLettersAnalyzer = aUse;
    }

EXPORT_C TSGEFontGraphicLetterMargins* CSGEFontGraphic::LetterMargins( TChar aLetter )
    {
    if ( (TUint) iLettersMargins.Count() > aLetter - iCharacterOffset )
        {
        return &iLettersMargins[ aLetter - iCharacterOffset ];
        }
    return NULL;
    }

EXPORT_C CSGEFontGraphic* CSGEFontGraphic::DuplicateL()
    {
    CSGEFontGraphic *font = new (ELeave) CSGEFontGraphic( iLetterSize, iCharacterOffset, iLettersAnalyzerThreshold );
    CleanupStack::PushL( font );
    
    font->iBitmapsOwned = iBitmapsOwned;
    font->iInvertMask = iInvertMask;
    font->iLettersStep = iLettersStep;
    
    if ( !iBitmapsOwned )
        {
        font->iBitmap = iBitmap;
        font->iBitmapMask = iBitmapMask;
        }
    else
        {
        font->iBitmap = SGEBitmapUtils::DuplicateFbsBitmapL( *iBitmap );
        font->iBitmapMask = SGEBitmapUtils::DuplicateFbsBitmapL( *iBitmapMask );
        }

    THashMapIter< TUint, TUint > iter( iCharactersMap );
    iter.Reset();
    while ( iter.NextKey() )
        {
        font->iCharactersMap.InsertL( *iter.CurrentKey(), *iter.CurrentValue() );
        }
    
    for ( TInt i = 0; i < iLettersMargins.Count(); i++ )
        {
        font->iLettersMargins.AppendL( iLettersMargins[ i ] );
        }
    
    CleanupStack::Pop( font );
    
    return font;
    }

EXPORT_C CSGEFontGraphic* CSGEFontGraphic::DuplicateL( CSGEBitmapItem &aNewBitmap )
    {
    CSGEFontGraphic *font = new (ELeave) CSGEFontGraphic( iLetterSize, iCharacterOffset, iLettersAnalyzerThreshold );
    CleanupStack::PushL( font );
    
    font->iBitmapsOwned = EFalse;
    font->iInvertMask = iInvertMask;
    font->iLettersStep = iLettersStep;
    
    font->iBitmap = aNewBitmap.Bitmap();
    font->iBitmapMask = aNewBitmap.BitmapMask();

    THashMapIter< TUint, TUint > iter( iCharactersMap );
    iter.Reset();
    while ( iter.NextKey() )
        {
        font->iCharactersMap.InsertL( *iter.CurrentKey(), *iter.CurrentValue() );
        }
    
    for ( TInt i = 0; i < iLettersMargins.Count(); i++ )
        {
        font->iLettersMargins.AppendL( iLettersMargins[ i ] );
        }
    
    CleanupStack::Pop( font );
    
    return font;
    }

TInt CSGEFontGraphic::CalculateHorizontalPosition( const TDesC& aText, TRect aRect, TSGEFontAlignHorizontal aHorizontalAlign )
    {
    TInt w = GetTextWidth( aText );

    if ( aHorizontalAlign == ESGEFontAlignHorizontalLeft )
        {
        return aRect.iTl.iX;
        }
    else if ( aHorizontalAlign == ESGEFontAlignHorizontalRight )
        {
        return aRect.iBr.iX - w;
        }
    else if ( aHorizontalAlign == ESGEFontAlignHorizontalJustify )
        {
        if ( aText.Length() > 1 )
            {
            iLettersStep.iWidth = ( aRect.Width() - w ) / ( aText.Length() - 1 );
            }
        return aRect.iTl.iX + ( aRect.iBr.iX - aRect.iTl.iX ) / 2 - w / 2;
        }
    else // ESGEFontAlignHorizontalCenter
        {
        return aRect.iTl.iX + ( aRect.iBr.iX - aRect.iTl.iX ) / 2 - w / 2;
        }
    }

TInt CSGEFontGraphic::CalculateVerticalPosition( const TDesC& aText, TRect aRect, TSGEFontAlignVertical aVerticalAlign )
    {
    TInt h = GetMaxHeight();
    TInt lines = CountLines( aText );

    if ( aVerticalAlign == ESGEFontAlignVerticalTop )
        {
        return aRect.iTl.iY + h; // * lines;
        }
    else if ( aVerticalAlign == ESGEFontAlignVerticalBottom )
        {
        return aRect.iBr.iY;
        }
    else // ESGEFontAlignVerticalCenter
        {
        return aRect.iTl.iY + ( aRect.iBr.iY - aRect.iTl.iY ) / 2 + ( h * lines ) / 2;
        }
    }

TInt CSGEFontGraphic::CountLines( const TDesC& aText )
    {
    TInt lines = 1;
    for ( TInt i = 0; i < aText.Length(); i++ )
        {
        if ( aText[ i ] == '\n' )
            {
            lines++;
            }
        }
    return lines;
    }

TInt CSGEFontGraphic::LongestLineWidth( const TDesC& aText )
    {
    TInt w = 0, j = 0;
    for ( TInt i = 0; i < aText.Length(); i++ )
        {
        if ( aText[ i ] != '\n' )
            {
            j++;
            }
        else
            {
            if ( j > w )
                {
                w = j;
                }
            j = 0;
            }
        }
    if ( j > w )
        {
        w = j;
        }
    return w;
    }

TRect CSGEFontGraphic::CalculateLetterRect( TChar aLetter )
    {
    TSize size = iBitmap->SizeInPixels();
    
    TInt chIdx = aLetter - iCharacterOffset;
    ASSERT( chIdx >= 0 );

    TInt yDiv = ( iLetterSize.iHeight * chIdx ) % size.iHeight;
    TInt xDiv = ( ( iLetterSize.iHeight * chIdx ) / size.iHeight ) * iLetterSize.iWidth;

    return TRect( TPoint( xDiv, yDiv ), iLetterSize );
    }

void CSGEFontGraphic::AnalyzeLettersL()
    {
    TChar ch( iCharacterOffset );
    
    TSize size = iBitmap->SizeInPixels();
    
    iLettersMargins.Close();
    
    for ( TInt i = 0; i < size.iHeight / iLetterSize.iHeight; i++ )
        {
        for ( TInt j = 0; j < size.iWidth / iLetterSize.iWidth; j++ )
            {
            TSGEFontGraphicLetterMargins m = { 0, 0 };
            
            iLettersMargins.AppendL( m );

            if ( ch != ' ' )
                { // not space
                AnalyzeLetter( ch );
                }

            ch += 1;
            }    
        }
    }

void CSGEFontGraphic::AnalyzeLetter( TChar aLetter )
    {
    TRect r = CalculateLetterRect( aLetter );
    
    TInt condition = iLettersAnalyzerThreshold;
    
    CFbsBitmap *bmp = ( iBitmapMask ? iBitmapMask : iBitmap );
    
    TBool br = EFalse;
    for ( TInt i = r.iTl.iX; i < r.iBr.iX && !br; i++ )
        {
        for ( TInt j = r.iTl.iY; j < r.iBr.iY; j++ )
            {
            TRgb color;
            bmp->GetPixel( color, TPoint( i, j ) );
            if ( color.Red() > condition || color.Blue() > condition || color.Green() > condition )
                {
                iLettersMargins[ aLetter - iCharacterOffset ].iLeft = i - r.iTl.iX;
                br = ETrue;
                break;
                }
            }
        }
    
    br = EFalse;
    for ( TInt i = r.iBr.iX - 1; i >= r.iTl.iX && !br; i-- )
        {
        for ( TInt j = r.iTl.iY; j < r.iBr.iY; j++ )
            {
            TRgb color;
            bmp->GetPixel( color, TPoint( i, j ) );
            if ( color.Red() > condition || color.Blue() > condition || color.Green() > condition )
                {
                iLettersMargins[ aLetter - iCharacterOffset ].iRight = r.iBr.iX - 1 - i;
                br = ETrue;
                break;
                }
            }
        }    
    }

TInt CSGEFontGraphic::GetTextWidth( const TDesC& aText )
    {
    if ( iUseLettersAnalyzer )
        {
        TInt w = 0;
        TInt j = 0;
        for ( TInt i = 0; i < aText.Length(); i++ )
            {
            TChar ch = aText[ i ];
            if ( ch == '\n' )
                {
                if ( w < j )
                    {
                    w = j;
                    }
                j = 0;
                continue;
                }            
            TUint *map = iCharactersMap.Find( ch );
            if ( map )
                {
                ch = *map;
                }
            
            j += iLettersStep.iWidth + iLetterSize.iWidth - ( iLettersMargins[ ch - iCharacterOffset ].iLeft + 
                                                              iLettersMargins[ ch - iCharacterOffset ].iRight );
            }
        if ( w < j )
            {
            w = j;
            }
        return w;
        }
    else
        {
        return aText.Length() * ( iLetterSize.iWidth + iLettersStep.iWidth );
        }
    }

TInt CSGEFontGraphic::GetMaxHeight()
    {
    return iLetterSize.iHeight;
    }

void CSGEFontGraphic::DrawText( CBitmapContext &aGc, TPoint aPosition, const TDesC& aText )
    {
    DrawText( aGc, aPosition, aText, TRect() );
    }

void CSGEFontGraphic::DrawText( CBitmapContext &aGc, TRect aRect, const TDesC& aText, 
                                  TSGEFontAlignHorizontal aHorizontalAlign,
                                  TSGEFontAlignVertical aVerticalAlign )
    {
    DrawText( aGc, aRect, aText, TRect(), aHorizontalAlign, aVerticalAlign );
    }

EXPORT_C void CSGEFontGraphic::DrawText( CBitmapContext &aGc, TPoint aPosition, const TDesC& aText, TRect aDrawRect )
    {
    TPoint pos( aPosition );
    pos.iY -= iLetterSize.iHeight;
    TSize size = iBitmap->SizeInPixels();

    for ( TInt i = 0; i < aText.Length(); i++ )
        {
        TChar ch = aText[ i ];
        if ( ch == '\n' )
            {
            pos.iY += iLetterSize.iHeight;
            pos.iX = aPosition.iX;
            continue;
            }
        TUint *map = iCharactersMap.Find( ch );
        if ( map )
            {
            ch = *map;
            }

        TRect r = CalculateLetterRect( ch );
        
        if ( iUseLettersAnalyzer )
            {
            r.iTl.iX += iLettersMargins[ ch - iCharacterOffset ].iLeft;
            r.iBr.iX -= iLettersMargins[ ch - iCharacterOffset ].iRight;
            }

        TBool dr = EFalse;
        if ( !aDrawRect.IsEmpty() && aDrawRect.Intersects( TRect( pos, r.Size() ) ) )
            {
            dr = ETrue;
            TRect tr( pos, r.Size() );
            tr.Intersection( aDrawRect );
            TPoint newPos = tr.iTl - pos;
            r.iTl += newPos;
            r.SetSize( tr.Size() );
            pos += newPos;
            }
        
        if ( aDrawRect.IsEmpty() || dr )
            {
            if ( iBitmapMask )
                {
                aGc.BitBltMasked( pos, iBitmap, r, iBitmapMask, iInvertMask );
                }
            else
                {
                aGc.BitBlt( pos, iBitmap, r );
                }
            }
        
        pos.iX += r.Width() + iLettersStep.iWidth;
        }
    }

EXPORT_C void CSGEFontGraphic::DrawText( CBitmapContext &aGc, TRect aRect, const TDesC& aText, TRect aDrawRect, 
                                         TSGEFontAlignHorizontal aHorizontalAlign, 
                                         TSGEFontAlignVertical aVerticalAlign )
    {
    TSize tmpStep = iLettersStep;
    TPoint pos( CalculateHorizontalPosition( aText, aRect, aHorizontalAlign ),
                CalculateVerticalPosition( aText, aRect, aVerticalAlign ) );

    if ( !iDivideLines )
        {
        DrawText( aGc, pos, aText, aDrawRect );
        }
    else
        {
        // @todo: iDivideLine
        }

    iLettersStep = tmpStep;
    }


// End of file
