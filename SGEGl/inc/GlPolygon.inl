/*
 ============================================================================
 Name		: GlPolygon.inl
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : TGlPolygon declaration
 ============================================================================
 */

#ifndef GLPOLYGON_INL
#define GLPOLYGON_INL

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <gles/gl.h>
#include <SGEGl/GlPolygon.h>

/**
 *  TGlPolygon
 * 
 */
template <class T> TGlPolygon<T>::TGlPolygon( GLenum aType ) : iType( GL_TRIANGLES ),
        iCullFaceEnabled( ETrue ), iCullFaceType( GL_BACK ), iOffsetFillEnabled( EFalse ),
        iOffsetFactor( 0 ), iOffsetUnits( 0 )
    {
    SetType( aType );
    }

template <class T> TGlPolygon<T>::TGlPolygon( GLenum aCullFaceType, GLenum aType ) : iType( GL_TRIANGLES ),
        iCullFaceEnabled( ETrue ), iCullFaceType( aCullFaceType ), iOffsetFillEnabled( EFalse ),
        iOffsetFactor( 0 ), iOffsetUnits( 0 )
    {
    SetType( aType );
    }

template <class T> GLenum TGlPolygon<T>::Type() const
    {
    return iType;
    }

template <class T> TBool TGlPolygon<T>::SetType( GLenum aType )
    {
    if ( aType == GL_TRIANGLES || aType == GL_TRIANGLE_STRIP || aType == GL_TRIANGLE_FAN )
        {
        iType = aType;
        return ETrue;
        }
    return EFalse;
    }

template <class T> void TGlPolygon<T>::SetCullFace( TBool aEnabled )
    {
    iCullFaceEnabled = aEnabled;
    }

template <class T> TBool TGlPolygon<T>::SetCullFaceType( GLenum aCullFaceType )
    {
    if ( aCullFaceType == GL_FRONT || aCullFaceType == GL_BACK || aCullFaceType == GL_FRONT_AND_BACK )
        {
        iCullFaceType = aCullFaceType;
        return ETrue;
        }
    return EFalse;
    }

template <class T> void TGlPolygon<T>::CullFaceFront()
    {
    iCullFaceType = GL_FRONT;
    }

template <class T> void TGlPolygon<T>::CullFaceBack()
    {
    iCullFaceType = GL_BACK;
    }

template <class T> void TGlPolygon<T>::CullFaceFrontAndBack()
    {
    iCullFaceType = GL_FRONT_AND_BACK;
    }

template <class T> void TGlPolygon<T>::SetOffsetFill( TBool aEnabled )
    {
    iOffsetFillEnabled = aEnabled;
    }

template <class T> void TGlPolygon<T>::SetOffsetFillParam( T aFactor, T aUnits )    
    {
    iOffsetFactor = aFactor;
    iOffsetUnits = aUnits;
    }


#endif // GLPOLYGON_INL
