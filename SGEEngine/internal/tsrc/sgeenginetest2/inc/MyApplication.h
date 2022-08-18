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
#include <SGEEngineControllerObserver.h>

class CMyEngineController;


class CMyApplication : public CSGEGameApplicationDS, MSGEEngineControllerObserver
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

    void EngineControllerClose( TInt aError );


private: // data

    CMyEngineController *iEngineCntrl;

    };

#endif /* CMYAPPLICATION_H_ */
