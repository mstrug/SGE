/*
 ============================================================================
 Name		: GlPrimitiveType.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : TGlPrimitiveType declaration
 ============================================================================
 */

#ifndef GLPRIMITIVETYPE_H
#define GLPRIMITIVETYPE_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <gles/gl.h>


// CLASS DECLARATION

/**
 *  TGlPrimitiveType
 * 
 */
class TGlPrimitiveType
	{
public:
	
	virtual void Use() = 0;
	
	virtual GLenum Type() const = 0;
	
protected:

	TGlPrimitiveType();

	};

#endif // GLPRIMITIVETYPE_H
