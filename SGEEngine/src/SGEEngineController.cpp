/*
 ============================================================================
  Name        : SGEEngineController.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#include "SGEEngineController.h"
#include "SGEEngineControllerObserver.h"
#include "SGEEngineCommonObjects.h"
#include "SGEEngine.h"
#include <SGEBitmap/SGEBitmapProvider.h>
#include <SGEFont/SGEFontProvider.h>
#include <SGEFile/SGEFileProvider.h>
#include <SGEString/SGEStringProvider.h>
#include <SGEAudio/SGEAudioProvider.h>


// -----------------------------------------------------------------------------
// CSGEEngineController::CSGEEngineController()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEEngineController::CSGEEngineController( MSGEEngineControllerObserver& aObserver, TSize aSize )
        : iObserver( aObserver ), iSize( aSize )
    {
    }

// -----------------------------------------------------------------------------
// CSGEEngineController::~CSGEEngineController()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEEngineController::~CSGEEngineController()
    {
    iEngines.ResetAndDestroy();
    delete iCommonObjects;
    }

// -----------------------------------------------------------------------------
// CSGEEngineController::BaseConstructL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEEngineController::BaseConstructL()
    {
    iCommonObjects = CreateCommonObjectsL();
    }

// -----------------------------------------------------------------------------
// CSGEEngineController::CreateCommonObjectsL()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEEngineCommonObjects* CSGEEngineController::CreateCommonObjectsL()
    {
    CSGEEngineCommonObjects *objects = new (ELeave) CSGEEngineCommonObjects();
    CleanupStack::PushL( objects );
    
    objects->SetFileProvider( CSGEFileProvider::NewL() );
    objects->SetBitmapProvider( CSGEBitmapProvider::NewL( iCurrentEngine ) );
    objects->SetFontProvider( CSGEFontProvider::NewL() );
    objects->SetStringProvider( CSGEStringProvider::NewL() );
    
    TSGEAudioConfig defaultConfig;
    CSGEAudioProvider *audio = CSGEAudioProvider::NewL( iCurrentEngine, defaultConfig ); 
    objects->SetAudioProvider( audio );
    
    CleanupStack::Pop( objects );
    return objects;
    }

// -----------------------------------------------------------------------------
// CSGEEngineController::HandleKeyEventL()
// -----------------------------------------------------------------------------
//
EXPORT_C TKeyResponse CSGEEngineController::HandleKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType )
    {
    if ( iCurrentEngine )
        {
        TBool ret = EFalse;
        if ( aType == EEventKeyDown )
            {
            ret = iCurrentEngine->KeyDownEvent( aKeyEvent );
            }
        else if ( aType == EEventKeyUp )
            {
            ret = iCurrentEngine->KeyUpEvent( aKeyEvent );
            }
        else if ( aType == EEventKey )
            {
            ret = iCurrentEngine->KeyEvent( aKeyEvent );
            }
        if ( ret )
            {
            return EKeyWasConsumed;
            }
        else
            {
            return EKeyWasNotConsumed;
            }
        }
    else
        {
        return EKeyWasNotConsumed;
        }
    }

// -----------------------------------------------------------------------------
// CSGEEngineController::HandlePointerEventL()
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSGEEngineController::HandlePointerEventL( const TPointerEvent &aPointerEvent )
    {
    if ( iCurrentEngine )
        {
        return iCurrentEngine->PointerEvent( aPointerEvent );
        }
    else
        {
        return EFalse;
        }
    }

// -----------------------------------------------------------------------------
// CSGEEngineController::Update()
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSGEEngineController::Update( CBitmapContext *aGc, TRect aDrawRect, TTimeIntervalMicroSeconds32 aTimeInterval )
    {
    TBool ret = EFalse;
    if ( iCurrentEngine )
        {
        ret = iCurrentEngine->Update( aTimeInterval );
        if ( ret )
            {
            iCurrentEngine->Draw( aGc, aDrawRect );
            }
        }
    return ret;
    }

// -----------------------------------------------------------------------------
// CSGEEngineController::SizeChanged()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEEngineController::SizeChanged( TSize aNewSize )
    {
    iSize = aNewSize;
    if ( iCurrentEngine )
        {
        iCurrentEngine->SizeChanged( aNewSize );
        }
    }

// -----------------------------------------------------------------------------
// CSGEEngineController::HandleForegroundEventL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEEngineController::HandleForegroundEventL( TBool aForeground )
    {
    if ( iCurrentEngine )
        {
        iCurrentEngine->SetPause( !aForeground );
        }
    }

// -----------------------------------------------------------------------------
// CSGEEngineController::Draw()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEEngineController::Draw( CBitmapContext* aGc, const TRect aDrawRect ) const
    {
    iCurrentEngine->Draw( aGc, aDrawRect );
    }

// -----------------------------------------------------------------------------
// CSGEEngineController::CommonObjects()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEEngineCommonObjects& CSGEEngineController::CommonObjects() const
    {
    return *iCommonObjects;
    }

// -----------------------------------------------------------------------------
// CSGEEngineController::AddEngineL()
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CSGEEngineController::AddEngine( CSGEEngine *aEngine )
    {
    if ( !EngineAdded( aEngine->Id() ) )
        {
        return iEngines.Append( aEngine );
        }
    return KErrAlreadyExists;
    }

// -----------------------------------------------------------------------------
// CSGEEngineController::Engine()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEEngine* CSGEEngineController::Engine( TUint aId ) const
    {
    for ( TInt i = 0; i < iEngines.Count(); i++ )
        {
        if ( iEngines[ i ]->Id() == aId )
            {
            return iEngines[ i ];
            }
        }
    return NULL;
    }

// -----------------------------------------------------------------------------
// CSGEEngineController::EngineAdded()
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSGEEngineController::EngineAdded( TUint aId ) const
    {
    for ( TInt i = 0; i < iEngines.Count(); i++ )
        {
        if ( iEngines[ i ]->Id() == aId )
            {
            return ETrue;
            }
        }
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CSGEEngineController::SetCurrentEngine()
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CSGEEngineController::SetCurrentEngine( TUint aId )
    {
    for ( TInt i = 0; i < iEngines.Count(); i++ )
        {
        if ( iEngines[ i ]->Id() == aId )
            {
            if ( iCurrentEngine )
                {
                iCurrentEngine->Deactivate();
                }
            iCurrentEngine = iEngines[ i ];
            
            if ( iCommonObjects->BitmapProvider() )
                {
                iCommonObjects->BitmapProvider()->SetObserver( iCurrentEngine );
                }            
            if ( iCommonObjects->AudioProvider() )
                {
                iCommonObjects->AudioProvider()->SetObserver( iCurrentEngine );
                }
            
            DoSetCurrentEngine();
            
            iCurrentEngine->Activate();
            return KErrNone;
            }
        }
    return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// CSGEEngineController::CurrentEngine()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEEngine* CSGEEngineController::CurrentEngine() const
    {
    return iCurrentEngine;
    }

// -----------------------------------------------------------------------------
// CSGEEngineController::DoSetCurrentEngine()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEEngineController::DoSetCurrentEngine()
    {    
    }

// -----------------------------------------------------------------------------
// CSGEEngineController::EngineClose()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEEngineController::EngineClose( TUint /*aId*/, TInt aError )
    {
    iObserver.EngineControllerClose( aError );
    }

// -----------------------------------------------------------------------------
// CSGEEngineController::EngineSwitch()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEEngineController::EngineSwitch( TUint /*aId*/ )
    {
    }


// End of file
