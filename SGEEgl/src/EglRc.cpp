/*
 ============================================================================
 Name		: EglRc.cpp
 Author	  : Michal Strug
 Version	 : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEglRc implementation
 ============================================================================
 */

#include "EglRc.h"
#include "EglDevice.h"
#include "EglSurface.h"

CEglRc::CEglRc( const CEglDevice &aDevice ) : iDevice( aDevice )
    {
    }

EXPORT_C CEglRc::~CEglRc()
    {
    eglDestroyContext( iDevice.Display(), iContext );
    }

EXPORT_C CEglRc* CEglRc::NewLC( const CEglDevice &aDevice )
    {
    CEglRc *self = new (ELeave) CEglRc( aDevice );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

EXPORT_C CEglRc* CEglRc::NewL( const CEglDevice &aDevice )
    {
    CEglRc *self = CEglRc::NewLC( aDevice );
    CleanupStack::Pop(); // self;
    return self;
    }

void CEglRc::ConstructL()
    {
    const EGLint attrib_list[] = { EGL_NONE };

    iContext = eglCreateContext( iDevice.Display(), iDevice.Config(), EGL_NO_CONTEXT, attrib_list );
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
    }

EXPORT_C TInt CEglRc::MakeCurrent( CEglSurface &aSurface )
    {
    return MakeCurrent( aSurface, aSurface );
    }

EXPORT_C TInt CEglRc::MakeCurrent( CEglSurface &aDrawSurface, CEglSurface &aReadSurface )
    {
    if ( eglMakeCurrent( iDevice.Display(), aDrawSurface.Surface(), aReadSurface.Surface(), iContext ) 
                                                                                                == EGL_FALSE )
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

EXPORT_C const EGLContext CEglRc::RC() const
    {
    return iContext;
    }

