/*
 ============================================================================
 Name		: EglSurfacePbuffer.cpp
 Author	  : Michal Strug
 Version	 : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEglSurfacePbuffer implementation
 ============================================================================
 */

#include "EglSurfacePbuffer.h"
#include "EglDevice.h"
#include <e32math.h>
	

CEglSurfacePbuffer::CEglSurfacePbuffer( const CEglDevice &aDevice ) : CEglSurface( aDevice )
    {
    }

EXPORT_C CEglSurfacePbuffer::~CEglSurfacePbuffer()
    {
    if ( iContext )
        {
        eglDestroyContext( iDevice.Display(), iContext );
        }
    }

EXPORT_C CEglSurfacePbuffer* CEglSurfacePbuffer::NewLC( const CEglDevice &aDevice, TSize aSize, 
                                                        TBool aTexture, TBool aAlpha, TBool aMipmap, 
                                                        TInt aMipmapLevel, EGLContext aShareRC )
    {
    CEglSurfacePbuffer *self = new (ELeave) CEglSurfacePbuffer( aDevice );
    CleanupStack::PushL( self );
    self->ConstructL( aSize, aTexture, aAlpha, aMipmap, aMipmapLevel, aShareRC );
    return self;
    }

EXPORT_C CEglSurfacePbuffer* CEglSurfacePbuffer::NewL( const CEglDevice &aDevice, TSize aSize, 
                                                       TBool aTexture, TBool aAlpha, TBool aMipmap, 
                                                       TInt aMipmapLevel, EGLContext aShareRC )
    {
    CEglSurfacePbuffer *self = CEglSurfacePbuffer::NewLC( aDevice, aSize, aTexture, aAlpha, aMipmap, aMipmapLevel, aShareRC );
    CleanupStack::Pop(); // self;
    return self;
    }

void CEglSurfacePbuffer::ConstructL( TSize aSize, TBool aTexture, TBool aAlpha, 
                                     TBool aMipmap, TInt aMipmapLevel, EGLContext aShareRC )
    {
    BaseConstructL();

    if ( !IsPowerOf2( aSize.iWidth ) || !IsPowerOf2( aSize.iHeight ) )
        {
        User::Leave( KErrArgument );
        }    

    iSize = aSize;

    if ( aTexture )
        {
        ConstructRenderToTextureL( aSize, aAlpha, aMipmap, aMipmapLevel, aShareRC );
        return;
        }

    const EGLint attrib_list[] = 
        {
        EGL_WIDTH, aSize.iWidth,
        EGL_HEIGHT, aSize.iHeight,
        EGL_NONE
        };

    iSurface = eglCreatePbufferSurface( iDevice.Display(), iDevice.Config(), attrib_list );

    if ( iSurface == EGL_NO_SURFACE )
        {
        EGLint error = eglGetError();
        if ( error == EGL_BAD_MATCH || error == EGL_BAD_CONFIG || error == EGL_BAD_NATIVE_PIXMAP )
            {
            User::Leave( KErrArgument );
            }
        else if ( error == EGL_BAD_ALLOC )
            {
            User::Leave( KErrNoMemory );
            }
        else if ( error == EGL_BAD_ATTRIBUTE )
            {
            User::Leave( KErrNotSupported );
            }
        User::Leave( KErrGeneral );
        }
    }

