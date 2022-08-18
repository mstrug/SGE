/*
 ============================================================================
 Name		: GlTransformation.h
 Author	  : Michal Strug
 Version	 : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : GlTransformation declaration
 ============================================================================
 */

#ifndef GLTRANSFORMATION_H
#define GLTRANSFORMATION_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <gles/gl.h>

// CLASS DECLARATION

/**
 *  TGlTransformation
 * 
 */
class TGlTransformation
    {
public:
    virtual void Transform();
    };


/*
 * TGlTransIdentity
 */

class TGlTransIdentity : public TGlTransformation
    {
public:
    void Transform();
    };


/*
 * TGlTransTranslation
 */

class TGlTransTranslationX : public TGlTransformation
    {
    GLfixed iX;
    GLfixed iY;
    GLfixed iZ;
public:
    TGlTransTranslationX( GLfixed aX, GLfixed aY, GLfixed aZ = 0 );
    void Transform();
    };

class TGlTransTranslationF : public TGlTransformation
    {
    GLfloat iX;
    GLfloat iY;
    GLfloat iZ;
public:
    TGlTransTranslationF( GLfloat aX, GLfloat aY, GLfloat aZ = 0 );
    void Transform();
    };


/*
 * TGlTransRotation
 */

class TGlTransRotationX : public TGlTransformation
    {
    GLfixed iT;
    GLfixed iX;
    GLfixed iY;
    GLfixed iZ;
public:
    TGlTransRotationX( GLfixed aT, GLfixed aX, GLfixed aY, GLfixed aZ = 0 );
    void Transform();
    };

class TGlTransRotationF : public TGlTransformation
    {
    GLfloat iT;
    GLfloat iX;
    GLfloat iY;
    GLfloat iZ;
public:
    TGlTransRotationF( GLfloat aT, GLfloat aX, GLfloat aY, GLfloat aZ = 0 );
    void Transform();
    };


/*
 * TGlTransScale
 */

class TGlTransScaleX : public TGlTransformation
    {
    GLfixed iX;
    GLfixed iY;
    GLfixed iZ;
public:
    TGlTransScaleX( GLfixed aX, GLfixed aY, GLfixed aZ = 1 );
    void Transform();
    };

class TGlTransScaleF : public TGlTransformation
    {
    GLfloat iX;
    GLfloat iY;
    GLfloat iZ;
public:
    TGlTransScaleF( GLfloat aX, GLfloat aY, GLfloat aZ = 1 );
    void Transform();
    };


#endif // GLTRANSFORMATION_H
