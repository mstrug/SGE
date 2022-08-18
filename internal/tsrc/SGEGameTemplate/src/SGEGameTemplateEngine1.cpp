/*
 ============================================================================
  Name        : SGEGameTemplateEngine1.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#include "SGEGameTemplateEngine1.h"
#include "SGEGameTemplateCommonObjects.h"
#include "SGEGameTemplateIdBitmap.h"
#include "SGEGameTemplateIdFont.h"
#include "SGEGameTemplateIdString.h"
#include "SGEGameTemplateIdFile.h"
#include "SGEGameTemplateIdAudio.h"
#include <SGEFont/SGEFontProvider.h>
#include <SGEFont/SGEFontSystem.h>
#include <SGEFont/SGEFontGraphic.h>
#include <SGEFont/SGEFontGDR.h>
#include <SGEString/SGEStringProvider.h>
#include <SGEFile/SGEFileProvider.h>
#include <SGEBitmap/SGEBitmapProvider.h>
#include <SGEAudio/SGEAudioFile.h>
#include <SGEAudio/SGEAudioProvider.h>


// -----------------------------------------------------------------------------
// CSGEGameTemplateEngine1::CSGEGameTemplateEngine1()
// -----------------------------------------------------------------------------
//
CSGEGameTemplateEngine1::CSGEGameTemplateEngine1( MSGEEngineObserver& aObserver, 
                                                  CSGEEngineCommonObjects *aCommonObjects )
    : CSGEEngine( aObserver, aCommonObjects )
    {
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateEngine1::NewL()
// -----------------------------------------------------------------------------
//
CSGEGameTemplateEngine1* CSGEGameTemplateEngine1::NewL( MSGEEngineObserver& aObserver, 
                                                        CSGEEngineCommonObjects *aCommonObjects )
    {
    CSGEGameTemplateEngine1 *self = new (ELeave) CSGEGameTemplateEngine1( aObserver, aCommonObjects );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateEngine1::ConstructL()
// -----------------------------------------------------------------------------
//
void CSGEGameTemplateEngine1::ConstructL()
    {
    BaseConstructL();

    // adding SGEFile
    TPtrC file_sge1 = CommonObjects().FileProvider()->AppendToPrivateFolder( KSGEGameTemplateSGEFile1() );
    CommonObjects().FileProvider()->AddSGEFileL( file_sge1, KSGEGameTemplateIdFile1 );

    // adding strings
    CommonObjects().StringProvider()->AddStringL( _L("Loading..."), KSGEGameTemplateIdStringLoading );
    TPtrC file_lang1 = CommonObjects().FileProvider()->AppendToPrivateFolder( KSGEGameTemplateStringFile1() );
    CommonObjects().StringProvider()->AddFileL( file_lang1 );
    
    // loading Arial font
    CSGEFontSystem *font = CSGEFontSystem::NewL( _L("Arial"), 10 );
    CommonObjects().FontProvider()->AddFontLD( font, KSGEGameTemplateIdFontArial, 10 );
    TPtrC file_gdrfont = CommonObjects().FileProvider()->AppendToPrivateFolder( 
                                                                    KSGEGameTemplateFontFileJFRockSolid18() );
    CSGEFontGDR *gdrfont = CSGEFontGDR::NewL( file_gdrfont );
    CommonObjects().FontProvider()->AddFontLD( gdrfont, KSGEGameTemplateIdFontJFRockSolid, 18 );
    
    // audio will be loaded after all bitmaps are loaded 
            
    // loading images
    TPtrC file_img1 = CommonObjects().FileProvider()->AppendToPrivateFolder( KSGEGameTemplateBitmapTestFile() );
    CommonObjects().BitmapProvider()->AddImageToLoadL( file_img1, KSGEGameTemplateIdBitmapTest );
    TPtrC file_img2 = CommonObjects().FileProvider()->AppendToPrivateFolder( KSGEGameTemplateBitmapFontFile() );
    CommonObjects().BitmapProvider()->AddImageToLoadL( file_img2, KSGEGameTemplateIdBitmapFont1 );
    CommonObjects().BitmapProvider()->AddImageToLoadL( 
                                      CommonObjects().FileProvider()->GetSGEFileL( KSGEGameTemplateIdFile1 ),
                                      KSGEGameTemplateBitmapTest2File(), KSGEGameTemplateIdBitmapTest2 );
    CommonObjects().BitmapProvider()->StartLoadingImagesL();
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateEngine1::Construct2L()
// -----------------------------------------------------------------------------
//
void CSGEGameTemplateEngine1::Construct2L()
    {
    CSGEFontGraphic *font = CSGEFontGraphic::NewL( 
                              *CommonObjects().BitmapProvider()->BitmapItem( KSGEGameTemplateIdBitmapFont1 ), 
                              TSize( 10, 17 ) );
    CommonObjects().FontProvider()->AddFontLD( font, KSGEGameTemplateIdFontChiller, 17 );

    // all images are loaded, now loading audio files
    TPtrC file_wav1 = CommonObjects().FileProvider()->AppendToPrivateFolder( KSGEGameTemplateAudioFile1() );
    TSGEAudioFile audioFile( file_wav1, KSGEGameTemplateIdAudio1, 
                             ESGEAudioGroupAny, ESGEAudioFileTypeWAV );
    CommonObjects().AudioProvider()->LoadFileL( audioFile );
    
    TPtrC file_sge1 = CommonObjects().FileProvider()->AppendToPrivateFolder( KSGEGameTemplateSGEFile1() );
    TSGEAudioFile audioFile2( KSGEGameTemplateAudioFile2(), KSGEGameTemplateIdAudio2, 
                              ESGEAudioGroupAny, ESGEAudioFileTypeWAV );
    CommonObjects().AudioProvider()->LoadFileL( file_sge1, audioFile2 );
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateEngine1::Construct3L()
// -----------------------------------------------------------------------------
//
void CSGEGameTemplateEngine1::Construct3L()
    {
    // all resources are loaded, changing engine state to normal 
    iEngineState = ENormal;
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateEngine1::CommonObjects()
// -----------------------------------------------------------------------------
//
CSGEGameTemplateCommonObjects& CSGEGameTemplateEngine1::CommonObjects() const
    {
    return static_cast<CSGEGameTemplateCommonObjects&>( *iCommonObjects );
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateEngine1::Id()
// -----------------------------------------------------------------------------
//
TUint CSGEGameTemplateEngine1::Id() const
    {
    return KSGEGameTemplateEngine1Id;
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateEngine1::Draw()
// -----------------------------------------------------------------------------
//
void CSGEGameTemplateEngine1::Draw( CBitmapContext* aGc, const TRect aDrawRect )
    {
    aGc->Reset();

    // Clears the screen
    aGc->Clear( aDrawRect );
    
    CSGEFont *arial = CommonObjects().FontProvider()->GetFont( KSGEGameTemplateIdFontArial, 10 );
    ASSERT( arial );
    aGc->SetPenStyle( CBitmapContext::ESolidPen );
    
    if ( iEngineState == ELoading )
        {
        arial->DrawText( *aGc, aDrawRect, 
                         CommonObjects().StringProvider()->GetString( KSGEGameTemplateIdStringLoading ),
                         ESGEFontAlignHorizontalCenter, ESGEFontAlignVerticalCenter );
        }
    else
        {
        CommonObjects().BitmapProvider()->BitBlt( *aGc, aDrawRect.iTl, KSGEGameTemplateIdBitmapTest );
        
        aGc->SetPenColor( TRgb( 0, 0, 0 ) );
        arial->DrawText( *aGc, aDrawRect, 
                         CommonObjects().StringProvider()->GetString( R_STRING_APPLICATION_NAME, ETrue ),
                         ESGEFontAlignHorizontalCenter, ESGEFontAlignVerticalBottom );

        aGc->SetPenColor( TRgb( 160, 15, 50 ) );
        CSGEFont *jfrock = CommonObjects().FontProvider()->GetFont( KSGEGameTemplateIdFontJFRockSolid, 18 );
        ASSERT( jfrock );
        jfrock->DrawText( *aGc, aDrawRect, 
                          CommonObjects().StringProvider()->GetString( R_STRING_ENGINE_1_TEXT, ETrue ),
                          ESGEFontAlignHorizontalCenter, ESGEFontAlignVerticalCenter );
        }
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateEngine1::Update()
// -----------------------------------------------------------------------------
//
TBool CSGEGameTemplateEngine1::Update( const TTimeIntervalMicroSeconds32 /*aTimeIntercal*/ )
    {
    return !iPause;
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateEngine1::KeyEvent()
// -----------------------------------------------------------------------------
//
TBool CSGEGameTemplateEngine1::KeyEvent( const TKeyEvent &aKeyEvent )
    {
    if ( aKeyEvent.iScanCode == EStdKeyEscape )
        {
        iObserver.EngineClose( Id(), KErrNone );
        return ETrue;
        }
    else if ( aKeyEvent.iScanCode == EStdKeyRightArrow )
        {
        if ( iEngineState == ENormal )
            { // switching to next engine is possible after finised load of this engine
            CommonObjects().AudioProvider()->Play( KSGEGameTemplateIdAudio1, ESGEAudioRepeatNone );
        
            iObserver.EngineSwitch( Id() );
                
            return ETrue;
            }
        }
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateEngine1::SetPause()
// -----------------------------------------------------------------------------
//
void CSGEGameTemplateEngine1::SetPause( TBool aPause )
    {
    iPause = aPause;
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateEngine1::AudioAllFilesLoaded()
// -----------------------------------------------------------------------------
//
void CSGEGameTemplateEngine1::AudioAllFilesLoaded()
    {
    // all audio files are loaded, now creating audio stream
    TRAPD( err, CommonObjects().AudioProvider()->CreateAudioStreamL() );
    if ( err )
        {
        iObserver.EngineClose( Id(), err );
        }
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateEngine1::AudioStreamCreated()
// -----------------------------------------------------------------------------
//
void CSGEGameTemplateEngine1::AudioStreamCreated( TInt aError )
    {
    if ( aError )
        {
        iObserver.EngineClose( Id(), aError );
        }
    else
        {
        TRAPD( err, Construct3L() );
        if ( err )
            {
            iObserver.EngineClose( Id(), err );
            }
        }    
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateEngine1::BitmapLoaded()
// -----------------------------------------------------------------------------
//
void CSGEGameTemplateEngine1::BitmapLoaded( TUint /*aBitmapId*/, TInt aError )
    {
    if ( aError )
        {
        iObserver.EngineClose( Id(), aError );
        }
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateEngine1::BitmapsLoadedAll()
// -----------------------------------------------------------------------------
//
void CSGEGameTemplateEngine1::BitmapsLoadedAll()
    {
    TRAPD( err, Construct2L() );
    if ( err )
        {
        iObserver.EngineClose( Id(), err );
        }
    }


// End of file
