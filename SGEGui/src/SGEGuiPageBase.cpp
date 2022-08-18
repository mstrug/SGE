/*
 ============================================================================
 Name		: SGEGuiPageBase.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEGuiPageBase implementation
 ============================================================================
 */

#include "SGEGuiPageBase.h"
#include "SGEGuiManagerControl.h"


// -----------------------------------------------------------------------------
// CSGEGuiPageBase::CSGEGuiPageBase()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEGuiPageBase::CSGEGuiPageBase( TUint aPageId, TSize aSize )
    : iPageId( aPageId ), iSize( aSize )
    {
    // No implementation required
    }

// -----------------------------------------------------------------------------
// CSGEGuiPageBase::~CSGEGuiPageBase()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEGuiPageBase::~CSGEGuiPageBase()
    {
    delete iControlManager;
    }

// -----------------------------------------------------------------------------
// CSGEGuiPageBase::BaseConstructL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEGuiPageBase::BaseConstructL()
    {
    iControlManager = CSGEGuiManagerControl::NewL();
    }

// -----------------------------------------------------------------------------
// CSGEGuiPageBase::ActivateL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEGuiPageBase::ActivateL()
    {
    iControlManager->ResetControls();
    }

// -----------------------------------------------------------------------------
// CSGEGuiPageBase::DeactivateL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEGuiPageBase::DeactivateL()
    {
    }

// -----------------------------------------------------------------------------
// CSGEGuiPageBase::GetRedrawRect()
// -----------------------------------------------------------------------------
//
EXPORT_C TRect CSGEGuiPageBase::GetRedrawRect( TRect aRect ) const
    {
    return iControlManager->GetRedrawRect( aRect );
    }

// -----------------------------------------------------------------------------
// CSGEGuiPageBase::Id()
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CSGEGuiPageBase::Id() const
    {
    return iPageId;
    }
    
// -----------------------------------------------------------------------------
// CSGEGuiPageBase::Draw()
// -----------------------------------------------------------------------------
//
void CSGEGuiPageBase::Draw( CBitmapContext* aGc, const TRect aDrawRect )
    {
    iControlManager->Draw( aGc, aDrawRect );
    }

// -----------------------------------------------------------------------------
// CSGEGuiPageBase::Update()
// -----------------------------------------------------------------------------
//
TBool CSGEGuiPageBase::Update( const TTimeIntervalMicroSeconds32 aTimeInterval )
    {
    return iControlManager->Update( aTimeInterval );
    }

// -----------------------------------------------------------------------------
// CSGEGuiPageBase::KeyDownEvent()
// -----------------------------------------------------------------------------
//
TBool CSGEGuiPageBase::KeyDownEvent( const TKeyEvent &aKeyEvent )
    {
    return iControlManager->KeyDownEvent( aKeyEvent );
    }

// -----------------------------------------------------------------------------
// CSGEGuiPageBase::KeyUpEvent()
// -----------------------------------------------------------------------------
//
TBool CSGEGuiPageBase::KeyUpEvent( const TKeyEvent &aKeyEvent )
    {
    return iControlManager->KeyUpEvent( aKeyEvent );
    }

// -----------------------------------------------------------------------------
// CSGEGuiPageBase::KeyEvent()
// -----------------------------------------------------------------------------
//
TBool CSGEGuiPageBase::KeyEvent( const TKeyEvent &aKeyEvent )
    {
    return iControlManager->KeyEvent( aKeyEvent );
    }

// -----------------------------------------------------------------------------
// CSGEGuiPageBase::PointerEvent()
// -----------------------------------------------------------------------------
//
TBool CSGEGuiPageBase::PointerEvent( const TPointerEvent &aPointerEvent )
    {
    return iControlManager->PointerEvent( aPointerEvent );
    }


// End of file
