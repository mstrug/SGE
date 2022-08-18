/*
 ============================================================================
 Name		: GlTexture.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : TGlTexture implementation
 ============================================================================
 */

#include "GlTexture.h"
#include <e32math.h>


TGlTexture::TGlTexture() : iTextureId( 0 ), iTextureWrapS( 0 ), iTextureWrapT( 0 ), 
        iTextureMinification( GL_NEAREST ), iTextureMagnification( GL_NEAREST ), iGenerateMipmap( GL_FALSE ), 
        iFormat( 0 ), iGrayAsAlpha( EFalse )
    {
    }

TInt TGlTexture::CreateTexture( CFbsBitmap *aBitmap )
    {
    GLint internalFormat;
    GLsizei width = aBitmap->SizeInPixels().iWidth;
    GLsizei height = aBitmap->SizeInPixels().iHeight;
    GLenum type = GL_UNSIGNED_BYTE;

    if ( !IsPowerOf2( width ) || !IsPowerOf2( height ) )
        {
        return KErrArgument;
        }
    // todo: check size with GL_MAX_TEXTURE_SIZE

    internalFormat = GetDisplayMode( aBitmap->DisplayMode() );
    if ( internalFormat < KErrNone )
        {
        return internalFormat;
        }
    iFormat = internalFormat;

    iSize = TSize( width, height );

    DeleteTexture();

    glGenTextures( 1, &iTextureId );
    
    glBindTexture( GL_TEXTURE_2D, iTextureId );

    if ( iGenerateMipmap )
        {
        glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, iGenerateMipmap );
        }

    aBitmap->LockHeap();
    const GLvoid *pixels = (GLvoid*)aBitmap->DataAddress();

    glTexImage2D( GL_TEXTURE_2D, 0, internalFormat, width, height, 0, iFormat, type, pixels );

    aBitmap->UnlockHeap();

    GLenum err = glGetError();
    if ( err != GL_NO_ERROR )
    {
    	return err;
    }
    
    //SetParameters();
    
    Discard();
	glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE );

    return KErrNone;
    }

TInt TGlTexture::CreateTexture( TSize aSize, TDisplayMode aFormat, GLvoid *aData )
    {
    GLint internalFormat;
    GLsizei width = aSize.iWidth;
    GLsizei height = aSize.iHeight;
    GLenum type = GL_UNSIGNED_BYTE;

    if ( !IsPowerOf2( width ) || !IsPowerOf2( height ) )
        {
        return KErrArgument;
        }

    internalFormat = GetDisplayMode( aFormat );
    if ( internalFormat < KErrNone )
        {
        return internalFormat;
        }
    iFormat = internalFormat;

    iSize = aSize;

    DeleteTexture();

    glGenTextures( 1, &iTextureId );
    
    glBindTexture( GL_TEXTURE_2D, iTextureId );

    if ( iGenerateMipmap )
        {
        glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, iGenerateMipmap );
        }

    glTexImage2D( GL_TEXTURE_2D, 0, internalFormat, width, height, 0, iFormat, type, aData );
    
    GLenum err = glGetError();
    if ( err != GL_NO_ERROR )
    {
    	return err;
    }

    //SetParameters();
    
    Discard();
	glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE );

    return KErrNone;
    }

TInt TGlTexture::CreateCompressedTexture( GLsizei aWidth, GLsizei aHeight, GLint aFormat, GLsizei aDataSize, GLvoid *aData )
    {
    if ( !IsPowerOf2( aWidth ) || !IsPowerOf2( aHeight ) )
        {
        return KErrArgument;
        }

    iFormat = aFormat;

    iSize = TSize( aWidth, aHeight );

    DeleteTexture();

    glGenTextures( 1, &iTextureId );
    
    glBindTexture( GL_TEXTURE_2D, iTextureId );

    if ( iGenerateMipmap )
        {
        glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, iGenerateMipmap );
        }

    glCompressedTexImage2D( GL_TEXTURE_2D, 0, aFormat, aWidth, aHeight, 0, aDataSize, aData );
    
    GLenum err = glGetError();
    if ( err != GL_NO_ERROR )
    {
    	return err;
    }

    //SetParameters();
    
    Discard();
    glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE );

    return KErrNone;
    }

TInt TGlTexture::CreateMipmap( CFbsBitmap *aBitmap, TInt aLevel )
    {
    GLint internalFormat;
    GLsizei width = aBitmap->SizeInPixels().iWidth;
    GLsizei height = aBitmap->SizeInPixels().iHeight;
    GLenum format;
    GLenum type = GL_UNSIGNED_BYTE;

    if ( !iTextureId )
        {
        return KErrNotReady;
        }
    
    if ( !IsPowerOf2( width ) || !IsPowerOf2( height ) ||
         width >= iSize.iWidth || height >= iSize.iHeight ||
         aLevel <= 0 )
        {
        return KErrArgument;
        }
    
    internalFormat = GetDisplayMode( aBitmap->DisplayMode() );
    if ( internalFormat < KErrNone )
        {
        return internalFormat;
        }
    format = internalFormat;

    if ( format != iFormat )
        {
        return KErrArgument;
        }
    
    glBindTexture( GL_TEXTURE_2D, iTextureId );

    aBitmap->LockHeap();
    const GLvoid *pixels = (GLvoid*)aBitmap->DataAddress();

    glTexImage2D( GL_TEXTURE_2D, aLevel, internalFormat, width, height, 0, format, type, pixels );

    aBitmap->UnlockHeap();
    
    GLenum err = glGetError();
    if ( err != GL_NO_ERROR )
    {
    	return err;
    }

    //SetParameters();
    
    Discard();

    return KErrNone;
    }

