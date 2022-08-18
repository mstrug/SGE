/*
 ============================================================================
 Name		: SGEAudioStreamBase.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEAudioStreamBase implementation
 ============================================================================
 */

#include "SGEAudioStreamBase.h"

CSGEAudioStreamBase::CSGEAudioStreamBase( MSGEAudioStreamObserver &aObserver, TSGEAudioConfig &aConfig )
    : CTimer( EPriorityHigh ), iObserver( aObserver ), iConfig( aConfig )
    {
    // No implementation required
    }

CSGEAudioStreamBase::~CSGEAudioStreamBase()
    {
    User::Free( iStreamBuffer );
    }

void CSGEAudioStreamBase::BaseConstructL()
    {
    CTimer::ConstructL();
    
    iBufferSize =  iConfig.BufferSizeInBytes() * iConfig.iChannels;
    iStreamBuffer = User::AllocZL( iBufferSize );
    iBufferPtr.Set( (TUint8*) iStreamBuffer, iBufferSize );

    CActiveScheduler::Add( this );
    }

void CSGEAudioStreamBase::MixData()
    {
    iObserver.AudioStreamMixData();
    }

void CSGEAudioStreamBase::GetNextData()
    {
    iObserver.AudioStreamFillData( iStreamBuffer, iBufferSize );
    }

void CSGEAudioStreamBase::RunL()
    {
    }

TInt CSGEAudioStreamBase::RunError( TInt /*aError*/ )
    {
    return KErrNone;
    }
