/*
========================================================================
 Name        : SGEAudioTesterSettingItemList.cpp
 Author      : Michal Strug
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : 
========================================================================
*/
// [[[ begin generated region: do not modify [Generated System Includes]
#include <avkon.hrh>
#include <avkon.rsg>
#include <eikmenup.h>
#include <aknappui.h>
#include <eikcmobs.h>
#include <barsread.h>
#include <stringloader.h>
#include <aknpopupfieldtext.h>
#include <eikenv.h>
#include <aknnumedwin.h>
#include <eikappui.h>
#include <aknviewappui.h>
#include <SGEAudioTester.rsg>
// ]]] end generated region [Generated System Includes]

// [[[ begin generated region: do not modify [Generated User Includes]
#include "SGEAudioTesterSettingItemList.h"
#include "SGEAudioTesterSettingItemListSettings.h"
#include "SGEAudioTester.hrh"
#include "SGEAudioTesterContainer.hrh"
#include "SGEAudioTesterSettingItemList.hrh"
#include "SGEAudioTesterSettingItemListFile1.hrh"
#include "SGEAudioTesterSettingItemListView.h"
// ]]] end generated region [Generated User Includes]

// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

/**
 * Construct the CSGEAudioTesterSettingItemList instance
 * @param aCommandObserver command observer
 */ 
CSGEAudioTesterSettingItemList::CSGEAudioTesterSettingItemList( 
        TSGEAudioTesterSettingItemListSettings& aSettings, 
        MEikCommandObserver* aCommandObserver )
    : iSettings( aSettings ), iCommandObserver( aCommandObserver )
    {
    // [[[ begin generated region: do not modify [Generated Contents]
    // ]]] end generated region [Generated Contents]
    
    }
/** 
 * Destroy any instance variables
 */
CSGEAudioTesterSettingItemList::~CSGEAudioTesterSettingItemList()
    {
    // [[[ begin generated region: do not modify [Generated Contents]
    // ]]] end generated region [Generated Contents]
    
    }

/**
 * Handle system notification that the container's size has changed.
 */
void CSGEAudioTesterSettingItemList::SizeChanged()
    {
    if ( ListBox() ) 
        {
        ListBox()->SetRect( Rect() );
        }
    }

/**
 * Create one setting item at a time, identified by id.
 * CAknSettingItemList calls this method and takes ownership
 * of the returned value.  The CAknSettingItem object owns
 * a reference to the underlying data, which EditItemL() uses
 * to edit and store the value.
 */
CAknSettingItem* CSGEAudioTesterSettingItemList::CreateSettingItemL( TInt aId )
    {
    switch ( aId )
        {
    // [[[ begin generated region: do not modify [Initializers]
		case ESGEAudioTesterSettingItemListViewEnumeratedTextPopupSamplerate:
			{			
			CAknEnumeratedTextPopupSettingItem* item = new ( ELeave ) 
				CAknEnumeratedTextPopupSettingItem( 
					aId,
					iSettings.EnumeratedTextPopupSamplerate() );
			return item;
			}
		case ESGEAudioTesterSettingItemListViewBinarySettingChannels:
			{			
			CAknBinaryPopupSettingItem* item = new ( ELeave ) 
				CAknBinaryPopupSettingItem( 
					aId,
					iSettings.BinarySettingChannels() );
			return item;
			}
		case ESGEAudioTesterSettingItemListViewIntegerEditorBufferLength:
			{			
			CAknIntegerEdwinSettingItem* item = new ( ELeave ) 
				CAknIntegerEdwinSettingItem( 
					aId,
					iSettings.IntegerEditorBufferLength() );
			return item;
			}
		case ESGEAudioTesterSettingItemListViewEnumeratedTextPopupBufferDataType:
			{			
			CAknEnumeratedTextPopupSettingItem* item = new ( ELeave ) 
				CAknEnumeratedTextPopupSettingItem( 
					aId,
					iSettings.EnumeratedTextPopupBufferDataType() );
			return item;
			}
		case ESGEAudioTesterSettingItemListViewEnumeratedTextPopupSubsystem:
			{			
			CAknEnumeratedTextPopupSettingItem* item = new ( ELeave ) 
				CAknEnumeratedTextPopupSettingItem( 
					aId,
					iSettings.EnumeratedTextPopupSubsystem() );
			return item;
			}
		case ESGEAudioTesterSettingItemListViewBinarySettingMultithreeded:
			{			
			CAknBinaryPopupSettingItem* item = new ( ELeave ) 
				CAknBinaryPopupSettingItem( 
					aId,
					iSettings.BinarySettingMultithreeded() );
			return item;
			}
		case ESGEAudioTesterSettingItemListViewBinarySettingResampleFiles:
			{			
			CAknBinaryPopupSettingItem* item = new ( ELeave ) 
				CAknBinaryPopupSettingItem( 
					aId,
					iSettings.BinarySettingResampleFiles() );
			return item;
			}
		case ESGEAudioTesterSettingItemListViewVolumeSetting:
			{			
			CAknVolumeSettingItem* item = new ( ELeave ) 
				CAknVolumeSettingItem( 
					aId,
					iSettings.VolumeSetting() );
			return item;
			}
    // ]]] end generated region [Initializers]
    
        }
        
    return NULL;
    }
    
