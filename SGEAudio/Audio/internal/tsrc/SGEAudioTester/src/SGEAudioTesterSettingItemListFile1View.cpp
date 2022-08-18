/*
========================================================================
 Name        : SGEAudioTesterSettingItemListFile1View.cpp
 Author      : ${author}
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : 
========================================================================
*/
// [[[ begin generated region: do not modify [Generated System Includes]
#include <aknviewappui.h>
#include <eikmenub.h>
#include <avkon.hrh>
#include <akncontext.h>
#include <akntitle.h>
#include <stringloader.h>
#include <barsread.h>
#include <aknnavide.h>
#include <aknnavi.h>
#include <eikbtgpc.h>
#include <SGEAudioTester.rsg>
// ]]] end generated region [Generated System Includes]

// [[[ begin generated region: do not modify [Generated User Includes]

#include "SGEAudioTester.hrh"
#include "SGEAudioTesterSettingItemListFile1View.h"
#include "SGEAudioTesterContainer.hrh"
#include "SGEAudioTesterSettingItemList.hrh"
#include "SGEAudioTesterSettingItemListFile1.hrh"
#include "SGEAudioTesterSettingItemListFile1.h"
// ]]] end generated region [Generated User Includes]

#include "SGEAudioTesterContainerView.h"

// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

/**
 * First phase of Symbian two-phase construction. Should not contain any
 * code that could leave.
 */
CSGEAudioTesterSettingItemListFile1View::CSGEAudioTesterSettingItemListFile1View()
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	iNaviDecorator_ = NULL;
	// ]]] end generated region [Generated Contents]
	
	}

/** 
 * The view's destructor removes the container from the control
 * stack and destroys it.
 */
CSGEAudioTesterSettingItemListFile1View::~CSGEAudioTesterSettingItemListFile1View()
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	if ( iNaviDecorator_ != NULL )
		{
		delete iNaviDecorator_;
		iNaviDecorator_ = NULL;
		}
	// ]]] end generated region [Generated Contents]
	
	if ( iSGEAudioTesterSettingItemListFile1 )
	    {
        AppUi()->RemoveFromStack( iSGEAudioTesterSettingItemListFile1 );
	    }
    delete iSGEAudioTesterSettingItemListFile1;
    delete iSettings;
	}

/**
 * Symbian two-phase constructor.
 * This creates an instance then calls the second-phase constructor
 * without leaving the instance on the cleanup stack.
 * @return new instance of CSGEAudioTesterSettingItemListFile1View
 */
CSGEAudioTesterSettingItemListFile1View* CSGEAudioTesterSettingItemListFile1View::NewL()
	{
	CSGEAudioTesterSettingItemListFile1View* self = CSGEAudioTesterSettingItemListFile1View::NewLC();
	CleanupStack::Pop( self );
	return self;
	}

/**
 * Symbian two-phase constructor.
 * This creates an instance, pushes it on the cleanup stack,
 * then calls the second-phase constructor.
 * @return new instance of CSGEAudioTesterSettingItemListFile1View
 */
CSGEAudioTesterSettingItemListFile1View* CSGEAudioTesterSettingItemListFile1View::NewLC()
	{
	CSGEAudioTesterSettingItemListFile1View* self = new ( ELeave ) CSGEAudioTesterSettingItemListFile1View();
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}


/**
 * Second-phase constructor for view.  
 * Initialize contents from resource.
 */ 
void CSGEAudioTesterSettingItemListFile1View::ConstructL()
	{
	// [[[ begin generated region: do not modify [Generated Code]
	BaseConstructL( R_SGEAUDIO_TESTER_SETTING_ITEM_LIST_FILE1_SGEAUDIO_TESTER_SETTING_ITEM_LIST_FILE1_VIEW );
				
	// ]]] end generated region [Generated Code]
	
	// add your own initialization code here
	
	}

/**
 * @return The UID for this view
 */
TUid CSGEAudioTesterSettingItemListFile1View::Id() const
	{
	return TUid::Uid( ESGEAudioTesterSettingItemListFile1ViewId );
	}

/**
 * Handle a command for this view (override)
 * @param aCommand command id to be handled
 */
void CSGEAudioTesterSettingItemListFile1View::HandleCommandL( TInt aCommand )
	{
	// [[[ begin generated region: do not modify [Generated Code]
	TBool commandHandled = EFalse;
	switch ( aCommand )
		{	// code to dispatch to the AknView's menu and CBA commands is generated here
	
		case EAknSoftkeySelect:
			commandHandled = HandleControlPaneMiddleSoftKeyPressedL( aCommand );
			break;
	
		case EAknSoftkeyOk:
			commandHandled = HandleControlPaneLeftSoftKeyPressedL( aCommand );
			break;
	
		case EAknSoftkeyCancel:
			commandHandled = HandleControlPaneRightSoftKeyPressedL( aCommand );
			break;
		default:
			break;
		}
	
		
	if ( !commandHandled ) 
		{
	
		if ( aCommand == EAknSoftkeyCancel )
			{
			AppUi()->HandleCommandL( EEikCmdExit );
			}
	
		}
	// ]]] end generated region [Generated Code]
	
	}

