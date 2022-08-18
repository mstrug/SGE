/*
 ============================================================================
  Name        : SGEApplication.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#include "SGEApplication.h"
#include "SGESystemApplication.h"
#include "SGESystemAppUi.h"
#include "SGESystemAppView.h"
#include <eikstart.h>
#include <aknsoundsystem.h> // CAknKeySoundSystem
#include <akntitle.h> // CAknTitlePane
#include <apgcli.h>

// -----------------------------------------------------------------------------
// CSGEApplication::CSGEApplication()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEApplication::CSGEApplication()
    {
    }

// -----------------------------------------------------------------------------
// CSGEApplication::~CSGEApplication()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEApplication::~CSGEApplication()
    {
    }

// -----------------------------------------------------------------------------
// CSGEApplication::CreateAppUiL()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGESystemAppUi* CSGEApplication::CreateAppUiL()
    {
    iAppUi = new (ELeave) CSGESystemAppUi( *this );
    return iAppUi;
    }

// -----------------------------------------------------------------------------
// CSGEApplication::CreateAppViewL()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGESystemAppView* CSGEApplication::CreateAppViewL( TRect aRect )
    {
    iAppView = CSGESystemAppView::NewL( aRect, *this );
    return iAppView;
    }

// -----------------------------------------------------------------------------
// CSGEApplication::NewApplication()
// -----------------------------------------------------------------------------
//
EXPORT_C CApaApplication* CSGEApplication::NewApplication( CSGEApplication *aApplication )
    {
    if ( !aApplication )
        {
        return NULL;
        }
    return new CSGESystemApplication( aApplication );
    }

// -----------------------------------------------------------------------------
// CSGEApplication::Start()
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CSGEApplication::Start( TApaApplicationFactory::TFunction aFcn )
    {
    return EikStart::RunApplication( aFcn );
    }

// -----------------------------------------------------------------------------
// CSGEApplication::InitL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEApplication::InitL()
    {
    }

// -----------------------------------------------------------------------------
// CSGEApplication::Cleanup()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEApplication::Cleanup()
    {
    }

// -----------------------------------------------------------------------------
// CSGEApplication::Exit()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEApplication::Exit()
    {
    iAppUi->Exit();
    }

// -----------------------------------------------------------------------------
// CSGEApplication::DrawNow()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEApplication::DrawNow() const
    {
    iAppView->DrawNow();
    }

// -----------------------------------------------------------------------------
// CSGEApplication::SystemGc()
// -----------------------------------------------------------------------------
//
EXPORT_C CWindowGc& CSGEApplication::SystemGc() const
    {
    return iAppView->SystemGc();
    }

// -----------------------------------------------------------------------------
// CSGEApplication::Rect()
// -----------------------------------------------------------------------------
//
EXPORT_C TRect CSGEApplication::Rect() const
    {
    return iAppView->Rect();
    }

// -----------------------------------------------------------------------------
// CSGEApplication::AlertWin()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEApplication::AlertWin( const TDesC& aMsg1, const TDesC& aMsg2 )
    {
    iAppUi->AlertWin( aMsg1, aMsg2 );
    }

// -----------------------------------------------------------------------------
// CSGEApplication::Window()
// -----------------------------------------------------------------------------
//
EXPORT_C RWindow& CSGEApplication::Window() const
    {
    return iAppView->WindowControl();
    }

// -----------------------------------------------------------------------------
// CSGEApplication::WsSession()
// -----------------------------------------------------------------------------
//
EXPORT_C RWsSession& CSGEApplication::WsSession() const
    {
    return iAppView->WindowSession();
    }

// -----------------------------------------------------------------------------
// CSGEApplication::FsSession()
// -----------------------------------------------------------------------------
//
EXPORT_C RFs& CSGEApplication::FsSession() const
    {
    return iAppView->Fs();
    }

// -----------------------------------------------------------------------------
// CSGEApplication::SetOrientationL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEApplication::SetOrientationL( CAknAppUiBase::TAppUiOrientation aOrientation )
    {
    iAppUi->SetOrientationL( aOrientation );
    }

// -----------------------------------------------------------------------------
// CSGEApplication::EnableDragEvents()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEApplication::EnableDragEvents()
	{
	iAppView->EnableDragEvents();
	}

// -----------------------------------------------------------------------------
// CSGEApplication::SetTitleL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEApplication::SetTitleL( const TDesC& aTitle )
	{
    CAknTitlePane* title = (CAknTitlePane*) iAppUi->StatusPane()->ControlL( TUid::Uid( EEikStatusPaneUidTitle ) );
    if ( title )
    	{
    	title->SetTextL( aTitle );
    	}
	}

// -----------------------------------------------------------------------------
// CSGEApplication::PlaySystemSound()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEApplication::PlaySystemSound( TInt aSid )
    {
    iAppUi->KeySounds()->PlaySound( aSid );
    }

// -----------------------------------------------------------------------------
// CSGEApplication::MainCoeControl()
// -----------------------------------------------------------------------------
//
EXPORT_C CCoeControl* CSGEApplication::MainCoeControl() const
    {
    return iAppView;
    }

// -----------------------------------------------------------------------------
// CSGEApplication::OpenBrowserL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEApplication::OpenBrowserL( const TDesC& aUrl )
    {
    RApaLsSession apaLsSession;
    User::LeaveIfError( apaLsSession.Connect() );
    CleanupClosePushL( apaLsSession );
    
    //Get the application UID associated with web browsing
    _LIT8(KStartBrMime, "application/x-web-browse");
    TDataType mimeType( KStartBrMime );
    TUid appUid;
    TInt err = apaLsSession.AppForDataType( mimeType, appUid );

    //Start the web browser application passing it the URL to Open
    TThreadId thread;
    if ( !err )
        {
        err = apaLsSession.StartDocument( aUrl, appUid, thread );
        }
    CleanupStack::PopAndDestroy( &apaLsSession );
    if ( err )
        {
        //Fallback to the built-in browser
        const TInt KWmlBrowserUid = 0x10008D39;
        TUid id( TUid::Uid( KWmlBrowserUid ) );
        TApaTaskList taskList( CEikonEnv::Static()->WsSession() );
        TApaTask task = taskList.FindApp( id );
        if ( task.Exists() )
            {
            HBufC8* param = HBufC8::NewLC( aUrl.Length() + 2 );
            param->Des().Append( _L("4 ") ); //"4 " is to Start/Continue the browser specifying a URL
            param->Des().Append( aUrl );
            task.SendMessage( TUid::Uid( 0 ), *param ); // Uid is not used
            CleanupStack::PopAndDestroy( param );
            }
        else
            {
            HBufC16* param = HBufC16::NewLC( aUrl.Length() + 2 );
            param->Des().Append( _L("4 ") ); //"4 " is to Start/Continue the browser specifying a URL
            param->Des().Append( aUrl );
            RApaLsSession appArcSession;
            // connect to AppArc server 
            User::LeaveIfError( appArcSession.Connect() );
            TThreadId id;
            appArcSession.StartDocument( *param, TUid::Uid( KWmlBrowserUid ), id );
            appArcSession.Close();
            CleanupStack::PopAndDestroy( param );
            }
        }
    }

// -----------------------------------------------------------------------------
// CSGEApplication::FullScreen()
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSGEApplication::FullScreen() const
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CSGEApplication::ApplicationFlags()
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CSGEApplication::ApplicationFlags() const
    {
    return 0;
    }

// -----------------------------------------------------------------------------
// CSGEApplication::SilentKeys()
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSGEApplication::SilentKeys() const
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CSGEApplication::Orientation()
// -----------------------------------------------------------------------------
//
EXPORT_C TSGEApplicationOrientation CSGEApplication::Orientation() const
    {
    return ESGEApplicationOrientationDefault;
    }

// -----------------------------------------------------------------------------
// CSGEApplication::HandleCommandL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEApplication::HandleCommandL( TInt /*aCommand*/ )
    {
    }

