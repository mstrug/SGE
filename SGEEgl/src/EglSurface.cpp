/*
 ============================================================================
 Name		: EglSurface.cpp
 Author	  : Michal Strug
 Version	 : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEglSurface implementation
 ============================================================================
 */

#include "EglSurface.h"
#include "EglDevice.h"

CEglSurface::CEglSurface( const CEglDevice &aDevice ) : iDevice( aDevice )
    {
    }

EXPORT_C CEglSurface::~CEglSurface()
    {
    eglDestroySurface( iDevice.Display(), iSurface );
    }

void CEglSurface::BaseConstructL()
    {
    }

EXPORT_C EGLSurface CEglSurface::Surface() const
    {
    return iSurface;
    }
