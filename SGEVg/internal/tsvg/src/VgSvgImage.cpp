/*
 ============================================================================
 Name		: VgSvgImage.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CVgSvgImage implementation
 ============================================================================
 */

#include "VgSvgImage.h"
#include "VgSvgParser.h"


CVgSvgImage::CVgSvgImage()
    {
    }

CVgSvgImage::~CVgSvgImage()
    {
    for ( TInt i = 0; i < iPathItem.Count(); i++ )
        {
        vgDestroyPaint( iPathItem[ i ].iFill );
        vgDestroyPaint( iPathItem[ i ].iStroke );
        vgDestroyPath( iPathItem[ i ].iPath );
        }
    iPathItem.Close();
    }

CVgSvgImage* CVgSvgImage::NewL( const TDesC8& aData )
    {
    CVgSvgImage* self = new (ELeave) CVgSvgImage();
    CleanupStack::PushL(self);
    self->ConstructL( aData );
    CleanupStack::Pop(); // self;
    return self;
    }

CVgSvgImage* CVgSvgImage::NewL( TFileName aFileName )
    {
    CVgSvgImage* self = new (ELeave) CVgSvgImage();
    CleanupStack::PushL(self);
    //self->ConstructL();
    CleanupStack::Pop(); // self;
    return self;
    }

void CVgSvgImage::ConstructL( const TDesC8& aData )
    {
    CVgSvgParser* parser = CVgSvgParser::NewLC( *this );
    parser->ParseL( aData );
    CleanupStack::PopAndDestroy( parser );
    }

void CVgSvgImage::AddPathL( VGPath aPath, VGPaint aFill, VGPaint aStroke, VGfloat aStrokeWidth )
    {
    TPathEntry p = { aPath, aFill, aStroke, aStrokeWidth };
    if ( aPath )
        {
        iPathItem.AppendL( p );
        }
    }

void CVgSvgImage::Draw()
    {
    for ( TInt i = 0; i < iPathItem.Count(); i++ )
        {
        if ( iPathItem[ i ].iFill && iPathItem[ i ].iStroke )
            {
            vgSetPaint( iPathItem[ i ].iFill, VG_FILL_PATH );
            vgSetPaint( iPathItem[ i ].iStroke, VG_STROKE_PATH );
            vgSetf( VG_STROKE_LINE_WIDTH, iPathItem[ i ].iStrokeWidth );
            vgDrawPath( iPathItem[ i ].iPath, VG_STROKE_PATH | VG_FILL_PATH );
            }
        else if ( iPathItem[ i ].iFill )
            {
            vgSetPaint( iPathItem[ i ].iFill, VG_FILL_PATH );
            vgDrawPath( iPathItem[ i ].iPath, VG_FILL_PATH );
            }
        else if ( iPathItem[ i ].iStroke )
            {
            vgSetPaint( iPathItem[ i ].iStroke, VG_STROKE_PATH );
            vgSetf( VG_STROKE_LINE_WIDTH, iPathItem[ i ].iStrokeWidth );
            vgDrawPath( iPathItem[ i ].iPath, VG_STROKE_PATH );
            }
        else
            {
            vgDrawPath( iPathItem[ i ].iPath, VG_STROKE_PATH | VG_FILL_PATH );
            }
        }
    }


