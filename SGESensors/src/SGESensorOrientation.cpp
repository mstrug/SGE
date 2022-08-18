/*
 ============================================================================
 Name		: SGESensorOrientation.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGESensorOrientation implementation
 ============================================================================
 */

#ifndef __S60_3X__

#include "SGESensorOrientation.h"


CSGESensorOrientation::CSGESensorOrientation()
    {
    // No implementation required
    }

EXPORT_C CSGESensorOrientation::~CSGESensorOrientation()
    {
    }

EXPORT_C CSGESensorOrientation* CSGESensorOrientation::NewLC()
    {
    CSGESensorOrientation *self = new ( ELeave ) CSGESensorOrientation();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

EXPORT_C CSGESensorOrientation* CSGESensorOrientation::NewL()
    {
    CSGESensorOrientation *self = CSGESensorOrientation::NewLC();
    CleanupStack::Pop(); // self;
    return self;
    }

void CSGESensorOrientation::ConstructL()
    {
    BaseConstructL( KSensrvChannelTypeIdOrientationData );
    }

void CSGESensorOrientation::DataReceived( CSensrvChannel& aChannel, TInt /*aCount*/, TInt /*aDataLost*/ )
    {
    TSensrvOrientationData orData;
    TPckg<TSensrvOrientationData> orPackage( orData );
    aChannel.GetData( orPackage );
    
    iOrientation = orData.iDeviceOrientation;
    }

EXPORT_C TSensrvOrientationData::TSensrvDeviceOrientation CSGESensorOrientation::Orientation() const
    {
    return iOrientation;
    }

void CSGESensorOrientation::DataError( CSensrvChannel& /*aChannel*/, TSensrvErrorSeverity /*aError*/ )
    {    
    }


#endif // __S60_3X__

// End of file
