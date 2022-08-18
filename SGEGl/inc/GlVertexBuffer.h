/*
 ============================================================================
 Name		: GlVertexBuffer.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CGlVertexBuffer declaration
 ============================================================================
 */

#ifndef GLVERTEXBUFFER_H
#define GLVERTEXBUFFER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <SGEGl/GlVertexArray.h>

// CLASS DECLARATION

const TInt KBaseBuffersCount = 2;

/**
 *  CGlVertexBuffer
 * 
 */
class CGlVertexBuffer : public CGlVertexArray
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CGlVertexBuffer();

    /**
     * Two-phased constructor.
     */
    static CGlVertexBuffer* NewL( TGlVertexArrayConfig aConfig, TInt aVertexCount );

    /**
     * Two-phased constructor.
     */
    static CGlVertexBuffer* NewLC( TGlVertexArrayConfig aConfig, TInt aVertexCount );

    TInt CreateVertexBuffer( GLenum aUsage = GL_STATIC_DRAW );
    
    void UseData();

    void UseCleanup();

private:

    /**
     * Constructor for performing 1st stage construction
     */
    CGlVertexBuffer( TGlVertexArrayConfig aConfig, TInt aVertexCount );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();

    void GenerateBuffersIds();
    
    void DeleteBuffers();
    
    TInt UseVertexData();

    TInt UseNormalData();
    
    TInt UseColorData();

    TInt UsePointSizeData();

    TInt UseTexCoordData();

    TInt UseIndicesData();
    
private:
    
    TFixedArray< GLuint, KBaseBuffersCount > iBuffersIds;
    
    };

#endif // GLVERTEXBUFFER_H
