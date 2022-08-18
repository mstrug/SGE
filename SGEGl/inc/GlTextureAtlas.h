/*
 ============================================================================
 Name		: GlTextureAtlas.h
 Author	  : Michal Strug
 Version	 : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : TGlTextureAtlas declaration
 ============================================================================
 */

#ifndef GLTEXTUREATLAS_H
#define GLTEXTUREATLAS_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <gles/gl.h>
#include <SGEGl/GlTexture.h>

// CLASS DECLARATION


/**
 *  TGlTextureAtlas
 * 
 */
class TGlTextureAtlas : public MGlTexture
    {

    TUint iTextureId;

    TGlTexture *iTexture;
    
    TRect iRect;
    
public:
    
    TGlTextureAtlas( TUint iTextureId, TRect aRect );

    TUint TextureId() const;

    TGlTexture* Texture() const;

    void SetTexture( TGlTexture* aTexture );

    TSize Size() const;

public: // from MGlTexture    
    
    /*
     * @param aTextureUnitNumber GL_TEXTURE0, GL_TEXTURE1, ...
     */
    void Use( GLenum aTextureUnitNumber = GL_TEXTURE0 );

    void Discard();

    MGlTexture::TGlTextureType TextureType() const;
    
    };

#endif // GLTEXTUREATLAS_H
