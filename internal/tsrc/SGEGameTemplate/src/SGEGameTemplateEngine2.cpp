/*
 ============================================================================
  Name        : SGEGameTemplateEngine2.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#include "SGEGameTemplateEngine2.h"
#include "SGEGameTemplateCommonObjects.h"
#include "SGEGameTemplateIdBitmap.h"
#include "SGEGameTemplateIdFont.h"
#include "SGEGameTemplateIdString.h"
#include "SGEGameTemplateIdFile.h"
#include "SGEGameTemplateIdAudio.h"
#include <SGEFont/SGEFontProvider.h>
#include <SGEFont/SGEFontSystem.h>
#include <SGEString/SGEStringProvider.h>
#include <SGEFile/SGEFileProvider.h>
#include <SGEBitmap/SGEBitmapProvider.h>
#include <SGEBitmap/SGEBitmapItem.h>
#include <SGEAudio/SGEAudioFile.h>
#include <SGEAudio/SGEAudioProvider.h>
#include <FBS.H>


// -----------------------------------------------------------------------------
// CSGEGameTemplateEngine2::CSGEGameTemplateEngine2()
// -----------------------------------------------------------------------------
//
CSGEGameTemplateEngine2::CSGEGameTemplateEngine2( MSGEEngineObserver& aObserver, 
                                                  CSGEEngineCommonObjects *aCommonObjects )
    : CSGEEngine( aObserver, aCommonObjects )
    {
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateEngine2::NewL()
// -----------------------------------------------------------------------------
//
CSGEGameTemplateEngine2* CSGEGameTemplateEngine2::NewL( MSGEEngineObserver& aObserver, 
                                                        CSGEEngineCommonObjects *aCommonObjects )
    {
    CSGEGameTemplateEngine2 *self = new (ELeave) CSGEGameTemplateEngine2( aObserver, aCommonObjects );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateEngine2::ConstructL()
// -----------------------------------------------------------------------------
//
void CSGEGameTemplateEngine2::ConstructL()
    {
    BaseConstructL();

    // all resources are loaded by Engine 1
    CSGEBitmapItem *bi = CommonObjects().BitmapProvider()->BitmapItem( KSGEGameTemplateIdBitmapTest2 );
    bi->TransformCurves( BitmapTransformFunction );
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateEngine2::CommonObjects()
// -----------------------------------------------------------------------------
//
CSGEGameTemplateCommonObjects& CSGEGameTemplateEngine2::CommonObjects() const
    {
    return static_cast<CSGEGameTemplateCommonObjects&>( *iCommonObjects );
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateEngine2::BitmapTransformFunction()
// -----------------------------------------------------------------------------
//
void CSGEGameTemplateEngine2::BitmapTransformFunction( TUint8& aRed, TUint8& aGreen, TUint8& aBlue )
    {
    if ( aRed == 255 && aGreen == 0 && aBlue == 0 )
        {
        aRed = 0;
        aBlue = 255;
        }
    else if ( aRed == 255 && aGreen == 255 && aBlue == 0 )
        {
        aGreen = 0;
        }
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateEngine2::Id()
// -----------------------------------------------------------------------------
//
TUint CSGEGameTemplateEngine2::Id() const
    {
    return KSGEGameTemplateEngine2Id;
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateEngine2::Draw()
// -----------------------------------------------------------------------------
//
void CSGEGameTemplateEngine2::Draw( CBitmapContext* aGc, const TRect aDrawRect )
    {
    aGc->Reset();

    // Clears the screen
    aGc->Clear( aDrawRect );
    
    CSGEFont *chiller = CommonObjects().FontProvider()->GetFont( KSGEGameTemplateIdFontChiller, 17 );
    ASSERT( chiller );

    CSGEBitmapItem *bi = CommonObjects().BitmapProvider()->BitmapItem( KSGEGameTemplateIdBitmapTest2 );
    TSize s = bi->Bitmap()->SizeInPixels();
    CommonObjects().BitmapProvider()->BitBlt( *aGc, aDrawRect.iBr - s, KSGEGameTemplateIdBitmapTest2 );
    chiller->DrawText( *aGc, aDrawRect, 
                     CommonObjects().StringProvider()->GetString( R_STRING_ENGINE_2_TEXT, ETrue ),
                     ESGEFontAlignHorizontalCenter, ESGEFontAlignVerticalCenter );

    CSGEFont *arial = CommonObjects().FontProvider()->GetFont( KSGEGameTemplateIdFontArial, 10 );
    ASSERT( arial );
    aGc->SetPenColor( TRgb( 0, 0, 0 ) );
    aGc->SetPenStyle( CBitmapContext::ESolidPen );
    arial->DrawText( *aGc, aDrawRect, 
                     CommonObjects().StringProvider()->GetString( R_STRING_APPLICATION_NAME, ETrue ),
                     ESGEFontAlignHorizontalCenter, ESGEFontAlignVerticalTop );
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateEngine2::Update()
// -----------------------------------------------------------------------------
//
TBool CSGEGameTemplateEngine2::Update( const TTimeIntervalMicroSeconds32 /*aTimeIntercal*/ )
    {
    return !iPause;
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateEngine2::KeyEvent()
// -----------------------------------------------------------------------------
//
TBool CSGEGameTemplateEngine2::KeyEvent( const TKeyEvent &aKeyEvent )
    {
    if ( aKeyEvent.iScanCode == EStdKeyEscape )
        {
        iObserver.EngineClose( Id(), KErrNone );
        return ETrue;
        }
    else if ( aKeyEvent.iScanCode == EStdKeyLeftArrow )
        {
        CommonObjects().AudioProvider()->Play( KSGEGameTemplateIdAudio1, 1 );

        iObserver.EngineSwitch( Id() );
        
        return ETrue;
        }
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateEngine2::SetPause()
// -----------------------------------------------------------------------------
//
void CSGEGameTemplateEngine2::SetPause( TBool aPause )
    {
    iPause = aPause;
    }


// End of file
