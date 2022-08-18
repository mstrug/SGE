/*
 ============================================================================
 Name		: EglSurface.h
 Author	  : Michal Strug
 Version	 : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEglSurface declaration
 ============================================================================
 */

#ifndef EGLSURFACE_H
#define EGLSURFACE_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <gles/egl.h>

// CLASS DECLARATION

class CEglDevice;

/**
 *  CEglSurface
 * 
 */
NONSHARABLE_CLASS( CEglSurface ) : public CBase
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    IMPORT_C ~CEglSurface();

    IMPORT_C EGLSurface Surface() const;
    
protected:

    /**
     * Constructor for performing 1st stage construction
     */
    CEglSurface( const CEglDevice &aDevice );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void BaseConstructL();

protected: // data
    
    const CEglDevice& iDevice;
    
    EGLSurface iSurface;
    
    };

#endif // EGLSURFACE_H
