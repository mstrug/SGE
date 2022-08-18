/*
 ============================================================================
  Name        : SGEGameAppViewDSB.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

// INCLUDE FILES
#include "SGEGameAppViewDSB.h"


// -----------------------------------------------------------------------------
// CSGEGameAppViewDSB::NewL()
// -----------------------------------------------------------------------------
//
CSGEGameAppViewDSB* CSGEGameAppViewDSB::NewL( const TRect& aRect, CSGEGameApplication& aApplication )
    {
    CSGEGameAppViewDSB* self = CSGEGameAppViewDSB::NewLC( aRect, aApplication );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewDSB::NewLC()
// -----------------------------------------------------------------------------
//
CSGEGameAppViewDSB* CSGEGameAppViewDSB::NewLC( const TRect& aRect, CSGEGameApplication& aApplication )
    {
    CSGEGameAppViewDSB* self = new (ELeave) CSGEGameAppViewDSB( aApplication );
    CleanupStack::PushL( self );
    self->ConstructL( aRect );
    return self;
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewDSB::ConstructL()
// -----------------------------------------------------------------------------
//
void CSGEGameAppViewDSB::ConstructL( const TRect& aRect )
    {
    CSGEGameAppViewDS::ConstructL( aRect );

    iDSB = CSGEGameAppDSB::NewL( *this );        
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewDSB::CSGEGameAppViewDSB()
// -----------------------------------------------------------------------------
//
CSGEGameAppViewDSB::CSGEGameAppViewDSB( CSGEGameApplication& aApplication ) : CSGEGameAppViewDS( aApplication )
    {
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewDSB::~CSGEGameAppViewDSB()
// -----------------------------------------------------------------------------
//
CSGEGameAppViewDSB::~CSGEGameAppViewDSB()
    {
    delete iDSB;
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewDSB::BitBltOffscreenBitmap()
// -----------------------------------------------------------------------------
//
void CSGEGameAppViewDSB::BitBltOffscreenBitmap()
    {
    if ( iGc )
        {
        TAcceleratedBitmapInfo bi = iDSB->BeginDraw();
        
        TSize size = iOffScreenBitmap->SizeInPixels();
        TInt scanLineLength = CFbsBitmap::ScanLineLength( size.iWidth, iOffScreenBitmap->DisplayMode() );
        
        iOffScreenBitmap->LockHeap();
        
        Mem::Copy( bi.iAddress, iOffScreenBitmap->DataAddress(), size.iHeight * scanLineLength );
        
        iOffScreenBitmap->UnlockHeap();
        
        iDSB->EndDraw();
        }
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewDSB::Restart()
// -----------------------------------------------------------------------------
//
void CSGEGameAppViewDSB::Restart( RDirectScreenAccess::TTerminationReasons aReason )
    {
    CSGEGameAppViewDS::Restart( aReason );
    if ( iGc )
        {
        if ( iDSB->Create( TRect( TPoint(), Size() ) ) != KErrNone )
            {
            iGc = NULL;
            }
        }
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewDSB::AbortNow()
// -----------------------------------------------------------------------------
//
void CSGEGameAppViewDSB::AbortNow( RDirectScreenAccess::TTerminationReasons aReason )
    {
    CSGEGameAppViewDS::AbortNow( aReason );
    iDSB->Close();
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewDSB::ProcessFrame()
// -----------------------------------------------------------------------------
//
void CSGEGameAppViewDSB::ProcessFrame()
    {
    }

// End of File
