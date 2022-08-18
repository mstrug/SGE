/*
 ============================================================================
 Name		: SGEAudioProvider.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEAudioProvider implementation
 ============================================================================
 */

#include "SGEAudioProvider.h"
#include "SGEAudioProviderInternal.h"
#include <centralrepository.h>
#include <profileenginesdkcrkeys.h>


CSGEAudioProvider::CSGEAudioProvider()
    {
    // No implementation required
    }

EXPORT_C CSGEAudioProvider::~CSGEAudioProvider()
    {
    delete iProvider;
    }

EXPORT_C CSGEAudioProvider* CSGEAudioProvider::NewLC( MSGEAudioProviderObserver *aObserver, 
                                                      TSGEAudioConfig aConfig )
    {
    CSGEAudioProvider *self = new (ELeave) CSGEAudioProvider();
    CleanupStack::PushL( self );
    self->ConstructL( aObserver, aConfig );
    return self;
    }

EXPORT_C CSGEAudioProvider* CSGEAudioProvider::NewL( MSGEAudioProviderObserver *aObserver, 
                                                     TSGEAudioConfig aConfig )
    {
    CSGEAudioProvider *self = CSGEAudioProvider::NewLC( aObserver, aConfig );
    CleanupStack::Pop(); // self;
    return self;
    }

void CSGEAudioProvider::ConstructL( MSGEAudioProviderObserver *aObserver, TSGEAudioConfig aConfig )
    {
    if ( ( aConfig.SizeOfBufferDataType() != 2 && aConfig.SizeOfBufferDataType() != 1 ) ||
         aConfig.iResampleFiles )
        {
        User::Leave( KErrNotSupported );
        }
    
    iProvider = CSGEAudioProviderInternal::NewL( aObserver, aConfig );
    }

EXPORT_C void CSGEAudioProvider::SetObserver( MSGEAudioProviderObserver *aObserver )
    {
    iProvider->iObserver = aObserver;
    }

EXPORT_C void CSGEAudioProvider::LoadFileL( TSGEAudioFile aFile )
    {
    if ( iProvider->iEngine )
        {
        User::LeaveIfError( iProvider->iEngine->AddFileToLoad( aFile ) );
        iProvider->iEngine->StartLoadingFilesL();
        }
    else if ( iProvider->iSimpleEngine )
        {
        User::LeaveIfError( iProvider->iSimpleEngine->AddFileToLoad( aFile ) );
        iProvider->iSimpleEngine->StartLoadingFilesL();
        }
    else
        {
        User::LeaveIfError( iProvider->iClient.AddFileToLoad( aFile ) );
        
        if ( !iProvider->iInitializationAO->IsActive() )
            {
            iProvider->iClient.StartLoadingFiles( iProvider->iInitializationAO->iStatus );   /* async */
            iProvider->iInitializationAO->SetActive();
            }
        }
    }

EXPORT_C void CSGEAudioProvider::LoadFileL( const TDesC& aSGEFilePath, TSGEAudioFile aFile )
    {
    if ( iProvider->iEngine )
        {
        User::LeaveIfError( iProvider->iEngine->AddFileToLoad( aSGEFilePath, aFile ) );
        iProvider->iEngine->StartLoadingFilesL();
        }
    else if ( iProvider->iSimpleEngine )
        {
        User::LeaveIfError( iProvider->iSimpleEngine->AddFileToLoad( aSGEFilePath, aFile ) );
        iProvider->iSimpleEngine->StartLoadingFilesL();
        }
    else
        {
        User::LeaveIfError( iProvider->iClient.AddFileToLoadFromSGEFile( aSGEFilePath, aFile ) );
        if ( !iProvider->iInitializationAO->IsActive() )
            {
            iProvider->iClient.StartLoadingFiles( iProvider->iInitializationAO->iStatus );   /* async */
            iProvider->iInitializationAO->SetActive();
            }
        }
    }

