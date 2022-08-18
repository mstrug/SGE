/*
 ============================================================================
  Name        : SGEGameAppViewDSB.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#ifndef __SGEGAMEAPPVIEWDSB_H__
#define __SGEGAMEAPPVIEWDSB_H__

// INCLUDES
#include "SGEGameAppViewDS.h"
#include "SGEGameAppDSB.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
 *  CSGEGameAppViewDSB
 * 
 */
NONSHARABLE_CLASS( CSGEGameAppViewDSB ) : public CSGEGameAppViewDS, public MSGEGameAppDSBObserver
    {
public:

    static CSGEGameAppViewDSB* NewL( const TRect& aRect, CSGEGameApplication& aApplication );

    static CSGEGameAppViewDSB* NewLC( const TRect& aRect, CSGEGameApplication& aApplication );

    virtual ~CSGEGameAppViewDSB();

    void BitBltOffscreenBitmap();

private:

    void ConstructL( const TRect& aRect );

    CSGEGameAppViewDSB( CSGEGameApplication& aApplication );

private: // from base class

    void Restart( RDirectScreenAccess::TTerminationReasons aReason );

    void AbortNow( RDirectScreenAccess::TTerminationReasons aReason );

private: // from MSGEGameAppDSBObserver

    void ProcessFrame();

private: // data
    
    CSGEGameAppDSB *iDSB; // owned
    
    };

#endif // __SGEGAMEAPPVIEWDS_H__
