/*
 ============================================================================
 Name		: MyApplication.cpp
 Author	  : 
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CApplication implementation
 ============================================================================
 */

#include "MyApplication.h"
#include <e32math.h>
#include <EglSurfacePbuffer.h>
#include <EglRc.h>
#include <EglDevice.h>
#include <SGEGl/GlUtils.h>
#include <SGEGl/GlPolygon.h>

#define _UID3 0xE22B50A4

// MACROS
#define FRUSTUM_LEFT   -1.f     //left vertical clipping plane
#define FRUSTUM_RIGHT   1.f     //right vertical clipping plane
#define FRUSTUM_BOTTOM -1.f     //bottom horizontal clipping plane
#define FRUSTUM_TOP     1.f     //top horizontal clipping plane
#define FRUSTUM_NEAR    3.f     //near depth clipping plane
#define FRUSTUM_FAR  1000.f     //far depth clipping plane

/** Vertice coordinates for the cube. */
static const GLbyte vertices[8 * 3] =
     {
     -1,  1,  1,
      1,  1,  1,
      1, -1,  1,
     -1, -1,  1,

     -1,  1, -1,
      1,  1, -1,
      1, -1, -1,
     -1, -1, -1
     };

/** Colors for vertices (Red, Green, Blue, Alpha). */
static const GLubyte colors[8 * 4] =
    {
    0  ,255,  0,255,
    0  ,  0,255,255,
    0  ,255,  0,255,
    255,  0,  0,255,

    0  ,  0,255,255,
    255,  0,  0,255,
    0  ,  0,255,255,
    0  ,255,  0,255
    };


/**
 * Indices for drawing the triangles.
 * The color of the triangle is determined by
 * the color of the last vertex of the triangle.
 */
static const GLubyte triangles[12 * 3] =
    {
    /* front */
    1,0,3,
    1,3,2,

    /* right */
    2,6,5,
    2,5,1,

    /* back */
    7,4,5,
    7,5,6,

    /* left */
    0,4,7,
    0,7,3,

    /* top */
    5,4,0,
    5,0,1,

    /* bottom */
    3,7,6,
    3,6,2
    };


CMyApplication::CMyApplication()
    {
    }

CMyApplication::~CMyApplication()
    {
    delete iObjCube;
    delete iObjRect;
    
    delete iCubeData;
    delete iCube;
    
    delete iRc;
    delete iPbuffer;
    delete iDevice;
    delete iPixmap;
    }

CApaApplication* CMyApplication::NewApplication()
    {
    return CSGEGameApplication::NewApplication( new CMyApplication );
    }

TBool CMyApplication::GoBackgroundOnEndKey() const
    {
    return EFalse;
    }

TUid CMyApplication::ApplicationUid()
    {
    return TUid::Uid(_UID3);
    }

TBool CMyApplication::FullScreen() const
    {
    return ETrue;
    }

void CMyApplication::SizeChanged()
    {
    SetScreenSize( Size().iWidth, Size().iHeight );
    }

void CMyApplication::Draw( const TRect& /*aRect*/ ) const
    {
    const_cast< CMyApplication* >( this )->DrawScene();
    }


void CMyApplication::DoGameLoopEvent( TTimeIntervalMicroSeconds32 /*aTimeInterval*/ )
    {
    UpdateScene();
    DrawScene();
    }

