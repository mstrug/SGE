/*
 ============================================================================
  Name        : SGEGameApplicationDS.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#include "SGEGameApplicationDS.h"
#include "SGEGameAppViewDS.h"


// -----------------------------------------------------------------------------
// CSGEGameApplicationDS::Gc()
// -----------------------------------------------------------------------------
//
EXPORT_C CFbsBitGc* CSGEGameApplicationDS::Gc() const
    {
    return static_cast<CSGEGameAppViewDS*>( iAppView )->Gc();
    }

// -----------------------------------------------------------------------------
// CSGEGameApplicationDS::Size()
// -----------------------------------------------------------------------------
//
EXPORT_C TSize CSGEGameApplicationDS::Size() const
    {
    return iAppView->Size();
    }

// -----------------------------------------------------------------------------
// CSGEGameApplicationDS::DisplayMode()
// -----------------------------------------------------------------------------
//
EXPORT_C TDisplayMode CSGEGameApplicationDS::DisplayMode() const
    {
    return static_cast<CSGEGameAppViewDS*>( iAppView )->DisplayMode();
    }

// -----------------------------------------------------------------------------
// CSGEGameApplicationDS::GameLoopEvent()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEGameApplicationDS::GameLoopEvent()
    {
    CSGEGameApplication::GameLoopEvent();
    
    static_cast<CSGEGameAppViewDS*>( iAppView )->BitBltOffscreenBitmap();
    }

// -----------------------------------------------------------------------------
// CSGEGameApplicationDS::CreateAppViewL()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGESystemAppView* CSGEGameApplicationDS::CreateAppViewL( TRect aRect )
    {
    iAppView = CSGEGameAppViewDS::NewL( aRect, *this );
    return iAppView;
    }


// End of file
