/*
 ============================================================================
 Name		: SGEStringFileMemory.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEStringFileMemory declaration
 ============================================================================
 */

#ifndef SGESTRINGFILEMEMORY_H
#define SGESTRINGFILEMEMORY_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <f32file.h>
#include <BaRsc2.h>
#include "SGEStringFileBase.h"

// FORWARD DECLARATIONS
class CSGEFile;

// CLASS DECLARATION
/**
 *  CSGEStringFileMemory
 * 
 */
NONSHARABLE_CLASS( CSGEStringFileMemory ) : public CSGEStringFileBase
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CSGEStringFileMemory();

    /**
     * Two-phased constructor.
     */
    static CSGEStringFileMemory* NewL( RFs &aFS, CSGEFile &aSGEFile, const TDesC& aFileName );

    /**
     * Two-phased constructor.
     */
    static CSGEStringFileMemory* NewLC( RFs &aFS, CSGEFile &aSGEFile, const TDesC& aFileName );

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
    CSGEStringFileMemory( RFs &aFS );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL( CSGEFile &aSGEFile, const TDesC& aFileName );
    
private: // data
    
    RFs &iFs; // not owned
    
    RBuf8 iFileData;

    CResourceFile *iFile; // owned
    
    };

#endif // SGESTRINGFILEMEMORY_H
