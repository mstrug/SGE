/*
========================================================================
 Name        : SGEAudioTesterContainerView.cpp
 Author      : Michal Strug
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : 
========================================================================
*/
// [[[ begin generated region: do not modify [Generated System Includes]
#include <aknviewappui.h>
#include <eikmenub.h>
#include <avkon.hrh>
#include <barsread.h>
#include <stringloader.h>
#include <eiklabel.h>
#include <eikenv.h>
#include <akncontext.h>
#include <akntitle.h>
#include <aknnavide.h>
#include <aknnavi.h>
#include <eikbtgpc.h>
#include <aknnotewrappers.h>
#include <SGEAudioTester.rsg>
// ]]] end generated region [Generated System Includes]

// [[[ begin generated region: do not modify [Generated User Includes]

#include "SGEAudioTester.hrh"
#include "SGEAudioTesterContainerView.h"
#include "SGEAudioTesterContainer.hrh"
#include "SGEAudioTesterSettingItemList.hrh"
#include "SGEAudioTesterSettingItemListFile1.hrh"
#include "SGEAudioTesterContainer.h"
// ]]] end generated region [Generated User Includes]

#include "SGEAudioTesterSettingItemListFile1View.h"
#include <COEUTILS.H>
#include <SGEFile/SGEFile.h>

// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

const TInt KAudioFilesCount = 9;

/**
 * First phase of Symbian two-phase construction. Should not contain any
 * code that could leave.
 */
CSGEAudioTesterContainerView::CSGEAudioTesterContainerView()
    : iMSKStartActive( ETrue )
    {
    // [[[ begin generated region: do not modify [Generated Contents]
	iNaviDecorator_ = NULL;
	iSGEAudioTesterContainer = NULL;
    // ]]] end generated region [Generated Contents]
    
    iConfig.iSampleRate = ESGEAudioSampleRate16000Hz;

    for ( TInt i = 0; i < iTrackSettings.Count(); i++ )
        {
        iTrackSettings[ i ].iFileNameNumber = i;
        iTrackSettings[ i ].iTrackState.iSampleId = i + 1;
        iTrackSettings[ i ].iTrackState.iMuted = EFalse;
        iTrackSettings[ i ].iTrackState.iPaused = EFalse;
        iTrackSettings[ i ].iTrackState.iRepeat = 0;
        iTrackSettings[ i ].iTrackState.iVolume = 0;
        }
    
    iCurrentStatusText.Copy( _L("Status: Stopped") );
    }

/** 
 * The view's destructor removes the container from the control
 * stack and destroys it.
 */
CSGEAudioTesterContainerView::~CSGEAudioTesterContainerView()
    {
    delete iAudioProvider;
    iTrackInfos.Close();
    iTrackInfo.Close();
    delete iDataFile;

    // [[[ begin generated region: do not modify [Generated Contents]
	if ( iNaviDecorator_ != NULL )
		{
		delete iNaviDecorator_;
		iNaviDecorator_ = NULL;
		}
	delete iSGEAudioTesterContainer;
	iSGEAudioTesterContainer = NULL;
    // ]]] end generated region [Generated Contents]
    }

/**
 * Symbian two-phase constructor.
 * This creates an instance then calls the second-phase constructor
 * without leaving the instance on the cleanup stack.
 * @return new instance of CSGEAudioTesterContainerView
 */
