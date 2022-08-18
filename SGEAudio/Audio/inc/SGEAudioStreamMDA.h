/*
 ============================================================================
 Name		: SGEAudioStreamMDA.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEAudioStreamMDA declaration
 ============================================================================
 */

#ifndef SGEAUDIOSTREAMMDA_H
#define SGEAUDIOSTREAMMDA_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <MdaAudioOutputStream.h> 
#include "SGEAudioStreamBase.h"

// CLASS DECLARATION

/**
 *  CSGEAudioStreamMDA
 * 
 */
NONSHARABLE_CLASS( CSGEAudioStreamMDA ) : public CSGEAudioStreamBase, MMdaAudioOutputStreamCallback
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CSGEAudioStreamMDA();

    /**
     * Two-phased constructor.
     */
    static CSGEAudioStreamMDA* NewL( MSGEAudioStreamObserver &aObserver, TSGEAudioConfig &aConfig );

    /**
     * Two-phased constructor.
     */
    static CSGEAudioStreamMDA* NewLC( MSGEAudioStreamObserver &aObserver, TSGEAudioConfig &aConfig );
    
    void UpdateNow();

    TInt MaxVolume();
    
private:

    /**
     * Constructor for performing 1st stage construction
     */
    CSGEAudioStreamMDA( MSGEAudioStreamObserver &aObserver, TSGEAudioConfig &aConfig );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();

    void ConfigureAudioL();

    void WriteToStreamL();
    
private: // from MMdaAudioOutputStreamCallback
    
    void MaoscOpenComplete( TInt aError );
    
    void MaoscBufferCopied( TInt aError, const TDesC8 &aBuffer );

    void MaoscPlayComplete( TInt aError );
    
private: // data
    
    CMdaAudioOutputStream *iMdaAudioOutputStream;   // owned 

    };

#endif // SGEAUDIOSTREAMMDA_H
