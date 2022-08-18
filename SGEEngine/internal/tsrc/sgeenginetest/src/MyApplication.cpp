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
#include "MyEngineController.h"

#define _UID3 0xE22B5Db0

CMyApplication::CMyApplication()
    {
    }

CMyApplication::~CMyApplication()
    {
    delete iEngineCntrl;
    }

CApaApplication* CMyApplication::NewApplication()
    {
    return CSGEGameApplication::NewApplication( new CMyApplication );
    }

TUid CMyApplication::ApplicationUid()
    {
    return TUid::Uid(_UID3);
    }

void CMyApplication::DoGameLoopEvent( TTimeIntervalMicroSeconds32 aTimeInterval )
    {
    iEngineCntrl->Update( Gc(), Size(), aTimeInterval );
    }

TBool CMyApplication::FullScreen() const
    {
    return ETrue;
    }


void CMyApplication::InitL()
    {
    CSGEGameApplication::InitL();

    iEngineCntrl = CMyEngineController::NewL( *this, Rect().Size() );

    StartGameLoop();
    }

void CMyApplication::HandleForegroundEventL( TBool aForeground )
    {
    CSGEGameApplicationDS::HandleForegroundEventL( aForeground );

    if ( aForeground )
        {
        SetGameLoopMaxFPS( 60 );
        StartGameLoop();
        }

    iEngineCntrl->HandleForegroundEventL( aForeground );
    }

void CMyApplication::Draw( const TRect& /*aRect*/ ) const
    {
    iEngineCntrl->Draw( &SystemGc(), TRect( TPoint(), Size() ) );
    }

TKeyResponse CMyApplication::HandleKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType )
    {
    return iEngineCntrl->HandleKeyEventL( aKeyEvent, aType );
    }

void CMyApplication::SizeChanged()
    {
    iEngineCntrl->SizeChanged( Size() );
    }

void CMyApplication::EngineControllerClose( TInt aError )
    {
    if ( aError != KErrNone )
        {
        AlertWin( _L("Error"), _L("Try restart or reinstall application.") );
        }
    Exit();
    }