CSGEAudioTesterContainerView* CSGEAudioTesterContainerView::NewL()
    {
    CSGEAudioTesterContainerView* self = CSGEAudioTesterContainerView::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

/**
 * Symbian two-phase constructor.
 * This creates an instance, pushes it on the cleanup stack,
 * then calls the second-phase constructor.
 * @return new instance of CSGEAudioTesterContainerView
 */
CSGEAudioTesterContainerView* CSGEAudioTesterContainerView::NewLC()
    {
    CSGEAudioTesterContainerView* self = new ( ELeave ) CSGEAudioTesterContainerView();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


/**
 * Second-phase constructor for view.  
 * Initialize contents from resource.
 */ 
void CSGEAudioTesterContainerView::ConstructL()
    {
    // [[[ begin generated region: do not modify [Generated Code]
	BaseConstructL( R_SGEAUDIO_TESTER_CONTAINER_SGEAUDIO_TESTER_CONTAINER_VIEW );
				
    // ]]] end generated region [Generated Code]
    
    // add your own initialization code here

	for ( TInt i = 0; i < 64; i++ )
	    {
        iTrackInfos.AppendL( _L("") );
	    }
	iTrackInfo.Create( 64 * 32 );

	iEikonEnv->FsSession().PrivatePath( iPath );
    iPath.Append( _L("data.sge") );
    iPath.Insert( 0, _L("c:") );
    if ( !ConeUtils::FileExists( iPath ) )
        {
        iPath[ 0 ] = 'd';
        if ( !ConeUtils::FileExists( iPath ) )
            {
            iPath[ 0 ] = 'e';
            if ( !ConeUtils::FileExists( iPath ) )
                {
                iPath[ 0 ] = 'f';
                if ( !ConeUtils::FileExists( iPath ) )
                    {
                    iPath[ 0 ] = 'g';
                    if ( !ConeUtils::FileExists( iPath ) )
                        {
                        User::Leave( KErrNotFound );
                        }
                    }
                }
            }
        }
	        
//    iDataFile = CSGEFile::NewL();
//    iDataFile->LoadL( iPath );
    }

/**
 * @return The UID for this view
 */
TUid CSGEAudioTesterContainerView::Id() const
    {
    return TUid::Uid( ESGEAudioTesterContainerViewId );
    }

/**
 * Handle a command for this view (override)
 * @param aCommand command id to be handled
 */
void CSGEAudioTesterContainerView::HandleCommandL( TInt aCommand )
    {
    if ( aCommand >= ESGEAudioTesterContainerViewNumber1 && aCommand <= ESGEAudioTesterContainerViewNumber9 )
        {
        PressedNumberL( aCommand - ESGEAudioTesterContainerViewNumber1 + 1 );
        return;
        }
    else if ( aCommand == ESGEAudioTesterContainerViewHash )
        {
        HandleSettingsMenuItemSelectedL( aCommand );
        return;
        }
    
    // [[[ begin generated region: do not modify [Generated Code]
	TBool commandHandled = EFalse;
	switch ( aCommand )
		{	// code to dispatch to the AknView's menu and CBA commands is generated here
	
		case ESGEAudioTesterContainerViewControlPaneMiddleId2:
			commandHandled = HandleControlPaneMiddleSoftKeyPressedL( aCommand );
			break;
		case ESGEAudioTesterContainerViewAbout_MenuItemCommand:
			commandHandled = HandleAbout_MenuItemSelectedL( aCommand );
			break;
		case ESGEAudioTesterContainerViewSettingsMenuItemCommand:
			commandHandled = HandleSettingsMenuItemSelectedL( aCommand );
			break;
		case ESGEAudioTesterContainerViewFile_1MenuItemCommand:
			commandHandled = HandleFile_1MenuItemSelectedL( aCommand );
			break;
		case ESGEAudioTesterContainerViewFile_2MenuItemCommand:
			commandHandled = HandleFile_1MenuItemSelectedL( aCommand );
			break;
		case ESGEAudioTesterContainerViewFile_3MenuItemCommand:
			commandHandled = HandleFile_1MenuItemSelectedL( aCommand );
			break;
		case ESGEAudioTesterContainerViewFile_4MenuItemCommand:
			commandHandled = HandleFile_1MenuItemSelectedL( aCommand );
			break;
		case ESGEAudioTesterContainerViewFile_5MenuItemCommand:
			commandHandled = HandleFile_1MenuItemSelectedL( aCommand );
			break;
		case ESGEAudioTesterContainerViewFile_6MenuItemCommand:
			commandHandled = HandleFile_1MenuItemSelectedL( aCommand );
			break;
		case ESGEAudioTesterContainerViewFile_7MenuItemCommand:
			commandHandled = HandleFile_1MenuItemSelectedL( aCommand );
			break;
		case ESGEAudioTesterContainerViewFile_8MenuItemCommand:
			commandHandled = HandleFile_1MenuItemSelectedL( aCommand );
			break;
		case ESGEAudioTesterContainerViewFile_9MenuItemCommand:
			commandHandled = HandleFile_1MenuItemSelectedL( aCommand );
			break;
		case ESGEAudioTesterContainerViewInitializeMenuItemCommand:
			commandHandled = HandleInitializeMenuItemSelectedL( aCommand );
			break;
		default:
			break;
		}
	
		
	if ( !commandHandled ) 
		{
	
		if ( aCommand == EAknSoftkeyExit )
			{
			AppUi()->HandleCommandL( EEikCmdExit );
			}
	
		}
    // ]]] end generated region [Generated Code]
    
    }

/**
 *    Handles user actions during activation of the view, 
 *    such as initializing the content.
 */
void CSGEAudioTesterContainerView::DoActivateL( 
        const TVwsViewId& /*aPrevViewId*/,
        TUid /*aCustomMessageId*/,
        const TDesC8& /*aCustomMessage*/ )
    {
    // [[[ begin generated region: do not modify [Generated Contents]
	SetupStatusPaneL();
	
				
				
	
	if ( iSGEAudioTesterContainer == NULL )
		{
		iSGEAudioTesterContainer = CreateContainerL();
		iSGEAudioTesterContainer->SetMopParent( this );
		AppUi()->AddToStackL( *this, iSGEAudioTesterContainer );
		} 
    // ]]] end generated region [Generated Contents]

	if ( iMSKStartActive )
	    {
        CEikButtonGroupContainer* cba = CEikButtonGroupContainer::Current();
        TRAP_IGNORE( cba->SetCommandL( ESGEAudioTesterContainerViewControlPaneMiddleId2, _L("Start") ) );
        cba->DrawNow();
	    }
	else
	    {
        CEikButtonGroupContainer* cba = CEikButtonGroupContainer::Current();
        TRAP_IGNORE( cba->SetCommandL( ESGEAudioTesterContainerViewControlPaneMiddleId2, _L("Close") ) );
        cba->DrawNow();
	    }
    ChangeStatusPaneTextL( iCurrentStatusText );
    if ( iAudioProvider )
        {
        UpdateTrackInfos( iAudioProvider->TrackCount() );
        }
    }

/**
 */
void CSGEAudioTesterContainerView::DoDeactivate()
    {
    // [[[ begin generated region: do not modify [Generated Contents]
	CleanupStatusPane();
	
	if ( iSGEAudioTesterContainer != NULL )
		{
		AppUi()->RemoveFromViewStack( *this, iSGEAudioTesterContainer );
		delete iSGEAudioTesterContainer;
		iSGEAudioTesterContainer = NULL;
		}
    // ]]] end generated region [Generated Contents]
    
    }

/** 
 * Handle status pane size change for this view (override)
 */
void CSGEAudioTesterContainerView::HandleStatusPaneSizeChange()
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
    
    TRAP_IGNORE( ChangeStatusPaneTextL( iCurrentStatusText ) );
    }

// [[[ begin generated function: do not modify
void CSGEAudioTesterContainerView::SetupStatusPaneL()
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
		iEikonEnv->CreateResourceReaderLC( reader, R_SGEAUDIO_TESTER_CONTAINER_TITLE_RESOURCE );
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
				
		HBufC* labelText = StringLoader::LoadLC( R_SGEAUDIO_TESTER_CONTAINER_NAVI_TEXT );
		iNaviDecorator_ = naviPane->CreateNavigationLabelL( *labelText );
		CleanupStack::PopAndDestroy( labelText );			
				
		naviPane->PushL( *iNaviDecorator_ );
		}
				
	}

