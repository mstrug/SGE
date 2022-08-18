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
#include "MyEngine.h"


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

    CMyEngine *engine = CMyEngine::NewL( *this, iCommonObjects );
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


void CMyEngineController::EngineClose( TUint /*aId*/ )
    {
    iObserver.EngineControllerClose( KErrNone );
    }

