/*
========================================================================
 Name        : SGEAudioTesterSettingItemListView.h
 Author      : Michal Strug
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : 
========================================================================
*/
#ifndef SGEAUDIOTESTERSETTINGITEMLISTVIEW_H
#define SGEAUDIOTESTERSETTINGITEMLISTVIEW_H

// [[[ begin generated region: do not modify [Generated Includes]
#include <aknview.h>
#include "SGEAudioTesterSettingItemListSettings.h"
// ]]] end generated region [Generated Includes]


// [[[ begin [Event Handler Includes]
// ]]] end [Event Handler Includes]

// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

// [[[ begin generated region: do not modify [Generated Forward Declarations]
class CSGEAudioTesterSettingItemList;
// ]]] end generated region [Generated Forward Declarations]

/**
 * Avkon view class for SGEAudioTesterSettingItemListView. It is register with the view server
 * by the AppUi. It owns the container control.
 * @class    CSGEAudioTesterSettingItemListView SGEAudioTesterSettingItemListView.h
 */                        
            
class CSGEAudioTesterSettingItemListView : public CAknView
    {
    
    
    // [[[ begin [Public Section]
public:
    // constructors and destructor
    CSGEAudioTesterSettingItemListView();
    static CSGEAudioTesterSettingItemListView* NewL();
    static CSGEAudioTesterSettingItemListView* NewLC();        
    void ConstructL();
    virtual ~CSGEAudioTesterSettingItemListView();
                        
    // from base class CAknView
    TUid Id() const;
    void HandleCommandL( TInt aCommand );
    
    // [[[ begin generated region: do not modify [Generated Methods]
    // ]]] end generated region [Generated Methods]
    
    // ]]] end [Public Section]
    
    
    // [[[ begin [Protected Section]
protected:
    // from base class CAknView
    void DoActivateL(
        const TVwsViewId& aPrevViewId,
        TUid aCustomMessageId,
        const TDesC8& aCustomMessage );
    void DoDeactivate();
    void HandleStatusPaneSizeChange();
    
    // [[[ begin generated region: do not modify [Overridden Methods]
    // ]]] end generated region [Overridden Methods]
    
    
    // [[[ begin [User Handlers]
    TBool HandleChangeSelectedSettingItemL( TInt aCommand );
    TBool HandleControlPaneLeftSoftKeyPressedL( TInt aCommand );
    TBool HandleControlPaneMiddleSoftKeyPressedL( TInt aCommand );
    TBool HandleControlPaneRightSoftKeyPressedL( TInt aCommand );
    // ]]] end [User Handlers]
    
    // ]]] end [Protected Section]
    
    
    // [[[ begin [Private Section]
private:
    void SetupStatusPaneL();
    void CleanupStatusPane();
    
    // [[[ begin generated region: do not modify [Generated Instance Variables]
	CSGEAudioTesterSettingItemList* iSGEAudioTesterSettingItemList;
	TSGEAudioTesterSettingItemListSettings* iSettings;
    // ]]] end generated region [Generated Instance Variables]
    
    // [[[ begin generated region: do not modify [Generated Methods]
    // ]]] end generated region [Generated Methods]
    
    // ]]] end [Private Section]
    
    void LoadSettingsL();
    };

#endif // SGEAUDIOTESTERSETTINGITEMLISTVIEW_H
