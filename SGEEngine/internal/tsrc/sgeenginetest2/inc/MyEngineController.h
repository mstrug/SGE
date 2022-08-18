/*
 * MyEngineController.h
 *
 *  Created on: 2009-10-14
 *      Author: michal.strug
 *   Copyright: Copyright (c) 2022 Michal Strug. All rights reserved.
 */

#ifndef CMYENGINECONTROLLER_H_
#define CMYENGINECONTROLLER_H_

#include <SGEEngineController.h>
#include <SGEEngineObserver.h>
#include "MyEngine1.h"
#include "MyEngine2.h"


class CMyEngineController : public CSGEEngineController, public MSGEEngine1Observer, public MSGEEngine2Observer
    {
public:

    static CMyEngineController* NewL( MSGEEngineControllerObserver& aObserver, TSize aSize );

private:

    void ConstructL();
    
    CMyEngineController( MSGEEngineControllerObserver& aObserver, TSize aSize );

private: // from MSGEEngineObserver

    void EngineClose( TUint aId, TInt aError );

    void EngineSwitch( TUint aId );

private: // from MSGEEngine1Observer

    void Engine1Switch();

private: // from MSGEEngine2Observer

    void Engine2Switch();

private: // data

    };

#endif // CMYENGINECONTROLLER_H_
