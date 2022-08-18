/*
 ============================================================================
 Name		: SGEGuiManagerControl.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEGuiManagerControl implementation
 ============================================================================
 */

#include "SGEGuiManagerControl.h"
#include "SGEGuiControlBase.h"


const TInt KPushEventMinTime = 200000; // 1 sec


// -----------------------------------------------------------------------------
// CSGEGuiManagerControl::CSGEGuiManagerControl()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEGuiManagerControl::CSGEGuiManagerControl()
    {
    // No implementation required
    }

// -----------------------------------------------------------------------------
// CSGEGuiManagerControl::~CSGEGuiManagerControl()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEGuiManagerControl::~CSGEGuiManagerControl()
    {
    iControls.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CSGEGuiManagerControl::NewLC()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEGuiManagerControl* CSGEGuiManagerControl::NewLC()
    {
    CSGEGuiManagerControl *self = new ( ELeave ) CSGEGuiManagerControl();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CSGEGuiManagerControl::NewL()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEGuiManagerControl* CSGEGuiManagerControl::NewL()
    {
    CSGEGuiManagerControl *self = CSGEGuiManagerControl::NewLC();
    CleanupStack::Pop(); // self;
    return self;
    }

// -----------------------------------------------------------------------------
// CSGEGuiManagerControl::ConstructL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEGuiManagerControl::ConstructL()
    {
    iPushedEventTime.HomeTime();
    }

// -----------------------------------------------------------------------------
// CSGEGuiManagerControl::AddControlL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEGuiManagerControl::AddControlL( CSGEGuiControlBase *aControl )
    {
    CleanupStack::PushL( aControl );
    iControls.AppendL( aControl );
    CleanupStack::Pop( aControl );
    }

// -----------------------------------------------------------------------------
// CSGEGuiManagerControl::GetControl()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEGuiControlBase* CSGEGuiManagerControl::GetControl( TUint aControlId ) const
    {
    for ( TInt i = 0; i < iControls.Count(); i++ )
        {
        if ( iControls[ i ]->Id() == aControlId )
            {
            return iControls[ i ];
            }
        CSGEGuiControlBase *ctrl = iControls[ i ]->GetControl( aControlId );
        if ( ctrl )
            {
            return ctrl;
            }
        }
    return NULL;
    }

// -----------------------------------------------------------------------------
// CSGEGuiManagerControl::GetRedrawRect()
// -----------------------------------------------------------------------------
//
EXPORT_C TRect CSGEGuiManagerControl::GetRedrawRect( TRect aRect ) const
    {
    TRect ret;
    
    for ( TInt i = 0; i < iControls.Count(); i++ )
        {
        if ( iControls[ i ]->NeedsRedraw() )
            {
            TRect r = iControls[ i ]->RedrawRect();
            if ( ret.IsEmpty() )
                {
                ret = r;
                }
            else if ( !r.IsEmpty() )
                {
                if ( r.iTl.iX < ret.iTl.iX )
                    {
                    ret.iTl.iX = r.iTl.iX;
                    }
                if ( r.iTl.iY < ret.iTl.iY )
                    {
                    ret.iTl.iY = r.iTl.iY;
                    }
                if ( r.iBr.iX > ret.iBr.iX )
                    {
                    ret.iBr.iX = r.iBr.iX;
                    }
                if ( r.iBr.iY > ret.iBr.iY )
                    {
                    ret.iBr.iY = r.iBr.iY;
                    }
                }
            }
        }
    
    ret.Intersects( aRect );
    
    return ret;
    }

// -----------------------------------------------------------------------------
// CSGEGuiManagerControl::ResetControls()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEGuiManagerControl::ResetControls()
    {
    for ( TInt i = 0; i < iControls.Count(); i++ )
        {
        iControls[ i ]->ResetState();
        }
    }

// -----------------------------------------------------------------------------
// CSGEGuiManagerControl::Draw()
// -----------------------------------------------------------------------------
//
void CSGEGuiManagerControl::Draw( CBitmapContext* aGc, const TRect aDrawRect )
    {
    for ( TInt i = 0; i < iControls.Count(); i++ )
        {
        iControls[ i ]->Draw( aGc, aDrawRect );
        }
    }

// -----------------------------------------------------------------------------
// CSGEGuiManagerControl::Update()
// -----------------------------------------------------------------------------
//
TBool CSGEGuiManagerControl::Update( const TTimeIntervalMicroSeconds32 aTimeInterval )
    {
    TBool ret = EFalse;
    for ( TInt i = 0; i < iControls.Count(); i++ )
        {
        ret |= iControls[ i ]->Update( aTimeInterval );
        }
    return ret;
    }

// -----------------------------------------------------------------------------
// CSGEGuiManagerControl::KeyDownEvent()
// -----------------------------------------------------------------------------
//
TBool CSGEGuiManagerControl::KeyDownEvent( const TKeyEvent& /*aKeyEvent*/ )
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CSGEGuiManagerControl::KeyUpEvent()
// -----------------------------------------------------------------------------
//
TBool CSGEGuiManagerControl::KeyUpEvent( const TKeyEvent& /*aKeyEvent*/ )
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CSGEGuiManagerControl::KeyEvent()
// -----------------------------------------------------------------------------
//
TBool CSGEGuiManagerControl::KeyEvent( const TKeyEvent& /*aKeyEvent*/ )
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CSGEGuiManagerControl::PointerEvent()
// -----------------------------------------------------------------------------
//
TBool CSGEGuiManagerControl::PointerEvent( const TPointerEvent &aPointerEvent )
    {
    for ( TInt i = iControls.Count() - 1; i >= 0; i-- )
        {
        if ( iControls[ i ]->Hidden() )
            {
            continue;
            }
        if ( iControls[ i ]->CheckPointerEvent( aPointerEvent.iParentPosition ) )
            {
            if ( aPointerEvent.iType == TPointerEvent::EButton1Down )
                {
                /* preventing too fast control pressing */
                TInt time = iControls[ i ]->MinimumPushedEventTime();
                if ( time )
                    {
                    TTime now;
                    now.HomeTime();
                    if ( iPushedEventControl == iControls[ i ] )
                        {
                        TTimeIntervalMicroSeconds tm = now.MicroSecondsFrom( iPushedEventTime );
                        if ( tm.Int64() < time )
                            {
                            return EFalse;
                            }
                        }
                    iPushedEventTime = now;
                    iPushedEventControl = iControls[ i ];
                    }
                
                /* updating controls states */
                iControls[ i ]->SetPushed( ETrue, aPointerEvent.iPosition );
                for ( TInt j = 0; j < iControls.Count(); j++ )
                    {
                    if ( i != j )
                        {
                        iControls[ j ]->SetPushed( EFalse, aPointerEvent.iPosition );
                        }
                    }
                return ETrue;
                }
            else if ( aPointerEvent.iType == TPointerEvent::EButton1Up )
                {
                iControls[ i ]->SetFocused( ETrue, aPointerEvent.iPosition );
                for ( TInt j = 0; j < iControls.Count(); j++ )
                    {
                    if ( i != j )
                        {
                        iControls[ j ]->SetFocused( EFalse, aPointerEvent.iPosition );
                        }
                    }
                return ETrue;
                }
            }
        }
    
    if ( aPointerEvent.iType == TPointerEvent::EButton1Down || 
         aPointerEvent.iType == TPointerEvent::EButton1Up )
        {
        for ( TInt j = 0; j < iControls.Count(); j++ )
            {
            if ( !iControls[ j ]->Hidden() )
                {
                iControls[ j ]->SetPushed( EFalse, aPointerEvent.iPosition );
                }
            }
        return ETrue;
        }
        
    return EFalse;
    }


// End of file
