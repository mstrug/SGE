/*
 * MyApplication.h
 *
 *  Created on: 2009-10-14
 *      Author: michal.strug
 *   Copyright: Copyright (c) 2022 Michal Strug. All rights reserved.
 */

#ifndef CMYAPPLICATION_H_
#define CMYAPPLICATION_H_

#include <SGEGameApplicationVG.h>
#include <VG/openvg.h>


class CMyApplication : public CSGEGameApplicationVG
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

TInt SampleBuffers();

TInt SampleSize();

    TInt DepthSize();
    
private:

    void DoGameLoopEvent( TTimeIntervalMicroSeconds32 aTimeInterval );

    void UpdateScene();

    void DrawScene();

private: // data

    TInt iAngle;

    TBool iPause;

    VGImage iVimg;

    };

#endif /* CMYAPPLICATION_H_ */
