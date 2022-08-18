/*
 ============================================================================
 Name		: GlMaterial.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : TGlMaterial implementation
 ============================================================================
 */

#include "GlMaterial.h"
#include "GlTexture.h"


/*
 * TGlMaterial
 */

TGlMaterial::TGlMaterial()
    {
    iTextures.Reset();
    }

void TGlMaterial::Use()
    {
    for ( TInt i = 0; i < iTextures.Count(); i++ )
        {
        if ( iTextures[ i ] )
            {
            iTextures[ i ]->Use();
            }
        }
    }

void TGlMaterial::Copy( TGlMaterial aSourceMaterial )
    {
    for ( TInt i = 0; i < iTextures.Count(); i++ )
        {
        iTextures[ i ] = aSourceMaterial.iTextures[ i ];
        }
    }



/*
 * TGlTransTranslationX
 */

void TGlMaterialX::Use()
    {
    glMaterialxv( GL_FRONT_AND_BACK, GL_AMBIENT, iAmbient );
    glMaterialxv( GL_FRONT_AND_BACK, GL_DIFFUSE, iDiffuse );
    glMaterialxv( GL_FRONT_AND_BACK, GL_SPECULAR, iSpecular );
    glMaterialxv( GL_FRONT_AND_BACK, GL_EMISSION, iEmission );
    glMaterialx( GL_FRONT_AND_BACK, GL_SHININESS, iShininess );
    
    TGlMaterial::Use();
    }




/*
 * TGlTransTranslationF
 */

void TGlMaterialF::Use()
    {
    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, iAmbient );
    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, iDiffuse );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, iSpecular );
    glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, iEmission );
    glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, iShininess );

    TGlMaterial::Use();
    }

