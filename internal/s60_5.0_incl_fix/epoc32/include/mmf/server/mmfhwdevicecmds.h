// MMFHwDeviceCmds.h
//
// Copyright (c) 2002 Symbian Ltd.  All rights reserved.
//

#if !defined(__MMFHWDEVICECMDS_H__)
#define __MMFHWDEVICECMDS_H__

enum TDeviceFunc
{
	EDevNullFunc,
	EDevEncode,
	EDevDecode
};

enum TDeviceFlow
{
	EDevNullFlow,
	EDevOutFlow,
	EDevInFlow,
	EDevInAndOutFlow,
	EDevNoFlow
};

#endif __MMFHWDEVICECMDS_H__
