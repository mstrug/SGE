/*
 ============================================================================
 Name		: SGEAudioMixerBase.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEAudioMixerBase implementation
 ============================================================================
 */

#include "SGEAudioMixerBase.h"

CSGEAudioMixerBase::CSGEAudioMixerBase( TSGEAudioConfig &aConfig, 
                                        RPointerArray< CSGEAudioSoundTrack > &aSoundTracks )
    : iConfig( aConfig ), iSoundTracks( aSoundTracks ) 
    {
    // No implementation required
    }

CSGEAudioMixerBase::~CSGEAudioMixerBase()
    {
    User::Free( iBufferMixed );
    User::Free( iBufferOutput[ 0 ] );
    User::Free( iBufferOutput[ 1 ] );
    }

void CSGEAudioMixerBase::BaseConstructL()
    {
    iBufferMixedSize = sizeof( TInt32 ) * iConfig.iBufferSize * iConfig.iChannels;
    iBufferMixed = (TInt32*) User::AllocZL( iBufferMixedSize );
    
    iBufferOutputSize = iConfig.BufferSizeInBytes() * iConfig.iChannels;
    iBufferOutput[ 0 ] = User::AllocZL( iBufferOutputSize );
    iBufferOutput[ 1 ] = User::AllocZL( iBufferOutputSize );
    }


