/*
 ============================================================================
 Name		: SGEAudioProvider.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEAudioProvider declaration
 ============================================================================
 */

#ifndef SGEAUDIOPROVIDER_H
#define SGEAUDIOPROVIDER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <SGEAudio/SGEAudioConfig.h>
#include <SGEAudio/SGEAudioFile.h>
#include <SGEFile/SGEFile.h>

// CLASS DECLARATION

class CSGEAudioProviderInternal;


/**
 *  MSGEAudioProviderObserver
 * 
 */
class MSGEAudioProviderObserver
    {
public:
    
    virtual void AudioFileLoaded( TInt aSoundSampleId, TInt aError ) = 0;
    
    virtual void AudioAllFilesLoaded() = 0;

    virtual void AudioSoundTrackPlayStarted( TUint aTrackId ) = 0;

    virtual void AudioSoundTrackPlayFinished( TUint aTrackId ) = 0;

    virtual void AudioStreamCreated( TInt aError ) = 0;

    virtual void AudioStreamError( TInt aError ) = 0;
    };


/**
 * CSGEAudioProvider
 *  
 * @version 0.1
 * 
 * part of SGE
 * 
 * 
 * example usage:
 *
 * TSGEAudioConfig config;
 * CSGEAudioProvider *audioProvider = CSGEAudioProvider::NewL( observer, config );
 * audioProvider->LoadFileL( TSGEAudioFile( _L("c:\\test1.wav"), KSampleId1, 
 *                                          ESGEAudioGroupSound, ESGEAudioFileTypeWAV ) );
 * ...
 * audioProvider->Play( KSampleId1, (TSGEAudioRepeat) 3 );
 * ...
 * delete audioProvider;
 * 
 */
NONSHARABLE_CLASS( CSGEAudioProvider ) : public CBase
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    IMPORT_C ~CSGEAudioProvider();

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CSGEAudioProvider* NewL( MSGEAudioProviderObserver *aObserver, TSGEAudioConfig aConfig );

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CSGEAudioProvider* NewLC( MSGEAudioProviderObserver *aObserver, TSGEAudioConfig aConfig );

    IMPORT_C void SetObserver( MSGEAudioProviderObserver *aObserver );
    
    // Initialization functions
    
    IMPORT_C void LoadFileL( TSGEAudioFile aFile );

    IMPORT_C void LoadFileL( const TDesC& aSGEFilePath, TSGEAudioFile aFile );
    
    IMPORT_C void LoadFilesL( RArray< TSGEAudioFile > &aFiles );

    IMPORT_C void LoadFilesL( const TDesC& aSGEFilePath, RArray< TSGEAudioFile > &aFiles );
    
    IMPORT_C void CreateAudioStreamL();
    
    IMPORT_C void DestroyAudioStream();
    
    // Track functions

    IMPORT_C TSGEAudioTrackState TrackState( TUint aSoundTrack );
    
    IMPORT_C TInt TrackCount();
    
    IMPORT_C void CreateEmptyTracksL( TInt aTrackCount );

    // Usage functions

    /*
     * @para aRepeat TSGEAudioRepeat or TInt
     * @return track number on which auido sample is played is returned (0..n) or system error
     */
    IMPORT_C TInt Play( TUint aAudioSampleId, TInt aRepeat );
    
    IMPORT_C void Stop( TUint aSoundTrack );
    
    IMPORT_C void Pause( TUint aSoundTrack );
    
    IMPORT_C void Resume( TUint aSoundTrack );

    IMPORT_C void Mute( TUint aSoundTrack );
    
    IMPORT_C void Unmute( TUint aSoundTrack );

    IMPORT_C void StopAll();
    
    IMPORT_C void PauseAll();
    
    IMPORT_C void ResumeAll();
    
    IMPORT_C void MuteAll();

    IMPORT_C void UnmuteAll();

    /*
     * @param aVolume volume multiplier/divider (power of 2) 
     *        0 - no change
     *        1,2,3... - increase
     *        -1,-2... - decrese
     *        sample value is shifted by this value
     *        sample << aVolume
     *
     */
    IMPORT_C void SetVolume( TUint aSoundTrack, TInt aVolume );
    
    IMPORT_C void SetVolumeGroup( TSGEAudioGroup aGroup, TInt aVolume );

    IMPORT_C void SetVolumeAll( TInt aVolume );

    IMPORT_C void SetVolumeDefault( TSGEAudioGroup aGroup, TInt aVolume );

    IMPORT_C TInt GetMaxVolume() const;
    
    /**
     * Returns ETrue when currently used profile is silent or ringtone is silent,
     * and depending on parameter aMettingAndBeep also if ringtone is beep or
     * profile is meeting.
     */
    IMPORT_C static TBool IsSilentProfileL( TBool aMettingAndBeep = ETrue );
    
private:

    /**
     * Constructor for performing 1st stage construction
     */
    CSGEAudioProvider();

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL( MSGEAudioProviderObserver *aObserver, TSGEAudioConfig aConfig );
    
private: // data
    
    CSGEAudioProviderInternal *iProvider;
    
    };

#endif // SGEAUDIOPROVIDER_H
