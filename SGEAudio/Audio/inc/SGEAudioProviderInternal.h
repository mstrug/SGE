/*
 ============================================================================
 Name		: SGEAudioProviderInternal.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEAudioProviderInternal declaration
 ============================================================================
 */

#ifndef SGEAUDIOPROVIDERINTERNAL_H
#define SGEAUDIOPROVIDERINTERNAL_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "SGEAudioIPCClient.h"
#include "SGEAudioEngine.h"
#include "SGEAudioSimpleEngine.h"
#include "SGEAudioProvider.h"
#include "SGEAudioGPAO.h"

// CLASS DECLARATION

/**
 *  CSGEAudioProviderInternal
 * 
 */
NONSHARABLE_CLASS(CSGEAudioProviderInternal ) : public CBase, MSGEAudioEngineObserver, MSGEAudioGPAOObserver
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CSGEAudioProviderInternal();

    /**
     * Two-phased constructor.
     */
    static CSGEAudioProviderInternal* NewL( MSGEAudioProviderObserver *aObserver, TSGEAudioConfig aConfig );

    /**
     * Two-phased constructor.
     */
    static CSGEAudioProviderInternal* NewLC( MSGEAudioProviderObserver *aObserver, TSGEAudioConfig aConfig );

private:

    /**
     * Constructor for performing 1st stage construction
     */
    CSGEAudioProviderInternal( MSGEAudioProviderObserver *aObserver );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL( TSGEAudioConfig aConfig );

private: // from MSGEAudioEngineObserver
    
    void AudioEngineFileLoaded( TInt aSoundSampleId, TInt aError );
    
    void AudioEngineAllFilesLoaded();

    void AudioEngineSoundTrackPlayStarted( TUint aTrackId );

    void AudioEngineSoundTrackPlayFinished( TUint aTrackId );

    void AudioEngineStreamCreated( TInt aError );

    void AudioEngineStreamError( TInt aError );

private: // from MSGEAudioGPAOObserver
    
    void GPAONotifierCancel( CSGEAudioGPAO *aGPAO );
    
    void GPAONotifierL( CSGEAudioGPAO *aGPAO, TInt aStatus );
    
    TInt GPAONotifierError( CSGEAudioGPAO *aGPAO, TInt aError );

public:

    // for singlethreeding:
    
    MSGEAudioProviderObserver *iObserver;
    
    CSGEAudioEngine *iEngine;

    CSGEAudioSimpleEngine *iSimpleEngine;
    
    // for multithreeding:
    
    RSGEAudioIPCClient iClient;

    CSGEAudioGPAO *iInitializationAO;
    
    CSGEAudioGPAO *iTrackEventsAO;
    
    };

#endif // SGEAUDIOPROVIDERINTERNAL_H
