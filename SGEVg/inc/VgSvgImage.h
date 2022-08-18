/*
 ============================================================================
 Name		: VgSvgImage.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CVgSvgImage declaration
 ============================================================================
 */

#ifndef VGSVGIMAGE_H
#define VGSVGIMAGE_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <f32file.h>
#include <vg/openvg.h>
#include <SGEVg/VgImage.h>
#include <SGEVg/VgPrimitives.h>
#include <SGEFile/SGEFile.h>

// CLASS DECLARATION

/**
 *  CVgSvgImage
 * 
 */
class CVgSvgImage : public CBase, public MVgImage
    {    
public:
    
    ~CVgSvgImage();
    
    static CVgSvgImage* NewL( const TDesC8& aData );
    
    static CVgSvgImage* NewL( RFs& aFs, const TDesC& aFileName );
    
    static CVgSvgImage* NewL( CSGEFile& aSgeFile, const TDesC& aFileName );

    MVgImage::TVgImageType ImageType() const;
    
    void Draw( const TVgPoint& aPoint ) const;

    void Draw( const TVgPoint& aPoint, TReal aAngle ) const;

    void DrawCentered( const TVgPoint& aPoint ) const;

    void DrawCentered( const TVgPoint& aPoint, TReal aAngle ) const;

    TVgSize Size() const;
    
    void AddPathL( VGPath aPath, VGPaint aFill, VGPaint aStroke, TVgSvgImageStrokeProperties aStrokeProperties );

    void FlipHorizontally( TInt aHeight );
    
private:
    
    CVgSvgImage();
    
    void ConstructL( const TDesC8& aData );

    void ConstructL( RFs& aFs, const TDesC& aFileName );    

    void ConstructL( CSGEFile& aSgeFile, const TDesC& aFileName );    
    
private:

    typedef struct
    {
        VGPath iPath;
        VGPaint iFill;
        VGPaint iStroke;
        TVgSvgImageStrokeProperties iStrokeProperties;
    } TPathEntry;
    
    RArray< TPathEntry > iPathItem;
    
    TInt iFlipHeight;
    
    TVgSize iSize;
    
    };

#endif // VGSVGIMAGE_H
