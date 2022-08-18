/*
 ============================================================================
 Name		: GlPolygon.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CGlPolygon implementation
 ============================================================================
 */

#include "GlPolygon.h"



/*
 *  TGlPolygonX
 */
TGlPolygonX::TGlPolygonX( GLenum aType ) : TGlPolygon< GLfixed >( aType )
    {
    }

TGlPolygonX::TGlPolygonX( GLenum aCullFaceType, GLenum aType ) : TGlPolygon< GLfixed >( aCullFaceType, aType )
    {
    }

void TGlPolygonX::Use()
    {
    if ( iCullFaceEnabled )
        {
        glCullFace( iCullFaceType );
        glEnable( GL_CULL_FACE ); 
        }
    else
        {
        glDisable( GL_CULL_FACE ); 
        }
    
    if ( iOffsetFillEnabled )
        {
        glPolygonOffsetx( iOffsetFactor, iOffsetUnits );
        }
    }


/*
 *  TGlPolygonF
 */
TGlPolygonF::TGlPolygonF( GLenum aType ) : TGlPolygon< GLfloat >( aType )
    {
    }

TGlPolygonF::TGlPolygonF( GLenum aCullFaceType, GLenum aType ) : TGlPolygon< GLfloat >( aCullFaceType, aType )
    {
    }

void TGlPolygonF::Use()
    {
    if ( iCullFaceEnabled )
        {
        glCullFace( iCullFaceType );
        glEnable( GL_CULL_FACE ); 
        }
    else
        {
        glDisable( GL_CULL_FACE ); 
        }
    
    if ( iOffsetFillEnabled )
        {
        glPolygonOffset( iOffsetFactor, iOffsetUnits );
        }
    }



