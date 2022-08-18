/*
 ============================================================================
  Name        : SGEGameAppDSB.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

// INCLUDE FILES
#include "SGEGameAppDSB.h"


// -----------------------------------------------------------------------------
// CSGEGameAppDSB::NewL()
// -----------------------------------------------------------------------------
//
CSGEGameAppDSB* CSGEGameAppDSB::NewL( MSGEGameAppDSBObserver& aObserver )
    {
    CSGEGameAppDSB* self = CSGEGameAppDSB::NewLC( aObserver );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CSGEGameAppDSB::NewLC()
// -----------------------------------------------------------------------------
//
CSGEGameAppDSB* CSGEGameAppDSB::NewLC( MSGEGameAppDSBObserver& aObserver )
    {
    CSGEGameAppDSB* self = new (ELeave) CSGEGameAppDSB( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CSGEGameAppDSB::ConstructL()
// -----------------------------------------------------------------------------
//
void CSGEGameAppDSB::ConstructL()
    {
    iDirectScreenBitmap = CDirectScreenBitmap::NewL();
    
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CSGEGameAppDSB::CSGEGameAppDSB()
// -----------------------------------------------------------------------------
//
CSGEGameAppDSB::CSGEGameAppDSB( MSGEGameAppDSBObserver& aObserver ) 
        : CActive( EPriorityNormal ), iObserver( aObserver )
    {
    }

// -----------------------------------------------------------------------------
// CSGEGameAppDSB::~CSGEGameAppDSB()
// -----------------------------------------------------------------------------
//
CSGEGameAppDSB::~CSGEGameAppDSB()
    {
    Cancel();
    
    if ( iDirectScreenBitmap )
        {
        Close();
        delete iDirectScreenBitmap;
        }
    }

// -----------------------------------------------------------------------------
// CSGEGameAppDSB::CreateL()
// -----------------------------------------------------------------------------
//
TInt CSGEGameAppDSB::Create( TRect aRect )
    {
    return iDirectScreenBitmap->Create( aRect, CDirectScreenBitmap::EDoubleBuffer );
    }

// -----------------------------------------------------------------------------
// CSGEGameAppDSB::Close()
// -----------------------------------------------------------------------------
//
void CSGEGameAppDSB::Close()
    {
    Cancel();
    iDirectScreenBitmap->Close();
    }

// -----------------------------------------------------------------------------
// CSGEGameAppDSB::BeginDraw()
// -----------------------------------------------------------------------------
//
TAcceleratedBitmapInfo CSGEGameAppDSB::BeginDraw()
    {
    // Obtain the screen address every time before drawing the frame, 
    // since the address always changes
    TAcceleratedBitmapInfo bitmapInfo;
    iDirectScreenBitmap->BeginUpdate( bitmapInfo );        
    return bitmapInfo;
    }

// -----------------------------------------------------------------------------
// CSGEGameAppDSB::EndDraw()
// -----------------------------------------------------------------------------
//
void CSGEGameAppDSB::EndDraw()
    {
    if ( IsActive() )
        {
        Cancel();
        }
    iDirectScreenBitmap->EndUpdate( iStatus );
    SetActive();
    // We don't need to wait to complete the request
    // We can start making the next frame instead.
    iObserver.ProcessFrame();
    }

// -----------------------------------------------------------------------------
// CSGEGameAppDSB::DoCancel()
// -----------------------------------------------------------------------------
//
void CSGEGameAppDSB::DoCancel()
    {
    }

// -----------------------------------------------------------------------------
// CSGEGameAppDSB::RunL()
// -----------------------------------------------------------------------------
//
void CSGEGameAppDSB::RunL()
    {
    }


// End of File

