/*
 ============================================================================
 Name		: GlTextureFileCustom.cpp
 Author	  : Michal Strug
 Version	 : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : GlTextureFileCustom implementation
 ============================================================================
 */

#include "GlTextureFileCustom.h"


struct SKTXHeader
{
	TUint8 identifier[ 12 ];
	TUint32 endianness;
	TUint32 glType;
	TUint32 glTypeSize;
	TUint32 glFormat;
	TUint32 glInternalFormat;
	TUint32 glBaseInternalFormat;
	TUint32 pixelWidth;
	TUint32 pixelHeight;
	TUint32 pixelDepth;
	TUint32 numberOfArrayElements;
	TUint32 numberOfFaces;
	TUint32 numberOfMipmapLevels;
	TUint32 bytesOfKeyValueData;
};


TBool GlTextureFileCustom::LoadKTX( RBuf8& aData, STextureCustom& aTexture )
    {
    const TInt KTextureKTXMagicNumber[ 12 ] = { 0xAB, 'K', 'T', 'X', ' ', '1', '1', 0xBB, 0x0D, 0x0A, 0x1A, 0x0A }; // offset 0

    const TUint8* ptr = aData.Ptr();
    const SKTXHeader* hdr = (const SKTXHeader*) ptr;
    ptr += sizeof( SKTXHeader );

    for ( TInt i = 0; i < 12; i++ )
	{
	if ( hdr->identifier[ i ] != KTextureKTXMagicNumber[ i ] )
		{
		return EFalse;
		}
	}

    //TBool endianOk = ( hdr->endianness == 0x04030201 );

    // hdr->glType == 0 
    // hdr->glTypeSize == 1
    // hdr->glFormat == 0
    aTexture.iFormat = hdr->glInternalFormat;
    // hdr->glBaseInternalFormat (n/a)
    aTexture.iWidth = hdr->pixelWidth;
    aTexture.iHeight = hdr->pixelHeight;
    // hdr->pixelDepth == 0
    // hdr->numberOfArrayElements ?
    // hdr->numberOfFaces == 1
    // hdr->numberOfMipmapLevels
    // hdr->bytesOfKeyValueData == 0 !

    for ( TUint i = 0; i < hdr->bytesOfKeyValueData; )
	{
	const TUint8* tmpPtr = ptr;
	TUint32 keyAndValueByteSize = *ptr;
	ptr += sizeof( TUint32 );

	ptr += keyAndValueByteSize * sizeof( TUint8 );  // keyAndValue
	ptr += ( 3 - ((keyAndValueByteSize + 3) % 4) ) * sizeof( TUint8 );  // valuePadding

	i += ptr - tmpPtr;
	}

    for ( TUint i = 0; i < ( hdr->numberOfMipmapLevels == 0 ? 1 : hdr->numberOfMipmapLevels ); i++ )
	{
	TUint32* imageSize = (TUint32*) ptr;
	ptr += sizeof( TUint32 );

	aTexture.iDataSize = *imageSize;

	for ( TUint j = 0; j < ( hdr->numberOfArrayElements == 0 ? 1 : hdr->numberOfArrayElements ); j++ )
		{
		for ( TUint k = 0; k < hdr->numberOfFaces; k++ )
			{
			for( TUint m = 0; m < ( hdr->pixelDepth == 0 ? 1 : hdr->pixelDepth ); m++ )
				{
				for ( TUint n = 0; n < ( hdr->pixelHeight == 0 ? 1 : hdr->pixelHeight ); n++ )
					{
					aTexture.iData = (GLvoid*) ptr;

					// not supporting mip maps for now !!!
					ptr += aTexture.iDataSize * sizeof( TUint8 );

					return ETrue;
					}
				}
			//ptr += cubePadding[0-3]
			}
		}
		ptr += ( 3 - ((*imageSize + 3) % 4) ) * sizeof( TUint8 );  // mipPadding
	}

    return EFalse;
    }

TBool GlTextureFileCustom::LoadPVR( RBuf8& /*aData*/, STextureCustom& /*aTexture*/ )
    {
    //const TInt KTexturePVRMagicNumber[ 4 ] = { 'P', 'V', 'R', '!' }; // offset 0x2C
    return EFalse;
    }



