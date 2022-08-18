/*
 ============================================================================
 Name        : tsvgappApplication.cpp
 Author      : Michal Strug
 Company     : 
 Version     : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : Main application class
 ============================================================================
 */

// INCLUDE FILES
#include "tsvgappApplication.h"
#include <VG/vgu.h>
#include <VG/openvg.h>


_LIT8(KSvgFile0, "<?xml version=\"1.0\" standalone=\"no\"?>\r\n<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"\r\n\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\r\n<svg width=\"4cm\" height=\"4cm\" viewBox=\"0 0 400 400\"\r\n     xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\r\n  <title>Example triangle01- simple example of a 'path'</title>\r\n  <desc>A path that draws a triangle</desc>\r\n  <rect x=\"1\" y=\"1\" width=\"398\" height=\"398\" fill=\"white\" stroke=\"green\" />\r\n  <path d=\"M 100 100 L 300 100 L 200 300 z\" fill=\"red\" stroke=\"blue\" stroke-width=\"3\" />\r\n</svg>\r\n" );
_LIT8(KSvgFile1, "<?xml version=\"1.0\" standalone=\"no\"?>\r\n<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"\r\n\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\r\n<svg width=\"4cm\" height=\"4cm\" viewBox=\"0 0 400 400\"\r\n     xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\r\n  <title>Example triangle01- simple example of a 'path'</title>\r\n  <desc>A path that draws a triangle</desc>\r\n  <rect x=\"1\" y=\"1\" width=\"398\" height=\"398\" fill=\"none\" stroke=\"blue\" />\r\n  <path d=\" M100 100 L300,100 L 200 , 300 z \" fill=\"red\" stroke=\"blue\" stroke-width=\"3\" />\r\n</svg>\r\n" );
_LIT8(KSvgFile2, "<?xml version=\"1.0\" standalone=\"no\"?>\r\n<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"\r\n\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\r\n<svg width=\"37.5pt\" height=\"37.5pt\" viewBox=\"0 0 37.5 37.5\"\r\n     xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\r\n  <title>Example triangle01- simple example of a 'path'</title>\r\n  <desc>A path that draws a triangle</desc>\r\n  <rect x=\"0.751\" y=\"0.75\" width=\"36\" height=\"36\" fill=\"#bf6200\" fill-opacity=\"0.654902\" stroke=\"none\" />\r\n  <path d=\"M 19.5,27 L 8.25,27 L 12.75,18 L 8.25,6.75 L 21.75,6.75 L 19.5,27 Z\" stroke=\"#ffffff\" stroke-opacity=\"0.772549\" stroke-width=\"0.75\" />\r\n</svg>\r\n" );

const TInt Kw3corgfilesCount = 12;
const TBufC<64> Kw3corgfiles[ Kw3corgfilesCount ] =
	{
	_L("c:\\data\\svg\\path_1.svg"),
	_L("c:\\data\\svg\\path_2.svg"),
	_L("c:\\data\\svg\\path_3.svg"),
	_L("c:\\data\\svg\\path_4.svg"),
	_L("c:\\data\\svg\\path_5.svg"),
	_L("c:\\data\\svg\\rect.svg"),
	_L("c:\\data\\svg\\round_rect.svg"),
	_L("c:\\data\\svg\\circle.svg"),
	_L("c:\\data\\svg\\ellipse.svg"),
	_L("c:\\data\\svg\\line.svg"),
	_L("c:\\data\\svg\\polygon.svg"),
	_L("c:\\data\\svg\\polyline.svg")
	};



// -----------------------------------------------------------------------------
// CtsvgappApplication::CtsvgappApplication()
// -----------------------------------------------------------------------------
//
CtsvgappApplication::CtsvgappApplication()
	{
	}

// -----------------------------------------------------------------------------
// CtsvgappApplication::~CtsvgappApplication()
// -----------------------------------------------------------------------------
//
CtsvgappApplication::~CtsvgappApplication()
	{
    delete iSvgImage[ 0 ];
    delete iSvgImage[ 1 ];
    delete iSvgImage[ 2 ];
    delete iSvgImage[ 3 ];
    delete iSvgImage[ 4 ];
	}  

// -----------------------------------------------------------------------------
// CtsvgappApplication::InitL()
// -----------------------------------------------------------------------------
//
void CtsvgappApplication::InitL()
	{
	CSGEGameApplicationVG::InitL();

	//iSvgImage1 = CVgSvgImage::NewL( KSvgFile2() );
		
    iSvgImage[ 0 ] = CVgSvgImage::NewL( FsSession(), Kw3corgfiles[ iCurrentImageW3C ] );
    iSvgImage[ 1 ] = CVgSvgImage::NewL( FsSession(), _L( "c:\\data\\svg\\img1.svg") );
    iSvgImage[ 2 ] = CVgSvgImage::NewL( FsSession(), _L( "c:\\data\\svg\\img2.svg") );
    iSvgImage[ 3 ] = CVgSvgImage::NewL( FsSession(), _L( "c:\\data\\svg\\img3.svg") );
	iSvgImage[ 4 ] = CVgSvgImage::NewL( FsSession(), _L( "c:\\data\\svg\\img4.svg") );

    iSvgImage[ 0 ]->FlipHorizontally( Size().iHeight );
    iSvgImage[ 1 ]->FlipHorizontally( Size().iHeight );
    iSvgImage[ 2 ]->FlipHorizontally( Size().iHeight );
    iSvgImage[ 3 ]->FlipHorizontally( Size().iHeight );
    iSvgImage[ 4 ]->FlipHorizontally( Size().iHeight );
	}

