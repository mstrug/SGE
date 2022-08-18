/*
 ============================================================================
 Name		: GlVertexArray.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CGlVertexArray implementation
 ============================================================================
 */

#include "GlVertexArray.h"


/*
 * MEMORY PATTERN:
 * 
 * 
 * Vertex, Normal, Color, PointSize, TextCoord0, TextCoord1...,
 * Vertex, Normal, Color, PointSize, TextCoord0, TextCoord1...,
 * Vertex, Normal, Color, PointSize, TextCoord0, TextCoord1...,
 * 
 */


    
CGlVertexArray::CGlVertexArray( TGlVertexArrayConfig aConfig, TInt aVertexCount ) 
    : iConfig( aConfig ), iVertexCount( aVertexCount ), iDataOwnership( ETrue ), iIndicesOwnership( ETrue ),
      iVertexArrayEnabled( ETrue ), iNormalArrayEnabled( ETrue ), iColorArrayEnabled( ETrue ),
      iPointSizeArrayEnabled( ETrue ), iIndicesArrayEnabled( ETrue )
    {
    CalculateVertexDataSize();
    
    for ( TInt i = 0; i < iTexCoordArrayEnabled.Count(); i++ )
        {
        iTexCoordArrayEnabled[ i ] = ETrue;
        }
    }

CGlVertexArray::~CGlVertexArray()
    {
    FreeData();
    }

void CGlVertexArray::FreeData()
    {
    if ( iDataOwnership )
        {
        User::Free( iData );
        }
    iData = NULL;
    if ( iIndicesOwnership )
        {
        User::Free( iIndices );
        }
    iIndices = NULL;
    }

