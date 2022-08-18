/*
 ============================================================================
 Name		: GlTexture.h
 Author	  : Michal Strug
 Version	 : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : TGlTexture declaration
 ============================================================================
 */

#ifndef GLTEXTURE_H
#define GLTEXTURE_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <gles/gl.h>
#include <FBS.H> 

// CLASS DECLARATION

class MGlTexture
    {
public:
    
    enum TGlTextureType
        {
        EGlTextureTypeTexture,
        EGlTextureTypeTextureAtlas,
        EGlTextureTypePbuffer
        };
    
    virtual TGlTextureType TextureType() const = 0;
    
    virtual void Use( GLenum aTextureUnitNumber = GL_TEXTURE0 ) = 0;
    
    virtual void Discard() = 0;

    virtual TSize Size() const = 0;
    
    };


/**
 *  TGlTexture
 * 
 */
class TGlTexture : public MGlTexture
    {

    GLuint iTextureId;

public:
    
    GLint iTextureWrapS;    // GL_REPEAT, GL_CLAMP_TO_EDGE
    GLint iTextureWrapT;    // GL_REPEAT, GL_CLAMP_TO_EDGE
    GLint iTextureMinification; // GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST
                                // GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR
    GLint iTextureMagnification; // GL_NEAREST, GL_LINEAR
    GLint iGenerateMipmap;  // GL_TRUE, GL_FALSE

private:
    
    GLenum iFormat;
    
    TSize iSize;

    TBool iGrayAsAlpha;
    
public:
    
    TGlTexture();
    
    TInt CreateTexture( CFbsBitmap *aBitmap );
    
    TInt CreateTexture( TSize aSize, TDisplayMode aFormat, GLvoid *aData );

    TInt CreateCompressedTexture( GLsizei aWidth, GLsizei aHeight, GLint aFormat, GLsizei aDataSize, GLvoid *aData );

    TInt CreateMipmap( CFbsBitmap *aBitmap, TInt aLevel );

    TInt CreateMipmap( TSize aSize, TDisplayMode aFormat, GLvoid *aData, TInt aLevel );

    void DeleteTexture();

    void Use( TRect aTextureAtlasRect, GLenum aTextureUnitNumber = GL_TEXTURE0 );

    GLuint TextureId() const;
    
    TSize Size() const;
    
    GLenum Format() const;
    
    TBool IsValid() const;

    void SetGrayAsAlpha( TBool aValue );

public: // from MGlTexture
    
    /*
     * @param aTextureUnitNumber GL_TEXTURE0, GL_TEXTURE1, ...
     */
    void Use( GLenum aTextureUnitNumber = GL_TEXTURE0 );
    
    void Discard();

    MGlTexture::TGlTextureType TextureType() const;
    
private:
    
    TBool IsPowerOf2( TInt aVal ) const;

    void SetParameters();
    
    GLint GetDisplayMode( TDisplayMode aDisplayMode ) const;
    
    };

#endif // GLTEXTURE_H
