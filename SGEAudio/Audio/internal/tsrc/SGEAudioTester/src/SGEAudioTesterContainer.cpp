/*
========================================================================
 Name        : SGEAudioTesterContainer.cpp
 Author      : Michal Strug
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : 
========================================================================
*/
// [[[ begin generated region: do not modify [Generated System Includes]
#include <barsread.h>
#include <stringloader.h>
#include <eiklabel.h>
#include <eikenv.h>
#include <aknviewappui.h>
#include <eikappui.h>
#include <SGEAudioTester.rsg>
// ]]] end generated region [Generated System Includes]

// [[[ begin generated region: do not modify [Generated User Includes]
#include "SGEAudioTesterContainer.h"
#include "SGEAudioTesterContainerView.h"
#include "SGEAudioTester.hrh"
#include "SGEAudioTesterContainer.hrh"
#include "SGEAudioTesterSettingItemList.hrh"
#include "SGEAudioTesterSettingItemListFile1.hrh"
// ]]] end generated region [Generated User Includes]

// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

const TInt KLabelFontHeight = 11;

/**
 * First phase of Symbian two-phase construction. Should not 
 * contain any code that could leave.
 */
CSGEAudioTesterContainer::CSGEAudioTesterContainer()
    {
    // [[[ begin generated region: do not modify [Generated Contents]
	iLabelTrack1 = NULL;
    // ]]] end generated region [Generated Contents]
    
    }
/** 
 * Destroy child controls.
 */
CSGEAudioTesterContainer::~CSGEAudioTesterContainer()
    {
    // [[[ begin generated region: do not modify [Generated Contents]
	delete iLabelTrack1;
	iLabelTrack1 = NULL;
    // ]]] end generated region [Generated Contents]
    if ( iLableFont )
        {
        iCoeEnv->ScreenDevice()->ReleaseFont( iLableFont );
        }
    if ( iTapFont )
        {
        iCoeEnv->ScreenDevice()->ReleaseFont( iTapFont );
        }
	delete iPeriodic;
    }
                
/**
 * Construct the control (first phase).
 *  Creates an instance and initializes it.
 *  Instance is not left on cleanup stack.
 * @param aRect bounding rectangle
 * @param aParent owning parent, or NULL
 * @param aCommandObserver command observer
 * @return initialized instance of CSGEAudioTesterContainer
 */
CSGEAudioTesterContainer* CSGEAudioTesterContainer::NewL( 
        const TRect& aRect, 
        const CCoeControl* aParent, 
        MEikCommandObserver* aCommandObserver )
    {
    CSGEAudioTesterContainer* self = CSGEAudioTesterContainer::NewLC( 
            aRect, 
            aParent, 
            aCommandObserver );
    CleanupStack::Pop( self );
    return self;
    }

/**
 * Construct the control (first phase).
 *  Creates an instance and initializes it.
 *  Instance is left on cleanup stack.
 * @param aRect The rectangle for this window
 * @param aParent owning parent, or NULL
 * @param aCommandObserver command observer
 * @return new instance of CSGEAudioTesterContainer
 */
CSGEAudioTesterContainer* CSGEAudioTesterContainer::NewLC( 
        const TRect& aRect, 
        const CCoeControl* aParent, 
        MEikCommandObserver* aCommandObserver )
    {
    CSGEAudioTesterContainer* self = new ( ELeave ) CSGEAudioTesterContainer();
    CleanupStack::PushL( self );
    self->ConstructL( aRect, aParent, aCommandObserver );
    return self;
    }
            
/**
 * Construct the control (second phase).
 *  Creates a window to contain the controls and activates it.
 * @param aRect bounding rectangle
 * @param aCommandObserver command observer
 * @param aParent owning parent, or NULL
 */ 
