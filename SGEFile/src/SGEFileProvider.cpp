/*
 ============================================================================
  Name        : SGEFileProvider.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#include "SGEFileProvider.h"
#include "SGEFile.h"
#include <EIKENV.H>
#include <EIKAPPUI.H>
#include <EIKAPP.H>
#include <e32std.h>


// -----------------------------------------------------------------------------
// CSGEFileProvider::CSGEFileProvider()
// -----------------------------------------------------------------------------
//
CSGEFileProvider::CSGEFileProvider()
    {
    }

// -----------------------------------------------------------------------------
// CSGEFileProvider::~CSGEFileProvider()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEFileProvider::~CSGEFileProvider()
    {
    iSGEFiles.ResetAndDestroy();
    iFs.Close();
    }

// -----------------------------------------------------------------------------
// CSGEFileProvider::NewL()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEFileProvider* CSGEFileProvider::NewL()
    {
    CSGEFileProvider *self = CSGEFileProvider::NewLC();
    CleanupStack::Pop(); // self;
    return self;
    }

// -----------------------------------------------------------------------------
// CSGEFileProvider::NewLC()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEFileProvider* CSGEFileProvider::NewLC()
    {
    CSGEFileProvider *self = new (ELeave) CSGEFileProvider();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CSGEFileProvider::ConstructL()
// -----------------------------------------------------------------------------
//
void CSGEFileProvider::ConstructL()
    {
    User::LeaveIfError( iFs.Connect() );

#ifdef __WINSCW__
    iInstallDrive = TChar( 'c' );
#else
    iInstallDrive = CEikonEnv::Static()->EikAppUi()->Application()->AppFullName()[ 0 ];
#endif
    }

// -----------------------------------------------------------------------------
// CSGEFileProvider::AddSGEFileL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEFileProvider::AddSGEFileL( const TDesC& aFileName, TUint aFileId )
    {
    CSGEFile *file = CSGEFile::NewLC( iFs );
    file->LoadL( aFileName );
    CleanupStack::Pop( file );
    AddSGEFileL( file, aFileId );
    }

// -----------------------------------------------------------------------------
// CSGEFileProvider::AddSGEFileL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEFileProvider::AddSGEFileL( CSGEFile *aSGEFile, TUint aFileId )
    {
    ASSERT( aSGEFile );
    
    CleanupStack::PushL( aSGEFile );
    
    TUint *id = new (ELeave) TUint;
    CleanupStack::PushL( id );
    *id = aFileId;
    
    if ( iSGEFiles.Find( *id ) )
        {
        User::Leave( KErrAlreadyExists );
        }
    
    iSGEFiles.InsertL( id, aSGEFile );

    CleanupStack::Pop( id );
    CleanupStack::Pop( aSGEFile );
    }

// -----------------------------------------------------------------------------
// CSGEFileProvider::GetSGEFileL()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEFile& CSGEFileProvider::GetSGEFileL( TUint aFileId )
    {
    return iSGEFiles.FindL( aFileId );
    }

// -----------------------------------------------------------------------------
// CSGEFileProvider::RemoveSGEFile()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEFileProvider::RemoveSGEFile( TUint aFileId )
    {
    delete iSGEFiles.Find( aFileId );
    iSGEFiles.Remove( &aFileId );
    }

// -----------------------------------------------------------------------------
// CSGEFileProvider::RemoveSGEFileAll()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEFileProvider::RemoveSGEFileAll()
    {
    iSGEFiles.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CSGEFileProvider::InstallDrive()
// -----------------------------------------------------------------------------
//
EXPORT_C TChar CSGEFileProvider::InstallDrive() const
    {
    return iInstallDrive;
    }

// -----------------------------------------------------------------------------
// CSGEFileProvider::PrivateFolder()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEFileProvider::PrivateFolder( TDes& aPath )
    {
    iFs.PrivatePath( aPath );
    
    const TUint KDrive = ':';
    
    TBuf<2> drive;
    drive.Append( iInstallDrive );
    drive.Append( TChar( KDrive ) );
    
    aPath.Insert( 0, drive );
    }

// -----------------------------------------------------------------------------
// CSGEFileProvider::AppendToPrivateFolder()
// -----------------------------------------------------------------------------
//
EXPORT_C TFileName& CSGEFileProvider::AppendToPrivateFolder( const TDesC& aText )
    {
    PrivateFolder( iFileName );
    if ( aText.Left( 1 ).Compare( _L("\\") ) == 0 )
        {
        iFileName.Append( aText.Right( aText.Length() - 1 ) );
        }
    else
        {
        iFileName.Append( aText );
        }
    return iFileName;
    }

// -----------------------------------------------------------------------------
// CSGEFileProvider::Fs()
// -----------------------------------------------------------------------------
//
EXPORT_C RFs& CSGEFileProvider::Fs()
    {
    return iFs;
    }


// end of file