// -----------------------------------------------------------------------------
// CSGEApplication::HandleStatusPaneSizeChange()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEApplication::HandleStatusPaneSizeChange()
    {
    }

// -----------------------------------------------------------------------------
// CSGEApplication::HandleWsEventL()
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSGEApplication::HandleWsEventL( const TWsEvent& /*aEvent*/, CCoeControl* /*aDestination*/ )
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CSGEApplication::HandleForegroundEventL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEApplication::HandleForegroundEventL( TBool /*aForeground*/ )
    {
    }

// -----------------------------------------------------------------------------
// CSGEApplication::HandleKeyEventL()
// -----------------------------------------------------------------------------
//
EXPORT_C TKeyResponse CSGEApplication::HandleKeyEventL( const TKeyEvent& /*aKeyEvent*/, TEventCode /*aType*/ )
    {
    return EKeyWasNotConsumed;
    }

// -----------------------------------------------------------------------------
// CSGEApplication::ProcessCommandParametersL()
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSGEApplication::ProcessCommandParametersL( TApaCommand /*aCommand*/, TFileName& aDocumentName, const TDesC8& /*aTail*/ )
    {
    aDocumentName.Zero();
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CSGEApplication::Draw()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEApplication::Draw( const TRect& /*aRect*/ ) const
    {
    }

// -----------------------------------------------------------------------------
// CSGEApplication::SizeChanged()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEApplication::SizeChanged()
    {
    }

// -----------------------------------------------------------------------------
// CSGEApplication::HandlePointerEventL()
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSGEApplication::HandlePointerEventL( const TPointerEvent& /*aPointerEvent*/ )
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CSGEApplication::HandlePointerEventL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEApplication::HandleResourceChange( TInt aType )
    {
    iAppUi->HandleResourceChangeL( aType );
    }


// End of file
