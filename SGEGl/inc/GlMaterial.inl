/*
 ============================================================================
 Name		: GlMaterial.inl
 Author	  : Michal Strug
 Version	 : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : TGlMaterial declaration
 ============================================================================
 */

#ifndef GLMATERIAL_INL
#define GLMATERIAL_INL

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <gles/gl.h>
#include <SGEGl/GlMaterial.h>



/*
 * TGlMaterialT
 */
template <class T> void TGlMaterialT<T>::Copy( TGlMaterialT<T> aSourceMaterial )
    {
    for ( TInt i = 0; i < KGlMaterialAmbientSize; i++ )
        {
        iAmbient[ i ] = aSourceMaterial.iAmbient[ i ];
        }
    for ( TInt i = 0; i < KGlMaterialDiffuseSize; i++ )
        {
        iDiffuse[ i ] = aSourceMaterial.iDiffuse[ i ];
        }
    for ( TInt i = 0; i < KGlMaterialEmissionSize; i++ )
        {
        iEmission[ i ] = aSourceMaterial.iEmission[ i ];
        }
    for ( TInt i = 0; i < KGlMaterialSpecularSize; i++ )
        {
        iSpecular[ i ] = aSourceMaterial.iSpecular[ i ];
        }
    iShininess = aSourceMaterial.iShininess;
    
    TGlMaterial::Copy( aSourceMaterial );
    }


#endif // GLMATERIAL_INL
