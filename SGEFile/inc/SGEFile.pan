/*
 ============================================================================
 Name		: SGEFile.pan
 Author	  : Michal Strug
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : Panic codes
 ============================================================================
 */

#ifndef __SGEFILE_PAN__
#define __SGEFILE_PAN__

//  Data Types

enum TSGEFilePanic
	{
	ESGEFileNullPointer
	};

//  Function Prototypes

GLREF_C void Panic(TSGEFilePanic aPanic);

#endif  // __SGEFILE_PAN__

