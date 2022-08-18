/*
 ============================================================================
 Name		: GlVertexArray.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CGlVertexArray declaration
 ============================================================================
 */

#ifndef GLVERTEXARRAY_H
#define GLVERTEXARRAY_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <gles/gl.h>
#include <SGEGl/GlVertexArrayConfig.h>
#include <SGEGl/GlMultitexture.h>

// CLASS DECLARATION


/**
 *  CGlVertexArray
 * 
 */
class CGlVertexArray : public CBase
    {
    
    
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CGlVertexArray();

    /**
     * Two-phased constructor.
     */
    static CGlVertexArray* NewL( TGlVertexArrayConfig aConfig, TInt aVertexCount );

    /**
     * Two-phased constructor.
     */
    static CGlVertexArray* NewLC( TGlVertexArrayConfig aConfig, TInt aVertexCount );

    /*
     * aData is owned by this object and shouldn't be used by caller anymore
     */
    TInt SetData( TAny *aData, TInt aSize, TBool aTakeOwnership = ETrue );

    TInt SetVertexData( TAny *aData, TInt aSize );

    TInt SetNormalData( TAny *aData, TInt aSize );

    TInt SetColorData( TAny *aData, TInt aSize );

    TInt SetPointSizeData( TAny *aData, TInt aSize );

    TInt SetTexCoordData( TInt aNum, TAny *aData, TInt aSize );

    /*
     * aIndices is owned by this object and shouldn't be used by caller anymore
     */
    TInt SetIndices( TAny *aIndices, TInt aSize, TBool aTakeOwnership = ETrue );

    virtual void UseData();

    virtual void UseCleanup();

    /*
     * @param aType GL primitive type: GL_POINTS, GL_LINE_STRIP, 
     *              GL_LINE_LOOP, GL_LINES, GL_TRIANGLE_STRIP, 
     *              GL_TRIANGLE_FAN, GL_TRIANGLES
     */
    void Draw( GLenum aType );

    void Redraw( GLenum aType );
    
    void VertexArrayEnabled( TBool aEnabled );
    
    void NormalArrayEnabled( TBool aEnabled );
    
    void ColorArrayEnabled( TBool aEnabled );
    
    void PointSizeArrayEnabled( TBool aEnabled );
    
    void IndicesArrayEnabled( TBool aEnabled );

    void TexCoordArrayEnabled( TInt aNum, TBool aEnabled );
    
    TInt UsedArrayCount();

    TInt SetVertexCount( TUint aVertexCount );
    
protected:

    /**
     * Constructor for performing 1st stage construction
     */
    CGlVertexArray( TGlVertexArrayConfig aConfig, TInt aVertexCount );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();

    void FreeData();

    TInt CreateData();

    TInt CreateIndices();
    
    void CalculateVertexDataSize();    

    void CopyMemory( TAny *aDestination, TInt aDestinationOffset, TAny *aSource, TInt aSourceOffset );

    virtual TInt UseVertexData();

    virtual TInt UseNormalData();
    
    virtual TInt UseColorData();

    virtual TInt UsePointSizeData();

    virtual TInt UseTexCoordData();
    
protected:
    
    TGlVertexArrayConfig iConfig;

    TInt iVertexDataSize;

    TInt iVertexCount;
    
    TAny *iData;
    
    TAny *iIndices;
    
    TBool iDataOwnership;

    TBool iIndicesOwnership;
    
    TBool iVertexArrayEnabled;

    TBool iNormalArrayEnabled;

    TBool iColorArrayEnabled;
    
    TBool iPointSizeArrayEnabled;

    TBool iIndicesArrayEnabled;

    TFixedArray< TBool, KGlMaxTextureUnits > iTexCoordArrayEnabled;
    };

#endif // GLVERTEXARRAY_H
