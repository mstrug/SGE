/*
 ============================================================================
  Name        : SGEGameAppViewDS.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

// INCLUDE FILES
#include "SGEGameAppViewDS.h"
#include "SGEGameApplication.h"
#include <EIKENV.H> 


// -----------------------------------------------------------------------------
// CSGEGameAppViewDS::NewL()
// -----------------------------------------------------------------------------
//
CSGEGameAppViewDS* CSGEGameAppViewDS::NewL( const TRect& aRect, CSGEGameApplication& aApplication )
    {
    CSGEGameAppViewDS* self = CSGEGameAppViewDS::NewLC( aRect, aApplication );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewDS::NewLC()
// -----------------------------------------------------------------------------
//
CSGEGameAppViewDS* CSGEGameAppViewDS::NewLC( const TRect& aRect, CSGEGameApplication& aApplication )
    {
    CSGEGameAppViewDS* self = new (ELeave) CSGEGameAppViewDS( aApplication );
    CleanupStack::PushL( self );
    self->ConstructL( aRect );
    return self;
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewDS::ConstructL()
// -----------------------------------------------------------------------------
//
void CSGEGameAppViewDS::ConstructL( const TRect& aRect )
    {
    CSGESystemAppView::ConstructL( aRect );
    
    CreateDirectScreenAccessL();
    
    CreateOffscreenBitmapL();
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewDS::CreateDirectScreenAccessL()
// -----------------------------------------------------------------------------
//
void CSGEGameAppViewDS::CreateDirectScreenAccessL()
    {
    iDirectScreenAccess = CDirectScreenAccess::NewL( iEikonEnv->WsSession(),
                                                     *iEikonEnv->ScreenDevice(), 
                                                     Window(), *this );
    iDirectScreenAccess->StartL();
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewDS::CreateOffscreenBitmapL()
// -----------------------------------------------------------------------------
//
void CSGEGameAppViewDS::CreateOffscreenBitmapL()
    {
    iOffScreenBitmap = new (ELeave) CWsBitmap( iCoeEnv->WsSession() );
    iOffScreenBitmap->Create( Size(), iDirectScreenAccess->ScreenDevice()->DisplayMode() );
    iOffScreenBitmapDevice = CFbsBitmapDevice::NewL( iOffScreenBitmap );
    iOffScreenBitmapDevice->CreateContext( iOffScreenBitmapGc );
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewDS::CSGEGameAppViewDS()
// -----------------------------------------------------------------------------
//
CSGEGameAppViewDS::CSGEGameAppViewDS( CSGEGameApplication& aApplication ) : CSGESystemAppView( aApplication )
    {
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewDS::~CSGEGameAppViewDS()
// -----------------------------------------------------------------------------
//
CSGEGameAppViewDS::~CSGEGameAppViewDS()
    {
    if ( iDirectScreenAccess )
        {
        iDirectScreenAccess->Cancel();
        delete iDirectScreenAccess;
        }
    
    delete iOffScreenBitmapGc;
    delete iOffScreenBitmapDevice;
    delete iOffScreenBitmap;
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewDS::Gc()
// -----------------------------------------------------------------------------
//
CFbsBitGc* CSGEGameAppViewDS::Gc()
    {
    return iOffScreenBitmapGc;
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewDS::DisplayMode()
// -----------------------------------------------------------------------------
//
TDisplayMode CSGEGameAppViewDS::DisplayMode()
    {
    return iOffScreenBitmap->DisplayMode();
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewDS::BitBltOffscreenBitmap()
// -----------------------------------------------------------------------------
//
void CSGEGameAppViewDS::BitBltOffscreenBitmap()
    {
    if ( iGc )
        {
        iGc->BitBlt( Rect().iTl, iOffScreenBitmap );

        iDirectScreenAccess->ScreenDevice()->Update();
        }
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewDS::RecreateOffscreenBitmapL()
// -----------------------------------------------------------------------------
//
void CSGEGameAppViewDS::RecreateOffscreenBitmapL()
    {
    delete iOffScreenBitmapGc;
    iOffScreenBitmapGc = NULL;
    delete iOffScreenBitmapDevice;
    iOffScreenBitmapDevice = NULL;
    delete iOffScreenBitmap;
    iOffScreenBitmap = NULL;

    CreateOffscreenBitmapL();
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewDS::SizeChanged()
// -----------------------------------------------------------------------------
//
void CSGEGameAppViewDS::SizeChanged()
    {
    if ( iOffScreenBitmap && iOffScreenBitmap->SizeInPixels() != Size() )
        {
        TRAPD( err, RecreateOffscreenBitmapL() );
        if ( err != KErrNone )
            {
            iApplication.Exit();
            }
        else
            {
            CSGESystemAppView::SizeChanged();
            }
        }
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewDS::Restart()
// -----------------------------------------------------------------------------
//
void CSGEGameAppViewDS::Restart( RDirectScreenAccess::TTerminationReasons /*aReason*/ )
    {
    TRAPD( dsaErr, iDirectScreenAccess->StartL() );
    if ( dsaErr == KErrNone )
        {
        iGc = iDirectScreenAccess->Gc();
        iRegion = iDirectScreenAccess->DrawingRegion();
        iGc->SetClippingRegion( iRegion );
        }
    }

// -----------------------------------------------------------------------------
// CSGEGameAppViewDS::AbortNow()
// -----------------------------------------------------------------------------
//
void CSGEGameAppViewDS::AbortNow( RDirectScreenAccess::TTerminationReasons /*aReason*/ )
    {
    iGc = NULL;
    }

// End of File
