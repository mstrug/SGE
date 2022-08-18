/*
 ============================================================================
  Name        : SGEGameAppViewDS.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#ifndef __SGEGAMEAPPVIEWDS_H__
#define __SGEGAMEAPPVIEWDS_H__

// INCLUDES
#include "SGESystemAppView.h"

// FORWARD DECLARATIONS
class CSGEGameApplication;

// CLASS DECLARATION
/**
 *  CSGEGameAppViewDS
 * 
 */
NONSHARABLE_CLASS( CSGEGameAppViewDS ) : public CSGESystemAppView, public MDirectScreenAccess
    {
public:

    static CSGEGameAppViewDS* NewL( const TRect& aRect, CSGEGameApplication& aApplication );

    static CSGEGameAppViewDS* NewLC( const TRect& aRect, CSGEGameApplication& aApplication );

    virtual ~CSGEGameAppViewDS();

    CFbsBitGc* Gc();
    
    TDisplayMode DisplayMode();
    
    virtual void BitBltOffscreenBitmap();
    
protected:

    void ConstructL( const TRect& aRect );

    CSGEGameAppViewDS( CSGEGameApplication& aApplication );

    void CreateDirectScreenAccessL();

    void CreateOffscreenBitmapL();
    
    void RecreateOffscreenBitmapL();

protected: // from base class
    
    void SizeChanged();
    
protected: // from MDirectScreenAccess 

    virtual void Restart( RDirectScreenAccess::TTerminationReasons aReason );

    virtual void AbortNow( RDirectScreenAccess::TTerminationReasons aReason );

protected: // data
    
    CDirectScreenAccess *iDirectScreenAccess; // owned
    
    CFbsBitGc *iGc; // not owned, by ref, from DSA
    
    RRegion *iRegion; // not owned, by ref, from DSA

    CFbsBitmap *iOffScreenBitmap; // owned

    CFbsBitmapDevice *iOffScreenBitmapDevice; // owned

    CFbsBitGc* iOffScreenBitmapGc; // owned

    };

#endif // __SGEGAMEAPPVIEWDS_H__