/**
 *	Handles user actions during activation of the view, 
 *	such as initializing the content.
 */
void CSGEAudioTesterSettingItemListFile1View::DoActivateL( 
		const TVwsViewId& /*aPrevViewId*/,
		TUid /*aCustomMessageId*/,
		const TDesC8& /*aCustomMessage*/ )
	{
	// [[[ begin generated region: do not modify [Generated Contents]
	SetupStatusPaneL();
	
				
				
	
	if ( iSGEAudioTesterSettingItemListFile1 == NULL )
		{
		iSettings = TSGEAudioTesterSettingItemListFile1Settings::NewL();
		iSGEAudioTesterSettingItemListFile1 = new ( ELeave ) CSGEAudioTesterSettingItemListFile1( *iSettings, this );
		iSGEAudioTesterSettingItemListFile1->SetMopParent( this );
		iSGEAudioTesterSettingItemListFile1->ConstructFromResourceL( R_SGEAUDIO_TESTER_SETTING_ITEM_LIST_FILE1_SGEAUDIO_TESTER_SETTING_ITEM_LIST_FILE1 );
		iSGEAudioTesterSettingItemListFile1->ActivateL();
		iSGEAudioTesterSettingItemListFile1->LoadSettingValuesL();
		iSGEAudioTesterSettingItemListFile1->LoadSettingsL();
		AppUi()->AddToStackL( *this, iSGEAudioTesterSettingItemListFile1 );
		} 
	// ]]] end generated region [Generated Contents]
	
    iSettings->SetEnumeratedTextPopupFileName( iCurrentSettings.iFileNameNumber );
    iSettings->SetEnumeratedTextPopupRepeat( iCurrentSettings.iTrackState.iRepeat );
    iSettings->SetSliderVolume( iCurrentSettings.iTrackState.iVolume );
    iSettings->SetEnumeratedTextPopupKey( iCurrentSettings.iFileNameNumber );
    TBuf<30> name;
    name.Copy( _L("File ") );
    name.AppendNum( iCurrentSettings.iFileNameNumber + 1 );
    ChangeStatusPaneTextL( name );
    iSGEAudioTesterSettingItemListFile1->LoadSettingsL();
	}

/**
 */
void CSGEAudioTesterSettingItemListFile1View::DoDeactivate()
	{
    // [[[ begin generated region: do not modify [Generated Contents]
	CleanupStatusPane();
	
	if ( iSGEAudioTesterSettingItemListFile1 != NULL )
		{
		AppUi()->RemoveFromStack( iSGEAudioTesterSettingItemListFile1 );
		delete iSGEAudioTesterSettingItemListFile1;
		iSGEAudioTesterSettingItemListFile1 = NULL;
		delete iSettings;
		iSettings = NULL;
		}
	// ]]] end generated region [Generated Contents]
	
	}

/** 
 * Handle status pane size change for this view (override)
 */
void CSGEAudioTesterSettingItemListFile1View::HandleStatusPaneSizeChange()
	{
	CAknView::HandleStatusPaneSizeChange();
	
	// this may fail, but we're not able to propagate exceptions here
	TVwsViewId view;
	AppUi()->GetActiveViewId( view );
	if ( view.iViewUid == Id() )
		{
		TInt result;
		TRAP( result, SetupStatusPaneL() );
		}
	
	// [[[ begin generated region: do not modify [Generated Code]
	// ]]] end generated region [Generated Code]
	
	}

// [[[ begin generated function: do not modify
void CSGEAudioTesterSettingItemListFile1View::SetupStatusPaneL()
	{
	// reset the context pane
	TUid contextPaneUid = TUid::Uid( EEikStatusPaneUidContext );
	CEikStatusPaneBase::TPaneCapabilities subPaneContext = 
		StatusPane()->PaneCapabilities( contextPaneUid );
	if ( subPaneContext.IsPresent() && subPaneContext.IsAppOwned() )
		{
		CAknContextPane* context = static_cast< CAknContextPane* > ( 
			StatusPane()->ControlL( contextPaneUid ) );
		context->SetPictureToDefaultL();
		}
	
	// setup the title pane
	TUid titlePaneUid = TUid::Uid( EEikStatusPaneUidTitle );
	CEikStatusPaneBase::TPaneCapabilities subPaneTitle = 
		StatusPane()->PaneCapabilities( titlePaneUid );
	if ( subPaneTitle.IsPresent() && subPaneTitle.IsAppOwned() )
		{
		CAknTitlePane* title = static_cast< CAknTitlePane* >( 
			StatusPane()->ControlL( titlePaneUid ) );
		TResourceReader reader;
		iEikonEnv->CreateResourceReaderLC( reader, R_SGEAUDIO_TESTER_SETTING_ITEM_LIST_FILE1_TITLE_RESOURCE );
		title->SetFromResourceL( reader );
		CleanupStack::PopAndDestroy(); // reader internal state
		}
				
	// set the navi pane content
	TUid naviPaneUid = TUid::Uid( EEikStatusPaneUidNavi );
	CEikStatusPaneBase::TPaneCapabilities subPaneNavi = 
		StatusPane()->PaneCapabilities( naviPaneUid );
	if ( subPaneNavi.IsPresent() && subPaneNavi.IsAppOwned() )
		{
		CAknNavigationControlContainer* naviPane = 
			static_cast< CAknNavigationControlContainer* >( 
				StatusPane()->ControlL( naviPaneUid ) );
		if ( iNaviDecorator_ != NULL )
			{
			delete iNaviDecorator_;
			iNaviDecorator_ = NULL;
			}
				
		HBufC* labelText = StringLoader::LoadLC( R_SGEAUDIO_TESTER_SETTING_ITEM_LIST_FILE1_NAVI_TEXT );
		iNaviDecorator_ = naviPane->CreateNavigationLabelL( *labelText );
		CleanupStack::PopAndDestroy( labelText );			
				
		naviPane->PushL( *iNaviDecorator_ );
		}
				
	}

