/*
 ============================================================================
 Name		: GlObjectRect.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CGlObjectRect declaration
 ============================================================================
 */

#ifndef GLOBJECTRECT_H
#define GLOBJECTRECT_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <gles/gl.h>
#include <SGEGl/GlObject.h>

// FORWARD DECLARATIONS

class CGlVertexArray;
class TGlPolygonX;

// CLASS DECLARATION

const TInt KGlObjectRectVertexCount = 8;

/**
 *  CGlObjectRect
 * 
 */
class CGlObjectRect : public CGlObject
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CGlObjectRect();

    /**
     * Two-phased constructor.
     */
    static CGlObjectRect* NewL();

    /**
     * Two-phased constructor.
     */
    static CGlObjectRect* NewLC();

    TGlPolygonX& PrimitiveType() const;
    
    // default size is 1x1
    void SetRectSize( TSize aSize );
    
private:

    /**
     * Constructor for performing 1st stage construction
     */
    CGlObjectRect();

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();
    
private:
    
    CGlVertexArray* iDataRect;

    TFixedArray<GLshort, KGlObjectRectVertexCount> iVertexData;

    TSize iCurrentSize;
    
    };

#endif // GLOBJECTRECT_H
