/*
 ============================================================================
 Name		: SGEAudioIPCServer.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEAudioIPCServer declaration
 ============================================================================
 */

#ifndef SGEAUDIOIPCSERVER_H
#define SGEAUDIOIPCSERVER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "SGEAudioEngine.h"
#include "SGEAudioConfig.h"
#include "SGEAudioIPCSession.h"

const TInt KSGEAudioIPCServerDefaultStackSize = 0x2000; 
const TInt KSGEAudioIPCServerDefaultHeapMinSize = 0x100000; 

// CLASS DECLARATION

/**
 *  CSGEAudioIPCServer
 * 
 */
NONSHARABLE_CLASS( CSGEAudioIPCServer ) : public CServer2, public MSGEAudioIPCSessionObserver, 
                                          MSGEAudioEngineObserver
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CSGEAudioIPCServer();

    static void CreateServerL( TSGEAudioConfig aAudioConfig,
                               TThreadPriority aThreadPriority = EPriorityRealTime,
                               TInt aStackSize = KSGEAudioIPCServerDefaultStackSize, 
                               TInt aHeapMinSize = KSGEAudioIPCServerDefaultHeapMinSize );
    
    static TInt CloseServer();
    
private:
    
    /**
     * Two-phased constructor.
     */
    static CSGEAudioIPCServer* NewL( TSGEAudioConfig aAudioConfig );

    /**
     * Two-phased constructor.
     */
    static CSGEAudioIPCServer* NewLC( TSGEAudioConfig aAudioConfig );

private:

    /**
     * Constructor for performing 1st stage construction
     */
    CSGEAudioIPCServer();

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL( TSGEAudioConfig aAudioConfig );
    
    static TInt ThreadFunction( TAny *aParam );

    static TInt ThreadFunctionL( TSGEAudioConfig aAudioConfig );

    static void PanicServer( TInt aPanic );
    
private: // from CServer2

    CSession2* NewSessionL( const TVersion &aVersion, const RMessage2 &aMessage ) const;

private: // from MSGEAudioIPCSessionObserver
    
    void SessionClosed();
    
    void SessionRequestedToCloseServer();
    
private: // from MSGEAudioEngineObserver
    
    void AudioEngineFileLoaded( TInt aSoundSampleId, TInt aError );
    
    void AudioEngineAllFilesLoaded();

    void AudioEngineSoundTrackPlayStarted( TUint aTrackId );

    void AudioEngineSoundTrackPlayFinished( TUint aTrackId );

    void AudioEngineStreamCreated( TInt aError );

    void AudioEngineStreamError( TInt aError );
    
private: // data

    CSGEAudioEngine *iAudioEngine;

    TBool iCloseRequested;
    
    };

#endif // SGEAUDIOIPCSERVER_H
