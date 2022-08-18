/*
 ============================================================================
 Name		: SGESecurity.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : 
 ============================================================================
 */

#include "SGESecurity.h"
#include <caf.h>


using namespace ContentAccess;


EXPORT_C void SGESecurity::GetDrmFileL( RBuf8& aFileData, const TDesC& aFileName, TInt aMaxFileSize )
    {
    // Create a CContent object, supplying protected content file name
    // CAF will figure out the appropriate agent
    CContent* content = CContent::NewLC( aFileName );

    // Create a CData object to read the content.
    // Tell the agent we are planning to "execute" the content.
    // *** if OpenContentL gets error -6 here: KErrArgument,
    //     then CAF sees that you have rights but does not like the 
    //     content MIME type and DRM intent match.
    //     For example: image/png in DCF with EView is OK
    //                  image/png in DCF with EPlay is _not_ OK
    //
    // For Ovi Store, always use EExecute intent.
    CData* data = content->OpenContentL( ContentAccess::EExecute );
   
    // Don't need content object any more
    CleanupStack::PopAndDestroy( content );
    CleanupStack::PushL( data );

    // Get the size of the plaintext content
    TInt size;
    data->DataSizeL( size );

    // Guard against potential heap overflow
    if ( aMaxFileSize && size > aMaxFileSize )
        {
        User::Leave( KErrTooBig );
        }

    // Execute the intent, tell the agent that we plan to "execute" the content.
    // Always call ExecuteIntent shortly before attempting to read the data.
    User::LeaveIfError( data->ExecuteIntent( ContentAccess::EExecute ) );

    aFileData.CreateL( size );
    aFileData.CleanupClosePushL();

    // read data
    //TPtr8 pDecryptedContent = decryptedContent->Des();
    User::LeaveIfError( data->Read( aFileData ) );

    CleanupStack::Pop( &aFileData ); 

    // Cleanup loaded resources
    CleanupStack::PopAndDestroy( data );
    }


// end of file
