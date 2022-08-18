/*
 ============================================================================
 Name		: SGEFilePack.cpp
 Author	  : Michal Strug
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : Exe source file
 ============================================================================
 */

//  Include Files  

#include "SGEFilePack.h"
#include <e32base.h>
#include <e32std.h>
#include <e32cons.h>			// Console
#include <SGEFile.h>
#include <SGEFileHeader.h>

//  Constants

_LIT(KTextConsoleTitle, "Console");
_LIT(KTextFailed, " failed, leave code = %d");
_LIT(KTextPressAnyKey, " [press any key]\n");

//  Global Variables

LOCAL_D CConsoleBase* console; // write all messages to this


//  Local Functions
void CreateSGEFileL( const TDesC& aSourceDir, const TDesC& aDestinationFile, 
					 const TDesC& aVersionNumber, const TDesC& aTypeNumber,
					 const TDesC& aDescription, const TDesC& aAuthor, 
					 const TDesC& aContact, const TDesC& aDefaultFile )
	{
	CSGEFileHeader *hdr = CSGEFileHeader::NewLC();

	console->Write( _L("CreateSGEFileL:\n") );
	console->Write( aSourceDir );
	console->Write( _L("\n") );
	console->Write( aDestinationFile );
	if ( aVersionNumber.Compare( KNullDesC() ) != 0 )
		{
		console->Write( _L("\nVersion: ") );
		console->Write( aVersionNumber );
		TLex lex( aVersionNumber );
		lex.Val( hdr->iVersion, EHex );
		}
	if ( aTypeNumber.Compare( KNullDesC() ) != 0 )
		{
		console->Write( _L("\nType: ") );
		console->Write( aTypeNumber );
		TLex lex( aTypeNumber );
		lex.Val( hdr->iType, EHex );
		}
	if ( aDescription.Compare( KNullDesC() ) != 0 )
		{
		console->Write( _L("\nDescription: ") );
		console->Write( aDescription );
		hdr->iDescription.Copy( aDescription );
		}
	if ( aAuthor.Compare( KNullDesC() ) != 0 )
		{
		console->Write( _L("\nAuthor: ") );
		console->Write( aAuthor );
		hdr->iAuthor.Copy( aAuthor );
		}
	if ( aContact.Compare( KNullDesC() ) != 0 )
		{
		console->Write( _L("\nContact: ") );
		console->Write( aContact );
		hdr->iContact.Copy( aContact );
		}
	if ( aDefaultFile.Compare( KNullDesC() ) != 0 )
		{
		console->Write( _L("\nDefault file: ") );
		console->Write( aDefaultFile );
		hdr->iDefaultFile.Copy( aDefaultFile );
		}
	console->Write( _L("\n\n") );
	
	
	CSGEFile::CreateL( aDestinationFile, aSourceDir, hdr );
	
	CleanupStack::PopAndDestroy( hdr );
	}

void ExtractSGEFileL( const TDesC& aSourceFile, const TDesC& aDestinationDir )
	{
	console->Write( _L("ExtractSGEFileL:\n") );
	console->Write( aSourceFile );
	console->Write( _L("\n") );
	console->Write( aDestinationDir );
	console->Write( _L("\n\n") );
	
	CSGEFile *file = CSGEFile::NewLC(); 
	file->LoadL( aSourceFile );

	file->ExtractAllTreeL( aDestinationDir );
	
	CleanupStack::PopAndDestroy( file );
	}

