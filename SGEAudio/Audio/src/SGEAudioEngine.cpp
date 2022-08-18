/*
 ============================================================================
 Name		: SGEAudioEngine.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEAudioEngine implementation
 ============================================================================
 */

#include "SGEAudioEngine.h"
#include "SGEAudioSample.h"
#include "SGEAudioStreamMDA.h"
#include "SGEAudioStreamMMF.h"
#include "SGEAudioMixerMono.h"
#include "SGEAudioMixerStereo.h"


CSGEAudioEngine::CSGEAudioEngine( MSGEAudioEngineObserver &aObserver, TSGEAudioConfig aConfig )
    : iObserver( aObserver ), iConfig( aConfig ), iSoundTracks( 8 )
    {
    // No implementation required
    }

CSGEAudioEngine::~CSGEAudioEngine()
    {
    iFs.Close();
    delete iAudioMixer;
    delete iAudioStream;
    delete iFileLoader;
    iSoundTracks.ResetAndDestroy();
    iAudioSamples.ResetAndDestroy();
    iVolumeDefault.Close();
    }

CSGEAudioEngine* CSGEAudioEngine::NewLC( MSGEAudioEngineObserver &aObserver, TSGEAudioConfig aConfig )
    {
    CSGEAudioEngine *self = new (ELeave) CSGEAudioEngine( aObserver, aConfig );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

CSGEAudioEngine* CSGEAudioEngine::NewL( MSGEAudioEngineObserver &aObserver, TSGEAudioConfig aConfig )
    {
    CSGEAudioEngine *self = CSGEAudioEngine::NewLC( aObserver, aConfig );
    CleanupStack::Pop(); // self;
    return self;
    }

void CSGEAudioEngine::ConstructL()
    {
    User::LeaveIfError( iFs.Connect() );
    
    const TInt KDefaultSuondSampleCount = 16;
    iAudioSamples.ReserveL( KDefaultSuondSampleCount );
    
    iFileLoader = CSGEAudioFileLoader::NewL( *this, iFs, iConfig );
    }

void CSGEAudioEngine::Cancel()
    {
    iFileLoader->Cancel();
    if ( iAudioStream )
        {
        iAudioStream->Cancel();
        }
    }

void CSGEAudioEngine::AddAudioFromBufferL( const TAny *aData, TUint aId, TUint aBufferDataTypeSize, 
                                           TUint aDataLength, TUint aFrequency, 
                                           TUint aChannels, TSGEAudioGroup aAudioGroup )
    {
    CSGEAudioSample *sample = CSGEAudioSample::NewLC( aId, aBufferDataTypeSize, aDataLength );
    sample->iFrequency = aFrequency;
    sample->iChannels = aChannels;
    sample->iAudioGroup = aAudioGroup;
    sample->SetBufferL( aData );

    User::LeaveIfError( AddAudioSample( sample ) );
    
    CleanupStack::Pop( sample );
    }

TInt CSGEAudioEngine::AddFileToLoad( TSGEAudioFile aFile )
    {
    return iFileLoader->LoadFile( aFile );
    }

TInt CSGEAudioEngine::AddFileToLoad( const TDesC& aSGEFilePath, TSGEAudioFile aFile )
    {
    return iFileLoader->LoadFile( aSGEFilePath, aFile );
    }

void CSGEAudioEngine::RemoveAudioSample( TUint aId )
    {
    for ( TInt i = 0; i < iSoundTracks.Count(); i++ )
        {
        if ( iSoundTracks[ i ]->AudioSample()->iId == aId )
            {
            iSoundTracks[ i ]->Stop();
            }
        }
    delete iAudioSamples.Find( aId );
    iAudioSamples.Remove( &aId );
    iFileLoader->RemoveFile( aId );
    }

void CSGEAudioEngine::RemoveAudioSampleAll()
    {
    iSoundTracks.ResetAndDestroy();
    iAudioSamples.ResetAndDestroy();
    iFileLoader->RemoveFileAll();
    }

void CSGEAudioEngine::StartLoadingFilesL()
    {
    iFileLoader->StartLoadingFilesL();
    }

void CSGEAudioEngine::CreateAudioStreamL()
    {
    DestroyAudioStream();

    if ( iConfig.iChannels == ESGEAudioChannelsMono )
        {
        iAudioMixer = CSGEAudioMixerMono::NewL( iConfig, iSoundTracks );
        }
    else if ( iConfig.iChannels == ESGEAudioChannelsStereo )
        {
        iAudioMixer = CSGEAudioMixerStereo::NewL( iConfig, iSoundTracks );
        }

    if ( iConfig.iSubsystem == ESGEAudioSubsystemMDA )
        {
        iAudioStream = CSGEAudioStreamMDA::NewL( *this, iConfig );
        }
    else if ( iConfig.iSubsystem == ESGEAudioSubsystemMMF )
        { // dev sound
        iAudioStream = CSGEAudioStreamMMF::NewL( *this, iConfig );
        }
    }

void CSGEAudioEngine::DestroyAudioStream()
    {
    delete iAudioMixer;
    iAudioMixer = NULL;
    
    delete iAudioStream;
    iAudioStream = NULL;
    }

void CSGEAudioEngine::CompactTracks( TInt aTracksCount )
    {
    for ( TInt i = 0; i < iSoundTracks.Count() && iSoundTracks.Count() > aTracksCount; i++ )
        {
        if ( !iSoundTracks[ i ]->AudioSample() )
            {
            iSoundTracks.Remove( i-- );
            }
        }
    iSoundTracks.Compress();
    }

TSGEAudioTrackState CSGEAudioEngine::TrackState( TUint aSoundTrack )
    {
    if ( aSoundTrack < 0 || aSoundTrack >= (TUint)iSoundTracks.Count() )
        {
        TSGEAudioTrackState state = { KErrNotFound, KErrNotFound, EFalse, EFalse, 0, 0, 0 };
        return state;
        }
    else
        {
        TSGEAudioTrackState state = 
            { 
            iSoundTracks[ aSoundTrack ]->Id(),
            ( iSoundTracks[ aSoundTrack ]->AudioSample() ? 
                (TInt) iSoundTracks[ aSoundTrack ]->AudioSample()->iId : 
                KErrNotFound ),
            iSoundTracks[ aSoundTrack ]->IsPaused(),
            iSoundTracks[ aSoundTrack ]->IsMuted(),
            iSoundTracks[ aSoundTrack ]->Repeat(),
            iSoundTracks[ aSoundTrack ]->Volume(),
            iSoundTracks[ aSoundTrack ]->CurrentDataLength()            
            };
        return state;
        }
    }

TInt CSGEAudioEngine::TrackCount() const
    {
    return iSoundTracks.Count();
    }

void CSGEAudioEngine::CreateEmptyTracksL( TInt aTrackCount )
    {
    if ( (TUint)aTrackCount > iConfig.iMaxTracksCount )
        {
        aTrackCount = iConfig.iMaxTracksCount;
        }

    for ( TInt i = 0; i < aTrackCount; i++ )
        {
        CSGEAudioSoundTrack *track = new CSGEAudioSoundTrack( *this, iSoundTracks.Count() );
        if ( track )
            {
            if ( iSoundTracks.Append( track ) != KErrNone )
                {
                delete track;
                }
            }
        }
    }

TInt CSGEAudioEngine::Play( TUint aAudioSampleId, TInt aRepeat )
    {
    CSGEAudioSoundTrack *track = GetFreeTrack();
    if ( track )
        {
        CSGEAudioSample *sample = iAudioSamples.Find( aAudioSampleId );
        if ( sample )
            {
            track->Play( sample, (TSGEAudioRepeat)aRepeat );

            TInt *vol = iVolumeDefault.Find( sample->iAudioGroup );
            if ( vol )
                {
                track->SetVolume( *vol );
                }
            
            //iAudioStream->UpdateNow(); // causes clipping on low freq samples
            return track->Id();
            }
        else
            {
            return KErrNotFound;
            }
        }
    else
        {
        return KErrNoMemory;
        }
    }

void CSGEAudioEngine::Stop( TUint aSoundTrack )
    {
    if ( aSoundTrack >= 0 && aSoundTrack < (TUint)iSoundTracks.Count() )
        {
        iSoundTracks[ aSoundTrack ]->Stop();
        }
    }

void CSGEAudioEngine::Pause( TUint aSoundTrack )
    {
    if ( aSoundTrack >= 0 && aSoundTrack < (TUint)iSoundTracks.Count() )
        {
        iSoundTracks[ aSoundTrack ]->SetPaused( ETrue );
        }
    }

void CSGEAudioEngine::Resume( TUint aSoundTrack )
    {
    if ( aSoundTrack >= 0 && aSoundTrack < (TUint)iSoundTracks.Count() )
        {
        iSoundTracks[ aSoundTrack ]->SetPaused( EFalse );
        }
    }

void CSGEAudioEngine::Mute( TUint aSoundTrack )
    {
    if ( aSoundTrack >= 0 && aSoundTrack < (TUint)iSoundTracks.Count() )
        {
        iSoundTracks[ aSoundTrack ]->SetMuted( ETrue );
        }
    }

void CSGEAudioEngine::Unmute( TUint aSoundTrack )
    {
    if ( aSoundTrack >= 0 && aSoundTrack < (TUint)iSoundTracks.Count() )
        {
        iSoundTracks[ aSoundTrack ]->SetMuted( EFalse );
        }
    }

void CSGEAudioEngine::StopAll()
    {
    for ( TInt i = 0; i < iSoundTracks.Count(); i++ )
        {
        iSoundTracks[ i ]->Stop();
        }
    }

void CSGEAudioEngine::PauseAll()
    {
    for ( TInt i = 0; i < iSoundTracks.Count(); i++ )
        {
        iSoundTracks[ i ]->SetPaused( ETrue );
        }
    }

void CSGEAudioEngine::ResumeAll()
    {
    for ( TInt i = 0; i < iSoundTracks.Count(); i++ )
        {
        iSoundTracks[ i ]->SetPaused( EFalse );
        }
    }

void CSGEAudioEngine::MuteAll()
    {
    for ( TInt i = 0; i < iSoundTracks.Count(); i++ )
        {
        iSoundTracks[ i ]->SetMuted( ETrue );
        }
    }

void CSGEAudioEngine::UnmuteAll()
    {
    for ( TInt i = 0; i < iSoundTracks.Count(); i++ )
        {
        iSoundTracks[ i ]->SetMuted( EFalse );
        }
    }

void CSGEAudioEngine::SetVolume( TUint aSoundTrack, TInt aVolume )
    {
    if ( aSoundTrack >= 0 && aSoundTrack < (TUint)iSoundTracks.Count() )
        {
        iSoundTracks[ aSoundTrack ]->SetVolume( aVolume );
        }
    }

void CSGEAudioEngine::SetVolumeGroup( TSGEAudioGroup aGroup, TInt aVolume )
    {
    if ( aGroup == ESGEAudioGroupAny )
        {
        SetVolumeAll( aVolume );
        }
    else
        {
        for ( TInt i = 0; i < iSoundTracks.Count(); i++ )
            {
            if ( iSoundTracks[ i ]->AudioSample() && iSoundTracks[ i ]->AudioSample()->iAudioGroup == aGroup )
                {
                iSoundTracks[ i ]->SetVolume( aVolume );
                }
            }
        }
    }

void CSGEAudioEngine::SetVolumeAll( TInt aVolume )
    {
    for ( TInt i = 0; i < iSoundTracks.Count(); i++ )
        {
        iSoundTracks[ i ]->SetVolume( aVolume );
        }
    }

void CSGEAudioEngine::SetVolumeDefault( TSGEAudioGroup aGroup, TInt aVolume )
    {
    if ( aGroup == ESGEAudioGroupAny )
        {
        iVolumeDefault.Insert( ESGEAudioGroupMusic, aVolume );
        iVolumeDefault.Insert( ESGEAudioGroupSound, aVolume );
        iVolumeDefault.Insert( ESGEAudioGroupAny, aVolume );
        }
    else
        {
        iVolumeDefault.Insert( aGroup, aVolume );
        }
    }

TInt CSGEAudioEngine::GetMaxVolume() const
    {
    if ( iAudioStream )
        {
        return iAudioStream->MaxVolume();
        }
    return 0;
    }

CSGEAudioSoundTrack* CSGEAudioEngine::GetFreeTrack()
    {
    for ( TInt i = 0; i < iSoundTracks.Count(); i++ )
        {
        if ( !iSoundTracks[ i ]->AudioSample() )
            {
            return iSoundTracks[ i ];
            }
        }
    
    CSGEAudioSoundTrack *track = new CSGEAudioSoundTrack( *this, iSoundTracks.Count() );
    if ( track )
        {
        if ( iSoundTracks.Append( track ) != KErrNone )
            {
            delete track;
            track = NULL;
            }
        }
    return track;
    }

TInt CSGEAudioEngine::AddAudioSample( CSGEAudioSample *aSoundSample )
    {
    ASSERT( aSoundSample );
    
    TUint *id = new TUint;
    if ( !id )
        {
        delete aSoundSample;
        return KErrNoMemory;
        }
    *id = aSoundSample->iId;
    
    if ( iAudioSamples.Find( *id ) )
        {
        delete aSoundSample;
        delete id;
        return KErrAlreadyExists;
        }
    
    TInt err = iAudioSamples.Insert( id, aSoundSample );
    
    if ( err != KErrNone )
        {
        delete aSoundSample;
        delete id;
        return err;
        }
    
    return KErrNone;
    }

void CSGEAudioEngine::AudioFileLoaded( CSGEAudioSample *aSoundSample, TUint aSampleId, TInt aError )
    {
    if ( aError == KErrNone && aSoundSample )
        {
        TInt err = AddAudioSample( aSoundSample );
        iObserver.AudioEngineFileLoaded( aSampleId, err );
        }
    else
        {
        delete aSoundSample;
        iObserver.AudioEngineFileLoaded( aSampleId, aError );
        }
    }

void CSGEAudioEngine::AudioFileListEmpty()
    {
    iObserver.AudioEngineAllFilesLoaded();
    }

void CSGEAudioEngine::AudioSoundTrackPlayStarted( TUint aTrackId )
    {
    iObserver.AudioEngineSoundTrackPlayStarted( aTrackId );
    }

void CSGEAudioEngine::AudioSoundTrackPlayFinished( TUint aTrackId )
    {
    iObserver.AudioEngineSoundTrackPlayFinished( aTrackId );
    }

void CSGEAudioEngine::AudioStreamCreated( TInt aError )
    {
    iObserver.AudioEngineStreamCreated( aError );
    }

void CSGEAudioEngine::AudioStreamMixData()
    {
    iAudioMixer->MixTracks();
    }

void CSGEAudioEngine::AudioStreamFillData( TAny *aBuffer, TInt aBufferSize )
    {
    iAudioMixer->FillBuffer( aBuffer, aBufferSize );
    }

void CSGEAudioEngine::AudioStreamError( TInt aError )
    {
    iObserver.AudioEngineStreamError( aError );
    }


