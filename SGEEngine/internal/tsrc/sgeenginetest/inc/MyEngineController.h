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

class CMyEngine;


class CMyEngineController : public CSGEEngineController
    {
public:

    static CMyEngineController* NewL( MSGEEngineControllerObserver& aObserver, TSize aSize );

private:

    void ConstructL();
    
    CMyEngineController( MSGEEngineControllerObserver& aObserver, TSize aSize );

private: // from MSGEEngineObserver

    void EngineClose( TUint aId );

private: // data

    };

#endif // CMYENGINECONTROLLER_H_
