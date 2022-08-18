/*
 ============================================================================
 Name		: SGEAudioIPCCommon.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : IPC common declaration
 ============================================================================
 */

#ifndef SGEAUDIOIPCCOMMON_H
#define SGEAUDIOIPCCOMMON_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

_LIT( KSGEAudioIPCServerName, "SGEAudioIPCServer" );
_LIT( KSGEAudioIPCServerThreadName, "SGEAudioIPCServerThread" );
_LIT( KSGEAudioIPCServerSemaphoreName, "SGEAudioIPCServerSemaphore" );

_LIT( KSGEAudioIPCServerPanic, "SGEAudioIPCServerPanic" );
_LIT( KSGEAudioIPCClientPanic, "SGEAudioIPCClientPanic" );

const TInt KSGEAudioIPCServerVersionMajor = 0;
const TInt KSGEAudioIPCServerVersionMinor = 1;
const TInt KSGEAudioIPCServerVersionBuild = 0;

enum TSGEAudioIPCMessages
    {
    ESGEAudioIPCCloseServer,
    ESGEAudioIPCCancelPendingRequests,
    ESGEAudioIPCAddAudioFromBuffer,
    ESGEAudioIPCAddFileToLoad,
    ESGEAudioIPCAddFileToLoadFromSGEFile,
    ESGEAudioIPCRemoveAudioSample,
    ESGEAudioIPCRemoveAudioSampleAll,
    ESGEAudioIPCStartLoadingFiles,
    ESGEAudioIPCCreateAudioStream,
    ESGEAudioIPCDestroyAudioStream,
    ESGEAudioIPCCompactTracks,
    ESGEAudioIPCTrackState,
    ESGEAudioIPCTrackCount,
    ESGEAudioIPCPlay,
    ESGEAudioIPCStop,
    ESGEAudioIPCPause,
    ESGEAudioIPCResume,
    ESGEAudioIPCMute,
    ESGEAudioIPCUnmute,
    ESGEAudioIPCStopAll,
    ESGEAudioIPCPauseAll,
    ESGEAudioIPCResumeAll,
    ESGEAudioIPCMuteAll,
    ESGEAudioIPCUnmuteAll,
    ESGEAudioIPCSetVolume,
    ESGEAudioIPCSetVolumeAll,
    ESGEAudioIPCSubscribeForInitializationEvents,
    ESGEAudioIPCSubscribeForTrackEvents
    };

enum TSGEAudioIPCInitializationEvents
    {
    ESGEAudioIPCInitializationEventFileLoaded,
    ESGEAudioIPCInitializationEventAllFilesLoaded,
    ESGEAudioIPCInitializationEventStreamCreated,
    ESGEAudioIPCInitializationEventStreamError
    };

enum TSGEAudioIPCSoundTrackEvents
    {
    ESGEAudioIPCSoundTrackEventPlayStarted,
    ESGEAudioIPCSoundTrackEventPlayFinished
    };

NONSHARABLE_CLASS( TSGEAudioIPCAddAudioFromBufferParams )
    {
public:
    TUint iId;
    TUint16 iBufferDataTypeSize;
    TUint8 iChannels;
    TUint8 iAudioGroup;
    TUint iDataLength;
    TUint iFrequency;
    };

#endif // SGEAUDIOIPCCOMMON_H

