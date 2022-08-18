/*
 ============================================================================
 Name		: SGESensorBase.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGESensorBase declaration
 ============================================================================
 */

#ifndef SGESENSORBASE_H
#define SGESENSORBASE_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <sensrvdatalistener.h>


// FORWARD DECLARATIONS
class CSensrvChannel;

// CLASS DECLARATION

/**
 *  CSGESensorBase
 * 
 */
NONSHARABLE_CLASS( CSGESensorBase ) : public CBase, public MSensrvDataListener
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    IMPORT_C ~CSGESensorBase();
    
    IMPORT_C virtual void StartListeningL();
    
    IMPORT_C virtual void StopListening();

protected:

    /**
     * Constructor for performing 1st stage construction
     */
    CSGESensorBase();

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void BaseConstructL( TSensrvChannelTypeId aChannelType );
    
protected: // from MSensrvDataListener

    // void DataReceived( CSensrvChannel& aChannel, TInt aCount, TInt aDataLost ); // implemented in derived class
    
    // void DataError( CSensrvChannel& aChannel, TSensrvErrorSeverity aError ); // implemented in derived class
    
    void GetDataListenerInterfaceL( TUid aInterfaceUid, TAny*& aInterface );

protected: // data
    
    CSensrvChannel *iSensrvChannel;
    
    };

#endif // SGESENSORBASE_H
