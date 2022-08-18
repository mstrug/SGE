
// SoundDevice.inl

// Copyright (c) 2001-2005 Symbian Software Limited. All rights reserved

#ifndef SOUNDDEVICE_INL
#define SOUNDDEVICE_INL

#include <mmf/server/MmfDevSoundCustomInterfaceSupport.h>
#include <mmf/common/mmfstandardcustomcommands.h>

inline TInt CMMFDevSound::RegisterAsClient(TUid aEventType, const TDesC8& aNotificationRegistrationData)
	{
	MAutoPauseResumeSupport* support = 
				static_cast<MAutoPauseResumeSupport*>(CustomInterface(KMmfUidDevSoundAudioResourceCustomInterface));
	if (support==NULL)
		{
		return KErrNotSupported;
		}
	if(aEventType != KMMFEventCategoryAudioResourceAvailable)
		{
		return KErrNotSupported;
		}
	else
		{ 
		return support->RegisterAsClient(aEventType, aNotificationRegistrationData);	
		}
	}

inline TInt CMMFDevSound::CancelRegisterAsClient(TUid aEventType)
	{
	MAutoPauseResumeSupport* support = 
			static_cast<MAutoPauseResumeSupport*>(CustomInterface(KMmfUidDevSoundAudioResourceCustomInterface));
	if (support==NULL)
		{
		return KErrNotSupported;
		}
	if(aEventType != KMMFEventCategoryAudioResourceAvailable)
		{
		return KErrNotSupported;
		}
	else
		{
		return support->CancelRegisterAsClient(aEventType);
		}
	}

inline TInt CMMFDevSound::GetResourceNotificationData(TUid aEventType,TDes8& aNotificationData)
	{
	MAutoPauseResumeSupport* support = 
			static_cast<MAutoPauseResumeSupport*>(CustomInterface(KMmfUidDevSoundAudioResourceCustomInterface));
	if (support==NULL)
		{
		return KErrNotSupported;
		}
	if(aEventType != KMMFEventCategoryAudioResourceAvailable)
		{
		return KErrNotSupported;
		}
	else
		{
		return support->GetResourceNotificationData(aEventType, aNotificationData);
		}
	}

inline TInt CMMFDevSound::WillResumePlay()
	{
	MAutoPauseResumeSupport* support = 
			static_cast<MAutoPauseResumeSupport*>(CustomInterface(KMmfUidDevSoundAudioResourceCustomInterface));
	if (support==NULL)
		{
		return KErrNotSupported;
		}
	return support->WillResumePlay();
	}

inline TInt CMMFDevSound::EmptyBuffers()
	{
	MMMFDevSoundEmptyBuffers* emptybuffers = 
			static_cast<MMMFDevSoundEmptyBuffers*>(CustomInterface(KMmfUidDevSoundEmptyBuffersCustomInterface));
	if (emptybuffers == NULL)
		{
		return KErrNotSupported;
		}
	return emptybuffers->EmptyBuffers();
	}

// SOUNDDEVICE_INL
#endif
