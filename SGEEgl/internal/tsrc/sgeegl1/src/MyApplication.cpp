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

#define _UID3 0xE22B50Ad

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
    delete iRc;
    delete iPbuffer;
    delete iDevice;
    delete iPixmap;
    }

CApaApplication* CMyApplication::NewApplication()
    {
    return CSGEGameApplication::NewApplication( new CMyApplication );
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

void CMyApplication::InitL()
    {
    CSGEGameApplication::InitL();

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
    glEnable( GL_CULL_FACE  );
    
    // Enable vertex arrays.
    glEnableClientState( GL_VERTEX_ARRAY );
    
    // Set array pointers.
    glVertexPointer( 3, GL_BYTE, 0, vertices );
    
    // Enable color arrays.
    glEnableClientState( GL_COLOR_ARRAY );
    
    // Set color pointers.
    glColorPointer( 4, GL_UNSIGNED_BYTE, 0, colors );
    
    // Set the initial shading mode
    glShadeModel( GL_SMOOTH );
    
    // Do not use perspective correction
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST );

    
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

    glDrawElements( GL_TRIANGLES, 12 * 3, GL_UNSIGNED_BYTE, triangles );
    }

void CMyApplication::DrawScene()
    {
    iRc->MakeCurrent( *iPbuffer );
    
    const GLint cameraDistance = 100;

    glClear( GL_COLOR_BUFFER_BIT );

    /* Animate and draw box */
    glLoadIdentity();
    glTranslatex( 0 , 0 , -cameraDistance << 16 );
    glRotatex( -iAngle << 16, 1 << 16,    0   ,    0    );
    glRotatex( -iAngle << 15,    0   , 1 << 16,    0    );
    glRotatex( -iAngle << 14,    0   ,    0   , 1 << 16 );
    DrawBox( 15.f, 15.f, 15.f );

    
    
    MakeCurrentWindowEglContext();

    glClear( GL_COLOR_BUFFER_BIT );

    /* Animate and draw box */
    glLoadIdentity();
    glTranslatex( 0 , 0 , -cameraDistance << 16 );
    glRotatex( iAngle << 16, 1 << 16,    0   ,    0    );
    glRotatex( iAngle << 15,    0   , 1 << 16,    0    );
    glRotatex( iAngle << 14,    0   ,    0   , 1 << 16 );
    DrawBox( 15.f, 15.f, 15.f );

    }

