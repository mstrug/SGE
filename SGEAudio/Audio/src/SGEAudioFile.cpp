/*
 ============================================================================
 Name		: SGEAudioFile.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : TSGEAudioFile implementation
 ============================================================================
 */

#include "SGEAudioFile.h"

EXPORT_C TSGEAudioFile::TSGEAudioFile( const TDesC& aFileName, TUint aId, TSGEAudioGroup aAudioGroup, 
                              TSGEAudioFileType aFileType )
    : iId( aId ), iAudioGroup( aAudioGroup ), iFileType( aFileType )
    {
    iFileName.Copy( aFileName );
    }