void DirSGEFileL( const TDesC& aSourceFile, const TDesC& aOutputFile )
	{
	TBool saveToFile = EFalse;
	console->Write( _L("DirSGEFileL:\n") );
	console->Write( aSourceFile );
	if ( aOutputFile.CompareF( KNullDesC() ) != 0 )
		{
		saveToFile = ETrue;
		console->Write( _L("\n") );
		console->Write( aOutputFile );
		}
	console->Write( _L("\n\n") );
	
	RFs fs;
	RFile oFile;
	if ( saveToFile )
		{
		fs.Connect();
		CleanupClosePushL( fs );
		oFile.Replace( fs, aOutputFile, EFileWrite );
		CleanupClosePushL( oFile );
		}
	
	CSGEFile *file = CSGEFile::NewLC(); 
	file->LoadL( aSourceFile );

	const CSGEFileHeader* hdr = file->GetHeader();
	if ( saveToFile )
		{
		TBuf8<256> buf;
		
		buf.Copy( aSourceFile );
		oFile.Write( _L8("File: ") );
		oFile.Write( buf );
		oFile.Write( _L8("\n\nHeader:\n\n") );

		buf.Zero();
		buf.AppendNum( hdr->iInternalVersion, EHex );
		oFile.Write( _L8("InternalVersion: ") );
		oFile.Write( buf );
		oFile.Write( _L8("\n") );

		buf.Zero();
		buf.AppendNum( hdr->iInternalType, EHex );
		oFile.Write( _L8("InternalType: ") );
		oFile.Write( buf );
		oFile.Write( _L8("\n") );

		buf.Zero();
		buf.AppendNum( hdr->iVersion, EHex );
		oFile.Write( _L8("FileVersion: ") );
		oFile.Write( buf );
		oFile.Write( _L8("\n") );

		buf.Zero();
		buf.AppendNum( hdr->iType, EHex );
		oFile.Write( _L8("Type: ") );
		oFile.Write( buf );
		oFile.Write( _L8("\n") );

		buf.Copy( hdr->iAuthor );
		oFile.Write( _L8("Author: ") );
		oFile.Write( buf );
		oFile.Write( _L8("\n") );

		buf.Copy( hdr->iDescription );
		oFile.Write( _L8("Description: ") );
		oFile.Write( buf );
		oFile.Write( _L8("\n") );

		buf.Copy( hdr->iContact );
		oFile.Write( _L8("Contact: ") );
		oFile.Write( buf );
		oFile.Write( _L8("\n") );

		buf.Copy( hdr->iDefaultFile );
		oFile.Write( _L8("DefaultFile: ") );
		oFile.Write( buf );
		oFile.Write( _L8("\n") );

		oFile.Write( _L8("\n\nContent:\n\n") );
		}
	else
		{
		console->Write( _L("File: ") );
		console->Write( aSourceFile );

		console->Write( _L("\n\nHeader:\n\n") );

		TBuf<32> buf;

		buf.Zero();
		buf.AppendNum( hdr->iInternalVersion, EHex );
		console->Write( _L("InternalVersion: ") );
		console->Write( buf );
		console->Write( _L("\n") );

		buf.Zero();
		buf.AppendNum( hdr->iInternalType, EHex );
		console->Write( _L("InternalType: ") );
		console->Write( buf );
		console->Write( _L("\n") );

		buf.Zero();
		buf.AppendNum( hdr->iVersion, EHex );
		console->Write( _L("Version: ") );
		console->Write( buf );
		console->Write( _L("\n") );

		buf.Zero();
		buf.AppendNum( hdr->iType, EHex );
		console->Write( _L("Type: ") );
		console->Write( buf );
		console->Write( _L("\n") );
		
		console->Write( _L("Author: ") );
		console->Write( hdr->iAuthor );
		console->Write( _L("\n") );
		
		console->Write( _L("Description: ") );
		console->Write( hdr->iDescription );
		console->Write( _L("\n") );
		
		console->Write( _L("Contact: ") );
		console->Write( hdr->iContact );
		console->Write( _L("\n") );
		
		console->Write( _L("DefaultFile: ") );
		console->Write( hdr->iDefaultFile );
		console->Write( _L("\n") );

		console->Write( _L("\n\nContent:\n\n") );
		}
	
	RArray< TEntry > array = file->DirTreeL( _L("") );
	CleanupClosePushL( array );
	for ( TInt i = 0; i < array.Count(); i++ )
		{
		if ( saveToFile )
			{
			RBuf8 buf;
			buf.CreateL( array[ i ].iName.Length() + 5 );
			buf.Copy( array[ i ].iName );
			buf.Append( _L8("\n") );
			oFile.Write( buf );
			buf.Close();
			}
		else
			{
			console->Write( array[ i ].iName );
			console->Write( _L("\n") );
			}
		}
	CleanupStack::PopAndDestroy( &array );	
	CleanupStack::PopAndDestroy( file );

	if ( saveToFile )
		{
		CleanupStack::PopAndDestroy( &oFile );	
		CleanupStack::PopAndDestroy( &fs );
		}
	}

void PrintInfo()
	{
	console->Write( _L("SGEFilePack\nv0.1\n\nUsage:\n"));
	console->Write( _L(" Create SGE file:\n  sgefilepack -c dir -o outputfile [-v fileversionhexnumber] [-t typehexnumber] [-d description] [-a author] [-m contact] [-f defaultfile]\n\n"));
	console->Write( _L(" Extract SGE file:\n  sgefilepack -e sgefile -d outputdir\n\n"));
	console->Write( _L(" List SGE file:\n  sgefilepack -l sgefile\n  sgefilepack -l sgefile -o outputfile\n\n"));
	}

