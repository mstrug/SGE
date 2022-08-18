/*
 ============================================================================
 Name		: SGESensorRotation.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGESensorRotation declaration
 ============================================================================
 */

#ifndef SGESENSORROTATION_H
#define SGESENSORROTATION_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "SGESensorBase.h"
#include "SGECircularBuffer.h"

// CLASS DECLARATION

const TInt KSGESensorRotationDefaultBufferLen = 16;

/**
 *  CSGESensorRotation
 * 
 */
class CSGESensorRotation : public CSGESensorBase
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    IMPORT_C ~CSGESensorRotation();

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CSGESensorRotation* NewL( TInt aBufferLength = KSGESensorRotationDefaultBufferLen );

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CSGESensorRotation* NewLC( TInt aBufferLength = KSGESensorRotationDefaultBufferLen );

    IMPORT_C TInt AxisX() const;
    
    IMPORT_C TInt AxisY() const;
    
    IMPORT_C TInt AxisZ() const;
    
    IMPORT_C TInt SetLowPassFilter( TBool aUse, TInt aDt = 1, TInt aRc = 4 );

private:

    /**
     * Constructor for performing 1st stage construction
     */
    CSGESensorRotation();

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL( TInt aBufferLength );

private: // from MSensrvDataListener
    
    void DataReceived( CSensrvChannel& aChannel, TInt aCount, TInt aDataLost );
        
    void DataError( CSensrvChannel& aChannel, TSensrvErrorSeverity aError );
    
private: // data
    
    CSGECircularBuffer< TInt > *iAxisX;
    
    CSGECircularBuffer< TInt > *iAxisY;
    
    CSGECircularBuffer< TInt > *iAxisZ;

    };

#endif // SGESENSORROTATION_H
