/*
 ============================================================================
  Name        : SGEGameTemplateEngineController.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#include "SGEGameTemplateEngineController.h"
#include "SGEGameTemplateEngine1.h"
#include "SGEGameTemplateEngine2.h"
#include "SGEGameTemplateCommonObjects.h"
#include "SGEGameTemplateSettings.h"
#include <SGEBitmap/SGEBitmapProvider.h>
#include <SGEFont/SGEFontProvider.h>
#include <SGEFile/SGEFileProvider.h>
#include <SGEString/SGEStringProvider.h>
#include <SGEAudio/SGEAudioProvider.h>


// -----------------------------------------------------------------------------
// CSGEGameTemplateEngineController::CSGEGameTemplateEngineController()
// -----------------------------------------------------------------------------
//
CSGEGameTemplateEngineController::CSGEGameTemplateEngineController( MSGEEngineControllerObserver& aObserver, TSize aSize )
    : CSGEEngineController( aObserver, aSize )
    {
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateEngineController::NewL()
// -----------------------------------------------------------------------------
//
CSGEGameTemplateEngineController* CSGEGameTemplateEngineController::NewL( MSGEEngineControllerObserver& aObserver, TSize aSize )
    {
    CSGEGameTemplateEngineController *self = new (ELeave) CSGEGameTemplateEngineController( aObserver, aSize );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateEngineController::ConstructL()
// -----------------------------------------------------------------------------
//
void CSGEGameTemplateEngineController::ConstructL()
    {
    BaseConstructL();

    CSGEEngine* engine = CSGEGameTemplateEngine1::NewL( *this, iCommonObjects );
    CleanupStack::PushL( engine );
    User::LeaveIfError( AddEngine( engine ) );
    CleanupStack::Pop( engine );
    
    SetCurrentEngine( engine->Id() );
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateEngineController::CreateCommonObjectsL()
// -----------------------------------------------------------------------------
//
CSGEEngineCommonObjects* CSGEGameTemplateEngineController::CreateCommonObjectsL()
    {
    CSGEGameTemplateCommonObjects *objects = new (ELeave) CSGEGameTemplateCommonObjects();
    CleanupStack::PushL( objects );
    
    objects->SetFileProvider( CSGEFileProvider::NewL() );
    objects->SetBitmapProvider( CSGEBitmapProvider::NewL( CurrentEngine() ) );
    objects->SetFontProvider( CSGEFontProvider::NewL() );
    objects->SetStringProvider( CSGEStringProvider::NewL() );
    
    TSGEAudioConfig defaultConfig;
    CSGEAudioProvider *audio = CSGEAudioProvider::NewL( CurrentEngine(), defaultConfig ); 
    objects->SetAudioProvider( audio );

    objects->SetSettings( CSGEGameTemplateSettings::NewL() );

    CleanupStack::Pop( objects );
    return objects;
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateEngineController::Update()
// -----------------------------------------------------------------------------
//
void CSGEGameTemplateEngineController::Update( CBitmapContext* /*aGc*/, TRect /*aDrawRect*/, 
                                               TTimeIntervalMicroSeconds32 aTimeInterval )
    {
    if ( CurrentEngine() )
        {
        CurrentEngine()->Update( aTimeInterval );
        }
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateEngineController::EngineClose()
// -----------------------------------------------------------------------------
//
void CSGEGameTemplateEngineController::EngineClose( TUint /*aId*/, TInt aError )
    {
    iObserver.EngineControllerClose( aError );
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateEngineController::EngineSwitch()
// -----------------------------------------------------------------------------
//
void CSGEGameTemplateEngineController::EngineSwitch( TUint aId )
    {
    switch ( aId )
        {
        case KSGEGameTemplateEngine1Id:
            { // switch to engine 2 - lazy creation
            if ( !EngineAdded( KSGEGameTemplateEngine2Id ) )
                {
                CSGEEngine* engine = NULL;
                TRAPD( err, engine = CSGEGameTemplateEngine2::NewL( *this, iCommonObjects ) );
                if ( err != KErrNone )
                    {
                    iObserver.EngineControllerClose( err );
                    return;
                    }
                err = AddEngine( engine );
                if ( err != KErrNone )
                    {
                    delete engine;
                    iObserver.EngineControllerClose( err );
                    return;
                    }
                }
            
            TInt err = SetCurrentEngine( KSGEGameTemplateEngine2Id );
            if ( err != KErrNone )
                {
                iObserver.EngineControllerClose( err );
                return;
                }
            }
            break;
        case KSGEGameTemplateEngine2Id:
            { // switch to engine 1 which is created during construction
            TInt err = SetCurrentEngine( KSGEGameTemplateEngine1Id );
            if ( err != KErrNone )
                {
                iObserver.EngineControllerClose( err );
                return;
                }
            }
            break;
        default:
            {
            iObserver.EngineControllerClose( KErrGeneral );
            return;
            }
        }
    }


// End of file
