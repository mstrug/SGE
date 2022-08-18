/*
 ============================================================================
 Name		: SGEGuiManagerPage.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEGuiManagerPage implementation
 ============================================================================
 */

#include "SGEGuiManagerPage.h"
#include "SGEGuiPageBase.h"


// -----------------------------------------------------------------------------
// CSGEGuiManagerPage::CSGEGuiManagerPage()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEGuiManagerPage::CSGEGuiManagerPage( MSGEGuiManagerPageObserver &aObserver )
    : iObserver( aObserver )
    {
    // No implementation required
    }

// -----------------------------------------------------------------------------
// CSGEGuiManagerPage::~CSGEGuiManagerPage()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEGuiManagerPage::~CSGEGuiManagerPage()
    {
    iPages.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CSGEGuiManagerPage::NewLC()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEGuiManagerPage* CSGEGuiManagerPage::NewLC( MSGEGuiManagerPageObserver &aObserver )
    {
    CSGEGuiManagerPage *self = new ( ELeave ) CSGEGuiManagerPage( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CSGEGuiManagerPage::NewL()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEGuiManagerPage* CSGEGuiManagerPage::NewL( MSGEGuiManagerPageObserver &aObserver )
    {
    CSGEGuiManagerPage *self = CSGEGuiManagerPage::NewLC( aObserver );
    CleanupStack::Pop(); // self;
    return self;
    }

// -----------------------------------------------------------------------------
// CSGEGuiManagerPage::ConstructL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEGuiManagerPage::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CSGEGuiManagerPage::AddPageL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEGuiManagerPage::AddPageL( CSGEGuiPageBase *aPage )
    {
    CleanupStack::PushL( aPage );
    iPages.AppendL( aPage );
    CleanupStack::Pop( aPage );
    
    if ( !iCurrentPage )
        {
        iCurrentPage = aPage;
        }
    }

// -----------------------------------------------------------------------------
// CSGEGuiManagerPage::SwitchPageL()
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSGEGuiManagerPage::SwitchPageL( TUint aPageId )
    {
    if ( aPageId == iCurrentPage->Id() )
        {
        return ETrue;
        }
    
    for ( TInt i = 0; i < iPages.Count(); i++ )
        {
        if ( iPages[ i ]->Id() == aPageId )
            {
            iCurrentPage->DeactivateL();
            iObserver.PageDeactivated( iCurrentPage->Id() );
                    
            iCurrentPage = iPages[ i ];
            
            iCurrentPage->ActivateL();
            iObserver.PageActivated( iCurrentPage->Id() );
            
            return ETrue;
            }
        }
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CSGEGuiManagerPage::CurrentPage()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEGuiPageBase* CSGEGuiManagerPage::CurrentPage() const
    {
    return iCurrentPage;
    }


// End of file
