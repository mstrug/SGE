/*
 ============================================================================
  Name        : SGEGameApplicationVG.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#include "SGEGameApplicationVG.h"
#include "SGEGameAppViewVG.h"
#include <SGEEgl/EglDevice.h>
#include <SGEEgl/EglSurface.h>
#include <SGEEgl/EglRc.h>



// -----------------------------------------------------------------------------
// CSGEGameApplicationVG::EglRenderingContext()
// -----------------------------------------------------------------------------
//
EXPORT_C CEglRc* CSGEGameApplicationVG::EglRenderingContext() const
    {
	if ( SupportGL() )
		{
		return static_cast<CSGEGameAppViewVG*>( iAppView )->EglRenderingContext();
		}
	else
		{
		return NULL;
		}
    }

// -----------------------------------------------------------------------------
// CSGEGameApplicationVG::EglRenderingContextVG()
// -----------------------------------------------------------------------------
//
EXPORT_C CEglRc* CSGEGameApplicationVG::EglRenderingContextVG() const
    {
	if ( SupportGL() )
		{
		return static_cast<CSGEGameAppViewVG*>( iAppView )->EglRenderingContext();
		}
	else
		{
		return static_cast<CSGEGameAppViewVG*>( iAppView )->EglRenderingContextVG();
		}
    }

// -----------------------------------------------------------------------------
// CSGEGameApplicationVG::MakeCurrentWindowEglContext()
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CSGEGameApplicationVG::MakeCurrentWindowEglContext()
    {
	if ( SupportGL() )
		{
		eglBindAPI( EGL_OPENGL_ES_API );
		return EglRenderingContext()->MakeCurrent( *EglSurface() );
		}
	else
		{
		return KErrNotSupported;
		}
    }

// -----------------------------------------------------------------------------
// CSGEGameApplicationVG::MakeCurrentWindowEglContextVG()
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CSGEGameApplicationVG::MakeCurrentWindowEglContextVG()
    {
    eglBindAPI( EGL_OPENVG_API );
    return EglRenderingContextVG()->MakeCurrent( *EglSurface() );
    }

// -----------------------------------------------------------------------------
// CSGEGameApplicationVG::SupportGL()
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSGEGameApplicationVG::SupportGL() const
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CSGEGameApplicationVG::CreateAppViewL()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGESystemAppView* CSGEGameApplicationVG::CreateAppViewL( TRect aRect )
    {
    iAppView = CSGEGameAppViewVG::NewL( aRect, (CSGEGameAppViewGL::TEglSurfaceType) SurfaceType(), DepthSize(), SampleBuffers(), SampleSize(), *this );
    return iAppView;
    }


// End of file
