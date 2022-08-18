/*
========================================================================
 Name        : SGEAudioTesterSettingItemListSettings.h
 Author      : Michal Strug
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : 
========================================================================
*/
#ifndef SGEAUDIOTESTERSETTINGITEMLISTSETTINGS_H
#define SGEAUDIOTESTERSETTINGITEMLISTSETTINGS_H
            
// [[[ begin generated region: do not modify [Generated Includes]
#include <e32std.h>
// ]]] end generated region [Generated Includes]

// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

/**
 * @class    TSGEAudioTesterSettingItemListSettings SGEAudioTesterSettingItemListSettings.h
 */
class TSGEAudioTesterSettingItemListSettings
    {
public:
    // construct and destroy
    static TSGEAudioTesterSettingItemListSettings* NewL();
    void ConstructL();
        
private:
    // constructor
    TSGEAudioTesterSettingItemListSettings();
    // [[[ begin generated region: do not modify [Generated Accessors]
public:
	TInt& EnumeratedTextPopupSamplerate();
	void SetEnumeratedTextPopupSamplerate(const TInt& aValue);
	TBool& BinarySettingChannels();
	void SetBinarySettingChannels(const TBool& aValue);
	TInt& IntegerEditorBufferLength();
	void SetIntegerEditorBufferLength(const TInt& aValue);
	TInt& EnumeratedTextPopupBufferDataType();
	void SetEnumeratedTextPopupBufferDataType(const TInt& aValue);
	TInt& EnumeratedTextPopupSubsystem();
	void SetEnumeratedTextPopupSubsystem(const TInt& aValue);
	TBool& BinarySettingMultithreeded();
	void SetBinarySettingMultithreeded(const TBool& aValue);
	TBool& BinarySettingResampleFiles();
	void SetBinarySettingResampleFiles(const TBool& aValue);
	TInt& VolumeSetting();
	void SetVolumeSetting(const TInt& aValue);
    // ]]] end generated region [Generated Accessors]
    
    // [[[ begin generated region: do not modify [Generated Members]
protected:
	TInt iEnumeratedTextPopupSamplerate;
	TBool iBinarySettingChannels;
	TInt iIntegerEditorBufferLength;
	TInt iEnumeratedTextPopupBufferDataType;
	TInt iEnumeratedTextPopupSubsystem;
	TBool iBinarySettingMultithreeded;
	TBool iBinarySettingResampleFiles;
	TInt iVolumeSetting;
    // ]]] end generated region [Generated Members]
    
    };
#endif // SGEAUDIOTESTERSETTINGITEMLISTSETTINGS_H
