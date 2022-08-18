/*
 * MyEngine.h
 *
 *  Created on: 2009-10-14
 *      Author: michal.strug
 *   Copyright: Copyright (c) 2022 Michal Strug. All rights reserved.
 */

#ifndef CMYENGINE_H_
#define CMYENGINE_H_

#include <SGEEngine.h>

const TUint KMyEngineId = 1;


class CMyEngine : public CSGEEngine
    {
public:

    static CMyEngine* NewL( MSGEEngineObserver& aObserver, CSGEEngineCommonObjects *aCommonObjects );

private:

    void ConstructL();
    
    CMyEngine( MSGEEngineObserver& aObserver, CSGEEngineCommonObjects *aCommonObjects );

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

#endif // CMYENGINE_H_