// ]]] end generated function

void CSGEAudioTesterContainerView::ChangeStatusPaneTextL( const TDesC& aText )
    {
    iCurrentStatusText.Copy( aText );
    
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

void CSGEAudioTesterContainerView::SetMSKTextStart()
    {
    CEikButtonGroupContainer* cba = CEikButtonGroupContainer::Current();
    TRAP_IGNORE( cba->SetCommandL( ESGEAudioTesterContainerViewControlPaneMiddleId2, _L("Start") ) );
    cba->DrawNow();
    iMSKStartActive = ETrue;
    }

void CSGEAudioTesterContainerView::SetMSKTextNone()
    {
    CEikButtonGroupContainer* cba = CEikButtonGroupContainer::Current();
    TRAP_IGNORE( cba->SetCommandL( ESGEAudioTesterContainerViewControlPaneMiddleId2, _L("Close") ) );
    cba->DrawNow();
    iMSKStartActive = EFalse;
    }

// [[[ begin generated function: do not modify
void CSGEAudioTesterContainerView::CleanupStatusPane()
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

/**
 *    Creates the top-level container for the view.  You may modify this method's
 *    contents and the CSGEAudioTesterContainer::NewL() signature as needed to initialize the
 *    container, but the signature for this method is fixed.
 *    @return new initialized instance of CSGEAudioTesterContainer
 */
CSGEAudioTesterContainer* CSGEAudioTesterContainerView::CreateContainerL()
    {
    return CSGEAudioTesterContainer::NewL( ClientRect(), NULL, this );
    }

void CSGEAudioTesterContainerView::DynInitMenuPaneL(TInt aResourceId, CEikMenuPane *aMenuPane )
    {
    if ( aResourceId != R_SGEAUDIO_TESTER_CONTAINER_MENU_PANE1_MENU_PANE )
        {
        return;
        }
    
    if ( iAudioProvider )
        {
        aMenuPane->ItemData( ESGEAudioTesterContainerViewInitializeMenuItemCommand ).
                                                                        iText.Copy( _L("Close") );
        }
    else
        {
        aMenuPane->ItemData( ESGEAudioTesterContainerViewInitializeMenuItemCommand ).
                                                                        iText.Copy( _L("Start") );
        }
    }

/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CSGEAudioTesterContainerView::HandleSettingsMenuItemSelectedL( TInt aCommand )
    {
    // TODO: implement selected event handler
    AppUi()->ActivateLocalViewL( TUid::Uid( ESGEAudioTesterSettingItemListViewId ) );
    //CEikonEnv::Static()->AlertWin( _L("HandleSettingsMenuItemSelectedL") );
    return ETrue;
    }

/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CSGEAudioTesterContainerView::HandleFile_1MenuItemSelectedL( TInt aCommand )
    {
    CSGEAudioTesterSettingItemListFile1View *view = 
                    static_cast< CSGEAudioTesterSettingItemListFile1View* >( 
                    AppUi()->View( TUid::Uid( ESGEAudioTesterSettingItemListFile1ViewId ) ) );
    TInt idx = aCommand - ESGEAudioTesterContainerViewFile_1MenuItemCommand;
    view->SetSettingsL( idx, iTrackSettings[ idx ].iFileNameNumber, iTrackSettings[ idx ].iTrackState );
    AppUi()->ActivateLocalViewL( TUid::Uid( ESGEAudioTesterSettingItemListFile1ViewId ) );
   
    return ETrue;
    }
                
/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CSGEAudioTesterContainerView::HandleFile_2MenuItemSelectedL( TInt aCommand )
    {
    return EFalse;
    }

/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CSGEAudioTesterContainerView::HandleInitializeMenuItemSelectedL( TInt aCommand )
    {
    // TODO: implement selected event handler
//    TBuf<20> aa;
//    aa.AppendNum( aCommand, EHex );
//    CEikonEnv::Static()->AlertWin( aa );

    if ( iAudioProvider )
        {
        DestroyAudioL();
        }
    else
        {
        CreateAudioL();
        }
    
    return ETrue;
    }
                
/** 
 * Handle the middleSoftKeyPressed event.
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CSGEAudioTesterContainerView::HandleControlPaneMiddleSoftKeyPressedL( TInt aCommand )
    {
    // MSK Start pressed
    if ( iMSKStartActive )
        {
        CreateAudioL();
        }
    else
        {
        DestroyAudioL();
        }
    return EFalse;
    }


// [[[ begin generated function: do not modify
/**
 * Show the popup note for noteAbout
 * @param aOverrideText optional override text
 */
void CSGEAudioTesterContainerView::RunNoteAboutL( const TDesC* aOverrideText )
	{
	CAknInformationNote* note = new ( ELeave ) CAknInformationNote();
	if ( aOverrideText == NULL )
		{
		HBufC* noteText = StringLoader::LoadLC( R_SGEAUDIO_TESTER_CONTAINER_NOTE_ABOUT );
		note->ExecuteLD( *noteText );
		CleanupStack::PopAndDestroy( noteText );
		}
	else
		{
		note->ExecuteLD( *aOverrideText );
		}
	}
// ]]] end generated function

/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CSGEAudioTesterContainerView::HandleAbout_MenuItemSelectedL( TInt aCommand )
    {
    RunNoteAboutL();
    return ETrue;
    }
                

void CSGEAudioTesterContainerView::CreateAudioL()
    {
    ChangeStatusPaneTextL( _L("Status: Stopped") );

    SetMSKTextNone();

    delete iAudioProvider;
    iAudioProvider = NULL;
    iAudioProvider = CSGEAudioProvider::NewL( this, iConfig );
    
    ChangeStatusPaneTextL( _L("Audio Provider created") );
    
    TBuf<50> file;
    
    RArray< TSGEAudioFile > files;
    CleanupClosePushL( files );
    
    for ( TInt i = 1; i <= KAudioFilesCount; i++ )
        {
        file.Copy( _L("audiotest") );
        file.AppendNum( i );
        file.Append( _L(".wav") );
    
        files.AppendL( TSGEAudioFile( file, i, ESGEAudioGroupAny, ESGEAudioFileTypeWAV ) );
        }
    
    TRAPD( err, iAudioProvider->LoadFilesL( iPath, files ) );
    if ( err )
        {
        TBuf<40> buf;
        buf.Copy( _L("Files error: ") );
        buf.AppendNum( err );
        ChangeStatusPaneTextL( buf );
        User::Leave( err );
        }
    
    CleanupStack::PopAndDestroy( &files );
    
    ChangeStatusPaneTextL( _L("Loading files") );
    }

void CSGEAudioTesterContainerView::DestroyAudioL()
    {
    ChangeStatusPaneTextL( _L("Status: Stopped") );

    delete iAudioProvider;
    iAudioProvider = NULL;

    SetMSKTextStart();
    
    for ( TInt i = 0; i < iTrackInfos.Count(); i++ )
        {
        iTrackInfos[ i ].Zero();
        }
    
    if ( iSGEAudioTesterContainer )
        {
        iSGEAudioTesterContainer->SetLabelTextL( 0, KNullDesC() );
        }
    }

TSGEAudioConfig& CSGEAudioTesterContainerView::AudioConfig()
    {
    return iConfig;
    }

void CSGEAudioTesterContainerView::PressedNumberL( TInt aNumber )
    {
    if ( aNumber >= 1 && aNumber <= 9 )
        {
        if ( !iAudioProvider )
            {
            TInt cmd = ESGEAudioTesterContainerViewFile_1MenuItemCommand;
            HandleFile_1MenuItemSelectedL( aNumber + cmd - 1 );
            return;
            }
        
        TInt trackId = iAudioProvider->Play( iTrackSettings[ aNumber - 1 ].iTrackState.iSampleId, 
                                             iTrackSettings[ aNumber - 1 ].iTrackState.iRepeat );
        iTrackSettings[ aNumber - 1 ].iTrackState.iTrackId = trackId;
        iAudioProvider->SetVolume( trackId, iTrackSettings[ aNumber - 1 ].iTrackState.iVolume );
//        if ( iTrackSettings[ aNumber - 1 ].iTrackState.iPaused )
//            {
//            iAudioProvider->Pause( trackId );
//            }
//        else
//            {
//            iAudioProvider->Resume( trackId );
//            }
//        if ( iTrackSettings[ aNumber - 1 ].iTrackState.iMuted )
//            {
//            iAudioProvider->Mute( trackId );
//            }
//        else
//            {
//            iAudioProvider->Unmute( trackId );
//            }
//        if ( iTrackSettings[ aNumber - 1 ].iTrackState.iRepeat == ESGEAudioRepeatInfinite || 
//             iTrackSettings[ aNumber - 1 ].iTrackState.iRepeat > 10 )
//            {
//            iAudioProvider->Stop( trackId );
//            }
        }
    }

void CSGEAudioTesterContainerView::SetTrackSettings( TInt aTrackNumber, TInt aFileNameNumber, 
                                                     TSGEAudioTrackState aTrackState )
    {
    iTrackSettings[ aTrackNumber ].iFileNameNumber = aFileNameNumber;
    iTrackSettings[ aTrackNumber ].iTrackState = aTrackState;
    iTrackSettings[ aTrackNumber ].iTrackState.iSampleId = aFileNameNumber + 1;
    }

void CSGEAudioTesterContainerView::UpdateTrackInfos( TInt aNumber )
    {
    if ( iSGEAudioTesterContainer )
        {
        iTrackInfo.Zero();
        for ( TInt i = 0; i < iTrackInfos.Count(); i++ )
            {
            iTrackInfo.Append( iTrackInfos[ i ] );
            iTrackInfo.Append( _L("\n") );
            }
        TRAP_IGNORE( iSGEAudioTesterContainer->SetLabelTextL( aNumber, iTrackInfo ) );
        }
    }

TBool CSGEAudioTesterContainerView::IsAudioCreated()
    {
    return (TBool)iAudioProvider;
    }

void CSGEAudioTesterContainerView::AudioFileLoaded( TInt aSoundSampleId, TInt aError )
    {
    if ( aError != KErrNone )
        {
        TBuf<30> buf;
        buf.Copy( _L("Loading files error: ") );
        buf.AppendNum( aError );
        
        TRAP_IGNORE( ChangeStatusPaneTextL( buf ) );

//        CEikonEnv::Static()->AlertWin( buf );
        
        SetMSKTextStart();
        }
    else
        {
        TBuf<30> buf;
        buf.Copy( _L("File loaded: ") );
        buf.AppendNum( aSoundSampleId );
        buf.Append( _L(" / ") );
        buf.AppendNum( KAudioFilesCount );
        TRAP_IGNORE( iSGEAudioTesterContainer->SetLabelTextL( 0, buf ) );
        }
    }

void CSGEAudioTesterContainerView::AudioAllFilesLoaded()
    {
    TRAP_IGNORE( ChangeStatusPaneTextL( _L("All files loaded") ) );
    
    TRAPD( err, iAudioProvider->CreateAudioStreamL() );
    TBuf<30> buf;
    if ( err )
        {
        buf.Copy( _L("Stream create error: ") );
        buf.AppendNum( err );
        SetMSKTextStart();
        }
    else
        {
        buf.Copy( _L("Creating Audio Stream") );
        }
    
    TRAP_IGNORE( ChangeStatusPaneTextL( buf ) );
    }

void CSGEAudioTesterContainerView::AudioSoundTrackPlayStarted( TUint aTrackId )
    {
    if ( aTrackId < (TUint) iTrackInfos.Count() )
        {
        TBuf<30> buf;
        buf.Copy( _L("Track ") );
        buf.AppendNum( aTrackId );
        buf.Append( _L(": Playing") );
        iTrackInfos[ aTrackId ].Copy( buf );
        
        UpdateTrackInfos( aTrackId );
        }
    }

void CSGEAudioTesterContainerView::AudioSoundTrackPlayFinished( TUint aTrackId )
    {
    if ( aTrackId < (TUint) iTrackInfos.Count() )
        {
        TBuf<30> buf;
        buf.Copy( _L("Track ") );
        buf.AppendNum( aTrackId );
        buf.Append( _L(": Stopped") );
        iTrackInfos[ aTrackId ].Copy( buf );
        
        UpdateTrackInfos( aTrackId );
        }
    }

void CSGEAudioTesterContainerView::AudioStreamCreated( TInt aError )
    {
    TBuf<30> buf;
    if ( aError != KErrNone )
        {
        buf.Copy( _L("Stream create error: ") );
        buf.AppendNum( aError );
        SetMSKTextStart();
        }
    else
        {
        if ( iConfig.iSubsystem == ESGEAudioSubsystemSimple )
            {
            buf.Copy( _L("Status: playing") );
            }
        else
            {
            buf.Copy( _L("Status: playing (") );
            TUint val = iConfig.CalculateSoundLatency();
            if ( val < 1000 )
                {
                buf.AppendNum( val );
                buf.Append( _L("us)") );
                }
            else
                {
                buf.AppendNum( val / 1000 );
                buf.Append( _L("ms)") );
                }
            }
        }

    TRAP_IGNORE( iSGEAudioTesterContainer->SetLabelTextL( 0, KNullDesC() ); 
                 ChangeStatusPaneTextL( buf ) );
    }

void CSGEAudioTesterContainerView::AudioStreamError( TInt aError )
    {
    if ( iConfig.iSubsystem == ESGEAudioSubsystemSimple && aError == KErrInUse )
        {
        TBuf<40> buf;
        buf.Copy( _L("Status: playing (only 1 player)") );
        TRAP_IGNORE( ChangeStatusPaneTextL( buf ) );
        UpdateTrackInfos( iAudioProvider->TrackCount() );
        }
    else if ( aError != KErrNone )
        {
        TBuf<40> buf;
        buf.Copy( _L("Stream error: ") );
        buf.AppendNum( aError );
        
        TRAP_IGNORE( ChangeStatusPaneTextL( buf ) );
        SetMSKTextStart();
        if ( iConfig.iSubsystem == ESGEAudioSubsystemSimple )
            {
            UpdateTrackInfos( iAudioProvider->TrackCount() );
            }
        }
    }
