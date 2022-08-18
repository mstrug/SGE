/*
========================================================================
 Name        : SGEAudioTesterSettingItemListFile1View.h
 Author      : ${author}
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : 
========================================================================
*/
#ifndef SGEAUDIOTESTERSETTINGITEMLISTFILE1VIEW_H
#define SGEAUDIOTESTERSETTINGITEMLISTFILE1VIEW_H

// [[[ begin generated region: do not modify [Generated Includes]
#include <aknview.h>
#include "SGEAudioTesterSettingItemListFile1Settings.h"
// ]]] end generated region [Generated Includes]

#include <SGEAudio/SGEAudioConfig.h>

// [[[ begin [Event Handler Includes]
// ]]] end [Event Handler Includes]

// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

// [[[ begin generated region: do not modify [Generated Forward Declarations]
class CAknNavigationDecorator;
class CSGEAudioTesterSettingItemListFile1;
// ]]] end generated region [Generated Forward Declarations]

/**
 * Avkon view class for SGEAudioTesterSettingItemListFile1View. It is register with the view server
 * by the AppUi. It owns the container control.
 * @class	CSGEAudioTesterSettingItemListFile1View SGEAudioTesterSettingItemListFile1View.h
 */						
			
class CSGEAudioTesterSettingItemListFile1View : public CAknView
	{
	
	
	// [[[ begin [Public Section]
public:
	// constructors and destructor
	CSGEAudioTesterSettingItemListFile1View();
	static CSGEAudioTesterSettingItemListFile1View* NewL();
	static CSGEAudioTesterSettingItemListFile1View* NewLC();        
	void ConstructL();
	virtual ~CSGEAudioTesterSettingItemListFile1View();
						
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
	// any current navi decorator
	CAknNavigationDecorator* iNaviDecorator_;
	CSGEAudioTesterSettingItemListFile1* iSGEAudioTesterSettingItemListFile1;
	TSGEAudioTesterSettingItemListFile1Settings* iSettings;
	// ]]] end generated region [Generated Instance Variables]
	
	// [[[ begin generated region: do not modify [Generated Methods]
	// ]]] end generated region [Generated Methods]
	
	// ]]] end [Private Section]
	
public:
    void ChangeStatusPaneTextL( const TDesC& aText );

    void SetSettingsL( TInt aTrackNumber, TInt aFileNameNumber, TSGEAudioTrackState aTrackState );
	
private:
    
    struct SSettings
        {
        TInt iTrackNumber;
        TInt iFileNameNumber;
        TSGEAudioTrackState iTrackState;
        };

    SSettings iCurrentSettings;
    
	};

#endif // SGEAUDIOTESTERSETTINGITEMLISTFILE1VIEW_H
