/*
 ============================================================================
  Name        : SGESystemDocument.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

// INCLUDE FILES
#include "SGESystemDocument.h"
#include "SGESystemAppUi.h"
#include "SGEApplication.h"


// -----------------------------------------------------------------------------
// CSGESystemDocument::NewL()
// -----------------------------------------------------------------------------
//
CSGESystemDocument* CSGESystemDocument::NewL( CEikApplication& aEikApp, CSGEApplication& aApplication )
    {
    CSGESystemDocument *self = NewLC( aEikApp, aApplication );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CSGESystemDocument::NewLC()
// -----------------------------------------------------------------------------
//
CSGESystemDocument* CSGESystemDocument::NewLC( CEikApplication& aEikApp, CSGEApplication& aApplication )
    {
    CSGESystemDocument* self = new (ELeave) CSGESystemDocument( aEikApp, aApplication );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CSGESystemDocument::ConstructL()
// -----------------------------------------------------------------------------
//
void CSGESystemDocument::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CSGESystemDocument::CSGESystemDocument()
// -----------------------------------------------------------------------------
//
CSGESystemDocument::CSGESystemDocument( CEikApplication& aEikApp, CSGEApplication& aApplication ) 
        : CAknDocument( aEikApp ), iApplication( aApplication )
    {
    }

// ---------------------------------------------------------------------------
// CSGESystemDocument::~CSGESystemDocument()
// ---------------------------------------------------------------------------
//
CSGESystemDocument::~CSGESystemDocument()
    {
    }

// ---------------------------------------------------------------------------
// CSGESystemDocument::CreateAppUiL()
// ---------------------------------------------------------------------------
//
CEikAppUi* CSGESystemDocument::CreateAppUiL()
    {
    return iApplication.CreateAppUiL();
    }

// End of File