EXPORT_C void CSGEAudioProvider::LoadFilesL( RArray< TSGEAudioFile > &aFiles )
    {
    if ( iProvider->iEngine )
        {
        for ( TInt i = 0; i < aFiles.Count(); i++ )
            {
            User::LeaveIfError( iProvider->iEngine->AddFileToLoad( aFiles[ i ] ) );
            }
        iProvider->iEngine->StartLoadingFilesL();
        }
    else if ( iProvider->iSimpleEngine )
        {
        for ( TInt i = 0; i < aFiles.Count(); i++ )
            {
            User::LeaveIfError( iProvider->iSimpleEngine->AddFileToLoad( aFiles[ i ] ) );
            }
        iProvider->iSimpleEngine->StartLoadingFilesL();
        }
    else
        {
        for ( TInt i = 0; i < aFiles.Count(); i++ )
            {
            User::LeaveIfError( iProvider->iClient.AddFileToLoad( aFiles[ i ] ) );
            }
        if ( !iProvider->iInitializationAO->IsActive() )
            {
            iProvider->iClient.StartLoadingFiles( iProvider->iInitializationAO->iStatus );   /* async */
            iProvider->iInitializationAO->SetActive();
            }
        }
    }

EXPORT_C void CSGEAudioProvider::LoadFilesL( const TDesC& aSGEFilePath, RArray< TSGEAudioFile > &aFiles )
    {
    if ( iProvider->iEngine )
        {
        for ( TInt i = 0; i < aFiles.Count(); i++ )
            {
            User::LeaveIfError( iProvider->iEngine->AddFileToLoad( aSGEFilePath, aFiles[ i ] ) );
            }
        iProvider->iEngine->StartLoadingFilesL();
        }
    else if ( iProvider->iSimpleEngine )
        {
        for ( TInt i = 0; i < aFiles.Count(); i++ )
            {
            User::LeaveIfError( iProvider->iSimpleEngine->AddFileToLoad( aSGEFilePath, aFiles[ i ] ) );
            }
        iProvider->iSimpleEngine->StartLoadingFilesL();
        }
    else
        {
        for ( TInt i = 0; i < aFiles.Count(); i++ )
            {
            User::LeaveIfError( iProvider->iClient.AddFileToLoadFromSGEFile( aSGEFilePath, aFiles[ i ] ) );
            }
        if ( !iProvider->iInitializationAO->IsActive() )
            {
            iProvider->iClient.StartLoadingFiles( iProvider->iInitializationAO->iStatus );   /* async */
            iProvider->iInitializationAO->SetActive();
            }
        }
    }

EXPORT_C void CSGEAudioProvider::CreateAudioStreamL()
    {
    if ( iProvider->iEngine )
        {
        iProvider->iEngine->CreateAudioStreamL();
        }
    else if ( iProvider->iSimpleEngine )
        {
        iProvider->iSimpleEngine->CreateAudioStreamL();
        }
    else
        {
        if ( iProvider->iInitializationAO->IsActive() )
            {
            User::Leave( KErrInUse );
            }
        iProvider->iClient.CreateAudioStream( iProvider->iInitializationAO->iStatus );   /* async */
        iProvider->iInitializationAO->SetActive();
        }
    }

EXPORT_C void CSGEAudioProvider::DestroyAudioStream()
    {
    if ( iProvider->iEngine )
        {
        iProvider->iEngine->DestroyAudioStream();
        }
    else if ( iProvider->iSimpleEngine )
        {
        iProvider->iSimpleEngine->DestroyAudioStream();
        }
    else
        {
        iProvider->iClient.DestroyAudioStream();
        }
    }

EXPORT_C TSGEAudioTrackState CSGEAudioProvider::TrackState( TUint aSoundTrack )
    {
    if ( iProvider->iEngine )
        {
        return iProvider->iEngine->TrackState( aSoundTrack );
        }
    else if ( iProvider->iSimpleEngine )
        {
        return iProvider->iSimpleEngine->TrackState( aSoundTrack );
        }
    else
        {
        TSGEAudioTrackState state;
        iProvider->iClient.TrackState( aSoundTrack, state );
        return state;
        }
    }

EXPORT_C TInt CSGEAudioProvider::TrackCount()
    {
    if ( iProvider->iEngine )
        {
        return iProvider->iEngine->TrackCount();
        }
    else if ( iProvider->iSimpleEngine )
        {
        return iProvider->iSimpleEngine->TrackCount();
        }
    else
        {
        return iProvider->iClient.TrackCount();
        }
    }

