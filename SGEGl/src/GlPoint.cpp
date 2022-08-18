/*
 ============================================================================
 Name		: GlPoint.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CGlPoint implementation
 ============================================================================
 */

#include "GlPoint.h"


/*
 *  TGlPointX
 */
TGlPointX::TGlPointX()
    {
    iSize = 1 << 8;
    
    iPointSizeMin = 0;
    iPointSizeMax = 1 << 8;
    
    iPointDistanceAttenuation[ 0 ] = 1 << 8;
    iPointDistanceAttenuation[ 1 ] = iPointDistanceAttenuation[ 2 ] = 0;
    
    iPointFadeThresholdSize = 1 << 8;
    }

TGlPointX::TGlPointX( GLfixed aSize ) : TGlPoint< GLfixed >( aSize )
    {
    iPointSizeMin = 0;
    iPointSizeMax = 1 << 8;
    
    iPointDistanceAttenuation[ 0 ] = 1 << 8;
    iPointDistanceAttenuation[ 1 ] = iPointDistanceAttenuation[ 2 ] = 0;
    
    iPointFadeThresholdSize = 1 << 8;
    }

TGlPointX::TGlPointX( GLfixed aSize, TBool aAntialiased ) : TGlPoint< GLfixed >( aSize, aAntialiased )
    {
    iPointSizeMin = 0;
    iPointSizeMax = 1 << 8;
    
    iPointDistanceAttenuation[ 0 ] = 1 << 8;
    iPointDistanceAttenuation[ 1 ] = iPointDistanceAttenuation[ 2 ] = 0;
    
    iPointFadeThresholdSize = 1 << 8;
    }

void TGlPointX::Use()
    {
    ( iPointSmooth ? glEnable( GL_POINT_SMOOTH ) : glDisable( GL_POINT_SMOOTH ) );
    
    if ( iPointSprites )
        {
        glEnable( GL_POINT_SPRITE_OES );
        glTexEnvx( GL_POINT_SPRITE_OES, GL_COORD_REPLACE_OES, ( iCoordReplace ? GL_TRUE : GL_FALSE ) );
        }
    else
        {
        glDisable( GL_POINT_SPRITE_OES );
        }
    
    glPointParameterx( GL_POINT_SIZE_MIN, iPointSizeMin );
    glPointParameterx( GL_POINT_SIZE_MAX, iPointSizeMax );
    glPointParameterx( GL_POINT_FADE_THRESHOLD_SIZE, iPointFadeThresholdSize );
    glPointParameterxv( GL_POINT_DISTANCE_ATTENUATION, iPointDistanceAttenuation );
    
    glPointSizex( iSize );
    }



/* 
 *  TGlPointF
 */
TGlPointF::TGlPointF()
    {
    iSize = 1.0;
    
    iPointSizeMin = 0;
    iPointSizeMax = 1.0;
    
    iPointDistanceAttenuation[ 0 ] = 1.0;
    iPointDistanceAttenuation[ 1 ] = iPointDistanceAttenuation[ 2 ] = 0;
    
    iPointFadeThresholdSize = 1.0;
    }

TGlPointF::TGlPointF( GLfloat aSize ) : TGlPoint< GLfloat >( aSize )
    {
    iPointSizeMin = 0;
    iPointSizeMax = 1.0;
    
    iPointDistanceAttenuation[ 0 ] = 1.0;
    iPointDistanceAttenuation[ 1 ] = iPointDistanceAttenuation[ 2 ] = 0;
    
    iPointFadeThresholdSize = 1.0;
    }

TGlPointF::TGlPointF( GLfloat aSize, TBool aAntialiased ) : TGlPoint< GLfloat >( aSize, aAntialiased )
    {
    iPointSizeMin = 0;
    iPointSizeMax = 1.0;
    
    iPointDistanceAttenuation[ 0 ] = 1.0;
    iPointDistanceAttenuation[ 1 ] = iPointDistanceAttenuation[ 2 ] = 0;
    
    iPointFadeThresholdSize = 1.0;    
    }

void TGlPointF::Use()
    {
    ( iPointSmooth ? glEnable( GL_POINT_SMOOTH ) : glDisable( GL_POINT_SMOOTH ) );
    
    if ( iPointSprites )
        {
        glEnable( GL_POINT_SPRITE_OES );
        glTexEnvf( GL_POINT_SPRITE_OES, GL_COORD_REPLACE_OES, ( iCoordReplace ? GL_TRUE : GL_FALSE ) );
        }
    else
        {
        glDisable( GL_POINT_SPRITE_OES );
        }
    
    glPointParameterf( GL_POINT_SIZE_MIN, iPointSizeMin );
    glPointParameterf( GL_POINT_SIZE_MAX, iPointSizeMax );
    glPointParameterf( GL_POINT_FADE_THRESHOLD_SIZE, iPointFadeThresholdSize );
    glPointParameterfv( GL_POINT_DISTANCE_ATTENUATION, iPointDistanceAttenuation );
    
    glPointSize( iSize );
    }



