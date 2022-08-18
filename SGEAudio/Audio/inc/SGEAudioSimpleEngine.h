/*
 ============================================================================
 Name		: SGEAudioSimpleEngine.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEAudioSimpleEngine declaration
 ============================================================================
 */

#ifndef SGEAUDIOSIMPLEENGINE_H
#define SGEAUDIOSIMPLEENGINE_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <e32hashtab.h>
#include "SGEAudioConfig.h"
#include "SGEAudioFile.h"
#include "SGEAudioEngineObserver.h"
#include "SGEAudioSimplePlayer.h"


// FORWARD DECLARATIONS
class CSGEAudioSample;

// CLASS DECLARATION
/**
 *  CSGEAudioSimpleEngine
 * 
 */
NONSHARABLE_CLASS( CSGEAudioSimpleEngine ) : public CBase, public MSGEAudioSimplePlayerObserver
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CSGEAudioSimpleEngine();

    /**
     * Two-phased constructor.
     */
    static CSGEAudioSimpleEngine* NewL( MSGEAudioEngineObserver &aObserver, TSGEAudioConfig aConfig );

    /**
     * Two-phased constructor.
     */
    static CSGEAudioSimpleEngine* NewLC( MSGEAudioEngineObserver &aObserver, TSGEAudioConfig aConfig );

    void AddAudioFromBufferL( const TAny *aData, TUint aId, TUint aBufferDataTypeSize, TUint aDataLength,
                              TUint aFrequency, TUint aChannels, TSGEAudioGroup aAudioGroup );
    
    TInt AddFileToLoad( TSGEAudioFile aFile );

    TInt AddFileToLoad( const TDesC& aSGEFilePath, TSGEAudioFile aFile );
    
    void RemoveAudioSample( TUint aId );

    void RemoveAudioSampleAll();
    
    void StartLoadingFilesL();
    
    void CreateAudioStreamL();
    
    void DestroyAudioStream();

    TSGEAudioTrackState TrackState( TUint aSoundTrack );
    
    TInt TrackCount() const;
    
    void CreateEmptyTracksL( TInt aTrackCount );

    TInt Play( TUint aAudioSampleId, TInt aRepeat );
    
    void Stop( TUint aSoundTrack );
    
    void Pause( TUint aSoundTrack );
    
    void Resume( TUint aSoundTrack );

    void Mute( TUint aSoundTrack );
    
    void Unmute( TUint aSoundTrack );

    void StopAll();
    
    void PauseAll();
    
    void ResumeAll();
    
    void MuteAll();

    void UnmuteAll();

    void SetVolume( TUint aSoundTrack, TInt aVolume );

    void SetVolumeGroup( TSGEAudioGroup aGroup, TInt aVolume );
    
    void SetVolumeAll( TInt aVolume );

    void SetVolumeDefault( TSGEAudioGroup aGroup, TInt aVolume );

    TInt GetMaxVolume() const;
    
private:

    /**
     * Constructor for performing 1st stage construction
     */
    CSGEAudioSimpleEngine( MSGEAudioEngineObserver &aObserver, TSGEAudioConfig aConfig );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();

    void ResetAudioSamples();

    TInt FindPlayer( TUint aAudioSample );

    TInt GetPlayerL();

    void AddFileToLoadL( const TDesC& aSGEFilePath, TSGEAudioFile aFile );

    TInt PlayL( TUint aAudioSampleId, TInt aRepeat );

    static TInt StartLoadingFilesCallback( TAny *aParam );

    static TInt CreateStreamCallback( TAny *aParam );

    static TInt PlayStartedCallback( TAny *aParam );
    
private: // from MSGEAudioSimplePlayerObserver
    
    void SimplePlayerInitComplete( CSGEAudioSimplePlayer *aPlayer, TInt aError );
    
    void SimplePlayerPlayComplete( CSGEAudioSimplePlayer *aPlayer, TInt aError );
    
private: // data

    RFs iFs;
    
    MSGEAudioEngineObserver &iObserver;
    
    TSGEAudioConfig iConfig;

    RHashMap< TUint, CSGEAudioSample* > iAudioSamples;

    RPointerArray< CSGEAudioSimplePlayer > iPlayers;

    RHashMap< TInt, TInt > iVolumeDefault;    // TSGEAudioGroup

    RPointerArray< TAny > iCallbackParameters;

    CAsyncCallBack *iAsyncCallback1;

    CAsyncCallBack *iAsyncCallback2;

    CAsyncCallBack *iAsyncCallback3;
    
    };

#endif // SGEAUDIOSIMPLEENGINE_H
