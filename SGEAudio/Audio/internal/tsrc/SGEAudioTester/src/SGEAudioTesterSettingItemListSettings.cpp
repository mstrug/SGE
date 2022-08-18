/*
========================================================================
 Name        : SGEAudioTesterSettingItemListSettings.cpp
 Author      : Michal Strug
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : 
========================================================================
*/
/**
 *    Generated helper class which manages the settings contained 
 *    in 'SGEAudioTesterSettingItemList'.  Each CAknSettingItem maintains
 *    a reference to data in this class so that changes in the setting
 *    item list can be synchronized with this storage.
 */
 
// [[[ begin generated region: do not modify [Generated Includes]
#include <e32base.h>
#include <stringloader.h>
#include <barsread.h>
#include <SGEAudioTester.rsg>
#include "SGEAudioTesterSettingItemListSettings.h"
// ]]] end generated region [Generated Includes]

/**
 * C/C++ constructor for settings data, cannot throw
 */
TSGEAudioTesterSettingItemListSettings::TSGEAudioTesterSettingItemListSettings()
    {
    }

/**
 * Two-phase constructor for settings data
 */
TSGEAudioTesterSettingItemListSettings* TSGEAudioTesterSettingItemListSettings::NewL()
    {
    TSGEAudioTesterSettingItemListSettings* data = new( ELeave ) TSGEAudioTesterSettingItemListSettings;
    CleanupStack::PushL( data );
    data->ConstructL();
    CleanupStack::Pop( data );
    return data;
    }
    
/**
 *    Second phase for initializing settings data
 */
void TSGEAudioTesterSettingItemListSettings::ConstructL()
    {
    // [[[ begin generated region: do not modify [Generated Initializers]
	SetEnumeratedTextPopupSamplerate( 22050 );
	SetBinarySettingChannels( 0 );
	SetIntegerEditorBufferLength( 4096 );
	SetEnumeratedTextPopupBufferDataType( 2 );
	SetEnumeratedTextPopupSubsystem( 1 );
	SetBinarySettingMultithreeded( 0 );
	SetBinarySettingResampleFiles( 0 );
	SetVolumeSetting( 5 );
    // ]]] end generated region [Generated Initializers]
    
    }
    
// [[[ begin generated region: do not modify [Generated Contents]
TInt& TSGEAudioTesterSettingItemListSettings::EnumeratedTextPopupSamplerate()
	{
	return iEnumeratedTextPopupSamplerate;
	}

void TSGEAudioTesterSettingItemListSettings::SetEnumeratedTextPopupSamplerate(const TInt& aValue)
	{
	iEnumeratedTextPopupSamplerate = aValue;
	}

TBool& TSGEAudioTesterSettingItemListSettings::BinarySettingChannels()
	{
	return iBinarySettingChannels;
	}

void TSGEAudioTesterSettingItemListSettings::SetBinarySettingChannels(const TBool& aValue)
	{
	iBinarySettingChannels = aValue;
	}

TInt& TSGEAudioTesterSettingItemListSettings::IntegerEditorBufferLength()
	{
	return iIntegerEditorBufferLength;
	}

void TSGEAudioTesterSettingItemListSettings::SetIntegerEditorBufferLength(const TInt& aValue)
	{
	iIntegerEditorBufferLength = aValue;
	}

TInt& TSGEAudioTesterSettingItemListSettings::EnumeratedTextPopupBufferDataType()
	{
	return iEnumeratedTextPopupBufferDataType;
	}

void TSGEAudioTesterSettingItemListSettings::SetEnumeratedTextPopupBufferDataType(const TInt& aValue)
	{
	iEnumeratedTextPopupBufferDataType = aValue;
	}

TInt& TSGEAudioTesterSettingItemListSettings::EnumeratedTextPopupSubsystem()
	{
	return iEnumeratedTextPopupSubsystem;
	}

void TSGEAudioTesterSettingItemListSettings::SetEnumeratedTextPopupSubsystem(const TInt& aValue)
	{
	iEnumeratedTextPopupSubsystem = aValue;
	}

TBool& TSGEAudioTesterSettingItemListSettings::BinarySettingMultithreeded()
	{
	return iBinarySettingMultithreeded;
	}

void TSGEAudioTesterSettingItemListSettings::SetBinarySettingMultithreeded(const TBool& aValue)
	{
	iBinarySettingMultithreeded = aValue;
	}

TBool& TSGEAudioTesterSettingItemListSettings::BinarySettingResampleFiles()
	{
	return iBinarySettingResampleFiles;
	}

void TSGEAudioTesterSettingItemListSettings::SetBinarySettingResampleFiles(const TBool& aValue)
	{
	iBinarySettingResampleFiles = aValue;
	}

TInt& TSGEAudioTesterSettingItemListSettings::VolumeSetting()
	{
	return iVolumeSetting;
	}

void TSGEAudioTesterSettingItemListSettings::SetVolumeSetting(const TInt& aValue)
	{
	iVolumeSetting = aValue;
	}

// ]]] end generated region [Generated Contents]

