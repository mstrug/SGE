/*
 ============================================================================
 Name		: SGEStringFileBuffer.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEStringFileBuffer implementation
 ============================================================================
 */

#include "SGEStringFileBuffer.h"

CSGEStringFileBuffer::CSGEStringFileBuffer( TInt aStringId ) : iStringId( aStringId )
    {
    }

CSGEStringFileBuffer::~CSGEStringFileBuffer()
    {
    iString.Close();
    }

CSGEStringFileBuffer* CSGEStringFileBuffer::NewLC( const TDesC& aString, TInt aStringId )
    {
    CSGEStringFileBuffer *self = new (ELeave) CSGEStringFileBuffer( aStringId );
    CleanupStack::PushL( self );
    self->ConstructL( aString );
    return self;
    }

CSGEStringFileBuffer* CSGEStringFileBuffer::NewL( const TDesC& aString, TInt aStringId )
    {
    CSGEStringFileBuffer *self = CSGEStringFileBuffer::NewLC( aString, aStringId );
    CleanupStack::Pop(); // self;
    return self;
    }

void CSGEStringFileBuffer::ConstructL( const TDesC& aString )
    {
    iString.CreateL( aString );
    }

TPtrC CSGEStringFileBuffer::GetStringL( TInt aResourceId )
    {
    if ( aResourceId != iStringId )
        {
        User::Leave( KErrNotFound );
        }
    return iString;
    }

TPtrC CSGEStringFileBuffer::GetString( TInt aResourceId )
    {
    if ( aResourceId != iStringId )
        {
        return KNullDesC();
        }
    return iString;
    }


// end of file
