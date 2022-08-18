/*
 ============================================================================
 Name        : sgeEglLog.cpp
 Author      : 
 Company     : 
 Version     : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : 
 ============================================================================
 */

// INCLUDE FILES
#include "sgeEglLogApplication.h"

// -----------------------------------------------------------------------------
// E32Main()
// -----------------------------------------------------------------------------
//
GLDEF_C TInt E32Main()
	{
	return CsgeEglLogApplication::Start(CsgeEglLogApplication::NewApplication);
	}

// End of file
