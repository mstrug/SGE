/*
 ============================================================================
 Name		: SGEGuiControlBase.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEGuiControlBase implementation
 ============================================================================
 */

#include "SGEGuiControlBase.h"



// -----------------------------------------------------------------------------
// CSGEGuiControlBase::CSGEGuiControlBase()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEGuiControlBase::CSGEGuiControlBase( TUint aControlId, TRect aControlRect )
    : iNeedsRedraw( ETrue ), iControlId( aControlId ), iControlRect( aControlRect )
    {
    }

// -----------------------------------------------------------------------------
// CSGEGuiControlBase::~CSGEGuiControlBase()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEGuiControlBase::~CSGEGuiControlBase()
    {
    }

// -----------------------------------------------------------------------------
// CSGEGuiControlBase::BaseConstructL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEGuiControlBase::BaseConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CSGEGuiControlBase::SetObserver()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEGuiControlBase::SetObserver( MSGEGuiControlObserver* aObserver )
    {
    iObserver = aObserver;
    }

// -----------------------------------------------------------------------------
// CSGEGuiControlBase::Observer()
// -----------------------------------------------------------------------------
//
EXPORT_C MSGEGuiControlObserver* CSGEGuiControlBase::Observer() const
    {
    return iObserver;
    }

// -----------------------------------------------------------------------------
// CSGEGuiControlBase::Focused()
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSGEGuiControlBase::Focused() const
    {
    return ( iControlState == EFocused ); 
    }

// -----------------------------------------------------------------------------
// CSGEGuiControlBase::SetFocused()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEGuiControlBase::SetFocused( TBool aFocused )
    {
    SetFocused( aFocused, TPoint() );
    }

// -----------------------------------------------------------------------------
// CSGEGuiControlBase::SetFocused()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEGuiControlBase::SetFocused( TBool aFocused, TPoint aPosition )
    {
    if ( iObserver && iControlState == EPushed )
        {
        if ( aPosition != TPoint() && !CheckPointerEvent( aPosition ) )
            {
            iObserver->GuiControlEvent( this, MSGEGuiControlObserver::EUnpushedOutside, aPosition );
            }
        else
            {
            iObserver->GuiControlEvent( this, MSGEGuiControlObserver::EUnpushed, aPosition );
            }
        }
    else if ( iObserver && iControlState == ENormal && aFocused )
        {
        iObserver->GuiControlEvent( this, MSGEGuiControlObserver::EFocused, aPosition );
        }

    if ( aFocused && iControlState != EFocused )
        {
        iControlState = EFocused;
        iNeedsRedraw = ETrue;
        }
    else if ( !aFocused && iControlState != ENormal )
        {
        iControlState = ENormal;
        iNeedsRedraw = ETrue;
        }
    }

// -----------------------------------------------------------------------------
// CSGEGuiControlBase::Pushed()
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSGEGuiControlBase::Pushed() const
    {
    return ( iControlState == EPushed );
    }

// -----------------------------------------------------------------------------
// CSGEGuiControlBase::SetPushed()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEGuiControlBase::SetPushed( TBool aPushed )
    {
    SetPushed( aPushed, TPoint() );
    }

// -----------------------------------------------------------------------------
// CSGEGuiControlBase::SetPushed()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEGuiControlBase::SetPushed( TBool aPushed, TPoint aPosition )
    {
    if ( iObserver && iControlState != EPushed && aPushed )
        {
        iObserver->GuiControlEvent( this, MSGEGuiControlObserver::EPushed, aPosition );
        }
    else if ( iObserver && iControlState == EPushed && !aPushed )
        {
        iObserver->GuiControlEvent( this, MSGEGuiControlObserver::EUnpushedOutside, aPosition );
        }
    
    if ( aPushed && iControlState != EPushed )
        {
        iControlState = EPushed;
        iNeedsRedraw = ETrue;

        InvokeFeedback();
        }
    else if ( !aPushed && iControlState != ENormal )
        {
        iControlState = ENormal;
        iNeedsRedraw = ETrue;
        }
    }

// -----------------------------------------------------------------------------
// CSGEGuiControlBase::Hidden()
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSGEGuiControlBase::Hidden() const
    {
    return iHidden;
    }

