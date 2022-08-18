/*
 * MyApplication.h
 *
 *  Created on: 2009-10-14
 *      Author: michal.strug
 *   Copyright: Copyright (c) 2022 Michal Strug. All rights reserved.
 */

#ifndef CMYAPPLICATION_H_
#define CMYAPPLICATION_H_

#include <SGEGameApplicationDS.h>
#include <SGEEngineObserver.h>

class CMyEngine;


class CMyApplication : public CSGEGameApplicationDS, MSGEEngineObserver
    {
public:
    
    CMyApplication();

    ~CMyApplication();

    TUid ApplicationUid();

    static CApaApplication* NewApplication();

    void InitL();

    void HandleForegroundEventL( TBool aForeground );

    TKeyResponse HandleKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );

    void SizeChanged();

    TBool FullScreen() const;

    void Draw( const TRect& aRect ) const;

private:

    void DoGameLoopEvent( TTimeIntervalMicroSeconds32 aTimeInterval );

private: // from MSGEEngineControllerObserver

    void EngineClose( TUint aId, TInt aError );

    void EngineSwitch( TUint aId );

private: // data

    CMyEngine *iEngine;

    };

#endif /* CMYAPPLICATION_H_ */
