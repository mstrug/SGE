/*
 ============================================================================
  Name        : SGEFontProvider.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#ifndef __SGEFONTPROVIDER_H__
#define __SGEFONTPROVIDER_H__

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <e32hashtab.h>


// FORWARD DECLARATIONS
class CSGEFont;
class CSGEFontFamily;

// CLASS DECLARATION
/**
 *  CSGEFontProvider
 * 
 */
NONSHARABLE_CLASS( CSGEFontProvider ) : public CBase
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    IMPORT_C ~CSGEFontProvider();

    /*
     * 
     */
    IMPORT_C static CSGEFontProvider* NewLC();
    
    /*
     * 
     */
    IMPORT_C static CSGEFontProvider* NewL();
    
    IMPORT_C void AddFontL( CSGEFont *aFont, TUint aFontFamilyId, TUint aFontSize );
    
    /**
     * This function befor adding aFont pushes it on cleanup stack, so it is safe
     * to call this function with newly created font without pushed on cleanup stack.
     */
    IMPORT_C void AddFontLD( CSGEFont *aFont, TUint aFontFamilyId, TUint aFontSize );
    
    IMPORT_C CSGEFont* GetFont( TUint aFontFamilyId, TUint aFontSize ) const;

    IMPORT_C CSGEFontFamily* GetFontFamily( TUint aFontFamilyId ) const;

    IMPORT_C void RemoveFontFamily( TUint aFontFamilyId );

    IMPORT_C void RemoveFontFamilyAll();

private:

    /**
     * Constructor for performing 1st stage construction
     */
    CSGEFontProvider();

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();
    
    CSGEFontFamily* AddFontFamilyL( TUint aFontFamilyId );

private: // data
    
    RPtrHashMap< TUint, CSGEFontFamily > iFontFamilies;

    };

#endif // __SGEFONTPROVIDER_H__