TInt TGlTexture::CreateMipmap( TSize aSize, TDisplayMode aFormat, GLvoid *aData, TInt aLevel )
    {
    GLint internalFormat;
    GLsizei width = aSize.iWidth;
    GLsizei height = aSize.iHeight;
    GLenum format;
    GLenum type = GL_UNSIGNED_BYTE;

    if ( !iTextureId )
        {
        return KErrNotReady;
        }
    
    if ( !IsPowerOf2( width ) || !IsPowerOf2( height ) ||
         width >= iSize.iWidth || height >= iSize.iHeight ||
         aLevel <= 0 )
        {
        return KErrArgument;
        }
    
    internalFormat = GetDisplayMode( aFormat );
    if ( internalFormat < KErrNone )
        {
        return internalFormat;
        }
    format = internalFormat;

    if ( format != iFormat )
        {
        return KErrArgument;
        }
    
    glBindTexture( GL_TEXTURE_2D, iTextureId );

    glTexImage2D( GL_TEXTURE_2D, aLevel, internalFormat, width, height, 0, format, type, (GLvoid*) aData );
    
    GLenum err = glGetError();
    if ( err != GL_NO_ERROR )
    {
        return err;
    }

    //SetParameters();
    
    Discard();

    return KErrNone;
    }

void TGlTexture::DeleteTexture()
    {
    glDeleteTextures( 1, &iTextureId );
    iTextureId = 0;
    }

void TGlTexture::Use( GLenum aTextureUnitNumber )
    {
    glActiveTexture( aTextureUnitNumber );              // @todo: add API to disable this call
    glClientActiveTexture( aTextureUnitNumber );        // @todo: add API to disable this call
    glBindTexture( GL_TEXTURE_2D, iTextureId );
    SetParameters();
    }

void TGlTexture::Use( TRect aTextureAtlasRect, GLenum aTextureUnitNumber )
    {
    Use( aTextureUnitNumber );
    
    //GLfixed w = aTextureAtlasRect.Width();
    //GLfixed h = aTextureAtlasRect.Height();
    //GLfixed x = aTextureAtlasRect.iTl.iX;

    glMatrixMode( GL_TEXTURE );

    glLoadIdentity();
    //glTranslatex( ( aTextureAtlasRect.iTl.iX << 16 ) / iSize.iWidth, ( aTextureAtlasRect.iTl.iY << 16 ) / iSize.iHeight, 0 );
    //glScalex( ( w << 16 ) / iSize.iWidth, ( h << 16 ) / iSize.iHeight, 1 << 16 );

    glTranslatef( (GLfloat)aTextureAtlasRect.iTl.iX / (GLfloat)iSize.iWidth, (GLfloat)aTextureAtlasRect.iTl.iY / (GLfloat)iSize.iHeight, 0 );
    glScalef( (GLfloat)aTextureAtlasRect.Width() / (GLfloat)iSize.iWidth, (GLfloat)aTextureAtlasRect.Height() / (GLfloat)iSize.iHeight, 1 );

    glMatrixMode( GL_MODELVIEW );
    }

void TGlTexture::Discard()
    {
    glBindTexture( GL_TEXTURE_2D, NULL );

    glMatrixMode( GL_TEXTURE );
    glLoadIdentity();
    glMatrixMode( GL_MODELVIEW );
    }

GLuint TGlTexture::TextureId() const
    {
    return iTextureId;
    }

TSize TGlTexture::Size() const
    {
    return iSize;
    }

GLenum TGlTexture::Format() const
    {
    return iFormat;
    }

void TGlTexture::SetGrayAsAlpha( TBool aValue )
    {
    iGrayAsAlpha = aValue;
    }

TBool TGlTexture::IsValid() const
    {
    if ( iSize.iHeight == 0 || iSize.iWidth == 0 )
        {
        return EFalse;
        }
    if ( iTextureId == 0 )
        {
        return EFalse;
        }
    if ( !IsPowerOf2( iSize.iHeight ) || !IsPowerOf2( iSize.iWidth ) )
        {
        return EFalse;
        }
    
    return ETrue;
    }

MGlTexture::TGlTextureType TGlTexture::TextureType() const
    {
    return MGlTexture::EGlTextureTypeTexture;
    }

TBool TGlTexture::IsPowerOf2( TInt aVal ) const
    {
    const TReal log2 = 0.30102999566398;
    TReal out;
    TReal in = aVal;
    if ( Math::Log( out, in ) != KErrNone )
        {
        return EFalse;
        }
    out /= log2;
    TInt iout = (TInt) out;
    return ( ( 1 << iout ) == aVal );
    }

void TGlTexture::SetParameters()
    {
    if ( iTextureWrapS )
        {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, iTextureWrapS );
        }
    if ( iTextureWrapT )
        {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, iTextureWrapT );
        }
    if ( iTextureMinification )
        {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, iTextureMinification );
        // todo: GL_GENERATE_MIPMAP generate mipmap must be GL_TRUE to use filter with MIPMAP?
        }
    if ( iTextureMagnification )
        {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, iTextureMagnification );
        }
    }

GLint TGlTexture::GetDisplayMode( TDisplayMode aDisplayMode ) const
    {
    switch( aDisplayMode )
        {
        case EGray256:
		if ( iGrayAsAlpha )
			{
	                return GL_ALPHA;
			}
		else
			{
			return GL_LUMINANCE;
			}
	case EColor64K:
		return GL_LUMINANCE_ALPHA;
        case EColor16M:
                return GL_RGB;
        case EColor16MA:
                return GL_RGBA;
        default:
                return KErrNotSupported;
        }
    }