CGlVertexArray* CGlVertexArray::NewLC( TGlVertexArrayConfig aConfig, TInt aVertexCount )
    {
    CGlVertexArray *self = new (ELeave) CGlVertexArray( aConfig, aVertexCount );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

CGlVertexArray* CGlVertexArray::NewL( TGlVertexArrayConfig aConfig, TInt aVertexCount )
    {
    CGlVertexArray *self = CGlVertexArray::NewLC( aConfig, aVertexCount );
    CleanupStack::Pop(); // self;
    return self;
    }

void CGlVertexArray::ConstructL()
    {
    User::LeaveIfError( CreateData() );
    User::LeaveIfError( CreateIndices() );
    }

TInt CGlVertexArray::CreateData()
    {  
    if ( iDataOwnership )
        {
        User::Free( iData );
        iData = NULL;
        }
    
    TInt size = iVertexCount * iVertexDataSize;
//    if ( size % 4 != 0 )
//        {
//        size += 4 - ( size % 4 );
//        }
    
    iData = User::Alloc( size );
    if ( !iData )
        {
        return KErrNoMemory;
        }
    Mem::FillZ( iData, size );

    return KErrNone;
    }

TInt CGlVertexArray::CreateIndices()
    {
    if ( iIndicesOwnership )
        {
        User::Free( iIndices );
        iIndices = NULL;
        }
    
    TInt size = iConfig.iIndicesCount * iConfig.iIndicesType;
//    if ( size % 4 != 0 )
//        {
//        size += 4 - ( size % 4 );
//        }
    
    iIndices = User::Alloc( size );
    if ( !iIndices )
        {
        return KErrNoMemory;
        }
    Mem::FillZ( iIndices, size );

    return KErrNone;
    }

void CGlVertexArray::CalculateVertexDataSize()
    {
    iVertexDataSize = iConfig.iVertexPointerSize * iConfig.iVertexPointerType + 
                      iConfig.iNormalPointerSize * iConfig.iNormalPointerType +
                      iConfig.iColorPointerSize * iConfig.iColorPointerType + 
                      iConfig.iPointSizePointerSize * iConfig.iPointSizePointerType +
                      iConfig.iTexCoordPointerSize * iConfig.iTexCoordPointerType * iConfig.iTexCoordPointerCount;
    // todo: add filler ?
//    if ( iVertexDataSize % 4 != 0 )
//        {
//        iVertexDataSize += 4 - ( iVertexDataSize % 4 );
//        }
    }

TInt CGlVertexArray::SetData( TAny *aData, TInt aSize, TBool aTakeOwnership )
    {
    if ( aSize != iVertexCount * iVertexDataSize )
        {
        return KErrArgument;
        }

    if ( !iData )
        {
        if ( CreateData() != KErrNone )
            {
            return KErrNoMemory;
            }
        }
    
    iDataOwnership = !aTakeOwnership;
    if ( aTakeOwnership )
        {
        User::Free( iData );
        iData = aData;
        }
    else
        {
        Mem::Copy( iData, aData, aSize );
        }
    
    return KErrNone;
    }

TInt CGlVertexArray::SetVertexData( TAny *aData, TInt aSize )
    {
    TInt dataSize = iConfig.iVertexPointerSize * iConfig.iVertexPointerType;
    if ( aSize != iVertexCount * dataSize )
        {
        return KErrArgument;
        }

    if ( !iData )
        {
        if ( CreateData() != KErrNone )
            {
            return KErrNoMemory;
            }
        }
    
    TInt offset0 = 0;
    TInt offset = iVertexDataSize;
    TAny *ptr = (TUint8*) iData + offset0;
    TAny *data = aData;
    CopyMemory( ptr, offset, data, dataSize );
    
    return KErrNone;
    }

TInt CGlVertexArray::SetNormalData( TAny *aData, TInt aSize )
    {
    TInt dataSize = iConfig.iNormalPointerSize * iConfig.iNormalPointerType;
    if ( aSize != iVertexCount * dataSize )
        {
        return KErrArgument;
        }

    if ( !iData )
        {
        if ( CreateData() != KErrNone )
            {
            return KErrNoMemory;
            }
        }
    
    TInt offset0 = iConfig.iVertexPointerSize * iConfig.iVertexPointerType;
    TInt offset = iVertexDataSize;
    TAny *ptr = (TUint8*) iData + offset0;
    TAny *data = aData;
    CopyMemory( ptr, offset, data, dataSize );
    
    return KErrNone;
    }

TInt CGlVertexArray::SetColorData( TAny *aData, TInt aSize )
    {
    TInt dataSize = iConfig.iColorPointerSize * iConfig.iColorPointerType;
    if ( aSize != iVertexCount * dataSize )
        {
        return KErrArgument;
        }

    if ( !iData )
        {
        if ( CreateData() != KErrNone )
            {
            return KErrNoMemory;
            }
        }
    
    TInt offset0 = iConfig.iVertexPointerSize * iConfig.iVertexPointerType + 
                   iConfig.iNormalPointerSize * iConfig.iNormalPointerType;
    TInt offset = iVertexDataSize;
    TAny *ptr = (TUint8*) iData + offset0;
    TAny *data = aData;
    CopyMemory( ptr, offset, data, dataSize );
    
    return KErrNone;
    }

TInt CGlVertexArray::SetPointSizeData( TAny *aData, TInt aSize )
    {
    TInt dataSize = iConfig.iPointSizePointerSize * iConfig.iPointSizePointerType;
    if ( aSize != iVertexCount * dataSize )
        {
        return KErrArgument;
        }

    if ( !iData )
        {
        if ( CreateData() != KErrNone )
            {
            return KErrNoMemory;
            }
        }
    
    TInt offset0 = iConfig.iVertexPointerSize * iConfig.iVertexPointerType + 
                   iConfig.iNormalPointerSize * iConfig.iNormalPointerType +
                   iConfig.iColorPointerSize * iConfig.iColorPointerType;
    TInt offset = iVertexDataSize;
    TAny *ptr = (TUint8*) iData + offset0;
    TAny *data = aData;
    CopyMemory( ptr, offset, data, dataSize );
    
    return KErrNone;
    }

TInt CGlVertexArray::SetTexCoordData( TInt aNum, TAny *aData, TInt aSize )
    {
    TInt dataSize = iConfig.iTexCoordPointerSize * iConfig.iTexCoordPointerType;
    if ( aSize != iVertexCount * dataSize ||
         aNum < 0 || aNum >= iConfig.iTexCoordPointerCount )
        {
        return KErrArgument;
        }

    if ( !iData )
        {
        if ( CreateData() != KErrNone )
            {
            return KErrNoMemory;
            }
        }
    
    //iTexCoordArrayEnabled[ aNum ] = ETrue;
    
    TInt offset0 = iConfig.iVertexPointerSize * iConfig.iVertexPointerType +
                   iConfig.iNormalPointerSize * iConfig.iNormalPointerType +
                   iConfig.iColorPointerSize * iConfig.iColorPointerType +
                   iConfig.iPointSizePointerSize * iConfig.iPointSizePointerType +
                   aNum * iConfig.iTexCoordPointerSize * iConfig.iTexCoordPointerType;
    TInt offset = iVertexDataSize;
    TAny *ptr = (TUint8*) iData + offset0;
    TAny *data = aData;
    CopyMemory( ptr, offset, data, dataSize );
    
    return KErrNone;
    }

TInt CGlVertexArray::SetIndices( TAny *aIndices, TInt aSize, TBool aTakeOwnership )
    {
    TInt dataSize = iConfig.iIndicesCount * iConfig.iIndicesType;
    if ( aSize != dataSize )
        {
        return KErrArgument;
        }
    
    if ( !iIndices )
        {
        if ( CreateIndices() != KErrNone )
            {
            return KErrNoMemory;
            }
        }
    
    iIndicesOwnership = !aTakeOwnership;
    if ( aTakeOwnership )
        {
        User::Free( iIndices );
        iIndices = aIndices;
        }
    else
        {
        Mem::Copy( iIndices, aIndices, aSize );
        }

    return KErrNone;
    }

void CGlVertexArray::CopyMemory( TAny *aDestination, TInt aDestinationOffset, TAny *aSource, TInt aSourceOffset )
    {
    for ( TInt i = 0; i < iVertexCount; 
          i++, aDestination = (TUint8*) aDestination + aDestinationOffset, 
               aSource = (TUint8*) aSource + aSourceOffset )
        {
        Mem::Copy( aDestination, aSource, aSourceOffset );
        }    
    }


TInt CGlVertexArray::UseVertexData()
    {
    if ( !iVertexArrayEnabled )
        {
        glDisableClientState( GL_VERTEX_ARRAY );
        return KErrNotSupported;
        }

    glEnableClientState( GL_VERTEX_ARRAY );

    glVertexPointer( iConfig.iVertexPointerSize, iConfig.iVertexPointerGlType, iVertexDataSize, iData );
    
    return KErrNone;
    }

TInt CGlVertexArray::UseNormalData()
    {
    if ( iConfig.iNormalPointerSize == ENormalSizeNone ||
         iConfig.iNormalPointerGlType == ENormalTypeNone ||
         !iNormalArrayEnabled )
        {
        glDisableClientState( GL_NORMAL_ARRAY );
        return KErrNotSupported;
        }
    
    glEnableClientState( GL_NORMAL_ARRAY );
    
    TAny *ptr = (TUint8*) iData + iConfig.iVertexPointerSize * iConfig.iVertexPointerType;
    glNormalPointer( iConfig.iNormalPointerGlType, iVertexDataSize, ptr );
    
    return KErrNone;
    }

TInt CGlVertexArray::UseColorData()
    {
    if ( iConfig.iColorPointerSize == EColorSizeNone ||
         iConfig.iColorPointerGlType == EColorTypeNone ||
         !iColorArrayEnabled )
        {
        glDisableClientState( GL_COLOR_ARRAY );
        return KErrNotSupported;
        }
    
    glEnableClientState( GL_COLOR_ARRAY );
    
    TAny *ptr = (TUint8*) iData + iConfig.iVertexPointerSize * iConfig.iVertexPointerType +
                                  iConfig.iNormalPointerSize * iConfig.iNormalPointerType;
    glColorPointer( iConfig.iColorPointerSize, iConfig.iColorPointerGlType, iVertexDataSize, ptr );
    
    return KErrNone;
    }

TInt CGlVertexArray::UsePointSizeData()
    { // todo: add compilation flat to compile with opengl version 1.0 or 1.1
#ifndef GL_OES_VERSION_1_1
    return KErrNotSupported;
#else
    /*
    if ( iConfig.iPointSizePointerSize == EPointSizeNone ||
         iConfig.iPointSizePointerGlType == EPointSizeTypeNone ||
         !iPointSizeArrayEnabled )
        {
        glDisableClientState( GL_POINT_SIZE_ARRAY_OES );
        return KErrNotSupported;
        }
    
    glEnableClientState( GL_POINT_SIZE_ARRAY_OES );
    
    TAny *ptr = (TUint8*) iData + iConfig.iVertexPointerSize * iConfig.iVertexPointerType +
                                  iConfig.iNormalPointerSize * iConfig.iNormalPointerType +
                                  iConfig.iColorPointerSize * iConfig.iColorPointerType;
    glPointSizePointerOES( iConfig.iPointSizePointerGlType, iVertexDataSize, ptr );
    */
    return KErrNone;
#endif
    }

TInt CGlVertexArray::UseTexCoordData()
    {
    if ( iConfig.iTexCoordPointerSize == ETexCoordSizeNone ||
         iConfig.iTexCoordPointerGlType == ETexCoordTypeNone )
        {
        glDisableClientState( GL_TEXTURE_COORD_ARRAY );
        return KErrNotSupported;
        }

    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    
    TInt disabledTex = 0;
    for ( TInt i = 0; i < iConfig.iTexCoordPointerCount; i++ )
        {
        if ( !iTexCoordArrayEnabled[ i ] )
            {
            disabledTex++;
            continue;
            }
        TAny *ptr = (TUint8*) iData + iConfig.iVertexPointerSize * iConfig.iVertexPointerType +
                                      iConfig.iNormalPointerSize * iConfig.iNormalPointerType +
                                      iConfig.iColorPointerSize * iConfig.iColorPointerType +
                                      iConfig.iPointSizePointerSize * iConfig.iPointSizePointerType +
                                      i * iConfig.iTexCoordPointerSize * iConfig.iTexCoordPointerType;
        glClientActiveTexture( GL_TEXTURE0 + i );
        glTexCoordPointer( iConfig.iTexCoordPointerSize, iConfig.iTexCoordPointerGlType, iVertexDataSize, ptr );
        }
    
    if ( disabledTex == iConfig.iTexCoordPointerCount )
        {
        glDisableClientState( GL_TEXTURE_COORD_ARRAY );
        return KErrNotSupported;
        }
        
    return KErrNone;
    }

void CGlVertexArray::UseData()
    {
    UseVertexData();
    UseNormalData();
    UseColorData();
    UsePointSizeData();
    UseTexCoordData();
    }

void CGlVertexArray::UseCleanup()
    {
    }

TInt CGlVertexArray::UsedArrayCount()
    {
    return 1 + ( iConfig.iNormalPointerGlType != 0 ? 1 : 0 ) +
               ( iConfig.iColorPointerGlType != 0 ? 1 : 0 ) +
               ( iConfig.iPointSizePointerGlType != 0 ? 1 : 0 ) +
               ( iConfig.iTexCoordPointerGlType != 0 ? 1 : 0 ) * iConfig.iTexCoordPointerCount;
    }

TInt CGlVertexArray::SetVertexCount( TUint aVertexCount )
    {
    iVertexCount = aVertexCount;

    CalculateVertexDataSize();

    TInt err = CreateData();
    if ( err )
        {
        return err;
        }
    err = CreateIndices();
    return err;
    }

void CGlVertexArray::Draw( GLenum aType )
    {
    UseData();
    if ( iConfig.iIndicesGlType == EIndicesTypeNone || !iIndicesArrayEnabled )
        {
        glDrawArrays( aType, 0, iVertexCount );
        }
    else
        {
        glDrawElements( aType, iConfig.iIndicesCount, iConfig.iIndicesGlType, iIndices );
        }
    UseCleanup();
    }

void CGlVertexArray::Redraw( GLenum aType )
    {
    if ( iConfig.iIndicesGlType == EIndicesTypeNone || !iIndicesArrayEnabled )
        {
        glDrawArrays( aType, 0, iVertexCount );
        }
    else
        {
        glDrawElements( aType, iConfig.iIndicesCount, iConfig.iIndicesGlType, iIndices );
        }
    }

void CGlVertexArray::VertexArrayEnabled( TBool aEnabled )
    {
    iVertexArrayEnabled = aEnabled;
    }

void CGlVertexArray::NormalArrayEnabled( TBool aEnabled )
    {
    iNormalArrayEnabled = aEnabled;
    }

void CGlVertexArray::ColorArrayEnabled( TBool aEnabled )
    {
    iColorArrayEnabled = aEnabled;
    }

void CGlVertexArray::PointSizeArrayEnabled( TBool aEnabled )
    {
    iPointSizeArrayEnabled = aEnabled;
    }

void CGlVertexArray::IndicesArrayEnabled( TBool aEnabled )
    {
    iIndicesArrayEnabled = aEnabled;
    }

void CGlVertexArray::TexCoordArrayEnabled( TInt aNum, TBool aEnabled )
    {
    if ( aNum >= 0 && aNum < iTexCoordArrayEnabled.Count() )
        {
        iTexCoordArrayEnabled[ aNum ] = aEnabled;
        }
    }

