/*
 ============================================================================
 Name		: EglSurfacePbuffer.h
 Author	  : Michal Strug
 Version	 : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEglSurfacePbuffer declaration
 ============================================================================
 */

#ifndef EGLSURFACEPBUFFER_H
#define EGLSURFACEPBUFFER_H

// INCLUDES
#include <W32STD.H> 
#include <SGEEgl/EglSurface.h>

// CLASS DECLARATION

class CEglDevice;

/**
 *  CEglSurfacePbuffer
 *  
 *  Size should be a power of 2 for most implementations.
 */
NONSHARABLE_CLASS( CEglSurfacePbuffer ) : public CEglSurface
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    IMPORT_C ~CEglSurfacePbuffer();

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CEglSurfacePbuffer* NewL( const CEglDevice &aDevice, TSize aSize,
                                              TBool aTexture = ETrue, 
                                              TBool aAlpha = ETrue, 
                                              TBool aMipmap = EFalse,
                                              TInt aMipmapLevel = 0,
                                              EGLContext aShareRC = 0 );

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CEglSurfacePbuffer* NewLC( const CEglDevice &aDevice, TSize aSize,
                                               TBool aTexture = ETrue, 
                                               TBool aAlpha = ETrue, 
                                               TBool aMipmap = EFalse,
                                               TInt aMipmapLevel = 0,
                                               EGLContext aShareRC = 0 );
    
    IMPORT_C TInt CopyBuffers( CWsBitmap *aBitmap );
    
    IMPORT_C TInt BindToTexture();

    IMPORT_C TInt ReleaseFromTexture(); 

    IMPORT_C TSize Size() const;

    // when rendering to texture use this method to make current rendering context
    IMPORT_C TInt MakeCurrent();
    
private:

    /**
     * Constructor for performing 1st stage construction
     */
    CEglSurfacePbuffer( const CEglDevice &aDevice );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL( TSize aSize, TBool aTexture, TBool aAlpha, TBool aMipmap, TInt aMipmapLevel, EGLContext aShareRC );

    // different config is needed (same display)
    void ConstructRenderToTextureL( TSize aSize, TBool aAlpha, TBool aMipmap, TInt aMipmapLevel, EGLContext aShareRC );

    TBool IsPowerOf2( TInt aVal ) const;

private:

    TSize iSize;

    // when rendering to texture different context is needed
    EGLContext iContext;

    };

#endif // EGLSURFACEPBUFFER_H
