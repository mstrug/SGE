/*
 ============================================================================
 Name		: SGECircularBuffer.inl
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGECircularBuffer implementation
 ============================================================================
 */

#ifndef SGECIRCULARBUFFER_INL
#define SGECIRCULARBUFFER_INL


#include <SGESensors/SGECircularBuffer.h>


const TInt KDefaultGranularity = 16;


// -----------------------------------------------------------------------------
// CSGECircularBuffer::CSGECircularBuffer()
// -----------------------------------------------------------------------------
//
template<class T> CSGECircularBuffer<T>::CSGECircularBuffer( TInt aBufferLength ) : iData( aBufferLength )
    {
    // No implementation required
    }

// -----------------------------------------------------------------------------
// CSGECircularBuffer::~CSGECircularBuffer()
// -----------------------------------------------------------------------------
//
template<class T> CSGECircularBuffer<T>::~CSGECircularBuffer()
    {
    iData.Close();
    }

// -----------------------------------------------------------------------------
// CSGECircularBuffer::NewLC()
// -----------------------------------------------------------------------------
//
template<class T> CSGECircularBuffer<T>* CSGECircularBuffer<T>::NewLC( TInt aBufferLength, T aFillValue )
    {
    CSGECircularBuffer<T> *self = new ( ELeave ) CSGECircularBuffer<T>( aBufferLength );
    CleanupStack::PushL( self );
    self->ConstructL( aBufferLength, aFillValue );
    return self;
    }

// -----------------------------------------------------------------------------
// CSGECircularBuffer::NewL()
// -----------------------------------------------------------------------------
//
template<class T> CSGECircularBuffer<T>* CSGECircularBuffer<T>::NewL( TInt aBufferLength, T aFillValue )
    {
    CSGECircularBuffer<T> *self = CSGECircularBuffer<T>::NewLC( aBufferLength, aFillValue );
    CleanupStack::Pop(); // self;
    return self;
    }

// -----------------------------------------------------------------------------
// CSGECircularBuffer::ConstructL()
// -----------------------------------------------------------------------------
//
template<class T> void CSGECircularBuffer<T>::ConstructL( TInt aBufferLength, T aFillValue )
    {
    ASSERT( sizeof( T ) >= 4 );
    ASSERT( aBufferLength > 0 );
    
    iData.ReserveL( aBufferLength );
    for ( TInt i = 0; i < aBufferLength; i++ )
        {
        iData.AppendL( aFillValue );
        }
    if ( aFillValue != 0 )
        {
        iMaxIdx = aBufferLength - 1;
        CalculateCurrentValue();
        }
    }

// -----------------------------------------------------------------------------
// CSGECircularBuffer::AddNewValue()
// -----------------------------------------------------------------------------
//
template<class T> void CSGECircularBuffer<T>::AddNewValue( T aNewValue )
    {
    if ( iUseLowPassFilter )
        {
        TInt idx;
        if ( iCurrentIdx == 0 )
            {
            idx = ( iMaxIdx == 0 ? 0 : iMaxIdx - 1 );
            }
        else
            {
            idx = iCurrentIdx - 1;
            }
        iData[ iCurrentIdx++ ] = iData[ idx ] + ( iDt * ( aNewValue - iData[ idx ] ) ) / ( iRc + iDt );
        }
    else
        {
        iData[ iCurrentIdx++ ] = aNewValue;    
        }
    
    if ( iCurrentIdx >= iData.Count() )
        {
        iCurrentIdx = 0;
        }
    if ( iCurrentIdx >= iMaxIdx )
        {
        iMaxIdx = iCurrentIdx + 1;
        }

    CalculateCurrentValue();
    }

// -----------------------------------------------------------------------------
// CSGECircularBuffer::CurrentValue()
// -----------------------------------------------------------------------------
//
template<class T> T CSGECircularBuffer<T>::CurrentValue() const
    {
    return iCurrentValue;
    }

// -----------------------------------------------------------------------------
// CSGECircularBuffer::Reset()
// -----------------------------------------------------------------------------
//
template<class T> void CSGECircularBuffer<T>::Reset()
    {
    iMaxIdx = 0;
    }

// -----------------------------------------------------------------------------
// CSGECircularBuffer::CalculateCurrentValue()
// -----------------------------------------------------------------------------
//
template<class T> void CSGECircularBuffer<T>::CalculateCurrentValue()
    {
    ASSERT( iMaxIdx > 0 );
    
    T sum = 0;
    for ( TInt i = 0; i < iMaxIdx; i++ )
        {
        sum += iData[ i ];
        }

    iCurrentValue = sum / iMaxIdx;
    }

// -----------------------------------------------------------------------------
// CSGECircularBuffer::UseLowPassFilter()
// -----------------------------------------------------------------------------
//
template<class T> TInt CSGECircularBuffer<T>::UseLowPassFilter( TInt aDt, TInt aRc )
    {
    if ( aDt + aRc == 0 )
        {
        iUseLowPassFilter = EFalse;
        return KErrArgument;
        }
    
    iDt = aDt;
    iRc = aRc;
    
    iUseLowPassFilter = ETrue;
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CSGECircularBuffer::TurnOffLowPassFilter()
// -----------------------------------------------------------------------------
//
template<class T> void CSGECircularBuffer<T>::TurnOffLowPassFilter()
    {
    iUseLowPassFilter = EFalse;
    }

// -----------------------------------------------------------------------------
// CSGECircularBuffer::IsLowPassFilterUsed()
// -----------------------------------------------------------------------------
//
template<class T> void CSGECircularBuffer<T>::IsLowPassFilterUsed() const
    {
    return iUseLowPassFilter;
    }


#endif

// End of file
