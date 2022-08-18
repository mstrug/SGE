/*
 ============================================================================
 Name		: SGESensorRotation.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGESensorRotation implementation
 ============================================================================
 */

#ifndef __S60_3X__

#include "SGESensorRotation.h"
#include <sensrvorientationsensor.h>


CSGESensorRotation::CSGESensorRotation()
    {
    // No implementation required
    }

EXPORT_C CSGESensorRotation::~CSGESensorRotation()
    {
    delete iAxisX;
    delete iAxisY;
    delete iAxisZ;
    }

EXPORT_C CSGESensorRotation* CSGESensorRotation::NewLC( TInt aBufferLength )
    {
    CSGESensorRotation *self = new ( ELeave ) CSGESensorRotation();
    CleanupStack::PushL( self );
    self->ConstructL( aBufferLength );
    return self;
    }

EXPORT_C CSGESensorRotation* CSGESensorRotation::NewL( TInt aBufferLength )
    {
    CSGESensorRotation *self = CSGESensorRotation::NewLC( aBufferLength );
    CleanupStack::Pop(); // self;
    return self;
    }

void CSGESensorRotation::ConstructL( TInt aBufferLength )
    {
    BaseConstructL( KSensrvChannelTypeIdRotationData );
    
    iAxisX = CSGECircularBuffer< TInt >::NewL( aBufferLength );
    iAxisY = CSGECircularBuffer< TInt >::NewL( aBufferLength );
    iAxisZ = CSGECircularBuffer< TInt >::NewL( aBufferLength );
    }

void CSGESensorRotation::DataReceived( CSensrvChannel& aChannel, TInt /*aCount*/, TInt /*aDataLost*/ )
    {
    TSensrvRotationData rotData;
    TPckg<TSensrvRotationData> rotPackage( rotData );
    aChannel.GetData( rotPackage );
    
    iAxisX->AddNewValue( rotData.iDeviceRotationAboutXAxis );
    iAxisY->AddNewValue( rotData.iDeviceRotationAboutYAxis );
    iAxisZ->AddNewValue( rotData.iDeviceRotationAboutZAxis );
    }

EXPORT_C TInt CSGESensorRotation::AxisX() const
    {
    return iAxisX->CurrentValue();
    }

EXPORT_C TInt CSGESensorRotation::AxisY() const
    {
    return iAxisY->CurrentValue();
    }

EXPORT_C TInt CSGESensorRotation::AxisZ() const
    {
    return iAxisZ->CurrentValue();
    }

EXPORT_C TInt CSGESensorRotation::SetLowPassFilter( TBool aUse, TInt aDt, TInt aRc )
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

void CSGESensorRotation::DataError( CSensrvChannel& /*aChannel*/, TSensrvErrorSeverity /*aError*/ )
    {    
    }


#endif // __S60_3X__

// End of file
