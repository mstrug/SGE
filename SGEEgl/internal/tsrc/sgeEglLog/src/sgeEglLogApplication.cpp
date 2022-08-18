/*
 ============================================================================
 Name        : sgeEglLogApplication.cpp
 Author      : 
 Company     : 
 Version     : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : Main application class
 ============================================================================
 */

// INCLUDE FILES
#include "sgeEglLogApplication.h"
#include "Logger.h"
#include <vg/openvg.h>


CsgeEglLogApplication::~CsgeEglLogApplication()
	{
	iGoomSession.Close();
	}

// -----------------------------------------------------------------------------
// CsgeEglLogApplication::ApplicationUid()
// -----------------------------------------------------------------------------
//
TUid CsgeEglLogApplication::ApplicationUid()
	{
	return KUidsgeEglLog;
	}

// -----------------------------------------------------------------------------
// CsgeEglLogApplication::HandleCommandL()
// -----------------------------------------------------------------------------
//
void CsgeEglLogApplication::HandleCommandL( TInt aCommand )
	{
	if ( aCommand == 0x34FD )
		{
		GetLog( ETrue, ETrue );
        AlertWin( _L("Done"), _L("Config saved.") );
		}
	else if ( aCommand == 0x34FE )
		{
		GetLog( ETrue, EFalse );
        AlertWin( _L("Done"), _L("Config saved.") );
		}
	else if ( aCommand == 0x34FF )
		{
		GetLog( EFalse, EFalse );
        AlertWin( _L("Done"), _L("Config saved.") );
		}
	else
		{
		Exit();
		}
	}

// -----------------------------------------------------------------------------
// CsgeEglLogApplication::NewApplication()
// -----------------------------------------------------------------------------
//
CApaApplication* CsgeEglLogApplication::NewApplication()
	{
	return CSGEApplication::NewApplication(new CsgeEglLogApplication);
	}

// -----------------------------------------------------------------------------
// CsgeEglLogApplication::Draw()
// -----------------------------------------------------------------------------
//
void CsgeEglLogApplication::Draw(const TRect& aRect) const
	{
	// Get the standard graphics context
	CWindowGc& gc = SystemGc();

	// Gets the control's extent
	TRect drawRect(aRect);

	// Clears the screen
	gc.Clear(drawRect);
	
//    float clearColor[] = { 0, 0, 0, 1 };
    float clearColor[] = { 0.6f, 0.8f, 1.0f, 1.0f };
    vgSetfv( VG_CLEAR_COLOR, 4, clearColor );

    vgSeti( VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE );

	vgClear( 0, 0, 100, 100 );

    vgLoadIdentity();

	}

