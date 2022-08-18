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
#include <vg/openvg.h>

// CLASS DECLARATION

/**
 *  CVgSvgImage
 * 
 */
class CVgSvgImage : public CBase
    {
public:
    
    ~CVgSvgImage();
    
    static CVgSvgImage* NewL( const TDesC8& aData );
    
    static CVgSvgImage* NewL( TFileName aFileName );
    
    void Draw();

    void AddPathL( VGPath aPath, VGPaint aFill, VGPaint aStroke, VGfloat aStrokeWidth );

private:
    
    CVgSvgImage();
    
    void ConstructL( const TDesC8& aData );

private:

    typedef struct
    {
        VGPath iPath;
        VGPaint iFill;
        VGPaint iStroke;
        VGfloat iStrokeWidth;
    } TPathEntry;
    
    RArray< TPathEntry > iPathItem;
    
    };

#endif // VGSVGIMAGE_H
