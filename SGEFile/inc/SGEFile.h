/*
 ============================================================================
 Name		: SGEFile.h
 Author	  : Michal Strug
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : SGEFile.h - CSGEFile class header
 ============================================================================
 */

// This file defines the API for SGEFile.dll

#ifndef SGEFILE_H_
#define SGEFILE_H_

#include <f32file.h>
#include <S32FILE.H> 
#include <SGEFile/SGEFileHeader.h>

class RHashMap2;
class RHashMapS;


/**
 * class CSGEFile
 */
NONSHARABLE_CLASS( CSGEFile ) : public CBase
{
public:
	/**
	 * 
	 * @return CSGEFile* 
	 */
	IMPORT_C static CSGEFile* NewL();
	IMPORT_C static CSGEFile* NewLC();

	IMPORT_C static CSGEFile* NewL( RFs& aFs );
	IMPORT_C static CSGEFile* NewLC( RFs& aFs );

	IMPORT_C virtual ~CSGEFile();
	
	IMPORT_C static void CreateL( const TDesC& aSGEFileName, const TDesC& aPath, CSGEFileHeader *aHeader ); 

	IMPORT_C void LoadL( const TDesC& aSGEFileName );

	IMPORT_C RArray< TEntry > DirL( const TDesC& aPath = (TDesC&)KNullDesC() );
	IMPORT_C RArray< TEntry > DirTreeL( const TDesC& aPath = (TDesC&)KNullDesC() );
	IMPORT_C TBool FileExists( const TDesC& aFileNameAndPath );
	
	IMPORT_C RReadStream OpenFileL( const TDesC& aFileNameAndPath, TInt *aFileSize = NULL );
	IMPORT_C RReadStream OpenDefaultFileL( TInt *aFileSize = NULL );

	IMPORT_C void ExtractFileL( const TDesC& aFileNameAndPath, const TDesC& aOutputFileNameAndPath );
	IMPORT_C void ExtractDirL( const TDesC& aPath, const TDesC& aOutputPath );
	IMPORT_C void ExtractTreeL( const TDesC& aPath, const TDesC& aOutputPath );
	IMPORT_C void ExtractAllTreeL( const TDesC& aOutputPath );

	IMPORT_C const CSGEFileHeader* GetHeader();
	
private:
	CSGEFile();
	CSGEFile( RFs& aFs );
	void ConstructL();
	void CreateFileL( const TDesC& aSGEFileName, const TDesC& aPath ); 
	
	TStreamId WriteDirL( const TDesC& aPath, TStreamId aId = 0 );
	void CheckSignatureL();
	TStreamId WriteSignatureAndHeader();
	void ReadHeaderL();

	void GetMapsFromStreamIdL( TStreamId aId, RHashMap2 &mapDirs, RHashMap2 &mapFiles, RHashMapS &mapSizes );
	void GetMapsFromDictionaryL( CStreamDictionary *aDictionary, RHashMap2 &mapDirs, RHashMap2 &mapFiles );
	void GetMapsFromDictionaryL( CStreamDictionary *aDictionary, RHashMap2 &mapDirs, RHashMap2 &mapFiles, RHashMapS &mapSizes );
	CStreamDictionary* GetDictionaryFromStreamIdLC( TStreamId aId );
	void GetDirTreeFromStreamIdL( TStreamId aId, RArray< TEntry > &aArray, TDesC& aPrevPath = (TDesC&)KNullDesC() );
	
	void GetArrayEntryFromStreamIdL( TStreamId aId, RArray< TEntry > &aArray );
	
	TStreamId GetDirectoryStreamIdL( TStreamId aId, const TDesC& aDirName );
	TStreamId GetLastDirectoryStreamIdFromPathL( TStreamId aId, const TDesC& aDirName );

	TBuf<64> GetOneDirFromPath( TPtr& aPath );

	TStreamId GetFileStreamIdFromPathL( const TDesC& aFileNameAndPath, TInt *aFileSize = NULL );

	void ExtractFileFromStreamIdL( TStreamId aId, TInt aFileSize, const TDesC& aOutputFileNameAndPath );
	void ExtractFileFromTEntryL( TEntry *aEntry, CStreamDictionary *aDictionary, const TDesC& aOutputPath );
	void ExtractDirFromStreamIdL( TStreamId aId, const TDesC& aOutputPath );
	void ExtractDirFromTEntryL( TEntry *aEntry, CStreamDictionary *aDictionary, const TDesC& aOutputPath, TStreamId *aStartId = NULL );
	
	void EnsurePathExistsL( const TDesC& aPath, TBool aAddSlash = ETrue );
	
private:
	CFileStore *iFileStore; 
	RFs iFs;
	CSGEFileHeader *iHeader;
	TBool iFsOwned;
};

#endif /*SGEFILE_H_*/
