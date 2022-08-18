/*
 ============================================================================
 Name		: GlTextureFileCustom.h
 Author	  : Michal Strug
 Version	 : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : GlTextureFileCustom declaration
 ============================================================================
 */

#ifndef GLTEXTUREFILECUSTOM_H
#define GLTEXTUREFILECUSTOM_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <gles/gl.h>

// CLASS DECLARATION


/**
 *  GlTextureFileCustom
 * 
 */
class GlTextureFileCustom
    {

public:

    struct STextureCustom
	{
	GLsizei iWidth;
	GLsizei iHeight;
	GLint iFormat;
	GLsizei iDataSize;
	GLvoid* iData; // not owned
	};

    static TBool LoadKTX( RBuf8& aData, STextureCustom& aTexture );

    static TBool LoadPVR( RBuf8& aData, STextureCustom& aTexture );

    };

#endif // GLTEXTUREFILECUSTOM_H
