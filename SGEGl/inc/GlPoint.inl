/*
 ============================================================================
 Name		: GlPoint.inl
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : TGlPoint declaration
 ============================================================================
 */

#ifndef GLPOINT_INL
#define GLPOINT_INL

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <gles/gl.h>
#include <SGEGl/GlPoint.h>


/*
 *  TGlPoint
 */
template <class T> TGlPoint<T>::TGlPoint() : iPointSmooth( ETrue )
    {
    }

template <class T> TGlPoint<T>::TGlPoint( T aSize ) : iSize( aSize ), iPointSmooth( ETrue )
    {
    }

template <class T> TGlPoint<T>::TGlPoint( T aSize, TBool aAntialiased ) : iSize( aSize ), iPointSmooth( aAntialiased )
    {
    }

template <class T> GLenum TGlPoint<T>::Type() const
    {
    return GL_POINTS;
    }

template <class T> TBool TGlPoint<T>::SetSize( T aSize )
    {
    if ( aSize >= 0 )
        {
        iSize = aSize;
        return ETrue;
        }
    return EFalse;
    }

template <class T> void TGlPoint<T>::SetAntialiased( TBool aAntialiased )
    {
    iPointSmooth = aAntialiased;
    }

template <class T> void TGlPoint<T>::SetPointSprites( TBool aPointSprites )
    {
    iPointSprites = aPointSprites;    
    }

template <class T> void TGlPoint<T>::SetCoordReplace( TBool aReplace )
    {
    iCoordReplace = aReplace;
    }

template <class T> void TGlPoint<T>::SetPointSizeMin( T aValue )
    {
    iPointSizeMin = aValue;
    }

template <class T> void TGlPoint<T>::SetPointSizeMax( T aValue )
    {
    iPointSizeMax = aValue;
    }

template <class T> void TGlPoint<T>::SetDistanceAttenuation( T aValue0, T aValue1, T aValue2 )
    {
    iPointDistanceAttenuation[ 0 ] = aValue0;
    iPointDistanceAttenuation[ 1 ] = aValue1;
    iPointDistanceAttenuation[ 2 ] = aValue2;
    }

template <class T> void TGlPoint<T>::SetPointFadeThresholdSize( T aValue )
    {
    iPointFadeThresholdSize = aValue;
    }



#endif // GLPOINT_H
