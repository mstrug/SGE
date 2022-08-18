/*
 ============================================================================
 Name		: SGEFontFamily.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEFontFamily declaration
 ============================================================================
 */

#ifndef SGEFONTFAMILY_H
#define SGEFONTFAMILY_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <e32hashtab.h>

// FORWARD DECLARATIONS
class CSGEFont;

// CLASS DECLARATION
/**
 *  CSGEFontFamily
 * 
 */
NONSHARABLE_CLASS( CSGEFontFamily ) : public CBase
    {
public:
    // Constructors and destructor
    
    /**
     * Constructor.
     */
    CSGEFontFamily();

    /**
     * Destructor.
     */
    ~CSGEFontFamily();
    
    void AddFontL( CSGEFont *aFont, TUint aFontSize );

    IMPORT_C void RemoveFont( TUint aFontSize );

    void RemoveFontAll();

    IMPORT_C CSGEFont* GetFont( TUint aFontSize ) const;
    
private:

    RPtrHashMap< TUint, CSGEFont > iFonts;
    
    };

#endif // SGEFONTFAMILY_H