// -----------------------------------------------------------------------------
// CSGEGuiControlBase::SetHidden()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEGuiControlBase::SetHidden( TBool aHidden )
    {
    if ( aHidden != iHidden )
        {
        iHidden = aHidden;
        iNeedsRedraw = ETrue;
        }
    }

// -----------------------------------------------------------------------------
// CSGEGuiControlBase::Rect()
// -----------------------------------------------------------------------------
//
EXPORT_C TRect CSGEGuiControlBase::Rect() const
    {
    return iControlRect;
    }

// -----------------------------------------------------------------------------
// CSGEGuiControlBase::SetRect()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEGuiControlBase::SetRect( TRect aRect )
    {
    iControlRect = aRect;
    iNeedsRedraw = ETrue;
    }

// -----------------------------------------------------------------------------
// CSGEGuiControlBase::Id()
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CSGEGuiControlBase::Id() const
    {
    return iControlId;
    }


// -----------------------------------------------------------------------------
// CSGEGuiControlBase::GetControl()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEGuiControlBase* CSGEGuiControlBase::GetControl( TUint /*aControlId*/ ) const
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// CSGEGuiControlBase::SetPointerEventMargins()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEGuiControlBase::SetPointerEventMargins( TInt aLeft, TInt aTop, TInt aRight, TInt aBottom )
    {
    iPointerEventMargin.iTl.SetXY( aLeft, aTop );
    iPointerEventMargin.iBr.SetXY( aRight, aBottom );
    }

// -----------------------------------------------------------------------------
// CSGEGuiControlBase::PointerEventMargins()
// -----------------------------------------------------------------------------
//
EXPORT_C TRect CSGEGuiControlBase::PointerEventMargins() const
    {
    return iPointerEventMargin;
    }
    
// -----------------------------------------------------------------------------
// CSGEGuiControlBase::CheckPointerEvent()
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSGEGuiControlBase::CheckPointerEvent( TPoint aPosition )
    {
    if ( iHidden )
        {
        return EFalse;
        }
    TRect r( iControlRect );
    r.iTl += iPointerEventMargin.iTl;
    r.iBr -= iPointerEventMargin.iBr;
    return r.Contains( aPosition );
    }

// -----------------------------------------------------------------------------
// CSGEGuiControlBase::SetNeedsRedraw()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEGuiControlBase::SetNeedsRedraw( TBool aVal )
    {
    iNeedsRedraw = aVal;
    }

// -----------------------------------------------------------------------------
// CSGEGuiControlBase::NeedsRedraw()
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSGEGuiControlBase::NeedsRedraw() const
    {
    return iNeedsRedraw;
    }

// -----------------------------------------------------------------------------
// CSGEGuiControlBase::RedrawRect()
// -----------------------------------------------------------------------------
//
EXPORT_C TRect CSGEGuiControlBase::RedrawRect() const
    {
    return iControlRect;
    }

// -----------------------------------------------------------------------------
// CSGEGuiControlBase::ResetState()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEGuiControlBase::ResetState()
    {
    iControlState = ENormal;
    iNeedsRedraw = ETrue;
    }

// -----------------------------------------------------------------------------
// CSGEGuiControlBase::SetMinimumPushedEventTime()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEGuiControlBase::SetMinimumPushedEventTime( TInt aMicroSeconds )
    {
    iMinimumPushedEventTime = aMicroSeconds;
    }

// -----------------------------------------------------------------------------
// CSGEGuiControlBase::MinimumPushedEventTime()
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CSGEGuiControlBase::MinimumPushedEventTime() const
    {
    return iMinimumPushedEventTime;
    }

// -----------------------------------------------------------------------------
// CSGEGuiControlBase::InvokeFeedback()
// -----------------------------------------------------------------------------
//

EXPORT_C void CSGEGuiControlBase::InvokeFeedback()
    {
    if ( iObserver )
        {
        iObserver->GuiControlFeedback();
        }
    }

// -----------------------------------------------------------------------------
// CSGEGuiControlBase::Update()
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSGEGuiControlBase::Update( const TTimeIntervalMicroSeconds32 /*aTimeIntercal*/ )
    {
    return iNeedsRedraw;
    }


// End of file