// -----------------------------------------------------------------------------
// CtsvgappApplication::NewApplication()
// -----------------------------------------------------------------------------
//
CApaApplication* CtsvgappApplication::NewApplication()
	{
	return CSGEGameApplicationVG::NewApplication(new CtsvgappApplication);
	}

// -----------------------------------------------------------------------------
// CtsvgappApplication::ApplicationUid()
// -----------------------------------------------------------------------------
//
TUid CtsvgappApplication::ApplicationUid()
	{
	return KUidtsvgapp;
	}

// -----------------------------------------------------------------------------
// CtsvgappApplication::HandleCommandL()
// -----------------------------------------------------------------------------
//
void CtsvgappApplication::HandleCommandL(TInt /*aCommand*/)
	{
	Exit();
	}

// -----------------------------------------------------------------------------
// CtsvgappApplication::Draw()
// -----------------------------------------------------------------------------
//
void CtsvgappApplication::Draw(const TRect& /*aRect*/) const
	{
	const_cast<CtsvgappApplication*> (this)->DrawScene();
	}

// -----------------------------------------------------------------------------
// CtsvgappApplication::FullScreen()
// -----------------------------------------------------------------------------
//
TBool CtsvgappApplication::FullScreen() const
	{
	return ETrue;
	}

// -----------------------------------------------------------------------------
// CtsvgappApplication::SampleBuffers()
// -----------------------------------------------------------------------------
//
TInt CtsvgappApplication::SampleBuffers()
	{
	return 0;
	}

// -----------------------------------------------------------------------------
// CtsvgappApplication::SampleSize()
// -----------------------------------------------------------------------------
//
TInt CtsvgappApplication::SampleSize()
	{
	return 0;
	}
TInt CtsvgappApplication::DepthSize()
	{
	return 0;
	}

TBool CtsvgappApplication::HandlePointerEventL( const TPointerEvent& aPointerEvent )
    {
    if ( aPointerEvent.iType == TPointerEvent::EButton1Down )
        {
    	if ( aPointerEvent.iPosition.iY < Size().iHeight / 2 )
    		{
    		iCurrentImageW3C++;
    		if ( iCurrentImageW3C >= Kw3corgfilesCount )
    			{
    			iCurrentImageW3C = 0;
    			}
    		delete iSvgImage[ 0 ];
    		iSvgImage[ 0 ] = NULL;
    	    iSvgImage[ 0 ] = CVgSvgImage::NewL( FsSession(), Kw3corgfiles[ iCurrentImageW3C ] );
    	    iSvgImage[ 0 ]->FlipHorizontally( Size().iHeight );
    	    iCurrentImage = 0;
    		}
    	else
    		{
    		iCurrentImage++;
    		if ( iCurrentImage >= KSvgImgCnt )
    			{
    			iCurrentImage = 0;
    			}
    		}
        }
    }

// -----------------------------------------------------------------------------
// CtsvgappApplication::DoGameLoopEvent()
// -----------------------------------------------------------------------------
//
void CtsvgappApplication::DoGameLoopEvent(
		TTimeIntervalMicroSeconds32 aTimeInterval)
	{
	if (UpdateScene(aTimeInterval))
		{
		DrawScene();
		}
	}

// -----------------------------------------------------------------------------
// CtsvgappApplication::UpdateScene()
// -----------------------------------------------------------------------------
//
TBool CtsvgappApplication::UpdateScene(TTimeIntervalMicroSeconds32 /*aTimeInterval*/)
	{
	return ETrue;
	}

// -----------------------------------------------------------------------------
// CtsvgappApplication::DrawScene()
// -----------------------------------------------------------------------------
//
void CtsvgappApplication::DrawScene()
	{
    float clearColor[] = { 1, 1, 1, 1 };
    vgSetfv( VG_CLEAR_COLOR, 4, clearColor );
    vgSeti( VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE );
    vgClear( 0, 0, Size().iWidth, Size().iHeight );
    vgLoadIdentity();

    //vgTranslate( 0, 360 );
    /*VGPaint fillg3 = vgCreatePaint();
    vgSetParameteri( fillg3, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR );
    VGfloat color3[ 4 ] = { 0, 0.5,  0.5, 1 };
    vgSetParameterfv( fillg3, VG_PAINT_COLOR, 4, color3 );
    
    vgSetPaint( fillg3, VG_FILL_PATH );
    
    VGPath path1 = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 10, 20, VG_PATH_CAPABILITY_ALL );
    vguRect( path1, 10, 10, Size().iWidth / 2, Size().iHeight / 2 );
    vgDrawPath( path1, VG_FILL_PATH );
    vgDestroyPath( path1 );
    
    vgDestroyPaint( fillg3 );*/
    
    iSvgImage[ iCurrentImage ]->Draw();
	}

// End of file
