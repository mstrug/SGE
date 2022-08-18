/*
========================================================================
 Name        : SGEAudioTesterContainerView.h
 Author      : Michal Strug
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : 
========================================================================
*/
#ifndef SGEAUDIOTESTERCONTAINERVIEW_H
#define SGEAUDIOTESTERCONTAINERVIEW_H

// [[[ begin generated region: do not modify [Generated Includes]
#include <aknview.h>
// ]]] end generated region [Generated Includes]

#include <SGEAudio/SGEAudioConfig.h>        
#include <SGEAudio/SGEAudioFile.h>        
#include <SGEAudio/SGEAudioProvider.h>        


// [[[ begin [Event Handler Includes]
// ]]] end [Event Handler Includes]

// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

// [[[ begin generated region: do not modify [Generated Forward Declarations]
class CAknNavigationDecorator;
class CSGEAudioTesterContainer;
// ]]] end generated region [Generated Forward Declarations]

class CSGEFile;

/**
 * Avkon view class for SGEAudioTesterContainerView. It is register with the view server
 * by the AppUi. It owns the container control.
 * @class    CSGEAudioTesterContainerView SGEAudioTesterContainerView.h
 */                        
            
class CSGEAudioTesterContainerView : public CAknView, MSGEAudioProviderObserver
    {
    
    
    // [[[ begin [Public Section]
public:
    // constructors and destructor
    CSGEAudioTesterContainerView();
    static CSGEAudioTesterContainerView* NewL();
    static CSGEAudioTesterContainerView* NewLC();        
    void ConstructL();
    virtual ~CSGEAudioTesterContainerView();
                        
    // from base class CAknView
    TUid Id() const;
    void HandleCommandL( TInt aCommand );
    
    // [[[ begin generated region: do not modify [Generated Methods]
	CSGEAudioTesterContainer* CreateContainerL();
	static void RunNoteAboutL( const TDesC* aOverrideText = NULL );
    // ]]] end generated region [Generated Methods]
    
    // ]]] end [Public Section]
    
    void DynInitMenuPaneL(TInt aResourceId, CEikMenuPane *aMenuPane);
    
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
    TBool HandleSettingsMenuItemSelectedL( TInt aCommand );
    TBool HandleInitializeMenuItemSelectedL( TInt aCommand );
	TBool HandleControlPaneMiddleSoftKeyPressedL( TInt aCommand );
	TBool HandleFile_1MenuItemSelectedL( TInt aCommand );
	TBool HandleFile_2MenuItemSelectedL( TInt aCommand );
	TBool HandleAbout_MenuItemSelectedL( TInt aCommand );
    // ]]] end [User Handlers]
    
    // ]]] end [Protected Section]
    
    
    // [[[ begin [Private Section]
private:
    void SetupStatusPaneL();
    void CleanupStatusPane();
    void ChangeStatusPaneTextL( const TDesC& aText );

    void SetMSKTextStart();
    void SetMSKTextNone();
    
    // [[[ begin generated region: do not modify [Generated Instance Variables]
	// any current navi decorator
	CAknNavigationDecorator* iNaviDecorator_;
	CSGEAudioTesterContainer* iSGEAudioTesterContainer;
    // ]]] end generated region [Generated Instance Variables]
    
    // [[[ begin generated region: do not modify [Generated Methods]
    // ]]] end generated region [Generated Methods]
    
    // ]]] end [Private Section]
    
public: // audio functions
    
    void CreateAudioL();

    void DestroyAudioL();

    TSGEAudioConfig& AudioConfig();

    void PressedNumberL( TInt aNumber );
    
    void SetTrackSettings( TInt aTrackNumber, TInt aFileNameNumber, TSGEAudioTrackState aTrackState );

    void UpdateTrackInfos( TInt aNumber );
    
    TBool IsAudioCreated();
    
private: // from MSGEAudioProviderObserver
    
    void AudioFileLoaded( TInt aSoundSampleId, TInt aError );
    
    void AudioAllFilesLoaded();

    void AudioSoundTrackPlayStarted( TUint aTrackId );

    void AudioSoundTrackPlayFinished( TUint aTrackId );

    void AudioStreamCreated( TInt aError );

    void AudioStreamError( TInt aError );
    
private: // data

    TSGEAudioConfig iConfig;
    
    CSGEAudioProvider *iAudioProvider;

    TBool iMSKStartActive;
    
    TBuf<60> iCurrentStatusText;
    
    struct STrackSettings
        {
        TInt iFileNameNumber;
        TSGEAudioTrackState iTrackState;
        };
    
    TFixedArray< STrackSettings, 9 > iTrackSettings;
    
    TFileName iPath;
    
    RArray< TBuf<30> > iTrackInfos;
    RBuf iTrackInfo;
    
    CSGEFile *iDataFile;
    
    };

#endif // SGEAUDIOTESTERCONTAINERVIEW_H
