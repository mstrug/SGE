/*
 ============================================================================
 Name		: SGEStringProvider.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEStringProvider implementation
 ============================================================================
 */

#include "SGEStringProvider.h"
#include "SGEStringFileBase.h"
#include "SGEStringFileBuffer.h"
#include "SGEStringFileMemory.h"
#include "SGEStringFileResource.h"


CSGEStringProvider::CSGEStringProvider()
    {
    }

EXPORT_C CSGEStringProvider::~CSGEStringProvider()
    {
    THashMapIter< TInt, RPointerArray< CSGEStringFileBase > > iter( iStringFiles );
    iter.Reset();
    while ( iter.NextKey() )
        {
        iter.CurrentValue()->ResetAndDestroy();
        }
    iStringFiles.Close();
    
    iStringFilesCommon.ResetAndDestroy();
    iFs.Close();
    }

EXPORT_C CSGEStringProvider* CSGEStringProvider::NewLC()
    {
    CSGEStringProvider *self = new (ELeave) CSGEStringProvider();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

EXPORT_C CSGEStringProvider* CSGEStringProvider::NewL()
    {
    CSGEStringProvider *self = CSGEStringProvider::NewLC();
    CleanupStack::Pop(); // self;
    return self;
    }

void CSGEStringProvider::ConstructL()
    {
    User::LeaveIfError( iFs.Connect() );
    }

EXPORT_C void CSGEStringProvider::AddStringL( const TDesC& aString, TUint aId )
    {
    CSGEStringFileBuffer *file = CSGEStringFileBuffer::NewL( aString, aId );
    CleanupStack::PushL( file );

    iStringFilesCommon.AppendL( file );

    CleanupStack::Pop( file );
    }

EXPORT_C void CSGEStringProvider::AddStringL( const TDesC& aString, TUint aId, TLanguage aLanguage )
    {
    RPointerArray< CSGEStringFileBase > *files = iStringFiles.Find( aLanguage );
    if ( !files )
        {
        RPointerArray< CSGEStringFileBase > newFiles;
        iStringFiles.InsertL( aLanguage, newFiles );
        files = &newFiles;
        }

    CSGEStringFileBuffer *file = CSGEStringFileBuffer::NewL( aString, aId );
    CleanupStack::PushL( file );

    files->AppendL( file );

    CleanupStack::Pop( file );
    }

EXPORT_C void CSGEStringProvider::AddFileL( const TDesC& aFileName )
    {
    CSGEStringFileResource *file = CSGEStringFileResource::NewL( iFs, aFileName );
    CleanupStack::PushL( file );

    iStringFilesCommon.AppendL( file );

    CleanupStack::Pop( file );
    }

EXPORT_C void CSGEStringProvider::AddFileL( CSGEFile& aSGEFile, const TDesC& aFileName )
    {
    CSGEStringFileMemory *file = CSGEStringFileMemory::NewL( iFs, aSGEFile, aFileName );
    CleanupStack::PushL( file );

    iStringFilesCommon.AppendL( file );

    CleanupStack::Pop( file );
    }

EXPORT_C void CSGEStringProvider::AddFileL( const TDesC& aFileName, TLanguage aLanguage )
    {
    RPointerArray< CSGEStringFileBase > *files = iStringFiles.Find( aLanguage );
    if ( !files )
        {
        RPointerArray< CSGEStringFileBase > newFiles;
        iStringFiles.InsertL( aLanguage, newFiles );
        files = iStringFiles.Find( aLanguage );
        ASSERT( files );
        }

    CSGEStringFileResource *file = CSGEStringFileResource::NewL( iFs, aFileName );
    CleanupStack::PushL( file );

    files->AppendL( file );

    CleanupStack::Pop( file );
    }

EXPORT_C void CSGEStringProvider::AddFileL( CSGEFile& aSGEFile, const TDesC& aFileName, TLanguage aLanguage )
    {
    RPointerArray< CSGEStringFileBase > *files = iStringFiles.Find( aLanguage );
    if ( !files )
        {
        RPointerArray< CSGEStringFileBase > newFiles;
        iStringFiles.InsertL( aLanguage, newFiles );
        files = iStringFiles.Find( aLanguage );
        ASSERT( files );
        }

    CSGEStringFileMemory *file = CSGEStringFileMemory::NewL( iFs, aSGEFile, aFileName );
    CleanupStack::PushL( file );

    files->AppendL( file );

    CleanupStack::Pop( file );
    }

EXPORT_C TPtrC CSGEStringProvider::GetStringL( TInt aResourceId )
    {
    // checking files specific to current language
    RPointerArray< CSGEStringFileBase > *files = iStringFiles.Find( iCurrentLanguage );
    if ( files )
        {
        for ( TInt i = 0; i < files->Count(); i++ )
            {
            TPtrC str;
            TRAPD( err, str.Set( (*files)[ i ]->GetStringL( aResourceId ) ) );
            if ( err == KErrNone )
                {
                return str;
                }
            else if ( err != KErrNotFound )
                {
                User::Leave( err );
                }
            }
        }

    // checking files common to all languages
    for ( TInt i = 0; i < iStringFilesCommon.Count(); i++ )
        {
        TPtrC str;
        TRAPD( err, str.Set( iStringFilesCommon[ i ]->GetStringL( aResourceId ) ) );
        if ( err == KErrNone )
            {
            return str;
            }
        else if ( err != KErrNotFound )
            {
            User::Leave( err );
            }
        }
    
    // if string not found then leave 
    User::Leave( KErrNotFound );
    
    return KNullDesC();
    }

EXPORT_C TPtrC CSGEStringProvider::GetString( TInt aResourceId, TBool aTryToLoad )
    {
    if ( aTryToLoad )
        {
        TPtrC ret;
        TRAP_IGNORE( ret.Set( GetStringL( aResourceId ) ) );
        return ret;
        }
    else
        {
        // checking files specific to current language
        RPointerArray< CSGEStringFileBase > *files = iStringFiles.Find( iCurrentLanguage );
        if ( files )
            {
            for ( TInt i = 0; i < files->Count(); i++ )
                {
                TPtrC str = (*files)[ i ]->GetString( aResourceId );
                if ( str.Compare( KNullDesC() ) != 0 )
                    {
                    return str;
                    }
                }
            }
    
        // checking files common to all languages
        for ( TInt i = 0; i < iStringFilesCommon.Count(); i++ )
            {
            TPtrC str = iStringFilesCommon[ i ]->GetString( aResourceId );
            if ( str.Compare( KNullDesC() ) != 0 )
                {
                return str;
                }
            }
        }
    
    return KNullDesC();
    }

EXPORT_C TPtrC CSGEStringProvider::GetStringL( TInt aResourceId, TLanguage aLanguage )
    {
    // checking files specific to current language
    RPointerArray< CSGEStringFileBase > &files = iStringFiles.FindL( aLanguage );
    for ( TInt i = 0; i < files.Count(); i++ )
        {
        TPtrC str;
        TRAPD( err, str.Set( files[ i ]->GetStringL( aResourceId ) ) );
        if ( err == KErrNone )
            {
            return str;
            }
        else if ( err != KErrNotFound )
            {
            User::Leave( err );
            }
        }
    
    // if string not found then leave 
    User::Leave( KErrNotFound );
    
    return KNullDesC();
    }

EXPORT_C void CSGEStringProvider::ChangeLanguageL( TLanguage aLanguage )
    {
    iCurrentLanguage = aLanguage;
    }

EXPORT_C TLanguage CSGEStringProvider::Language() const
    {
    return iCurrentLanguage;
    }


// end of file
