/*
 ============================================================================
 Name		: GlVertexArrayConfig.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : TGlVertexArrayConfig declaration
 ============================================================================
 */

#ifndef GLVERTEXARRAYCONFIG_H
#define GLVERTEXARRAYCONFIG_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <gles/gl.h>

// CLASS DECLARATION

    enum TGlVertexPointerType
        {
        EVertexTypeByte = GL_BYTE,
        EVertexTypeShort = GL_SHORT,
        EVertexTypeFixed = GL_FIXED,
        EVertexTypeFloat = GL_FLOAT
        };
    
    enum TGlNormalPointerType
        {
        ENormalTypeNone = 0,
        ENormalTypeByte = GL_BYTE,
        ENormalTypeShort = GL_SHORT,
        ENormalTypeFixed = GL_FIXED,
        ENormalTypeFloat = GL_FLOAT
        };
    
    enum TGlColorPointerType
        {
        EColorTypeNone = 0,
        EColorTypeUnsignedByte = GL_UNSIGNED_BYTE,
        EColorTypeFixed = GL_FIXED,
        EColorTypeFloat = GL_FLOAT
        };
    
    enum TGlPointSizePointerType
        {
        EPointSizeTypeNone = 0,
        EPointSizeTypeFixed = GL_FIXED,
        EPointSizeTypeFloat = GL_FLOAT
        };
    
    enum TGlTexCoordPointerType
        {
        ETexCoordTypeNone = 0,
        ETexCoordTypeByte = GL_BYTE,
        ETexCoordTypeShort = GL_SHORT,
        ETexCoordTypeFixed = GL_FIXED,
        ETexCoordTypeFloat = GL_FLOAT
        };
    
    enum TGlVertexPointerSize
        {
        EVertexSizeTwo = 2,
        EVertexSizeThree,
        EVertexSizeFour
        };

    enum TGlNormalPointerSize
        {
        ENormalSizeNone = 0,
        ENormalSizeThree = 3
        };

    enum TGlColorPointerSize
        {
        EColorSizeNone = 0,
        EColorSizeFour = 4
        };

    enum TGlPointSizePointerSize
        {
        EPointSizeNone = 0,
        EPointSizeOne = 1
        };

    enum TGlTexCoordPointerSize
        {
        ETexCoordSizeNone = 0,
        ETexCoordSizeTwo = 2,
        ETexCoordSizeThree,
        ETexCoordSizeFour
        };

    enum TGlIndicesType
        {
        EIndicesTypeNone = 0,
        EIndicesTypeUnsignedByte = GL_UNSIGNED_BYTE,
        EIndicesTypeUnsignedShort = GL_UNSIGNED_SHORT
        };


class TGlVertexArrayConfig
    {
public: // data
    
    // size of the type
    TInt iVertexPointerType;
    TInt iNormalPointerType;
    TInt iColorPointerType;
    TInt iPointSizePointerType;
    TInt iTexCoordPointerType;
    
    // type as gl type
    TGlVertexPointerType iVertexPointerGlType;
    TGlNormalPointerType iNormalPointerGlType;
    TGlColorPointerType iColorPointerGlType;
    TGlPointSizePointerType iPointSizePointerGlType;
    TGlTexCoordPointerType iTexCoordPointerGlType;
    
    // count of types per vertex
    TGlVertexPointerSize iVertexPointerSize;
    TGlNormalPointerSize iNormalPointerSize;
    TGlColorPointerSize iColorPointerSize;
    TGlPointSizePointerSize iPointSizePointerSize;
    TGlTexCoordPointerSize iTexCoordPointerSize;
    
    // count of tex coord arrays
    TInt iTexCoordPointerCount;
    
    // indices
    TGlIndicesType iIndicesGlType;

    // size of indices type
    TInt iIndicesType;
    
    // count of indices array
    TInt iIndicesCount;
    
public:
    TGlVertexArrayConfig( TGlVertexPointerType aVT, TGlNormalPointerType aNT, TGlColorPointerType aCT, 
                          TGlPointSizePointerType aPT, TGlTexCoordPointerType aTT,
                          TGlVertexPointerSize aVS, TGlNormalPointerSize aNS, TGlColorPointerSize aCS,
                          TGlPointSizePointerSize aPS, TGlTexCoordPointerSize aTS, TInt aTC,
                          TGlIndicesType aIndicesType = EIndicesTypeNone, TInt aIndicesCount = 0 );
    
private:    
    TInt SizeOf( TInt aType );
    };


#endif // GLVERTEXARRAYCONFIG_H
