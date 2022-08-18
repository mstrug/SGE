/*
 ============================================================================
 Name		: GlRttPBuffer.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CGlRttPbuffer declaration (Render to texture using pbuffer)
 ============================================================================
 */

#ifndef GLRTTPBUFFER_H
#define GLRTTPBUFFER_H

// INCLUDES
#include <e32std.h>
#include <SGEGl/GlTexture.h>

// FORWARD DECLARATIONS

// CLASS DECLARATION
class CEglSurfacePbuffer;
class CEglRc;
class CEglSurface;
class CEglDevice;
class CGlObjectRect;

/**
 *  CGlRttPbuffer
 * 
 */
class CGlRttPbuffer : public CBase, public MGlTexture
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CGlRttPbuffer();

    /**
     * Two-phased constructor.
     */
    static CGlRttPbuffer* NewL( CEglDevice& aDevice, CEglRc& aWindowRc, CEglSurface& aWindowSurface, TSize aSize, TBool aUseAlpha = ETrue );

    /**
     * Two-phased constructor.
     */
    static CGlRttPbuffer* NewLC( CEglDevice& aDevice, CEglRc& aWindowRc, CEglSurface& aWindowSurface, TSize aSize, TBool aUseAlpha = ETrue );

    TInt MakeCurrentPbufferContext();

    TInt MakeCurrentDefaultContext();

    void Draw();

    TSize Size() const;

public: // from MGlTexture

    virtual TGlTextureType TextureType() const;
    
    virtual void Use( GLenum aTextureUnitNumber = GL_TEXTURE0 );
    
    virtual void Discard();

private:

    /**
     * Constructor for performing 1st stage construction
     */
    CGlRttPbuffer( CEglDevice& aDevice, CEglRc& aWindowRc, CEglSurface& aWindowSurface );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL( TSize aSize, TBool aUseAlpha );
   
private:

    CEglSurfacePbuffer* iPbuffer; // owned

    CGlObjectRect* iRect; // owned

    TGlTexture iTexture; // owned

    CEglDevice& iDevice; // not owned

    CEglRc& iWindowRc; // not owned

    CEglSurface& iWindowSurface; // not owned
    
    };

#endif // GLRTTPBUFFER_H
