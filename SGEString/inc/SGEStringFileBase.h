/*
 ============================================================================
 Name		: SGEStringFileBase.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEStringFileBase declaration
 ============================================================================
 */

#ifndef SGESTRINGFILEBASE_H
#define SGESTRINGFILEBASE_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <e32hashtab.h>

// FORWARD DECLARATIONS
class CSGEStringFileItem;

// CLASS DECLARATION
/**
 *  CSGEStringFileBase
 * 
 */
NONSHARABLE_CLASS( CSGEStringFileBase ) : public CBase
    {
public:

    ~CSGEStringFileBase();

    /**
     * Function checks if aResourceId exists if not then tries to load it from file.
     * If resource is not found then leaves.
     */
    virtual TPtrC GetStringL( TInt aResourceId ) = 0;

    /**
     * Function checks if aResourceId exists if not then returns KNullDesC.
     */
    virtual TPtrC GetString( TInt aResourceId ) = 0;

    void UnloadStrings();
    
protected:

    CSGEStringFileBase();

    void BaseConstructL();

protected: // data
    
    RPtrHashMap< TInt, CSGEStringFileItem > iStringItems;
    
    };

#endif // SGESTRINGFILEBASE_H
