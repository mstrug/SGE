/*
 ============================================================================
 Name		: SGEAudioSoundTrack.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEAudioSoundTrack implementation
 ============================================================================
 */

#include "SGEAudioSoundTrack.h"
#include "SGEAudioSample.h"

const TInt KDefaultVolume = 0;

CSGEAudioSoundTrack::CSGEAudioSoundTrack( MSGEAudioSoundTrackObserver &aObserver, TUint aId )
    : iObserver( aObserver ), iId( aId ), iVolume( KDefaultVolume )
    {
    // No implementation required
    }

CSGEAudioSoundTrack::~CSGEAudioSoundTrack()
    {
    }

void CSGEAudioSoundTrack::Play( CSGEAudioSample *aAudioSample, TSGEAudioRepeat aRepeat )
    {
    ASSERT( !iAudioSample );
    iAudioSample = aAudioSample;
    iRepeat = (TInt) aRepeat;
    
    iCurrentData = iAudioSample->iData;
    iCurrentDataLength = iAudioSample->iDataLength;

    iPaused = EFalse;
    }

void CSGEAudioSoundTrack::Stop()
    {
    ResetState();
    }

void CSGEAudioSoundTrack::SetPaused( TBool aPaused )
    {
    iPaused = aPaused;
    }

TBool CSGEAudioSoundTrack::IsPaused() const
    {
    return iPaused;
    }

void CSGEAudioSoundTrack::SetMuted( TBool aMuted )
    {
    iMuted = aMuted;
    }

TBool CSGEAudioSoundTrack::IsMuted() const
    {
    return iMuted;
    }

CSGEAudioSample* CSGEAudioSoundTrack::AudioSample() const
    {
    return iAudioSample;
    }

TSGEAudioRepeat CSGEAudioSoundTrack::Repeat() const
    {
    return (TSGEAudioRepeat) iRepeat;
    }

void CSGEAudioSoundTrack::SetVolume( TInt aVolume )
    {
    iVolume = aVolume;
    }

TInt CSGEAudioSoundTrack::Volume() const
    {
    if ( iMuted )
        {
        return 0;
        }
    else
        {
        return iVolume;
        }
    }

TUint CSGEAudioSoundTrack::Id() const
    {
    return iId;
    }

TInt CSGEAudioSoundTrack::CurrentDataLength() const
    {
    return iCurrentDataLength;
    }

TAny* CSGEAudioSoundTrack::GetCurrentDataAndAdvance( TInt aBufferSize, TInt &aCurrentDataLength )
    {
    ASSERT( iAudioSample );

    if ( iPaused )
        {
        aCurrentDataLength = 0;
        return NULL;
        }

    // todo: mozliwe ze trzeba usunac te notyfikacje bo beda za dlugo trwaly
    if ( iCurrentData == iAudioSample->iData )
        {
        iObserver.AudioSoundTrackPlayStarted( iId );
        }
        
    TAny *ptr = iCurrentData;

    if ( iAudioSample->iDataTypeSize == 2 )
        {
        iCurrentData = (TUint16*) iCurrentData + aBufferSize;
        }
    else if ( iAudioSample->iDataTypeSize == 1 )
        {
        iCurrentData = (TUint8*) iCurrentData + aBufferSize;
        }
    iCurrentDataLength -=  aBufferSize;
    aCurrentDataLength = aBufferSize;
    
    if ( iCurrentDataLength <= 0 )
        {
        aCurrentDataLength = aBufferSize + iCurrentDataLength;
        if ( iRepeat == ESGEAudioRepeatNone )
            {
            iObserver.AudioSoundTrackPlayFinished( iId );
            ResetState();
            }
        else 
            {
            if ( iRepeat > 0 )
                {
                --iRepeat;
                }   // else = odtwarzanie nieskonczona petla
            iCurrentData = iAudioSample->iData;
            iCurrentDataLength = iAudioSample->iDataLength;
            }
        }

    return ptr;
    }

void CSGEAudioSoundTrack::ResetState()
    {
    iCurrentData = NULL;
    iAudioSample = NULL;
    iRepeat = (TInt) ESGEAudioRepeatNone;
    iCurrentDataLength = 0;
    iPaused = EFalse;
    }

