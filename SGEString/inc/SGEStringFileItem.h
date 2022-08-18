/*
 ============================================================================
 Name		: SGEStringFileItem.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEStringFileItem declaration
 ============================================================================
 */

#ifndef SGESTRINGFILEITEM_H
#define SGESTRINGFILEITEM_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <BARSREAD.H>

// FORWARD DECLARATIONS
class CResourceFile;
class RResourceFile;

// CLASS DECLARATION
/**
 *  CSGEStringFileItem
 * 
 */
NONSHARABLE_CLASS( CSGEStringFileItem ) : public CBase
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CSGEStringFileItem();

    /**
     * Two-phased constructor.
     */
    static CSGEStringFileItem* NewL( CResourceFile& aFile, TInt aResourceId );

    /**
     * Two-phased constructor.
     */
    static CSGEStringFileItem* NewL( RResourceFile& aFile, TInt aResourceId );

    TInt ResourceId() const;

    TPtrC String() const;
    
private:

    /**
     * Constructor for performing 1st stage construction
     */
    CSGEStringFileItem( TInt aResourceId );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    template<class T> void ConstructL( T &aFile );

private: // data
    
    TInt iResourceId;
    
    HBufC8* iBuffer; // owned

    TResourceReader iResourceReader;

    TPtrC iString;
    
    };

#endif // SGESTRINGFILEITEM_H
