/*
 ============================================================================
 Name		: SGEAudioStreamMMF.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEAudioStreamMMF declaration
 ============================================================================
 */

#ifndef SGEAUDIOSTREAMMMF_H
#define SGEAUDIOSTREAMMMF_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <mmf/server/sounddevice.h> 
#include "SGEAudioStreamBase.h" 

// CLASS DECLARATION

/**
 *  CSGEAudioStreamMMF
 * 
 */
NONSHARABLE_CLASS( CSGEAudioStreamMMF ) : public CSGEAudioStreamBase, MDevSoundObserver
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CSGEAudioStreamMMF();

    /**
     * Two-phased constructor.
     */
    static CSGEAudioStreamMMF* NewL( MSGEAudioStreamObserver &aObserver, TSGEAudioConfig &aConfig );

    /**
     * Two-phased constructor.
     */
    static CSGEAudioStreamMMF* NewLC( MSGEAudioStreamObserver &aObserver, TSGEAudioConfig &aConfig );
    
    void UpdateNow();

    TInt MaxVolume();
    
private:

    /**
     * Constructor for performing 1st stage construction
     */
    CSGEAudioStreamMMF( MSGEAudioStreamObserver &aObserver, TSGEAudioConfig &aConfig );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();

    void ConfigureAudioL();

    void Retry();

private: // from base class
    
    void RunL();

    TInt RunError( TInt aError );
    
private: // from MDevSoundObserver
    
    void InitializeComplete( TInt aError );
    
    void ToneFinished( TInt aError );
    
    void BufferToBeFilled( CMMFBuffer *aBuffer );
    
    void PlayError( TInt aError );
    
    void BufferToBeEmptied( CMMFBuffer *aBuffer );
    
    void RecordError( TInt aError );
    
    void ConvertError( TInt aError );
    
    void DeviceMessage( TUid aMessageType, const TDesC8 &aMsg );
    
    inline void SendEventToClient( const TMMFEvent& /*aEvent*/ ) { };
    
private: // data
    
    CMMFDevSound *iMMFDevSound;
    
    TUint32 iTickCount;

    };

#endif // SGEAUDIOSTREAMMMF_H
