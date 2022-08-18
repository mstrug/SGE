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

#define _UID3 0xE22B5DAc

CMyApplication::CMyApplication()
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
    UpdateScene();
    DrawScene( Gc() );
    }

TBool CMyApplication::FullScreen() const
    {
    return ETrue;
    }

void CMyApplication::Draw( const TRect& /*aRect*/ ) const
    {
    const_cast< CMyApplication* >( this )->DrawScene( &SystemGc() );
    }

void CMyApplication::InitL()
    {
    CSGEGameApplication::InitL();

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
        iAngle += 2;

        if ( iAngle >= 360 )
            {
            iAngle = 0;
            }
        }
    }

void CMyApplication::DrawScene( CBitmapContext* aGc )
    {
    if ( !aGc )
        { // DSA is not active
        return;
        }
    aGc->Reset();

    // Gets the control's extent
    TRect drawRect( TPoint(), Size() );

    // Clears the screen
    aGc->Clear(drawRect);


    // parametric equaion of circle
    // x = x0 + r * cos( angle )
    // y = y0 + r * sin( angle )

    TInt x0 = Size().iWidth / 2;
    TInt y0 = Size().iHeight / 2;
    TInt r = Size().iWidth / 4;

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

    aGc->SetBrushStyle( CWindowGc::ESolidBrush );
    aGc->SetBrushColor( TRgb( 0, 255, 0 ) );

    aGc->DrawRect( f );
    }

