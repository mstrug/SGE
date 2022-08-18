/*
 ============================================================================
  Name        : SGEGameAppUi.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#ifndef __SGEGAMEAPPUI_H__
#define __SGEGAMEAPPUI_H__

// INCLUDES
#include <aknappui.h>
#include <remconcoreapitargetobserver.h> // added implementation for Volume keys
#include "sgesystemappui.h"

// FORWARD DECLARATIONS
class CSGEGameApplication;
class CRemConInterfaceSelector;
class CRemConCoreApiTarget;

// CLASS DECLARATION
/**
 * CSGEGameAppUi
 *
 */
NONSHARABLE_CLASS( CSGEGameAppUi ) : public CSGESystemAppUi,
                                     public MRemConCoreApiTargetObserver
    {
    
public: // construction

    void ConstructL();

    CSGEGameAppUi( CSGEApplication& aApplication );

    ~CSGEGameAppUi();

public: // new functions
    
    /*
     * Getter for iOrientationCanBeChanged member variable.
     * @return ETrue when device support landscape and portrait switching 
     */
    TBool OrientationCanBeChanged();
    
    /*
     * Checks if current orientation is landscape or portrait.
     * @return ETrue orientation is landscape 
     */
    TBool CheckOrientationLandscape();
    
private: // new functions

    /*
     * Functions checks if device has capability to change orientations
     * beetwen landscape and portrait.
     */
    void SetupOrientationCanBeChanged();

    CSGEGameApplication& GameApplication();
    
private: // from base class
    
    void HandleCommandL( TInt aCommand );
    
    void HandleWsEventL( const TWsEvent& aEvent, CCoeControl *aDestination );

private: // from MRemConCoreApiTargetObserver
    
    void MrccatoCommand( TRemConCoreApiOperationId aOperationId, TRemConCoreApiButtonAction aButtonAct );

private: // data
    
    TBool iOrientationCanBeChanged;
    
    CRemConInterfaceSelector* iInterfaceSelector;
    
    CRemConCoreApiTarget* iCoreTarget;
    
    };

#endif // __SGEGAMEAPPUI_H__
