/*
 ============================================================================
  Name        : SGESystemAppUi.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

// INCLUDE FILES
#include "SGESystemAppUi.h"
#include "SGESystemAppView.h"
#include "SGEApplication.h"
#include <avkon.rsg>
#include <aknsoundsystem.h>


// -----------------------------------------------------------------------------
// CSGESystemAppUi::ConstructL()
// -----------------------------------------------------------------------------
//
void CSGESystemAppUi::ConstructL()
    {
    TInt orientation;
    switch ( iApplication.Orientation() )
        {
        case ESGEApplicationOrientationAuto: orientation = EAppOrientationAutomatic; break;
        case ESGEApplicationOrientationLandscape: orientation = EAppOrientationLandscape; break;
        case ESGEApplicationOrientationPortrait: orientation = EAppOrientationPortrait; break;
        default: orientation = 0; break;
        }
    
    BaseConstructL( CAknAppUi::EAknEnableSkin | orientation | iApplication.ApplicationFlags() );

    TRect r = ( iApplication.FullScreen() ? ApplicationRect() : ClientRect() );
    iAppView = iApplication.CreateAppViewL( r );

    iAppView->SetMopParent( this );

    if ( iApplication.SilentKeys() )
        {
        KeySounds()->PushContextL( R_AVKON_SILENT_SKEY_LIST );
        }
    else
        {
        KeySounds()->PushContextL( R_AVKON_DEFAULT_SKEY_LIST );
        }
    
    AddToStackL( iAppView );
    
    iCreated = ETrue;

    if ( iCallInit )
        {
        iApplication.InitL();
        }
    }

// -----------------------------------------------------------------------------
// CSGESystemAppUi::CSGESystemAppUi()
// -----------------------------------------------------------------------------
//
CSGESystemAppUi::CSGESystemAppUi( CSGEApplication& aApplication ) : iApplication( aApplication ), iCallInit( ETrue )
    {
    }

// -----------------------------------------------------------------------------
// CSGESystemAppUi::~CSGESystemAppUi()
// -----------------------------------------------------------------------------
//
CSGESystemAppUi::~CSGESystemAppUi()
    {
	iApplication.Cleanup();
    if ( iAppView )
        {
        iEikonEnv->RemoveFromStack( iAppView );
        delete iAppView;
        }
    }

// -----------------------------------------------------------------------------
// CSGESystemAppUi::AlertWin()
// -----------------------------------------------------------------------------
//
void CSGESystemAppUi::AlertWin( const TDesC& aMsg1, const TDesC& aMsg2 )
    {
    iEikonEnv->AlertWin( aMsg1, aMsg2 );
    }

// -----------------------------------------------------------------------------
// CSGESystemAppUi::HandleCommandL()
// -----------------------------------------------------------------------------
//
void CSGESystemAppUi::HandleCommandL( TInt aCommand )
    {
    if ( iCreated )
        {
        iApplication.HandleCommandL( aCommand );
        }
    }

// -----------------------------------------------------------------------------
// CSGESystemAppUi::HandleStatusPaneSizeChange()
// -----------------------------------------------------------------------------
//
void CSGESystemAppUi::HandleStatusPaneSizeChange()
    {
    if ( iAppView && !iApplication.FullScreen() )
        {
        iAppView->SetRect( ClientRect() );
        }
    if ( iCreated  )
        {
        iApplication.HandleStatusPaneSizeChange();
        }
    }

// -----------------------------------------------------------------------------
// CSGESystemAppUi::HandleWsEventL()
// -----------------------------------------------------------------------------
//
void CSGESystemAppUi::HandleWsEventL( const TWsEvent& aEvent, CCoeControl *aDestination )
    {
    if ( iCreated && !iApplication.HandleWsEventL( aEvent, aDestination ) )
        {
        CAknAppUi::HandleWsEventL( aEvent, aDestination );
        }
    }

// -----------------------------------------------------------------------------
// CSGESystemAppUi::HandleForegroundEventL()
// -----------------------------------------------------------------------------
//
void CSGESystemAppUi::HandleForegroundEventL( TBool aForeground )
    {
    if ( iCreated )
        {
        iApplication.HandleForegroundEventL( aForeground );
        }
    }

// -----------------------------------------------------------------------------
// CSGESystemAppUi::HandleKeyEventL()
// -----------------------------------------------------------------------------
//
TKeyResponse CSGESystemAppUi::HandleKeyEventL( const TKeyEvent &aKeyEvent, TEventCode aType )
    {
    if ( iCreated )
        {
        return iApplication.HandleKeyEventL( aKeyEvent, aType );
        }
    return EKeyWasNotConsumed;
    }

// -----------------------------------------------------------------------------
// CSGESystemAppUi::ProcessCommandParametersL()
// -----------------------------------------------------------------------------
//
TBool CSGESystemAppUi::ProcessCommandParametersL( TApaCommand aCommand, TFileName& aDocumentName, const TDesC8& aTail )
    {
    if ( iCreated )
        {
        return iApplication.ProcessCommandParametersL( aCommand, aDocumentName, aTail );
        }
    return EFalse;
    }


// End of File
