/*
 ============================================================================
 Name		: SGEAudioMixerMono.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEAudioMixerMono declaration
 ============================================================================
 */

#ifndef SGEAUDIOMIXERMONO_H
#define SGEAUDIOMIXERMONO_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "SGEAudioMixerBase.h"


// CLASS DECLARATION

/**
 *  CSGEAudioMixerMono
 * 
 */
NONSHARABLE_CLASS( CSGEAudioMixerMono ) : public CSGEAudioMixerBase
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CSGEAudioMixerMono();

    /**
     * Two-phased constructor.
     */
    static CSGEAudioMixerMono* NewL( TSGEAudioConfig &aConfig, RPointerArray< CSGEAudioSoundTrack > &aSoundTracks );

    /**
     * Two-phased constructor.
     */
    static CSGEAudioMixerMono* NewLC( TSGEAudioConfig &aConfig, RPointerArray< CSGEAudioSoundTrack > &aSoundTracks );

    void MixTracks();

    void FillBuffer( TAny *aBuffer, TInt aBufferSize );

private:

    /**
     * Constructor for performing 1st stage construction
     */
    CSGEAudioMixerMono( TSGEAudioConfig &aConfig, RPointerArray< CSGEAudioSoundTrack > &aSoundTracks );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();

    };

#endif // SGEAUDIOMIXERMONO_H
