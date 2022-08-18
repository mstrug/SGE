/*
 ============================================================================
 Name		: EglSurfacePixmap.cpp
 Author	  : Michal Strug
 Version	 : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEglSurfacePixmap implementation
 ============================================================================
 */

#include "EglSurfacePixmap.h"
#include "EglDevice.h"

CEglSurfacePixmap::CEglSurfacePixmap( const CEglDevice &aDevice ) : CEglSurface( aDevice )
    {
    }

EXPORT_C CEglSurfacePixmap::~CEglSurfacePixmap()
    {
    }

EXPORT_C CEglSurfacePixmap* CEglSurfacePixmap::NewLC( const CEglDevice &aDevice, CWsBitmap *aBitmap )
    {
    CEglSurfacePixmap *self = new (ELeave) CEglSurfacePixmap( aDevice );
    CleanupStack::PushL( self );
    self->ConstructL( aBitmap );
    return self;
    }

EXPORT_C CEglSurfacePixmap* CEglSurfacePixmap::NewL( const CEglDevice &aDevice, CWsBitmap *aBitmap )
    {
    CEglSurfacePixmap *self = CEglSurfacePixmap::NewLC( aDevice, aBitmap );
    CleanupStack::Pop(); // self;
    return self;
    }

void CEglSurfacePixmap::ConstructL( CWsBitmap *aBitmap )
    {
    BaseConstructL();
    
    const EGLint attrib_list[] = { EGL_NONE };
    
    iSurface = eglCreatePixmapSurface( iDevice.Display(), iDevice.Config(), aBitmap, attrib_list );
    if ( iSurface == EGL_NO_SURFACE )
        {
        EGLint error = eglGetError();
        if ( error == EGL_BAD_MATCH || error == EGL_BAD_CONFIG || error == EGL_BAD_NATIVE_PIXMAP )
            {
            User::Leave( KErrArgument );
            }
        else if ( error == EGL_BAD_ALLOC )
            {
            User::Leave( KErrInUse );   // KErrNoMemory
            }
        User::Leave( KErrGeneral );
        }
    }

