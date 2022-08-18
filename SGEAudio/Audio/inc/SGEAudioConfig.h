/*
 ============================================================================
 Name		: SGEAudioConfig.h
 Author	  : Michal Strug
 Version	 : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : TSGEAudioConfig declaration
 ============================================================================
 */

#ifndef SGEAUDIOCONFIG_H
#define SGEAUDIOCONFIG_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>


// CLASS DECLARATION

enum TSGEAudioSampleRate
    {
    ESGEAudioSampleRate8000Hz = 8000,
    ESGEAudioSampleRate11025Hz = 11025,
    ESGEAudioSampleRate12000Hz = 12000,
    ESGEAudioSampleRate16000Hz = 16000,
    ESGEAudioSampleRate22050Hz = 22050,
    ESGEAudioSampleRate24000Hz = 24000,
    ESGEAudioSampleRate32000Hz = 32000,
    ESGEAudioSampleRate44100Hz = 44100,
    ESGEAudioSampleRate48000Hz = 48000,
    ESGEAudioSampleRate64000Hz = 64000,
    ESGEAudioSampleRate88200Hz = 88200,
    ESGEAudioSampleRate96000Hz = 96000
    };

enum TSGEAudioChannels
    {
    ESGEAudioChannelsMono = 1,
    ESGEAudioChannelsStereo = 2
    };

enum TSGEAudioBufferDataType
    {
    ESGEAudioBufferDataTypePCM8,
    ESGEAudioBufferDataTypePCMU8,
    ESGEAudioBufferDataTypePCM16,
    ESGEAudioBufferDataTypePCMU16,
    ESGEAudioBufferDataTypePCM16B,
    ESGEAudioBufferDataTypePCMU16B,
    ESGEAudioBufferDataTypeIMAD,
    ESGEAudioBufferDataTypeIMAS,
    ESGEAudioBufferDataTypeALAW,
    ESGEAudioBufferDataTypeMuLAW,
    ESGEAudioBufferDataTypeGSM610,
    ESGEAudioBufferDataTypeAMR,
    ESGEAudioBufferDataTypeAAC,
    ESGEAudioBufferDataTypeAWB,
    ESGEAudioBufferDataTypeMP3,
    ESGEAudioBufferDataTypeATRAC3,
    ESGEAudioBufferDataTypeSBC,
    ESGEAudioBufferDataTypeAMRW,
    ESGEAudioBufferDataTypeADTS,
    ESGEAudioBufferDataTypeWMA
    };

enum TSGEAudioSubsystem
    {
    ESGEAudioSubsystemSimple, // CMdaAudioPlayerUtility
    ESGEAudioSubsystemMDA, // CMdaAudioOutputStream
    ESGEAudioSubsystemMMF, // CMMFDevSound
    ESGEAudioSubsystemFake
    };

enum TSGEAudioGroup
    {
    ESGEAudioGroupAny,
    ESGEAudioGroupSound,
    ESGEAudioGroupMusic
    };

enum TSGEAudioFileType
    {
    ESGEAudioFileTypeWAV,
    ESGEAudioFileTypeMP3
    };

enum TSGEAudioRepeat
    {
    ESGEAudioRepeatInfinite = -1,
    ESGEAudioRepeatNone = 0
    };

/**
 *  TSGEAudioTrackState
 *  @todo: TSGEAudioTrackState add sending properties of new threed heap and stack
 *  @todo: TSGEAudioTrackState add setting rescale (shift) value
 */
class TSGEAudioTrackState
    {
public:
    // KErrNotFound if there is no sample
    TInt iTrackId;

    // KErrNotFound if there is no sample
    TInt iSampleId;
    
    TBool iPaused;

    TBool iMuted;
    
    TInt iRepeat;
    
    TInt iVolume;

    TInt iCurrentDataLength;
    };

/**
 *  TSGEAudioConfig
 * 
 */
class TSGEAudioConfig
    {

public:

    /*
     * same as:
     * TSGEAudioConfig( 512, ESGEAudioSampleRate22050Hz,
     *                       ESGEAudioChannelsMono, ESGEAudioBufferDataTypePCM16,
     *                       ESGEAudioSubsystemMDA, EFalse, EFalse,
     *                       KDefaultMaxVolumePercent );
     */
    IMPORT_C TSGEAudioConfig();
    
    IMPORT_C TSGEAudioConfig( TUint aBufferSize, TSGEAudioSampleRate aSampleRate, TSGEAudioChannels aChannels,
                              TSGEAudioBufferDataType aBufferDataType, TSGEAudioSubsystem aSubsystem,
                              TBool aMultithreeded, TBool aResampleFiles, 
                              TUint aMaxVolumePercent );
    
    /*
     * Calculates buffer size by desired latency for specified sample rate and buffer data type.
     */
    IMPORT_C void SetBufferSizeByLatency( TUint aMicroseconds );
    
    /*
     * Calculates sound latency depending of sample rate and buffer size.
     * @return microseconds
     */
    IMPORT_C TUint CalculateSoundLatency() const;
    
    IMPORT_C TUint BufferSizeInBytes() const;
    
    IMPORT_C TUint SizeOfBufferDataType() const;

    IMPORT_C TUint ChannelsForSubsystem() const;

    IMPORT_C TUint SampleRateForSubsystem() const;

    IMPORT_C TUint BufferDataTypeForSubsystem() const; 

    IMPORT_C TUint FourCC() const; 
    
public: // data
    
    // buffer size in unit TSGEAudioBufferDataType (for each channel)
    // buffer sizes smaller than 256 may cause problems
    // buffer sizes smaller than 512 caused problems on some Samsung phones
    TUint iBufferSize;
    
    TSGEAudioSampleRate iSampleRate;
    
    TSGEAudioChannels iChannels;
    
    TSGEAudioBufferDataType iBufferDataType;
    
    TSGEAudioSubsystem iSubsystem;
    
    // set to ETrue if want to use audio engine in dedicated threed, all communication
    // is done by Symbian IPC
    TBool iMultithreeded;
    
    TBool iResampleFiles;
    
    // percent of max volume to use
    TUint iMaxVolumePercent;

    // maximal size of the heap in mulbithreeding variant in bytes
    // default value is 0x1000000
    // must be larger than 0x100000 (min heap size)
    TUint iHeapMaxSize;

    // maximum tracks count
    // default value is 8
    TUint iMaxTracksCount;
    };


const TUint KDefaultMaxVolumePercent = 50;

const TUint KDefaultBufferSize = 512;


#endif // TSGEAUDIOCONFIG_H
