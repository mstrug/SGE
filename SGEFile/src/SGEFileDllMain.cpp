/*
 ============================================================================
 Name		: SGEFileDll.cpp 
 Author	  : Michal Strug
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : SGEFileDll.cpp - main DLL source
 ============================================================================
 */

//  Include Files  

#include <e32std.h>		 // GLDEF_C
#include "SGEFile.pan"		// panic codes

//  Global Functions

GLDEF_C void Panic(TSGEFilePanic aPanic)
// Panics the thread with given panic code
	{
	User::Panic(_L("SGEFile"), aPanic);
	}

//  Exported Functions

#ifndef EKA2 // for EKA1 only
EXPORT_C TInt E32Dll(TDllReason /*aReason*/)
// Called when the DLL is loaded and unloaded. Note: have to define
// epoccalldllentrypoints in MMP file to get this called in THUMB.

	{
	return KErrNone;
	}
#endif

