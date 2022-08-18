/*
 ============================================================================
 Name		: SGEAudioIPCSession.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEAudioIPCSession declaration
 ============================================================================
 */

#ifndef SGEAUDIOIPCSESSION_H
#define SGEAUDIOIPCSESSION_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "SGEAudioConfig.h"
#include "SGEAudioFile.h"
#include "SGEAudioEngine.h"

// CLASS DECLARATION

class CSGEAudioIPCSession;

/**
 *  MSGEAudioIPCSessionObserver
 * 
 */
class MSGEAudioIPCSessionObserver
    {
public:
    
    virtual void SessionClosed() = 0;
    
    virtual void SessionRequestedToCloseServer() = 0;
    
    };

/**
 *  CSGEAudioIPCSession
 * 
 */
NONSHARABLE_CLASS(CSGEAudioIPCSession ) : public CSession2, MSGEAudioEngineObserver
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CSGEAudioIPCSession();

    /**
     * Two-phased constructor.
     */
    static CSGEAudioIPCSession* NewL( MSGEAudioIPCSessionObserver &aObserver, 
                                      CSGEAudioEngine &aAudioEngine );

    /**
     * Two-phased constructor.
     */
    static CSGEAudioIPCSession* NewLC( MSGEAudioIPCSessionObserver &aObserver, 
                                       CSGEAudioEngine &aAudioEngine );

private:

    /**
     * Constructor for performing 1st stage construction
     */
    CSGEAudioIPCSession( MSGEAudioIPCSessionObserver &aObserver, CSGEAudioEngine &aAudioEngine );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();
    
    void EventCancelPendingRequestsL( const RMessage2 &aMessage );
    void EventAddAudioFromBufferL( const RMessage2 &aMessage );
    void EventAddFileToLoadL( const RMessage2 &aMessage );
    void EventAddFileToLoadFromSGEFileL( const RMessage2 &aMessage );
    void EventRemoveAudioSampleL( const RMessage2 &aMessage );
    void EventRemoveAudioSampleAllL( const RMessage2 &aMessage );
    void EventStartLoadingFilesL( const RMessage2 &aMessage );
    void EventCreateAudioStreamL( const RMessage2 &aMessage );
    void EventDestroyAudioStreamL( const RMessage2 &aMessage );
    void EventCompactTracksL( const RMessage2 &aMessage );
    void EventTrackStateL( const RMessage2 &aMessage );
    void EventTrackCountL( const RMessage2 &aMessage );
    void EventPlayL( const RMessage2 &aMessage );
    void EventStopL( const RMessage2 &aMessage );
    void EventPauseL( const RMessage2 &aMessage );
    void EventResumeL( const RMessage2 &aMessage );
    void EventMuteL( const RMessage2 &aMessage );
    void EventUnmuteL( const RMessage2 &aMessage );
    void EventStopAllL( const RMessage2 &aMessage );
    void EventPauseAllL( const RMessage2 &aMessage );
    void EventResumeAllL( const RMessage2 &aMessage );
    void EventMuteAllL( const RMessage2 &aMessage );
    void EventUnmuteAllL( const RMessage2 &aMessage );
    void EventSetVolumeL( const RMessage2 &aMessage );
    void EventSetVolumeAllL( const RMessage2 &aMessage );
    void EventSubscribeForInitializationEventsL( const RMessage2 &aMessage );
    void EventSubscribeForTrackEventsL( const RMessage2 &aMessage );
    
private: // from CSession2
    
    void ServiceL( const RMessage2 &aMessage );

public: // from MSGEAudioEngineObserver
    
    void AudioEngineFileLoaded( TInt aSoundSampleId, TInt aError );
    
    void AudioEngineAllFilesLoaded();

    void AudioEngineSoundTrackPlayStarted( TUint aTrackId );

    void AudioEngineSoundTrackPlayFinished( TUint aTrackId );

    void AudioEngineStreamCreated( TInt aError );

    void AudioEngineStreamError( TInt aError );

private:

    MSGEAudioIPCSessionObserver &iObserver;
    
    CSGEAudioEngine &iAudioEngine;

    RMessage2 iMessageInitialization;
    
    RMessage2 iMessageTrackEvents;

    TBool iAllFilesLoadedNotificationQueued;
    
    struct STrackEventQueueElement
        {
        TInt iType;
        TUint iTrackid;
        };
    RArray< STrackEventQueueElement > iTrackEventsQueue;
    };

#endif // SGEAUDIOIPCSESSION_H
