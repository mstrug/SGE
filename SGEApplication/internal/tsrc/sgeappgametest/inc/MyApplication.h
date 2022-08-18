/*
 * MyApplication.h
 *
 *  Created on: 2009-10-14
 *      Author: michal.strug
 *   Copyright: Copyright (c) 2022 Michal Strug. All rights reserved.
 */

#ifndef CMYAPPLICATION_H_
#define CMYAPPLICATION_H_

#include <SGEGameApplication.h>

class CMyApplication : public CSGEGameApplication
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

private: // data

    TInt iAngle;

    TRgb iColor;

    };

#endif /* CMYAPPLICATION_H_ */
