/*
 ============================================================================
 Name		: GlObjectCube.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CGlObjectCube implementation
 ============================================================================
 */

#include "GlObjectCube.h"
#include "GlPrimitive.h"
#include "GlPolygon.h"
#include "GlVertexArray.h"


const TInt KVertexSize = 3;
const TInt KVertexCount = 8;
const TInt KVertexColorSize = 4;
const TInt KVertexPerTriangle = 3;
const TInt KTrianglesPerFace = 2;
const TInt KFaces = 6;

/** Vertice coordinates for the cube. */
static const GLbyte KVertices[ KVertexCount * KVertexSize ] =
     {
     -1,  1,  1,
      1,  1,  1,
      1, -1,  1,
     -1, -1,  1,

     -1,  1, -1,
      1,  1, -1,
      1, -1, -1,
     -1, -1, -1
     };

/** Colors for vertices (Red, Green, Blue, Alpha). */
static const GLubyte KColors[ KVertexCount * KVertexColorSize ] =
    {
    0  ,255,  0,255,
    0  ,  0,255,255,
    0  ,255,  0,255,
    255,  0,  0,255,

    0  ,  0,255,255,
    255,  0,  0,255,
    0  ,  0,255,255,
    0  ,255,  0,255
    };

/**
 * Indices for drawing the triangles.
 * The color of the triangle is determined by
 * the color of the last vertex of the triangle.
 */
static const GLubyte KTriangles[ KTrianglesPerFace * KFaces * KVertexPerTriangle ] =
    {
    /* front */
    1,0,3,
    1,3,2,

    /* right */
    2,6,5,
    2,5,1,

    /* back */
    7,4,5,
    7,5,6,

    /* left */
    0,4,7,
    0,7,3,

    /* top */
    5,4,0,
    5,0,1,

    /* bottom */
    3,7,6,
    3,6,2
    };




CGlObjectCube::CGlObjectCube()
    {
    // No implementation required
    }

CGlObjectCube::~CGlObjectCube()
    {
    delete iDataCube;
    }

CGlObjectCube* CGlObjectCube::NewLC()
    {
    CGlObjectCube* self = new ( ELeave ) CGlObjectCube();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

CGlObjectCube* CGlObjectCube::NewL()
    {
    CGlObjectCube* self = CGlObjectCube::NewLC();
    CleanupStack::Pop(); // self;
    return self;
    }

void CGlObjectCube::ConstructL()
    {
    CGlObject::ConstructL();
    
    iPrimitive = CGlPrimitive::NewL();
    iPrimitiveOwner = ETrue;

    TGlVertexArrayConfig cfg( EVertexTypeByte, ENormalTypeNone, EColorTypeUnsignedByte, 
                              EPointSizeTypeNone, ETexCoordTypeNone, 
                              EVertexSizeThree, ENormalSizeNone, EColorSizeFour, EPointSizeNone,
                              ETexCoordSizeNone, 0,
                              EIndicesTypeUnsignedByte, KTrianglesPerFace * KFaces * KVertexPerTriangle );
    
    iDataCube = CGlVertexArray::NewL( cfg, KVertexCount );

    iDataCube->SetVertexData( (TAny*) KVertices, KVertexCount * KVertexSize * sizeof( GLbyte ) );
    iDataCube->SetColorData( (TAny*) KColors, KVertexCount * KVertexColorSize * sizeof( GLubyte ) );
    iDataCube->SetIndices( (TAny*) KTriangles, KTrianglesPerFace * KFaces * KVertexPerTriangle * sizeof( GLubyte ), EFalse );
    
    //iPrimitive->PushTransformationL( new TGlTransScaleF( 1.f, 2.f, 3.f ) );
    iPrimitive->AddVertexArrayL( iDataCube );
    
    
    iPrimitiveType = new (ELeave) TGlPolygonX();
    iPrimitiveTypeOwner = ETrue;
    }
