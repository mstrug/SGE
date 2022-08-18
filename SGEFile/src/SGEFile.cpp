/*
 ============================================================================
 Name		: SGEFile.cpp
 Author	  : Michal Strug
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEFile DLL source
 ============================================================================
 */

#include "RHashMapT.h"
#include "SGEFile.h"
#include <BAUTILS.H>
 


_LIT8( KSGEFileSignature, "SGES" );

EXPORT_C CSGEFile* CSGEFile::NewLC()
	{
	CSGEFile *self = new (ELeave) CSGEFile;
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

EXPORT_C CSGEFile* CSGEFile::NewLC( RFs& aFs )
	{
	CSGEFile *self = new (ELeave) CSGEFile( aFs );
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

EXPORT_C CSGEFile* CSGEFile::NewL()
	{
	CSGEFile *self = NewLC();
	CleanupStack::Pop();
	return self;
	}

EXPORT_C CSGEFile* CSGEFile::NewL( RFs& aFs )
	{
	CSGEFile *self = NewLC( aFs );
	CleanupStack::Pop();
	return self;
	}

CSGEFile::CSGEFile() : iFsOwned( ETrue )
	{
	}

CSGEFile::CSGEFile( RFs& aFs ) : iFs( aFs )
	{
	}

void CSGEFile::ConstructL()
	{
        if ( iFsOwned )
        	{
		iFs.Connect();
		}
	}

EXPORT_C CSGEFile::~CSGEFile()
	{
	delete iFileStore;
	delete iHeader;
	if ( iFsOwned )
		{
		iFs.Close();
		}
	}


EXPORT_C void CSGEFile::CreateL( const TDesC& aSGEFileName, const TDesC& aPath, CSGEFileHeader *aHeader )
	{
	CSGEFile *file = NewLC();
	file->iHeader = CSGEFileHeader::NewL( aHeader );
	file->CreateFileL( aSGEFileName, aPath );
	CleanupStack::PopAndDestroy( file );
	}

void CSGEFile::CreateFileL( const TDesC& aSGEFileName, const TDesC& aPath )
	{
	iFs.Connect();
	iFileStore = CDirectFileStore::ReplaceL( iFs, aSGEFileName, EFileWrite );
	iFileStore->SetTypeL(TUidType(KDirectFileStoreLayoutUid));

	TStreamId id = WriteSignatureAndHeader();
	TStreamId rootId = WriteDirL( aPath, id );
	
	if( !rootId.Value() )
		{
		User::Leave( KErrCorrupt );
		}
	
	iFileStore->SetRootL( rootId );
	iFileStore->CommitL();
	}

TStreamId CSGEFile::WriteSignatureAndHeader()
	{
	iHeader->iVersion = 0x001;

	RStoreWriteStream stream;
	TStreamId id = stream.CreateL( *iFileStore );
	CleanupClosePushL( stream );
	stream.WriteL( KSGEFileSignature(), KSGEFileSignature().Length() );
	stream << *iHeader;
	stream.CommitL();
	CleanupStack::PopAndDestroy(&stream);	// stream
	iFileStore->CommitL();
	return id;
	}

TStreamId CSGEFile::WriteDirL( const TDesC& aDir, TStreamId aId )
	{
	RHashMap2 mapFiles, mapDirs;
	RHashMapS mapSizes;
	RBuf rbuf;
	
	CleanupClosePushL( mapFiles );	
	CleanupClosePushL( mapDirs );	
	CleanupClosePushL( mapSizes );	

	rbuf.CreateL( aDir.Length() + 4 );
	rbuf.CleanupClosePushL();
	rbuf.Copy( aDir );
	if( aDir.Right(1).Compare( _L("\\") ) == 0 )
		{
		User::Leave( KErrArgument );
		}
	rbuf.Append( _L("\\*.*") );

	CDir *dir = NULL;
	iFs.GetDir( rbuf, KEntryAttMatchMask | KEntryAttReadOnly | KEntryAttHidden | KEntryAttArchive | KEntryAttNormal | KEntryAttDir, EDirsLast, dir );
	CleanupStack::PopAndDestroy(&rbuf);	// rbuf
	
	if( !dir )
		{
		CleanupStack::PopAndDestroy(&mapSizes);
		CleanupStack::PopAndDestroy(&mapDirs);
		CleanupStack::PopAndDestroy(&mapFiles);
		return 0;
		}
	CleanupStack::PushL( dir );

	CStreamDictionary *streamDictionary = CStreamDictionary::NewLC();
	
	for ( TInt i = 0; i < dir->Count(); i++ )
		{
		TEntry entry = (*dir)[i];
		TUid uid = TUid::Uid( i + 0x01000 );

		rbuf.CreateL( aDir.Length() + entry.iName.Length() + 1 );
		rbuf.CleanupClosePushL();
		rbuf.Copy( aDir );
		rbuf.Append( _L("\\") );
		rbuf.Append( entry.iName );
		
		if( entry.IsDir() ) 
			{
			TStreamId id = WriteDirL( rbuf );
			if ( id.Value() )
				{
				streamDictionary->AssignL( uid, id );
				mapDirs.InsertL( entry.iName, uid );
				}
			}
		else
			{		
			RBuf8 buffile;
			RFile file;
			if( file.Open( iFs, rbuf, EFileRead ) != KErrNone )
				{
				CleanupStack::PopAndDestroy(&rbuf);	// rbuf
				continue;
				}
			CleanupClosePushL( file );

			TStreamId id(0);
			RStoreWriteStream stream;
			
			TInt filesize;
			file.Size(filesize);
			
			if( filesize > 100*1024 )
				{
				buffile.CreateL( 100*1024 );
				buffile.CleanupClosePushL();
				id = stream.CreateL( *iFileStore );
				CleanupClosePushL( stream );
				
				do
					{
					file.Read( buffile );
					stream.WriteL( buffile );
					}
				while( buffile.Length() > 0 );

				stream.CommitL();
				CleanupStack::PopAndDestroy(&stream);	// stream
				CleanupStack::PopAndDestroy(&buffile);	// buffile
				CleanupStack::PopAndDestroy(&file);		// file
				}
			else
				{
				buffile.CreateL( filesize );
				file.Read( buffile );
				CleanupStack::PopAndDestroy(&file);	// file
				buffile.CleanupClosePushL();

				id = stream.CreateL( *iFileStore );
				CleanupClosePushL( stream );
				stream.WriteL( buffile );
				stream.CommitL();
				CleanupStack::PopAndDestroy(&stream);	// stream
				CleanupStack::PopAndDestroy(&buffile);	// buffile
				}

			streamDictionary->AssignL( uid, id );
			mapFiles.InsertL( entry.iName, uid );
			mapSizes.Insert( uid, filesize );
			}
		
		CleanupStack::PopAndDestroy(&rbuf);	// rbuf
		}

	RStoreWriteStream stream;
	TStreamId id = stream.CreateL( *iFileStore );
	CleanupClosePushL( stream );
	stream << mapFiles;
	stream << mapDirs;
	stream << mapSizes;
	stream.CommitL();
	CleanupStack::PopAndDestroy(&stream);	// stream
	streamDictionary->AssignL( TUid::Uid(1), id );
	if ( aId.Value() )
		{
		streamDictionary->AssignL( TUid::Uid(0), aId );
		}

	id = stream.CreateL(*iFileStore);
	CleanupClosePushL( stream );
	stream << *streamDictionary;
	stream.CommitL();
	CleanupStack::PopAndDestroy(&stream);	// stream
	
	CleanupStack::PopAndDestroy(streamDictionary);	// streamDictionary
	CleanupStack::PopAndDestroy(dir);	// dir

	CleanupStack::PopAndDestroy(&mapSizes);
	CleanupStack::PopAndDestroy(&mapDirs);	// mapFiles, mapDirs
	CleanupStack::PopAndDestroy(&mapFiles);	// mapFiles, mapDirs
	
	return id;
	}

EXPORT_C void CSGEFile::LoadL( const TDesC& aSGEFileName )
	{
	iFileStore = CDirectFileStore::OpenL( iFs, aSGEFileName, EFileRead | EFileShareReadersOnly );
	CheckSignatureL();
	ReadHeaderL();
	}

void CSGEFile::CheckSignatureL()
	{
	TBuf8<4> buf;
	TInt offs = 20;
	iFileStore->File().Seek( ESeekStart, offs );
	iFileStore->File().Read( buf );
	if( buf.Compare( KSGEFileSignature() ) != 0 )
		{
		User::Leave( KErrCorrupt );
		}	
	}

void CSGEFile::ReadHeaderL()
	{
	iHeader = CSGEFileHeader::NewL();
	
	CStreamDictionary* dictionary = GetDictionaryFromStreamIdLC( iFileStore->Root() );
	TStreamId id = dictionary->At( TUid::Uid( 0 ) );
	CleanupStack::PopAndDestroy( dictionary );
	
	RStoreReadStream stream;
	stream.OpenLC( *iFileStore, id );
	stream.ReadL( KSGEFileSignature().Length() );
	stream >> *iHeader; 
	CleanupStack::PopAndDestroy(); // stream
	}

EXPORT_C RArray< TEntry > CSGEFile::DirL( const TDesC& aPath )
	{
	TStreamId id = GetLastDirectoryStreamIdFromPathL( iFileStore->Root(), aPath );
	
	RArray< TEntry > ret;

	GetArrayEntryFromStreamIdL( id, ret );

	return ret;
	}

EXPORT_C RArray< TEntry > CSGEFile::DirTreeL( const TDesC& aPath )
	{
	RArray< TEntry > ret;
	
	TStreamId id = GetLastDirectoryStreamIdFromPathL( iFileStore->Root(), aPath );

	GetDirTreeFromStreamIdL( id, ret );

	return ret;
	}

EXPORT_C TBool CSGEFile::FileExists( const TDesC& aFileNameAndPath )
	{
	if ( aFileNameAndPath.Right( 1 ).Compare( _L("\\") ) == 0 )
		{
		User::Leave( KErrArgument );
		}
	
	TInt size;
	TRAPD( error, GetFileStreamIdFromPathL( aFileNameAndPath, &size ) );
	if ( error )
		{
		return EFalse;
		}
	return ETrue;
	}

EXPORT_C RReadStream CSGEFile::OpenFileL( const TDesC& aFileNameAndPath, TInt *aFileSize )
	{
	if ( aFileNameAndPath.Right( 1 ).Compare( _L("\\") ) == 0 )
		{
		User::Leave( KErrArgument );
		}

	TStreamId id = GetFileStreamIdFromPathL( aFileNameAndPath, aFileSize );
			
	RStoreReadStream stream;
	stream.OpenL( *iFileStore, id );
	
	return stream; 	
	}

EXPORT_C RReadStream CSGEFile::OpenDefaultFileL( TInt *aFileSize )
	{
	if ( iHeader->iDefaultFile.Length() == 0 )
		{
		User::Leave( KErrNotFound );
		}
	RReadStream stream = OpenFileL( iHeader->iDefaultFile, aFileSize );
	return stream;
	}

EXPORT_C void CSGEFile::ExtractFileL( const TDesC& aFileNameAndPath, const TDesC& aOutputFileNameAndPath )
	{
	if ( aFileNameAndPath.Right( 1 ).Compare( _L("\\") ) == 0 || aFileNameAndPath.Length() == 0 )
		{
		User::Leave( KErrArgument );
		}

	TInt filesize;
	TStreamId id = GetFileStreamIdFromPathL( aFileNameAndPath, &filesize );

	EnsurePathExistsL( aOutputFileNameAndPath, EFalse );

	if ( aOutputFileNameAndPath.Right( 1 ).Compare( _L("\\") ) == 0 )
		{
		RBuf buf;
		TInt slash = aFileNameAndPath.LocateReverse( '\\' );
		TPtrC ptr( aFileNameAndPath.Right( aFileNameAndPath.Length() - slash - 1 ) );
		buf.CreateL( aOutputFileNameAndPath.Length() + ptr.Length() );
		buf.CleanupClosePushL();
		buf.Copy( aOutputFileNameAndPath );
		buf.Append( ptr );

		ExtractFileFromStreamIdL( id, filesize, buf );
		
		CleanupStack::PopAndDestroy( &buf );
		}
	else
		{
		ExtractFileFromStreamIdL( id, filesize, aOutputFileNameAndPath );
		}
	}

EXPORT_C void CSGEFile::ExtractDirL( const TDesC& aPath, const TDesC& aOutputPath )
	{
	TStreamId id = GetLastDirectoryStreamIdFromPathL( iFileStore->Root(), aPath );
	EnsurePathExistsL( aOutputPath );
	ExtractDirFromStreamIdL( id, aOutputPath );
	}

EXPORT_C void CSGEFile::ExtractTreeL( const TDesC& aPath, const TDesC& aOutputPath )
	{
	TStreamId id = GetLastDirectoryStreamIdFromPathL( iFileStore->Root(), aPath );

	if ( aOutputPath.LocateReverse( '\\' ) != aOutputPath.Length() - 1 )
		{
		RBuf buf;
		buf.CreateL( aOutputPath.Length() + 1 );
		buf.CleanupClosePushL();
		buf.Copy( aOutputPath );
		buf.Append( '\\' );
		
		ExtractDirFromTEntryL( NULL, NULL, buf, &id );

		CleanupStack::PopAndDestroy( &buf );
		}
	else
		{
		ExtractDirFromTEntryL( NULL, NULL, aOutputPath, &id );
		}
	}

EXPORT_C void CSGEFile::ExtractAllTreeL( const TDesC& aOutputPath )
	{
	ExtractTreeL( _L(""), aOutputPath );
	}

TStreamId CSGEFile::GetFileStreamIdFromPathL( const TDesC& aFileNameAndPath, TInt *aFileSize )
	{
	TStreamId dirId(0);
	
	TInt slash = aFileNameAndPath.LocateReverse( '\\' );
	if ( slash != KErrNotFound )
		{
		TPtrC ptr( aFileNameAndPath.Left( slash ) );
	
		dirId = GetLastDirectoryStreamIdFromPathL( iFileStore->Root(), ptr );

		slash++;
		}
	else
		{
		dirId = iFileStore->Root();
		}
	TPtrC ptrFileName( aFileNameAndPath.Right( aFileNameAndPath.Length() - slash ) );

	RHashMap2 mapDirs, mapFiles;
	RHashMapS mapSizes;	
	CleanupClosePushL( mapDirs );	
	CleanupClosePushL( mapFiles );	
	CleanupClosePushL( mapSizes );	

	CStreamDictionary* dictionary = GetDictionaryFromStreamIdLC( dirId );
	
	GetMapsFromDictionaryL( dictionary, mapDirs, mapFiles, mapSizes );
	
	TUid &uid = mapFiles.FindL( ptrFileName );
	
	if ( aFileSize )
		{
		*aFileSize = mapSizes.FindL( uid );
		}

	TStreamId fileId = dictionary->At( uid );
	
	CleanupStack::PopAndDestroy( dictionary );
	CleanupStack::PopAndDestroy( &mapSizes );
	CleanupStack::PopAndDestroy( &mapFiles );
	CleanupStack::PopAndDestroy( &mapDirs );
	
	return fileId;
	}

TBuf<64> CSGEFile::GetOneDirFromPath( TPtr& aPath )
	{
	TLex parse( aPath );

	if( parse.Peek() == '\\' )
		{
		parse.Get();
		}
	
	parse.Mark();
	do
		{
		parse.Inc();
		}
	while ( parse.Peek() != '\\' && !parse.Eos() );
	
	TBuf<64> ret = parse.MarkedToken();
	parse.Mark();
	aPath = parse.RemainderFromMark();
	return ret;
	}

void CSGEFile::GetArrayEntryFromStreamIdL( TStreamId aId, RArray< TEntry > &aArray )
	{
	RHashMap2 mapDirs, mapFiles;
	RHashMapS mapSizes;
	CleanupClosePushL( mapDirs );	
	CleanupClosePushL( mapFiles );	
	CleanupClosePushL( mapSizes );	

	GetMapsFromStreamIdL( aId, mapDirs, mapFiles, mapSizes );
	
	mapDirs.GetDataAsTEntry( aArray, KEntryAttDir );

	TInt i1 = aArray.Count();
	mapFiles.GetDataAsTEntry( aArray );
	for ( TInt i = i1; i < aArray.Count(); i++ )
		{
		aArray[i].iSize = *mapSizes.Find( aArray[i].iType[0] );
		}
	
	CleanupStack::PopAndDestroy( &mapSizes );
	CleanupStack::PopAndDestroy( &mapFiles );
	CleanupStack::PopAndDestroy( &mapDirs );
	}

void CSGEFile::GetMapsFromStreamIdL( TStreamId aId, RHashMap2 &mapDirs, RHashMap2 &mapFiles, RHashMapS &mapSizes )
	{
	CStreamDictionary* dictionary = GetDictionaryFromStreamIdLC( aId );
	
	GetMapsFromDictionaryL( dictionary, mapDirs, mapFiles, mapSizes );
	
	CleanupStack::PopAndDestroy( dictionary );
	}

void CSGEFile::GetMapsFromDictionaryL( CStreamDictionary *aDictionary, RHashMap2 &mapDirs, RHashMap2 &mapFiles, RHashMapS &mapSizes )
	{
	TStreamId id = aDictionary->At( TUid::Uid(1) );
	
	RStoreReadStream stream;
	stream.OpenLC( *iFileStore, id );
	stream >> mapFiles;
	stream >> mapDirs;
	stream >> mapSizes; 
	CleanupStack::PopAndDestroy(); // stream
	}

void CSGEFile::GetMapsFromDictionaryL( CStreamDictionary *aDictionary, RHashMap2 &mapDirs, RHashMap2 &mapFiles )
	{
	TStreamId id = aDictionary->At( TUid::Uid(1) );
	
	RStoreReadStream stream;
	stream.OpenLC( *iFileStore, id );
	stream >> mapFiles;
	stream >> mapDirs;
	CleanupStack::PopAndDestroy(); // stream
	}

CStreamDictionary* CSGEFile::GetDictionaryFromStreamIdLC( TStreamId aId )
	{
	CStreamDictionary* dictionary = CStreamDictionary::NewLC();

	RStoreReadStream stream;
	stream.OpenLC( *iFileStore, aId );
	stream >> *dictionary;
	CleanupStack::PopAndDestroy(); // stream
	
	return dictionary;
	}

void CSGEFile::GetDirTreeFromStreamIdL( TStreamId aId, RArray< TEntry > &aArray, TDesC& aPrevPath )
	{
	RHashMap2 mapDirs, mapFiles;

	CStreamDictionary* dictionary = GetDictionaryFromStreamIdLC( aId );
	CleanupClosePushL( mapDirs );	
	CleanupClosePushL( mapFiles );	

	GetMapsFromDictionaryL( dictionary, mapDirs, mapFiles );
	
	TInt startIdx = aArray.Count();
	mapDirs.GetDataAsTEntry( aArray, KEntryAttDir );
	mapFiles.GetDataAsTEntry( aArray );
	TInt stopIdx = aArray.Count();
	
	for ( TInt i = startIdx; i < stopIdx; i++ )
		{
		RBuf rbuf;
		rbuf.CreateL( KMaxFileName );
		rbuf.CleanupClosePushL();
		rbuf.Copy( aPrevPath );
		rbuf.Append( aArray[i].iName );

		if ( aArray[i].IsDir() )
			{
			rbuf.Append( '\\' );
			TStreamId id = dictionary->At( aArray[i].iType[0] );
			GetDirTreeFromStreamIdL( id , aArray, rbuf );
			}

		aArray[i].iName = rbuf;
		CleanupStack::PopAndDestroy( &rbuf );
		}
		
	CleanupStack::PopAndDestroy( &mapFiles );
	CleanupStack::PopAndDestroy( &mapDirs );
	CleanupStack::PopAndDestroy( dictionary ); 
	}


TStreamId CSGEFile::GetDirectoryStreamIdL( TStreamId aId, const TDesC& aDirName )
	{
	RHashMap2 mapDirs, mapFiles;
	RHashMapS mapSizes;

	CStreamDictionary* dictionary = GetDictionaryFromStreamIdLC( aId );
	CleanupClosePushL( mapDirs );	
	CleanupClosePushL( mapFiles );	
	CleanupClosePushL( mapSizes );	

	GetMapsFromDictionaryL( dictionary, mapDirs, mapFiles, mapSizes );

	TUid &uid = mapDirs.FindL( aDirName );
	
	TStreamId id = dictionary->At( uid );

	CleanupStack::PopAndDestroy( &mapSizes );
	CleanupStack::PopAndDestroy( &mapFiles );
	CleanupStack::PopAndDestroy( &mapDirs );
	CleanupStack::PopAndDestroy( dictionary ); 
	
	return id;
	}

TStreamId CSGEFile::GetLastDirectoryStreamIdFromPathL( TStreamId aId, const TDesC& aPath )
	{
	HBufC* path = aPath.AllocLC();
	TPtr pathPtr = path->Des();

	while ( ETrue )
		{
		TBuf<64> dir = GetOneDirFromPath( pathPtr );
		
		if( dir.Compare( KNullDesC() ) == 0 )
			{
			break;
			}
		
		aId = GetDirectoryStreamIdL( aId, dir );
		}
	
	CleanupStack::PopAndDestroy( path ); 
	
	return aId;
	}

void CSGEFile::ExtractFileFromStreamIdL( TStreamId aId, TInt aFileSize, const TDesC& aOutputFileNameAndPath )
	{
	RStoreReadStream stream;
	stream.OpenLC( *iFileStore, aId );

	RFile file;
	User::LeaveIfError( file.Replace( iFs, aOutputFileNameAndPath, EFileWrite ) );	
	CleanupClosePushL( file );
	
	RBuf8 buffile;

	if ( aFileSize > 100*1024 )
		{
		buffile.CreateL( 100*1024 );
		buffile.CleanupClosePushL();
		
		TInt cnt = 0; 
		while ( cnt + 100*1024 <= aFileSize )
			{
			stream.ReadL( buffile );
			file.Write( buffile );
			cnt += 100*1024;
			}
		TInt left = aFileSize - cnt;
		stream.ReadL( buffile, left );
		file.Write( buffile, left );
		CleanupStack::PopAndDestroy(&buffile);
		}
	else
		{
		buffile.CreateL( aFileSize );
		buffile.CleanupClosePushL();
		
		stream.ReadL( buffile );
		file.Write( buffile );
		CleanupStack::PopAndDestroy(&buffile);
		}

	CleanupStack::PopAndDestroy(&file);	
	CleanupStack::PopAndDestroy(&stream);
	}

void CSGEFile::ExtractDirFromStreamIdL( TStreamId aId, const TDesC& aOutputPath )
	{
	RArray< TEntry > array;
	CleanupClosePushL( array );
	
	GetArrayEntryFromStreamIdL( aId, array );
	CStreamDictionary* dictionary = GetDictionaryFromStreamIdLC( aId );
	
	for ( TInt i = 0; i < array.Count(); i++ )
		{
		if ( !array[i].IsDir() )
			{
			ExtractFileFromTEntryL( &array[i], dictionary, aOutputPath );
			}		
		}
	
	CleanupStack::PopAndDestroy( dictionary );
	CleanupStack::PopAndDestroy( &array );
	}

void CSGEFile::ExtractFileFromTEntryL( TEntry *aEntry, CStreamDictionary *aDictionary, const TDesC& aOutputPath )
	{
	RBuf buf;
	buf.CreateL( aOutputPath.Length() + aEntry->iName.Length() + 1 );
	buf.CleanupClosePushL();
	buf.Copy( aOutputPath );
	if ( aOutputPath.LocateReverse( '\\' ) != aOutputPath.Length() - 1 )
		{
		buf.Append( '\\' );
		}
	buf.Append( aEntry->iName );

	TStreamId fileId = aDictionary->At( aEntry->iType[0] );
	ExtractFileFromStreamIdL( fileId, aEntry->iSize, buf );

	CleanupStack::PopAndDestroy( &buf );
	}

void CSGEFile::ExtractDirFromTEntryL( TEntry *aEntry, CStreamDictionary *aDictionary, const TDesC& aOutputPath, TStreamId *aStartId )
	{
	RArray< TEntry > array;
	CleanupClosePushL( array );

	TStreamId id(0);
	if ( aDictionary == NULL && aEntry == NULL )
		{
		id = *aStartId;
		}
	else
		{
		id = aDictionary->At( aEntry->iType[0] );
		}
	
	GetArrayEntryFromStreamIdL( id, array );
	CStreamDictionary* dictionary = GetDictionaryFromStreamIdLC( id );
	
	for ( TInt i = 0; i < array.Count(); i++ )
		{
		if ( array[i].IsDir() )
			{
			RBuf buf;
			buf.CreateL( aOutputPath.Length() + array[i].iName.Length() + 1 );
			buf.CleanupClosePushL();
			buf.Copy( aOutputPath );
			buf.Append( array[i].iName );
			buf.Append( '\\' );
			EnsurePathExistsL( buf, EFalse );

			ExtractDirFromTEntryL( &array[i], dictionary, buf );

			CleanupStack::PopAndDestroy( &buf );
			}
		else
			{
			ExtractFileFromTEntryL( &array[i], dictionary, aOutputPath );
			}		
		}
	
	CleanupStack::PopAndDestroy( dictionary );
	CleanupStack::PopAndDestroy( &array );
	}

void CSGEFile::EnsurePathExistsL( const TDesC& aPath, TBool aAddSlash )
	{
	if ( aAddSlash && aPath.LocateReverse( '\\' ) != aPath.Length() - 1 )
		{
		RBuf buf;
		buf.CreateL( aPath.Length() + 1 );
		buf.CleanupClosePushL();
		buf.Copy( aPath );
		buf.Append( '\\' );
		BaflUtils::EnsurePathExistsL( iFs, buf );
		CleanupStack::PopAndDestroy( &buf );
		}
	else
		{
		BaflUtils::EnsurePathExistsL( iFs, aPath );
		}
	}

EXPORT_C const CSGEFileHeader* CSGEFile::GetHeader()
	{
	return iHeader;
	}

