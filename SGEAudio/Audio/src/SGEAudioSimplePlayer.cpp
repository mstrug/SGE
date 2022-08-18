/*
 ============================================================================
 Name		: SGEAudioSimplePlayer.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEAudioSimplePlayer implementation
 ============================================================================
 */

#include "SGEAudioSimplePlayer.h"
#include "SGEAudioConfig.h"
#include "SGEAudioSample.h"
#include <mda/client/Resource.h> 





CSGEAudioSimplePlayer::CSGEAudioSimplePlayer( MSGEAudioSimplePlayerObserver &aObserver )
    : iObserver( aObserver )
    {
    }

CSGEAudioSimplePlayer::~CSGEAudioSimplePlayer()
    {
    if ( iAudioPlayer )
        {
        iAudioPlayer->Close();
        }
    delete iAudioPlayer;
    }

CSGEAudioSimplePlayer* CSGEAudioSimplePlayer::NewLC( MSGEAudioSimplePlayerObserver &aObserver )
    {
    CSGEAudioSimplePlayer *self = new (ELeave) CSGEAudioSimplePlayer( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

CSGEAudioSimplePlayer* CSGEAudioSimplePlayer::NewL( MSGEAudioSimplePlayerObserver &aObserver )
    {
    CSGEAudioSimplePlayer *self = CSGEAudioSimplePlayer::NewLC( aObserver );
    CleanupStack::Pop(); // self;
    return self;
    }

void CSGEAudioSimplePlayer::ConstructL()
    {
    iAudioPlayer = CMdaAudioPlayerUtility::NewL( *this );
    iAudioPlayer->UseSharedHeap();
    }

void CSGEAudioSimplePlayer::PlayL( CSGEAudioSample* aAudioSample, TInt aRepeats )
    {
    ASSERT( !iPlayStarted );
    //iAudioPlayer->OpenDesL( TPtrC8( (TUint8*) aAudioSample->iData ) );
    iAudioPlayer->OpenDesL( aAudioSample->Data() );
    iRepeats = aRepeats;
    iPlayStarted = ETrue;
    iInitialized = EFalse;
    iAudioSample = aAudioSample;
    }

void CSGEAudioSimplePlayer::PlayAgain()
	{
	iAudioPlayer->Play(); // S^3 ERROR!!! this is not working now
    iPlayStarted = ETrue;
	}

TBool CSGEAudioSimplePlayer::IsPlaying() const
    {
    return iPlayStarted;
    }

TBool CSGEAudioSimplePlayer::IsInitialized() const
    {
    return iInitialized;
    }

void CSGEAudioSimplePlayer::Close()
	{
	if ( iPlayStarted )
		{
		iAudioPlayer->Stop();
		}
	if ( iInitialized )
		{
		iAudioPlayer->Close();
		}
	iPlayStarted = EFalse;
	iInitialized = EFalse;
	}

void CSGEAudioSimplePlayer::Stop()
    {
    if ( iInitialized )
        {
        iPlayStarted = EFalse;
        iAudioPlayer->Stop();
        }
    }

void CSGEAudioSimplePlayer::Mute()
    {
    if ( iInitialized )
        {
        TInt vol;
        iAudioPlayer->GetVolume( vol );
        if ( vol )
            {
            iVolume = vol;
            }
        iAudioPlayer->SetVolume( 0 );
        }
    }

void CSGEAudioSimplePlayer::Unmute()
    {
    if ( iInitialized )
        {
        iAudioPlayer->SetVolume( iVolume );
        }
    }

void CSGEAudioSimplePlayer::SetVolume( TInt aVolume )
    {
    iVolume += aVolume;
    if ( iInitialized )
        {
        iAudioPlayer->SetVolume( iVolume );
        }
    }

void CSGEAudioSimplePlayer::SetVolumeAbsolute( TInt aVolume )
    {
    iVolume = aVolume;
    if ( iInitialized )
        {
        iAudioPlayer->SetVolume( iVolume );
        }
    }

CMdaAudioPlayerUtility& CSGEAudioSimplePlayer::Player() const
    {
    return *iAudioPlayer;
    }

TInt CSGEAudioSimplePlayer::MaxVolume() const
	{
	return iAudioPlayer->MaxVolume();
	}

const CSGEAudioSample* CSGEAudioSimplePlayer::AudioSample() const
    {
    return iAudioSample;
    }

TInt CSGEAudioSimplePlayer::Repeats() const
    {
    return iRepeats;
    }

void CSGEAudioSimplePlayer::SetDefaultPercentVolume( TInt aPercentVolume )
    {
    iDefaultPercentVolume = aPercentVolume;
    }

void CSGEAudioSimplePlayer::MapcInitComplete( TInt aError, const TTimeIntervalMicroSeconds& /*aDuration*/ )
    {
    if ( aError == KErrNone )
        {
        iInitialized = ETrue;
        if ( iRepeats == ESGEAudioRepeatInfinite )
            {
            iAudioPlayer->SetRepeats( KMdaRepeatForever, 0 );
            }
        else 
            {
            iAudioPlayer->SetRepeats( iRepeats, 0 );
            }
        if ( iVolume == 0 )
            {
            iAudioPlayer->SetVolume( iAudioPlayer->MaxVolume() * iDefaultPercentVolume / 100 );
            }
        else
            {
            iAudioPlayer->SetVolume( iVolume );
            }
        iAudioPlayer->Play();
        }
    else
        {
        iPlayStarted = EFalse;
        }
    iObserver.SimplePlayerInitComplete( this, aError );
    }

void CSGEAudioSimplePlayer::MapcPlayComplete( TInt aError )
    {
    iPlayStarted = EFalse;
    iObserver.SimplePlayerPlayComplete( this, aError );
    }

// end of file
