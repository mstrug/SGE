/*
 ============================================================================
 Name		: SGEAudioFileDecoderMP3.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEAudioFileDecoderMP3 declaration
 ============================================================================
 */

#ifndef SGEAUDIOFILEDECODERMP3_H
#define SGEAUDIOFILEDECODERMP3_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <mmf/server/MmfCodec.h>
#include "SGEAudioFileDecoder.h"


// CLASS DECLARATION

/**
 *  CSGEAudioFileDecoderMP3
 * 
 */
NONSHARABLE_CLASS( CSGEAudioFileDecoderMP3 ) : public CSGEAudioFileDecoder 
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CSGEAudioFileDecoderMP3();

    /**
     * Two-phased constructor.
     */
    static CSGEAudioFileDecoderMP3* NewL( MSGEAudioFileDecoderObserver &aObserver, RBuf8 &aData );

    /**
     * Two-phased constructor.
     */
    static CSGEAudioFileDecoderMP3* NewLC( MSGEAudioFileDecoderObserver &aObserver, RBuf8 &aData );

    void StartDecodingL( const TSGEAudioFile &aAudioFile, const TSGEAudioConfig &aAudioConfig );
    
private:

    /**
     * Constructor for performing 1st stage construction
     */
    CSGEAudioFileDecoderMP3( MSGEAudioFileDecoderObserver &aObserver, RBuf8 &aData );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();

private:
    
    CMMFCodec* iCodec;

    };

#endif // SGEAUDIOFILEDECODERMP3_H
