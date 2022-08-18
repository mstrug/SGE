/*
 ============================================================================
 Name		: VgImage.h
 Author	  : Michal Strug
 Version	 : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : TVgImage declaration
 ============================================================================
 */

#ifndef VGIMAGE_H
#define VGIMAGE_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <vg/openvg.h>
#include <SGEVg/VgPrimitives.h>

// CLASS DECLARATION
class CFbsBitmap;

const VGint KDefaultDataStride = 0;



class MVgImage
    {
public:    
    enum TVgImageType
        {
        EVgImageTypeImage,
        EVgImageTypeSvg
        };
    
    virtual TVgImageType ImageType() const = 0;
    
    // draws image at specified point
    virtual void Draw( const TVgPoint& aPoint ) const = 0;
    
    // draws image at specified point and rotated by angle (across image center)
    virtual void Draw( const TVgPoint& aPoint, TReal aAngle ) const = 0;
	
	// draws image centered at specified point
	virtual void DrawCentered( const TVgPoint& aPoint ) const = 0;
	
	// draws image at specified point (centered) and rotated by angle (across image center)
	virtual void DrawCentered( const TVgPoint& aPoint, TReal aAngle ) const = 0;

    virtual TVgSize Size() const = 0;
    };


/**
 *  TVgImage
 *  todo: currently only RGBA images supported
 */
class TVgImage : public MVgImage
    {

	VGImage iImageHandle;

	VGImage iParentImageHandle;
    
    TSize iSize;
    
    VGImageFormat iFormat;
    
    VGbitfield iAllowedQuality;
    
public:
    
    TVgImage();
    
    // Bitmap display mode map:
    //  EGray256 -> VG_sL_8
    //  EColor64K -> VG_sRGB_565
    //  EColor16MA -> VG_sABGR_8888
    //  EColor16MU -> VG_sARGB_8888
    TInt CreateImage( CFbsBitmap *aBitmap, VGbitfield aAllowedQuality );
    
    TInt CreateImage( TSize aSize, VGImageFormat aFormat, const TAny *aData, VGbitfield aAllowedQuality, VGint aDataStride = KDefaultDataStride );

    TVgImage CreateChildImage( TPoint aOrigin, TSize aSize );

    TVgImage CopyImage();

    void DeleteImage();

    MVgImage::TVgImageType ImageType() const;
    
    void Draw( const TVgPoint& aPoint ) const;
    
    void Draw( const TVgPoint& aPoint, TReal aAngle ) const;

    void DrawCentered( const TVgPoint& aPoint ) const;

    void DrawCentered( const TVgPoint& aPoint, TReal aAngle ) const;

    VGImage Image() const;

    VGImage ParentImage() const;

    TVgSize Size() const;

    TBool IsChildImage() const;
    
    VGImageFormat Format() const;
    
    VGbitfield AllowedQuality() const;
    
    // see OpenVG 1.1 spec 12.3
    TVgImage MultiplyByColorMatrix( VGfloat* aMatrix );
    
    };

#endif // VGIMAGE_H
