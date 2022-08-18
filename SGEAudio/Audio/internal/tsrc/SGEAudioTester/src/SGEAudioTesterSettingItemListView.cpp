/*
========================================================================
 Name        : SGEAudioTesterSettingItemListView.cpp
 Author      : Michal Strug
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
#include <eikbtgpc.h>
#include <SGEAudioTester.rsg>
// ]]] end generated region [Generated System Includes]

// [[[ begin generated region: do not modify [Generated User Includes]

#include "SGEAudioTester.hrh"
#include "SGEAudioTesterSettingItemListView.h"
#include "SGEAudioTesterContainer.hrh"
#include "SGEAudioTesterSettingItemList.hrh"
#include "SGEAudioTesterSettingItemListFile1.hrh"
#include "SGEAudioTesterSettingItemList.h"
// ]]] end generated region [Generated User Includes]

#include "SGEAudioTesterContainerView.h"
#include <SGEAudio/SGEAudioConfig.h>

// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

/**
 * First phase of Symbian two-phase construction. Should not contain any
 * code that could leave.
 */
CSGEAudioTesterSettingItemListView::CSGEAudioTesterSettingItemListView()
    {
    // [[[ begin generated region: do not modify [Generated Contents]
    // ]]] end generated region [Generated Contents]
    
    }

/** 
 * The view's destructor removes the container from the control
 * stack and destroys it.
 */
CSGEAudioTesterSettingItemListView::~CSGEAudioTesterSettingItemListView()
    {
    // [[[ begin generated region: do not modify [Generated Contents]
    // ]]] end generated region [Generated Contents]
    
    if ( iSGEAudioTesterSettingItemList )
        {
        AppUi()->RemoveFromStack( iSGEAudioTesterSettingItemList );
        }
    delete iSGEAudioTesterSettingItemList;
    delete iSettings;
    }

/**
 * Symbian two-phase constructor.
 * This creates an instance then calls the second-phase constructor
 * without leaving the instance on the cleanup stack.
 * @return new instance of CSGEAudioTesterSettingItemListView
 */
