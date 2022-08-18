/*
 ============================================================================
 Name		: mixer.cpp
 Author	  : Michal Strug
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : Exe source file
 ============================================================================
 */

//  Include Files  

#include "mixer.h"
#include <e32base.h>
#include <e32std.h>
#include <e32cons.h>			// Console
#include <f32file.h> 
#include "SGEAudioConfig.h"
#include "SGEAudioMixerMono.h"
#include "SGEAudioSoundTrack.h"
#include "SGEAudioSample.h"

//  Constants

_LIT(KTextConsoleTitle, "Console");
_LIT(KTextFailed, " failed, leave code = %d");
_LIT(KTextPressAnyKey, " [press any key]\n");

//  Global Variables

LOCAL_D CConsoleBase* console; // write all messages to this

class CTestClass : public CBase, MSGEAudioSoundTrackObserver
    {
public:

    void RunTestL()
        {
        console->Write(_L("Test 1 start\n"));
        Test1L();
        console->Write(_L("Test 1 end\n"));

        console->Write(_L("Test 2 start\n"));
        Test2L();
        console->Write(_L("Test 2 end\n"));

        console->Write(_L("Test 3 start\n"));
        Test3L();
        console->Write(_L("Test 3 end\n"));

        console->Write(_L("Test 4 start\n"));
        Test4L();
        console->Write(_L("Test 4 end\n"));

        console->Write(_L("Test 5 start\n"));
        Test5L();
        console->Write(_L("Test 5 end\n"));
        }

    
    CTestClass()
        {
        }
    
    ~CTestClass()
        {
        User::Free( iStreamBuffer );
        delete iMonoMixer;
        iAudioSample.ResetAndDestroy();
        iSoundTracks.ResetAndDestroy();
        }

private:
    
    void Test1L()
        {
        iConfig.iBufferSize = 10;
        iConfig.iSampleRate = ESGEAudioSampleRate8000Hz;
        iConfig.iChannels = ESGEAudioChannelsMono;
        iConfig.iBufferDataType = ESGEAudioBufferDataTypePCM16;
        
        ASSERT( iConfig.BufferSizeInBytes() == 20 );

        // 30 bytes buffer (2 * 15)
        CSGEAudioSample *sample = CSGEAudioSample::NewLC( 1, iConfig.SizeOfBufferDataType(), 15 );
        const TInt16 KSample1[ 15 ] = { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 };
        Mem::Copy( sample->iData, KSample1, 2 * 15 );
        iAudioSample.AppendL( sample );
        CleanupStack::Pop( sample );

        // 14 bytes buffer (2 * 7)
        sample = CSGEAudioSample::NewLC( 2, iConfig.SizeOfBufferDataType(), 7 );
        const TInt16 KSample2[ 7 ] = { -5, -5, -5, -5, -5, -5, -5 };
        Mem::Copy( sample->iData, KSample2, 2 * 7 );
        iAudioSample.AppendL( sample );
        CleanupStack::Pop( sample );

        CSGEAudioSoundTrack *track = new (ELeave) CSGEAudioSoundTrack( *this, 1 );
        CleanupStack::PushL( track );
        iSoundTracks.AppendL( track );
        CleanupStack::Pop( track );

        track = new (ELeave) CSGEAudioSoundTrack( *this, 2 );
        CleanupStack::PushL( track );
        iSoundTracks.AppendL( track );
        CleanupStack::Pop( track );
        
        // play first sample on first track
        iSoundTracks[ 0 ]->Play( iAudioSample[ 0 ], ESGEAudioRepeatNone );

        // play second sample on second track
        iSoundTracks[ 1 ]->Play( iAudioSample[ 1 ], ESGEAudioRepeatNone );

        // create buffer
        iBufferSize =  iConfig.BufferSizeInBytes() * iConfig.iChannels;
        iStreamBuffer = User::AllocZL( iBufferSize );
        
        // create mixer
        iMonoMixer = CSGEAudioMixerMono::NewL( iConfig, iSoundTracks );

        // first mix
        iMonoMixer->MixTracks();
        iMonoMixer->FillBuffer( iStreamBuffer, iBufferSize );
        
        const TInt16 KBuffer1[ 10 ] = { 5, 5, 5, 5, 5, 5, 5, 10, 10, 10 };
        ASSERT( Mem::Compare( (TUint16*) iStreamBuffer, 10, (TUint16*) KBuffer1, 10 ) == 0 );

        // second mix
        iMonoMixer->MixTracks();
        iMonoMixer->FillBuffer( iStreamBuffer, iBufferSize );

        const TInt16 KBuffer2[ 10 ] = { 10, 10, 10, 10, 10, 0, 0, 0, 0, 0 };
        ASSERT( Mem::Compare( (TUint16*) iStreamBuffer, 10, (TUint16*) KBuffer2, 10 ) == 0 );
        
        
        // cleanup
        delete iMonoMixer;
        iMonoMixer = NULL;

        User::Free( iStreamBuffer );
        iStreamBuffer = NULL;

        iAudioSample.ResetAndDestroy();
        iSoundTracks.ResetAndDestroy();
        }
    
    void Test2L()
        {
        // 10 bytes buffer (10)
        CSGEAudioSample *sample = CSGEAudioSample::NewLC( 1, sizeof( TInt8 ), 10 );
        const TInt8 KSample1[ 10 ] = { 75, 10, -15, -60, -30, 1, 0, 8, 100, 120 };
        Mem::Copy( sample->iData, KSample1, 10 );

        sample->ChangeBpsL( 16 );

        const TInt16 KSample1out[ 10 ] = { 75*64, 10*64, -15*64, -60*64, -30*64, 64, 0, 8*64, 100*64, 120*64 };
        //ASSERT( Mem::Compare( (TUint16*) sample->iData, 10, (TUint16*) KSample1out, 10 ) == 0 );

        sample->ChangeBpsL( 8 );    // przeksztalcenie nie jednoznaczne poniewaz jest inna skala
        
        const TInt8 KSample1out2[ 10 ] = { 4, 0, 0, -3, -1, 0, 0, 0, 6, 7 };
//        ASSERT( Mem::Compare( (TUint8*) sample->iData, 10, (TUint8*) KSample1out2, 10 ) == 0 );
        
        CleanupStack::PopAndDestroy( sample );
        }

    
    void Test3L()
        {
        iConfig.iBufferSize = 10;
        iConfig.iSampleRate = ESGEAudioSampleRate44100Hz;
        iConfig.iChannels = ESGEAudioChannelsMono;
        iConfig.iBufferDataType = ESGEAudioBufferDataTypePCM16;
        
        ASSERT( iConfig.BufferSizeInBytes() == 20 );

        // 20 bytes buffer (2 * 10)
        CSGEAudioSample *sample = CSGEAudioSample::NewLC( 1, iConfig.SizeOfBufferDataType(), 10 );
        const TInt16 KSample1[ 10 ] = { 0x0000, 0x0000, 0x0000, 0x0000, 0x0001, 0xFFFF, 0x0000, 0x0001, 0xFFFE, 0x0002 };
        Mem::Copy( sample->iData, KSample1, 2 * 10 );
        iAudioSample.AppendL( sample );
        CleanupStack::Pop( sample );

        // 16 bytes buffer (2 * 8)
        sample = CSGEAudioSample::NewLC( 2, iConfig.SizeOfBufferDataType(), 8 );
        const TInt16 KSample2[ 8 ] = { 0x0002, 0x0007, 0x000B, 0x0014, 0x0016, 0x0010, 0x000D, 0x001D };
        Mem::Copy( sample->iData, KSample2, 2 * 8 );
        iAudioSample.AppendL( sample );
        CleanupStack::Pop( sample );

        // 16 bytes buffer (2 * 8)
        sample = CSGEAudioSample::NewLC( 3, iConfig.SizeOfBufferDataType(), 8 );
        const TInt16 KSample3[ 8 ] = { 0x00C2, 0x00E2, 0x010E, 0x013F, 0x015B, 0x016A, 0x0190, 0x01C3 };
        Mem::Copy( sample->iData, KSample3, 2 * 8 );
        iAudioSample.AppendL( sample );
        CleanupStack::Pop( sample );

        
        CSGEAudioSoundTrack *track = new (ELeave) CSGEAudioSoundTrack( *this, 1 );
        CleanupStack::PushL( track );
        iSoundTracks.AppendL( track );
        CleanupStack::Pop( track );

        track = new (ELeave) CSGEAudioSoundTrack( *this, 2 );
        CleanupStack::PushL( track );
        iSoundTracks.AppendL( track );
        CleanupStack::Pop( track );
        
        // play first sample on first track
        iSoundTracks[ 0 ]->Play( iAudioSample[ 0 ], ESGEAudioRepeatNone );

        // play first sample on second track
        iSoundTracks[ 1 ]->Play( iAudioSample[ 0 ], ESGEAudioRepeatNone );

        // create buffer
        iBufferSize =  iConfig.BufferSizeInBytes() * iConfig.iChannels;
        iStreamBuffer = User::AllocZL( iBufferSize );
        
        // create mixer
        iMonoMixer = CSGEAudioMixerMono::NewL( iConfig, iSoundTracks );

        // first mix
        iMonoMixer->MixTracks();
        iMonoMixer->FillBuffer( iStreamBuffer, iBufferSize );
        
        const TInt16 KBuffer1[ 10 ] = { 0x0000, 0x0000, 0x0000, 0x0000, 0x0002, 0xFFFE, 0x0000, 0x0002, 0xFFFC, 0x0004 };
        ASSERT( Mem::Compare( (TUint16*) iStreamBuffer, 10, (TUint16*) KBuffer1, 10 ) == 0 );

        
        // play second sample on first track
        iSoundTracks[ 0 ]->Play( iAudioSample[ 1 ], ESGEAudioRepeatNone );

        // play second sample on second track
        iSoundTracks[ 1 ]->Play( iAudioSample[ 1 ], ESGEAudioRepeatNone );
        
        // second mix
        iMonoMixer->MixTracks();
        iMonoMixer->FillBuffer( iStreamBuffer, iBufferSize );

        const TInt16 KBuffer2[ 8 ] = { 0x0004, 0x000E, 0x0016, 0x0028, 0x002C, 0x0020, 0x001A, 0x003A };
        ASSERT( Mem::Compare( (TUint16*) iStreamBuffer, 8, (TUint16*) KBuffer2, 8 ) == 0 );
        
        
        // play third sample on first track
        iSoundTracks[ 0 ]->Play( iAudioSample[ 2 ], ESGEAudioRepeatNone );

        // play third sample on second track
        iSoundTracks[ 1 ]->Play( iAudioSample[ 2 ], ESGEAudioRepeatNone );
        
        // second mix
        iMonoMixer->MixTracks();
        iMonoMixer->FillBuffer( iStreamBuffer, iBufferSize );

        const TInt16 KBuffer3[ 8 ] = { 0x0184, 0x01C4, 0x021C, 0x027E, 0x02B6, 0x02D4, 0x0320, 0x0386 };
        ASSERT( Mem::Compare( (TUint16*) iStreamBuffer, 8, (TUint16*) KBuffer3, 8 ) == 0 );

        
        // cleanup
        delete iMonoMixer;
        iMonoMixer = NULL;

        User::Free( iStreamBuffer );
        iStreamBuffer = NULL;

        iAudioSample.ResetAndDestroy();
        iSoundTracks.ResetAndDestroy();
        }
    
    void Test4L()
        {
        iConfig.iBufferSize = 10;
        iConfig.iSampleRate = ESGEAudioSampleRate44100Hz;
        iConfig.iChannels = ESGEAudioChannelsMono;
        iConfig.iBufferDataType = ESGEAudioBufferDataTypePCM16;
        
        ASSERT( iConfig.BufferSizeInBytes() == 20 );

        // 4 bytes buffer (2 * 2)
        CSGEAudioSample *sample = CSGEAudioSample::NewLC( 1, iConfig.SizeOfBufferDataType(), 2 );
        const TInt16 KSample1[ 2 ] = { 0x7D00, 0x7D00 };
        Mem::Copy( sample->iData, KSample1, 2 * 2 );
        iAudioSample.AppendL( sample );
        CleanupStack::Pop( sample );

        // 4 bytes buffer (2 * 2)
        sample = CSGEAudioSample::NewLC( 2, iConfig.SizeOfBufferDataType(), 2 );
        const TInt16 KSample2[ 2 ] = { 0x8300, 0x8300 };
        Mem::Copy( sample->iData, KSample2, 2 * 2 );
        iAudioSample.AppendL( sample );
        CleanupStack::Pop( sample );

        
        CSGEAudioSoundTrack *track = new (ELeave) CSGEAudioSoundTrack( *this, 1 );
        CleanupStack::PushL( track );
        iSoundTracks.AppendL( track );
        CleanupStack::Pop( track );

        track = new (ELeave) CSGEAudioSoundTrack( *this, 2 );
        CleanupStack::PushL( track );
        iSoundTracks.AppendL( track );
        CleanupStack::Pop( track );
        
        // play first sample on first track
        iSoundTracks[ 0 ]->Play( iAudioSample[ 0 ], ESGEAudioRepeatNone );

        // play first sample on second track
        iSoundTracks[ 1 ]->Play( iAudioSample[ 0 ], ESGEAudioRepeatNone );

        // create buffer
        iBufferSize =  iConfig.BufferSizeInBytes() * iConfig.iChannels;
        iStreamBuffer = User::AllocZL( iBufferSize );
        
        // create mixer
        iMonoMixer = CSGEAudioMixerMono::NewL( iConfig, iSoundTracks );

        // first mix
        iMonoMixer->MixTracks();
        iMonoMixer->FillBuffer( iStreamBuffer, iBufferSize );
        
        const TInt16 KBuffer1[ 2 ] = { 0x7FFF, 0x7FFF };
        ASSERT( Mem::Compare( (TUint16*) iStreamBuffer, 2, (TUint16*) KBuffer1, 2 ) == 0 );

        
        // play second sample on first track
        iSoundTracks[ 0 ]->Play( iAudioSample[ 1 ], ESGEAudioRepeatNone );

        // play second sample on second track
        iSoundTracks[ 1 ]->Play( iAudioSample[ 1 ], ESGEAudioRepeatNone );
        
        // second mix
        iMonoMixer->MixTracks();
        iMonoMixer->FillBuffer( iStreamBuffer, iBufferSize );

        const TInt16 KBuffer2[ 2 ] = { 0x8000, 0x8000 };
        ASSERT( Mem::Compare( (TUint16*) iStreamBuffer, 2, (TUint16*) KBuffer2, 2 ) == 0 );
  
        
        // cleanup
        delete iMonoMixer;
        iMonoMixer = NULL;

        User::Free( iStreamBuffer );
        iStreamBuffer = NULL;

        iAudioSample.ResetAndDestroy();
        iSoundTracks.ResetAndDestroy();
        }
    

    void Test5L()
        {
        const TInt KSndFileSize = 35480; // bytes
        
        iConfig.iBufferSize = KSndFileSize / 2;
        iConfig.iSampleRate = ESGEAudioSampleRate44100Hz;
        iConfig.iChannels = ESGEAudioChannelsMono;
        iConfig.iBufferDataType = ESGEAudioBufferDataTypePCM16;
        
        ASSERT( iConfig.BufferSizeInBytes() == KSndFileSize );

        // KSndFileSize bytes buffer
        CSGEAudioSample *sample = CSGEAudioSample::NewLC( 1, iConfig.SizeOfBufferDataType(), KSndFileSize / 2 );
        // reading file
        RFs fs;
        fs.Connect();
        RFile file;
        ASSERT( file.Open( fs, _L("c:\\snd.raw"), EFileRead ) == KErrNone );
        TInt size = 0;
        file.Size( size );
        ASSERT( size == KSndFileSize );
        TPtr8 ptr( (TUint8*)sample->iData, KSndFileSize );
        ASSERT( file.Read( ptr ) == KErrNone );
        file.Close();
        iAudioSample.AppendL( sample );
        CleanupStack::Pop( sample );

        
        CSGEAudioSoundTrack *track = new (ELeave) CSGEAudioSoundTrack( *this, 1 );
        CleanupStack::PushL( track );
        iSoundTracks.AppendL( track );
        CleanupStack::Pop( track );

        track = new (ELeave) CSGEAudioSoundTrack( *this, 2 );
        CleanupStack::PushL( track );
        iSoundTracks.AppendL( track );
        CleanupStack::Pop( track );
        
        // play first sample on first track
        iSoundTracks[ 0 ]->Play( iAudioSample[ 0 ], ESGEAudioRepeatNone );

        // play first sample on second track
        iSoundTracks[ 1 ]->Play( iAudioSample[ 0 ], ESGEAudioRepeatNone );

        // create buffer
        iBufferSize =  iConfig.BufferSizeInBytes() * iConfig.iChannels;
        iStreamBuffer = User::AllocZL( iBufferSize );
        
        // create mixer
        iMonoMixer = CSGEAudioMixerMono::NewL( iConfig, iSoundTracks );

        // mix
        iMonoMixer->MixTracks();
        iMonoMixer->FillBuffer( iStreamBuffer, iBufferSize );
        
        //const TInt16 KBuffer1[ 2 ] = { 0x7FFF, 0x7FFF };
        //ASSERT( Mem::Compare( (TUint16*) iStreamBuffer, 2, (TUint16*) KBuffer1, 2 ) == 0 );
        
        ASSERT( file.Replace( fs, _L("c:\\snd.out"), EFileWrite ) == KErrNone );
        TPtrC8 ptrOut( (TUint8*)iStreamBuffer, iBufferSize );
        ASSERT( file.Write( ptrOut ) == KErrNone );
        file.Close();
        
        file.Replace( fs, _L("c:\\snd.out.wav"), EFileWrite );
        const TUint8 wavHdr[ 0x2C ] = { 0x52, 0x49, 0x46, 0x46, 0xBC, 0x8A, 0x00, 0x00, 0x57, 0x41, 0x56, 0x45, 0x66, 0x6D, 0x74, 0x20, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x44, 0xAC, 0x00, 0x00, 0x88, 0x58, 0x01, 0x00, 0x02, 0x00, 0x10, 0x00, 0x64, 0x61, 0x74, 0x61, 0x98, 0x8A, 0x00, 0x00 };
        TPtrC8 ptrHdrOut( wavHdr, 0x2C );
        file.Write( ptrHdrOut );
        file.Write( ptrOut );
        file.Close();
        
        
        // cleanup
        delete iMonoMixer;
        iMonoMixer = NULL;

        User::Free( iStreamBuffer );
        iStreamBuffer = NULL;

        iAudioSample.ResetAndDestroy();
        iSoundTracks.ResetAndDestroy();
        
        fs.Close();
        }
    
private: // from MSGEAudioSoundTrackObserver
    
    void AudioSoundTrackPlayStarted( TUint aTrackId )
        {
        console->Write(_L("Test log: AudioSoundTrackPlayStarted\n"));
        }

    void AudioSoundTrackPlayFinished( TUint aTrackId )
        {
        console->Write(_L("Test log: AudioSoundTrackPlayFinished\n"));
        }
    
private: // data
    
    TSGEAudioConfig iConfig;
    
    CSGEAudioMixerMono *iMonoMixer;
    
    RPointerArray< CSGEAudioSample > iAudioSample;
    
    RPointerArray< CSGEAudioSoundTrack > iSoundTracks;
    
    TInt iBufferSize;
    
    TAny *iStreamBuffer;
    
    TInt iTestPass;
    
    };


