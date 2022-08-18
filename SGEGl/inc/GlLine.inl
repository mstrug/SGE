/*
 ============================================================================
 Name		: GlLine.inl
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : TGlLine declaration
 ============================================================================
 */

#ifndef GLLINE_INL
#define GLLINE_INL

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <gles/gl.h>
#include <SGEGl/GlLine.h>

/*
 *  TGlLine
 */
template <class T> TGlLine<T>::TGlLine( GLenum aType ) : iType( GL_LINES ), iLineSmooth( ETrue )
    {
    SetType( aType );
    }

template <class T> TGlLine<T>::TGlLine( T aWidth, GLenum aType ) : iType( GL_LINES ), iLineSmooth( ETrue )
    {
    SetWidth( aWidth );
    SetType( aType );
    }

template <class T> TGlLine<T>::TGlLine( T aWidth, TBool aAntialiased, GLenum aType ) : iLineSmooth( aAntialiased )
    {
    SetWidth( aWidth );
    SetType( aType );
    }

template <class T> GLenum TGlLine<T>::Type() const
    {
    return iType;
    }

template <class T> TBool TGlLine<T>::SetType( GLenum aType )
    {
    if ( aType == GL_LINES || aType == GL_LINE_LOOP || aType == GL_LINE_STRIP )
        {
        iType = aType;
        return ETrue;
        }
    return EFalse;
    }

template <class T> TBool TGlLine<T>::SetWidth( T aWidth )
    {
    if ( aWidth >= 0 )
        {
        iWidth = aWidth;
        return ETrue;
        }
    return EFalse;
    }

template <class T> void TGlLine<T>::SetAntialiased( TBool aAntialiased )
    {
    iLineSmooth = aAntialiased;
    }


#endif // GLLINE_INL
