/*
========================================================================
 Name        : SGEAudioTesterAppUi.h
 Author      : Michal Strug
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : 
========================================================================
*/
#ifndef SGEAUDIOTESTERAPPUI_H
#define SGEAUDIOTESTERAPPUI_H

// [[[ begin generated region: do not modify [Generated Includes]
#include <aknviewappui.h>
// ]]] end generated region [Generated Includes]

// [[[ begin generated region: do not modify [Generated Forward Declarations]
class CSGEAudioTesterContainerView;
class CSGEAudioTesterSettingItemListView;
class CSGEAudioTesterSettingItemListFile1View;
// ]]] end generated region [Generated Forward Declarations]

/**
 * @class    CSGEAudioTesterAppUi SGEAudioTesterAppUi.h
 * @brief The AppUi class handles application-wide aspects of the user interface, including
 *        view management and the default menu, control pane, and status pane.
 */
class CSGEAudioTesterAppUi : public CAknViewAppUi
    {
public: 
    // constructor and destructor
    CSGEAudioTesterAppUi();
    virtual ~CSGEAudioTesterAppUi();
    void ConstructL();

public:
    // from CCoeAppUi
    TKeyResponse HandleKeyEventL(
                const TKeyEvent& aKeyEvent,
                TEventCode aType );

    // from CEikAppUi
    void HandleCommandL( TInt aCommand );
    void HandleResourceChangeL( TInt aType );

    // from CAknAppUi
    void HandleViewDeactivation( 
            const TVwsViewId& aViewIdToBeDeactivated, 
            const TVwsViewId& aNewlyActivatedViewId );

private:
    void InitializeContainersL();
    // [[[ begin generated region: do not modify [Generated Methods]
public: 
    // ]]] end generated region [Generated Methods]
    
    // [[[ begin generated region: do not modify [Generated Instance Variables]
private: 
	CSGEAudioTesterContainerView* iSGEAudioTesterContainerView;
	CSGEAudioTesterSettingItemListView* iSGEAudioTesterSettingItemListView;
	CSGEAudioTesterSettingItemListFile1View* iSGEAudioTesterSettingItemListFile1View;
    // ]]] end generated region [Generated Instance Variables]
    
    
    // [[[ begin [User Handlers]
protected: 
    // ]]] end [User Handlers]
    
    };

#endif // SGEAUDIOTESTERAPPUI_H            
