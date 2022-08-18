/*
 ============================================================================
 Name		: SGEAudioFileDecoder.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEAudioFileDecoder implementation
 ============================================================================
 */

#include "SGEAudioFileDecoder.h"


CSGEAudioFileDecoder::CSGEAudioFileDecoder( MSGEAudioFileDecoderObserver &aObserver, RBuf8 &aData )
    : CTimer( EPriorityStandard ), iObserver( aObserver ), iData( aData )
    {
    // No implementation required
    }

CSGEAudioFileDecoder::~CSGEAudioFileDecoder()
    {
    }

void CSGEAudioFileDecoder::BaseConstructL()
    {
    CTimer::ConstructL();

    CActiveScheduler::Add( this );
    }

void CSGEAudioFileDecoder::RunL()
    {
    }

TInt CSGEAudioFileDecoder::RunError( TInt aError )
    {
    iObserver.AudioFileDecodingFinished( NULL, aError );
    
    return KErrNone;
    }

