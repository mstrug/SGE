/*
 ============================================================================
  Name        : SGEGameApplicationGL.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#ifndef __SGEGAMEAPPLICATIONGL_H__
#define __SGEGAMEAPPLICATIONGL_H__

// INCLUDES
#include <SGEApplication/SGEGameApplication.h>

// CLASS DECLARATION

class CEglDevice;
class CEglSurface;
class CEglRc;

/**
 *  CSGEGameApplicationGL
 * 
 */
class CSGEGameApplicationGL : public CSGEGameApplication
    {

public:
    
    /*
     * This functions returns size of the screen.
     * @return size
     */
    IMPORT_C TSize Size() const;
    
    /*
     * This functions returns EGL device.
     * @return egl device
     */
    IMPORT_C CEglDevice *EglDevice() const;
    
    /*
     * This functions returns EGL surface.
     * @return egl surface
     */
    IMPORT_C CEglSurface *EglSurface() const;
    
    /*
     * This functions returns EGL rendering context.
     * @return egl rc
     */
    IMPORT_C CEglRc *EglRenderingContext() const;
    
    /*
     * This functions makes current rendering context
     * created for window surface.
     * @return KErrNone or system error
     */
    IMPORT_C TInt MakeCurrentWindowEglContext();

    /*
     * Function used when surface type is sat as EEglSurfacePixmap.
     * It should be implemented by derived class and should return created CWsBitmap pointer.
     * @return by default NULL
     */
    IMPORT_C virtual CWsBitmap* WsBitmap();

protected:
    
    enum TEglSurfaceType
        {
        EEglSurfaceWindow,
        EEglSurfacePixmap,
        EEglSurfacePbuffer
        };
    
    /*
     * Function called during initialization with specific EGL Surface.
     * Implement in derived class to use different surfaces.
     * @return by default EEglSurfaceWindow
     */
    IMPORT_C virtual TEglSurfaceType SurfaceType();

    /*
     * Function called during initialization with EGL device. Be default returns 16.
     */
    IMPORT_C virtual TInt DepthSize();

    /*
     * Function called during initialization with EGL device (FSAA). Be default returns 1.
     */
    IMPORT_C virtual TInt SampleBuffers();

    /*
     * Function called during initialization with EGL device (FSAA). Be default returns 4.
     */
    IMPORT_C virtual TInt SampleSize();

public: // internal functions

    IMPORT_C void GameLoopEvent();
    
    IMPORT_C CSGESystemAppView* CreateAppViewL( TRect aRect );

private: // data

    };

#endif // __SGEGAMEAPPLICATIONGL_H__
