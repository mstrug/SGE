/*
 ============================================================================
 Name		: VgImage.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : TVgImage implementation
 ============================================================================
 */

#include "VgImage.h"
//#include <SGE/SGEVg/VgImage.h>
#include <FBS.H> 


TVgImage::TVgImage() : iImageHandle( NULL ), iParentImageHandle( NULL )
    {
    }

TInt TVgImage::CreateImage( CFbsBitmap *aBitmap, VGbitfield aAllowedQuality )
    {
	DeleteImage();
	
	TSize s = aBitmap->SizeInPixels();
	
	// todo: add mapping fbsBitmap format to VG image format
	
	VGImageFormat format;
	VGint dataStride;
    switch( aBitmap->DisplayMode() )
        {
        case EGray256: 
				format = VG_sL_8;
				dataStride = s.iWidth;
        	break;
        case EColor64K:
				format = VG_sRGB_565; 
				dataStride = s.iWidth * 2;
        	break;
        case EColor16M:
				format = VG_sRGBX_8888; // todo: not working!
				dataStride = s.iWidth * 3;
			break;
        case EColor16MA:
                format = VG_sABGR_8888; 
                dataStride = s.iWidth * 4;
            break;
        case EColor16MU:
                format = VG_sARGB_8888; 
                dataStride = s.iWidth * 4;
            break;
        default:
			return KErrNotSupported;
        }
	
	iImageHandle = vgCreateImage( format, s.iWidth, s.iHeight, aAllowedQuality );
	
	if ( !iImageHandle )
		{
		return KErrGeneral;
		}

	TUint32 *adr = aBitmap->DataAddress();
	TInt strd = aBitmap->DataStride();
	
	aBitmap->BeginDataAccess();
	vgImageSubData( iImageHandle, aBitmap->DataAddress(), dataStride, format, 0, 0, s.iWidth, s.iHeight );
	aBitmap->EndDataAccess();

	iSize = aBitmap->SizeInPixels();
	iFormat = format;
	iAllowedQuality = aAllowedQuality;

    return KErrNone;
    }

TInt TVgImage::CreateImage( TSize aSize, VGImageFormat aFormat, const TAny *aData, VGbitfield aAllowedQuality, VGint aDataStride )
    {
	DeleteImage();
	
	iImageHandle = vgCreateImage( aFormat, aSize.iWidth, aSize.iHeight, aAllowedQuality );

	if ( !iImageHandle )
		{
		return KErrGeneral;
		}
	
	if ( aDataStride == KDefaultDataStride )
		{ // todo: add datastride calculation basing image format
		aDataStride = aSize.iWidth * 4;
		}
	
	if ( aData )
		{
		vgImageSubData( iImageHandle, aData, aDataStride, aFormat, 0, 0, aSize.iWidth, aSize.iHeight );
		}
	
	iSize = aSize;
	iFormat = aFormat;
	iAllowedQuality = aAllowedQuality;
	
    return KErrNone;
    }

TVgImage TVgImage::CreateChildImage( TPoint aOrigin, TSize aSize )
	{
	TVgImage img;
	img.iParentImageHandle = iImageHandle;
	img.iSize = aSize;
	img.iAllowedQuality = iAllowedQuality;
	img.iFormat = iFormat;
	img.iImageHandle = vgChildImage( iImageHandle, aOrigin.iX, aOrigin.iY, aSize.iWidth, aSize.iHeight );
	return img;
	}

TVgImage TVgImage::CopyImage()
    {
    TVgImage img;
    img.CreateImage( iSize, iFormat, NULL, iAllowedQuality, KDefaultDataStride );
    vgCopyImage( img.Image(), 0, 0, iImageHandle, 0, 0, iSize.iWidth, iSize.iHeight, VG_FALSE );
    return img;
    }

void TVgImage::DeleteImage()
    {
	vgDestroyImage( iImageHandle );
	iImageHandle = NULL;
	iParentImageHandle = NULL;
    }

MVgImage::TVgImageType TVgImage::ImageType() const
	{
	return EVgImageTypeImage;
	}

void TVgImage::Draw( const TVgPoint& aPoint ) const
	{
    vgSeti( VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE );
    vgTranslate( aPoint.iX, aPoint.iY );
    vgDrawImage( iImageHandle );
	}

void TVgImage::Draw( const TVgPoint& aPoint, TReal aAngle ) const
    {
    vgTranslate( aPoint.iX, aPoint.iY );
    vgTranslate( (TReal)iSize.iWidth / (TReal)2, (TReal)iSize.iHeight / (TReal)2 );
    vgRotate( aAngle );
    vgTranslate( -(TReal)iSize.iWidth / (TReal)2, -(TReal)iSize.iHeight / (TReal)2 );
    vgDrawImage( iImageHandle );
    }

void TVgImage::DrawCentered( const TVgPoint& aPoint ) const
	{
    vgSeti( VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE );
    vgTranslate( aPoint.iX - iSize.iWidth / 2, aPoint.iY - iSize.iHeight / 2 );
    vgDrawImage( iImageHandle );
	}

void TVgImage::DrawCentered( const TVgPoint& aPoint, TReal aAngle ) const
    {
    vgSeti( VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE );
    vgTranslate( aPoint.iX, aPoint.iY );
    vgRotate( aAngle );
    vgTranslate( -(TReal)iSize.iWidth / (TReal)2, -(TReal)iSize.iHeight / (TReal)2 );
    vgDrawImage( iImageHandle );
    }

VGImage TVgImage::Image() const
    {
    return iImageHandle;
    }

VGImage TVgImage::ParentImage() const
    {
    return iParentImageHandle;
    }

TVgSize TVgImage::Size() const
    {
    return iSize;
    }

TBool TVgImage::IsChildImage() const
	{
	return (TBool) iParentImageHandle;
	}

VGImageFormat TVgImage::Format() const
	{
	return iFormat;
	}

VGbitfield TVgImage::AllowedQuality() const
	{
	return iAllowedQuality;
	}

TVgImage TVgImage::MultiplyByColorMatrix( VGfloat* aMatrix )
	{
	TVgImage img;
	img.CreateImage( iSize, iFormat, NULL, iAllowedQuality, KDefaultDataStride );
	vgColorMatrix( img.Image(), iImageHandle, aMatrix );
	return img;
	}

