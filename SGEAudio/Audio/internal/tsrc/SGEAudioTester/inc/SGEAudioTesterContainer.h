/*
========================================================================
 Name        : SGEAudioTesterContainer.h
 Author      : Michal Strug
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : 
========================================================================
*/
#ifndef SGEAUDIOTESTERCONTAINER_H
#define SGEAUDIOTESTERCONTAINER_H

// [[[ begin generated region: do not modify [Generated Includes]
#include <coecntrl.h>		
// ]]] end generated region [Generated Includes]

// [[[ begin [Event Handler Includes]
// ]]] end [Event Handler Includes]

// [[[ begin generated region: do not modify [Generated Forward Declarations]
class MEikCommandObserver;		
class CEikLabel;
// ]]] end generated region [Generated Forward Declarations]

/**
 * Container class for SGEAudioTesterContainer
 * 
 * @class    CSGEAudioTesterContainer SGEAudioTesterContainer.h
 */
class CSGEAudioTesterContainer : public CCoeControl
    {
public:
    // constructors and destructor
    CSGEAudioTesterContainer();
    static CSGEAudioTesterContainer* NewL( 
        const TRect& aRect, 
        const CCoeControl* aParent, 
        MEikCommandObserver* aCommandObserver );
    static CSGEAudioTesterContainer* NewLC( 
        const TRect& aRect, 
        const CCoeControl* aParent, 
        MEikCommandObserver* aCommandObserver );
    void ConstructL( 
        const TRect& aRect, 
        const CCoeControl* aParent, 
        MEikCommandObserver* aCommandObserver );
    virtual ~CSGEAudioTesterContainer();

public:
    // from base class CCoeControl
    TInt CountComponentControls() const;
    CCoeControl* ComponentControl( TInt aIndex ) const;
    TKeyResponse OfferKeyEventL( 
            const TKeyEvent& aKeyEvent, 
            TEventCode aType );
    void HandleResourceChange( TInt aType );
    void HandlePointerEventL(const TPointerEvent& aPointerEvent);
    
protected:
    // from base class CCoeControl
    void SizeChanged();

private:
    // from base class CCoeControl
    void Draw( const TRect& aRect ) const;

private:
    void InitializeControlsL();
    void LayoutControls();
    CCoeControl* iFocusControl;
    MEikCommandObserver* iCommandObserver;
    // [[[ begin generated region: do not modify [Generated Methods]
public: 
    // ]]] end generated region [Generated Methods]
    
    // [[[ begin generated region: do not modify [Generated Type Declarations]
public: 
    // ]]] end generated region [Generated Type Declarations]
    
    // [[[ begin generated region: do not modify [Generated Instance Variables]
private: 
	CEikLabel* iLabelTrack1;
    // ]]] end generated region [Generated Instance Variables]
    
    
    // [[[ begin [Overridden Methods]
protected: 
    // ]]] end [Overridden Methods]
    
    
    // [[[ begin [User Handlers]
protected: 
    // ]]] end [User Handlers]
    
public: 
    enum TControls
        {
        // [[[ begin generated region: do not modify [Generated Contents]
		ELabelTrack1,
		
        // ]]] end generated region [Generated Contents]
        
        // add any user-defined entries here...
        
        ELastControl
        };

public:
    
    void SetLabelTextL( TInt aNumber, const TDesC& aText );

private:
    
    static TInt CleanTapRect( TAny *aParam );
    
private:
    
    CFont *iLableFont;
    CFont *iTapFont;
    
    TPoint iTapRectDivider;
    
    CPeriodic *iPeriodic;
    
    };
                
#endif // SGEAUDIOTESTERCONTAINER_H
