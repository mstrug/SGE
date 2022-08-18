/*
 ============================================================================
 Name		: SGEAudioEngine.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEAudioEngine declaration
 ============================================================================
 */

#ifndef SGEAUDIOENGINE_H
#define SGEAUDIOENGINE_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <e32hashtab.h>
#include "SGEAudioEngineObserver.h"
#include "SGEAudioConfig.h"
#include "SGEAudioFileLoader.h"
#include "SGEAudioSoundTrack.h"
#include "SGEAudioStreamBase.h"
#include "SGEAudioFile.h"


// FORWARD DECLARATIONS
class CSGEAudioMixerBase;
class CSGEAudioSample;
class CSGEFile;


// CLASS DECLARATION
/**
 *  CSGEAudioEngine
 * 
 */
NONSHARABLE_CLASS( CSGEAudioEngine ) : public CBase, MSGEAudioFileLoaderObserver, 
                                       MSGEAudioSoundTrackObserver, MSGEAudioStreamObserver
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CSGEAudioEngine();

    /**
     * Two-phased constructor.
     */
    static CSGEAudioEngine* NewL( MSGEAudioEngineObserver &aObserver, TSGEAudioConfig aConfig );

    /**
     * Two-phased constructor.
     */
    static CSGEAudioEngine* NewLC( MSGEAudioEngineObserver &aObserver, TSGEAudioConfig aConfig );

    void Cancel();
    
    // Initialization functions

    /*
     * @param aData is copied
     */
    void AddAudioFromBufferL( const TAny *aData, TUint aId, TUint aBufferDataTypeSize, TUint aDataLength,
                              TUint aFrequency, TUint aChannels, TSGEAudioGroup aAudioGroup );
    
    TInt AddFileToLoad( TSGEAudioFile aFile );

    TInt AddFileToLoad( const TDesC& aSGEFilePath, TSGEAudioFile aFile );
    
    void RemoveAudioSample( TUint aId );

    void RemoveAudioSampleAll();
    
    void StartLoadingFilesL();
    
    void CreateAudioStreamL();
    
    void DestroyAudioStream();
    
    // Track functions

    /*
     * Removes all unused (stopped) tracks up to aTracksCount.
     */
    void CompactTracks( TInt aTracksCount );
    
    TSGEAudioTrackState TrackState( TUint aSoundTrack );
    
    TInt TrackCount() const;
    
    void CreateEmptyTracksL( TInt aTrackCount );
    
    // Usage functions
    
    /*
     * @param aRepeat TSGEAudioRepeat or repeat count
     * @return track number on which auido sample is played is returned or system error
     */
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
    CSGEAudioEngine( MSGEAudioEngineObserver &aObserver, TSGEAudioConfig aConfig );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();
    
    CSGEAudioSoundTrack* GetFreeTrack();
    
    TInt AddAudioSample( CSGEAudioSample *aSoundSample );
    
private: // from MSGEAudioFileLoaderObserver
    
    void AudioFileLoaded( CSGEAudioSample *aSoundSample, TUint aSampleId, TInt aError );

    void AudioFileListEmpty();

private: // from MSGEAudioSoundTrackObserver
    
    void AudioSoundTrackPlayStarted( TUint aTrackId );

    void AudioSoundTrackPlayFinished( TUint aTrackId );
    
private: // from MSGEAudioStreamObserver
    
    void AudioStreamCreated( TInt aError );

    void AudioStreamMixData();

    void AudioStreamFillData( TAny *aBuffer, TInt aBufferSize );

    void AudioStreamError( TInt aError );
    
private: // data
    
    RFs iFs;
    
    MSGEAudioEngineObserver &iObserver; 
    
    TSGEAudioConfig iConfig; 

    CSGEAudioFileLoader *iFileLoader; // owned
    
    CSGEAudioStreamBase *iAudioStream; // owned
    
    CSGEAudioMixerBase *iAudioMixer; // owned

    RPtrHashMap< TUint, CSGEAudioSample > iAudioSamples;

    RHashMap< TInt, TInt > iVolumeDefault;    // TSGEAudioGroup
    
    RPointerArray< CSGEAudioSoundTrack > iSoundTracks;
    };

#endif // SGEAUDIOENGINE_H
