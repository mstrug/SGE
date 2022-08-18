/*
 ============================================================================
  Name        : SGESystemAppUi.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#ifndef __SGESYSTEMAPPUI_H__
#define __SGESYSTEMAPPUI_H__

// INCLUDES
#include <aknappui.h>

// FORWARD DECLARATIONS
class CSGEApplication;
class CSGESystemAppView;

// CLASS DECLARATION
/**
 * CSGESystemAppUi
 *
 */
NONSHARABLE_CLASS( CSGESystemAppUi ) : public CAknAppUi
    {
public:

    virtual void ConstructL();

    CSGESystemAppUi( CSGEApplication& aApplication );

    virtual ~CSGESystemAppUi();

    void AlertWin( const TDesC& aMsg1, const TDesC& aMsg2 );

protected: // from base class

    void HandleCommandL( TInt aCommand );

    void HandleStatusPaneSizeChange();

    void HandleWsEventL( const TWsEvent& aEvent, CCoeControl *aDestination );

    void HandleForegroundEventL( TBool aForeground );

    TKeyResponse HandleKeyEventL( const TKeyEvent &aKeyEvent, TEventCode aType );
    
    TBool ProcessCommandParametersL( TApaCommand aCommand, TFileName& aDocumentName, const TDesC8& aTail );

protected: // data

    CSGESystemAppView *iAppView;  // owned

    CSGEApplication& iApplication;

    TBool iCreated;
    
    TBool iCallInit; 
    		
    };

#endif // __SGESYSMTEAPPUI_H__
