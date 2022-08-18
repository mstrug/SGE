/*
 ============================================================================
 Name		: SGEAudioIPCClient.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : RSGEAudioIPCClient implementation
 ============================================================================
 */

#include "SGEAudioIPCClient.h"
#include "SGEAudioIPCCommon.h"
#include "SGEAudioIPCServer.h"

RSGEAudioIPCClient::RSGEAudioIPCClient()
    : iPckgInitializationEvent( iInitializationEvent ), iPckgSoundTrackEvent( iSoundTrackEvent ),
      iPckgSampleId( iSampleId ) 
    {
    }

TInt RSGEAudioIPCClient::Connect()
    { 
    const TVersion KSGEAudioIPCServerVersion( KSGEAudioIPCServerVersionMajor, 
                                              KSGEAudioIPCServerVersionMinor, 
                                              KSGEAudioIPCServerVersionBuild );
    iInitializationEvent = EInitializationEventUninitialised;
    iSoundTrackEvent = ESoundTrackEventUninitialised;
    iSampleId = KErrNotFound;
    return CreateSession( KSGEAudioIPCServerName(), KSGEAudioIPCServerVersion );
    }

TInt RSGEAudioIPCClient::CloseServer()
    {    
    return SendReceive( ESGEAudioIPCCloseServer );
    }

TInt RSGEAudioIPCClient::CancelPendingRequests()
    {
    return SendReceive( ESGEAudioIPCCancelPendingRequests );
    }

TInt RSGEAudioIPCClient::AddAudioFromBuffer( const TAny *aData, TUint aId, TUint aBufferDataTypeSize, 
                                             TUint aDataLength, TUint aFrequency, 
                                             TUint aChannels, TSGEAudioGroup aAudioGroup )
    {
    TSGEAudioIPCAddAudioFromBufferParams params = 
        {
        aId,
        aBufferDataTypeSize,
        aChannels,
        aAudioGroup,
        aDataLength,
        aFrequency
        };
    TPckg< TSGEAudioIPCAddAudioFromBufferParams > pckgParams( params );
    return SendReceive( ESGEAudioIPCAddAudioFromBuffer, TIpcArgs( aData, &pckgParams ) );
    }

TInt RSGEAudioIPCClient::AddFileToLoad( TSGEAudioFile aAudioFile )
    {
    TPckg< TSGEAudioFile > file( aAudioFile );
    return SendReceive( ESGEAudioIPCAddFileToLoad, TIpcArgs( &file ) );
    }

TInt RSGEAudioIPCClient::AddFileToLoadFromSGEFile( const TDesC& aSGEFilePath, TSGEAudioFile aAudioFile )
    {
    TPckg< TSGEAudioFile > file( aAudioFile );
    return SendReceive( ESGEAudioIPCAddFileToLoadFromSGEFile, TIpcArgs( &aSGEFilePath, &file ) );
    }

TInt RSGEAudioIPCClient::RemoveAudioSample( TUint aSampleId )
    {
    return SendReceive( ESGEAudioIPCRemoveAudioSample, TIpcArgs( aSampleId ) );
    }

TInt RSGEAudioIPCClient::RemoveAudioSampleAll()
    {
    return SendReceive( ESGEAudioIPCRemoveAudioSampleAll );
    }

void RSGEAudioIPCClient::StartLoadingFiles( TRequestStatus &aStatus )
    {
    SendReceive( ESGEAudioIPCStartLoadingFiles, TIpcArgs( &iPckgInitializationEvent, &iPckgSampleId ), aStatus );
    }

void RSGEAudioIPCClient::CreateAudioStream( TRequestStatus &aStatus )
    {
    SendReceive( ESGEAudioIPCCreateAudioStream, TIpcArgs( &iPckgInitializationEvent, &iPckgSampleId ), aStatus );
    }

TInt RSGEAudioIPCClient::DestroyAudioStream()
    {
    return SendReceive( ESGEAudioIPCDestroyAudioStream );
    }

