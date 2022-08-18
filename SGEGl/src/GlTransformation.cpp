/*
 ============================================================================
 Name		: GlTransformation.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : GlTransformation implementation
 ============================================================================
 */

#include "GlTransformation.h"


/*
 * TGlTransformation
 */

void TGlTransformation::Transform()
    {
    }


/*
 * TGlTransTranslation
 */

void TGlTransIdentity::Transform()
    {
    glLoadIdentity();
    }

/*
 * TGlTransTranslation
 */

TGlTransTranslationX::TGlTransTranslationX( GLfixed aX, GLfixed aY, GLfixed aZ )
    : iX( aX ), iY( aY ), iZ( aZ )
    {
    }

void TGlTransTranslationX::Transform()
    {
    glTranslatex( iX, iY, iZ );
    }


TGlTransTranslationF::TGlTransTranslationF( GLfloat aX, GLfloat aY, GLfloat aZ )
    : iX( aX ), iY( aY ), iZ( aZ )
    {
    }

void TGlTransTranslationF::Transform()
    {
    glTranslatef( iX, iY, iZ );
    }



/*
 * TGlTransRotation
 */

TGlTransRotationX::TGlTransRotationX( GLfixed aT, GLfixed aX, GLfixed aY, GLfixed aZ )
    : iT( aT ), iX( aX ), iY( aY ), iZ( aZ ) 
    {    
    }

void TGlTransRotationX::Transform()
    {
    glRotatex( iT, iX, iY, iZ );
    }


TGlTransRotationF::TGlTransRotationF( GLfloat aT, GLfloat aX, GLfloat aY, GLfloat aZ )
    : iT( aT ), iX( aX ), iY( aY ), iZ( aZ ) 
    {    
    }

void TGlTransRotationF::Transform()
    {
    glRotatef( iT, iX, iY, iZ );
    }



/*
 * TGlTransScale
 */

TGlTransScaleX::TGlTransScaleX( GLfixed aX, GLfixed aY, GLfixed aZ )
    : iX( aX ), iY( aY ), iZ( aZ )
    {    
    }

void TGlTransScaleX::Transform()
    {
    glScalex( iX, iY, iZ );
    }


TGlTransScaleF::TGlTransScaleF( GLfloat aX, GLfloat aY, GLfloat aZ )
    : iX( aX ), iY( aY ), iZ( aZ )
    {    
    }

void TGlTransScaleF::Transform()
    {
    glScalef( iX, iY, iZ );
    }

