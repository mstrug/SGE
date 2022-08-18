/*
 ============================================================================
 Name		: SGEAudioFileDecoderWAV.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEAudioFileDecoderWAV declaration
 ============================================================================
 */

#ifndef SGEAUDIOFILEDECODERWAV_H
#define SGEAUDIOFILEDECODERWAV_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "SGEAudioFileDecoder.h"

// CLASS DECLARATION

/**
 *  CSGEAudioFileDecoderWAV
 * 
 */
NONSHARABLE_CLASS( CSGEAudioFileDecoderWAV ) : public CSGEAudioFileDecoder
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CSGEAudioFileDecoderWAV();

    /**
     * Two-phased constructor.
     */
    static CSGEAudioFileDecoderWAV* NewL( MSGEAudioFileDecoderObserver &aObserver, RBuf8 &aData );

    /**
     * Two-phased constructor.
     */
    static CSGEAudioFileDecoderWAV* NewLC( MSGEAudioFileDecoderObserver &aObserver, RBuf8 &aData );

    void StartDecodingL( const TSGEAudioFile &aAudioFile, const TSGEAudioConfig &aAudioConfig );
    
private:

    /**
     * Constructor for performing 1st stage construction
     */
    CSGEAudioFileDecoderWAV( MSGEAudioFileDecoderObserver &aObserver, RBuf8 &aData );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();

private:
    
    struct SWaveFmt
        {
        TUint16 iCompressionCode;
        TUint16 iNumberOfChannels;
        TUint32 iSampleRate;
        TUint32 iAverageBytesPerSecond;
        TUint16 iBlockAlign;
        TUint16 iSignificantBitsPerSample;
        };
    
    };

#endif // SGEAUDIOFILEDECODERWAV_H
