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

#define _UID3 0xE22B5DAa

CMyApplication::CMyApplication()
    {
    }

CMyApplication::~CMyApplication()
    {
    }

CApaApplication* CMyApplication::NewApplication()
    {
    return CSGEApplication::NewApplication( new CMyApplication );
    }

TUid CMyApplication::ApplicationUid()
    {
    return TUid::Uid(_UID3);
    }

void CMyApplication::HandleCommandL( TInt /*aCommand*/ )
    {
    Exit();
    }

TKeyResponse CMyApplication::HandleKeyEventL( const TKeyEvent& aKeyEvent, TEventCode /*aType*/ )
    {
    if ( aKeyEvent.iScanCode == EStdKeyEscape )
        {
        Exit();
        return EKeyWasConsumed;
        }
    return EKeyWasNotConsumed;
    }

void CMyApplication::Draw( const TRect& aRect ) const
    {
    // Get the standard graphics context
    CWindowGc& gc = SystemGc();

    // Gets the control's extent
    TRect drawRect( aRect );

    // Clears the screen
    gc.Clear(drawRect);
    }
