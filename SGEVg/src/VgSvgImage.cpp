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

CVgSvgImage* CVgSvgImage::NewL( RFs& aFs, const TDesC& aFileName )
    {
    CVgSvgImage* self = new (ELeave) CVgSvgImage();
    CleanupStack::PushL(self);
    self->ConstructL( aFs, aFileName );
    CleanupStack::Pop(); // self;
    return self;
    }

CVgSvgImage* CVgSvgImage::NewL( CSGEFile& aSgeFile, const TDesC& aFileName )
    {
    CVgSvgImage* self = new (ELeave) CVgSvgImage();
    CleanupStack::PushL(self);
    self->ConstructL( aSgeFile, aFileName );
    CleanupStack::Pop(); // self;
    return self;
    }

void CVgSvgImage::ConstructL( const TDesC8& aData )
    {
	// use libxml2 if data buffer is larger than 10kB
    CVgSvgParser* parser = CVgSvgParser::NewLC( *this, ( aData.Length() > 10 * 1024 ) );
    parser->ParseL( aData );
    iSize = parser->ViewBox().Size();
    CleanupStack::PopAndDestroy( parser );
    iFlipHeight = iSize.iHeight;
    }

void CVgSvgImage::ConstructL( RFs& aFs, const TDesC& aFileName )    
	{
	RFile file;
	User::LeaveIfError( file.Open( aFs, aFileName, EFileRead ) );
	CleanupClosePushL( file );
	TInt size;
	User::LeaveIfError( file.Size( size ) );
	RBuf8 buf;
	buf.CreateL( size );
	buf.CleanupClosePushL();
	file.Read( buf );
	ConstructL( buf );
    CleanupStack::PopAndDestroy( &buf );
    CleanupStack::PopAndDestroy( &file );
	}

void CVgSvgImage::ConstructL( CSGEFile& aSgeFile, const TDesC& aFileName )
    {
    TInt size;
    RBuf8 data;
    RReadStream file = aSgeFile.OpenFileL( aFileName, &size );
    file.PushL();
    data.CreateL( size );
    data.CleanupClosePushL();
    file.ReadL( data, size );
    ConstructL( data );
    CleanupStack::PopAndDestroy( &data );
    CleanupStack::PopAndDestroy( &file );
    }

void CVgSvgImage::AddPathL( VGPath aPath, VGPaint aFill, VGPaint aStroke, TVgSvgImageStrokeProperties aStrokeProperties )
    {
    TPathEntry p = { aPath, aFill, aStroke, aStrokeProperties };
    if ( aFill == 0 && aStroke == 0 )
        {
        vgDestroyPath( aPath );
        }
    else if ( aPath )
        {
        iPathItem.AppendL( p );
        }
    }

void CVgSvgImage::FlipHorizontally( TInt aHeight )
    {
    iFlipHeight = aHeight;
    }

MVgImage::TVgImageType CVgSvgImage::ImageType() const
	{
	return EVgImageTypeSvg;
	}

void CVgSvgImage::Draw( const TVgPoint& aPoint ) const
    {
    vgSeti( VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE );
	vgTranslate( aPoint.iX, aPoint.iY );
    if ( iFlipHeight )
        {
        vgScale( 1, -1 );
        vgTranslate( 0, -iFlipHeight );
        }
    for ( TInt i = 0; i < iPathItem.Count(); i++ )
        {
        if ( iPathItem[ i ].iFill && iPathItem[ i ].iStroke )
            {
            vgSetPaint( iPathItem[ i ].iFill, VG_FILL_PATH );
            vgSetPaint( iPathItem[ i ].iStroke, VG_STROKE_PATH );
            iPathItem[ i ].iStrokeProperties.Use();
            //vgSetf( VG_STROKE_LINE_WIDTH, iPathItem[ i ].iStrokeWidth );
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
            iPathItem[ i ].iStrokeProperties.Use();
            //vgSetf( VG_STROKE_LINE_WIDTH, iPathItem[ i ].iStrokeWidth );
            vgDrawPath( iPathItem[ i ].iPath, VG_STROKE_PATH );
            }
        else
            {
            //vgDrawPath( iPathItem[ i ].iPath, VG_STROKE_PATH | VG_FILL_PATH );
            }
        }
    }

void CVgSvgImage::Draw( const TVgPoint& aPoint, TReal aAngle ) const
    {
    vgTranslate( aPoint.iX, aPoint.iY );
    vgTranslate( (TReal)iSize.iWidth / (TReal)2, (TReal)iSize.iHeight / (TReal)2 );
    vgRotate( aAngle );
    vgTranslate( -(TReal)iSize.iWidth / (TReal)2, -(TReal)iSize.iHeight / (TReal)2 );
    Draw( TVgPoint() );
    }

void CVgSvgImage::DrawCentered( const TVgPoint& aPoint ) const
	{
    vgSeti( VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE );
	Draw( TVgPoint( aPoint.iX - iSize.iWidth / 2, aPoint.iY - iSize.iHeight / 2 ) );
	}

void CVgSvgImage::DrawCentered( const TVgPoint& aPoint, TReal aAngle ) const
	{
    vgSeti( VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE );
    vgTranslate( aPoint.iX, aPoint.iY );
    vgRotate( aAngle );
    vgTranslate( -(TReal)iSize.iWidth / (TReal)2, -(TReal)iSize.iHeight / (TReal)2 );
	Draw( TVgPoint() );
	}

TVgSize CVgSvgImage::Size() const
	{
	return iSize;
	}

