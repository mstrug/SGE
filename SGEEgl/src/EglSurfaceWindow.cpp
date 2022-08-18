/*
 ============================================================================
 Name		: EglSurfaceWindow.cpp
 Author	  : Michal Strug
 Version	 : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEglSurfaceWindow implementation
 ============================================================================
 */

#include "EglSurfaceWindow.h"
#include "EglDevice.h"

CEglSurfaceWindow::CEglSurfaceWindow( const CEglDevice &aDevice ) : CEglSurface( aDevice )
    {
    }

EXPORT_C CEglSurfaceWindow::~CEglSurfaceWindow()
    {
    }

EXPORT_C CEglSurfaceWindow* CEglSurfaceWindow::NewLC( const CEglDevice &aDevice, RWindow &aWindow )
    {
    CEglSurfaceWindow *self = new (ELeave) CEglSurfaceWindow( aDevice );
    CleanupStack::PushL( self );
    self->ConstructL( aWindow );
    return self;
    }

EXPORT_C CEglSurfaceWindow* CEglSurfaceWindow::NewL( const CEglDevice &aDevice, RWindow &aWindow )
    {
    CEglSurfaceWindow *self = CEglSurfaceWindow::NewLC( aDevice, aWindow );
    CleanupStack::Pop(); // self;
    return self;
    }

void CEglSurfaceWindow::ConstructL( RWindow &aWindow )
    {
    BaseConstructL();
    
    const EGLint attrib_list[] = { EGL_NONE };
    
    iSurface = eglCreateWindowSurface( iDevice.Display(), iDevice.Config(), &aWindow, attrib_list );
    if ( iSurface == EGL_NO_SURFACE )
        {
        EGLint error = eglGetError();
        if ( error == EGL_BAD_MATCH || error == EGL_BAD_CONFIG || error == EGL_BAD_NATIVE_WINDOW )
            {
            User::Leave( KErrArgument );
            }
        else if ( error == EGL_BAD_ALLOC )
            {
            User::Leave( KErrInUse );
            }
        User::Leave( KErrGeneral );
        }
    }

EXPORT_C TInt CEglSurfaceWindow::SwapBuffers()
    {
    if ( eglSwapBuffers( iDevice.Display(), iSurface ) == EGL_FALSE )
        {
        return KErrGeneral;
        }
    return KErrNone;
    }

