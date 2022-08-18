/*
 ============================================================================
  Name        : SGEGameApplicationGL.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#include "SGEGameApplicationGL.h"
#include "SGEGameAppViewGL.h"
#include <SGEEgl/EglDevice.h>
#include <SGEEgl/EglSurface.h>
#include <SGEEgl/EglRc.h>


// -----------------------------------------------------------------------------
// CSGEGameApplicationGL::Size()
// -----------------------------------------------------------------------------
//
EXPORT_C TSize CSGEGameApplicationGL::Size() const
    {
    return iAppView->Size();
    }

// -----------------------------------------------------------------------------
// CSGEGameApplicationGL::EglDevice()
// -----------------------------------------------------------------------------
//
EXPORT_C CEglDevice* CSGEGameApplicationGL::EglDevice() const
    {
    return static_cast<CSGEGameAppViewGL*>( iAppView )->EglDevice();
    }

// -----------------------------------------------------------------------------
// CSGEGameApplicationGL::EglWindowSurface()
// -----------------------------------------------------------------------------
//
EXPORT_C CEglSurface* CSGEGameApplicationGL::EglSurface() const
    {
    return static_cast<CSGEGameAppViewGL*>( iAppView )->EglSurface();
    }

// -----------------------------------------------------------------------------
// CSGEGameApplicationGL::EglRenderingContext()
// -----------------------------------------------------------------------------
//
EXPORT_C CEglRc* CSGEGameApplicationGL::EglRenderingContext() const
    {
    return static_cast<CSGEGameAppViewGL*>( iAppView )->EglRenderingContext();
    }

// -----------------------------------------------------------------------------
// CSGEGameApplicationGL::MakeCurrentWindowEglContext()
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CSGEGameApplicationGL::MakeCurrentWindowEglContext()
    {
    return EglRenderingContext()->MakeCurrent( *EglSurface() );
    }

// -----------------------------------------------------------------------------
// CSGEGameApplicationGL::SurfaceType()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEGameApplicationGL::TEglSurfaceType CSGEGameApplicationGL::SurfaceType()
    {
    return EEglSurfaceWindow;
    }

// -----------------------------------------------------------------------------
// CSGEGameApplicationGL::DepthSize()
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CSGEGameApplicationGL::DepthSize()
    {
    const TInt KDepthSize = 16;
    return KDepthSize;
    }

// -----------------------------------------------------------------------------
// CSGEGameApplicationGL::SampleBuffers()
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CSGEGameApplicationGL::SampleBuffers()
    {
    const TInt KSampleBuffers = 1;
    return KSampleBuffers;
    }

// -----------------------------------------------------------------------------
// CSGEGameApplicationGL::SampleSize()
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CSGEGameApplicationGL::SampleSize()
    {
    const TInt KSampleSize = 4;
    return KSampleSize;
    }

// -----------------------------------------------------------------------------
// CSGEGameApplicationGL::WsBitmap()
// -----------------------------------------------------------------------------
//
EXPORT_C CWsBitmap* CSGEGameApplicationGL::WsBitmap()
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// CSGEGameApplicationGL::GameLoopEvent()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEGameApplicationGL::GameLoopEvent()
    {
    CSGEGameApplication::GameLoopEvent();

    if ( SurfaceType() == EEglSurfaceWindow )
        {
        static_cast<CSGEGameAppViewGL*>( iAppView )->SwapBuffers();
        }
    }

// -----------------------------------------------------------------------------
// CSGEGameApplicationGL::CreateAppViewL()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGESystemAppView* CSGEGameApplicationGL::CreateAppViewL( TRect aRect )
    {
    iAppView = CSGEGameAppViewGL::NewL( aRect, (CSGEGameAppViewGL::TEglSurfaceType) SurfaceType(), DepthSize(), SampleBuffers(), SampleSize(), *this );
    return iAppView;
    }


// End of file
