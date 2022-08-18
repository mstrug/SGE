/*
 ============================================================================
 Name		: SGEAudioIPCClient.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : RSGEAudioIPCClient declaration
 ============================================================================
 */

#ifndef SGEAUDIOIPCCLIENT_H
#define SGEAUDIOIPCCLIENT_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "SGEAudioFile.h"

// CLASS DECLARATION

class CSGEFile;

/**
 *  RSGEAudioIPCClient
 * 
 */
NONSHARABLE_CLASS( RSGEAudioIPCClient ) : public RSessionBase
    {
public:

    RSGEAudioIPCClient();
    
    TInt Connect();
    
    TInt CloseServer();
    
    TInt CancelPendingRequests();
    
    TInt AddAudioFromBuffer( const TAny *aData, TUint aId, TUint aBufferDataTypeSize, 
                             TUint aDataLength, TUint aFrequency, 
                             TUint aChannels, TSGEAudioGroup aAudioGroup );
    
    TInt AddFileToLoad( TSGEAudioFile aAudioFile );
    
    TInt AddFileToLoadFromSGEFile( const TDesC& aSGEFilePath, TSGEAudioFile aAudioFile );
    
    TInt RemoveAudioSample( TUint aSampleId );
    
    TInt RemoveAudioSampleAll();
    
    void StartLoadingFiles( TRequestStatus &aStatus );
    
    void CreateAudioStream( TRequestStatus &aStatus );
    
    TInt DestroyAudioStream();
    
    TInt CompactTracks();
    
    TInt TrackState( TUint aTrackId, TSGEAudioTrackState &aState );
    
    TInt TrackCount();
    
    TInt Play( TUint aSampleId, TInt aRepeat );
    
    TInt Stop( TUint aTrackId );
    
    TInt Pause( TUint aTrackId );
    
    TInt Resume( TUint aTrackId );
    
    TInt Mute( TUint aTrackId );
    
    TInt Unmute( TUint aTrackId );
    
    TInt StopAll();
    
    TInt PauseAll();
    
    TInt ResumeAll();
    
    TInt MuteAll();
    
    TInt UnmuteAll();
    
    TInt SetVolume( TUint aSampleId, TInt aVolume );
    
    TInt SetVolumeAll( TInt aVolume );
    
    void SubscribeForInitializationEvents( TRequestStatus &aStatus );
    
    void SubscribeForTrackEvents( TRequestStatus &aStatus );
    
    enum TInitializationEventType
        {
        EFileLoaded,
        EAllFilesLoaded,
        EStreamCreated,
        EStreamError,
        EInitializationEventUninitialised
        };
    TInitializationEventType InitializationEventType();
    
    enum TSoundTrackEventType
        {
        EPlayStarted,
        EPlayFinished,
        ESoundTrackEventUninitialised
        };
    TSoundTrackEventType SoundTrackEventType();

    TInt SampleId();
    
private:

    TInt iInitializationEvent;
    
    TInt iSoundTrackEvent;

    TInt iSampleId;
    
    TPckg< TInt > iPckgInitializationEvent;
    TPckg< TInt > iPckgSoundTrackEvent;
    TPckg< TInt > iPckgSampleId;

    };

#endif // SGEAUDIOIPCCLIENT_H
