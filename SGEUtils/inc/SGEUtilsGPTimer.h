/*
 ============================================================================
  Name        : SGEUtilsGPTimer.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#ifndef __SGEUTILSGPTIMER_H__
#define __SGEUTILSGPTIMER_H__

// INCLUDES
#include <e32std.h>
#include <e32base.h>


// FORWARD DECLARATIONS
class MSGEUtilsGPTimerObserver;

// CLASS DECLARATION
/**
 * CSGEUtilsGPTimer
 * General Purpose Timer.
 */
class CSGEUtilsGPTimer : public CBase
	{
public: // Constructors and destructor

	/**
	 * Destructor.
	 */
	IMPORT_C ~CSGEUtilsGPTimer();

	/**
	 * Two-phased constructor.
	 */
	IMPORT_C static CSGEUtilsGPTimer* NewL( MSGEUtilsGPTimerObserver& aObserver );

	/**
	 * Two-phased constructor.
	 */
	IMPORT_C static CSGEUtilsGPTimer* NewLC( MSGEUtilsGPTimerObserver& aObserver );

	IMPORT_C void SetObserver( MSGEUtilsGPTimerObserver& aObserver );

	IMPORT_C void Start( TTimeIntervalMicroSeconds32 aTimeout, TBool aLooped = EFalse );

	IMPORT_C void Stop();

	IMPORT_C TBool IsTimerStarted() const;

	IMPORT_C TBool IsTimerLooped() const;

	IMPORT_C TTimeIntervalMicroSeconds32 TimerTimeout() const;

private: // construction

	CSGEUtilsGPTimer( MSGEUtilsGPTimerObserver& aObserver );

    void ConstructL();

    TInt DoEvent();

    static TInt TimerCallback( TAny* aParam );

private: // data

    MSGEUtilsGPTimerObserver* iObserver;

    CPeriodic* iTimer;

    TBool iStarted;

    TBool iLooped;

    TTimeIntervalMicroSeconds32 iTimeout;

	};

#endif // __SGEUTILSGPTIMER_H__
