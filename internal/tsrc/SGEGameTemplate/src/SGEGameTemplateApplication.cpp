/*
 ============================================================================
  Name        : SGEGameTemplateApplication.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#include "SGEGameTemplateApplication.h"
#include "SGEGameTemplateUid.hrh"
#include "SGEGameTemplateEngineController.h"
#include <SGEApplication/SGEApplication.h>
#include <gdi.h>
#include <apparc.h> 


const TInt KSGEGameTemplateMaxFPS = 30;


// -----------------------------------------------------------------------------
// CSGEGameTemplateApplication::CSGEGameTemplateApplication()
// -----------------------------------------------------------------------------
//
CSGEGameTemplateApplication::CSGEGameTemplateApplication()
    {
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateApplication::~CSGEGameTemplateApplication()
// -----------------------------------------------------------------------------
//
CSGEGameTemplateApplication::~CSGEGameTemplateApplication()
    {
    delete iEngineController;
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateApplication::ApplicationUid()
// -----------------------------------------------------------------------------
//
TUid CSGEGameTemplateApplication::ApplicationUid()
    {
    return TUid::Uid( _UID3 );
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateApplication::NewApplication()
// -----------------------------------------------------------------------------
//
CApaApplication* CSGEGameTemplateApplication::NewApplication()
    {
    return CSGEGameApplication::NewApplication( new CSGEGameTemplateApplication );
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateApplication::InitL()
// -----------------------------------------------------------------------------
//
void CSGEGameTemplateApplication::InitL()
    {
    CSGEGameApplication::InitL();

    iEngineController = CSGEGameTemplateEngineController::NewL( *this, Rect().Size() );

    StartGameLoop();
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateApplication::Draw()
// -----------------------------------------------------------------------------
//
void CSGEGameTemplateApplication::Draw( const TRect& /*aRect*/ ) const
    {
    iEngineController->Draw( &SystemGc(), Rect() );
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateApplication::HandleForegroundEventL()
// -----------------------------------------------------------------------------
//
void CSGEGameTemplateApplication::HandleForegroundEventL( TBool aForeground )
    {
    CSGEGameApplication::HandleForegroundEventL( aForeground );
    
    if ( aForeground )
        {
        SetGameLoopMaxFPS( KSGEGameTemplateMaxFPS );
        StartGameLoop();
        }

    iEngineController->HandleForegroundEventL( aForeground );
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateApplication::HandleKeyEventL()
// -----------------------------------------------------------------------------
//
TKeyResponse CSGEGameTemplateApplication::HandleKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType )
    {
    return iEngineController->HandleKeyEventL( aKeyEvent, aType );
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateApplication::HandlePointerEventL()
// -----------------------------------------------------------------------------
//
TBool CSGEGameTemplateApplication::HandlePointerEventL( const TPointerEvent& aPointerEvent )
    {
    return iEngineController->HandlePointerEventL( aPointerEvent );
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateApplication::FullScreen()
// -----------------------------------------------------------------------------
//
TBool CSGEGameTemplateApplication::FullScreen() const
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateApplication::SizeChanged()
// -----------------------------------------------------------------------------
//
void CSGEGameTemplateApplication::SizeChanged()
    {
    iEngineController->SizeChanged( Rect().Size() );
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateApplication::DoGameLoopEvent()
// -----------------------------------------------------------------------------
//
void CSGEGameTemplateApplication::DoGameLoopEvent( TTimeIntervalMicroSeconds32 aTimeInterval )
    {
    iEngineController->Update( &SystemGc(), Rect().Size(), aTimeInterval );
    DrawNow();
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateApplication::EngineControllerClose()
// -----------------------------------------------------------------------------
//
void CSGEGameTemplateApplication::EngineControllerClose( TInt aError )
    {
    if ( aError != KErrNone )
        {
        _LIT( KAlertTitle, "Error" );
        _LIT( KAlertMessage, "Try restart or reinstall application." );
        AlertWin( KAlertTitle(), KAlertMessage() );
        }
    Exit();
    }


// End of file
