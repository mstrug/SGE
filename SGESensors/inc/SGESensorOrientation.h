/*
 ============================================================================
 Name		: SGESensorOrientation.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGESensorOrientation declaration
 ============================================================================
 */

#ifndef SGESENSORORIENTATION_H
#define SGESENSORORIENTATION_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <sensrvorientationsensor.h>
#include "SGESensorBase.h"

// CLASS DECLARATION

/**
 *  CSGESensorOrientation
 * 
 */
class CSGESensorOrientation : public CSGESensorBase
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    IMPORT_C ~CSGESensorOrientation();

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CSGESensorOrientation* NewL();

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CSGESensorOrientation* NewLC();

    IMPORT_C TSensrvOrientationData::TSensrvDeviceOrientation Orientation() const;

private:

    /**
     * Constructor for performing 1st stage construction
     */
    CSGESensorOrientation();

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();

private: // from MSensrvDataListener
    
    void DataReceived( CSensrvChannel& aChannel, TInt aCount, TInt aDataLost );
        
    void DataError( CSensrvChannel& aChannel, TSensrvErrorSeverity aError );
    
private: // data
    
    TSensrvOrientationData::TSensrvDeviceOrientation iOrientation;

    };

#endif // SGESENSORORIENTATION_H
