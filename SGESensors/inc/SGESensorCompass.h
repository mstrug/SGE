/*
 ============================================================================
 Name		: SGESensorCompass.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGESensorCompass declaration
 ============================================================================
 */

#ifndef SGESENSORCOMPASS_H
#define SGESENSORCOMPASS_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <SGESensors/SGESensorBase.h>


// CLASS DECLARATION

class MSGESensorCompassObserver
    {
public:
    
    virtual void CompassOrientation( TInt aDegrees ) = 0;
    
    };

/**
 *  CSGESensorCompass
 * 
 */
class CSGESensorCompass : public CSGESensorBase
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    IMPORT_C ~CSGESensorCompass();

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CSGESensorCompass* NewL( MSGESensorCompassObserver &aObserver );

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CSGESensorCompass* NewLC( MSGESensorCompassObserver &aObserver );
    
    IMPORT_C void StartListeningL();

    // default value: 19400
    IMPORT_C void SetTreshold( TUint aValue );

    // default value: 0
    IMPORT_C void SetDuration( TTimeIntervalMicroSeconds aTime );

    // default value: 20
    IMPORT_C void SetReadRepeats( TUint aValue );
    
private:

    /**
     * Constructor for performing 1st stage construction
     */
    CSGESensorCompass( MSGESensorCompassObserver &aObserver );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();

protected: // from MSensrvDataListener
    
    void DataReceived( CSensrvChannel& aChannel, TInt aCount, TInt aDataLost );

    void DataError( CSensrvChannel& aChannel, TSensrvErrorSeverity aError );

private: // data

    MSGESensorCompassObserver &iObserver;

    TUint iTreshold;

    TInt iShakeReadsCounter;

    TInt iShakeReadsMax;
    
    TBool iShakeStarted;
    
    TBool iFirstRead;
    
    TTimeIntervalMicroSeconds iDuration;
    
    TTime iTime;
    
    TInt iAxisX;
    
    TInt iAxisY;
    
    TInt iAxisZ;
    
//    RFs iFs;
//    RFile iFile;
    };

#endif // SGESENSORACCELEROMETER_H
