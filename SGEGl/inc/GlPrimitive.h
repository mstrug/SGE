/*
 ============================================================================
 Name		: GlPrimitive.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CGlPrimitive declaration
 ============================================================================
 */

#ifndef GLPRIMITIVE_H
#define GLPRIMITIVE_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <gles/gl.h>
#include <SGEGl/GlTransformation.h>
#include <SGEGl/GlVertexArray.h>
#include <SGEGl/GlMultitexture.h>
#include <SGEGl/GlMaterial.h>

// CLASS DECLARATION

/**
 *  CGlPrimitive
 * 
 */
class CGlPrimitive : public CBase
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CGlPrimitive();

    /**
     * Two-phased constructor.
     */
    static CGlPrimitive* NewL();

    /**
     * Two-phased constructor.
     */
    static CGlPrimitive* NewLC();

    void AddChildPrimitiveL( CGlPrimitive *aPrimitive );

    void PushTransformationL( TGlTransformation *aTransformation );

    void PopTransformation();
    
    void AddVertexArrayL( CGlVertexArray *aVertexArray );

    void AddMaterialL( TGlMaterial aMaterial );
    
    void Draw( GLenum aType );

    void Redraw( GLenum aType );
    
    CGlVertexArray* VertexArray( TInt aIndex ) const;

private:

    /**
     * Constructor for performing 1st stage construction
     */
    CGlPrimitive();

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();
    
    void Transform();
    
private: // data
    
    RPointerArray< CGlPrimitive > iChildPrimitives; // not owned
    
    RPointerArray< TGlTransformation > iTransformations; // owned

    RPointerArray< CGlVertexArray > iVertexArray; // not owned
    
    RArray< TGlMaterial > iMaterials;
    
    TInt iMaxTextureUnits;
    
    };

#endif // GLPRIMITIVE_H
