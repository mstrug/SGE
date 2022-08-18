/*
 ============================================================================
 Name		: SGEAudioStreamBase.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEAudioStreamBase declaration
 ============================================================================
 */

#ifndef SGEAUDIOSTREAMBASE_H
#define SGEAUDIOSTREAMBASE_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "SGEAudioConfig.h"

// CLASS DECLARATION

class MSGEAudioStreamObserver
    {
public:
    
    virtual void AudioStreamCreated( TInt aError ) = 0; 

    virtual void AudioStreamMixData() = 0;
    
    virtual void AudioStreamFillData( TAny *aBuffer, TInt aBufferSize ) = 0;    

    virtual void AudioStreamError( TInt aError ) = 0; 
    
    };

/**
 *  CSGEAudioStreamBase
 * 
 */
NONSHARABLE_CLASS( CSGEAudioStreamBase ) : public CTimer
    {
public:

    /**
     * Destructor.
     */
    ~CSGEAudioStreamBase();

    virtual void UpdateNow() = 0;

    virtual TInt MaxVolume() = 0;
    
protected:

    /**
     * Constructor for performing 1st stage construction
     */
    CSGEAudioStreamBase( MSGEAudioStreamObserver &aObserver, TSGEAudioConfig &aConfig );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void BaseConstructL();
    
    void MixData();

    void GetNextData();
    
protected: // from CTimer
    
    virtual void RunL();

    TInt RunError( TInt aError );
    
protected: // data
    
    MSGEAudioStreamObserver &iObserver;
    
    TSGEAudioConfig &iConfig; 

    TAny *iStreamBuffer;    // owned
    
    TPtrC8 iBufferPtr;
    
    TInt iBufferSize;
    
    };

#endif // SGEAUDIOSTREAMBASE_H
