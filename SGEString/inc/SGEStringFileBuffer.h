/*
 ============================================================================
 Name		: SGEStringFileBuffer.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEStringFileBuffer declaration
 ============================================================================
 */

#ifndef SGESTRINGFILEBUFFER_H
#define SGESTRINGFILEBUFFER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "SGEStringFileBase.h"

// CLASS DECLARATION

/**
 *  CSGEStringFileBuffer
 * 
 */
NONSHARABLE_CLASS( CSGEStringFileBuffer ) : public CSGEStringFileBase
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CSGEStringFileBuffer();

    /**
     * Two-phased constructor.
     */
    static CSGEStringFileBuffer* NewL( const TDesC& aString, TInt aStringId );

    /**
     * Two-phased constructor.
     */
    static CSGEStringFileBuffer* NewLC( const TDesC& aString, TInt aStringId );

    /**
     * Function returns assigned string or leaves with KErrNotFound if id passed to function is
     * different than id of the string.
     */
    TPtrC GetStringL( TInt aResourceId );

    /**
     * Function returns assigned string or KNullDesC if id passed to function is
     * different than id of the string.
     */
    TPtrC GetString( TInt aResourceId );
    
private:

    /**
     * Constructor for performing 1st stage construction
     */
    CSGEStringFileBuffer( TInt aStringId );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL( const TDesC& aString );

private: // data
    
    TInt iStringId;
    
    RBuf iString;
    
    };

#endif // SGESTRINGFILEBUFFER_H