void CMyApplication::InitSGEGl()
    {
    iObjCube = CGlObjectCube::NewL();

    const GLubyte texData2[] = 
        { 
        255,255,255,255,    255,255,255,255,    255,255,255,255,    255,255,255,255,    255,255,255,255,    255,255,255,255,    0,255,0,255,    0,255,0,255,
        255,255,255,255,    255,255,255,255,    255,255,255,255,    255,255,255,255,    255,255,255,255,    255,255,255,255,    0,255,0,255,    0,255,0,255,
        255,255,255,255,    255,255,255,255,    255,255,255,255,    255,255,255,255,    255,255,255,255,    255,255,255,255,    0,255,0,255,    0,255,0,255,
        255,255,255,255,    255,255,255,255,    255,255,255,255,    255,255,255,255,    255,255,255,255,    255,255,255,255,    0,255,0,255,    0,255,0,255,
        255,255,255,255,    255,255,255,255,    255,255,255,255,    255,255,255,255,    255,255,255,255,    255,255,255,255,    0,255,0,255,    0,255,0,255,
        255,255,255,255,    255,255,255,255,    255,255,255,255,    255,255,255,255,    255,255,255,255,    255,255,255,255,    0,255,0,255,    0,255,0,255,
        255,255,255,255,    255,255,255,255,    255,255,255,255,    255,255,255,255,    255,255,255,255,    255,255,255,255,    0,255,0,255,    0,255,0,255,
        255,255,255,255,    255,255,255,255,    255,255,255,255,    255,255,255,255,    255,255,255,255,    255,255,255,255,    0,255,0,255,    0,255,0,255
        };
    
    const GLubyte texData1[] = 
        { 
        255,255,255,255,    255,255,255,255,    255,0,0,255,    255,0,0,255,
        255,255,255,255,    255,255,255,255,    255,0,0,255,    255,0,0,255,
        255,255,255,255,    255,255,255,255,    255,255,255,255,    255,255,255,255,
        255,255,255,255,    255,255,255,255,    255,255,255,255,    255,255,255,255
        };
    
    const GLubyte texData[] = 
        { 
        0,255,0,255,    0,255,0,255,
        0,255,0,255,    0,255,0,255
        };
    
    iTexture1.iGenerateMipmap = GL_TRUE;
    iTexture1.CreateTexture( TSize( 8, 8 ), EColor16MA, (GLvoid*)texData2 );
    iTexture1.CreateMipmap( TSize( 4, 4 ), EColor16MA, (GLvoid*)texData1, 1 );
    iTexture1.CreateMipmap( TSize( 2, 2 ), EColor16MA, (GLvoid*)texData, 2 );
    iTexture1.iTextureMinification = GL_LINEAR_MIPMAP_LINEAR;
    
    TGlMaterial matTex;
    matTex.iMaterialId = 0x01;
    matTex.iTextures[ 0 ] = &iTexture1;
    
    iObjRect = CGlObjectRect::NewL();
    iObjRect->Primitive().AddMaterialL( matTex );
    //iObjRect->Primitive().VertexArray( 0 )->TexCoordArrayEnabled( 0, EFalse );
    iObjRect->PrimitiveType().SetCullFace( EFalse );
    }

void CMyApplication::InitL()
    {
    CSGEGameApplication::InitL();
    
    InitSGEGl();

    iPixmap = new CWsBitmap( WsSession() );
    iPixmap->Create( Window().Size(), Window().DisplayMode() );
    
    iDevice = CEglDevice::NewL();
    iDevice->Configure( EFalse, EFalse, ETrue );
    
    iPbuffer = CEglSurfacePbuffer::NewL( *iDevice, TSize( 16, 16 ) );

    iRc = CEglRc::NewL( *iDevice );
    iRc->MakeCurrent( *iPbuffer );
    
    MakeCurrentWindowEglContext();
    
    
    SetScreenSize( Size().iWidth, Size().iHeight );

    // Set the screen background color.
    glClearColor( 0.f, 0.f, 0.f, 1.f );
    
    // Enable back face culling.
    glEnable( GL_CULL_FACE );
    
    // Set the initial shading mode
    GlUtils::SetShadeModelSmooth();
    
    // Do not use perspective correction
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );

    GlUtils::EnableTexturing();
    
    StartGameLoop();
    }

