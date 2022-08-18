/*
 ============================================================================
  Name        : SGESystemAppDSB.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#ifndef __SGEGAMEAPPDSB_H__
#define __SGEGAMEAPPDSB_H__

// INCLUDES
#include <e32base.h>
#include <cdsb.h>

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
 * MSGEGameAppDSBObserver
 *
 */
class MSGEGameAppDSBObserver
    {
public:

    virtual void ProcessFrame() = 0;

    };

/**
 *  CSGEGameAppDSB
 * 
 */
NONSHARABLE_CLASS( CSGEGameAppDSB ) : public CActive
    {
public:

    static CSGEGameAppDSB* NewL( MSGEGameAppDSBObserver& aObserver );

    static CSGEGameAppDSB* NewLC( MSGEGameAppDSBObserver& aObserver );

    virtual ~CSGEGameAppDSB();

    TInt Create( TRect aRect );

    void Close();

    TAcceleratedBitmapInfo BeginDraw();

    void EndDraw();

private:

    void ConstructL();

    CSGEGameAppDSB( MSGEGameAppDSBObserver& aObserver );

private: // from base class
    
    void DoCancel();

    void RunL();

private: // data
    
    CDirectScreenBitmap *iDirectScreenBitmap; // owned

    MSGEGameAppDSBObserver& iObserver;

    };

#endif // __SGEGAMEAPPDSB_H__
