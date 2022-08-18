/*
 ============================================================================
 Name		: GlMaterialManager.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CGlMaterialManager declaration
 ============================================================================
 */

#ifndef GLMATERIALMANAGER_H
#define GLMATERIALMANAGER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

// CLASS DECLARATION

/**
 *  CGlMaterialManager
 * 
 */
class CGlMaterialManager : public CBase
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CGlMaterialManager();

    /**
     * Two-phased constructor.
     */
    static CGlMaterialManager* NewL();

    /**
     * Two-phased constructor.
     */
    static CGlMaterialManager* NewLC();

private:

    /**
     * Constructor for performing 1st stage construction
     */
    CGlMaterialManager();

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();

    };

#endif // GLMATERIALMANAGER_H
