/*
 ============================================================================
 Name        : tsvgappApplication.h
 Author      : Michal Strug
 Company     : 
 Version     : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : Declares main application class.
 ============================================================================
 */

#ifndef __TSVGAPPAPPLICATION_H__
#define __TSVGAPPAPPLICATION_H__

// INCLUDES
#include <SGEApplication/SGEGameApplicationVG.h>
#include <SGEVg/VgSvgImage.h>

// CONSTANTS
// UID of the application
const TUid KUidtsvgapp =
	{
	0xE96893B0
	};
const TInt KSvgImgCnt = 5;
// CLASS DECLARATION
/**
 * CtsvgappApplication
 *
 */
class CtsvgappApplication : public CSGEGameApplicationVG
	{

public:

	CtsvgappApplication();

	~CtsvgappApplication();

	void InitL();

	static CApaApplication* NewApplication();

	TUid ApplicationUid();

	void HandleCommandL(TInt aCommand);

	void Draw(const TRect& aRect) const;

	TBool FullScreen() const;

	TInt SampleBuffers();

	TInt SampleSize();

	TInt DepthSize();

	TBool HandlePointerEventL( const TPointerEvent& aPointerEvent );

private:

	void DoGameLoopEvent(TTimeIntervalMicroSeconds32 aTimeInterval);

	TBool UpdateScene(TTimeIntervalMicroSeconds32 aTimeInterval);

	void DrawScene();

private:
	// data
	
	CVgSvgImage* iSvgImage[ KSvgImgCnt ];
	
	TInt iCurrentImage;

	TInt iCurrentImageW3C;
	
	};

#endif // __TSVGAPPAPPLICATION_H__
