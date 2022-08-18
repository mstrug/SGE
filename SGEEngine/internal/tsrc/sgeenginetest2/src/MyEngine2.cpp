/*
 ============================================================================
 Name		: MyEngine2.cpp
 Author	  : 
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : 
 ============================================================================
 */

#include "MyEngine2.h"
#include <SGEEngineObserver.h>
#include <e32math.h>


CMyEngine2* CMyEngine2::NewL( MSGEEngine2Observer& aObserver, CSGEEngineCommonObjects *aCommonObjects )
    {
    CMyEngine2 *self = new (ELeave) CMyEngine2( aObserver, aCommonObjects );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

void CMyEngine2::ConstructL()
    {
    BaseConstructL();
    }

CMyEngine2::CMyEngine2( MSGEEngine2Observer& aObserver, CSGEEngineCommonObjects *aCommonObjects )
        : CSGEEngine( aObserver, aCommonObjects )
    {
    }


TUint CMyEngine2::Id() const
    {
    return KMyEngine2Id;
    }

void CMyEngine2::Draw( CBitmapContext* aGc, const TRect aDrawRect )
    {
    if ( !aGc )
        { // DSA is not active
        return;
        }
    aGc->Reset();

    // Clears the screen
    aGc->Clear( aDrawRect );


    // parametric equaion of circle
    // x = x0 + r * cos( angle )
    // y = y0 + r * sin( angle )

    TInt x0 = aDrawRect.Size().iWidth / 2;
    TInt y0 = aDrawRect.Size().iHeight / 2;
    TInt r = aDrawRect.Size().iWidth / 4;

    TReal cosVal;
    TReal sinVal;
    const TReal pi = 3.14159265;

    Math::Cos( cosVal, iAngle * pi / 180 );
    Math::Sin( sinVal, iAngle * pi / 180 );

    TInt x = x0 + r * cosVal;
    TInt y = y0 + 2 * r * sinVal;


    // draw rectangle
    TInt w = 4;
    TRect f( TPoint( x - w, y - w ), TSize( 2 * w, 2 * w ) );

    aGc->SetBrushStyle( CWindowGc::ESolidBrush );
    aGc->SetBrushColor( TRgb( 255, 255, 0 ) );

    aGc->DrawRect( f );
    }

TBool CMyEngine2::Update( const TTimeIntervalMicroSeconds32 /*aTimeIntercal*/ )
    {
    if ( !iPause )
        {
        iAngle += 2;

        if ( iAngle >= 360 )
            {
            iAngle = 0;
            }
        return ETrue;
        }
    return EFalse;
    }

TBool CMyEngine2::KeyEvent( const TKeyEvent &aKeyEvent )
    {
    if ( aKeyEvent.iScanCode == EStdKeyEscape )
        {
        iObserver.EngineClose( Id(), KErrNone );
        return ETrue;
        }
    else if ( aKeyEvent.iScanCode == EStdKeyLeftArrow )
        {
        static_cast<MSGEEngine2Observer&>(iObserver).Engine2Switch();
        return ETrue;
        }
    return EFalse;
    }


void CMyEngine2::SetPause( TBool aPause )
    {
    iPause = aPause;
    }