void CSGEAudioTesterContainer::ConstructL( 
        const TRect& aRect, 
        const CCoeControl* aParent, 
        MEikCommandObserver* aCommandObserver )
    {
    if ( aParent == NULL )
        {
        CreateWindowL();
        }
    else
        {
        SetContainerWindowL( *aParent );
        }
    iFocusControl = NULL;
    iCommandObserver = aCommandObserver;
    InitializeControlsL();
    SetRect( aRect );
    ActivateL();
    // [[[ begin generated region: do not modify [Post-ActivateL initializations]
    // ]]] end generated region [Post-ActivateL initializations]
    
    iLabelTrack1->SetTextL( KNullDesC() );

    TFontSpec fs( _L("Arial"), KLabelFontHeight );
    User::LeaveIfError( iCoeEnv->ScreenDevice()->GetNearestFontInPixels( iLableFont, fs ) );
    iLabelTrack1->SetFont( iLableFont );

    iPeriodic = CPeriodic::New( CActive::EPriorityStandard );
    
    fs = TFontSpec( _L("Arial"), KLabelFontHeight * 2 );
    fs.iFontStyle.SetStrokeWeight( EStrokeWeightBold );
    User::LeaveIfError( iCoeEnv->ScreenDevice()->GetNearestFontInPixels( iTapFont, fs ) );
    }
            
/**
* Return the number of controls in the container (override)
* @return count
*/
TInt CSGEAudioTesterContainer::CountComponentControls() const
    {
    return ( int ) ELastControl;
    }
                
/**
* Get the control with the given index (override)
* @param aIndex Control index [0...n) (limited by #CountComponentControls)
* @return Pointer to control
*/
CCoeControl* CSGEAudioTesterContainer::ComponentControl( TInt aIndex ) const
    {
    // [[[ begin generated region: do not modify [Generated Contents]
	switch ( aIndex )
		{
		case ELabelTrack1:
			return iLabelTrack1;
		}
    // ]]] end generated region [Generated Contents]
    
    // handle any user controls here...
    
    return NULL;
    }
                
/**
 *    Handle resizing of the container. This implementation will lay out
 *  full-sized controls like list boxes for any screen size, and will layout
 *  labels, editors, etc. to the size they were given in the UI designer.
 *  This code will need to be modified to adjust arbitrary controls to
 *  any screen size.
 */                
void CSGEAudioTesterContainer::SizeChanged()
    {
    CCoeControl::SizeChanged();
    TRect r = iLabelTrack1->Rect();
    LayoutControls();
    if ( iLabelTrack1->Rect().Height() < r.Height() )
        {
        iLabelTrack1->SetRect( r );
        }
    
    // [[[ begin generated region: do not modify [Generated Contents]
			
    // ]]] end generated region [Generated Contents]
    
    }
                
// [[[ begin generated function: do not modify
/**
 * Layout components as specified in the UI Designer
 */
void CSGEAudioTesterContainer::LayoutControls()
	{
	iLabelTrack1->SetExtent( TPoint( 1, 1 ), TSize( 215, 68 ) );
	}
// ]]] end generated function

/**
 *    Handle key events.
 */                
