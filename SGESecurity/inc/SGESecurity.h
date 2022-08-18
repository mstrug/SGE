/*
 ============================================================================
  Name        : SGESecurity.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#ifndef __SGESECURITY_H__
#define __SGESECURITY_H__

// INCLUDES
#include <e32std.h>
#include <e32base.h>


// FORWARD DECLARATIONS

const TInt KMaxFileSizeNotSet = 0;

// CLASS DECLARATION
/**
 *  CSGESecurity
 * 
 */
NONSHARABLE_CLASS( SGESecurity )
    {
public:

    IMPORT_C static void GetDrmFileL( RBuf8& aFileData, const TDesC& aFileName, TInt aMaxFileSize = KMaxFileSizeNotSet );
    
    };


#endif // __SGESECURITY_H__
