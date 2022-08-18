/*
 ============================================================================
 Name		: SGEAudioFileLoader.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEAudioFileLoader implementation
 ============================================================================
 */

#include "SGEAudioFileLoader.h"
#include "SGEAudioFileDecoderWAV.h"
#include "SGEAudioFileDecoderMP3.h"


const TInt KFileLoadingDelayTime = 1000;

CSGEAudioFileLoader::CSGEAudioFileLoader( MSGEAudioFileLoaderObserver &aObserver, RFs &aFs,
                                          TSGEAudioConfig &aConfig )
    : CTimer( EPriorityStandard ), iObserver( aObserver ), iFs( aFs ), iConfig( aConfig )
    {
    // No implementation required
    }

CSGEAudioFileLoader::~CSGEAudioFileLoader()
    {
    delete iCurrentAudioFileDecoder;
    iFileToLoadList.ResetAndDestroy();
    iFileData.Close();
    }

CSGEAudioFileLoader* CSGEAudioFileLoader::NewLC( MSGEAudioFileLoaderObserver &aObserver, RFs &aFs,
                                                 TSGEAudioConfig &aConfig )
    {
    CSGEAudioFileLoader *self = new (ELeave) CSGEAudioFileLoader( aObserver, aFs, aConfig );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

CSGEAudioFileLoader* CSGEAudioFileLoader::NewL( MSGEAudioFileLoaderObserver &aObserver, RFs &aFs,
                                                TSGEAudioConfig &aConfig )
    {
    CSGEAudioFileLoader *self = CSGEAudioFileLoader::NewLC( aObserver, aFs, aConfig );
    CleanupStack::Pop(); // self;
    return self;
    }

void CSGEAudioFileLoader::ConstructL()
    {
    CTimer::ConstructL();
    
    CActiveScheduler::Add( this );
    }

TInt CSGEAudioFileLoader::LoadFile( TSGEAudioFile aFile )
    {
    TSGEAudioFileInternal *file = new TSGEAudioFileInternal( aFile );
    if ( !file )
        {
        return KErrNoMemory;
        }
    TInt err = iFileToLoadList.Append( file );
    if ( err != KErrNone )
        {
        delete file;
        }
    return err;
    }

TInt CSGEAudioFileLoader::LoadFile( const TDesC& aSGEFilePath, TSGEAudioFile aFile )
    {
    TSGEAudioFileInternal *file = new TSGEAudioFileInternal( aFile, aSGEFilePath );
    if ( !file )
        {
        return KErrNoMemory;
        }
    TInt err = iFileToLoadList.Append( file );
    if ( err != KErrNone )
        {
        delete file;
        }
    return err;
    }

void CSGEAudioFileLoader::RemoveFile( TUint aAudioSampleId )
    {
    if ( iFileToLoadList[ 0 ]->iId == aAudioSampleId )
        {
        if ( iCurrentAudioFileDecoder )
            {
            delete iCurrentAudioFileDecoder;
            iCurrentAudioFileDecoder = NULL;
            }
        RemoveFileFromList( 0 );
        }
    else
        {
        for ( TInt i = 1; i < iFileToLoadList.Count(); i++ )
            {
            if ( iFileToLoadList[ i ]->iId == aAudioSampleId )
                {
                delete iFileToLoadList[ i ];
                iFileToLoadList.Remove( i );
                break;
                }
            }
        }
    }

void CSGEAudioFileLoader::RemoveFileAll()
    {
    delete iCurrentAudioFileDecoder;
    iCurrentAudioFileDecoder = NULL;
    iFileData.Close();
    iFileToLoadList.ResetAndDestroy();
    }

void CSGEAudioFileLoader::StartLoadingFilesL()
    {
    if ( !IsActive() && iFileToLoadList.Count() > 0 )
        {
        if ( iFileToLoadList[ 0 ]->iSGEFilePath.Length() )
            {
            TInt size;
            
            CSGEFile *sgeFile = CSGEFile::NewL();
            CleanupStack::PushL( sgeFile );
            sgeFile->LoadL( iFileToLoadList[ 0 ]->iSGEFilePath );
            
            RReadStream file = sgeFile->OpenFileL( iFileToLoadList[ 0 ]->iFileName, &size );
            file.PushL();
            iFileData.Close();
            iFileData.CreateL( size );
            file.ReadL( iFileData, size );
            CleanupStack::PopAndDestroy( &file );
            CleanupStack::PopAndDestroy( sgeFile );
            }
        else
            {
            TInt size;
            RFile file;
            User::LeaveIfError( file.Open( iFs, iFileToLoadList[ 0 ]->iFileName, EFileRead ) );
            CleanupClosePushL( file );
            file.Size( size );
            iFileData.Close();
            iFileData.CreateL( size );
            User::LeaveIfError( file.Read( iFileData, size ) );
            CleanupStack::PopAndDestroy( &file );
            }
        
        After( KFileLoadingDelayTime );        
        }
    else if ( iFileToLoadList.Count() == 0 )
        {
        iObserver.AudioFileListEmpty();
        }
    }

TBool CSGEAudioFileLoader::AllFilesLoaded()
    {
    return ( iFileToLoadList.Count() == 0 );
    }

void CSGEAudioFileLoader::RemoveFileFromList( TInt aIdx )
    {
    delete iFileToLoadList[ aIdx ];
    iFileToLoadList.Remove( aIdx );
    iFileData.Close();
    }

void CSGEAudioFileLoader::RunL()
    {
    if ( iStatus == KErrNone )
        {
        if ( iFileToLoadList.Count() > 0 )
            {
            switch ( iFileToLoadList[ 0 ]->iFileType )
                {
                case ESGEAudioFileTypeWAV:
                        delete iCurrentAudioFileDecoder;
                        iCurrentAudioFileDecoder = NULL;
                        iCurrentAudioFileDecoder = CSGEAudioFileDecoderWAV::NewL( *this, iFileData );
                        iCurrentAudioFileDecoder->StartDecodingL( *iFileToLoadList[ 0 ], iConfig );
                    break;
                case ESGEAudioFileTypeMP3:
                        delete iCurrentAudioFileDecoder;
                        iCurrentAudioFileDecoder = NULL;
                        iCurrentAudioFileDecoder = CSGEAudioFileDecoderMP3::NewL( *this, iFileData );
                        iCurrentAudioFileDecoder->StartDecodingL( *iFileToLoadList[ 0 ], iConfig );
                    break;
                default:
                    {
                    TUint id = iFileToLoadList[ 0 ]->iId;
                    RemoveFileFromList( 0 );
                    iObserver.AudioFileLoaded( NULL, id, KErrNotSupported );
                    }
                }
            }
        else
            {
            iObserver.AudioFileLoaded( NULL, 0, KErrNotFound );
            }
        }
    else
        {
        TUint id = ( iFileToLoadList.Count() > 0 ? iFileToLoadList[ 0 ]->iId : 0 );
        RemoveFileFromList( 0 );
        iObserver.AudioFileLoaded( NULL, id, iStatus.Int() );
        }
    }

TInt CSGEAudioFileLoader::RunError( TInt aError )
    {
    TUint id = ( iFileToLoadList.Count() > 0 ? iFileToLoadList[ 0 ]->iId : 0 );
    RemoveFileFromList( 0 );
    iObserver.AudioFileLoaded( NULL, id, aError );
    
    return KErrNone;
    }

void CSGEAudioFileLoader::AudioFileDecodingFinished( CSGEAudioSample* aSoundSample, TInt aError )
    {
    TUint id = ( iFileToLoadList.Count() > 0 ? iFileToLoadList[ 0 ]->iId : 0 );
    if ( aError )
        {
        iObserver.AudioFileLoaded( aSoundSample, id, aError );
        }
    else if ( !aSoundSample )
        {
        iObserver.AudioFileLoaded( NULL, id, KErrUnknown );
        }
    else
        {
        // changing bps to match config
        const TInt KBitsInByte = 8;
        if ( aSoundSample->iBps != iConfig.SizeOfBufferDataType() * KBitsInByte )
            {
            TRAP_IGNORE( aSoundSample->ChangeBpsL( iConfig.SizeOfBufferDataType() * KBitsInByte ) );
            }
        // resampling
        if ( iConfig.iResampleFiles )
            {
            if ( aSoundSample->iFrequency != (TUint) iConfig.iSampleRate )
                {
                TRAP_IGNORE( aSoundSample->ResampleL( iConfig.iSampleRate ) );
                }
            }
        // chanel change
        if ( aSoundSample->iChannels != iConfig.iChannels )
            {
            TRAP_IGNORE( aSoundSample->ChangeChannelsL( iConfig.iChannels ) );
            }
        
        iObserver.AudioFileLoaded( aSoundSample, id, KErrNone );
        
        RemoveFileFromList( 0 );
        
        TRAPD( err, StartLoadingFilesL() )
        if ( err )
            {
            iObserver.AudioFileLoaded( NULL, 
                                       ( iFileToLoadList.Count() > 0 ? iFileToLoadList[ 0 ]->iId : 0 ), err );
            }
        }
    
    delete iCurrentAudioFileDecoder;
    iCurrentAudioFileDecoder = NULL;
    }


CSGEAudioFileLoader::TSGEAudioFileInternal::TSGEAudioFileInternal( TSGEAudioFile aFile )
    : TSGEAudioFile( aFile )
    {
    }

CSGEAudioFileLoader::TSGEAudioFileInternal::TSGEAudioFileInternal( TSGEAudioFile aFile, const TDesC& aSGEFilePath )
    : TSGEAudioFile( aFile )
    {
    iSGEFilePath.Copy( aSGEFilePath );
    }

