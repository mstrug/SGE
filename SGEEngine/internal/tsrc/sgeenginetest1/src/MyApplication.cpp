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
#include "MyEngine.h"

#define _UID3 0xE22B5Db2

CMyApplication::CMyApplication()
    {
    }

CMyApplication::~CMyApplication()
    {
    delete iEngine;
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
    if ( iEngine->Update( aTimeInterval ) )
        {
        iEngine->Draw( Gc(), Size() );
        }
    }

TBool CMyApplication::FullScreen() const
    {
    return ETrue;
    }


void CMyApplication::InitL()
    {
    CSGEGameApplication::InitL();

    iEngine = CMyEngine::NewL( *this, NULL );

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

    iEngine->SetPause( !aForeground );
    }

void CMyApplication::Draw( const TRect& /*aRect*/ ) const
    {
    const_cast< CMyApplication* >( this )->iEngine->Draw( &SystemGc(), TRect( TPoint(), Size() ) );
    }

TKeyResponse CMyApplication::HandleKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType )
    {
    TBool ret = EFalse;
    if ( aType == EEventKey )
        {
        ret = iEngine->KeyEvent( aKeyEvent );
        }
    if ( ret )
        {
        return EKeyWasConsumed;
        }
    else
        {
        return EKeyWasNotConsumed;
        }
    }

void CMyApplication::SizeChanged()
    {
    iEngine->SizeChanged( Size() );
    }

void CMyApplication::EngineClose( TUint /*aId*/, TInt aError )
    {
    if ( aError != KErrNone )
        {
        AlertWin( _L("Error"), _L("Try restart or reinstall application.") );
        }
    Exit();
    }

void CMyApplication::EngineSwitch( TUint /*aId*/ )
    {
    }

