/*
 ============================================================================
 Name		: SGEAudioConfig.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : TSGEAudioConfig implementation
 ============================================================================
 */

#include "SGEAudioConfig.h"
#include <mda/common/audio.h> 
#include <mmf/server/sounddevice.h> 
#include <mmf/common/MmfFourCC.h> 

const TInt KSGEAudioIPCServerDefaultHeapMaxSize = 0x1000000; 

const TInt KSGEAudioDefaultMaxTracksCount = 8;


EXPORT_C TSGEAudioConfig::TSGEAudioConfig()
    : iBufferSize( KDefaultBufferSize ), iSampleRate( ESGEAudioSampleRate22050Hz ), 
      iChannels( ESGEAudioChannelsMono ), iBufferDataType( ESGEAudioBufferDataTypePCM16 ), 
      iSubsystem( ESGEAudioSubsystemMDA ), iMultithreeded( EFalse ), iResampleFiles( EFalse ),
      iMaxVolumePercent( KDefaultMaxVolumePercent ), iHeapMaxSize( KSGEAudioIPCServerDefaultHeapMaxSize ),
      iMaxTracksCount( KSGEAudioDefaultMaxTracksCount )
    {
    if ( iChannels == ESGEAudioChannelsStereo && iBufferSize % 2 != 0 )
        {
        iBufferSize++;
        }
    }

EXPORT_C TSGEAudioConfig::TSGEAudioConfig( TUint aBufferSize, TSGEAudioSampleRate aSampleRate, TSGEAudioChannels aChannels,
                                  TSGEAudioBufferDataType aBufferDataType, TSGEAudioSubsystem aSubsystem,
                                  TBool aMultithreeded, TBool aResampleFiles, TUint aMaxVolumePercent )
    : iBufferSize( aBufferSize ), iSampleRate( aSampleRate ), iChannels( aChannels ),
      iBufferDataType( aBufferDataType ), iSubsystem( aSubsystem ),
      iMultithreeded( aMultithreeded ), iResampleFiles( aResampleFiles ),
      iMaxVolumePercent( aMaxVolumePercent ), iHeapMaxSize( KSGEAudioIPCServerDefaultHeapMaxSize ),
      iMaxTracksCount( KSGEAudioDefaultMaxTracksCount )
    {
    if ( iChannels == ESGEAudioChannelsStereo && iBufferSize % 2 != 0 )
        {
        iBufferSize++;
        }
    }

EXPORT_C void TSGEAudioConfig::SetBufferSizeByLatency( TUint aMicroseconds )
    {
    const TInt KOneSecond = 1000000; 
    iBufferSize = iSampleRate * aMicroseconds / KOneSecond;
    }

EXPORT_C TUint TSGEAudioConfig::CalculateSoundLatency() const
    {
    const TInt KOneSecond = 1000000; 
    return iBufferSize * KOneSecond / iSampleRate;
    }

EXPORT_C TUint TSGEAudioConfig::BufferSizeInBytes() const
    {
    return iBufferSize * SizeOfBufferDataType();
    }

EXPORT_C TUint TSGEAudioConfig::SizeOfBufferDataType() const
    {
    switch ( iBufferDataType )
        {
        case ESGEAudioBufferDataTypePCM8: return 1;
        case ESGEAudioBufferDataTypePCMU8: return 1;
        case ESGEAudioBufferDataTypePCM16: return 2;
        case ESGEAudioBufferDataTypePCMU16: return 2;
        case ESGEAudioBufferDataTypePCM16B: return 2;
        case ESGEAudioBufferDataTypePCMU16B: return 2;
        case ESGEAudioBufferDataTypeIMAD: return 1;
        case ESGEAudioBufferDataTypeIMAS: return 1;
        case ESGEAudioBufferDataTypeALAW: return 1;
        case ESGEAudioBufferDataTypeMuLAW: return 1;
        case ESGEAudioBufferDataTypeGSM610: return 1;
        case ESGEAudioBufferDataTypeAMR: return 1;
        case ESGEAudioBufferDataTypeAAC: return 1;
        case ESGEAudioBufferDataTypeAWB: return 1;
        case ESGEAudioBufferDataTypeMP3: return 1;
        case ESGEAudioBufferDataTypeATRAC3: return 1;
        case ESGEAudioBufferDataTypeSBC: return 1;
        case ESGEAudioBufferDataTypeADTS: return 1;
        case ESGEAudioBufferDataTypeWMA: return 1;
        default: return 0;
        }
    }

EXPORT_C TUint TSGEAudioConfig::ChannelsForSubsystem() const
    {
    if ( iSubsystem == ESGEAudioSubsystemMMF )
        {
        switch ( iChannels )
            {
            case ESGEAudioChannelsMono: return EMMFMono;
            case ESGEAudioChannelsStereo: return EMMFStereo;
            default: return 0;
            }
        }
    else if ( iSubsystem == ESGEAudioSubsystemMDA )
        {
        switch ( iChannels )
            {
            case ESGEAudioChannelsMono: return TMdaAudioDataSettings::EChannelsMono;
            case ESGEAudioChannelsStereo: return TMdaAudioDataSettings::EChannelsStereo;
            default: return 0;
            }
        }
    else if ( iSubsystem == ESGEAudioSubsystemFake )
        {
        return 0;
        }
    else
        {
        return 0;
        }
    }

