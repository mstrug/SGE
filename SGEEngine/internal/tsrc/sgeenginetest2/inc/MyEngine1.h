/*
 * MyEngine1.h
 *
 *  Created on: 2009-10-14
 *      Author: michal.strug
 *   Copyright: Copyright (c) 2022 Michal Strug. All rights reserved.
 */

#ifndef CMYENGINE1_H_
#define CMYENGINE1_H_

#include <SGEEngine.h>
#include <SGEEngineObserver.h>

const TUint KMyEngine1Id = 1;

class MSGEEngine1Observer : public MSGEEngineObserver
    {
public:
    virtual void Engine1Switch() = 0;
    };


class CMyEngine1 : public CSGEEngine
    {
public:

    static CMyEngine1* NewL( MSGEEngine1Observer& aObserver, CSGEEngineCommonObjects *aCommonObjects );

private:

    void ConstructL();
    
    CMyEngine1( MSGEEngine1Observer& aObserver, CSGEEngineCommonObjects *aCommonObjects );

public: // from base class

    TUint Id() const;

    void Draw( CBitmapContext* aGc, const TRect aDrawRect );

    TBool Update( const TTimeIntervalMicroSeconds32 aTimeIntercal );

    TBool KeyEvent( const TKeyEvent &aKeyEvent );

    void SetPause( TBool aPause );
    
private: // data

    TBool iPause;
    
    TInt iAngle;

    };

#endif // CMYENGINE1_H_
