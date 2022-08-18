
// MmfDevSoundCustomInterfaceSupport.h

// Copyright (c) Symbian Software Ltd 2005-2006.  All rights reserved.

#ifndef MMFDEVSOUNDCUSTOMINTERFACESUPPORT_H
#define MMFDEVSOUNDCUSTOMINTERFACESUPPORT_H

const TUid KMmfUidDevSoundAudioResourceCustomInterface = {0x101FD9F3};
const TUid KMmfUidDevSoundEmptyBuffersCustomInterface = {0x1027379c};

/**
@publishedAll
@released

Custom Interface providing support for AutoPauseResume Register, Cancel, GetResourceNotificationData and WillResumePlay.
*/

class MAutoPauseResumeSupport
	{
public:
	/**
	Registers the event for notification when resource is avaliable.

	@param aEventType
		The event which the client is notified of.
		
	@param aNotificationRegistrationData
		Notification registration specific data, which has been reserved for future use.

	@return An error code indicating if the function call was successful. KErrNone on success, otherwise
	        another of the system-wide error codes.
	*/
	virtual TInt RegisterAsClient(TUid aEventType, const TDesC8& aNotificationRegistrationData = KNullDesC8)=0;
	
	/**
	Cancels the registered notification event.

	@param  aEventType
		The event to notify the client.

	@return An error code indicating if the function call was successful. KErrNone on success, otherwise
	        another of the system-wide error codes.
	*/
	virtual TInt CancelRegisterAsClient(TUid aEventType)=0;
	/**
	Gets the notification data for the event.
	
	@param  aEventType
		The event which the client is notified of.
		
	@param aNotificationData
		The notification data for the client to resume playing. The actual data depends on the event type.
		Note that for the event type 'KMMFEventCategoryAudioResourceAvailable' the package buffer returned
		is TMMFTimeIntervalMicroSecondsPckg, but the contents should be converted to an integer and
		interpreted as the data returned is samples played, but not as a microsecond value.

	@return An error code indicating if the function call was successful. KErrNone on success, otherwise
	        another of the system-wide error codes.
	*/
	virtual TInt GetResourceNotificationData(TUid aEventType,TDes8& aNotificationData)=0;
	/**
	Waits for the client to resume the play even after the default timer expires.

	@return An error code indicating if the function call was successful. KErrNone on success, otherwise
	        another of the system-wide error codes.
	*/
	virtual TInt WillResumePlay()=0;
	};

/**
@publishedPartner
@prototype

Custom Interface class providing support for emptying the play buffers.
*/
class MMMFDevSoundEmptyBuffers
	{	
public:
	
	/**
	Empties the play buffers below DevSound without causing the codec to be deleted.

	@return An error code indicating if the function call was successful. KErrNone on success, KErrNotSupported 
			if called in record mode, otherwise another of the system-wide error codes.
	*/
	virtual TInt EmptyBuffers() = 0;
	};	

/**
@internal

The class is a mixin and is intended to support for closing the Mux and Demux pair in a
DevSound Custom Interface. The function encapsulated by this class is called whenever the 
MMMFDevSoundCustomInterfaceDeMuxPlugin::RefreshL method leaves.
*/
class MMMFDevSoundCustomInterfaceObserver
	{	
public:
	
	/**
	Closes the Mux and Demux pair of a DevSound Custom Interface.
	*/
	virtual void CloseCustomInterface(TInt aIndex) = 0;
	};	


// MMFDEVSOUNDCUSTOMINTERFACESUPPORT_H
#endif
