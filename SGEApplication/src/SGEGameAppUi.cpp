/*
 ============================================================================
  Name        : SGEGameAppUi.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

// INCLUDE FILES
#include "SGEGameAppUi.h"
#include "SGESystemAppView.h"
#include "SGEGameApplication.h"
#include <remconcoreapitarget.h> // link against RemConCoreApi.lib RemConInterfaceBase.lib
#include <remconinterfaceselector.h>


// -----------------------------------------------------------------------------
// CSGEGameAppUi::ConstructL()
// -----------------------------------------------------------------------------
//
void CSGEGameAppUi::ConstructL()
    {
	iCallInit = EFalse;
    CSGESystemAppUi::ConstructL();
    
    // used to retrive Volume keys events
    iInterfaceSelector = CRemConInterfaceSelector::NewL();
    iCoreTarget = CRemConCoreApiTarget::NewL( *iInterfaceSelector, *this );
    iInterfaceSelector->OpenTargetL();
    
    if ( !GameApplication().BlockKeys() )
        {
        SetKeyBlockMode( ENoKeyBlock );
        }
    
    SetupOrientationCanBeChanged();
    
    iApplication.InitL();
    }

// -----------------------------------------------------------------------------
// CSGEGameAppUi::CSGEGameAppUi()
// -----------------------------------------------------------------------------
//
CSGEGameAppUi::CSGEGameAppUi( CSGEApplication& aApplication ) : CSGESystemAppUi( aApplication )
    {
    }

// -----------------------------------------------------------------------------
// CSGEGameAppUi::~CSGEGameAppUi()
// -----------------------------------------------------------------------------
//
CSGEGameAppUi::~CSGEGameAppUi()
    {
    // iCoreTarget deleted by iInterfaceSelector
    delete iInterfaceSelector;
    }

// -----------------------------------------------------------------------------
// CSGEGameAppUi::OrientationCanBeChanged()
// -----------------------------------------------------------------------------
//
TBool CSGEGameAppUi::OrientationCanBeChanged()
    {
    return iOrientationCanBeChanged;
    }

// -----------------------------------------------------------------------------
// CSGEGameAppUi::CheckOrientationLandscape()
// -----------------------------------------------------------------------------
//
TBool CSGEGameAppUi::CheckOrientationLandscape()
    {
    TPixelsAndRotation orient;
    CWsScreenDevice* wsScreenDevice = iEikonEnv->ScreenDevice();

    wsScreenDevice->GetScreenModeSizeAndRotation( 0, orient );
    
    TBool landscape = EFalse;

    if ( orient.iPixelSize.iWidth > orient.iPixelSize.iHeight )
        {
        landscape = ( orient.iRotation == CFbsBitGc::EGraphicsOrientationNormal || 
                      orient.iRotation == CFbsBitGc::EGraphicsOrientationRotated180 );
        }
    else
        {
        landscape = ( orient.iRotation == CFbsBitGc::EGraphicsOrientationRotated90 || 
                      orient.iRotation == CFbsBitGc::EGraphicsOrientationRotated270 );
        }
    
    return landscape;
    }

// -----------------------------------------------------------------------------
// CSGEGameAppUi::SetupOrientationCanBeChanged()
// -----------------------------------------------------------------------------
//
void CSGEGameAppUi::SetupOrientationCanBeChanged()
    {
    CWsScreenDevice *wsScreenDevice = iEikonEnv->ScreenDevice();
    
    RArray<TPixelsAndRotation> res;
    
    for ( TInt i = 0; i < wsScreenDevice->NumScreenModes(); i++ )
        {
        TPixelsAndRotation orient;
        wsScreenDevice->GetScreenModeSizeAndRotation( i, orient );
        
        res.Append( orient );
        }
    
    for ( TInt i = 0; i < res.Count(); i++ )
        {
        for ( TInt j = i + 1; j < res.Count(); j++ )
            {
            if ( res[ i ].iPixelSize != res[ j ].iPixelSize && 
                 res[ i ].iRotation != res[ j ].iRotation )
                {
                res.Close();
                iOrientationCanBeChanged = ETrue;
                return;
                }
            }
        }
        
    res.Close();
    iOrientationCanBeChanged = EFalse;
    }

// -----------------------------------------------------------------------------
// CSGEGameAppUi::GameApplication()
// -----------------------------------------------------------------------------
//
CSGEGameApplication& CSGEGameAppUi::GameApplication()
    {
    return static_cast<CSGEGameApplication&>( iApplication );
    }

// -----------------------------------------------------------------------------
// CSGEGameAppUi::HandleCommandL()
// -----------------------------------------------------------------------------
//
void CSGEGameAppUi::HandleCommandL( TInt aCommand )
    {
    if ( aCommand == EEikCmdExit ||
         aCommand == EAknSoftkeyExit )
        {
        Exit();
        }
    else
        {
        CSGESystemAppUi::HandleCommandL( aCommand );
        }
    }

// -----------------------------------------------------------------------------
// CSGEGameAppUi::HandleWsEventL()
// -----------------------------------------------------------------------------
//
void CSGEGameAppUi::HandleWsEventL( const TWsEvent& aEvent, CCoeControl *aDestination )
    {
    if ( aEvent.Type() == KAknUidValueEndKeyCloseEvent && GameApplication().GoBackgroundOnEndKey() )
        {
        DeactivateActiveViewL();
        }
    else
        {
        CSGESystemAppUi::HandleWsEventL( aEvent, aDestination );
        }
    }

// -----------------------------------------------------------------------------
// CSGEGameAppUi::MrccatoCommand()
// -----------------------------------------------------------------------------
//
void CSGEGameAppUi::MrccatoCommand( TRemConCoreApiOperationId aOperationId, TRemConCoreApiButtonAction aButtonAct )
    {
    TRequestStatus status;
    if ( aOperationId == ERemConCoreApiVolumeUp )
        {
        TKeyEvent ev = { EKeyIncVolume, EStdKeyIncVolume, 0, 0 }; 
        TEventCode evc = EEventKey;
        switch ( aButtonAct )
            {
            case ERemConCoreApiButtonClick: evc = EEventKey; break;
            case ERemConCoreApiButtonPress: evc = EEventKeyDown; break;
            case ERemConCoreApiButtonRelease: evc = EEventKeyUp; break;
            }
        TKeyResponse res = EKeyWasNotConsumed;
        TRAP_IGNORE( res = HandleKeyEventL( ev, evc ) );
        if ( res == EKeyWasConsumed )
            {
            iCoreTarget->VolumeUpResponse( status, KErrNone );
            User::WaitForRequest( status );
            }
        }
    else if ( aOperationId == ERemConCoreApiVolumeDown )
        {
        TKeyEvent ev = { EKeyDecVolume, EStdKeyDecVolume, 0, 0 }; 
        TEventCode evc = EEventKey;
        switch ( aButtonAct )
            {
            case ERemConCoreApiButtonClick: evc = EEventKey; break;
            case ERemConCoreApiButtonPress: evc = EEventKeyDown; break;
            case ERemConCoreApiButtonRelease: evc = EEventKeyUp; break;
            }
        TKeyResponse res = EKeyWasNotConsumed;
        TRAP_IGNORE( res = HandleKeyEventL( ev, evc ) );
        if ( res == EKeyWasConsumed )
            {
            iCoreTarget->VolumeDownResponse( status, KErrNone );
            User::WaitForRequest( status );
            }
        }
    }


// End of File
