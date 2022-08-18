/*
 ============================================================================
 Name		: SGESensorShake.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGESensorShake implementation
 ============================================================================
 */

#ifndef __S60_3X__

#include "SGESensorShake.h"
#include <sensrvaccelerometersensor.h>

// range: -128 to 127 (KSensrvPropIdMeasureRange)
const TUint KDefaultTreshold = 110 * 110 + 80 * 80 + 30 * 30;
const TInt KMinShakeCounter = 20;


CSGESensorShake::CSGESensorShake( MSGESensorShakeObserver &aObserver ) : iObserver( aObserver ),
    iTreshold( KDefaultTreshold ), iShakeReadsMax( KMinShakeCounter )
    {
    // No implementation required
    }

EXPORT_C CSGESensorShake::~CSGESensorShake()
    {
    }

EXPORT_C CSGESensorShake* CSGESensorShake::NewLC( MSGESensorShakeObserver &aObserver )
    {
    CSGESensorShake* self = new ( ELeave ) CSGESensorShake( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

EXPORT_C CSGESensorShake* CSGESensorShake::NewL( MSGESensorShakeObserver &aObserver )
    {
    CSGESensorShake* self = CSGESensorShake::NewLC( aObserver );
    CleanupStack::Pop(); // self;
    return self;
    }

void CSGESensorShake::ConstructL()
    {
    BaseConstructL( KSensrvChannelTypeIdAccelerometerXYZAxisData );
    }

EXPORT_C void CSGESensorShake::StartListeningL()
    {
    iSensrvChannel->StartDataListeningL( this, 1, 1, 1 );
    
    iFirstRead = ETrue;
    }

EXPORT_C void CSGESensorShake::SetTreshold( TUint aValue )
    {
    iTreshold = aValue;
    }

EXPORT_C void CSGESensorShake::SetDuration( TTimeIntervalMicroSeconds aTime )
    {
    iDuration = aTime;
    }

EXPORT_C void CSGESensorShake::SetReadRepeats( TUint aValue )
    {
    iShakeReadsMax = aValue;
    }

void CSGESensorShake::DataReceived( CSensrvChannel& aChannel, TInt aCount, TInt aDataLost )
    {
    TSensrvAccelerometerAxisData accData;
    TPckg<TSensrvAccelerometerAxisData> accPackage( accData );
    aChannel.GetData( accPackage );
    
    if ( accData.iAxisX > 127 || accData.iAxisX < -128 ||
         accData.iAxisY > 127 || accData.iAxisY < -128 ||
         accData.iAxisZ > 127 || accData.iAxisZ < -128 )
        {
        return;
        }
            
    if ( !iFirstRead )
        {
        TUint val = ( accData.iAxisX - iAxisX ) * ( accData.iAxisX - iAxisX ) +
                    ( accData.iAxisY - iAxisY ) * ( accData.iAxisY - iAxisY ) +
                    ( accData.iAxisZ - iAxisZ ) * ( accData.iAxisZ - iAxisZ );
        
        if ( val > iTreshold )
            {
            if ( !iShakeStarted )
                {
                iObserver.ShakeStarted();
                iShakeStarted = ETrue;
                iTime.UniversalTime();
                }
            iShakeReadsCounter = 0;
            }
        
        if ( iShakeStarted )
            {
            iShakeReadsCounter++;
            
            if ( val < iTreshold && iShakeReadsCounter > iShakeReadsMax )
                {
                TTime now;
                now.UniversalTime();
                iObserver.ShakeEnded( now.MicroSecondsFrom( iTime ) >= iDuration );
                iShakeStarted = EFalse;
                iShakeReadsCounter = 0;
                }
            }
        }
    else
        {
        iFirstRead = EFalse;
        }
    
    iAxisX = accData.iAxisX;
    iAxisY = accData.iAxisY;
    iAxisZ = accData.iAxisZ;
    }

void CSGESensorShake::DataError( CSensrvChannel& aChannel, TSensrvErrorSeverity aError )
    {
    
    }


#endif // __S60_3X__

// End of file
