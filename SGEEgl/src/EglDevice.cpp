/*
 ============================================================================
 Name		: EglDevice.cpp
 Author	  : Michal Strug
 Version	 : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEglDevice implementation
 ============================================================================
 */

#include "EglDevice.h"

const TInt KVersionMajor = 1;
const TInt KVersionMinor = 1;


CEglDevice::CEglDevice()
    {
    // No implementation required
    }

EXPORT_C CEglDevice::~CEglDevice()
    {
    Terminate();
    }

EXPORT_C CEglDevice* CEglDevice::NewLC()
    {
    CEglDevice *self = new (ELeave) CEglDevice();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

EXPORT_C CEglDevice* CEglDevice::NewL()
    {
    CEglDevice *self = CEglDevice::NewLC();
    CleanupStack::Pop(); // self;
    return self;
    }

void CEglDevice::ConstructL()
    {
    User::LeaveIfError( Initialize() );
    }

//
// INTERNAL FUNCTIONS
//

TInt CEglDevice::Initialize()
    {
    EGLint vMajor;
    EGLint vMinor;
    
    iDisplay = eglGetDisplay( EGL_DEFAULT_DISPLAY );

    if ( iDisplay == EGL_NO_DISPLAY )
        {
        return KErrNotFound;
        }
    
    if ( eglInitialize( iDisplay, &vMajor, &vMinor ) == EGL_FALSE )
        {
        EGLint error = eglGetError();
        if ( error == EGL_BAD_DISPLAY )
            {
            return KErrArgument;
            }
        else if ( error == EGL_NOT_INITIALIZED )
            {
            return KErrUnknown;
            }
        return KErrGeneral;
        }

    ASSERT( vMajor >= KVersionMajor );
    ASSERT( vMinor >= KVersionMinor );

    return KErrNone;
    }

void CEglDevice::Terminate()
    {
    eglMakeCurrent( iDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
    eglTerminate( iDisplay );
    }

EXPORT_C EGLDisplay CEglDevice::Display() const
    {
    return iDisplay;
    }

EXPORT_C EGLConfig CEglDevice::Config() const
    {
    return iConfig;
    }

EXPORT_C TInt CEglDevice::Configure( TBool aWindow, TBool aPixmap, TBool aPBuffer )
    {
    ASSERT( aWindow | aPixmap | aPBuffer );
    
    const EGLint config1[] = 
        { 
        EGL_SURFACE_TYPE, ( aWindow ? EGL_WINDOW_BIT : 0 ) | 
                          ( aPixmap ? EGL_PIXMAP_BIT : 0 ) | 
                          ( aPBuffer ? EGL_PBUFFER_BIT : 0 ),
        EGL_BUFFER_SIZE, 32,
        EGL_DEPTH_SIZE, 16,
        EGL_NONE 
        };
    
    const EGLint config2[] = 
        { 
        EGL_SURFACE_TYPE, ( aWindow ? EGL_WINDOW_BIT : 0 ) | 
                          ( aPixmap ? EGL_PIXMAP_BIT : 0 ) | 
                          ( aPBuffer ? EGL_PBUFFER_BIT : 0 ),
        EGL_BUFFER_SIZE, 32,
        EGL_DEPTH_SIZE, 16,
        EGL_SAMPLE_BUFFERS, 1,
        EGL_SAMPLES, 4,
        EGL_NONE 
        };

    return Configure( config2, config1 );
    }

EXPORT_C TInt CEglDevice::Configure( TInt aDepthBufferSize, TInt aDepthSize, 
                                     TInt aMultisampleBuffersCount, TInt aSamplesPerPixel,
                                     TInt aStencilBufferSize, TBool aWindow, TBool aPixmap, TBool aPBuffer,
                                     TBool aBindToTexture, TBool aBindToTextureWithAlpha )
    {
    const EGLint config[] = 
        { 
        EGL_SURFACE_TYPE, ( aWindow ? EGL_WINDOW_BIT : 0 ) | 
                          ( aPixmap ? EGL_PIXMAP_BIT : 0 ) | 
                          ( aPBuffer ? EGL_PBUFFER_BIT : 0 ),
        EGL_BUFFER_SIZE, aDepthBufferSize,
        EGL_DEPTH_SIZE, aDepthSize,
        EGL_SAMPLE_BUFFERS, aMultisampleBuffersCount,
        EGL_SAMPLES, aSamplesPerPixel,
        EGL_STENCIL_SIZE, aStencilBufferSize,
        EGL_BIND_TO_TEXTURE_RGB, ( aBindToTexture ? EGL_TRUE : EGL_FALSE ),
        EGL_BIND_TO_TEXTURE_RGBA, ( aBindToTextureWithAlpha ? EGL_TRUE : EGL_FALSE ),
        EGL_NONE 
        };
    
    return Configure( config );
    }

EXPORT_C TInt CEglDevice::Configure( const EGLint *aConfig, const EGLint *aAlternativeConfig )
    {
    EGLConfig *configs = NULL;
    EGLint config_size = 0;
    EGLint config_num = 0;

    if ( eglGetConfigs( iDisplay, configs, config_size, &config_num ) == EGL_FALSE )
        {
        EGLint error = eglGetError();
        if ( error == EGL_NOT_INITIALIZED )
            {
            return KErrNotReady;
            }
        else if ( error == EGL_BAD_PARAMETER )
            {
            return KErrArgument;
            }
        return KErrGeneral;
        }
    
    config_size = config_num;
    configs = (EGLConfig*) User::Alloc( sizeof( EGLConfig ) * config_size );
    if ( !configs )
        {
        return KErrNoMemory;
        }
    
    if ( eglChooseConfig( iDisplay, aConfig, configs, config_size, &config_num ) == EGL_FALSE )
        {
        User::Free( configs );
        EGLint error = eglGetError();
        if ( error == EGL_BAD_ATTRIBUTE )
            {
            return KErrArgument;
            }
        return KErrGeneral;
        }
    
    if ( config_num == 0 && aAlternativeConfig )
        {
        if ( eglChooseConfig( iDisplay, aAlternativeConfig, configs, config_size, &config_num ) == EGL_FALSE )
            {
            User::Free( configs );
            EGLint error = eglGetError();
            if ( error == EGL_BAD_ATTRIBUTE )
                {
                return KErrArgument;
                }
            return KErrGeneral;
            }
        }

    if ( config_num == 0 )
        {
        User::Free( configs );
        return KErrNotSupported;
        }
    
    iConfig = configs[ 0 ];
    User::Free( configs );
    
    return KErrNone;
    }


