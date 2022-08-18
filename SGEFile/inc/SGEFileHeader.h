/*
 ============================================================================
  Name        : SGEFileHeader.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#ifndef SGEFILEHEADER_H_
#define SGEFILEHEADER_H_

#include <e32base.h>
#include <S32FILE.H> 


NONSHARABLE_CLASS( CSGEFileHeader ) : public CBase
	{
private:
	
	CSGEFileHeader();

public:

	IMPORT_C static CSGEFileHeader* NewLC();

	IMPORT_C static CSGEFileHeader* NewL();

	IMPORT_C static CSGEFileHeader* NewL( CSGEFileHeader *aHeader );

	void CloneL( CSGEFileHeader *aHeader );

	
public:
	
	inline void InternalizeL( RReadStream& aStream )
		{
		iInternalVersion = aStream.ReadUint16L();
		iInternalType = aStream.ReadUint16L();
		iVersion = aStream.ReadUint16L();
		iType = aStream.ReadUint16L();
		TInt len = aStream.ReadInt16L();
		if ( len )
			{
			aStream.ReadL( iDescription, len );
			}
		len = aStream.ReadInt16L();
		if ( len )
			{
			aStream.ReadL( iAuthor, len );
			}
		len = aStream.ReadInt16L();
		if ( len )
			{
			aStream.ReadL( iContact, len );
			}
		len = aStream.ReadInt16L();
		if ( len )
			{
			aStream.ReadL( iDefaultFile, len );
			}
		}
	
	inline void ExternalizeL( RWriteStream& aStream ) const
		{
		aStream.WriteUint16L( iInternalVersion );
		aStream.WriteUint16L( iInternalType );
		aStream.WriteUint16L( iVersion );
		aStream.WriteUint16L( iType );
		aStream.WriteInt16L( iDescription.Length() );
		if ( iDescription.Length() )
			{
			aStream.WriteL( iDescription, iDescription.Length() );	
			}
		aStream.WriteInt16L( iAuthor.Length() );
		if ( iAuthor.Length() )
			{
			aStream.WriteL( iAuthor, iAuthor.Length() );	
			}
		aStream.WriteInt16L( iContact.Length() );
		if ( iContact.Length() )
			{
			aStream.WriteL( iContact, iContact.Length() );	
			}
		aStream.WriteInt16L( iDefaultFile.Length() );
		if ( iDefaultFile.Length() )
			{
			aStream.WriteL( iDefaultFile, iDefaultFile.Length() );	
			}
		}

public:
	
	enum TSGEFileType
		{
		ENormal = 0
		};
	
public:
	
	TUint16 iInternalVersion;
	TUint16 iInternalType;
	TUint16 iVersion;
	TUint16 iType;
	TBuf<256> iDescription;
	TBuf<256> iAuthor;
	TBuf<256> iContact;
	TBuf<256> iDefaultFile;
	};


#endif /*SGEFILEHEADER_H_*/
