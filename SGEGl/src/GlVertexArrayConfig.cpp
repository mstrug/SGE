/*
 ============================================================================
 Name		: GlVertexArrayConfig.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : TGlVertexArrayConfig implementation
 ============================================================================
 */

#include "GlVertexArrayConfig.h"

TGlVertexArrayConfig::TGlVertexArrayConfig( TGlVertexPointerType aVT, TGlNormalPointerType aNT, 
                                            TGlColorPointerType aCT, TGlPointSizePointerType aPT, 
                                            TGlTexCoordPointerType aTT, TGlVertexPointerSize aVS, 
                                            TGlNormalPointerSize aNS, TGlColorPointerSize aCS,
                                            TGlPointSizePointerSize aPS, TGlTexCoordPointerSize aTS, TInt aTC,
                                            TGlIndicesType aIndicesType, TInt aIndicesCount )
    : iVertexPointerGlType( aVT ), iNormalPointerGlType( aNT ), iColorPointerGlType( aCT ), 
      iPointSizePointerGlType( aPT ), iTexCoordPointerGlType( aTT ), iVertexPointerSize( aVS ), 
      iNormalPointerSize( aNS ), iColorPointerSize( aCS ),
      iPointSizePointerSize( aPS ), iTexCoordPointerSize( aTS ), iTexCoordPointerCount( aTC ),
      iIndicesGlType( aIndicesType ), iIndicesCount( aIndicesCount )
    {
    iVertexPointerType = SizeOf( iVertexPointerGlType );
    iNormalPointerType = SizeOf( iNormalPointerGlType );
    iColorPointerType = SizeOf( iColorPointerGlType );
    iPointSizePointerType = SizeOf( iPointSizePointerGlType );
    iTexCoordPointerType = SizeOf( iTexCoordPointerGlType );
    iIndicesType = SizeOf( iIndicesGlType );
    }

TInt TGlVertexArrayConfig::SizeOf( TInt aType )
    {
    switch ( aType )
        {
        case GL_BYTE: return sizeof( GLbyte );
        case GL_UNSIGNED_BYTE: return sizeof( GLubyte );
        case GL_SHORT: return sizeof( GLshort );
        case GL_UNSIGNED_SHORT: return sizeof( GLushort );
        case GL_FIXED: return sizeof( GLfixed );
        case GL_FLOAT: return sizeof( GLfloat );
        default: return 0;
        }
    }




