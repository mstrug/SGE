/*
 ============================================================================
 Name		: SGEAudioMixerBase.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEAudioMixerBase declaration
 ============================================================================
 */

#ifndef SGEAUDIOMIXERBASE_H
#define SGEAUDIOMIXERBASE_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "SGEAudioSoundTrack.h"

// CLASS DECLARATION

const TInt KBufferOutputCount = 2;

/**
 *  CSGEAudioMixerBase
 * 
 */
NONSHARABLE_CLASS( CSGEAudioMixerBase ) : public CBase
    {
public:

    /**
     * Destructor.
     */
    ~CSGEAudioMixerBase();

    /*
     * This function mixes all tracks to one of the internal buffer
     */
    virtual void MixTracks() = 0;

    /*
     * This function copies mixed tracks from internal buffer
     * to the output buffer.
     */
    virtual void FillBuffer( TAny *aBuffer, TInt aBufferSize ) = 0;

protected:

    /**
     * Constructor for performing 1st stage construction
     */
    CSGEAudioMixerBase( TSGEAudioConfig &aConfig, RPointerArray< CSGEAudioSoundTrack > &aSoundTracks );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void BaseConstructL();

protected: // data
    
    TSGEAudioConfig &iConfig;
    
    RPointerArray< CSGEAudioSoundTrack > &iSoundTracks;
    
    // mixed tracks in TInt32 format
    TInt32 *iBufferMixed; // owned

    // mixed tracks in output format
    TFixedArray< TAny*, KBufferOutputCount > iBufferOutput; // owned
    
    TInt iBufferMixedSize;
    
    TInt iBufferOutputSize;
    
    // current buffer is that buffer which was lastly mixed
    TInt iCurrentOutputBuffer;
    };

#endif // SGEAUDIOMIXERBASE_H