/**
 * Edit the setting item identified by the given id and store
 * the changes into the store.
 * @param aIndex the index of the setting item in SettingItemArray()
 * @param aCalledFromMenu true: a menu item invoked editing, thus
 *    always show the edit page and interactively edit the item;
 *    false: change the item in place if possible, else show the edit page
 */
void CSGEAudioTesterSettingItemList::EditItemL ( TInt aIndex, TBool aCalledFromMenu )
    {
    CAknSettingItem* item = ( *SettingItemArray() )[aIndex];
    switch ( item->Identifier() )
        {
    // [[[ begin generated region: do not modify [Editing Started Invoker]
    // ]]] end generated region [Editing Started Invoker]
    
        }
    
    CAknSettingItemList::EditItemL( aIndex, aCalledFromMenu );
    
    TBool storeValue = ETrue;
    switch ( item->Identifier() )
        {
    // [[[ begin generated region: do not modify [Editing Stopped Invoker]
    // ]]] end generated region [Editing Stopped Invoker]
    
        }
        
    if ( storeValue )
        {
        item->StoreL();
        SaveSettingValuesL();
        }    
    }
/**
 *    Handle the "Change" option on the Options menu.  This is an
 *    alternative to the Selection key that forces the settings page
 *    to come up rather than changing the value in place (if possible).
 */
void CSGEAudioTesterSettingItemList::ChangeSelectedItemL()
    {
    if ( ListBox()->CurrentItemIndex() >= 0 )
        {
        EditItemL( ListBox()->CurrentItemIndex(), ETrue );
        }
    }

/**
 *    Load the initial contents of the setting items.  By default,
 *    the setting items are populated with the default values from
 *     the design.  You can override those values here.
 *    <p>
 *    Note: this call alone does not update the UI.  
 *    LoadSettingsL() must be called afterwards.
 */
void CSGEAudioTesterSettingItemList::LoadSettingValuesL()
    {
    // load values into iSettings
    }
    
/**
 *    Save the contents of the setting items.  Note, this is called
 *    whenever an item is changed and stored to the model, so it
 *    may be called multiple times or not at all.
 */
void CSGEAudioTesterSettingItemList::SaveSettingValuesL()
    {
    // store values from iSettings
    }


/** 
 * Handle global resource changes, such as scalable UI or skin events (override)
 */
void CSGEAudioTesterSettingItemList::HandleResourceChange( TInt aType )
    {
    CAknSettingItemList::HandleResourceChange( aType );
    SetRect( iAvkonViewAppUi->View( TUid::Uid( ESGEAudioTesterSettingItemListViewId ) )->ClientRect() );
    // [[[ begin generated region: do not modify [Generated Contents]
    // ]]] end generated region [Generated Contents]
    
    }
                
/** 
 * Handle key event (override)
 * @param aKeyEvent key event
 * @param aType event code
 * @return EKeyWasConsumed if the event was handled, else EKeyWasNotConsumed
 */
TKeyResponse CSGEAudioTesterSettingItemList::OfferKeyEventL( 
        const TKeyEvent& aKeyEvent, 
        TEventCode aType )
    {
    // [[[ begin generated region: do not modify [Generated Contents]
    // ]]] end generated region [Generated Contents]
    
    if ( aKeyEvent.iCode == EKeyLeftArrow 
        || aKeyEvent.iCode == EKeyRightArrow )
        {
        // allow the tab control to get the arrow keys
        return EKeyWasNotConsumed;
        }
    
    return CAknSettingItemList::OfferKeyEventL( aKeyEvent, aType );
    }
                
