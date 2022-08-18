/*
 * MyApplication.h
 *
 *  Created on: 2011-01-18
 *      Author: michal.strug
 *   Copyright: Copyright (c) 2022 Michal Strug. All rights reserved.
 */

#ifndef CMYAPPLICATION_H_
#define CMYAPPLICATION_H_

#include <SGEGameApplicationGL.h>
#include <gles/gl.h>
#include <SGEGl/GlPrimitive.h>
#include <SGEGl/GlVertexArray.h>
#include <SGEGl/GlVertexBuffer.h>
#include <SGEGl/GlObjectCube.h>
#include <SGEGl/GlObjectRect.h>
#include <SGEGl/GlTexture.h>

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
    
    TBool GoBackgroundOnEndKey() const;

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

    void DrawRect( GLfloat aSizeX, GLfloat aSizeY, GLfloat aSizeZ );

    void DrawScene();

    void InitSGEGl();
    
private: // data

    TInt iAngle;

    TBool iPause;

    CWsBitmap *iPixmap;

    CEglDevice *iDevice;
    
    CEglSurfacePbuffer *iPbuffer;
    
    CEglRc *iRc;
    
    CGlPrimitive *iCube;
    CGlVertexBuffer *iCubeData;
    
    CGlObjectCube *iObjCube;
    CGlObjectRect *iObjRect;
    TGlTexture iTexture1;
    };

#endif /* CMYAPPLICATION_H_ */