//  Local Functions

LOCAL_C void MainL()
    {
    console->Write(_L("Test log:\n"));
    
    CTestClass *test = new (ELeave) CTestClass();
    test->RunTestL();
    
    delete test;

    console->Write(_L("\nAll ok\n"));
    }

LOCAL_C void DoStartL()
    {
    // Create active scheduler (to run active objects)
    CActiveScheduler* scheduler = new (ELeave) CActiveScheduler();
    CleanupStack::PushL(scheduler);
    CActiveScheduler::Install(scheduler);

    MainL();

    // Delete active scheduler
    CleanupStack::PopAndDestroy(scheduler);
    }

//  Global Functions

GLDEF_C TInt E32Main()
    {
    // Create cleanup stack
    __UHEAP_MARK;
    CTrapCleanup* cleanup = CTrapCleanup::New();

    // Create output console
    TRAPD(createError, console = Console::NewL(KTextConsoleTitle, TSize(
            KConsFullScreen, KConsFullScreen)));
    if (createError)
        {
        delete cleanup;
        return createError;
        }

    // Run application code inside TRAP harness, wait keypress when terminated
    TRAPD(mainError, DoStartL());
    if (mainError)
        console->Printf(KTextFailed, mainError);
    console->Printf(KTextPressAnyKey);
    console->Getch();

    delete console;
    delete cleanup;
    __UHEAP_MARKEND;
    return KErrNone;
    }

