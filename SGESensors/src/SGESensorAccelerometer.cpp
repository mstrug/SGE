/*
 ============================================================================
 Name		: SGESensorAccelerometer.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGESensorAccelerometer implementation
 ============================================================================
 */

#ifndef __S60_3X__

#include "SGESensorAccelerometer.h"
#include <sensrvaccelerometersensor.h>


CSGESensorAccelerometer::CSGESensorAccelerometer()
    {
    // No implementation required
    }

EXPORT_C CSGESensorAccelerometer::~CSGESensorAccelerometer()
    {
    delete iAxisX;
    delete iAxisY;
    delete iAxisZ;
    }

EXPORT_C CSGESensorAccelerometer* CSGESensorAccelerometer::NewLC( TInt aBufferLength )
    {
    CSGESensorAccelerometer* self = new ( ELeave ) CSGESensorAccelerometer();
    CleanupStack::PushL( self );
    self->ConstructL( aBufferLength );
    return self;
    }

EXPORT_C CSGESensorAccelerometer* CSGESensorAccelerometer::NewL( TInt aBufferLength )
    {
    CSGESensorAccelerometer* self = CSGESensorAccelerometer::NewLC( aBufferLength );
    CleanupStack::Pop(); // self;
    return self;
    }

void CSGESensorAccelerometer::ConstructL( TInt aBufferLength )
    {
    BaseConstructL( KSensrvChannelTypeIdAccelerometerXYZAxisData );

    iAxisX = CSGECircularBuffer< TInt >::NewL( aBufferLength );
    iAxisY = CSGECircularBuffer< TInt >::NewL( aBufferLength );
    iAxisZ = CSGECircularBuffer< TInt >::NewL( aBufferLength );
    }

void CSGESensorAccelerometer::DataReceived( CSensrvChannel& aChannel, TInt /*aCount*/, TInt /*aDataLost*/ )
    {
    TSensrvAccelerometerAxisData accData;
    TPckg<TSensrvAccelerometerAxisData> accPackage( accData );
    aChannel.GetData( accPackage );
    
    iAxisX->AddNewValue( accData.iAxisX );
    iAxisY->AddNewValue( accData.iAxisY );
    iAxisZ->AddNewValue( accData.iAxisZ );
    }

EXPORT_C TInt CSGESensorAccelerometer::AxisX() const
    {
    return iAxisX->CurrentValue();
    }

EXPORT_C TInt CSGESensorAccelerometer::AxisY() const
    {
    return iAxisY->CurrentValue();
    }

EXPORT_C TInt CSGESensorAccelerometer::AxisZ() const
    {
    return iAxisZ->CurrentValue();
    }

EXPORT_C TInt CSGESensorAccelerometer::SetLowPassFilter( TBool aUse, TInt aDt, TInt aRc )
    {
    if ( !aUse )
        {
        iAxisX->TurnOffLowPassFilter();
        iAxisY->TurnOffLowPassFilter();
        iAxisZ->TurnOffLowPassFilter();
        return KErrNone;
        }
    else
        {
        TInt err;
        err = iAxisX->UseLowPassFilter( aDt, aRc );
        err = iAxisY->UseLowPassFilter( aDt, aRc );
        err = iAxisZ->UseLowPassFilter( aDt, aRc );
        return err;
        }
    }

void CSGESensorAccelerometer::DataError( CSensrvChannel& /*aChannel*/, TSensrvErrorSeverity /*aError*/ )
    {    
    }


#endif // __S60_3X__

// End of file
