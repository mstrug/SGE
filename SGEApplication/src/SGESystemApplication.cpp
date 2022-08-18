/*
 ============================================================================
  Name        : SGEApplication.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

// INCLUDE FILES
#include "SGESystemApplication.h"
#include "SGESystemDocument.h"
#include "SGEApplication.h"


// -----------------------------------------------------------------------------
// CSGESystemApplication::CSGESystemApplication()
// -----------------------------------------------------------------------------
//
CSGESystemApplication::CSGESystemApplication( CSGEApplication *aApplication ) 
        : iApplication( aApplication )
    {
    }

// -----------------------------------------------------------------------------
// CSGESystemApplication::~CSGESystemApplication()
// -----------------------------------------------------------------------------
//
CSGESystemApplication::~CSGESystemApplication()
    {
    delete iApplication;
    }

// -----------------------------------------------------------------------------
// CSGESystemApplication::CreateDocumentL()
// -----------------------------------------------------------------------------
//
CApaDocument* CSGESystemApplication::CreateDocumentL()
    {
    return CSGESystemDocument::NewL( *this, *iApplication );
    }

// -----------------------------------------------------------------------------
// CSGESystemApplication::AppDllUid()
// -----------------------------------------------------------------------------
//
TUid CSGESystemApplication::AppDllUid() const
    {
    return iApplication->ApplicationUid();
    }


// End of File
