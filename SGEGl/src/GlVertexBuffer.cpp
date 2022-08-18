/*
 ============================================================================
 Name		: GlVertexBuffer.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CGlVertexBuffer implementation
 ============================================================================
 */

#include "GlVertexBuffer.h"

/*
 * MEMORY PATTERN:
 * 
 * 
 * Vertex0, Vertex1, Vertex2, ... , 
 * Normal0, Normal1, Normal2, ... ,
 * Color0, Color1, Color2, ... ,
 * PointSize0, PointSize1, PointSize2, ... ,
 * TextCoord00, TextCoord01, TextCoord02, ... ,
 * TextCoord10, TextCoord11, TextCoord12, ...
 * 
 */

// http://www.opengl.org/wiki/VBO_-_just_examples


const TInt KDataBufferIdx = 0;
const TInt KIndicesBufferIdx = 1;

CGlVertexBuffer::CGlVertexBuffer( TGlVertexArrayConfig aConfig, TInt aVertexCount )
    : CGlVertexArray( aConfig, aVertexCount )
    {
    iBuffersIds.Reset();
    }

CGlVertexBuffer::~CGlVertexBuffer()
    {
    DeleteBuffers();
    }

CGlVertexBuffer* CGlVertexBuffer::NewLC( TGlVertexArrayConfig aConfig, TInt aVertexCount )
    {
    CGlVertexBuffer *self = new (ELeave) CGlVertexBuffer( aConfig, aVertexCount );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

CGlVertexBuffer* CGlVertexBuffer::NewL( TGlVertexArrayConfig aConfig, TInt aVertexCount )
    {
    CGlVertexBuffer* self = CGlVertexBuffer::NewLC( aConfig, aVertexCount );
    CleanupStack::Pop(); // self;
    return self;
    }

void CGlVertexBuffer::ConstructL()
    {
    CGlVertexArray::ConstructL();
    }

void CGlVertexBuffer::GenerateBuffersIds()
    {
    glGenBuffers( iBuffersIds.Count(), iBuffersIds.Begin() );
    }

void CGlVertexBuffer::DeleteBuffers()
    {
    glDeleteBuffers( iBuffersIds.Count(), iBuffersIds.Begin() );
    iBuffersIds.Reset();
    }

TInt CGlVertexBuffer::CreateVertexBuffer( GLenum aUsage )
    {
    DeleteBuffers();
    GenerateBuffersIds();
    
    if ( !iData || ( !iIndices && iConfig.iIndicesGlType != EIndicesTypeNone ) )
        {
        return KErrNotReady;
        }
    
    glBindBuffer( GL_ARRAY_BUFFER, iBuffersIds[ KDataBufferIdx ] );
    glBufferData( GL_ARRAY_BUFFER, iVertexDataSize * iVertexCount, iData, aUsage );
    
    if ( iConfig.iIndicesGlType != EIndicesTypeNone )
        {
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, iBuffersIds[ KIndicesBufferIdx ] );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, iConfig.iIndicesCount * iConfig.iIndicesType, iIndices, aUsage );
        } 
    
    FreeData();

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    
    return KErrNone;
    }

TInt CGlVertexBuffer::UseVertexData()
    {
    if ( !iVertexArrayEnabled )
        {
        glDisableClientState( GL_VERTEX_ARRAY );
        return KErrNotSupported;
        }

    glEnableClientState( GL_VERTEX_ARRAY );

    glVertexPointer( iConfig.iVertexPointerSize, iConfig.iVertexPointerGlType, iVertexDataSize, NULL );
    
    return KErrNone;
    }

TInt CGlVertexBuffer::UseNormalData()
    {
    if ( iConfig.iNormalPointerSize == ENormalSizeNone ||
         iConfig.iNormalPointerGlType == ENormalTypeNone ||
         !iNormalArrayEnabled )
        {
        glDisableClientState( GL_NORMAL_ARRAY );
        return KErrNotSupported;
        }
    
    glEnableClientState( GL_NORMAL_ARRAY );
    
    TInt offset = iConfig.iVertexPointerSize * iConfig.iVertexPointerType;

    glNormalPointer( iConfig.iNormalPointerGlType, iVertexDataSize, (GLvoid*) offset );
    
    return KErrNone;
    }

