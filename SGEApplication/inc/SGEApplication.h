/*
 ============================================================================
  Name        : SGEApplication.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#ifndef __SGEAPPLICATION_H__
#define __SGEAPPLICATION_H__

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <APPARC.H> 
#include <W32STD.H>
#include <COEDEF.H>
#include <AKNAPPUI.H>

// FORWARD DECLARATIONS
class CApaApplication;
class CSGESystemAppUi;
class CSGESystemAppView;
class CCoeControl;

// CLASS DECLARATION

/**
 *  TSGEApplicationOrientation
 * SGE Application supported orientations.
 */
enum TSGEApplicationOrientation
    {
    ESGEApplicationOrientationDefault,
    ESGEApplicationOrientationAuto,
    ESGEApplicationOrientationLandscape,
    ESGEApplicationOrientationPortrait
    };

/**
 *  CSGEApplication
 *
 */
class CSGEApplication : public CBase
    {
public: // construct and destruct functions

    IMPORT_C ~CSGEApplication();

    IMPORT_C static TInt Start( TApaApplicationFactory::TFunction );

public: // new functions

    /*
     * This function is intended to create all necessary objects.
     * It has same role as ConstructL() function.
     * It is called by framework.
     * Default implementation does nothing.
     */
    IMPORT_C virtual void InitL();

    /*
     * This function is intended to free all resources which has
     * to be released prior to call SGEApplication destructor.
     * It is called by framework.
     * Default implementation does nothing.
     */
    IMPORT_C virtual void Cleanup();

    IMPORT_C void Exit();

    IMPORT_C void DrawNow() const;

    IMPORT_C CWindowGc& SystemGc() const;

    IMPORT_C TRect Rect() const;

    IMPORT_C void AlertWin( const TDesC& aMsg1, const TDesC& aMsg2 );

    IMPORT_C RWindow& Window() const;

    IMPORT_C RWsSession& WsSession() const;

    IMPORT_C RFs& FsSession() const;

    IMPORT_C void SetOrientationL( CAknAppUiBase::TAppUiOrientation aOrientation );
    
    IMPORT_C void EnableDragEvents();

    IMPORT_C void SetTitleL( const TDesC& aTitle );
    
    /**
    * This function plays sounds by sound ID.
    * @param aSid Sound ID. These SID numbers are specified in Avkon enum TAvkonSystemSID.
    */
    IMPORT_C void PlaySystemSound( TInt aSid );

    IMPORT_C CCoeControl* MainCoeControl() const;

    IMPORT_C static void OpenBrowserL( const TDesC& aUrl );

public: // abstract and virtual functions intended to implement in derived class

    virtual TUid ApplicationUid() = 0;

    // return ETure if application should extent to whole screen
    IMPORT_C virtual TBool FullScreen() const;

    // Flags passed to CAknAppUi::BaseConstructL function.
    // Only flag CAknAppUi::EAknEnableSkin is passed always.
    // Other flags which may be used: EAppOrientationPortrait...
    IMPORT_C virtual TInt ApplicationFlags() const;

    // return ETure if application should turn off keys/touch event sounds, default is EFalse
    IMPORT_C virtual TBool SilentKeys() const;

    // return which orientation is supported, default ESGEApplicationOrientationDefault
    IMPORT_C virtual TSGEApplicationOrientation Orientation() const;

    // from CAknAppUi

    IMPORT_C virtual void HandleCommandL( TInt aCommand );

    IMPORT_C virtual void HandleStatusPaneSizeChange();

    // return ETrue when event was handled
    IMPORT_C virtual TBool HandleWsEventL( const TWsEvent& aEvent, CCoeControl *aDestination );

    IMPORT_C virtual void HandleForegroundEventL( TBool aForeground );

    IMPORT_C virtual TKeyResponse HandleKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );

    IMPORT_C virtual TBool ProcessCommandParametersL( TApaCommand aCommand, TFileName& aDocumentName, const TDesC8& aTail );

    // from CAknAppView

    IMPORT_C virtual void Draw( const TRect& aRect ) const;

    IMPORT_C virtual void SizeChanged();

    // return ETrue when event was handled
    IMPORT_C virtual TBool HandlePointerEventL( const TPointerEvent& aPointerEvent );

    IMPORT_C virtual void HandleResourceChange( TInt aType );
    
protected:

    IMPORT_C CSGEApplication();

    IMPORT_C static CApaApplication* NewApplication( CSGEApplication *app );

public: // internal functions

    IMPORT_C virtual CSGESystemAppUi* CreateAppUiL();

    IMPORT_C virtual CSGESystemAppView* CreateAppViewL( TRect aRect );

protected: // data

    CSGESystemAppUi *iAppUi;      // not owned

    CSGESystemAppView *iAppView;  // not owned

    };

#endif // __SGEAPPLICATION_H__
