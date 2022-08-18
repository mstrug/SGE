/*
 ============================================================================
 Name		: SGESensorShake.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGESensorShake declaration
 ============================================================================
 */

#ifndef SGESENSORSHAKE_H
#define SGESENSORSHAKE_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <SGESensors/SGESensorBase.h>


// CLASS DECLARATION

class MSGESensorShakeObserver
    {
public:
    
    virtual void ShakeStarted() = 0;

    // passed ETrue when between start and stop of shake specified time passed
    virtual void ShakeEnded( TBool aPassedDuration ) = 0;
    
    };

/**
 *  CSGESensorShake
 * 
 */
class CSGESensorShake : public CSGESensorBase
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    IMPORT_C ~CSGESensorShake();

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CSGESensorShake* NewL( MSGESensorShakeObserver &aObserver );

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CSGESensorShake* NewLC( MSGESensorShakeObserver &aObserver );
    
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
    CSGESensorShake( MSGESensorShakeObserver &aObserver );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();

protected: // from MSensrvDataListener
    
    void DataReceived( CSensrvChannel& aChannel, TInt aCount, TInt aDataLost );

    void DataError( CSensrvChannel& aChannel, TSensrvErrorSeverity aError );

private: // data

    MSGESensorShakeObserver &iObserver;

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
