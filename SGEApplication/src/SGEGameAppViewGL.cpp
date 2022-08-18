/*
 ============================================================================
  Name        : SGEGameAppViewGL.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

// INCLUDE FILES
#include "SGEGameAppViewGL.h"
#include "SGEGameApplication.h"
#include "SGEGameApplicationGL.h"
#include <SGEEgl/EglDevice.h>
#include <SGEEgl/EglSurfaceWindow.h>
#include <SGEEgl/EglSurfacePixmap.h>
#include <SGEEgl/EglSurfacePbuffer.h>
#include <SGEEgl/EglRc.h>



// -----------------------------------------------------------------------------
// CSGEGameAppViewGL::NewL()
// -----------------------------------------------------------------------------
//
CSGEGameAppViewGL* CSGEGameAppViewGL::NewL( const TRect& aRect, TEglSurfaceType aEglSurfaceType,
                                            CSGEGameApplication& aApplication )
    {
    CSGEGameAppViewGL* self = CSGEGameAppViewGL::NewLC( aRect, aEglSurfaceType, aApplication );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewGL::NewLC()
// -----------------------------------------------------------------------------
//
CSGEGameAppViewGL* CSGEGameAppViewGL::NewLC( const TRect& aRect, TEglSurfaceType aEglSurfaceType,
                                             CSGEGameApplication& aApplication )
    {
    CSGEGameAppViewGL* self = new (ELeave) CSGEGameAppViewGL( aEglSurfaceType, KDepthSize, KSampleBuffers, KSampleSize, aApplication );
    CleanupStack::PushL( self );
    self->ConstructL( aRect, EFalse );
    return self;
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewGL::NewL()
// -----------------------------------------------------------------------------
//
CSGEGameAppViewGL* CSGEGameAppViewGL::NewL( const TRect& aRect, TEglSurfaceType aEglSurfaceType, TInt aDepthSize, TInt aSampleBuffers, TInt aSampleSize,
                                    CSGEGameApplication& aApplication )
    {
    CSGEGameAppViewGL* self = new (ELeave) CSGEGameAppViewGL( aEglSurfaceType, aDepthSize, aSampleBuffers, aSampleSize, aApplication );
    CleanupStack::PushL( self );
    self->ConstructL( aRect, EFalse );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewGL::ConstructL()
// -----------------------------------------------------------------------------
//
void CSGEGameAppViewGL::ConstructL( const TRect& aRect, TBool aOpenVGSupport )
    {
    CSGESystemAppView::ConstructL( aRect );
    
    ConstructGLL( aOpenVGSupport );
    
    InitViewport();
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewGL::CSGEGameAppViewGL()
// -----------------------------------------------------------------------------
//
CSGEGameAppViewGL::CSGEGameAppViewGL( TEglSurfaceType aEglSurfaceType, TInt aDepthSize, TInt aSampleBuffers, TInt aSampleSize, CSGEGameApplication& aApplication ) 
    : CSGESystemAppView( aApplication ), iEglSurfaceType( aEglSurfaceType ), iDepthSize( aDepthSize ), iSampleBuffers( aSampleBuffers ), iSampleSize( aSampleSize )
    {
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewGL::~CSGEGameAppViewGL()
// -----------------------------------------------------------------------------
//
CSGEGameAppViewGL::~CSGEGameAppViewGL()
    {
    delete iRc;
    delete iSurface;
    delete iDevice;
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewGL::ConstructGLL()
// -----------------------------------------------------------------------------
//
void CSGEGameAppViewGL::ConstructGLL( TBool aOpenVGSupport )
    {
    iDevice = CEglDevice::NewL();

    // Define properties for the wanted EGLSurface
    TDisplayMode dMode = Window().DisplayMode();
    TInt bufferSize = 0;
    
    const TInt KBufferSize12 = 12;
    const TInt KBufferSize16 = 16;
    const TInt KBufferSize24 = 24;
    const TInt KBufferSize32 = 32;
    switch ( dMode )
        {
        case EColor4K:
            bufferSize = KBufferSize12;
            break;
        case EColor64K:
            bufferSize = KBufferSize16;
            break;
        case EColor16M:
            bufferSize = KBufferSize24;
            break;
        case EColor16MU:
        case EColor16MA:
            bufferSize = KBufferSize32;
            break;
        default:
            User::Leave( KErrGeneral );
            break;
        }

    TInt surfaceType;
    switch ( iEglSurfaceType )
        {
        case EEglSurfacePbuffer: 
                surfaceType = EGL_PBUFFER_BIT;
            break;
        case EEglSurfacePixmap: 
                surfaceType = EGL_PIXMAP_BIT;
            break;
        default: // EEglSurfaceWindow
                surfaceType = EGL_WINDOW_BIT;
            break;
        }

    // Define properties for requesting a non-antialiased window surface
    const EGLint attrib_list[] =
        {
        EGL_SURFACE_TYPE, surfaceType,
        EGL_BUFFER_SIZE, bufferSize,
        EGL_DEPTH_SIZE, iDepthSize,
        EGL_RENDERABLE_TYPE, ( aOpenVGSupport ? ( EGL_OPENGL_ES_BIT | EGL_OPENVG_BIT ) : EGL_OPENGL_ES_BIT ),
        EGL_NONE
        };
       
    if ( iSampleBuffers && iSampleSize )
        {
        // Define properties for requesting a full-screen antialiased window surface
        const EGLint attrib_list_fsaa[] =
            {
            EGL_SURFACE_TYPE, surfaceType,
            EGL_BUFFER_SIZE, bufferSize,
            EGL_DEPTH_SIZE, iDepthSize,
            EGL_SAMPLE_BUFFERS, iSampleBuffers,
            EGL_SAMPLES, iSampleSize,
            EGL_RENDERABLE_TYPE, ( aOpenVGSupport ? ( EGL_OPENGL_ES_BIT | EGL_OPENVG_BIT ) : EGL_OPENGL_ES_BIT ),
            EGL_NONE
            };

        iDevice->Configure( attrib_list_fsaa, attrib_list );
        }
    else
        {
        iDevice->Configure( attrib_list );
        }
 

    if ( iEglSurfaceType == EEglSurfacePixmap )
        {
        iSurface = CEglSurfacePixmap::NewL( *iDevice, 
                                            static_cast<CSGEGameApplicationGL&>( iApplication ).WsBitmap() );
        }
    else if ( iEglSurfaceType == EEglSurfacePbuffer )
        {
        iSurface = CEglSurfacePbuffer::NewL( *iDevice, Window().Size(), EFalse, EFalse, EFalse, 0, 0 );
        }
    else
        {
        iSurface = CEglSurfaceWindow::NewL( *iDevice, Window() );
        }

    iRc = CEglRc::NewL( *iDevice );
    
    iRc->MakeCurrent( *iSurface );
    
    iOpenGlInitialized = ETrue;
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewGL::InitViewport()
// -----------------------------------------------------------------------------
//
void CSGEGameAppViewGL::InitViewport()
    {
    glViewport( 0, 0, Size().iWidth, Size().iHeight );
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewGL::SwapBuffers()
// -----------------------------------------------------------------------------
//
void CSGEGameAppViewGL::SwapBuffers()
    {
    if ( iEglSurfaceType == EEglSurfaceWindow )
        {
        static_cast<CEglSurfaceWindow*>( iSurface )->SwapBuffers();
        }
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewGL::EglDevice()
// -----------------------------------------------------------------------------
//
CEglDevice *CSGEGameAppViewGL::EglDevice() const
    {
    return iDevice;
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewGL::EglSurface()
// -----------------------------------------------------------------------------
//
CEglSurface *CSGEGameAppViewGL::EglSurface() const
    {
    return iSurface;
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewGL::EglRenderingContext()
// -----------------------------------------------------------------------------
//
CEglRc *CSGEGameAppViewGL::EglRenderingContext() const
    {
    return iRc;
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewGL::SizeChanged()
// -----------------------------------------------------------------------------
//
void CSGEGameAppViewGL::SizeChanged()
    {
    if ( iOpenGlInitialized )
        {
        InitViewport();
        CSGESystemAppView::SizeChanged();
        }
    }

// End of File
