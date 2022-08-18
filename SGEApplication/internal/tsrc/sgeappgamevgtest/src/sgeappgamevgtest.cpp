/*
 ============================================================================
 Name		: sgeappgamevgtest.cpp
 Author	  : 
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : Main application class
 ============================================================================
 */

// INCLUDE FILES

#include "MyApplication.h"


GLDEF_C TInt E32Main()
    {
    return CMyApplication::Start( CMyApplication::NewApplication );
    }

