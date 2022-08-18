/*
 ============================================================================
 Name		: SGEAudioSample.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEAudioSample implementation
 ============================================================================
 */

#include "SGEAudioSample.h"

const TUint KByte = 8;

CSGEAudioSample::CSGEAudioSample( TUint aId, TUint aBufferDataTypeSize, TUint aDataLength )
    : iId( aId ), iBps( KByte * aBufferDataTypeSize ), iDataTypeSize( aBufferDataTypeSize ),
      iDataLength( aDataLength )      
    {
    // No implementation required
    }

CSGEAudioSample::~CSGEAudioSample()
    {
    User::Free( iData );
    }

CSGEAudioSample* CSGEAudioSample::NewLC( TUint aId, TUint aBufferDataTypeSize, TUint aDataLength )
    {
    CSGEAudioSample *self = new (ELeave) CSGEAudioSample( aId, aBufferDataTypeSize, aDataLength );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

CSGEAudioSample* CSGEAudioSample::NewL( TUint aId, TUint aBufferDataTypeSize, TUint aDataLength )
    {
    CSGEAudioSample *self = CSGEAudioSample::NewLC( aId, aBufferDataTypeSize, aDataLength );
    CleanupStack::Pop(); // self;
    return self;
    }

void CSGEAudioSample::ConstructL()
    {
    iData = User::AllocZL( DataSize() );
    }

TUint CSGEAudioSample::DataSize()
    {
    return iDataTypeSize * iDataLength;
    }

const TPtrC8& CSGEAudioSample::Data()
    {
    iDataPtr.Set( (const TUint8*) iData, DataSize() );
    return iDataPtr;
    }

void CSGEAudioSample::SetBufferL( const TAny *aBuffer )
    {
    User::Free( iData );
    iData = User::AllocZL( DataSize() );
    Mem::Copy( iData, aBuffer, DataSize() );
    }

void CSGEAudioSample::AdoptBufferSizeL( TUint aNewSizeBytes )
    {
    iDataLength = aNewSizeBytes / iDataTypeSize;
    iData = User::ReAllocL( iData, aNewSizeBytes );
    }

void CSGEAudioSample::ResampleL( TUint aFrequency )
    {
    if ( iFrequency == aFrequency )
        {
        return;
        }
    // todo: resample
    
//    if ( iFrequency < aFrequency )
//        {
//        const TInt KMicrosecond = 1000000;
//        TInt duration = iDataLength * KMicrosecond / iFrequency;   // in microseconds
//        TInt duration2 = iDataLength * KMicrosecond / aFrequency;   // in microseconds
//        TInt delta = ( duration - duration2 ) / KMicrosecond;
//    
//        ASSERT( delta > 0 );
//        
//        iData = User::ReAllocL( iData, ( iDataLength + delta ) * iDataTypeSize, RAllocator::EAllowMoveOnShrink );
//        
//        iDataLength += delta;
//        
//        TInt step1 = aFrequency / iFrequency - 1;
//        TInt step2 = 10 * aFrequency / iFrequency;
//        step2 -= ( step2 / 10 ) * 10;
//        
//        // todo: fix for stereo
//        if ( iBps == 8 )    // 1 byte per sample
//            {
//            TInt8 *buf = (TInt8*) iData;
//            }
//        else if ( iBps == 16 )    // 2 bytes per sample
//            {
//            TInt8 *buf = (TInt16*) iData;
//            }
//        }
//    else if ( iFrequency > aFrequency )
//        {
//        const TInt KMicrosecond = 1000000;
//        TInt duration = iDataLength * KMicrosecond / iFrequency;   // in microseconds
//        TInt duration2 = iDataLength * KMicrosecond / aFrequency;   // in microseconds
//        TInt delta = ( duration - duration2 ) / KMicrosecond;
//    
//        ASSERT( delta > 0 );
//        
//        iData = User::ReAllocL( iData, ( iDataLength + delta ) * iDataTypeSize, RAllocator::EAllowMoveOnShrink );
//        
//        iDataLength += delta;
//        }
    }

void CSGEAudioSample::ChangeChannelsL( TSGEAudioChannels aChannels )
    {
    if ( aChannels == iChannels )
        {
        return;
        }

    if ( aChannels == ESGEAudioChannelsMono )
	{ // stereo sample -> mono
	if ( iBps == 16 )
	    {
	    TInt16 *data = (TInt16*) User::AllocZL( iDataLength / 2 * sizeof( TInt16 ) );

            for ( TUint i = 0; i < iDataLength; i++ )
		{
		TInt32 val_l = (((TInt16*) iData)[ i++ ]);
		TInt32 val_r = (((TInt16*) iData)[ i ]);
		data[ i / 2 ] = ( val_l + val_r ) / 2;
		}

	    User::Free( iData );
            iData = (TAny*) data;
	    iDataLength /= 2;
	    iChannels = ESGEAudioChannelsMono;
	    }
	else if ( iBps == 8 )
	    {
	    TInt8 *data = (TInt8*) User::AllocZL( iDataLength / 2 * sizeof( TInt8 ) );

            for ( TUint i = 0; i < iDataLength; i++ )
		{
		TInt16 val_l = (((TInt8*) iData)[ i++ ]);
		TInt16 val_r = (((TInt8*) iData)[ i ]);
		data[ i / 2 ] = ( val_l + val_r ) / 2;
		}

	    User::Free( iData );
            iData = (TAny*) data;
	    iDataLength /= 2;
	    iChannels = ESGEAudioChannelsMono;
	    }
	}
    else if ( aChannels == ESGEAudioChannelsStereo )
	{ // mono sample -> stereo
	if ( iBps == 16 )
	    {
	    TInt16 *data = (TInt16*) User::AllocZL( iDataLength * 2 * sizeof( TInt16 ) );

            for ( TUint i = 0; i < iDataLength; i++ )
		{
		data[ i * 2 ] = (((TInt16*) iData)[ i ]);
		data[ i * 2 + 1 ] = (((TInt16*) iData)[ i ]);
		}

	    User::Free( iData );
            iData = (TAny*) data;
	    iDataLength *= 2;
	    iChannels = ESGEAudioChannelsStereo;
	    }
	else if ( iBps == 8 )
	    {
	    TInt8 *data = (TInt8*) User::AllocZL( iDataLength * 2 * sizeof( TInt8 ) );

            for ( TUint i = 0; i < iDataLength; i++ )
		{
		data[ i * 2 ] = (((TInt8*) iData)[ i ]);
		data[ i * 2 + 1 ] = (((TInt8*) iData)[ i ]);
		}

	    User::Free( iData );
            iData = (TAny*) data;
	    iDataLength *= 2;
	    iChannels = ESGEAudioChannelsStereo;
	    }
	}
    }

void CSGEAudioSample::ChangeBpsL( TUint8 aBps, TUint8 aShift )
    {
    if ( aBps == iBps )
        {
        return;
        }

    if ( aBps / iBps == 2 )
        { // change 8 -> 16
        TInt16 *data = (TInt16*) User::AllocZL( iDataLength * sizeof( TInt16 ) );
      
        for ( TUint i = 0; i < iDataLength; i++ )
            {
            data[ i ] = ( ((TUint8*) iData)[ i ] - 0x80 ) << aShift;
            }

        User::Free( iData );
        iData = (TAny*) data;
        iDataTypeSize = sizeof( TInt16 );
        iBps = iDataTypeSize * KByte;
        }
    else if ( iBps / aBps == 2 )
        { // change 16 -> 8
        TInt8 *data = (TInt8*) User::AllocZL( iDataLength * sizeof( TInt8 ) );
      
        for ( TUint i = 0; i < iDataLength; i++ )
            {
            TInt32 val = (((TInt16*) iData)[ i ]);
            data[ i ] = val / 0x101;    // 0xffff/0xff
            }
    
        User::Free( iData );
        iData = (TAny*) data;
        iDataTypeSize = sizeof( TInt8 );
        iBps = iDataTypeSize * KByte;
        }
    }


