/*
 ============================================================================
 Name		: SGEAudioFileDecoder.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEAudioFileDecoder declaration
 ============================================================================
 */

#ifndef SGEAUDIOFILEDECODER_H
#define SGEAUDIOFILEDECODER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "SGEAudioSample.h"
#include "SGEAudioFile.h"


// CLASS DECLARATION

class MSGEAudioFileDecoderObserver
    {
public:
    
    virtual void AudioFileDecodingFinished( CSGEAudioSample *aSoundSample, TInt aError ) = 0;
    
    };

/**
 *  CSGEAudioFileDecoder
 * 
 */
NONSHARABLE_CLASS( CSGEAudioFileDecoder ) : public CTimer
    {
public:

    /**
     * Destructor.
     */
    ~CSGEAudioFileDecoder();

    virtual void StartDecodingL( const TSGEAudioFile &aAudioFile, const TSGEAudioConfig &aAudioConfig ) = 0;
    
protected:

    /**
     * Constructor for performing 1st stage construction
     */
    CSGEAudioFileDecoder( MSGEAudioFileDecoderObserver &aObserver, RBuf8 &aData );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void BaseConstructL();
    
protected:    // from CTimer  
    
    void RunL();
    
    TInt RunError( TInt aError );

protected:
    
    MSGEAudioFileDecoderObserver &iObserver;
    
    RBuf8 &iData;
    
    };

#endif // SGEAUDIOFILEDECODER_H