TKeyResponse CSGEAudioTesterContainer::OfferKeyEventL( 
        const TKeyEvent& aKeyEvent, 
        TEventCode aType )
    {
    // [[[ begin generated region: do not modify [Generated Contents]
	
    // ]]] end generated region [Generated Contents]
    
    if ( aType == EEventKeyDown )
        { 
        if ( aKeyEvent.iScanCode == EStdKeyNkp1 || aKeyEvent.iScanCode == 0x31 )
            { // 1
            iAvkonViewAppUi->View( TUid::Uid( ESGEAudioTesterContainerViewId ) )->
                                                    HandleCommandL( ESGEAudioTesterContainerViewNumber1 );
            return EKeyWasConsumed;
            }
        else if ( aKeyEvent.iScanCode == EStdKeyNkp2 || aKeyEvent.iScanCode == 0x32 )
            { // 2
            iAvkonViewAppUi->View( TUid::Uid( ESGEAudioTesterContainerViewId ) )->
                                                    HandleCommandL( ESGEAudioTesterContainerViewNumber2 );
            return EKeyWasConsumed;
            }
        else if ( aKeyEvent.iScanCode == EStdKeyNkp3 || aKeyEvent.iScanCode == 0x33 )
            { // 3
            iAvkonViewAppUi->View( TUid::Uid( ESGEAudioTesterContainerViewId ) )->
                                                    HandleCommandL( ESGEAudioTesterContainerViewNumber3 );
            return EKeyWasConsumed;
            }
        else if ( aKeyEvent.iScanCode == EStdKeyNkp4 || aKeyEvent.iScanCode == 0x34 )
            { // 4
            iAvkonViewAppUi->View( TUid::Uid( ESGEAudioTesterContainerViewId ) )->
                                                    HandleCommandL( ESGEAudioTesterContainerViewNumber4 );
            return EKeyWasConsumed;
            }
        else if ( aKeyEvent.iScanCode == EStdKeyNkp5 || aKeyEvent.iScanCode == 0x35 )
            { // 5
            iAvkonViewAppUi->View( TUid::Uid( ESGEAudioTesterContainerViewId ) )->
                                                    HandleCommandL( ESGEAudioTesterContainerViewNumber5 );
            return EKeyWasConsumed;
            }
        else if ( aKeyEvent.iScanCode == EStdKeyNkp6 || aKeyEvent.iScanCode == 0x36 )
            { // 6
            iAvkonViewAppUi->View( TUid::Uid( ESGEAudioTesterContainerViewId ) )->
                                                    HandleCommandL( ESGEAudioTesterContainerViewNumber6 );
            return EKeyWasConsumed;
            }
        else if ( aKeyEvent.iScanCode == EStdKeyNkp7 || aKeyEvent.iScanCode == 0x37 )
            { // 7
            iAvkonViewAppUi->View( TUid::Uid( ESGEAudioTesterContainerViewId ) )->
                                                    HandleCommandL( ESGEAudioTesterContainerViewNumber7 );
            return EKeyWasConsumed;
            }
        else if ( aKeyEvent.iScanCode == EStdKeyNkp8 || aKeyEvent.iScanCode == 0x38 )
            { // 8
            iAvkonViewAppUi->View( TUid::Uid( ESGEAudioTesterContainerViewId ) )->
                                                    HandleCommandL( ESGEAudioTesterContainerViewNumber8 );
            return EKeyWasConsumed;
            }
        else if ( aKeyEvent.iScanCode == EStdKeyNkp9 || aKeyEvent.iScanCode == 0x39 )
            { // 9
            iAvkonViewAppUi->View( TUid::Uid( ESGEAudioTesterContainerViewId ) )->
                                                    HandleCommandL( ESGEAudioTesterContainerViewNumber9 );
            return EKeyWasConsumed;
            }
        else if ( aKeyEvent.iScanCode == EStdKeyHash || aKeyEvent.iScanCode == 0x23 )
            { // #
            iAvkonViewAppUi->View( TUid::Uid( ESGEAudioTesterContainerViewId ) )->
                                                    HandleCommandL( ESGEAudioTesterContainerViewHash );
            return EKeyWasConsumed;
            }
        }

    
    if ( iFocusControl != NULL
        && iFocusControl->OfferKeyEventL( aKeyEvent, aType ) == EKeyWasConsumed )
        {
        return EKeyWasConsumed;
        }
    return CCoeControl::OfferKeyEventL( aKeyEvent, aType );
    }
                
// [[[ begin generated function: do not modify
/**
 *	Initialize each control upon creation.
 */				
