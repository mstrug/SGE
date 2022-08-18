/*
 ============================================================================
 Name		: GlPolygon.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : TGlPolygon declaration
 ============================================================================
 */

#ifndef GLPOLYGON_H
#define GLPOLYGON_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <gles/gl.h>
#include <SGEGl/GlPrimitiveType.h>

// CLASS DECLARATION

/**
 *  TGlPolygon
 * 
 */
template <class T> 
class TGlPolygon : public TGlPrimitiveType
    {
public:

    /**
     * Creates TGlPolygon with turned on culling of back face
     */
    TGlPolygon( GLenum aType );

    TGlPolygon( GLenum aCullFaceType, GLenum aType );    
    
    GLenum Type() const;

    TBool SetType( GLenum aType );
    
    void SetCullFace( TBool aEnabled );

    TBool SetCullFaceType( GLenum aCullFaceType );

    void CullFaceFront();
    
    void CullFaceBack();
    
    void CullFaceFrontAndBack();
    
    void SetOffsetFill( TBool aEnabled );
    
    void SetOffsetFillParam( T aFactor, T aUnits );    
    
protected:
    
    GLenum iType;
    
    TBool iCullFaceEnabled;

    GLenum iCullFaceType;    // GL_FRONT, GL_BACK, GL_FRONT_AND_BACK
    
    TBool iOffsetFillEnabled;
    
    T iOffsetFactor;
    
    T iOffsetUnits;
    
    };



/**
 *  TGlPolygonX
 * 
 */
class TGlPolygonX : public TGlPolygon< GLfixed >
    {
public:
    TGlPolygonX( GLenum aType = GL_TRIANGLES );

    TGlPolygonX( GLenum aCullFaceType, GLenum aType = GL_TRIANGLES );  

    void Use();
    };


/**
 *  TGlPolygonF
 * 
 */
class TGlPolygonF : public TGlPolygon< GLfloat >
    {
public:
    TGlPolygonF( GLenum aType = GL_TRIANGLES );

    TGlPolygonF( GLenum aCullFaceType, GLenum aType = GL_TRIANGLES );  

    void Use();
    };


#include <SGEGl/GlPolygon.inl>

#endif // GLPOLYGON_H
