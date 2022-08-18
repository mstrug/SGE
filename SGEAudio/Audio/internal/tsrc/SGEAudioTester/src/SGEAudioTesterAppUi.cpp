/*
========================================================================
 Name        : SGEAudioTesterAppUi.cpp
 Author      : Michal Strug
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : 
========================================================================
*/
// [[[ begin generated region: do not modify [Generated System Includes]
#include <eikmenub.h>
#include <akncontext.h>
#include <akntitle.h>
#include <SGEAudioTester.rsg>
// ]]] end generated region [Generated System Includes]

// [[[ begin generated region: do not modify [Generated User Includes]
#include "SGEAudioTesterAppUi.h"
#include "SGEAudioTester.hrh"
#include "SGEAudioTesterSettingItemList.hrh"
#include "SGEAudioTesterSettingItemListFile1.hrh"
#include "SGEAudioTesterContainerView.h"
#include "SGEAudioTesterSettingItemListView.h"
#include "SGEAudioTesterSettingItemListFile1View.h"
// ]]] end generated region [Generated User Includes]

#include "SGEAudioTesterContainer.hrh"

// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

/**
 * Construct the CSGEAudioTesterAppUi instance
 */ 
CSGEAudioTesterAppUi::CSGEAudioTesterAppUi()
    {
    // [[[ begin generated region: do not modify [Generated Contents]
    // ]]] end generated region [Generated Contents]
    
    }

/** 
 * The appui's destructor removes the container from the control
 * stack and destroys it.
 */
CSGEAudioTesterAppUi::~CSGEAudioTesterAppUi()
    {
    // [[[ begin generated region: do not modify [Generated Contents]
    // ]]] end generated region [Generated Contents]
    
    }

// [[[ begin generated function: do not modify
void CSGEAudioTesterAppUi::InitializeContainersL()
	{
	iSGEAudioTesterContainerView = CSGEAudioTesterContainerView::NewL();
	AddViewL( iSGEAudioTesterContainerView );
	SetDefaultViewL( *iSGEAudioTesterContainerView );
	iSGEAudioTesterSettingItemListView = CSGEAudioTesterSettingItemListView::NewL();
	AddViewL( iSGEAudioTesterSettingItemListView );
	iSGEAudioTesterSettingItemListFile1View = CSGEAudioTesterSettingItemListFile1View::NewL();
	AddViewL( iSGEAudioTesterSettingItemListFile1View );
	}
// ]]] end generated function

/**
 * Handle a command for this appui (override)
 * @param aCommand command id to be handled
 */
void CSGEAudioTesterAppUi::HandleCommandL( TInt aCommand )
    {
    // [[[ begin generated region: do not modify [Generated Code]
	TBool commandHandled = EFalse;
	switch ( aCommand )
		{ // code to dispatch to the AppUi's menu and CBA commands is generated here
		default:
			break;
		}
	
		
	if ( !commandHandled ) 
		{
		if ( aCommand == EAknSoftkeyExit || aCommand == EEikCmdExit )
			{
			Exit();
			}
		}
    // ]]] end generated region [Generated Code]
    
    }

/** 
 * Override of the HandleResourceChangeL virtual function
 */
void CSGEAudioTesterAppUi::HandleResourceChangeL( TInt aType )
    {
    CAknViewAppUi::HandleResourceChangeL( aType );
    // [[[ begin generated region: do not modify [Generated Code]
    // ]]] end generated region [Generated Code]
    
    }
                
/** 
 * Override of the HandleKeyEventL virtual function
 * @return EKeyWasConsumed if event was handled, EKeyWasNotConsumed if not
 * @param aKeyEvent 
 * @param aType 
 */
TKeyResponse CSGEAudioTesterAppUi::HandleKeyEventL(
        const TKeyEvent& aKeyEvent,
        TEventCode aType )
    {
    // The inherited HandleKeyEventL is private and cannot be called
    // [[[ begin generated region: do not modify [Generated Contents]
    // ]]] end generated region [Generated Contents]
    
    return EKeyWasNotConsumed;
    }

/** 
 * Override of the HandleViewDeactivation virtual function
 *
 * @param aViewIdToBeDeactivated 
 * @param aNewlyActivatedViewId 
 */
void CSGEAudioTesterAppUi::HandleViewDeactivation( 
        const TVwsViewId& aViewIdToBeDeactivated, 
        const TVwsViewId& aNewlyActivatedViewId )
    {
    CAknViewAppUi::HandleViewDeactivation( 
            aViewIdToBeDeactivated, 
            aNewlyActivatedViewId );
    // [[[ begin generated region: do not modify [Generated Contents]
    // ]]] end generated region [Generated Contents]
    
    }

/**
 * @brief Completes the second phase of Symbian object construction. 
 * Put initialization code that could leave here. 
 */ 
void CSGEAudioTesterAppUi::ConstructL()
    {
    // [[[ begin generated region: do not modify [Generated Contents]
	
	BaseConstructL( EAknEnableSkin  | 
					 EAknEnableMSK ); 
	InitializeContainersL();
    // ]]] end generated region [Generated Contents]
    
    }

