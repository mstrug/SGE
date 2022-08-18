/*
 ============================================================================
 Name		: GlLine.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : TGlLine declaration
 ============================================================================
 */

#ifndef GLLINE_H
#define GLLINE_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <gles/gl.h>
#include <SGEGl/GlPrimitiveType.h>

// CLASS DECLARATION

/**
 *  TGlLine
 * 
 */
template <class T> 
class TGlLine : public TGlPrimitiveType
    {
public:

    /**
     * Creates TGlLine with width set to 1.0 and enabled line smooth
     */
    TGlLine( GLenum aType );

    /**
     * Creates TGlLine with enabled line smooth
     */
    TGlLine( T aWidth, GLenum aType );    

    TGlLine( T aWidth, TBool aAntialiased, GLenum aType );    
    
    GLenum Type() const;

    TBool SetType( GLenum aType );

    TBool SetWidth( T aWidth );
    
    void SetAntialiased( TBool aAntialiased );
    
protected:
    
    T iWidth;
    
    GLenum iType;
    
    TBool iLineSmooth;
    
    };


/**
 *  TGlLineX
 * 
 */
class TGlLineX : public TGlLine< GLfixed >
    {
public:
    
    TGlLineX( GLenum aType = GL_LINES );

    TGlLineX( GLfixed aWidth, GLenum aType = GL_LINES );    

    TGlLineX( GLfixed aWidth, TBool aAntialiased, GLenum aType = GL_LINES );
    
    void Use();
    };


/**
 *  TGlLineF
 * 
 */
class TGlLineF : public TGlLine< GLfloat >
    {
public:
    
    TGlLineF( GLenum aType = GL_LINES );

    TGlLineF( GLfloat aWidth, GLenum aType = GL_LINES );    

    TGlLineF( GLfloat aWidth, TBool aAntialiased, GLenum aType = GL_LINES );
    
    void Use();
    };



#include <SGEGl/GlLine.inl>

#endif // GLLINE_H