EXPORT_C TUint TSGEAudioConfig::SampleRateForSubsystem() const
    {
    if ( iSubsystem == ESGEAudioSubsystemMMF )
        {
        switch ( iSampleRate )
            {
            case ESGEAudioSampleRate8000Hz: return EMMFSampleRate8000Hz;
            case ESGEAudioSampleRate11025Hz: return EMMFSampleRate11025Hz;
            case ESGEAudioSampleRate12000Hz: return EMMFSampleRate12000Hz;
            case ESGEAudioSampleRate16000Hz: return EMMFSampleRate16000Hz;
            case ESGEAudioSampleRate22050Hz: return EMMFSampleRate22050Hz;
            case ESGEAudioSampleRate24000Hz: return EMMFSampleRate24000Hz;
            case ESGEAudioSampleRate32000Hz: return EMMFSampleRate32000Hz;
            case ESGEAudioSampleRate44100Hz: return EMMFSampleRate44100Hz;
            case ESGEAudioSampleRate48000Hz: return EMMFSampleRate48000Hz;
            case ESGEAudioSampleRate64000Hz: return EMMFSampleRate64000Hz;
            case ESGEAudioSampleRate88200Hz: return EMMFSampleRate88200Hz;
            case ESGEAudioSampleRate96000Hz: return EMMFSampleRate96000Hz;
            default: return 0;
            }
        }
    else if ( iSubsystem == ESGEAudioSubsystemMDA )
        {
        switch ( iSampleRate )
            {
            case ESGEAudioSampleRate8000Hz: return TMdaAudioDataSettings::ESampleRate8000Hz;
            case ESGEAudioSampleRate11025Hz: return TMdaAudioDataSettings::ESampleRate11025Hz;
            case ESGEAudioSampleRate12000Hz: return TMdaAudioDataSettings::ESampleRate12000Hz;
            case ESGEAudioSampleRate16000Hz: return TMdaAudioDataSettings::ESampleRate16000Hz;
            case ESGEAudioSampleRate22050Hz: return TMdaAudioDataSettings::ESampleRate22050Hz;
            case ESGEAudioSampleRate24000Hz: return TMdaAudioDataSettings::ESampleRate24000Hz;
            case ESGEAudioSampleRate32000Hz: return TMdaAudioDataSettings::ESampleRate32000Hz;
            case ESGEAudioSampleRate44100Hz: return TMdaAudioDataSettings::ESampleRate44100Hz;
            case ESGEAudioSampleRate48000Hz: return TMdaAudioDataSettings::ESampleRate48000Hz;
            case ESGEAudioSampleRate64000Hz: return TMdaAudioDataSettings::ESampleRate64000Hz;
            case ESGEAudioSampleRate88200Hz: return 0;  // not supported by this subsystem
            case ESGEAudioSampleRate96000Hz: return TMdaAudioDataSettings::ESampleRate96000Hz;
            default: return 0;
            }
        }
    else if ( iSubsystem == ESGEAudioSubsystemFake )
        {
        return 0;
        }
    else
        {
        return 0;
        }
    }