void CMyApplication::SetScreenSize( TUint aWidth, TUint aHeight )
    {
    glViewport( 0, 0, aWidth, aHeight );

    // Recalculate the view frustrum
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    GLfloat aspectRatio = (GLfloat)(aWidth) / (GLfloat)(aHeight);
    glFrustumf( FRUSTUM_LEFT * aspectRatio, FRUSTUM_RIGHT * aspectRatio,
                FRUSTUM_BOTTOM, FRUSTUM_TOP,
                FRUSTUM_NEAR, FRUSTUM_FAR );
    glMatrixMode( GL_MODELVIEW );
    }

void CMyApplication::HandleForegroundEventL( TBool aForeground )
    {
    CSGEGameApplication::HandleForegroundEventL( aForeground );

    iPause = !aForeground;
    if ( !iPause )
        {
        SetGameLoopMaxFPS( 60 );
        StartGameLoop();
        }
    }

void CMyApplication::UpdateScene()
    {
    if ( !iPause )
        {
        iAngle++;
        }
    }

void CMyApplication::DrawBox( GLfloat aSizeX, GLfloat aSizeY, GLfloat aSizeZ )
    {
    glScalef( aSizeX, aSizeY, aSizeZ );
    glScalef( 0.5, 0.5, 0.5 );
    iObjCube->Draw();
    }

void CMyApplication::DrawRect( GLfloat aSizeX, GLfloat aSizeY, GLfloat aSizeZ )
    {
    glScalef( aSizeX, aSizeY, aSizeZ );

    iObjRect->Draw();
    
    }

void CMyApplication::DrawScene()
    {
//    iRc->MakeCurrent( *iPbuffer );
    
    const GLint cameraDistance = 100;

//    glClear( GL_COLOR_BUFFER_BIT );
//
//    /* Animate and draw box */
//    glLoadIdentity();
//    glTranslatex( 0 , 0 , -cameraDistance << 16 );
//    glRotatex( -iAngle << 16, 1 << 16,    0   ,    0    );
//    glRotatex( -iAngle << 15,    0   , 1 << 16,    0    );
//    glRotatex( -iAngle << 14,    0   ,    0   , 1 << 16 );
//    DrawBox( 15.f, 15.f, 15.f );
    
    MakeCurrentWindowEglContext();

    glClear( GL_COLOR_BUFFER_BIT );

    /* Animate and draw box */
    glLoadIdentity();
    glTranslatex( 0 , 0 , -cameraDistance << 16 );
    glTranslatex( 0, 20 << 16, 0 );
    glRotatex( iAngle << 16, 1 << 16,    0   ,    0    );
    glRotatex( iAngle << 15,    0   , 1 << 16,    0    );
    glRotatex( iAngle << 14,    0   ,    0   , 1 << 16 );
    
    DrawBox( 10.f, 10.f, 10.f );
    
    glLoadIdentity();
    glTranslatex( 0 , 0 , -cameraDistance << 16 );
//    glRotatex( iAngle << 16, 1 << 16,    0   ,    0    );
//    glRotatex( iAngle << 15,    0   , 1 << 16,    0    );
//    glRotatex( iAngle << 14,    0   ,    0   , 1 << 16 );
    
    DrawRect( 10.f, 10.f, 10.f );
    
    
    glLoadIdentity();
    glTranslatex( 0 , 0 , -cameraDistance << 16 );
    glTranslatex( 0, -10 << 16, 0 );
//    glRotatex( iAngle << 16, 1 << 16,    0   ,    0    );
//    glRotatex( iAngle << 15,    0   , 1 << 16,    0    );
//    glRotatex( iAngle << 14,    0   ,    0   , 1 << 16 );
    
    DrawRect( 5.f, 5.f, 5.f );
    
    
    glLoadIdentity();
    glTranslatex( 0 , 0 , -cameraDistance << 16 );
    glTranslatex( 0, -20 << 16, 0 );
//    glRotatex( iAngle << 16, 1 << 16,    0   ,    0    );
//    glRotatex( iAngle << 15,    0   , 1 << 16,    0    );
//    glRotatex( iAngle << 14,    0   ,    0   , 1 << 16 );
    
    DrawRect( 1.f, 1.f, 1.f );
    }

