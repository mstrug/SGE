/*
 ============================================================================
 Name		: SGEStringFileResource.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEStringFileResource declaration
 ============================================================================
 */

#ifndef SGESTRINGFILERESOURCE_H
#define SGESTRINGFILERESOURCE_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <f32file.h>
#include <BARSC.H>
#include "SGEStringFileBase.h"

// CLASS DECLARATION

/**
 *  CSGEStringFileResource
 * 
 */
NONSHARABLE_CLASS( CSGEStringFileResource ) : public CSGEStringFileBase
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CSGEStringFileResource();

    /**
     * Two-phased constructor.
     */
    static CSGEStringFileResource* NewL( RFs &aFS, const TDesC& aFileName );

    /**
     * Two-phased constructor.
     */
    static CSGEStringFileResource* NewLC( RFs &aFS, const TDesC& aFileName );

    /**
     * Function checks if aResourceId exists if not then tries to load it from file.
     */
    TPtrC GetStringL( TInt aResourceId );

    /**
     * Function checks if aResourceId exists if not then returns KNullDesC.
     */
    TPtrC GetString( TInt aResourceId );

private:

    /**
     * Constructor for performing 1st stage construction
     */
    CSGEStringFileResource( RFs &aFS );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL( const TDesC& aFileName );
    
private: // data
    
    RFs &iFs; // not owned
    
    RResourceFile iFile;
    
    };

#endif // SGESTRINGFILERESOURCE_H
