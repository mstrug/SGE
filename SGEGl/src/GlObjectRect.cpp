/*
 ============================================================================
 Name		: GlObjectRect.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CGlObjectRect implementation
 ============================================================================
 */

#include "GlObjectRect.h"
#include "GlPrimitive.h"
#include "GlPolygon.h"
#include "GlVertexArray.h"


// Redefine value of pi.
#undef PI
#define PI 3.1415926535897932384626433832795f


// Converts angle __a from deg. to rad.
#define DEG_2_RAD(__a) ((__a)*(PI/180))

// Converts angle __a from rad. to deg.
#define RAD_2_DEG(__a) ((__a)*(180/PI))

// 16.16 Fixed point conversions and operations
#define INT_2_FIXED(__a) ( ( (GLfixed) (__a) ) << 16 )
#define FIXED_2_INT(__a) ( (GLint) ( (__a) >> 16 ) )
#define FLOAT_2_FIXED(__a) ( (GLfixed) ( 65536.0f * ( __a ) ) )
#define FIXED_2_FLOAT(__a) ( ( (GLfloat) (__a) ) * ( 1 / 65536.0f ) )
#define FIXED_ONE INT_2_FIXED( 1 )

#define FIXED_PI FLOAT_2_FIXED( 3.1415926535897932384626433832795f )



const TInt KVertexSize = 2;
const TInt KVertexCount = 4;
const TInt KVertexColorSize = 4;
const TInt KVertexPerTriangle = 3;
const TInt KTrianglesPerFace = 2;
const TInt KTexCoordSize = 2;

/** Vertice coordinates for the rect. */
//static const GLbyte KVertices[ KVertexCount * KVertexSize ] =
//     {
//      0,  1,
//      1,  1,
//      1,  0,
//      0,  0,
//     };
static const GLshort KVertices[ KVertexCount * KVertexSize ] =
     {
      0,  1,
      1,  1,
      1,  0,
      0,  0,
     };

/** Colors for vertices (Red, Green, Blue, Alpha). */
static const GLubyte KColors[ KVertexCount * KVertexColorSize ] =
    {
    255  ,255,  255, 255,
    255  ,255,  255, 255,
    255  ,255,  255, 255,
    255  ,255,  255, 255
    };

/**
 * Indices for drawing the triangles.
 * The color of the triangle is determined by
 * the color of the last vertex of the triangle.
 */
static const GLubyte KTriangles[ KTrianglesPerFace * KVertexPerTriangle ] =
    {
    /* front */
    0, 3, 1,
    3, 2, 1
    };


/** Texture coordinates. */
static const GLfixed KTexCoords[ KVertexCount * KTexCoordSize ] =
    {
    0, 0,
    INT_2_FIXED( 1 ), 0,
    INT_2_FIXED( 1 ), INT_2_FIXED( 1 ),
    0, INT_2_FIXED( 1 )
    };
//static const GLbyte KTexCoords[ KVertexCount * KTexCoordSize ] =
//    {
//    0, 0,
//    1, 0,
//    1, 1,
//    0, 1
//    };
//static const GLshort KTexCoords[ KVertexCount * KTexCoordSize ] =
//    {
//    0, 0,
//    1, 0,
//    1, 1,
//    0, 1
//    };
//static const GLfloat KTexCoords[ KVertexCount * KTexCoordSize ] =
//    {
//    0, 0,
//    1.0, 0,
//    1.0, 1.0,
//    0, 1.0
//    };

CGlObjectRect::CGlObjectRect() : iCurrentSize( 1, 1 )
    {
    // No implementation required
    }

CGlObjectRect::~CGlObjectRect()
    {
    delete iDataRect;
    }

CGlObjectRect* CGlObjectRect::NewLC()
    {
    CGlObjectRect* self = new ( ELeave ) CGlObjectRect();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

CGlObjectRect* CGlObjectRect::NewL()
    {
    CGlObjectRect* self = CGlObjectRect::NewLC();
    CleanupStack::Pop(); // self;
    return self;
    }

void CGlObjectRect::ConstructL()
    {
    CGlObject::ConstructL();
    
    iPrimitive = CGlPrimitive::NewL();
    iPrimitiveOwner = ETrue;

//    TGlVertexArrayConfig cfg( EVertexTypeByte, ENormalTypeNone, EColorTypeUnsignedByte, 
//                              EPointSizeTypeNone, ETexCoordTypeByte,
//                              EVertexSizeTwo, ENormalSizeNone, EColorSizeFour, EPointSizeNone,
//                              ETexCoordSizeTwo, 1,
//                              EIndicesTypeUnsignedByte, KTrianglesPerFace * KVertexPerTriangle );
    TGlVertexArrayConfig cfg( EVertexTypeShort, ENormalTypeNone, EColorTypeUnsignedByte, 
                              EPointSizeTypeNone, ETexCoordTypeFixed,
                              EVertexSizeTwo, ENormalSizeNone, EColorSizeFour, EPointSizeNone,
                              ETexCoordSizeTwo, 1,
                              EIndicesTypeUnsignedByte, KTrianglesPerFace * KVertexPerTriangle );
    
    iDataRect = CGlVertexArray::NewL( cfg, KVertexCount );

//    iDataRect->SetVertexData( (TAny*) KVertices, KVertexCount * KVertexSize * sizeof( GLbyte ) );
    iDataRect->SetVertexData( (TAny*) KVertices, KVertexCount * KVertexSize * sizeof( GLshort ) );
    iDataRect->SetColorData( (TAny*) KColors, KVertexCount * KVertexColorSize * sizeof( GLubyte ) );
//    iDataRect->SetTexCoordData( 0, (TAny*) KTexCoords, KVertexCount * KTexCoordSize * sizeof( GLbyte ) );
    iDataRect->SetTexCoordData( 0, (TAny*) KTexCoords, KVertexCount * KTexCoordSize * sizeof( GLfixed ) );
    iDataRect->SetIndices( (TAny*) KTriangles, KTrianglesPerFace * KVertexPerTriangle * sizeof( GLubyte ), EFalse );
    
    //iPrimitive->PushTransformationL( new TGlTransScaleF( 1.f, 2.f, 3.f ) );
    iPrimitive->AddVertexArrayL( iDataRect );    
    
    iPrimitiveType = new (ELeave) TGlPolygonX();
    iPrimitiveTypeOwner = ETrue;
    }

TGlPolygonX& CGlObjectRect::PrimitiveType() const
    {
    return dynamic_cast<TGlPolygonX&>( *iPrimitiveType );
    }

void CGlObjectRect::SetRectSize( TSize aSize )
    {
    if ( aSize != iCurrentSize )
        {
        iVertexData[ 0 ] = 0;
        iVertexData[ 1 ] = aSize.iHeight; 
        iVertexData[ 2 ] = aSize.iWidth;
        iVertexData[ 3 ] = aSize.iHeight; 
        iVertexData[ 4 ] = aSize.iWidth; 
        iVertexData[ 5 ] = 0; 
        iVertexData[ 6 ] = 0;
        iVertexData[ 7 ] = 0; 
    
        iDataRect->SetVertexData( (TAny*) iVertexData.Begin(), iVertexData.Count() * sizeof( GLshort ) );
        
        iCurrentSize = aSize;
        }
    }


