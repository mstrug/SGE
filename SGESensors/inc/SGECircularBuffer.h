/*
 ============================================================================
 Name		: SGECircularBuffer.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGECircularBuffer declaration
 ============================================================================
 */

#ifndef SGECIRCULARBUFFER_H
#define SGECIRCULARBUFFER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

// CLASS DECLARATION

/**
 *  CSGECircularBuffer
 * 
 */
template<class T> class CSGECircularBuffer : public CBase
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    inline ~CSGECircularBuffer();

    /**
     * Two-phased constructor.
     */
    inline static CSGECircularBuffer<T>* NewL( TInt aBufferLength, T aFillValue = (T)0 );

    /**
     * Two-phased constructor.
     */
    inline static CSGECircularBuffer<T>* NewLC( TInt aBufferLength, T aFillValue = (T)0 );

    inline void AddNewValue( T aNewValue );
    
    inline T CurrentValue() const;
    
    inline void Reset();

    inline TInt UseLowPassFilter( TInt aDt, TInt aRc );

    inline void TurnOffLowPassFilter();
    
    inline void IsLowPassFilterUsed() const;
    
private:

    /**
     * Constructor for performing 1st stage construction
     */
    inline CSGECircularBuffer( TInt aBufferLength );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    inline void ConstructL( TInt aBufferLength, T aFillValue );

    inline void CalculateCurrentValue();

private: // data
    
    RArray< T > iData;
    
    T iCurrentValue;
    
    TInt iCurrentIdx;
    
    TInt iMaxIdx;

    TBool iUseLowPassFilter;
    
    TInt iDt;
    
    TInt iRc;
    
    };

#include <SGESensors/SGECircularBuffer.inl>

#endif // SGECIRCULARBUFFER_H
