/*
 ============================================================================
 Name		: SGEAudioFileLoader.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEAudioFileLoader declaration
 ============================================================================
 */

#ifndef SGEAUDIOFILELOADER_H
#define SGEAUDIOFILELOADER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <SGEFile/SGEFile.h>
#include "SGEAudioSample.h"
#include "SGEAudioConfig.h"
#include "SGEAudioFile.h"
#include "SGEAudioFileDecoder.h"

// CLASS DECLARATION


/**
 *  MSGEAudioFileLoaderObserver
 * 
 */
class MSGEAudioFileLoaderObserver
    {
public:

    virtual void AudioFileLoaded( CSGEAudioSample *aSoundSample, TUint aSampleId, TInt aError ) = 0;

    virtual void AudioFileListEmpty() = 0;

    };


/**
 *  CSGEAudioFileLoader
 * 
 */
NONSHARABLE_CLASS( CSGEAudioFileLoader ) : public CTimer, public MSGEAudioFileDecoderObserver
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CSGEAudioFileLoader();

    /**
     * Two-phased constructor.
     */
    static CSGEAudioFileLoader* NewL( MSGEAudioFileLoaderObserver &aObserver, RFs &aFs, 
                                      TSGEAudioConfig &aConfig );

    /**
     * Two-phased constructor.
     */
    static CSGEAudioFileLoader* NewLC( MSGEAudioFileLoaderObserver &aObserver, RFs &aFs, 
                                       TSGEAudioConfig &aConfig );

    TInt LoadFile( TSGEAudioFile aFile );

    TInt LoadFile( const TDesC& aSGEFilePath, TSGEAudioFile aFile );

    void RemoveFile( TUint aAudioSampleId );

    void RemoveFileAll();

    /*
     * Starts automatically loading all files one by one.
     */
    void StartLoadingFilesL();
    
    TBool AllFilesLoaded();
    
private:

    /**
     * Constructor for performing 1st stage construction
     */
    CSGEAudioFileLoader( MSGEAudioFileLoaderObserver &aObserver, RFs &aFs, TSGEAudioConfig &aConfig );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();
    
    void RemoveFileFromList( TInt aIdx );
    
private:    // from CTimer  
    
    void RunL();
    
    TInt RunError( TInt aError );

private:    // from MSGEAudioFileDecoderObserver
    
    void AudioFileDecodingFinished( CSGEAudioSample *aSoundSample, TInt aError );
    
private:
    
    class TSGEAudioFileInternal : public TSGEAudioFile
        {

    public:
        
        TSGEAudioFileInternal( TSGEAudioFile aFile );
        
        TSGEAudioFileInternal( TSGEAudioFile aFile, const TDesC& aSGEFilePath );
        
    public:
        
        TFileName iSGEFilePath;
        
        };

private:
    
    MSGEAudioFileLoaderObserver &iObserver;
    
    RFs &iFs;
    
    TSGEAudioConfig &iConfig; 
    
    RPointerArray< TSGEAudioFileInternal > iFileToLoadList;

    RBuf8 iFileData;    // owned

    CSGEAudioFileDecoder *iCurrentAudioFileDecoder; // owned
    
    };

#endif // SGEAUDIOFILELOADER_H
