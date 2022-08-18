/*
 ============================================================================
 Name		: SGEAudioEngineObserver.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : MSGEAudioEngineObserver declaration
 ============================================================================
 */

#ifndef SGEAUDIOENGINEOBSERVER_H
#define SGEAUDIOENGINEOBSERVER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>


// CLASS DECLARATION

/**
 *  MSGEAudioEngineObserver
 * 
 */
class MSGEAudioEngineObserver
    {
public:
    
    virtual void AudioEngineFileLoaded( TInt aSoundSampleId, TInt aError ) = 0;
    
    virtual void AudioEngineAllFilesLoaded() = 0;

    virtual void AudioEngineSoundTrackPlayStarted( TUint aTrackId ) = 0;

    virtual void AudioEngineSoundTrackPlayFinished( TUint aTrackId ) = 0;

    virtual void AudioEngineStreamCreated( TInt aError ) = 0;

    virtual void AudioEngineStreamError( TInt aError ) = 0;

    };


#endif // SGEAUDIOENGINEOBSERVER_H
