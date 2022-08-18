/*
 ============================================================================
 Name		: GlUtils.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : GlUtils implementation
 ============================================================================
 */

#include "GlUtils.h"
#include <gles/gl.h>


/**
 * TRANSPARENCY
 */
void GlUtils::EnableTransparency()
    {
    glEnable( GL_BLEND ); 
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    }

void GlUtils::DisableTransparency()
    {
    glDisable( GL_BLEND ); 
    }

/**
 * TRANSPARENCY
 */
void GlUtils::EnableAlphaTest()
    {
    glEnable( GL_ALPHA_TEST );
    glAlphaFuncx( GL_GREATER, 0 );    
    }

void GlUtils::DisableAlphaTest()
    {
    glDisable( GL_ALPHA_TEST );
    }

/**
 * MULTISAMPLING
 */
void GlUtils::EnableMultisampling()
    {
    glEnable( GL_MULTISAMPLE );
    }

void GlUtils::DisableMultisampling()
    {
    glDisable( GL_MULTISAMPLE );
    }

TBool GlUtils::IsMultisamplingSupported()
    {
    GLint buf[ 1 ];
    glGetIntegerv( GL_SAMPLE_BUFFERS, buf );
    return ( buf[ 0 ] == 1 );
    }

/**
 * SHADE MODEL
 */
void GlUtils::SetShadeModelFlat()
    {
    glShadeModel( GL_FLAT );
    }

void GlUtils::SetShadeModelSmooth()
    {
    glShadeModel( GL_SMOOTH );
    }

/**
 * HINTS
 */
void GlUtils::SetPerspectiveCorrectionFastest()
    {
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST );
    }

void GlUtils::SetPerspectiveCorrectionNicest()
    {
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
    }

void GlUtils::UnsetPerspectiveCorrection()
    {
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_DONT_CARE );
    }

/**
 * CLEARING
 */
void GlUtils::SetBlackClearColor()
    {
    glClearColorx( 0, 0, 0, 1 << 8 );
    }

void GlUtils::ClearColor()
    {
    glClear( GL_COLOR_BUFFER_BIT );
    }


/**
 * TEXTURING
 */
void GlUtils::EnableTexturing()
    {
    glEnable( GL_TEXTURE_2D );
    }

void GlUtils::DisableTexturing()
    {
    glDisable( GL_TEXTURE_2D );
    }

void GlUtils::UnbindTexture( TBool aResetMatrix  )
    {
    glBindTexture( GL_TEXTURE_2D, NULL );

    if ( aResetMatrix )
        {
        glMatrixMode( GL_TEXTURE );
        glLoadIdentity();
        glMatrixMode( GL_MODELVIEW );
        }
    }

/**
 * VIEW
 */
void GlUtils::SetViewportAsScreen( TSize aScreenSize )
    {
    glViewport( 0, 0, aScreenSize.iWidth, aScreenSize.iHeight );
    }

void GlUtils::SetSceneSizeAsScreen( TSize aScreenSize )
    {
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrthox( 0, aScreenSize.iWidth << 16, 0, aScreenSize.iHeight << 16, 1 << 16, -1 << 16 );
    glMatrixMode( GL_MODELVIEW );
    }

/**
 * MATRIX
 */
void GlUtils::LoadIdentity()
    {
    glLoadIdentity();
    }

void GlUtils::Translate2D( TPoint aVector )
	{
	glTranslatef( aVector.iX, aVector.iY, 0 );
	}

void GlUtils::Scale2D( TSize aVector )
	{
	glScalef( aVector.iWidth, aVector.iHeight, 0 );
	}

/**
 * INFO
 */
TPtrC8 GlUtils::GetExtensions()
	{
	const TUint8* extensions = glGetString(GL_EXTENSIONS);
	TPtrC8 ptr( extensions );
	return ptr;
	}

/**
 * TEST DRAW
 */
void GlUtils::TestDraw()
	{
    static const GLshort vertices[ 4 * 2 ] =
         {
         0, 0,
         200, 0,
         200, 200,
         0, 200
         };

    static const GLubyte colors[ 4 * 4 ] =
         {
         255, 0, 0, 255,
         0, 255, 0, 255,
         0, 0, 255, 128,
         255, 0, 255, 10
         };

    static const GLubyte triangles[ 2 * 3 ] =
        {
        0, 1, 2,
        0, 2, 3
        };

    glEnableClientState( GL_VERTEX_ARRAY );

    glVertexPointer( 2, GL_SHORT, 0, vertices );

    glEnableClientState( GL_COLOR_ARRAY );

    glColorPointer( 4, GL_UNSIGNED_BYTE, 0, colors );

    glDrawElements( GL_TRIANGLES, 2 * 3, GL_UNSIGNED_BYTE, triangles );

    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_COLOR_ARRAY );
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	}


