/*
 ============================================================================
  Name        : SGEFileProvider.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#ifndef __SGEFILEPROVIDER_H__
#define __SGEFILEPROVIDER_H__

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <e32hashtab.h>
#include <f32file.h>

// FORWARD DECLARATIONS
class CSGEFile;

// CLASS DECLARATION

/**
 *  CSGEFileProvider
 * 
 */
NONSHARABLE_CLASS( CSGEFileProvider ) : public CBase
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    IMPORT_C ~CSGEFileProvider();

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CSGEFileProvider* NewL();

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CSGEFileProvider* NewLC();

    IMPORT_C void AddSGEFileL( const TDesC& aFileName, TUint aFileId );

    IMPORT_C void AddSGEFileL( CSGEFile *aSGEFile, TUint aFileId );

    IMPORT_C CSGEFile& GetSGEFileL( TUint aFileId );
    
    IMPORT_C void RemoveSGEFile( TUint aFileId );
    
    IMPORT_C void RemoveSGEFileAll();
    
    IMPORT_C TChar InstallDrive() const;
    
    /**
     * On returns aPath contains path to the private folder on install drive.
     */
    IMPORT_C void PrivateFolder( TDes& aPath );
    
    /**
     * Returns appended to the private folder on install drive specified text.
     * Private path ends with '\' character
     */
    IMPORT_C TFileName& AppendToPrivateFolder( const TDesC& aText );
    
    IMPORT_C RFs& Fs();
    
private:

    /**
     * Constructor for performing 1st stage construction
     */
    CSGEFileProvider();

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();

private: // data

    RFs iFs;

    RPtrHashMap< TUint, CSGEFile > iSGEFiles;

    TChar iInstallDrive;

    TFileName iFileName;
    
    };

#endif // __SGEFILEPROVIDER_H__