void CSGEAudioTesterContainer::InitializeControlsL()
	{
	iLabelTrack1 = new ( ELeave ) CEikLabel;
	iLabelTrack1->SetContainerWindowL( *this );
		{
		TResourceReader reader;
		iEikonEnv->CreateResourceReaderLC( reader, R_SGEAUDIO_TESTER_CONTAINER_LABEL_TRACK1 );
		iLabelTrack1->ConstructFromResourceL( reader );
		CleanupStack::PopAndDestroy(); // reader internal state
		}
	
	}
// ]]] end generated function

/** 
 * Handle global resource changes, such as scalable UI or skin events (override)
 */
void CSGEAudioTesterContainer::HandleResourceChange( TInt aType )
    {
    CCoeControl::HandleResourceChange( aType );
    SetRect( iAvkonViewAppUi->View( TUid::Uid( ESGEAudioTesterContainerViewId ) )->ClientRect() );
    // [[[ begin generated region: do not modify [Generated Contents]
    // ]]] end generated region [Generated Contents]
    
    }
                
void CSGEAudioTesterContainer::HandlePointerEventL(const TPointerEvent& aPointerEvent)
    {    
    TBool b = static_cast< CSGEAudioTesterContainerView* >( 
                 iAvkonViewAppUi->View( TUid::Uid( ESGEAudioTesterContainerViewId ) ) )->IsAudioCreated();
    if ( aPointerEvent.iType == TPointerEvent::EButton1Down )
        {
        if ( aPointerEvent.iPosition.iX < Rect().Width() / 3 )
            {
            if ( aPointerEvent.iPosition.iY < Rect().Height() / 3 )
                {
                if ( b )
                    {
                    iAvkonViewAppUi->View( TUid::Uid( ESGEAudioTesterContainerViewId ) )->
                                                        HandleCommandL( ESGEAudioTesterContainerViewNumber1 );
                    }
                iTapRectDivider.SetXY( 1, 1 );
                }
            else if ( aPointerEvent.iPosition.iY < 2 * Rect().Height() / 3 )
                {
                if ( b )
                    {
                    iAvkonViewAppUi->View( TUid::Uid( ESGEAudioTesterContainerViewId ) )->
                                                        HandleCommandL( ESGEAudioTesterContainerViewNumber4 );
                    }
                iTapRectDivider.SetXY( 1, 2 );
                }
            else
                {
                if ( b )
                    {
                    iAvkonViewAppUi->View( TUid::Uid( ESGEAudioTesterContainerViewId ) )->
                                                        HandleCommandL( ESGEAudioTesterContainerViewNumber7 );
                    }
                iTapRectDivider.SetXY( 1, 3 );
                }
            }
        else if ( aPointerEvent.iPosition.iX < 2 * Rect().Width() / 3 )
            {
            if ( aPointerEvent.iPosition.iY < Rect().Height() / 3 )
                {
                if ( b )
                    {
                    iAvkonViewAppUi->View( TUid::Uid( ESGEAudioTesterContainerViewId ) )->
                                                        HandleCommandL( ESGEAudioTesterContainerViewNumber2 );
                    }
                iTapRectDivider.SetXY( 2, 1 );
                }
            else if ( aPointerEvent.iPosition.iY < 2 * Rect().Height() / 3 )
                {
                if ( b )
                    {
                    iAvkonViewAppUi->View( TUid::Uid( ESGEAudioTesterContainerViewId ) )->
                                                        HandleCommandL( ESGEAudioTesterContainerViewNumber5 );
                    }
                iTapRectDivider.SetXY( 2, 2 );
                }
            else
                {
                if ( b )
                    {
                    iAvkonViewAppUi->View( TUid::Uid( ESGEAudioTesterContainerViewId ) )->
                                                        HandleCommandL( ESGEAudioTesterContainerViewNumber8 );
                    }
                iTapRectDivider.SetXY( 2, 3 );
                }
            }
        else
            {
            if ( aPointerEvent.iPosition.iY < Rect().Height() / 3 )
                {
                if ( b )
                    {
                    iAvkonViewAppUi->View( TUid::Uid( ESGEAudioTesterContainerViewId ) )->
                                                        HandleCommandL( ESGEAudioTesterContainerViewNumber3 );
                    }
                iTapRectDivider.SetXY( 3, 1 );
                }
            else if ( aPointerEvent.iPosition.iY < 2 * Rect().Height() / 3 )
                {
                if ( b )
                    {
                    iAvkonViewAppUi->View( TUid::Uid( ESGEAudioTesterContainerViewId ) )->
                                                        HandleCommandL( ESGEAudioTesterContainerViewNumber6 );
                    }
                iTapRectDivider.SetXY( 3, 2 );
                }
            else
                {
                if ( b )
                    {
                    iAvkonViewAppUi->View( TUid::Uid( ESGEAudioTesterContainerViewId ) )->
                                                        HandleCommandL( ESGEAudioTesterContainerViewNumber9 );
                    }
                iTapRectDivider.SetXY( 3, 3 );
                }
            }
        DrawNow();
        
        TCallBack cb( CleanTapRect, (TAny*) this );
        iPeriodic->Cancel();
        iPeriodic->Start( 500000, 0, cb );
        }
    else if ( aPointerEvent.iType == TPointerEvent::EButton1Up )
        {
//        iTapRectDivider.SetXY( 0, 0 );
//        DrawNow();
        }

    CCoeControl::HandlePointerEventL( aPointerEvent );
//    CEikonEnv::Static()->AlertWin( _L("aa") );
    }


