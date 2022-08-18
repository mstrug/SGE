/*
========================================================================
 Name        : SGEAudioTesterSettingItemListFile1Settings.cpp
 Author      : ${author}
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : 
========================================================================
*/
/**
 *	Generated helper class which manages the settings contained 
 *	in 'SGEAudioTesterSettingItemListFile1'.  Each CAknSettingItem maintains
 *	a reference to data in this class so that changes in the setting
 *	item list can be synchronized with this storage.
 */
 
// [[[ begin generated region: do not modify [Generated Includes]
#include <e32base.h>
#include <stringloader.h>
#include <barsread.h>
#include <SGEAudioTester.rsg>
#include "SGEAudioTesterSettingItemListFile1Settings.h"
// ]]] end generated region [Generated Includes]

/**
 * C/C++ constructor for settings data, cannot throw
 */
TSGEAudioTesterSettingItemListFile1Settings::TSGEAudioTesterSettingItemListFile1Settings()
	{
	}

/**
 * Two-phase constructor for settings data
 */
TSGEAudioTesterSettingItemListFile1Settings* TSGEAudioTesterSettingItemListFile1Settings::NewL()
	{
	TSGEAudioTesterSettingItemListFile1Settings* data = new( ELeave ) TSGEAudioTesterSettingItemListFile1Settings;
	CleanupStack::PushL( data );
	data->ConstructL();
	CleanupStack::Pop( data );
	return data;
	}
	
/**
 *	Second phase for initializing settings data
 */
void TSGEAudioTesterSettingItemListFile1Settings::ConstructL()
	{
	// [[[ begin generated region: do not modify [Generated Initializers]
	SetEnumeratedTextPopupFileName( 0 );
	SetSliderVolume( 0 );
	SetEnumeratedTextPopupRepeat( 0 );
	SetEnumeratedTextPopupKey( 0 );
	// ]]] end generated region [Generated Initializers]
	
	}
	
// [[[ begin generated region: do not modify [Generated Contents]
TInt& TSGEAudioTesterSettingItemListFile1Settings::EnumeratedTextPopupFileName()
	{
	return iEnumeratedTextPopupFileName;
	}

void TSGEAudioTesterSettingItemListFile1Settings::SetEnumeratedTextPopupFileName(const TInt& aValue)
	{
	iEnumeratedTextPopupFileName = aValue;
	}

TInt& TSGEAudioTesterSettingItemListFile1Settings::SliderVolume()
	{
	return iSliderVolume;
	}

void TSGEAudioTesterSettingItemListFile1Settings::SetSliderVolume(const TInt& aValue)
	{
	iSliderVolume = aValue;
	}

TInt& TSGEAudioTesterSettingItemListFile1Settings::EnumeratedTextPopupRepeat()
	{
	return iEnumeratedTextPopupRepeat;
	}

void TSGEAudioTesterSettingItemListFile1Settings::SetEnumeratedTextPopupRepeat(const TInt& aValue)
	{
	iEnumeratedTextPopupRepeat = aValue;
	}

TInt& TSGEAudioTesterSettingItemListFile1Settings::EnumeratedTextPopupKey()
	{
	return iEnumeratedTextPopupKey;
	}

void TSGEAudioTesterSettingItemListFile1Settings::SetEnumeratedTextPopupKey(const TInt& aValue)
	{
	iEnumeratedTextPopupKey = aValue;
	}

// ]]] end generated region [Generated Contents]