CSGEAudioTesterSettingItemListView* CSGEAudioTesterSettingItemListView::NewL()
    {
    CSGEAudioTesterSettingItemListView* self = CSGEAudioTesterSettingItemListView::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

/**
 * Symbian two-phase constructor.
 * This creates an instance, pushes it on the cleanup stack,
 * then calls the second-phase constructor.
 * @return new instance of CSGEAudioTesterSettingItemListView
 */
CSGEAudioTesterSettingItemListView* CSGEAudioTesterSettingItemListView::NewLC()
    {
    CSGEAudioTesterSettingItemListView* self = new ( ELeave ) CSGEAudioTesterSettingItemListView();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


/**
 * Second-phase constructor for view.  
 * Initialize contents from resource.
 */ 
void CSGEAudioTesterSettingItemListView::ConstructL()
    {
    // [[[ begin generated region: do not modify [Generated Code]
	BaseConstructL( R_SGEAUDIO_TESTER_SETTING_ITEM_LIST_SGEAUDIO_TESTER_SETTING_ITEM_LIST_VIEW );
				
    // ]]] end generated region [Generated Code]
    
    // add your own initialization code here
    }

/**
 * @return The UID for this view
 */
TUid CSGEAudioTesterSettingItemListView::Id() const
    {
    return TUid::Uid( ESGEAudioTesterSettingItemListViewId );
    }

/**
 * Handle a command for this view (override)
 * @param aCommand command id to be handled
 */
void CSGEAudioTesterSettingItemListView::HandleCommandL( TInt aCommand )
    {
    // [[[ begin generated region: do not modify [Generated Code]
	TBool commandHandled = EFalse;
	switch ( aCommand )
		{	// code to dispatch to the AknView's menu and CBA commands is generated here
	
		case EAknSoftkeyDetails:
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
	
		}
    // ]]] end generated region [Generated Code]
    
    }

/**
 *    Handles user actions during activation of the view, 
 *    such as initializing the content.
 */
void CSGEAudioTesterSettingItemListView::DoActivateL( 
        const TVwsViewId& /*aPrevViewId*/,
        TUid /*aCustomMessageId*/,
        const TDesC8& /*aCustomMessage*/ )
    {
    // [[[ begin generated region: do not modify [Generated Contents]
	SetupStatusPaneL();
	
				
				
	
	if ( iSGEAudioTesterSettingItemList == NULL )
		{
		iSettings = TSGEAudioTesterSettingItemListSettings::NewL();
		iSGEAudioTesterSettingItemList = new ( ELeave ) CSGEAudioTesterSettingItemList( *iSettings, this );
		iSGEAudioTesterSettingItemList->SetMopParent( this );
		iSGEAudioTesterSettingItemList->ConstructFromResourceL( R_SGEAUDIO_TESTER_SETTING_ITEM_LIST_SGEAUDIO_TESTER_SETTING_ITEM_LIST );
		iSGEAudioTesterSettingItemList->ActivateL();
		iSGEAudioTesterSettingItemList->LoadSettingValuesL();
		iSGEAudioTesterSettingItemList->LoadSettingsL();
		AppUi()->AddToStackL( *this, iSGEAudioTesterSettingItemList );
		} 
    // ]]] end generated region [Generated Contents]
    
    LoadSettingsL();
    }

void CSGEAudioTesterSettingItemListView::LoadSettingsL()
    {
    // set settings
    TSGEAudioConfig& config = static_cast< CSGEAudioTesterContainerView* >( 
                AppUi()->View( TUid::Uid( ESGEAudioTesterContainerViewId ) ) )->AudioConfig();

    iSettings->SetIntegerEditorBufferLength( config.iBufferSize );
    iSettings->SetEnumeratedTextPopupBufferDataType( config.iBufferDataType );
    if ( config.iChannels == ESGEAudioChannelsMono )
        {
        iSettings->SetBinarySettingChannels( EFalse );
        }
    else
        {
        iSettings->SetBinarySettingChannels( ETrue );
        }
    iSettings->SetVolumeSetting( config.iMaxVolumePercent / 10 );
    iSettings->SetBinarySettingMultithreeded( config.iMultithreeded );
    iSettings->SetBinarySettingResampleFiles( config.iResampleFiles );
    if ( config.iSampleRate == 88200 )
        {
        iSettings->SetEnumeratedTextPopupSamplerate( 64010 );
        }
    else if ( config.iSampleRate == 96000 )
        {
        iSettings->SetEnumeratedTextPopupSamplerate( 64020 );
        }
    else
        {
        iSettings->SetEnumeratedTextPopupSamplerate( config.iSampleRate );
        }
    if ( config.iSubsystem == ESGEAudioSubsystemSimple )
        {
        iSettings->SetEnumeratedTextPopupSubsystem( 0 );
        }
    else if ( config.iSubsystem == ESGEAudioSubsystemMMF )
        {
        iSettings->SetEnumeratedTextPopupSubsystem( 2 );
        }
    else
        {
        iSettings->SetEnumeratedTextPopupSubsystem( 1 );
        }
    iSGEAudioTesterSettingItemList->LoadSettingsL();
    
    }

/**
 */
void CSGEAudioTesterSettingItemListView::DoDeactivate()
    {
    // [[[ begin generated region: do not modify [Generated Contents]
	CleanupStatusPane();
	
	if ( iSGEAudioTesterSettingItemList != NULL )
		{
		AppUi()->RemoveFromStack( iSGEAudioTesterSettingItemList );
		delete iSGEAudioTesterSettingItemList;
		iSGEAudioTesterSettingItemList = NULL;
		delete iSettings;
		iSettings = NULL;
		}
    // ]]] end generated region [Generated Contents]
    
    }

/** 
 * Handle status pane size change for this view (override)
 */
void CSGEAudioTesterSettingItemListView::HandleStatusPaneSizeChange()
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
void CSGEAudioTesterSettingItemListView::SetupStatusPaneL()
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
		iEikonEnv->CreateResourceReaderLC( reader, R_SGEAUDIO_TESTER_SETTING_ITEM_LIST_TITLE_RESOURCE );
		title->SetFromResourceL( reader );
		CleanupStack::PopAndDestroy(); // reader internal state
		}
				
	}

