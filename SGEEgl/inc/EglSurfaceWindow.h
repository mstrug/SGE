/*
 ============================================================================
 Name		: EglSurfaceWindow.h
 Author	  : Michal Strug
 Version	 : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEglSurfaceWindow declaration
 ============================================================================
 */

#ifndef EGLSURFACEWINDOW_H
#define EGLSURFACEWINDOW_H

// INCLUDES
#include <W32STD.H> 
#include <SGEEgl/EglSurface.h>

// CLASS DECLARATION

class CEglDevice;

/**
 *  CEglSurfaceWindow
 * 
 */
NONSHARABLE_CLASS( CEglSurfaceWindow ) : public CEglSurface
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    IMPORT_C ~CEglSurfaceWindow();

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CEglSurfaceWindow* NewL( const CEglDevice &aDevice, RWindow &aWindow );

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CEglSurfaceWindow* NewLC( const CEglDevice &aDevice, RWindow &aWindow );

    IMPORT_C TInt SwapBuffers();
    
private:

    /**
     * Constructor for performing 1st stage construction
     */
    CEglSurfaceWindow( const CEglDevice &aDevice );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL( RWindow &aWindow );

    };

#endif // EGLSURFACEWINDOW_H
