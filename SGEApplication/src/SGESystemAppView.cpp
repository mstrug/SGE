/*
 ============================================================================
  Name        : SGESystemAppView.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

// INCLUDE FILES
#include "SGESystemAppView.h"
#include "SGEApplication.h"
#include <akndef.h>
#include <COEMAIN.H>


// -----------------------------------------------------------------------------
// CSGESystemAppView::NewL()
// -----------------------------------------------------------------------------
//
CSGESystemAppView* CSGESystemAppView::NewL( const TRect& aRect, CSGEApplication& aApplication )
    {
    CSGESystemAppView* self = CSGESystemAppView::NewLC( aRect, aApplication );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CSGESystemAppView::NewLC()
// -----------------------------------------------------------------------------
//
CSGESystemAppView* CSGESystemAppView::NewLC( const TRect& aRect, CSGEApplication& aApplication )
    {
    CSGESystemAppView* self = new (ELeave) CSGESystemAppView( aApplication );
    CleanupStack::PushL( self );
    self->ConstructL( aRect );
    return self;
    }

// -----------------------------------------------------------------------------
// CSGESystemAppView::ConstructL()
// -----------------------------------------------------------------------------
//
void CSGESystemAppView::ConstructL( const TRect& aRect )
    {
    CreateWindowL();
    
    if ( iApplication.FullScreen() )
        {
        SetExtentToWholeScreen();
        }
    else
        {
        SetRect( aRect );
        }
        
    ActivateL();
    
    iCreated = ETrue;
    }

// -----------------------------------------------------------------------------
// CSGESystemAppView::CSGESystemAppView()
// -----------------------------------------------------------------------------
//
CSGESystemAppView::CSGESystemAppView( CSGEApplication& aApplication ) : iApplication( aApplication )
    {
    }

// -----------------------------------------------------------------------------
// CSGESystemAppView::~CSGESystemAppView()
// -----------------------------------------------------------------------------
//
CSGESystemAppView::~CSGESystemAppView()
    {
    }

// -----------------------------------------------------------------------------
// CSGESystemAppView::WindowControl()
// -----------------------------------------------------------------------------
//
RWindow& CSGESystemAppView::WindowControl() const
    {
    return Window();
    }

// -----------------------------------------------------------------------------
// CSGESystemAppView::Draw()
// -----------------------------------------------------------------------------
//
RWsSession& CSGESystemAppView::WindowSession() const
    {
    return iCoeEnv->WsSession();
    }

// -----------------------------------------------------------------------------
// CSGESystemAppView::Fs()
// -----------------------------------------------------------------------------
//
RFs& CSGESystemAppView::Fs() const
    {
    return iCoeEnv->FsSession();
    }

// -----------------------------------------------------------------------------
// CSGESystemAppView::EnableDragEvents()
// -----------------------------------------------------------------------------
//
void CSGESystemAppView::EnableDragEvents()
	{
	CCoeControl::EnableDragEvents();
	}

// -----------------------------------------------------------------------------
// CSGESystemAppView::Draw()
// -----------------------------------------------------------------------------
//
void CSGESystemAppView::Draw( const TRect& aRect ) const
    {
    if ( iCreated )
        {
        iApplication.Draw( aRect );
        }
    }

// -----------------------------------------------------------------------------
// CSGESystemAppView::SizeChanged()
// -----------------------------------------------------------------------------
//
void CSGESystemAppView::SizeChanged()
    {
    if ( iCreated )
        {
        iApplication.SizeChanged();
        }
    }

// -----------------------------------------------------------------------------
// CSGESystemAppView::HandlePointerEventL()
// -----------------------------------------------------------------------------
//
void CSGESystemAppView::HandlePointerEventL( const TPointerEvent& aPointerEvent )
    {
    if ( iCreated && !iApplication.HandlePointerEventL( aPointerEvent ) )
        {
        CCoeControl::HandlePointerEventL( aPointerEvent );
        }
    }

// -----------------------------------------------------------------------------
// CSGESystemAppView::HandlePointerEventL()
// -----------------------------------------------------------------------------
//
void CSGESystemAppView::HandleResourceChange( TInt aType )
   {
   iApplication.HandleResourceChange( aType );
   if ( iCreated && aType == KEikDynamicLayoutVariantSwitch && iApplication.FullScreen() )
       {
       SetExtentToWholeScreen();
       }
   }

// End of File
