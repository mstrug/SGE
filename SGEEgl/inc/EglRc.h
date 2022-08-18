/*
 ============================================================================
 Name		: EglRc.h
 Author	  : Michal Strug
 Version	 : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEglRc declaration
 ============================================================================
 */

#ifndef EGLRC_H
#define EGLRC_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <gles/egl.h>

// CLASS DECLARATION

class CEglDevice;
class CEglSurface;

/**
 *  CEglRc
 * 
 * EGL Rendering Context
 * 3.7
 * 
 */
NONSHARABLE_CLASS(  CEglRc ) : public CBase
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    IMPORT_C ~CEglRc();

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CEglRc* NewL( const CEglDevice &aDevice );

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CEglRc* NewLC( const CEglDevice &aDevice );

    IMPORT_C TInt MakeCurrent( CEglSurface &aSurface );

    IMPORT_C TInt MakeCurrent( CEglSurface &aDrawSurface, CEglSurface &aReadSurface );

    IMPORT_C const EGLContext RC() const;
    
private:

    /**
     * Constructor for performing 1st stage construction
     */
    CEglRc( const CEglDevice &aDevice );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();

private: // data
    
    const CEglDevice &iDevice;
    
    EGLContext iContext;
    
    };

#endif // EGLRC_H
