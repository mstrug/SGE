/*
 ============================================================================
 Name		: SGEAudioSample.h
 Author	  : Michal Strug
 Version	 : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEAudioSample declaration
 ============================================================================
 */

#ifndef SGEAUDIOSAMPLE_H
#define SGEAUDIOSAMPLE_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <SGEAudio/SGEAudioConfig.h>


// CLASS DECLARATION

const TUint8 KDefaultRescaleShift = 6;

/**
 *  CSGEAudioSample
 * 
 */
NONSHARABLE_CLASS( CSGEAudioSample ) : public CBase
    {

public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CSGEAudioSample();

    /**
     * Two-phased constructor.
     */
    static CSGEAudioSample* NewL( TUint aId, TUint aBufferDataTypeSize, TUint aDataLength );

    /**
     * Two-phased constructor.
     */
    static CSGEAudioSample* NewLC( TUint aId, TUint aBufferDataTypeSize, TUint aDataLength );

    TUint DataSize();
    
    const TPtrC8& Data();

    void SetBufferL( const TAny *aBuffer );
    
    void ResampleL( TUint aFrequency );

    void ChangeChannelsL( TSGEAudioChannels aChannels );

    void ChangeBpsL( TUint8 aBps, TUint8 aShift = KDefaultRescaleShift );
    
    // reallocates iData buffer to specified size in bytes
    void AdoptBufferSizeL( TUint aNewSizeBytes );
    
private:

    /**
     * Constructor for performing 1st stage construction
     */
    CSGEAudioSample( TUint aId, TUint aBufferDataTypeSize, TUint aDataLength );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();

public: // data

    TUint iId;
    
    TUint iFrequency;

    TUint8 iBps;
    
    TUint8 iChannels;
    
    TUint16 iDataTypeSize;
    
    TAny *iData;    // owned

    // number of iDataTypeSize in iData variable
    TUint iDataLength;
    
    TSGEAudioGroup iAudioGroup;
    
    TPtrC8 iDataPtr;
    
    };



#endif // SGEAUDIOSAMPLE_H