TPtrC ExtractParameterL( const TDesC& aStr, const TDesC& aParam )
	{
	TInt idx = aStr.FindF( aParam );
	if ( idx > KErrNotFound )
		{
		TPtrC param( aStr.Right( aStr.Length() - ( idx + aParam.Length() ) ) );
		TInt j = -1;
		for ( TInt i = 0; i < param.Length(); i++ )
			{
			if ( param[ i ] == _L(" ")[ 0 ] )
				{
				j = i;
				}
			else
				{
				break;
				}
			}
		if ( j > -1 )
			{
			param.Set( param.Right( param.Length() - ++j ) );
			}
		TInt idxSpace = param.FindF( _L(" ") );
		if ( param[ 0 ] == '\"' )
			{
			param.Set( param.Right( param.Length() - 1 ) );
			idxSpace = param.FindF( _L("\"") );
			if ( idxSpace == KErrNotFound )
				{
				idxSpace = param.FindF( _L(" ") );
				}
			}
		if ( idxSpace > KErrNotFound )
			{
			return TPtrC( param.Left( idxSpace ) );
			}
		else
			{
			return param;
			}
		}
	return KNullDesC();
	}

void ParseL()
	{
	if ( User::CommandLineLength() < 6 )
		{
		User::Leave( KErrNotFound );
		}
	RBuf param;
	param.CreateL( User::CommandLineLength() );
	param.CleanupClosePushL();
	User::CommandLine( param );
	
	TPtrC paramC( ExtractParameterL( param, _L("-c ") ) );
	TPtrC paramE( ExtractParameterL( param, _L("-e ") ) );
	TPtrC paramL( ExtractParameterL( param, _L("-l ") ) );
	if ( paramC.Compare( KNullDesC() ) != 0 )
		{
		TPtrC paramO( ExtractParameterL( param, _L("-o ") ) );
		if ( paramO.Compare( KNullDesC() ) != 0 )
			{
			TPtrC paramV( ExtractParameterL( param, _L("-v ") ) );
			TPtrC paramT( ExtractParameterL( param, _L("-t ") ) );
			TPtrC paramD( ExtractParameterL( param, _L("-d ") ) );
			TPtrC paramA( ExtractParameterL( param, _L("-a ") ) );
			TPtrC paramM( ExtractParameterL( param, _L("-m ") ) );
			TPtrC paramF( ExtractParameterL( param, _L("-f ") ) );
			CreateSGEFileL( paramC, paramO, paramV, paramT, paramD, paramA, paramM, paramF );
			}
		else
			{
			User::Leave( KErrNotFound );
			}
		}
	else if ( paramE.Compare( KNullDesC() ) != 0 )
		{
		TPtrC paramD( ExtractParameterL( param, _L("-d ") ) );
		if ( paramD.Compare( KNullDesC() ) != 0 )
			{			
			ExtractSGEFileL( paramE, paramD );
			}
		else
			{
			User::Leave( KErrNotFound );
			}
		}
	else if ( paramL.Compare( KNullDesC() ) != 0 )
		{
		TPtrC paramO( ExtractParameterL( param, _L("-o ") ) );
		DirSGEFileL( paramL, paramO );
		}
	else
		{
		User::Leave( KErrNotFound );
		}

	CleanupStack::PopAndDestroy( &param );
	}

LOCAL_C void MainL()
	{
	console->Write(_L("Parsing parameters\n"));

	TRAPD( err, ParseL() );
	if ( err != KErrNone )
		{
		PrintInfo();
		return;
		}
	
	console->Write(_L("End\n"));
	}

LOCAL_C void DoStartL()
	{
	// Create active scheduler (to run active objects)
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler();
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);

	MainL();

	// Delete active scheduler
	CleanupStack::PopAndDestroy(scheduler);
	}

//  Global Functions

GLDEF_C TInt E32Main()
	{
	// Create cleanup stack
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();

	// Create output console
	TRAPD(createError, console = Console::NewL(KTextConsoleTitle, TSize(
			KConsFullScreen, KConsFullScreen)));
	if (createError)
		return createError;

	// Run application code inside TRAP harness, wait keypress when terminated
	TRAPD(mainError, DoStartL());
	if (mainError)
		console->Printf(KTextFailed, mainError);
	console->Printf(KTextPressAnyKey);
	console->Getch();

	delete console;
	delete cleanup;
	__UHEAP_MARKEND;
	return KErrNone;
	}