EXPORT_C TUint TSGEAudioConfig::BufferDataTypeForSubsystem() const
    {
    if ( iSubsystem == ESGEAudioSubsystemMMF )
        {
        switch ( iBufferDataType )
            {
            case ESGEAudioBufferDataTypePCM8: return EMMFSoundEncoding8BitPCM;
            case ESGEAudioBufferDataTypePCMU8: return 0;  // not supported by this subsystem 
            case ESGEAudioBufferDataTypePCM16: return EMMFSoundEncoding16BitPCM;
            case ESGEAudioBufferDataTypePCMU16: return 0;  // not supported by this subsystem 
            case ESGEAudioBufferDataTypePCM16B: return 0;  // not supported by this subsystem 
            case ESGEAudioBufferDataTypePCMU16B: return 0;  // not supported by this subsystem 
            case ESGEAudioBufferDataTypeIMAD: return 0;  // not supported by this subsystem 
            case ESGEAudioBufferDataTypeIMAS: return 0;  // not supported by this subsystem 
            case ESGEAudioBufferDataTypeALAW: return EMMFSoundEncoding8BitALaw;
            case ESGEAudioBufferDataTypeMuLAW: return EMMFSoundEncoding8BitMuLaw;
            case ESGEAudioBufferDataTypeGSM610: return 0;  // not supported by this subsystem 
            case ESGEAudioBufferDataTypeAMR: return 0;  // not supported by this subsystem 
            case ESGEAudioBufferDataTypeAAC: return 0;  // not supported by this subsystem 
            case ESGEAudioBufferDataTypeAWB: return 0;  // not supported by this subsystem 
            case ESGEAudioBufferDataTypeMP3: return 0;  // not supported by this subsystem 
            case ESGEAudioBufferDataTypeATRAC3: return 0;  // not supported by this subsystem 
            case ESGEAudioBufferDataTypeSBC: return 0;  // not supported by this subsystem 
            case ESGEAudioBufferDataTypeADTS: return 0;  // not supported by this subsystem 
            case ESGEAudioBufferDataTypeWMA: return 0;  // not supported by this subsystem 
            default: return 0;
            }
        }
    else if ( iSubsystem == ESGEAudioSubsystemMDA )
        {
        switch ( iBufferDataType )
            {
            case ESGEAudioBufferDataTypePCM8: return KMMFFourCCCodePCM8;
            case ESGEAudioBufferDataTypePCMU8: return KMMFFourCCCodePCMU8; 
            case ESGEAudioBufferDataTypePCM16: return KMMFFourCCCodePCM16;
            case ESGEAudioBufferDataTypePCMU16: return KMMFFourCCCodePCMU16; 
            case ESGEAudioBufferDataTypePCM16B: return KMMFFourCCCodePCM16B; 
            case ESGEAudioBufferDataTypePCMU16B: return KMMFFourCCCodePCMU16B; 
            case ESGEAudioBufferDataTypeIMAD: return KMMFFourCCCodeIMAD; 
            case ESGEAudioBufferDataTypeIMAS: return KMMFFourCCCodeIMAS; 
            case ESGEAudioBufferDataTypeALAW: return KMMFFourCCCodeALAW;
            case ESGEAudioBufferDataTypeMuLAW: return KMMFFourCCCodeMuLAW;
            case ESGEAudioBufferDataTypeGSM610: return KMMFFourCCCodeGSM610; 
            case ESGEAudioBufferDataTypeAMR: return KMMFFourCCCodeAMR; 
            case ESGEAudioBufferDataTypeAAC: return KMMFFourCCCodeAAC; 
            case ESGEAudioBufferDataTypeAWB: return KMMFFourCCCodeAWB; 
            case ESGEAudioBufferDataTypeMP3: return KMMFFourCCCodeMP3; 
            case ESGEAudioBufferDataTypeATRAC3: return KMMFFourCCCodeATRAC3; 
            case ESGEAudioBufferDataTypeSBC: return KMMFFourCCCodeSBC; 
            case ESGEAudioBufferDataTypeAMRW: return KMMFFourCCCodeAMRW;
            case ESGEAudioBufferDataTypeADTS: return KMMFFourCCCodeADTS; 
            case ESGEAudioBufferDataTypeWMA: return KMMFFourCCCodeWMA; 
            default: return KMMFFourCCCodeNULL;
            }
        }
    else if ( iSubsystem == ESGEAudioSubsystemFake )
        {
        return 0;
        }
    else
        {
        return 0;
        }
    }

EXPORT_C TUint TSGEAudioConfig::FourCC() const
    {
    switch ( iBufferDataType )
        {
        case ESGEAudioBufferDataTypePCM8: return KMMFFourCCCodePCM8;
        case ESGEAudioBufferDataTypePCMU8: return KMMFFourCCCodePCMU8; 
        case ESGEAudioBufferDataTypePCM16: return KMMFFourCCCodePCM16;
        case ESGEAudioBufferDataTypePCMU16: return KMMFFourCCCodePCMU16; 
        case ESGEAudioBufferDataTypePCM16B: return KMMFFourCCCodePCM16B; 
        case ESGEAudioBufferDataTypePCMU16B: return KMMFFourCCCodePCMU16B; 
        case ESGEAudioBufferDataTypeIMAD: return KMMFFourCCCodeIMAD; 
        case ESGEAudioBufferDataTypeIMAS: return KMMFFourCCCodeIMAS; 
        case ESGEAudioBufferDataTypeALAW: return KMMFFourCCCodeALAW;
        case ESGEAudioBufferDataTypeMuLAW: return KMMFFourCCCodeMuLAW;
        case ESGEAudioBufferDataTypeGSM610: return KMMFFourCCCodeGSM610; 
        case ESGEAudioBufferDataTypeAMR: return KMMFFourCCCodeAMR; 
        case ESGEAudioBufferDataTypeAAC: return KMMFFourCCCodeAAC; 
        case ESGEAudioBufferDataTypeAWB: return KMMFFourCCCodeAWB; 
        case ESGEAudioBufferDataTypeMP3: return KMMFFourCCCodeMP3; 
        case ESGEAudioBufferDataTypeATRAC3: return KMMFFourCCCodeATRAC3; 
        case ESGEAudioBufferDataTypeSBC: return KMMFFourCCCodeSBC; 
        case ESGEAudioBufferDataTypeAMRW: return KMMFFourCCCodeAMRW;
        case ESGEAudioBufferDataTypeADTS: return KMMFFourCCCodeADTS; 
        case ESGEAudioBufferDataTypeWMA: return KMMFFourCCCodeWMA; 
        default: return KMMFFourCCCodeNULL;
        }
    }