TInt RSGEAudioIPCClient::CompactTracks()
    {
    return SendReceive( ESGEAudioIPCCompactTracks );
    }

TInt RSGEAudioIPCClient::TrackState( TUint aTrackId, TSGEAudioTrackState &aState )
    {
    TPckg< TSGEAudioTrackState > state( aState );
    return  SendReceive( ESGEAudioIPCTrackState, TIpcArgs( aTrackId, &state ) );
    }

TInt RSGEAudioIPCClient::TrackCount()
    {
    return SendReceive( ESGEAudioIPCTrackCount );
    }

TInt RSGEAudioIPCClient::Play( TUint aSampleId, TInt aRepeat )
    {
    return SendReceive( ESGEAudioIPCPlay, TIpcArgs( aSampleId, aRepeat ) );
    }

TInt RSGEAudioIPCClient::Stop( TUint aTrackId )
    {
    return SendReceive( ESGEAudioIPCStop, TIpcArgs( aTrackId ) );
    }

TInt RSGEAudioIPCClient::Pause( TUint aTrackId )
    {
    return SendReceive( ESGEAudioIPCPause, TIpcArgs( aTrackId ) );
    }

TInt RSGEAudioIPCClient::Resume( TUint aTrackId )
    {
    return SendReceive( ESGEAudioIPCResume, TIpcArgs( aTrackId ) );
    }

TInt RSGEAudioIPCClient::Mute( TUint aTrackId )
    {
    return SendReceive( ESGEAudioIPCMute, TIpcArgs( aTrackId ) );
    }

TInt RSGEAudioIPCClient::Unmute( TUint aTrackId )
    {
    return SendReceive( ESGEAudioIPCUnmute, TIpcArgs( aTrackId ) );
    }

TInt RSGEAudioIPCClient::StopAll()
    {
    return SendReceive( ESGEAudioIPCStopAll );
    }

TInt RSGEAudioIPCClient::PauseAll()
    {
    return SendReceive( ESGEAudioIPCPauseAll );
    }

TInt RSGEAudioIPCClient::ResumeAll()
    {
    return SendReceive( ESGEAudioIPCResumeAll );
    }

TInt RSGEAudioIPCClient::MuteAll()
    {
    return SendReceive( ESGEAudioIPCMuteAll );
    }

TInt RSGEAudioIPCClient::UnmuteAll()
    {
    return SendReceive( ESGEAudioIPCUnmuteAll );
    }

TInt RSGEAudioIPCClient::RSGEAudioIPCClient::SetVolume( TUint aSampleId, TInt aVolume )
    {
    return SendReceive( ESGEAudioIPCSetVolume, TIpcArgs( aSampleId, aVolume ) );
    }

TInt RSGEAudioIPCClient::SetVolumeAll( TInt aVolume )
    {
    return SendReceive( ESGEAudioIPCSetVolume, TIpcArgs( aVolume ) );
    }

void RSGEAudioIPCClient::SubscribeForInitializationEvents( TRequestStatus &aStatus )
    {
    SendReceive( ESGEAudioIPCSubscribeForInitializationEvents, 
                 TIpcArgs( &iPckgInitializationEvent, &iPckgSampleId ), aStatus );
    }

void RSGEAudioIPCClient::SubscribeForTrackEvents( TRequestStatus &aStatus )
    {
    SendReceive( ESGEAudioIPCSubscribeForTrackEvents, TIpcArgs( &iPckgSoundTrackEvent ), aStatus );
    }

RSGEAudioIPCClient::TInitializationEventType RSGEAudioIPCClient::InitializationEventType()
    {
    return (RSGEAudioIPCClient::TInitializationEventType) iInitializationEvent;
    }

RSGEAudioIPCClient::TSoundTrackEventType RSGEAudioIPCClient::SoundTrackEventType()
    {
    return (RSGEAudioIPCClient::TSoundTrackEventType) iSoundTrackEvent;
    }

TInt RSGEAudioIPCClient::SampleId()
    {
    return iSampleId;
    }


