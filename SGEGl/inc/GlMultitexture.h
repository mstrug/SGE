/*
 ============================================================================
 Name		: GlMultitexture.h
 Author	  : Michal Strug
 Version	 : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : TGlMultitexture declaration
 ============================================================================
 */

#ifndef GLMULTITEXTURE_H
#define GLMULTITEXTURE_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <gles/gl.h>

// FORWARD DECLARATIONS

class TGlTexture;

// CLASS DECLARATION

const TInt KGlMaxTextureUnits = 2;

/**
 *  TGlMultitexture
 * 
 */
typedef TFixedArray< TGlTexture*, KGlMaxTextureUnits > TGlMultitexture;

#endif // GLMULTITEXTURE_H