EXPORT_C void CSGEAudioProvider::CreateEmptyTracksL( TInt aTrackCount )
	{
    if ( iProvider->iEngine )
        {
        iProvider->iEngine->CreateEmptyTracksL( aTrackCount );
        }
    else if ( iProvider->iSimpleEngine )
        {
        iProvider->iSimpleEngine->CreateEmptyTracksL( aTrackCount );
        }
	else
		{
		// todo
		}
	}

EXPORT_C TInt CSGEAudioProvider::Play( TUint aAudioSampleId, TInt aRepeat )
    {
    if ( iProvider->iEngine )
        {
        return iProvider->iEngine->Play( aAudioSampleId, aRepeat );
        }
    else if ( iProvider->iSimpleEngine )
        {
        return iProvider->iSimpleEngine->Play( aAudioSampleId, aRepeat );
        }
    else
        {
        return iProvider->iClient.Play( aAudioSampleId, aRepeat );
        }
    }

EXPORT_C void CSGEAudioProvider::Stop( TUint aSoundTrack )
    {
    if ( iProvider->iEngine )
        {
        iProvider->iEngine->Stop( aSoundTrack );
        }
    else if ( iProvider->iSimpleEngine )
        {
        iProvider->iSimpleEngine->Stop( aSoundTrack );
        }
    else
        {
        iProvider->iClient.Stop( aSoundTrack );
        }
    }

EXPORT_C void CSGEAudioProvider::Pause( TUint aSoundTrack )
    {
    if ( iProvider->iEngine )
        {
        iProvider->iEngine->Pause( aSoundTrack );
        }
    else if ( iProvider->iSimpleEngine )
        {
        iProvider->iSimpleEngine->Pause( aSoundTrack );
        }
    else
        {
        iProvider->iClient.Pause( aSoundTrack );
        }
    }

EXPORT_C void CSGEAudioProvider::Resume( TUint aSoundTrack )
    {
    if ( iProvider->iEngine )
        {
        iProvider->iEngine->Resume( aSoundTrack );
        }
    else if ( iProvider->iSimpleEngine )
        {
        iProvider->iSimpleEngine->Resume( aSoundTrack );
        }
    else
        {
        iProvider->iClient.Resume( aSoundTrack );
        }
    }

EXPORT_C void CSGEAudioProvider::Mute( TUint aSoundTrack )
    {
    if ( iProvider->iEngine )
        {
        iProvider->iEngine->Mute( aSoundTrack );
        }
    else if ( iProvider->iSimpleEngine )
        {
        iProvider->iSimpleEngine->Mute( aSoundTrack );
        }
    else
        {
        iProvider->iClient.Mute( aSoundTrack );
        }
    }

EXPORT_C void CSGEAudioProvider::Unmute( TUint aSoundTrack )
    {
    if ( iProvider->iEngine )
        {
        iProvider->iEngine->Unmute( aSoundTrack );
        }
    else if ( iProvider->iSimpleEngine )
        {
        iProvider->iSimpleEngine->Unmute( aSoundTrack );
        }
    else
        {
        iProvider->iClient.Unmute( aSoundTrack );    
        }
    }

EXPORT_C void CSGEAudioProvider::StopAll()
    {
    if ( iProvider->iEngine )
        {
        iProvider->iEngine->StopAll();
        }
    else if ( iProvider->iSimpleEngine )
        {
        iProvider->iSimpleEngine->StopAll();
        }
    else
        {
        iProvider->iClient.StopAll();
        }
    }

EXPORT_C void CSGEAudioProvider::PauseAll()
    {
    if ( iProvider->iEngine )
        {
        iProvider->iEngine->PauseAll();
        }
    else if ( iProvider->iSimpleEngine )
        {
        iProvider->iSimpleEngine->PauseAll();
        }
    else
        {
        iProvider->iClient.PauseAll();
        }
    }

EXPORT_C void CSGEAudioProvider::ResumeAll()
    {
    if ( iProvider->iEngine )
        {
        iProvider->iEngine->ResumeAll();
        }
    else if ( iProvider->iSimpleEngine )
        {
        iProvider->iSimpleEngine->ResumeAll();
        }
    else
        {
        iProvider->iClient.ResumeAll();
        }
    }