TInt CGlVertexBuffer::UseColorData()
    {
    if ( iConfig.iColorPointerSize == EColorSizeNone ||
         iConfig.iColorPointerGlType == EColorTypeNone ||
         !iColorArrayEnabled )
        {
        glDisableClientState( GL_COLOR_ARRAY );
        return KErrNotSupported;
        }
    
    glEnableClientState( GL_COLOR_ARRAY );
    
    TInt offset = iConfig.iVertexPointerSize * iConfig.iVertexPointerType + 
                  iConfig.iNormalPointerSize * iConfig.iNormalPointerType;

    glColorPointer( iConfig.iColorPointerSize, iConfig.iColorPointerGlType, iVertexDataSize, (GLvoid*) offset );
    
    return KErrNone;
    }

TInt CGlVertexBuffer::UsePointSizeData()
    { // todo: add compilation flat to compile with opengl version 1.0 or 1.1
#ifndef GL_OES_VERSION_1_1
    return KErrNotSupported;
#else
    if ( iConfig.iPointSizePointerSize == EPointSizeNone ||
         iConfig.iPointSizePointerGlType == EPointSizeTypeNone ||
         !iPointSizeArrayEnabled )
        {
        glDisableClientState( GL_POINT_SIZE_ARRAY_OES );
        return KErrNotSupported;
        }
    
    glEnableClientState( GL_POINT_SIZE_ARRAY_OES );
    
    TInt offset = iConfig.iVertexPointerSize * iConfig.iVertexPointerType + 
                  iConfig.iNormalPointerSize * iConfig.iNormalPointerType +
                  iConfig.iColorPointerSize * iConfig.iColorPointerType;

    glPointSizePointerOES( iConfig.iPointSizePointerGlType, iVertexDataSize, (GLvoid*) offset );

    return KErrNone;
#endif
    }

TInt CGlVertexBuffer::UseTexCoordData()
    {
    if ( iConfig.iTexCoordPointerSize == ETexCoordSizeNone ||
         iConfig.iTexCoordPointerGlType == ETexCoordTypeNone )
        {
        glDisableClientState( GL_TEXTURE_COORD_ARRAY );
        return KErrNotSupported;
        }
    
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    
    TInt disabledTex = 0;
    for ( TInt i = 0; i < iConfig.iTexCoordPointerCount; i++ )
        {
        if ( !iTexCoordArrayEnabled[ i ] )
            {
            disabledTex++;
            continue;
            }

        TInt offset = iConfig.iVertexPointerSize * iConfig.iVertexPointerType +
                      iConfig.iNormalPointerSize * iConfig.iNormalPointerType +
                      iConfig.iColorPointerSize * iConfig.iColorPointerType +
                      iConfig.iPointSizePointerSize * iConfig.iPointSizePointerType +
                      i * iConfig.iTexCoordPointerSize * iConfig.iTexCoordPointerType;

        glClientActiveTexture( GL_TEXTURE0 + i );
        glTexCoordPointer( iConfig.iTexCoordPointerSize, iConfig.iTexCoordPointerGlType, iVertexDataSize, (GLvoid*) offset );
        }
    
    if ( disabledTex == iConfig.iTexCoordPointerCount )
        {
        glDisableClientState( GL_TEXTURE_COORD_ARRAY );
        return KErrNotSupported;
        }
        
    return KErrNone;
    }

TInt CGlVertexBuffer::UseIndicesData()
    {
    if ( iConfig.iIndicesGlType == EIndicesTypeNone ||
         !iIndicesArrayEnabled )
        {
        return KErrNotSupported;
        }
    
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, iBuffersIds[ KIndicesBufferIdx ] );
    
    return KErrNone;
    }

void CGlVertexBuffer::UseData()
    {
    glBindBuffer( GL_ARRAY_BUFFER, iBuffersIds[ KDataBufferIdx ] );

    UseVertexData();
    UseNormalData();
    UseColorData();
    UsePointSizeData();
    UseTexCoordData();
    UseIndicesData();
    }

void CGlVertexBuffer::UseCleanup()
    {
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    }

