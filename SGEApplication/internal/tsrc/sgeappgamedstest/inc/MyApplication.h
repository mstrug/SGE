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

class CMyApplication : public CSGEGameApplicationDS
    {
public:
    
    CMyApplication();

    ~CMyApplication();

    TUid ApplicationUid();

    static CApaApplication* NewApplication();

    void Draw( const TRect& aRect ) const;

    void InitL();

    void HandleForegroundEventL( TBool aForeground );

    TBool FullScreen() const;

private:

    void DoGameLoopEvent( TTimeIntervalMicroSeconds32 aTimeInterval );

    void UpdateScene();

    void DrawScene( CBitmapContext* aGc );

private: // data

    TInt iAngle;

    TBool iPause;

    };

#endif /* CMYAPPLICATION_H_ */
