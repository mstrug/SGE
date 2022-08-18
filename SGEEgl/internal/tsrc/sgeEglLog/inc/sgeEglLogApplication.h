/*
 ============================================================================
 Name        : sgeEglLogApplication.h
 Author      : 
 Company     : 
 Version     : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : Declares main application class.
 ============================================================================
 */

#ifndef __SGEEGLLOGAPPLICATION_H__
#define __SGEEGLLOGAPPLICATION_H__

// INCLUDES
#include <SGEApplication.h>
#include <SGEEgl/EglRc.h>
#include <SGEEgl/EglDevice.h>
#include <goommonitorsession.h>




// CONSTANTS
// UID of the application
const TUid KUidsgeEglLog =
	{
	0xE6FE9B1A
	};

// CLASS DECLARATION
/**
 * CsgeEglLogApplication
 *
 */
class CsgeEglLogApplication : public CSGEApplication
	{

public:

	~CsgeEglLogApplication();
	
	/*
	 * Application Uid getter
	 * @return Application Uid
	 */
	TUid ApplicationUid();

	/*
	 * Command handler
	 * @param aCommand Command identifier
	 */
	void HandleCommandL(TInt aCommand);

	/*
	 * Function used to launch application
	 * @return Newly created system application object
	 */
	static CApaApplication* NewApplication();

	/*
	 * Draws application background
	 * @param aRect drawing rectangle
	 */
	void Draw(const TRect& aRect) const;

    void InitL();
    
    void GetLog( TBool aTestRc, TBool aChooseConfig );

    void GetNativeCfgLog();

    void DebugConfig( EGLDisplay aDisplay, EGLConfig aConfig, TBool aTestRc );

private:
    
    RGOomMonitorSession iGoomSession;
	
	
	};

#endif // __SGEEGLLOGAPPLICATION_H__