TBool GlUtils::TestBlendFunc( TInt aIdx )
	{
	switch( aIdx )
	                {
	                        case 0: glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ); break;
	                        case 1: glBlendFunc( GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA ); break;
	                        case 2: glBlendFunc( GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA ); break;
	                        case 3: glBlendFunc( GL_ONE_MINUS_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR ); break;
	                        case 4: glBlendFunc( GL_ONE_MINUS_SRC_COLOR, GL_ONE_MINUS_DST_COLOR ); break;
	                        case 5: glBlendFunc( GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR ); break;
	                        case 6: glBlendFunc( GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_DST_COLOR ); break;
	                        case 7: glBlendFunc( GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA ); break;
	                        case 8: glBlendFunc( GL_ONE_MINUS_DST_ALPHA, GL_SRC_ALPHA ); break;
	                        case 9: glBlendFunc( GL_SRC_ALPHA, GL_SRC_ALPHA ); break;
	                        case 10: glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ); break;
	                        case 11: glBlendFunc( GL_DST_ALPHA, GL_ONE_MINUS_SRC_ALPHA ); break;
	                        case 12: glBlendFunc( GL_DST_ALPHA, GL_SRC_ALPHA ); break;
	                        case 13: glBlendFunc( GL_DST_ALPHA, GL_DST_ALPHA ); break;
	                        case 14: glBlendFunc( GL_SRC_ALPHA, GL_DST_ALPHA ); break;
	                        case 15: glBlendFunc( GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_DST_COLOR ); break;
	                        case 16: glBlendFunc( GL_ONE_MINUS_DST_ALPHA, GL_ONE_MINUS_DST_COLOR ); break;
	                        case 17: glBlendFunc( GL_ONE_MINUS_DST_ALPHA, GL_ONE_MINUS_SRC_COLOR ); break;
	                        case 18: glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR ); break;
	                        case 19: glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR ); break;
	                        case 20: glBlendFunc( GL_DST_ALPHA, GL_ONE_MINUS_SRC_COLOR ); break;
	                        case 21: glBlendFunc( GL_ONE_MINUS_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR ); break;
	                        case 22: glBlendFunc( GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR ); break;
	                        case 23: glBlendFunc( GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR ); break;
	                        case 24: glBlendFunc( GL_ONE_MINUS_DST_ALPHA, GL_ONE_MINUS_SRC_COLOR ); break;
	                        case 25: glBlendFunc( GL_ONE_MINUS_SRC_COLOR , GL_SRC_ALPHA ); break;
	                        case 26: glBlendFunc( GL_ONE_MINUS_SRC_COLOR , GL_DST_ALPHA ); break;
	                        case 27: glBlendFunc( GL_ONE_MINUS_SRC_COLOR , GL_ONE_MINUS_DST_COLOR ); break;
	                        case 28: glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR ); break;
	                        case 29: glBlendFunc( GL_DST_ALPHA, GL_ONE_MINUS_SRC_COLOR ); break;
	                        case 30: glBlendFunc( GL_ONE_MINUS_SRC_COLOR, GL_DST_ALPHA ); break;
	                        case 31: glBlendFunc( GL_ONE_MINUS_DST_COLOR, GL_DST_ALPHA ); break;
	                        case 32: glBlendFunc( GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA ); break;
	                        case 33: glBlendFunc( GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA ); break;
	                        case 34: glBlendFunc( GL_ONE_MINUS_SRC_COLOR , GL_DST_ALPHA ); break;
	                        case 35: glBlendFunc( GL_DST_ALPHA, GL_ONE_MINUS_SRC_ALPHA ); break;
	                        case 36: glBlendFunc( GL_DST_ALPHA, GL_ONE_MINUS_DST_COLOR ); break;
	                        case 37: glBlendFunc( GL_DST_COLOR, GL_ZERO ); break;
	                        case 38: glBlendFunc( GL_SRC_COLOR, GL_ZERO ); break;
	                        case 39: glBlendFunc( GL_DST_COLOR, GL_ONE ); break;
	                        case 40: glBlendFunc( GL_SRC_COLOR, GL_ONE ); break;
	                        case 41: glBlendFunc( GL_DST_ALPHA, GL_ONE ); break;
	                        case 42: glBlendFunc( GL_DST_ALPHA, GL_ZERO ); break;
	                        case 43: glBlendFunc( GL_SRC_ALPHA, GL_ONE ); break;
	                        case 44: glBlendFunc( GL_SRC_ALPHA, GL_ZERO ); break;
	                        case 45: glBlendFunc( GL_ZERO, GL_ONE_MINUS_SRC_COLOR ); break;
	                        case 46: glBlendFunc( GL_ZERO, GL_DST_ALPHA ); break;
	                        case 47: glBlendFunc( GL_ONE, GL_SRC_ALPHA ); break;
	                        case 48: glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_COLOR ); break;
	                        case 49: glBlendFunc( GL_ZERO, GL_ONE_MINUS_DST_ALPHA ); break;
	                        case 50: glBlendFunc( GL_ZERO, GL_ONE_MINUS_SRC_ALPHA ); break;
	                        case 51: glBlendFunc( GL_ONE, GL_DST_ALPHA ); break;
	                        case 52: glBlendFunc( GL_ONE, GL_ONE_MINUS_DST_COLOR ); break;
	                        default: return EFalse;
	                }	
	return ETrue;
	}
