/*
 ============================================================================
 Name        : tsvgapp.cpp
 Author      : Michal Strug
 Company     : 
 Version     : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : 
 ============================================================================
 */

// INCLUDE FILES
#include "tsvgappApplication.h"

// -----------------------------------------------------------------------------
// E32Main()
// -----------------------------------------------------------------------------
//
GLDEF_C TInt E32Main()
	{
	return CtsvgappApplication::Start(CtsvgappApplication::NewApplication);
	}

// End of file
