/*
 ============================================================================
 Name		: GlPoint.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : TGlPoint declaration
 ============================================================================
 */

#ifndef GLPOINT_H
#define GLPOINT_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <gles/gl.h>
#include <SGEGl/GlPrimitiveType.h>

// CLASS DECLARATION

/**
 *  TGlPoint
 * 
 */
template <class T> 
class TGlPoint : public TGlPrimitiveType
    {
public:

    /**
     * Creates TGlPoint with size set to 1.0 and enabled point smooth
     */
    TGlPoint();

    /**
     * Creates TGlPoint with enabled point smooth
     */
    TGlPoint( T aSize );    

    TGlPoint( T aSize, TBool aAntialiased );    
    
    GLenum Type() const;

    TBool SetSize( T aSize );
    
    void SetAntialiased( TBool aAntialiased );

    void SetPointSprites( TBool aPointSprites );

    void SetCoordReplace( TBool aReplace );

    void SetPointSizeMin( T aValue );

    void SetPointSizeMax( T aValue );
    
    void SetDistanceAttenuation( T aValue0, T aValue1, T aValue2 );
    
    void SetPointFadeThresholdSize( T aValue );
    
protected:
    
    T iSize;
    
    TBool iPointSmooth;
    
    TBool iPointSprites;
    
    TBool iCoordReplace;
    
    T iPointSizeMin;

    T iPointSizeMax;

    T iPointDistanceAttenuation[ 3 ];
    
    T iPointFadeThresholdSize;
    
    };



/**
 *  TGlPointX
 * 
 */
class TGlPointX : public TGlPoint< GLfixed >
    {
public:
    TGlPointX();

    TGlPointX( GLfixed aSize );    

    TGlPointX( GLfixed aSize, TBool aAntialiased );
    
    void Use();
    };


/**
 *  TGlPointF
 * 
 */
class TGlPointF : public TGlPoint< GLfloat >
    {
public:
    TGlPointF();
    
    TGlPointF( GLfloat aSize );    
    
    TGlPointF( GLfloat aSize, TBool aAntialiased );
    
    void Use();
    };



#include <SGEGl/GlPoint.inl>

#endif // GLPOINT_H
