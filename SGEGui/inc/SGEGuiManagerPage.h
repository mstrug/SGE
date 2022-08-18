/*
 ============================================================================
 Name		: SGEGuiManagerPage.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEGuiManagerPage declaration
 ============================================================================
 */

#ifndef SGEGUIMANAGERPAGE_H
#define SGEGUIMANAGERPAGE_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

// FORWARD DECLARATIONS
class CSGEGuiPageBase;

// CLASS DECLARATION

/**
 *  MSGEGuiManagerPageObserver
 * 
 */
class MSGEGuiManagerPageObserver
    {
public:
    
    virtual void PageActivated( TUint aPageId ) = 0;
    
    virtual void PageDeactivated( TUint aPageId ) = 0;    
    
    };


/**
 *  CSGEGuiManagerPage
 * 
 */
class CSGEGuiManagerPage : public CBase
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    IMPORT_C ~CSGEGuiManagerPage();

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CSGEGuiManagerPage* NewL( MSGEGuiManagerPageObserver &aObserver );

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CSGEGuiManagerPage* NewLC( MSGEGuiManagerPageObserver &aObserver );

    IMPORT_C void AddPageL( CSGEGuiPageBase *aPage );
    
    IMPORT_C TBool SwitchPageL( TUint aPageId );
    
    IMPORT_C CSGEGuiPageBase* CurrentPage() const;
    
protected:

    /**
     * Constructor for performing 1st stage construction
     */
    IMPORT_C CSGEGuiManagerPage( MSGEGuiManagerPageObserver &aObserver );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    IMPORT_C void ConstructL();

protected: // data
    
    MSGEGuiManagerPageObserver &iObserver;
    
    RPointerArray< CSGEGuiPageBase > iPages;
    
    CSGEGuiPageBase *iCurrentPage; // not owned
    
    };

#endif // SGEGUIMANAGERPAGE_H