void CEglSurfacePbuffer::ConstructRenderToTextureL( TSize aSize, TBool aAlpha, TBool aMipmap, TInt aMipmapLevel, EGLContext aShareRC )
    {
    EGLint depthSize = 16;
    EGLint bufferSize = 32;
    EGLint samples = 0;
    EGLint sampleBuffers = 0;
    if ( aShareRC )
        {
        EGLint configId;
        if ( eglQueryContext( iDevice.Display(), aShareRC, EGL_CONFIG_ID, &configId ) == EGL_TRUE )
            {
            eglGetConfigAttrib( iDevice.Display(), configId, EGL_DEPTH_SIZE, &depthSize );
            eglGetConfigAttrib( iDevice.Display(), configId, EGL_BUFFER_SIZE, &bufferSize );
            eglGetConfigAttrib( iDevice.Display(), configId, EGL_SAMPLES, &samples );
            eglGetConfigAttrib( iDevice.Display(), configId, EGL_SAMPLE_BUFFERS, &sampleBuffers );
            }
        }

    const EGLint config[] = 
        { 
        EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
        EGL_BUFFER_SIZE, bufferSize,
        EGL_DEPTH_SIZE, depthSize,
        EGL_SAMPLES, samples,
        EGL_SAMPLE_BUFFERS, sampleBuffers,
        ( aAlpha ? EGL_BIND_TO_TEXTURE_RGBA : EGL_BIND_TO_TEXTURE_RGB ), EGL_TRUE,
        EGL_NONE 
        };

    EGLConfig *configs = NULL;
    EGLint config_size = 0;
    EGLint config_num = 0;

    if ( eglGetConfigs( iDevice.Display(), configs, config_size, &config_num ) == EGL_FALSE )
        {
        EGLint error = eglGetError();
        if ( error == EGL_NOT_INITIALIZED )
            {
            User::Leave( KErrNotReady );
            }
        else if ( error == EGL_BAD_PARAMETER )
            {
            User::Leave( KErrArgument );
            }
        User::Leave( KErrGeneral );
        }

    config_size = config_num;
    configs = (EGLConfig*) User::Alloc( sizeof( EGLConfig ) * config_size );
    if ( !configs )
        {
        User::Leave( KErrNoMemory );
        }

    if ( eglChooseConfig( iDevice.Display(), config, configs, config_size, &config_num ) == EGL_FALSE )
        {
        User::Free( configs );
        EGLint error = eglGetError();
        if ( error == EGL_BAD_ATTRIBUTE )
            {
            User::Leave( KErrArgument );
            }
        User::Leave( KErrGeneral );
        }
    
    if ( config_num == 0 )
        {
        User::Free( configs );
        User::Leave( KErrNotSupported );
        }

    TInt configVal = configs[ 0 ];
    User::Free( configs );

    const EGLint attrib_list0[] = { EGL_NONE };

    iContext = eglCreateContext( iDevice.Display(), configVal, ( aShareRC ? aShareRC : EGL_NO_CONTEXT ), attrib_list0 );
    if ( iContext == EGL_NO_CONTEXT )
        {
        EGLint error = eglGetError();
        if ( error == EGL_BAD_CONTEXT || error == EGL_BAD_CONFIG || error == EGL_BAD_MATCH )
            {
            User::Leave( KErrArgument );
            }
        else if ( error == EGL_BAD_ALLOC )
            {
            User::Leave( KErrNoMemory );
            }
        User::Leave( KErrGeneral );
        }

    const EGLint attrib_list[] = 
        {
        EGL_WIDTH, aSize.iWidth,
        EGL_HEIGHT, aSize.iHeight,
        EGL_TEXTURE_TARGET, EGL_TEXTURE_2D,
        EGL_TEXTURE_FORMAT, ( aAlpha ? EGL_TEXTURE_RGBA : EGL_TEXTURE_RGBA ),
        EGL_MIPMAP_TEXTURE, ( aMipmap ? EGL_TRUE : EGL_FALSE ),
        ( aMipmap ? EGL_MIPMAP_LEVEL, aMipmapLevel, EGL_NONE : EGL_NONE )
        };

    iSurface = eglCreatePbufferSurface( iDevice.Display(), configVal, attrib_list );
    if ( iSurface == EGL_NO_SURFACE )
        {
        EGLint error = eglGetError();
        if ( error == EGL_BAD_MATCH || error == EGL_BAD_CONFIG || error == EGL_BAD_NATIVE_PIXMAP )
            {
            User::Leave( KErrArgument );
            }
        else if ( error == EGL_BAD_ALLOC )
            {
            User::Leave( KErrNoMemory );
            }
        else if ( error == EGL_BAD_ATTRIBUTE )
            {
            User::Leave( KErrNotSupported );
            }
        User::Leave( KErrGeneral );
        }
    }

EXPORT_C TInt CEglSurfacePbuffer::CopyBuffers( CWsBitmap *aBitmap )
    {
    if ( eglCopyBuffers( iDevice.Display(), iSurface, aBitmap ) == EGL_FALSE )
        {
        return KErrGeneral;
        }
    return KErrNone;
    }

EXPORT_C TInt CEglSurfacePbuffer::BindToTexture()
    { // todo: add compilation flat to compile with opengl version 1.0 or 1.1
#ifndef EGL_VERSION_1_1
    return KErrNotSupported;
#else
    if ( eglBindTexImage( iDevice.Display(), iSurface, EGL_BACK_BUFFER ) == EGL_FALSE )
        {
        return KErrGeneral;
        }
    return KErrNone;
#endif
    }

EXPORT_C TInt CEglSurfacePbuffer::ReleaseFromTexture()
    { // todo: add compilation flat to compile with opengl version 1.0 or 1.1
#ifndef EGL_VERSION_1_1
    return KErrNotSupported;
#else
    if ( eglReleaseTexImage( iDevice.Display(), iSurface, EGL_BACK_BUFFER ) == EGL_FALSE )
        {
        return KErrGeneral;
        }
    return KErrNone;
#endif
    }

TBool CEglSurfacePbuffer::IsPowerOf2( TInt aVal ) const
    {
    const TReal log2 = 0.30102999566398;
    TReal out;
    TReal in = aVal;
    if ( Math::Log( out, in ) != KErrNone )
        {
        return EFalse;
        }
    out /= log2;
    TInt iout = (TInt) out;
    return ( ( 1 << iout ) == aVal );
    }

EXPORT_C TSize CEglSurfacePbuffer::Size() const
    {
    return iSize;
    }

EXPORT_C TInt CEglSurfacePbuffer::MakeCurrent()
    {
    if ( eglMakeCurrent( iDevice.Display(), iSurface, iSurface, iContext ) == EGL_FALSE )
        {
        EGLint error = eglGetError();
        if ( error == EGL_BAD_CONTEXT || error == EGL_BAD_SURFACE || error == EGL_BAD_MATCH )
            {
            return KErrArgument;
            }
        else if ( error == EGL_BAD_ACCESS )
            {
            return KErrInUse;
            }
        else if ( error == EGL_BAD_NATIVE_WINDOW )
            {
            return KErrBadHandle;
            }
        else if ( error == EGL_BAD_ALLOC )
            {
            return KErrNoMemory;
            }
        else if ( error == EGL_CONTEXT_LOST )
            {
            return KErrDied;
            }
        return KErrGeneral;
        }
    return KErrNone;
    }

