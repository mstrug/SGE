/*
 ============================================================================
  Name        : SGEGameAppViewVG.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

// INCLUDE FILES
#include "SGEGameAppViewVG.h"
#include "SGEGameApplication.h"
#include "SGEGameApplicationVG.h"
#include <SGEEgl/EglDevice.h>
#include <SGEEgl/EglSurfaceWindow.h>
#include <SGEEgl/EglSurfacePixmap.h>
#include <SGEEgl/EglSurfacePbuffer.h>
#include <SGEEgl/EglRc.h>



// -----------------------------------------------------------------------------
// CSGEGameAppViewVG::NewL()
// -----------------------------------------------------------------------------
//
CSGEGameAppViewVG* CSGEGameAppViewVG::NewL( const TRect& aRect, TEglSurfaceType aEglSurfaceType,
                                            CSGEGameApplication& aApplication )
    {
    CSGEGameAppViewVG* self = CSGEGameAppViewVG::NewLC( aRect, aEglSurfaceType, aApplication );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewVG::NewLC()
// -----------------------------------------------------------------------------
//
CSGEGameAppViewVG* CSGEGameAppViewVG::NewLC( const TRect& aRect, TEglSurfaceType aEglSurfaceType,
                                             CSGEGameApplication& aApplication )
    {
    CSGEGameAppViewVG* self = new (ELeave) CSGEGameAppViewVG( aEglSurfaceType, KDepthSize, KSampleBuffers, KSampleSize, aApplication );
    CleanupStack::PushL( self );
    self->ConstructL( aRect );
    return self;
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewVG::NewL()
// -----------------------------------------------------------------------------
//
CSGEGameAppViewVG* CSGEGameAppViewVG::NewL( const TRect& aRect, TEglSurfaceType aEglSurfaceType, TInt aDepthSize, TInt aSampleBuffers, TInt aSampleSize,
                                    CSGEGameApplication& aApplication )
    {
    CSGEGameAppViewVG* self = new (ELeave) CSGEGameAppViewVG( aEglSurfaceType, aDepthSize, aSampleBuffers, aSampleSize, aApplication );
    CleanupStack::PushL( self );
    self->ConstructL( aRect );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewVG::ConstructL()
// -----------------------------------------------------------------------------
//
void CSGEGameAppViewVG::ConstructL( const TRect& aRect )
    {
	TBool glSupport = static_cast<CSGEGameApplicationVG&>( iApplication ).SupportGL();
	
	if ( glSupport )
		{
		eglBindAPI( EGL_OPENGL_ES_API );
		}
	else
		{
	    eglBindAPI( EGL_OPENVG_API );
		}

    CSGEGameAppViewGL::ConstructL( aRect, ETrue );
    
    if ( glSupport )
    	{
    	eglBindAPI( EGL_OPENVG_API );
    
    	iRcVG = CEglRc::NewL( *iDevice );
    
    	iRcVG->MakeCurrent( *iSurface );
    	}
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewVG::CSGEGameAppViewVG()
// -----------------------------------------------------------------------------
//
CSGEGameAppViewVG::CSGEGameAppViewVG( TEglSurfaceType aEglSurfaceType, TInt aDepthSize, TInt aSampleBuffers, TInt aSampleSize, CSGEGameApplication& aApplication ) 
    : CSGEGameAppViewGL( aEglSurfaceType, aDepthSize, aSampleBuffers, aSampleSize, aApplication )
    {
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewVG::~CSGEGameAppViewVG()
// -----------------------------------------------------------------------------
//
CSGEGameAppViewVG::~CSGEGameAppViewVG()
    {
    delete iRcVG;
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewVG::EglRenderingContextVG()
// -----------------------------------------------------------------------------
//
CEglRc *CSGEGameAppViewVG::EglRenderingContextVG() const
    {
    return iRcVG;
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewVG::InitViewport()
// -----------------------------------------------------------------------------
//
void CSGEGameAppViewVG::InitViewport()
	{	
	}

// End of File
