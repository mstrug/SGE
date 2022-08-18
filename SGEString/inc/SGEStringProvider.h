/*
 ============================================================================
  Name        : SGEStringsProvider.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#ifndef __SGESTRINGSPROVIDER_H__
#define __SGESTRINGSPROVIDER_H__

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <e32hashtab.h>
#include <f32file.h>


// FORWARD DECLARATIONS
class CSGEFile;
class CSGEStringFileBase;

// CLASS DECLARATION
/**
 *  CSGEStringProvider 
 * 
 */
NONSHARABLE_CLASS( CSGEStringProvider ) : public CBase
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    IMPORT_C ~CSGEStringProvider();

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CSGEStringProvider* NewLC();
    
    /**
     * Two-phased constructor.
     */
    IMPORT_C static CSGEStringProvider* NewL();
    
    IMPORT_C void AddStringL( const TDesC& aString, TUint aId );

    IMPORT_C void AddStringL( const TDesC& aString, TUint aId, TLanguage aLanguage );

    IMPORT_C void AddFileL( const TDesC& aFileName );

    IMPORT_C void AddFileL( CSGEFile& aSGEFile, const TDesC& aFileName );

    IMPORT_C void AddFileL( const TDesC& aFileName, TLanguage aLanguage );

    IMPORT_C void AddFileL( CSGEFile& aSGEFile, const TDesC& aFileName, TLanguage aLanguage );

    IMPORT_C TPtrC GetStringL( TInt aResourceId );

    IMPORT_C TPtrC GetString( TInt aResourceId, TBool aTryToLoad = EFalse );

    IMPORT_C TPtrC GetStringL( TInt aResourceId, TLanguage aLanguage );

    IMPORT_C void ChangeLanguageL( TLanguage aLanguage );

    IMPORT_C TLanguage Language() const;

private:
    
    /**
     * Constructor for performing 1st stage construction
     */
    CSGEStringProvider();

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();
    
private: // data

    RFs iFs;
    
    TLanguage iCurrentLanguage;

    RHashMap< TInt, RPointerArray< CSGEStringFileBase > > iStringFiles;

    RPointerArray< CSGEStringFileBase > iStringFilesCommon;
    
    };


#endif // __SGESTRINGSPROVIDER_H__