EXPORT_C void CSGEAudioProvider::MuteAll()
    {
    if ( iProvider->iEngine )
        {
        iProvider->iEngine->MuteAll();
        }
    else if ( iProvider->iSimpleEngine )
        {
        iProvider->iSimpleEngine->MuteAll();
        }
    else
        {
        iProvider->iClient.MuteAll();
        }
    }

EXPORT_C void CSGEAudioProvider::UnmuteAll()
    {
    if ( iProvider->iEngine )
        {
        iProvider->iEngine->UnmuteAll();
        }
    else if ( iProvider->iSimpleEngine )
        {
        iProvider->iSimpleEngine->UnmuteAll();
        }
    else
        {
        iProvider->iClient.UnmuteAll();
        }
    }

EXPORT_C void CSGEAudioProvider::SetVolume( TUint aSoundTrack, TInt aVolume )
    {
    if ( iProvider->iEngine )
        {
        iProvider->iEngine->SetVolume( aSoundTrack, aVolume );
        }
    else if ( iProvider->iSimpleEngine )
        {
        iProvider->iSimpleEngine->SetVolume( aSoundTrack, aVolume );
        }
    else
        {
        iProvider->iClient.SetVolume( aSoundTrack, aVolume );
        }
    }

EXPORT_C void CSGEAudioProvider::SetVolumeGroup( TSGEAudioGroup aGroup, TInt aVolume )
	{
    if ( iProvider->iEngine )
        {
        iProvider->iEngine->SetVolumeGroup( aGroup, aVolume );
        }
    else if ( iProvider->iSimpleEngine )
        {
        iProvider->iSimpleEngine->SetVolumeGroup( aGroup, aVolume );
        }
	else
		{
		// todo 
		}
	}

EXPORT_C void CSGEAudioProvider::SetVolumeAll( TInt aVolume )
    {
    if ( iProvider->iEngine )
        {
        iProvider->iEngine->SetVolumeAll( aVolume );
        }
    else if ( iProvider->iSimpleEngine )
        {
        iProvider->iSimpleEngine->SetVolumeAll( aVolume );
        }
    else
        {
        iProvider->iClient.SetVolumeAll( aVolume );
        }
    }

EXPORT_C void CSGEAudioProvider::SetVolumeDefault( TSGEAudioGroup aGroup, TInt aVolume )
	{
    if ( iProvider->iEngine )
        {
        iProvider->iEngine->SetVolumeDefault( aGroup, aVolume );
        }
    else if ( iProvider->iSimpleEngine )
        {
        iProvider->iSimpleEngine->SetVolumeDefault( aGroup, aVolume );
        }
    else
        {
        // @todo
        }
	}

EXPORT_C TInt CSGEAudioProvider::GetMaxVolume() const
    {
    if ( iProvider->iEngine )
        {
        return iProvider->iEngine->GetMaxVolume();
        }
    else if ( iProvider->iSimpleEngine )
        {
        return iProvider->iSimpleEngine->GetMaxVolume();
        }
    return 0;
    }

EXPORT_C TBool CSGEAudioProvider::IsSilentProfileL( TBool aMettingAndBeep )
    {
    TBool ret = EFalse;
    
    CRepository* cr = CRepository::NewLC( KCRUidProfileEngine );
    TInt actProfile;
    User::LeaveIfError( cr->Get( KProEngActiveProfile, actProfile ) );
    
    const TInt KSilentProfile = 1;
    const TInt KMeetingProfile = 2;

    if ( actProfile == KSilentProfile || ( aMettingAndBeep && actProfile == KMeetingProfile ) )
        {
        ret = ETrue;
        }
    else
        {
        TInt ringingType;
        User::LeaveIfError( cr->Get( KProEngActiveRingingType, ringingType ) );

        const TInt KSilentRingtone = 4;
        const TInt KBeepRingtone = 3;
        
        if ( ringingType == KSilentRingtone || ( aMettingAndBeep && ringingType == KBeepRingtone ) )
            {
            ret = ETrue;
            }       
        }
    
    CleanupStack::PopAndDestroy( cr );

    return ret;
    }



