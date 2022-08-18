/*
 ============================================================================
 Name		: SGESensorAccelerometer.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGESensorAccelerometer declaration
 ============================================================================
 */

#ifndef SGESENSORACCELEROMETER_H
#define SGESENSORACCELEROMETER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <SGESensors/SGECircularBuffer.h>
#include <SGESensors/SGESensorBase.h>

// CLASS DECLARATION

const TInt KSGESensorAccelerometerDefaultBufferLen = 16;

/**
 *  CSGESensorAccelerometer
 * 
 */
class CSGESensorAccelerometer : public CSGESensorBase
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    IMPORT_C ~CSGESensorAccelerometer();

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CSGESensorAccelerometer* NewL( TInt aBufferLength = KSGESensorAccelerometerDefaultBufferLen );

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CSGESensorAccelerometer* NewLC( TInt aBufferLength = KSGESensorAccelerometerDefaultBufferLen );

    IMPORT_C TInt AxisX() const;
    
    IMPORT_C TInt AxisY() const;
    
    IMPORT_C TInt AxisZ() const;
    
    IMPORT_C TInt SetLowPassFilter( TBool aUse, TInt aDt = 1, TInt aRc = 4 );
    
private:

    /**
     * Constructor for performing 1st stage construction
     */
    CSGESensorAccelerometer();

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL( TInt aBufferLength );

protected: // from MSensrvDataListener
    
    void DataReceived( CSensrvChannel& aChannel, TInt aCount, TInt aDataLost );
        
    void DataError( CSensrvChannel& aChannel, TSensrvErrorSeverity aError );

private: // data
    
    CSGECircularBuffer< TInt > *iAxisX;
    
    CSGECircularBuffer< TInt > *iAxisY;
    
    CSGECircularBuffer< TInt > *iAxisZ;
    
    };

#endif // SGESENSORACCELEROMETER_H
