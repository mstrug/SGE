/*
 ============================================================================
  Name        : SGEFileProvider.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#include "SGEFileHeader.h"


EXPORT_C CSGEFileHeader* CSGEFileHeader::NewLC()
	{
	CSGEFileHeader *self = new (ELeave) CSGEFileHeader();
	CleanupStack::PushL( self );
	return self;
	}

EXPORT_C CSGEFileHeader* CSGEFileHeader::NewL()
	{
	CSGEFileHeader *self = NewLC();
	CleanupStack::Pop( self );
	return self;
	}

EXPORT_C CSGEFileHeader* CSGEFileHeader::NewL( CSGEFileHeader *aHeader )
	{
	CSGEFileHeader *self = NewLC();
	self->CloneL( aHeader );
	CleanupStack::Pop( self );
	return self;
	}

void CSGEFileHeader::CloneL( CSGEFileHeader *aHeader )
	{
	iInternalVersion = aHeader->iInternalVersion;
	iInternalType = aHeader->iInternalType;
	iVersion = aHeader->iVersion;
	iType = aHeader->iType;
	iDescription.Copy( aHeader->iDescription );
	iAuthor.Copy( aHeader->iAuthor );
	iContact.Copy( aHeader->iContact );
	iDefaultFile.Copy( aHeader->iDefaultFile );
	}

CSGEFileHeader::CSGEFileHeader()
	{
	}
