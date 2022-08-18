/*
 ============================================================================
 Name		: GlLine.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : TGlLine implementation
 ============================================================================
 */

#include "GlLine.h"



/*
 *  TGlLineX
 */
TGlLineX::TGlLineX( GLenum aType ) : TGlLine< GLfixed >( aType )
    {
    iWidth = 1 << 16;
    }

TGlLineX::TGlLineX( GLfixed aWidth, GLenum aType ) : TGlLine< GLfixed >( aWidth, aType )
    {
    }

TGlLineX::TGlLineX( GLfixed aWidth, TBool aAntialiased, GLenum aType ) : TGlLine< GLfixed >( aWidth, aAntialiased, aType )
    {
    }

void TGlLineX::Use()
    {
    ( iLineSmooth ? glEnable( GL_LINE_SMOOTH ) : glDisable( GL_LINE_SMOOTH ) );
    glLineWidthx( iWidth );
    }



/*
 *  TGlLineF
 */
TGlLineF::TGlLineF( GLenum aType ) : TGlLine< GLfloat >( aType )
    {
    iWidth = 1.0;
    }

TGlLineF::TGlLineF( GLfloat aWidth, GLenum aType ) : TGlLine< GLfloat >( aWidth, aType )
    {
    }

TGlLineF::TGlLineF( GLfloat aWidth, TBool aAntialiased, GLenum aType ) : TGlLine< GLfloat >( aWidth, aAntialiased, aType )
    {
    }

void TGlLineF::Use()
    {
    ( iLineSmooth ? glEnable( GL_LINE_SMOOTH ) : glDisable( GL_LINE_SMOOTH ) );
    glLineWidth( iWidth );
    }


