/*
 ============================================================================
  Name        : SGESystemAppView.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#ifndef __SGESYSTEMAPPVIEW_H__
#define __SGESYSTEMAPPVIEW_H__

// INCLUDES
#include <coecntrl.h>

// FORWARD DECLARATIONS
class CSGEApplication;

// CLASS DECLARATION
/**
 *  CSGESystemAppView
 * 
 */
NONSHARABLE_CLASS( CSGESystemAppView ) : public CCoeControl
    {
public:

    static CSGESystemAppView* NewL( const TRect& aRect, CSGEApplication& aApplication );

    static CSGESystemAppView* NewLC( const TRect& aRect, CSGEApplication& aApplication );

    virtual ~CSGESystemAppView();

    RWindow& WindowControl() const;

    RWsSession& WindowSession() const;
    
    RFs& Fs() const;
    
    void EnableDragEvents();
    
protected:

    virtual void ConstructL( const TRect& aRect );

    CSGESystemAppView( CSGEApplication& aApplication );

protected: // from base class

    void Draw( const TRect& aRect ) const;

    virtual void SizeChanged();

    void HandlePointerEventL( const TPointerEvent& aPointerEvent );

    void HandleResourceChange( TInt aType );
    
protected: // data
    
    CSGEApplication& iApplication;
    
    TBool iCreated;
    
    };

#endif // __SGESYSTEMAPPVIEW_H__
