/*
 ============================================================================
 Name		: GlMaterial.h
 Author	  : Michal Strug
 Version	 : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : TGlMaterial declaration
 ============================================================================
 */

#ifndef GLMATERIAL_H
#define GLMATERIAL_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <gles/gl.h>
#include <SGEGl/GlMultitexture.h>

// CLASS DECLARATION

/**
 *  TGlMaterial
 * todo: add glShadeModel to material?
 */

const TInt KGlMaterialAmbientSize = 4;
const TInt KGlMaterialDiffuseSize = 4;
const TInt KGlMaterialSpecularSize = 4;
const TInt KGlMaterialEmissionSize = 4;

class TGlMaterial
    {
public:

    TUint iMaterialId;

    TGlMultitexture iTextures;

public:
    TGlMaterial();
    virtual void Use();
    void Copy( TGlMaterial aSourceMaterial );
    };


// this class is only for internal usage
template <class T> 
class TGlMaterialT : public TGlMaterial
    {
public:

    T iAmbient[ KGlMaterialAmbientSize ];
    T iDiffuse[ KGlMaterialDiffuseSize ];
    T iSpecular[ KGlMaterialSpecularSize ];
    T iEmission[ KGlMaterialEmissionSize ];
    T iShininess;

public:
    void Copy( TGlMaterialT aSourceMaterial );
    };



class TGlMaterialX : public TGlMaterialT< GLfixed >
    {
public:
    void Use();
    };


class TGlMaterialF : public TGlMaterialT< GLfloat >
    {
public:
    void Use();
    };


#include <SGEGl/GlMaterial.inl>

#endif // GLMATERIAL_H