// ]]] end generated function

// [[[ begin generated function: do not modify
void CSGEAudioTesterSettingItemListView::CleanupStatusPane()
	{
	}

// ]]] end generated function

/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CSGEAudioTesterSettingItemListView::HandleChangeSelectedSettingItemL( TInt aCommand )
    {
    iSGEAudioTesterSettingItemList->ChangeSelectedItemL();
    return ETrue;
    }
                

/** 
 * Handle the leftSoftKeyPressed event.
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CSGEAudioTesterSettingItemListView::HandleControlPaneLeftSoftKeyPressedL( TInt aCommand )
    {
    // pressed OK

    TSGEAudioConfig& config = static_cast< CSGEAudioTesterContainerView* >( 
                AppUi()->View( TUid::Uid( ESGEAudioTesterContainerViewId ) ) )->AudioConfig();

    config.iBufferSize = iSettings->IntegerEditorBufferLength();
    config.iBufferDataType = (TSGEAudioBufferDataType) iSettings->EnumeratedTextPopupBufferDataType();
    if ( !iSettings->BinarySettingChannels() )
        {
        config.iChannels = ESGEAudioChannelsMono;
        }
    else
        {
        config.iChannels = ESGEAudioChannelsStereo;
        }
    config.iMaxVolumePercent = iSettings->VolumeSetting() * 10;
    config.iMultithreeded = iSettings->BinarySettingMultithreeded();
    config.iResampleFiles = iSettings->BinarySettingResampleFiles();
    if ( iSettings->EnumeratedTextPopupSamplerate() == 64010 )
        {
        config.iSampleRate = ESGEAudioSampleRate88200Hz;
        }
    else if ( iSettings->EnumeratedTextPopupSamplerate() == 64020 )
        {
        config.iSampleRate = ESGEAudioSampleRate96000Hz;
        }
    else
        {
        config.iSampleRate = (TSGEAudioSampleRate) iSettings->EnumeratedTextPopupSamplerate();
        }
    if ( iSettings->EnumeratedTextPopupSubsystem() == 0 )
        {
        config.iSubsystem = ESGEAudioSubsystemSimple;
        }
    else if ( iSettings->EnumeratedTextPopupSubsystem() == 2 )
        {
        config.iSubsystem = ESGEAudioSubsystemMMF;
        }
    else
        {
        config.iSubsystem = ESGEAudioSubsystemMDA;
        }
    
    static_cast< CSGEAudioTesterContainerView* >( 
                    AppUi()->View( TUid::Uid( ESGEAudioTesterContainerViewId ) ) )->DestroyAudioL();
    AppUi()->ActivateLocalViewL( TUid::Uid( ESGEAudioTesterContainerViewId ) );
    return ETrue;
    }
                
/** 
 * Handle the middleSoftKeyPressed event.
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CSGEAudioTesterSettingItemListView::HandleControlPaneMiddleSoftKeyPressedL( TInt aCommand )
    {
    // pressed Change
    iSGEAudioTesterSettingItemList->ChangeSelectedItemL();
    return ETrue;
    }
                
/** 
 * Handle the rightSoftKeyPressed event.
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CSGEAudioTesterSettingItemListView::HandleControlPaneRightSoftKeyPressedL( TInt aCommand )
    {
    // pressed cancel
    AppUi()->ActivateLocalViewL( TUid::Uid( ESGEAudioTesterContainerViewId ) );
    return ETrue;
    }
                
