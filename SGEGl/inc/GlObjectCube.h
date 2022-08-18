/*
 ============================================================================
 Name		: GlObjectCube.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CGlObjectCube declaration
 ============================================================================
 */

#ifndef GLOBJECTCUBE_H
#define GLOBJECTCUBE_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <gles/gl.h>
#include <SGEGl/GlObject.h>

// FORWARD DECLARATIONS

class CGlVertexArray;

// CLASS DECLARATION

/**
 *  CGlObjectCube
 * 
 */
class CGlObjectCube : public CGlObject
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CGlObjectCube();

    /**
     * Two-phased constructor.
     */
    static CGlObjectCube* NewL();

    /**
     * Two-phased constructor.
     */
    static CGlObjectCube* NewLC();

private:

    /**
     * Constructor for performing 1st stage construction
     */
    CGlObjectCube();

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();
    
private:
    
    CGlVertexArray* iDataCube;

    };

#endif // GLOBJECTCUBE_H
