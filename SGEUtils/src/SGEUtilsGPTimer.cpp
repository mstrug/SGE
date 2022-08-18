/*
 ============================================================================
 Name		: SGEUtilsGPTimer.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEUtilsGPTimer implementation
 ============================================================================
 */

#include "SGEUtilsGPTimer.h"
#include "SGEUtilsGPTimerObserver.h"


// -----------------------------------------------------------------------------
// CSGEUtilsGPTimer::CSGEUtilsGPTimer()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEUtilsGPTimer::CSGEUtilsGPTimer( MSGEUtilsGPTimerObserver& aObserver ) : iObserver( &aObserver )
	{
	}

// -----------------------------------------------------------------------------
// CSGEUtilsGPTimer::~CSGEUtilsGPTimer()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEUtilsGPTimer::~CSGEUtilsGPTimer()
	{
    delete iTimer;
	}

// -----------------------------------------------------------------------------
// CSGEUtilsGPTimer::NewLC()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEUtilsGPTimer* CSGEUtilsGPTimer::NewLC( MSGEUtilsGPTimerObserver& aObserver )
    {
	CSGEUtilsGPTimer* self = new ( ELeave ) CSGEUtilsGPTimer( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CSGEUtilsGPTimer::NewL()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEUtilsGPTimer* CSGEUtilsGPTimer::NewL( MSGEUtilsGPTimerObserver& aObserver )
    {
	CSGEUtilsGPTimer* self = CSGEUtilsGPTimer::NewLC( aObserver );
    CleanupStack::Pop(); // self;
    return self;
    }

// -----------------------------------------------------------------------------
// CSGEUtilsGPTimer::ConstructL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEUtilsGPTimer::ConstructL()
    {
	iTimer = CPeriodic::NewL( CActive::EPriorityStandard );
    }

// -----------------------------------------------------------------------------
// CSGEUtilsGPTimer::SetObserver()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEUtilsGPTimer::SetObserver( MSGEUtilsGPTimerObserver& aObserver )
	{
	iObserver = &aObserver;
	}

// -----------------------------------------------------------------------------
// CSGEUtilsGPTimer::Start()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEUtilsGPTimer::Start( TTimeIntervalMicroSeconds32 aTimeout, TBool aLooped )
	{
	iLooped = aLooped;
	iTimeout = aTimeout;
	if ( !iStarted )
		{
	    TCallBack cb( TimerCallback, this );
		iTimer->Start( aTimeout, aTimeout, cb );
		iStarted = ETrue;
		}
	}

// -----------------------------------------------------------------------------
// CSGEUtilsGPTimer::Stop()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEUtilsGPTimer::Stop()
	{
    if ( iStarted )
		{
    	iTimer->Cancel();
    	iStarted = EFalse;
		}
	}

// -----------------------------------------------------------------------------
// CSGEUtilsGPTimer::DoEvent()
// -----------------------------------------------------------------------------
//
TInt CSGEUtilsGPTimer::DoEvent()
    {
    if ( !iLooped )
        {
        iTimer->Cancel();
        iStarted = EFalse;
        }
    if ( iObserver )
        {
        iObserver->GPTimerEvent( this );
        }
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CSGEUtilsGPTimer::TimerCallback()
// -----------------------------------------------------------------------------
//
TInt CSGEUtilsGPTimer::TimerCallback( TAny* aParam )
	{
    CSGEUtilsGPTimer* self = (CSGEUtilsGPTimer*) aParam;
    
    self->DoEvent();
	
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CSGEUtilsGPTimer::IsTimerStarted()
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSGEUtilsGPTimer::IsTimerStarted() const
	{
	return iStarted;
	}

// -----------------------------------------------------------------------------
// CSGEUtilsGPTimer::IsTimerStarted()
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSGEUtilsGPTimer::IsTimerLooped() const
	{
	return iLooped;
	}

// -----------------------------------------------------------------------------
// CSGEUtilsGPTimer::TimerTimeout()
// -----------------------------------------------------------------------------
//
EXPORT_C TTimeIntervalMicroSeconds32 CSGEUtilsGPTimer::TimerTimeout() const
	{
	return iTimeout;
	}




