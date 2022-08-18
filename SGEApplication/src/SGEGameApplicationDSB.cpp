/*
 ============================================================================
  Name        : SGEGameApplicationDSB.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#include "SGEGameApplicationDSB.h"
#include "SGEGameAppViewDSB.h"


// -----------------------------------------------------------------------------
// CSGEGameApplicationDSB::CreateAppViewL()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGESystemAppView* CSGEGameApplicationDSB::CreateAppViewL( TRect aRect )
    {
    iAppView = CSGEGameAppViewDSB::NewL( aRect, *this );
    return iAppView;
    }


// End of file
