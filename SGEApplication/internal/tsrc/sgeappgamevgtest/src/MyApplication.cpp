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
#include <VG/vgu.h>

#define _UID3 0xE23B6DAd



CMyApplication::CMyApplication()
    {
    }

CMyApplication::~CMyApplication()
    {
    vgDestroyImage( iVimg );
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

TInt CMyApplication::DepthSize()
	{
	return 0;
	}

TInt CMyApplication::SampleBuffers()
    {
    return 0;
    }

TInt CMyApplication::SampleSize()
    {
    return 0;
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
    
    iVimg = vgCreateImage( VG_sRGBA_8888, 2, 2, VG_IMAGE_QUALITY_BETTER );

    const TUint32 imageData[] = { 0xFF0000FF, 0xFFFF00FF, 0x00FF00FF, 0xFF000000 };

    vgImageSubData( iVimg, imageData, 4 * 2, VG_sRGBA_8888, 0, 0, 2, 2 );

    StartGameLoop();
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

void CMyApplication::DrawScene()
    {
    float clearColor[] = { 0.006f, 0.8f, 1.0f, 1.0f };
    vgSetfv( VG_CLEAR_COLOR, 4, clearColor );

    vgSeti( VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE );

    vgClear( 0, 0, Size().iWidth, Size().iHeight );

    vgLoadIdentity();

    VGPath path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 10, 20, VG_PATH_CAPABILITY_ALL );

    VGPaint paint = vgCreatePaint();
    vgSetParameteri( paint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR );
    vgSetColor( paint, 0xFF0000FF );
    vgSetf( VG_STROKE_LINE_WIDTH, 1 );

    vgSetPaint( paint, VG_STROKE_PATH );

    vguLine( path, 0, 0, Size().iWidth, Size().iHeight );

    vgDrawPath( path, VG_STROKE_PATH );

    vgClearPath( path, VG_PATH_CAPABILITY_ALL );

    vgTranslate( Size().iWidth / 2, Size().iHeight / 2 );
    vgRotate( iAngle );

    vgSetColor( paint, 0xFFFF00FF );
    vgSetf( VG_STROKE_LINE_WIDTH, 3 );

    vguRoundRect( path, -50, -50, 100, 100, 10, 10 );

    vgDrawPath( path, VG_STROKE_PATH );


    vgSeti( VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE );
    vgLoadIdentity();
    vgTranslate( Size().iWidth / 2, Size().iHeight / 2 );
    vgRotate( -2 * iAngle );
    vgTranslate( Size().iHeight / 4, Size().iHeight / 4 );

    vgSeti( VG_IMAGE_MODE, VG_DRAW_IMAGE_NORMAL );

    vgScale( 20, 20 );
    vgDrawImage( iVimg );

    vgDestroyPaint( paint );

    vgDestroyPath( path );
    }

