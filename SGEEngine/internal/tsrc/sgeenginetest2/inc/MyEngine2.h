/*
 * MyEngine.h
 *
 *  Created on: 2009-10-14
 *      Author: michal.strug
 *   Copyright: Copyright (c) 2022 Michal Strug. All rights reserved.
 */

#ifndef CMYENGINE2_H_
#define CMYENGINE2_H_

#include <SGEEngine.h>
#include <SGEEngineObserver.h>

const TUint KMyEngine2Id = 2;

class MSGEEngine2Observer : public MSGEEngineObserver
    {
public:
    virtual void Engine2Switch() = 0;
    };


class CMyEngine2 : public CSGEEngine
    {
public:

    static CMyEngine2* NewL( MSGEEngine2Observer& aObserver, CSGEEngineCommonObjects *aCommonObjects );

private:

    void ConstructL();
    
    CMyEngine2( MSGEEngine2Observer& aObserver, CSGEEngineCommonObjects *aCommonObjects );

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

#endif // CMYENGINE2_H_