#define LOGATTR( attr ) 	ret = eglGetConfigAttrib( aDisplay, aConfig, attr, &value ); \
							if ( ret == EGL_FALSE ) Logger::LogStrNum( _L("c:\\data\\eglLog.txt"), _L(#attr ": error: "), eglGetError() ); \
							else { if ( attr == EGL_CONFIG_CAVEAT ) {\
								if ( value == EGL_NONE ) Logger::LogStrNum( _L("c:\\data\\eglLog.txt"), _L(#attr ": EGL_NONE "), value ); \
								else if ( value == EGL_SLOW_CONFIG ) Logger::LogStrNum( _L("c:\\data\\eglLog.txt"), _L(#attr ": EGL_SLOW_CONFIG "), value ); \
								else if ( value == EGL_NON_CONFORMANT_CONFIG ) Logger::LogStrNum( _L("c:\\data\\eglLog.txt"), _L(#attr ": EGL_NON_CONFORMANT_CONFIG "), value ); \
								else Logger::LogStrNum( _L("c:\\data\\eglLog.txt"), _L(#attr ": "), value ); }\
							else if ( attr == EGL_SURFACE_TYPE )  { \
								Logger::LogStrNum( _L("c:\\data\\eglLog.txt"), _L(#attr ": "), value ); \
								if ( value & EGL_PBUFFER_BIT ) Logger::LogStrNum( _L("c:\\data\\eglLog.txt"), _L("  EGL_PBUFFER_BIT "), EGL_PBUFFER_BIT ); \
								if ( value & EGL_PIXMAP_BIT ) Logger::LogStrNum( _L("c:\\data\\eglLog.txt"), _L("  EGL_PIXMAP_BIT "), EGL_PIXMAP_BIT ); \
								if ( value & EGL_WINDOW_BIT ) Logger::LogStrNum( _L("c:\\data\\eglLog.txt"), _L("  EGL_WINDOW_BIT "), EGL_WINDOW_BIT ); \
								if ( value & EGL_VG_COLORSPACE_LINEAR_BIT ) Logger::LogStrNum( _L("c:\\data\\eglLog.txt"), _L("  EGL_VG_COLORSPACE_LINEAR_BIT "), EGL_VG_COLORSPACE_LINEAR_BIT ); \
								if ( value & EGL_VG_ALPHA_FORMAT_PRE_BIT ) Logger::LogStrNum( _L("c:\\data\\eglLog.txt"), _L("  EGL_VG_ALPHA_FORMAT_PRE_BIT "), EGL_VG_ALPHA_FORMAT_PRE_BIT ); \
								if ( value & EGL_MULTISAMPLE_RESOLVE_BOX_BIT ) Logger::LogStrNum( _L("c:\\data\\eglLog.txt"), _L("  EGL_MULTISAMPLE_RESOLVE_BOX_BIT "), EGL_MULTISAMPLE_RESOLVE_BOX_BIT ); \
								if ( value & EGL_SWAP_BEHAVIOR_PRESERVED_BIT ) Logger::LogStrNum( _L("c:\\data\\eglLog.txt"), _L("  EGL_SWAP_BEHAVIOR_PRESERVED_BIT "), EGL_SWAP_BEHAVIOR_PRESERVED_BIT ); }\
							else if ( attr == EGL_TRANSPARENT_TYPE ) { \
								if ( value == EGL_NONE ) Logger::LogStrNum( _L("c:\\data\\eglLog.txt"), _L(#attr ": EGL_NONE "), value ); \
								else if ( value == EGL_TRANSPARENT_RGB ) Logger::LogStrNum( _L("c:\\data\\eglLog.txt"), _L(#attr ": EGL_TRANSPARENT_RGB "), value ); \
								else Logger::LogStrNum( _L("c:\\data\\eglLog.txt"), _L(#attr ": "), value ); }\
							else if ( attr == EGL_COLOR_BUFFER_TYPE ) { \
								if ( value == EGL_RGB_BUFFER ) Logger::LogStrNum( _L("c:\\data\\eglLog.txt"), _L(#attr ": EGL_RGB_BUFFER "), value ); \
								else if ( value == EGL_LUMINANCE_BUFFER ) Logger::LogStrNum( _L("c:\\data\\eglLog.txt"), _L(#attr ": EGL_LUMINANCE_BUFFER "), value ); \
								Logger::LogStrNum( _L("c:\\data\\eglLog.txt"), _L(#attr ": "), value ); }\
							else if ( attr == EGL_RENDERABLE_TYPE || attr == EGL_CONFORMANT ) { \
								Logger::LogStrNum( _L("c:\\data\\eglLog.txt"), _L(#attr ": "), value ); \
								if ( value & EGL_OPENGL_ES_BIT ) Logger::LogStrNum( _L("c:\\data\\eglLog.txt"), _L("  EGL_OPENGL_ES_BIT "), EGL_OPENGL_ES_BIT ); \
								if ( value & EGL_OPENVG_BIT ) Logger::LogStrNum( _L("c:\\data\\eglLog.txt"), _L("  EGL_OPENVG_BIT "), EGL_OPENVG_BIT ); \
								if ( value & EGL_OPENGL_ES2_BIT ) Logger::LogStrNum( _L("c:\\data\\eglLog.txt"), _L("  EGL_OPENGL_ES2_BIT "), EGL_OPENGL_ES2_BIT ); \
								if ( value & EGL_OPENGL_BIT ) Logger::LogStrNum( _L("c:\\data\\eglLog.txt"), _L("  EGL_OPENGL_BIT "), EGL_OPENGL_BIT ); } \
							else Logger::LogStrNum( _L("c:\\data\\eglLog.txt"), _L(#attr ": "), value ); } 


void CsgeEglLogApplication::DebugConfig( EGLDisplay aDisplay, EGLConfig aConfig, TBool aTestRc )
	{
	EGLint value;
	EGLBoolean ret;
	Logger::LogStr( _L("c:\\data\\eglLog.txt"), _L("eglGetConfigAttrib:") );
	LOGATTR( EGL_CONFIG_ID )
	LOGATTR( EGL_BUFFER_SIZE )
	LOGATTR( EGL_ALPHA_SIZE )
	LOGATTR( EGL_BLUE_SIZE )
	LOGATTR( EGL_GREEN_SIZE )
	LOGATTR( EGL_RED_SIZE )
	LOGATTR( EGL_DEPTH_SIZE )
	LOGATTR( EGL_STENCIL_SIZE )
	LOGATTR( EGL_CONFIG_CAVEAT ) // enum
	LOGATTR( EGL_MAX_PBUFFER_HEIGHT )
	LOGATTR( EGL_MAX_PBUFFER_PIXELS )
	LOGATTR( EGL_MAX_PBUFFER_WIDTH )
	LOGATTR( EGL_NATIVE_RENDERABLE )
	LOGATTR( EGL_NATIVE_VISUAL_ID )
	LOGATTR( EGL_NATIVE_VISUAL_TYPE )
	LOGATTR( EGL_PRESERVED_RESOURCES )
	LOGATTR( EGL_SAMPLES )
	LOGATTR( EGL_SAMPLE_BUFFERS )
	LOGATTR( EGL_SURFACE_TYPE )	// bitmask
	LOGATTR( EGL_TRANSPARENT_TYPE ) // enum
	LOGATTR( EGL_TRANSPARENT_BLUE_VALUE )
	LOGATTR( EGL_TRANSPARENT_GREEN_VALUE )
	LOGATTR( EGL_TRANSPARENT_RED_VALUE )
	LOGATTR( EGL_BIND_TO_TEXTURE_RGB )
	LOGATTR( EGL_BIND_TO_TEXTURE_RGBA )
	LOGATTR( EGL_MIN_SWAP_INTERVAL )
	LOGATTR( EGL_MAX_SWAP_INTERVAL )
	LOGATTR( EGL_LUMINANCE_SIZE )
	LOGATTR( EGL_ALPHA_MASK_SIZE )
	LOGATTR( EGL_COLOR_BUFFER_TYPE ) // enum
	LOGATTR( EGL_RENDERABLE_TYPE ) // bitmask
	LOGATTR( EGL_MATCH_NATIVE_PIXMAP )
	LOGATTR( EGL_CONFORMANT ) // bitmask
	
	if ( aTestRc )
		{
		const EGLint attrib_list[] = { EGL_NONE };
		EGLContext cntx = eglCreateContext( aDisplay, aConfig, NULL, attrib_list );
		if ( cntx == EGL_NO_CONTEXT )
			{
			Logger::LogStrNum( _L("c:\\data\\eglLog.txt"), _L("eglCreateContext: error: "), eglGetError() );
			}
		else
			{
			Logger::LogStr( _L("c:\\data\\eglLog.txt"), _L("eglCreateContext: OK") );
			eglDestroyContext( aDisplay, aConfig );
			}
		}
	}

void CsgeEglLogApplication::InitL()
	{
	User::LeaveIfError( iGoomSession.Connect() );
	Logger::LogStr( _L("c:\\data\\eglLog.txt"), _L("iGoomSession.Connect()"), EFalse );
	iGoomSession.ApplicationAboutToStart( ApplicationUid() );
	if ( iGoomSession.RequestFreeMemory( 16 * 1024 * 1024 ) != KErrNone )
		{
		Logger::LogStr( _L("c:\\data\\eglLog.txt"), _L("iGoomSession.RequestFreeMemory( 16 * 1024 * 1024 ): failed") );
		if ( iGoomSession.RequestFreeMemory( 8 * 1024 * 1024 ) != KErrNone )
			{
			Logger::LogStr( _L("c:\\data\\eglLog.txt"), _L("iGoomSession.RequestFreeMemory( 8 * 1024 * 1024 ): failed") );
			if ( iGoomSession.RequestFreeMemory( 1024 * 1024 ) != KErrNone )
				{
				Logger::LogStr( _L("c:\\data\\eglLog.txt"), _L("iGoomSession.RequestFreeMemory( 1024 * 1024 ): failed") );
				}
			else
				{
				Logger::LogStr( _L("c:\\data\\eglLog.txt"), _L("iGoomSession.RequestFreeMemory( 1024 * 1024 ): OK") );
				}
			}
		else
			{
			Logger::LogStr( _L("c:\\data\\eglLog.txt"), _L("iGoomSession.RequestFreeMemory( 8 * 1024 * 1024 ): OK") );
			}
		}
	else
		{
		Logger::LogStr( _L("c:\\data\\eglLog.txt"), _L("iGoomSession.RequestFreeMemory( 16 * 1024 * 1024 ): OK") );
		}
	}

void CsgeEglLogApplication::GetLog( TBool aTestRc, TBool aChooseConfig )
	{
	EGLDisplay display = eglGetDisplay( EGL_DEFAULT_DISPLAY );
    if ( display == EGL_NO_DISPLAY )
        {
    	Logger::LogStr( _L("c:\\data\\eglLog.txt"), _L("eglGetDisplay: EGL_NO_DISPLAY") );
    	return;
        }
	Logger::LogStr( _L("c:\\data\\eglLog.txt"), _L("eglGetDisplay: OK") );
    
    EGLint vMajor;
    EGLint vMinor;
    EGLBoolean ret = eglInitialize( display, &vMajor, &vMinor );
    if ( ret == EGL_FALSE )
    	{
        EGLint error = eglGetError();
    	Logger::LogStrNum( _L("c:\\data\\eglLog.txt"), _L("eglInitialize: error: "), error );
    	return;
    	}
    else
    	{
    	Logger::LogStrNum( _L("c:\\data\\eglLog.txt"), _L("eglGetDisplay: major: "), vMajor );
    	Logger::LogStrNum( _L("c:\\data\\eglLog.txt"), _L("eglGetDisplay: minor: "), vMinor ); 	
    	}
    
    EGLint num_config;
    ret = eglGetConfigs( display, NULL, 0, &num_config );
    if ( ret == EGL_FALSE )
    	{
        EGLint error = eglGetError();
    	Logger::LogStrNum( _L("c:\\data\\eglLog.txt"), _L("eglGetConfigs(NULL): error: "), error );
    	return;
    	}
    else
    	{
    	Logger::LogStrNum( _L("c:\\data\\eglLog.txt"), _L("eglGetConfigs(NULL): num_config: "), num_config );
    	}
    
    EGLConfig* configs = (EGLConfig*) User::Alloc( num_config * sizeof( EGLConfig ) );
    ret = eglGetConfigs( display, configs, num_config, &num_config );
    if ( ret == EGL_FALSE )
    	{
        EGLint error = eglGetError();
    	Logger::LogStrNum( _L("c:\\data\\eglLog.txt"), _L("eglGetConfigs: error: "), error );
    	return;
    	}
    else
    	{
    	Logger::LogStrNum( _L("c:\\data\\eglLog.txt"), _L("eglGetConfigs: num_config: "), num_config );
    	for ( TInt i = 0; i < num_config; i++ )
    		{
        	Logger::LogStrNum( _L("c:\\data\\eglLog.txt"), _L("\r\neglGetConfigs: config index: "), i );
    		DebugConfig( display, configs[ i ], aTestRc );    		
    		}
    	}

    if ( aChooseConfig )
    	{
		const EGLint myCfg[] = 
			{ 
			EGL_NATIVE_RENDERABLE, EGL_TRUE,
			EGL_NONE 
			};
		ret = eglChooseConfig( display, myCfg, configs, num_config, &num_config );
		if ( ret == EGL_FALSE )
			{
			EGLint error = eglGetError();
			Logger::LogStrNum( _L("c:\\data\\eglLog.txt"), _L("\r\neglChooseConfig: error: "), error );
			return;
			}
		else
			{
			Logger::LogStrNum( _L("c:\\data\\eglLog.txt"), _L("\r\neglChooseConfig: num_config: "), num_config );
			for ( TInt i = 0; i < num_config; i++ )
				{
				Logger::LogStrNum( _L("c:\\data\\eglLog.txt"), _L("\r\neglGetConfigs: config index: "), i );
				DebugConfig( display, configs[ i ], aTestRc );    		
				}
			}
    	}
    
    User::Free( configs );
    
    eglTerminate( display );
	}


// End of file
