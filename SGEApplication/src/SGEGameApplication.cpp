/*
 ============================================================================
  Name        : SGEGameApplication.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#include "SGEGameApplication.h"
#include "SGEGameAppUi.h"
#include <HAL.H>


const TUint KDefaultMaxFPS = 30;
const TUint KOneSecond = 1000000;   // us 

// -----------------------------------------------------------------------------
// CSGEGameApplication::~CSGEGameApplication()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEGameApplication::CSGEGameApplication() : iMaxFPS( KDefaultMaxFPS )
    {
    }

// -----------------------------------------------------------------------------
// CSGEGameApplication::~CSGEGameApplication()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEGameApplication::~CSGEGameApplication()
    {
    if ( iGameLoop )
        {
        iGameLoop->Cancel();
        delete iGameLoop;
        }
    }

// -----------------------------------------------------------------------------
// CSGEGameApplication::InitL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEGameApplication::InitL()
    {
    User::LeaveIfError( HAL::Get( HAL::ENanoTickPeriod, iNanokernelTickPeriod ) );
    iNanokernelTickCount = User::NTickCount();
    
    iGameLoop = CPeriodic::NewL( CActive::EPriorityIdle );
    }

// -----------------------------------------------------------------------------
// CSGEGameApplication::BlockKeys()
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSGEGameApplication::BlockKeys() const
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CSGEGameApplication::GoBackgroundOnEndKey()
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSGEGameApplication::GoBackgroundOnEndKey() const
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CSGEGameApplication::StopGameLoop()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEGameApplication::StopGameLoop()
    {
    iGameLoop->Cancel();
    }

// -----------------------------------------------------------------------------
// CSGEGameApplication::SetGameLoopInterval()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEGameApplication::StartGameLoop()
    {
    const TUint KOne = 1;   // us 
    
    iGameLoop->Cancel();
    TCallBack cb( GameLoopEvent, this );
    
    TUint interval;
    if ( iMaxFPS == 0 )
        {
        interval = KOne;
        }
    else
        {
        interval = KOneSecond / iMaxFPS;
        }
    
    iGameLoop->Start( KOne, interval, cb );
    
    iFPSTime = 0;
    iFPSCounter = 0;
    iFPS = 0;
    iNanokernelTickCount = User::NTickCount();
    }

// -----------------------------------------------------------------------------
// CSGEGameApplication::SetGameLoopMaxFPS()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEGameApplication::SetGameLoopMaxFPS( TUint aMaxFPS )
    {
    iMaxFPS = aMaxFPS;
    }

// -----------------------------------------------------------------------------
// CSGEGameApplication::GameLoopMaxFPS()
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CSGEGameApplication::GameLoopMaxFPS() const
    {
    return iMaxFPS;
    }

// -----------------------------------------------------------------------------
// CSGEGameApplication::GameLoopFPS()
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CSGEGameApplication::GameLoopFPS() const
    {
    return iFPS;
    }

// -----------------------------------------------------------------------------
// CSGEGameApplication::GameLoopFPSCounter()
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CSGEGameApplication::GameLoopFPSCounter() const
    {
    return iFPSCounter;
    }

// -----------------------------------------------------------------------------
// CSGEGameApplication::GameLoopStarted()
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSGEGameApplication::GameLoopStarted()
    {
    return iGameLoop->IsActive();
    }

// -----------------------------------------------------------------------------
// CSGEGameApplication::HandleForegroundEventL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEGameApplication::HandleForegroundEventL( TBool aForeground )
    {
    if ( aForeground )
        {
        if ( !iGameLoop->IsActive() )
            {
            StartGameLoop();
            }
        }
    else
        {
        StopGameLoop();
        }
    }

// -----------------------------------------------------------------------------
// CSGEGameApplication::GameLoopEvent()
// -----------------------------------------------------------------------------
//
TInt CSGEGameApplication::GameLoopEvent( TAny *aSelf )
    {
    CSGEGameApplication *self = static_cast<CSGEGameApplication*>( aSelf );
    self->GameLoopEvent();
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CSGEGameApplication::CalculateGameLoopInterval()
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds32 CSGEGameApplication::CalculateGameLoopInterval()
    {
    TUint32 count = User::NTickCount();
    TTimeIntervalMicroSeconds32 ret = ( count - iNanokernelTickCount ) * iNanokernelTickPeriod;
    iNanokernelTickCount = count;
    return ret;
    }

// -----------------------------------------------------------------------------
// CSGEGameApplication::GameLoopEvent()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEGameApplication::GameLoopEvent()
    {
    TTimeIntervalMicroSeconds32 time = CalculateGameLoopInterval();
    
    iFPSCounter++;
    iFPSTime += time.Int();
    if ( iFPSTime >= KOneSecond )
        {
        iFPSTime -= KOneSecond;
        iFPS = iFPSCounter;
        iFPSCounter = 0;
        }
    
    DoGameLoopEvent( time );
    }

// -----------------------------------------------------------------------------
// CSGEGameApplication::CreateAppUiL()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGESystemAppUi* CSGEGameApplication::CreateAppUiL()
    {
    iAppUi = new (ELeave) CSGEGameAppUi( *this );
    return iAppUi;
    }


// End of file
