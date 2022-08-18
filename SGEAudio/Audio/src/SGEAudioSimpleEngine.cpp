/*
 ============================================================================
 Name		: SGEAudioSimpleEngine.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEAudioSimpleEngine implementation
 ============================================================================
 */

#include "SGEAudioSimpleEngine.h"
#include "SGEAudioSample.h"
#include <SGEFile/SGEFile.h>
#include <ecom/ECom.h>


const TInt KDefaultCallbackParametersCount = 8;


CSGEAudioSimpleEngine::CSGEAudioSimpleEngine( MSGEAudioEngineObserver &aObserver, TSGEAudioConfig aConfig )
    : iObserver( aObserver ), iConfig( aConfig ), iCallbackParameters( KDefaultCallbackParametersCount )
    {
    }

CSGEAudioSimpleEngine::~CSGEAudioSimpleEngine()
    {
    ResetAudioSamples();
    iPlayers.ResetAndDestroy();
    iVolumeDefault.Close();
	
    iFs.Close();
    delete iAsyncCallback1;
    delete iAsyncCallback2;
    delete iAsyncCallback3;
    
    for ( TInt i = 0; i < iCallbackParameters.Count(); i++ )
        {
        struct STmp { CSGEAudioSimpleEngine *self; TInt idx; } *param = (STmp*) iCallbackParameters[ i ];
        delete param;
        }
    iCallbackParameters.Close();
    
    REComSession::FinalClose();
    }