// ]]] end generated function

// [[[ begin generated function: do not modify
void CSGEAudioTesterSettingItemListFile1View::CleanupStatusPane()
	{
	// reset the navi pane 
	TUid naviPaneUid = TUid::Uid( EEikStatusPaneUidNavi );
	CEikStatusPaneBase::TPaneCapabilities subPaneNavi = 
		StatusPane()->PaneCapabilities( naviPaneUid );
	if ( subPaneNavi.IsPresent() && subPaneNavi.IsAppOwned() )
		{
		CAknNavigationControlContainer* naviPane = 
			static_cast< CAknNavigationControlContainer* >( 
				StatusPane()->ControlL( naviPaneUid ) );
		if ( iNaviDecorator_ != NULL )
			{
			naviPane->Pop( iNaviDecorator_ );
			delete iNaviDecorator_;
			iNaviDecorator_ = NULL;
			}
		}
	
	}

// ]]] end generated function

void CSGEAudioTesterSettingItemListFile1View::ChangeStatusPaneTextL( const TDesC& aText )
    {
    TUid naviPaneUid = TUid::Uid( EEikStatusPaneUidNavi );
    CEikStatusPaneBase::TPaneCapabilities subPaneNavi = 
        StatusPane()->PaneCapabilities( naviPaneUid );
    if ( subPaneNavi.IsPresent() && subPaneNavi.IsAppOwned() )
        {
        CAknNavigationControlContainer* naviPane = 
            static_cast< CAknNavigationControlContainer* >( 
                StatusPane()->ControlL( naviPaneUid ) );
        if ( iNaviDecorator_ != NULL )
            {
            delete iNaviDecorator_;
            iNaviDecorator_ = NULL;
            }
                
        iNaviDecorator_ = naviPane->CreateNavigationLabelL( aText );
        naviPane->PushL( *iNaviDecorator_ );
        }
    }

/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CSGEAudioTesterSettingItemListFile1View::HandleChangeSelectedSettingItemL( TInt aCommand )
	{
	// TODO: implement selected event handler
	return ETrue;
	}
				

/** 
 * Handle the leftSoftKeyPressed event.
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CSGEAudioTesterSettingItemListFile1View::HandleControlPaneLeftSoftKeyPressedL( TInt aCommand )
	{
    iCurrentSettings.iFileNameNumber = iSettings->EnumeratedTextPopupFileName();
    iCurrentSettings.iTrackState.iRepeat = iSettings->EnumeratedTextPopupRepeat();
    iCurrentSettings.iTrackState.iVolume = iSettings->SliderVolume();
    
    static_cast< CSGEAudioTesterContainerView* >( 
                        AppUi()->View( TUid::Uid( ESGEAudioTesterContainerViewId ) ) )->
                        SetTrackSettings( iCurrentSettings.iTrackNumber, 
                                          iCurrentSettings.iFileNameNumber,
                                          iCurrentSettings.iTrackState );
    
    AppUi()->ActivateLocalViewL( TUid::Uid( ESGEAudioTesterContainerViewId ) );
	return ETrue;
	}
				
/** 
 * Handle the middleSoftKeyPressed event.
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CSGEAudioTesterSettingItemListFile1View::HandleControlPaneMiddleSoftKeyPressedL( TInt aCommand )
	{
    if ( iSGEAudioTesterSettingItemListFile1 )
        {
        iSGEAudioTesterSettingItemListFile1->ChangeSelectedItemL();
        }
	return ETrue;
	}
				
/** 
 * Handle the rightSoftKeyPressed event.
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CSGEAudioTesterSettingItemListFile1View::HandleControlPaneRightSoftKeyPressedL( TInt aCommand )
	{
    AppUi()->ActivateLocalViewL( TUid::Uid( ESGEAudioTesterContainerViewId ) );
	return ETrue;
	}
				
void CSGEAudioTesterSettingItemListFile1View::SetSettingsL( TInt aTrackNumber, TInt aFileNameNumber, 
                                                            TSGEAudioTrackState aTrackState )
    {
    iCurrentSettings.iTrackNumber = aTrackNumber;
    iCurrentSettings.iFileNameNumber = aFileNameNumber;
    iCurrentSettings.iTrackState = aTrackState;    
    }

