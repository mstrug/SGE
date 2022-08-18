/*
========================================================================
 Name        : SGEAudioTesterSettingItemListFile1Settings.h
 Author      : ${author}
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : 
========================================================================
*/
#ifndef SGEAUDIOTESTERSETTINGITEMLISTFILE1SETTINGS_H
#define SGEAUDIOTESTERSETTINGITEMLISTFILE1SETTINGS_H
			
// [[[ begin generated region: do not modify [Generated Includes]
#include <e32std.h>
// ]]] end generated region [Generated Includes]

// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

/**
 * @class	TSGEAudioTesterSettingItemListFile1Settings SGEAudioTesterSettingItemListFile1Settings.h
 */
class TSGEAudioTesterSettingItemListFile1Settings
	{
public:
	// construct and destroy
	static TSGEAudioTesterSettingItemListFile1Settings* NewL();
	void ConstructL();
		
private:
	// constructor
	TSGEAudioTesterSettingItemListFile1Settings();
	// [[[ begin generated region: do not modify [Generated Accessors]
public:
	TInt& EnumeratedTextPopupFileName();
	void SetEnumeratedTextPopupFileName(const TInt& aValue);
	TInt& SliderVolume();
	void SetSliderVolume(const TInt& aValue);
	TInt& EnumeratedTextPopupRepeat();
	void SetEnumeratedTextPopupRepeat(const TInt& aValue);
	TInt& EnumeratedTextPopupKey();
	void SetEnumeratedTextPopupKey(const TInt& aValue);
	// ]]] end generated region [Generated Accessors]
	
	// [[[ begin generated region: do not modify [Generated Members]
protected:
	TInt iEnumeratedTextPopupFileName;
	TInt iSliderVolume;
	TInt iEnumeratedTextPopupRepeat;
	TInt iEnumeratedTextPopupKey;
	// ]]] end generated region [Generated Members]
	
	};
#endif // SGEAUDIOTESTERSETTINGITEMLISTFILE1SETTINGS_H
