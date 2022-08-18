/*
 ============================================================================
 Name		: MyEngineController.cpp
 Author	  : 
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : 
 ============================================================================
 */

#include "MyEngineController.h"
#include <SGEEngineControllerObserver.h>
#include "MyEngine1.h"


CMyEngineController* CMyEngineController::NewL( MSGEEngineControllerObserver& aObserver, TSize aSize )
    {
    CMyEngineController *self = new (ELeave) CMyEngineController( aObserver, aSize );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

void CMyEngineController::ConstructL()
    {
    BaseConstructL();

    CSGEEngine* engine = CMyEngine1::NewL( *this, iCommonObjects );
    TInt err = AddEngine( engine );
    if ( err != KErrNone )
        {
        delete engine;
        iObserver.EngineControllerClose( err );
        return;
        }
    SetCurrentEngine( engine->Id() );
    }

CMyEngineController::CMyEngineController( MSGEEngineControllerObserver& aObserver, TSize aSize )
        : CSGEEngineController( aObserver, aSize )
    {
    }


void CMyEngineController::EngineClose( TUint /*aId*/, TInt aError )
    {
    iObserver.EngineControllerClose( aError );
    }

void CMyEngineController::EngineSwitch( TUint /*aId*/ )
    {
    }

void CMyEngineController::Engine1Switch()
    {
    if ( !EngineAdded( KMyEngine2Id ) )
        {
        CSGEEngine* engine = NULL;
        TRAPD( err, engine = CMyEngine2::NewL( *this, iCommonObjects ) );
        if ( err != KErrNone )
            {
            iObserver.EngineControllerClose( err );
            return;
            }
        err = AddEngine( engine );
        if ( err != KErrNone )
            {
            delete engine;
            iObserver.EngineControllerClose( err );
            return;
            }
        }
    
    SetCurrentEngine( KMyEngine2Id );
    }

void CMyEngineController::Engine2Switch()
    {
    SetCurrentEngine( KMyEngine1Id );
    }
