/*
 ============================================================================
 Name		: SGEAudioSoundTrack.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEAudioSoundTrack declaration
 ============================================================================
 */

#ifndef SGEAUDIOSOUNDTRACK_H
#define SGEAUDIOSOUNDTRACK_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "SGEAudioConfig.h"


// FORWARD DECLARATIONS

class CSGEAudioSoundTrack;

class CSGEAudioSample;


// CLASS DECLARATION

class MSGEAudioSoundTrackObserver
    {
public:
    
    virtual void AudioSoundTrackPlayStarted( TUint aTrackId ) = 0;

    virtual void AudioSoundTrackPlayFinished( TUint aTrackId ) = 0;

    };


/**
 *  CSGEAudioSoundTrack
 * 
 */
NONSHARABLE_CLASS( CSGEAudioSoundTrack ) : public CBase
    {
public:

    /**
     * Constructor for performing 1st stage construction
     */
    CSGEAudioSoundTrack( MSGEAudioSoundTrackObserver &aObserver, TUint aId );

    /**
     * Destructor.
     */
    ~CSGEAudioSoundTrack();

    void Play( CSGEAudioSample *aAudioSample, TSGEAudioRepeat aRepeat );

    void Stop();
    
    void SetPaused( TBool aPaused );

    TBool IsPaused() const;

    void SetMuted( TBool aMuted );

    TBool IsMuted() const;
    
    CSGEAudioSample *AudioSample() const;

    TSGEAudioRepeat Repeat() const;

    void SetVolume( TInt aVolume );
    
    TInt Volume() const;

    TUint Id() const;
    
    TInt CurrentDataLength() const;
    
    /*
     * Returns pointer to the audio data with length returned in aCurrentDataLength
     * parameter.
     * 
     * @param aBufferSize size in bytes of the audio buffer
     * @param aCurrentDataLength length of returned data
     * @return pointer to the data in associated sound sample
     */
    TAny* GetCurrentDataAndAdvance( TInt aBufferSize, TInt &aCurrentDataLength );
    
private:
    
    void ResetState();
    
private: // data
    
    MSGEAudioSoundTrackObserver &iObserver;
    
    TUint iId;
    
    TBool iPaused;

    TBool iMuted;
    
    TInt iRepeat;
    
    // volume multiplier (power of 2)
    // sample << iVolume
    TInt iVolume;

    // pointer to the current data
    TAny *iCurrentData; // not owned
        
    TInt iCurrentDataLength;

    // audio sample played on this track
    CSGEAudioSample *iAudioSample; // not owned

    };

#endif // SGEAUDIOSOUNDTRACK_H
