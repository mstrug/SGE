// MmfHwDevice.inl
//
// Copyright (c) 2002 Symbian Ltd.  All rights reserved.
//
#ifndef __MMF_HW_DEVICE_INL__
#define __MMF_HW_DEVICE_INL__

#include <ECom.h>


/**
Create a CMMFHwDevice object from .dll.
Will leave if dll cannot be loaded (KErrNotFound).

@param dll Implementation .dll

@return an instantiated CMMFHwDevice derived object from a DLL.
*/
inline CMMFHwDevice* CMMFHwDevice::NewL(TFileName dll)
	{
	RLibrary library;
	User::LeaveIfError(library.Load(dll));
	TLibraryFunction entry=library.Lookup(2); // 1 ponits to implementation table
	CMMFHwDevice* mmfHwDevice = (CMMFHwDevice*)entry();
	return mmfHwDevice;
	}

/**
 *
 * Destructor
 *
 */

CMMFHwDevice::~CMMFHwDevice()
	{
	// Destroy any instance variables and then
	// inform ecom that this specific
	// instance of the interface has been destroyed.
	REComSession::DestroyedImplementation(iDtor_ID_Key);
	}

/**
 *
 *	Create a CMMFHwDevice object with a known implementation Uid.
 *  Will leave if it is not found (KErrNotFound).
 *
 *	@param aUid   -  The Uid of a plugin implementation
 *
 *	@return An instantiated CMMFHwDevice derived object from ECom.
 *
 */
inline CMMFHwDevice* CMMFHwDevice::NewL(TUid aUid)
	{
	CMMFHwDevice* device = REINTERPRET_CAST(CMMFHwDevice*,
											REComSession::CreateImplementationL(aUid, _FOFF(CMMFHwDevice,iDtor_ID_Key)));
	return device;
	}

#endif