CSGEAudioSimpleEngine* CSGEAudioSimpleEngine::NewLC( MSGEAudioEngineObserver &aObserver, TSGEAudioConfig aConfig )
    {
    CSGEAudioSimpleEngine *self = new (ELeave) CSGEAudioSimpleEngine( aObserver, aConfig );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

CSGEAudioSimpleEngine* CSGEAudioSimpleEngine::NewL( MSGEAudioEngineObserver &aObserver, TSGEAudioConfig aConfig )
    {
    CSGEAudioSimpleEngine *self = CSGEAudioSimpleEngine::NewLC( aObserver, aConfig );
    CleanupStack::Pop(); // self;
    return self;
    }

void CSGEAudioSimpleEngine::ConstructL()
    {
    User::LeaveIfError( iFs.Connect() );
    
    iAsyncCallback1 = new (ELeave) CAsyncCallBack( CActive::EPriorityStandard );
    iAsyncCallback2 = new (ELeave) CAsyncCallBack( CActive::EPriorityStandard );
    iAsyncCallback3 = new (ELeave) CAsyncCallBack( CActive::EPriorityStandard );
    }

void CSGEAudioSimpleEngine::AddAudioFromBufferL( const TAny *aData, TUint aId, TUint aBufferDataTypeSize, 
                                                 TUint aDataLength, TUint /*aFrequency*/, 
                                                 TUint /*aChannels*/, TSGEAudioGroup aAudioGroup )
    {
    if ( iAudioSamples.Find( aId ) )
        {
        User::Leave( KErrAlreadyExists );
        }
    
    //TInt size = aDataLength * aBufferDataTypeSize;

    CSGEAudioSample* sample = CSGEAudioSample::NewLC( aId, aBufferDataTypeSize, aDataLength );
    sample->SetBufferL( aData );
    sample->iAudioGroup = aAudioGroup;
    
    iAudioSamples.InsertL( aId, sample );

    CleanupStack::Pop( sample );
    }

TInt CSGEAudioSimpleEngine::AddFileToLoad( TSGEAudioFile aFile )
    {
    if ( iAudioSamples.Find( aFile.iId ) )
        {
        iObserver.AudioEngineFileLoaded( aFile.iId, KErrAlreadyExists );
        return KErrAlreadyExists;
        }            

    RFile file;
    TInt err = file.Open( iFs, aFile.iFileName, EFileRead | EFileShareAny );
    if ( err )
        {
        iObserver.AudioEngineFileLoaded( aFile.iId, err );
        return err;
        }
    TInt size;
    file.Size( size );
    
	const TInt KOneByte = 1;
	
    CSGEAudioSample* sample = CSGEAudioSample::NewLC( aFile.iId, KOneByte, size );
    sample->iAudioGroup = aFile.iAudioGroup;

    TPtr8 ptr( (TUint8*) sample->iData, size );
    err = file.Read( ptr, size );
    if ( err )
        {
        file.Close();
        CleanupStack::PopAndDestroy( sample );
        iObserver.AudioEngineFileLoaded( aFile.iId, err );
        return err;
        }
    
    file.Close();
    
    err = iAudioSamples.Insert( aFile.iId, sample );
    if ( err )
        {
        CleanupStack::PopAndDestroy( sample );
        }
	else
		{
		CleanupStack::Pop( sample );
		}
    
    iObserver.AudioEngineFileLoaded( aFile.iId, err );
    
    return err;
    }

TInt CSGEAudioSimpleEngine::AddFileToLoad( const TDesC& aSGEFilePath, TSGEAudioFile aFile )
    {
    TRAPD( err, AddFileToLoadL( aSGEFilePath, aFile ) );
    if ( err )
        {
        iObserver.AudioEngineFileLoaded( aFile.iId, err );
        }
    return err;
    }

void CSGEAudioSimpleEngine::AddFileToLoadL( const TDesC& aSGEFilePath, TSGEAudioFile aFile )
    {
    TInt size;

    if ( iAudioSamples.Find( aFile.iId ) )
        {
        User::Leave( KErrAlreadyExists );
        }
    
    CSGEFile *sgeFile = CSGEFile::NewL();
    CleanupStack::PushL( sgeFile );
    sgeFile->LoadL( aSGEFilePath );
    
    RReadStream file = sgeFile->OpenFileL( aFile.iFileName, &size );
    file.PushL();

    const TInt KOneByte = 1;
    
    CSGEAudioSample* sample = CSGEAudioSample::NewLC( aFile.iId, KOneByte, size );
    sample->iAudioGroup = aFile.iAudioGroup;

    TPtr8 ptr( (TUint8*) sample->iData, size );
    file.ReadL( ptr, size );

    iAudioSamples.InsertL( aFile.iId, sample );

    CleanupStack::Pop( sample );
    
    CleanupStack::PopAndDestroy( &file );
    CleanupStack::PopAndDestroy( sgeFile );
    
    iObserver.AudioEngineFileLoaded( aFile.iId, KErrNone );
    }

void CSGEAudioSimpleEngine::RemoveAudioSample( TUint aId )
    {
    for ( TInt i = 0; i < iPlayers.Count(); i++ )
        {
        if ( iPlayers[ i ]->AudioSample() && iPlayers[ i ]->AudioSample()->iId == aId )
            {
            delete iPlayers[ i ];
            iPlayers.Remove( i-- );
            }
        }

    delete (*iAudioSamples.Find( aId ));
    iAudioSamples.Remove( aId );
    }

void CSGEAudioSimpleEngine::RemoveAudioSampleAll()
    {
    ResetAudioSamples();
    }

void CSGEAudioSimpleEngine::StartLoadingFilesL()
    {
    TCallBack cb( StartLoadingFilesCallback, (TAny*) this );
    iAsyncCallback1->Cancel();
    iAsyncCallback1->Set( cb );
    iAsyncCallback1->CallBack();
    }

void CSGEAudioSimpleEngine::CreateAudioStreamL()
    {
    TCallBack cb( CreateStreamCallback, (TAny*) this );
    iAsyncCallback2->Cancel();
    iAsyncCallback2->Set( cb );
    iAsyncCallback2->CallBack();
    }

void CSGEAudioSimpleEngine::DestroyAudioStream()
    {
    StopAll();
    }

TSGEAudioTrackState CSGEAudioSimpleEngine::TrackState( TUint aSoundTrack )
    {
    if ( aSoundTrack < 0 || aSoundTrack >= (TUint)iPlayers.Count() )
        {
        TSGEAudioTrackState state = { KErrNotFound, KErrNotFound, EFalse, EFalse, 0, 0, 0 };
        return state;
        }
    else
        {
        TInt vol;
        iPlayers[ aSoundTrack ]->Player().GetVolume( vol );
        TTimeIntervalMicroSeconds pos;
        iPlayers[ aSoundTrack ]->Player().GetPosition( pos );
        
        TSGEAudioTrackState state = 
            { 
            aSoundTrack,
            ( iPlayers[ aSoundTrack ]->AudioSample() ? iPlayers[ aSoundTrack ]->AudioSample()->iId : KErrNotFound ),
            EFalse,
            EFalse,
            iPlayers[ aSoundTrack ]->Repeats(),
            vol,
            pos.Int64()
            };
        return state;
        }
    }

TInt CSGEAudioSimpleEngine::TrackCount() const
    {
    return iPlayers.Count();
    }

void CSGEAudioSimpleEngine::CreateEmptyTracksL( TInt aTrackCount )
	{
	if ( (TUint)aTrackCount > iConfig.iMaxTracksCount )
		{
		aTrackCount = iConfig.iMaxTracksCount;
		}

	for ( TInt i = 0; i < aTrackCount; i++ )
		{
		CSGEAudioSimplePlayer *player = CSGEAudioSimplePlayer::NewLC( *this );
		iPlayers.AppendL( player );
        CleanupStack::Pop( player );
		}
	}

TInt CSGEAudioSimpleEngine::Play( TUint aAudioSampleId, TInt aRepeat )
    {
    TInt idx = KErrNotFound;
    TRAPD( err, idx = PlayL( aAudioSampleId, aRepeat ) );
    if ( err )
        {
        return err;
        }
    return idx;
    }

TInt CSGEAudioSimpleEngine::PlayL( TUint aAudioSampleId, TInt aRepeat )
    {
    TInt idx = FindPlayer( aAudioSampleId );
    if ( idx != KErrNotFound )
        {
        // temporary fix because of S^3 error
        iPlayers[ idx ]->Close();
        iPlayers[ idx ]->PlayL( (CSGEAudioSample*) iPlayers[ idx ]->AudioSample(), aRepeat );
        //iPlayers[ idx ]->PlayAgain();
        
        struct STmp { CSGEAudioSimpleEngine *self; TInt idx; } *param = new (ELeave) STmp;
        param->self = this;
        param->idx = idx;
        CleanupStack::PushL( param );
        iCallbackParameters.AppendL( param );
        CleanupStack::Pop( param );
        TCallBack cb( PlayStartedCallback, (TAny*) param );
        if ( !iAsyncCallback3->IsActive() )
            {
            iAsyncCallback3->Set( cb );
            iAsyncCallback3->CallBack();
            }
        else if ( !iAsyncCallback2->IsActive() )
            {
            iAsyncCallback2->Set( cb );
            iAsyncCallback2->CallBack();
            }
        else
            {
            iAsyncCallback1->Cancel();
            iAsyncCallback1->Set( cb );
            iAsyncCallback1->CallBack();
            }
        }
    else
        {
        CSGEAudioSample** sample = iAudioSamples.Find( aAudioSampleId );
        if ( !sample )
			{
        	return KErrNotFound;
			}

        idx = GetPlayerL();
        if ( idx == KErrNotFound )
			{
        	return KErrOverflow;
			}

//        CSGEAudioSimplePlayer *player = CSGEAudioSimplePlayer::NewLC( *this );
//        iPlayers.AppendL( player );
//        CleanupStack::Pop( player );
//        idx = iPlayers.Count() - 1;

        iPlayers[ idx ]->PlayL( *sample, aRepeat );

        TInt *vol = iVolumeDefault.Find( (*sample)->iAudioGroup );
        if ( vol )
			{
        	iPlayers[ idx ]->SetVolumeAbsolute( *vol );
			}
        }
    
    return idx;
    }

void CSGEAudioSimpleEngine::Stop( TUint aSoundTrack )
    {
    if ( aSoundTrack >= 0 && aSoundTrack < (TUint)iPlayers.Count() )
        {
        iPlayers[ aSoundTrack ]->Stop();
        }
    }

void CSGEAudioSimpleEngine::Pause( TUint aSoundTrack )
    {
    if ( aSoundTrack >= 0 && aSoundTrack < (TUint)iPlayers.Count() && 
         iPlayers[ aSoundTrack ]->IsInitialized() )
        {
        iPlayers[ aSoundTrack ]->Player().Pause();
        }
    }

void CSGEAudioSimpleEngine::Resume( TUint aSoundTrack )
    {
    if ( aSoundTrack >= 0 && aSoundTrack < (TUint)iPlayers.Count() && 
         iPlayers[ aSoundTrack ]->IsInitialized() )
        {
        iPlayers[ aSoundTrack ]->Player().Play();
        }
    }

void CSGEAudioSimpleEngine::Mute( TUint aSoundTrack )
    {
    if ( aSoundTrack >= 0 && aSoundTrack < (TUint)iPlayers.Count() )
        {
        iPlayers[ aSoundTrack ]->Mute();
        }
    }

void CSGEAudioSimpleEngine::Unmute( TUint aSoundTrack )
    {
    if ( aSoundTrack >= 0 && aSoundTrack < (TUint)iPlayers.Count() )
        {
        iPlayers[ aSoundTrack ]->Unmute();
        }
    }

void CSGEAudioSimpleEngine::StopAll()
    {
    for ( TInt i = 0; i < iPlayers.Count(); i++ )
        {
        iPlayers[ i ]->Stop();
        }
    }

void CSGEAudioSimpleEngine::PauseAll()
    {
    for ( TInt i = 0; i < iPlayers.Count(); i++ )
        {
        if ( iPlayers[ i ]->IsInitialized() && iPlayers[ i ]->IsPlaying() )
            {
			// check player state?
            iPlayers[ i ]->Player().Pause();
            }
        }
    }

void CSGEAudioSimpleEngine::ResumeAll()
    {
    for ( TInt i = 0; i < iPlayers.Count(); i++ )
        {
        if ( iPlayers[ i ]->IsInitialized() && iPlayers[ i ]->IsPlaying() )
            {
			// check player state?
            iPlayers[ i ]->Player().Play();
            }
        }
    }

void CSGEAudioSimpleEngine::MuteAll()
    {
    for ( TInt i = 0; i < iPlayers.Count(); i++ )
        {
        iPlayers[ i ]->Mute();
        }
    }

void CSGEAudioSimpleEngine::UnmuteAll()
    {
    for ( TInt i = 0; i < iPlayers.Count(); i++ )
        {
        iPlayers[ i ]->Unmute();
        }
    }

void CSGEAudioSimpleEngine::SetVolume( TUint aSoundTrack, TInt aVolume )
    {
    if ( aSoundTrack >= 0 && aSoundTrack < (TUint)iPlayers.Count() )
        {
        iPlayers[ aSoundTrack ]->SetVolumeAbsolute( aVolume );
        }
    }

void CSGEAudioSimpleEngine::SetVolumeGroup( TSGEAudioGroup aGroup, TInt aVolume )
	{
	if ( aGroup == ESGEAudioGroupAny )
		{
		SetVolumeAll( aVolume );
		}
	else
		{
		for ( TInt i = 0; i < iPlayers.Count(); i++ )
			{
			if ( iPlayers[ i ]->AudioSample() && iPlayers[ i ]->AudioSample()->iAudioGroup == aGroup )
				{
				iPlayers[ i ]->SetVolumeAbsolute( aVolume );
				}
			}
		}
	}

void CSGEAudioSimpleEngine::SetVolumeAll( TInt aVolume )
    {
    for ( TInt i = 0; i < iPlayers.Count(); i++ )
        {
        iPlayers[ i ]->SetVolumeAbsolute( aVolume );
        }
    }

void CSGEAudioSimpleEngine::SetVolumeDefault( TSGEAudioGroup aGroup, TInt aVolume )
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

TInt CSGEAudioSimpleEngine::GetMaxVolume() const
    {
    if ( iPlayers.Count() > 0 )
        {
        return iPlayers[ 0 ]->MaxVolume();
        }
    return 0;
    }

void CSGEAudioSimpleEngine::ResetAudioSamples()
    {
    THashMapIter< TUint, CSGEAudioSample* > iter( iAudioSamples );
    iter.Reset();
    iter.NextKey();
    
    while ( iter.CurrentKey() != NULL )
        {
        for ( TInt i = 0; i < iPlayers.Count(); i++ )
            {
            if ( iPlayers[ i ]->AudioSample() && iPlayers[ i ]->AudioSample()->iId == *iter.CurrentKey() )
                {
                delete iPlayers[ i ];
                iPlayers.Remove( i-- );
                }
            }
        CSGEAudioSample **sample = iter.CurrentValue();
        if ( sample )
            {
            delete (*sample);
            }
        iter.NextKey();
        }
    iAudioSamples.Close();
    }

TInt CSGEAudioSimpleEngine::FindPlayer( TUint aAudioSample )
    {
    for ( TInt i = 0; i < iPlayers.Count(); i++ )
        {
        if ( !iPlayers[ i ]->IsPlaying() && iPlayers[ i ]->IsInitialized() &&
        	 iPlayers[ i ]->AudioSample() &&
             iPlayers[ i ]->AudioSample()->iId == aAudioSample )
            {
            return i;
            }
        }
    return KErrNotFound;
    }

TInt CSGEAudioSimpleEngine::GetPlayerL()
    {
    for ( TInt i = 0; i < iPlayers.Count(); i++ )
        {
        if ( !iPlayers[ i ]->IsPlaying() && !iPlayers[ i ]->IsInitialized() )
            {
            return i;
            }
        }
    
	if ( (TUint)iPlayers.Count() >= iConfig.iMaxTracksCount )
		{
	    for ( TInt i = 0; i < iPlayers.Count(); i++ )
	        {
	        if ( !iPlayers[ i ]->IsPlaying() && iPlayers[ i ]->IsInitialized() )
	            {
	        	iPlayers[ i ]->Close();
	            return i;
	            }
	        }
	    return KErrNotFound;
		}

    CSGEAudioSimplePlayer *player = CSGEAudioSimplePlayer::NewLC( *this );
    iPlayers.AppendL( player );
    CleanupStack::Pop( player );

    player->SetDefaultPercentVolume( iConfig.iMaxVolumePercent );
    
    return ( iPlayers.Count() - 1 );
    }

TInt CSGEAudioSimpleEngine::StartLoadingFilesCallback( TAny *aParam )
    {
    CSGEAudioSimpleEngine *self = (CSGEAudioSimpleEngine*) aParam;
    self->iObserver.AudioEngineAllFilesLoaded();
    return KErrNone;
    }

TInt CSGEAudioSimpleEngine::CreateStreamCallback( TAny *aParam )
    {
    CSGEAudioSimpleEngine *self = (CSGEAudioSimpleEngine*) aParam;
    self->iObserver.AudioEngineStreamCreated( KErrNone );
    return KErrNone;
    }

TInt CSGEAudioSimpleEngine::PlayStartedCallback( TAny *aParam )
    {
    struct STmp { CSGEAudioSimpleEngine *self; TInt idx; } *param = (STmp*) aParam;
    param->self->iObserver.AudioEngineSoundTrackPlayStarted( param->idx );
    TInt idx = param->self->iCallbackParameters.Find( param );
    if ( idx > KErrNotFound )
        {
        param->self->iCallbackParameters.Remove( idx );
        }
    delete param;
    return KErrNone;
    }

void CSGEAudioSimpleEngine::SimplePlayerInitComplete( CSGEAudioSimplePlayer *aPlayer, TInt aError )
    {
    if ( aError )
        {
        iObserver.AudioEngineStreamError( aError );
        }
    else
        {
        //aPlayer->SetVolumeAbsolute( aPlayer->MaxVolume() * iConfig.iMaxVolumePercent / 100 );
        iObserver.AudioEngineSoundTrackPlayStarted( iPlayers.Find( aPlayer ) );
        }
    }

void CSGEAudioSimpleEngine::SimplePlayerPlayComplete( CSGEAudioSimplePlayer *aPlayer, TInt aError )
    {
    if ( aError )
        {
        iObserver.AudioEngineStreamError( aError );
        }
    else
        {
        iObserver.AudioEngineSoundTrackPlayFinished( iPlayers.Find( aPlayer ) );
        }
    }

// end of file
