/*
 ============================================================================
 Name		: SGEAudioSimplePlayer.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEAudioSimplePlayer declaration
 ============================================================================
 */

#ifndef SGEAUDIOSIMPLEPLAYER_H
#define SGEAUDIOSIMPLEPLAYER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <MdaAudioSamplePlayer.h>

// FORWARD DECLARATIONS
class CSGEAudioSimplePlayer;
class CSGEAudioSample;

// CLASS DECLARATION

/**
 *  MSGEAudioSimplePlayerObserver
 * 
 */
class MSGEAudioSimplePlayerObserver
    {
public:
    
    virtual void SimplePlayerInitComplete( CSGEAudioSimplePlayer *aPlayer, TInt aError ) = 0;
    
    virtual void SimplePlayerPlayComplete( CSGEAudioSimplePlayer *aPlayer, TInt aError ) = 0;
    
    };

/**
 *  CSGEAudioSimplePlayer
 * 
 */
NONSHARABLE_CLASS( CSGEAudioSimplePlayer ) : public CBase, public MMdaAudioPlayerCallback
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CSGEAudioSimplePlayer();

    /**
     * Two-phased constructor.
     */
    static CSGEAudioSimplePlayer* NewL( MSGEAudioSimplePlayerObserver &aObserver );

    /**
     * Two-phased constructor.
     */
    static CSGEAudioSimplePlayer* NewLC( MSGEAudioSimplePlayerObserver &aObserver );

    void PlayL( CSGEAudioSample* aAudioSample, TInt aRepeats );

    void PlayAgain();

    TBool IsPlaying() const;

    TBool IsInitialized() const;

    void Close();

    void Stop();
    
    void Mute();
    
    void Unmute();

    void SetVolume( TInt aVolume );

    void SetVolumeAbsolute( TInt aVolume );

    CMdaAudioPlayerUtility& Player() const;
    
    TInt MaxVolume() const; // volume on Bada OS is from 0 to 99

    const CSGEAudioSample* AudioSample() const;
    
    TInt Repeats() const;
    
    void SetDefaultPercentVolume( TInt aPercentVolume );
    
private:

    /**
     * Constructor for performing 1st stage construction
     */
    CSGEAudioSimplePlayer( MSGEAudioSimplePlayerObserver &aObserver );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();

private: // from MMdaAudioPlayerCallback
    
    void MapcInitComplete( TInt aError, const TTimeIntervalMicroSeconds &aDuration );
    
    void MapcPlayComplete( TInt aError );

private: // data

    MSGEAudioSimplePlayerObserver &iObserver; 
    
    CMdaAudioPlayerUtility *iAudioPlayer; // owned
    
    TBool iPlayStarted;
    
    TBool iInitialized;
    
    TInt iVolume;
    
    TInt iDefaultPercentVolume;
    
    TInt iRepeats;
    
    CSGEAudioSample* iAudioSample;
    
    };

#endif // SGEAUDIOSIMPLEPLAYER_H
