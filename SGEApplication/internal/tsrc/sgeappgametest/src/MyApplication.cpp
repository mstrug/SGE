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

#define _UID3 0xE22B5DAb

const TRgb KWhite( 255, 255, 255 );
const TRgb KBlue( 0, 0, 255 );


CMyApplication::CMyApplication() : iColor( KWhite )
    {
    }

CMyApplication::~CMyApplication()
    {
    }

CApaApplication* CMyApplication::NewApplication()
    {
    return CSGEGameApplication::NewApplication( new CMyApplication );
    }

TUid CMyApplication::ApplicationUid()
    {
    return TUid::Uid(_UID3);
    }

void CMyApplication::DoGameLoopEvent( TTimeIntervalMicroSeconds32 /*aTimeInterval*/ )
    {

    iAngle += 2;

    if ( iAngle >= 360 )
        {
        iAngle = 0;
        }

    DrawNow();
    }


void CMyApplication::InitL()
    {
    CSGEGameApplication::InitL();

    StartGameLoop();
    }

void CMyApplication::HandleForegroundEventL( TBool aForeground )
    {
    CSGEGameApplication::HandleForegroundEventL( aForeground );

    if ( aForeground )
        {
        iColor = KWhite;
        }
    else
        {
        iColor = KBlue;
        }
    }

TBool CMyApplication::FullScreen() const
    {
    return ETrue;
    }

void CMyApplication::Draw( const TRect& aRect ) const
    {
    // Get the standard graphics context
    CWindowGc& gc = SystemGc();

    // Gets the control's extent
    TRect drawRect( aRect );

    gc.SetBrushColor( iColor );

    // Clears the screen
    gc.Clear(drawRect);


    // parametric equaion of circle
    // x = x0 + r * cos( angle )
    // y = y0 + r * sin( angle )

    TInt x0 = aRect.Width() / 2;
    TInt y0 = aRect.Height() / 2;
    TInt r = aRect.Width() / 4;

    TReal cosVal;
    TReal sinVal;
    const TReal pi = 3.14159265;

    Math::Cos( cosVal, iAngle * pi / 180 );
    Math::Sin( sinVal, iAngle * pi / 180 );

    TInt x = x0 + r * cosVal;
    TInt y = y0 + r * sinVal;


    // draw rectangle
    TInt w = 4;
    TRect f( TPoint( x - w, y - w ), TSize( 2 * w, 2 * w ) );

    gc.SetBrushStyle( CWindowGc::ESolidBrush );
    gc.SetBrushColor( TRgb( 255, 0, 0 ) );

    gc.DrawRect( f );
    }

