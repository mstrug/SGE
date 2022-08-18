/*
 ============================================================================
 Name		: SGEAudioMixerStereo.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEAudioMixerStereo declaration
 ============================================================================
 */

#ifndef SGEAUDIOMIXERSTEREO_H
#define SGEAUDIOMIXERSTEREO_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "SGEAudioMixerBase.h"


// CLASS DECLARATION

/**
 *  CSGEAudioMixerStereo
 * 
 */
NONSHARABLE_CLASS( CSGEAudioMixerStereo ) : public CSGEAudioMixerBase
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CSGEAudioMixerStereo();

    /**
     * Two-phased constructor.
     */
    static CSGEAudioMixerStereo* NewL( TSGEAudioConfig &aConfig, RPointerArray< CSGEAudioSoundTrack > &aSoundTracks );

    /**
     * Two-phased constructor.
     */
    static CSGEAudioMixerStereo* NewLC( TSGEAudioConfig &aConfig, RPointerArray< CSGEAudioSoundTrack > &aSoundTracks );

    void MixTracks();
    
    void FillBuffer( TAny *aBuffer, TInt aBufferSize );

private:

    /**
     * Constructor for performing 1st stage construction
     */
    CSGEAudioMixerStereo( TSGEAudioConfig &aConfig, RPointerArray< CSGEAudioSoundTrack > &aSoundTracks );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();

    };

#endif // SGEAUDIOMIXERSTEREO_H
