/*
 ============================================================================
 Name		: EglSurfacePixmap.h
 Author	  : Michal Strug
 Version	 : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEglSurfacePixmap declaration
 ============================================================================
 */

#ifndef EGLSURFACEPIXMAP_H
#define EGLSURFACEPIXMAP_H

// INCLUDES
#include <W32STD.H> 
#include <SGEEgl/EglSurface.h>

// CLASS DECLARATION

class CEglDevice;

/**
 *  CEglSurfacePixmap
 * 
 */
NONSHARABLE_CLASS( CEglSurfacePixmap ) : public CEglSurface
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    IMPORT_C ~CEglSurfacePixmap();

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CEglSurfacePixmap* NewL( const CEglDevice &aDevice, CWsBitmap *aBitmap );

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CEglSurfacePixmap* NewLC( const CEglDevice &aDevice, CWsBitmap *aBitmap );

private:

    /**
     * Constructor for performing 1st stage construction
     */
    CEglSurfacePixmap( const CEglDevice &aDevice );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL( CWsBitmap *aBitmap );

    };

#endif // EGLSURFACEPIXMAP_H
