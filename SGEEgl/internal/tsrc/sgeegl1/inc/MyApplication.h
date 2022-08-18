/*
 * MyApplication.h
 *
 *  Created on: 2009-10-14
 *      Author: michal.strug
 *   Copyright: Copyright (c) 2022 Michal Strug. All rights reserved.
 */

#ifndef CMYAPPLICATION_H_
#define CMYAPPLICATION_H_

#include <SGEGameApplicationGL.h>
#include <gles/gl.h>

class CEglDevice;
class CEglSurfacePbuffer;
class CEglRc;

// example based on S60_3rd_FP2_SDK\S60CppExamples\OpenGLEx\SimpleCube

class CMyApplication : public CSGEGameApplicationGL
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

    void SizeChanged();
    
private:

    void DoGameLoopEvent( TTimeIntervalMicroSeconds32 aTimeInterval );

    void UpdateScene();
    
    void SetScreenSize( TUint aWidth, TUint aHeight );
    
    void DrawBox( GLfloat aSizeX, GLfloat aSizeY, GLfloat aSizeZ );

    void DrawScene();

private: // data

    TInt iAngle;

    TBool iPause;

    CWsBitmap *iPixmap;

    CEglDevice *iDevice;
    
    CEglSurfacePbuffer *iPbuffer;
    
    CEglRc *iRc;
    };

#endif /* CMYAPPLICATION_H_ */
