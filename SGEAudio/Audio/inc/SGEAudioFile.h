/*
 ============================================================================
 Name		: SGEAudioFile.h
 Author	  : Michal Strug
 Version	 : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : TSGEAudioFile declaration
 ============================================================================
 */

#ifndef SGEAUDIOFILE_H
#define SGEAUDIOFILE_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <SGEAudio/SGEAudioConfig.h>


// CLASS DECLARATION


/**
 *  TSGEAudioFile
 * 
 * This class should be created on heap.
 *  
 */
class TSGEAudioFile
    {

public:

    IMPORT_C TSGEAudioFile( const TDesC& aFileName, TUint aId, 
                            TSGEAudioGroup aAudioGroup, TSGEAudioFileType aFileType );
    
public:
    
    TFileName iFileName;
    
    TUint iId;
    
    TSGEAudioGroup iAudioGroup;
    
    TSGEAudioFileType iFileType;
    
    };


#endif // SGEAUDIOFILE_H