/**
 *    Draw container contents.
 */                
void CSGEAudioTesterContainer::Draw( const TRect& aRect ) const
    {
    // [[[ begin generated region: do not modify [Generated Contents]
	CWindowGc& gc = SystemGc();
	gc.Clear( aRect );
	
    // ]]] end generated region [Generated Contents]

	if ( iTapRectDivider != TPoint() )
	    {
        gc.SetBrushColor( TRgb( 255, 255, 255 ) );
        gc.SetBrushStyle( CWindowGc::EForwardDiagonalHatchBrush );
        gc.SetPenStyle( CWindowGc::ENullPen );
        gc.SetPenColor( TRgb( 180, 0, 0 ) );
        TInt w = Rect().Width() / 3;
        TInt h = Rect().Height() / 3;
        TRect r( TPoint( ( iTapRectDivider.iX - 1 ) * w, ( iTapRectDivider.iY - 1 ) * h ), TSize( w, h ) );
        r.Shrink( 5, 5 );
        gc.DrawRect( r );
        
        gc.UseFont( iTapFont );
        gc.SetPenColor( TRgb( 0, 0, 0 ) );
        TBuf<30> buf;
        buf.AppendNum( iTapRectDivider.iX * iTapRectDivider.iY );
        TPoint tpos( r.iBr );
        TInt tmargin = 5;
        tpos.iX -= tmargin + iTapFont->TextWidthInPixels( buf );
        tpos.iY -= tmargin;
        gc.DrawText( buf, tpos );
        gc.DiscardFont();
	    }
    }

void CSGEAudioTesterContainer::SetLabelTextL( TInt aNumber, const TDesC& aText )
    {
    TRect r( iLabelTrack1->Rect() );
    TInt h = ( aNumber + 3 ) * KLabelFontHeight - r.Height();
    if ( h > 0 )
        {
        r.iBr.iY += h;
        }
    iLabelTrack1->SetRect( r );
    iLabelTrack1->SetTextL( aText );
    DrawNow();
    }

TInt CSGEAudioTesterContainer::CleanTapRect( TAny *aParam )
    {
    CSGEAudioTesterContainer *self = (CSGEAudioTesterContainer*) aParam;
    self->iTapRectDivider.SetXY( 0, 0 );
    self->DrawNow();
    self->iPeriodic->Cancel();
    return KErrNone;
    }



