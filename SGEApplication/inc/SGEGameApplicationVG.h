/*
 ============================================================================
  Name        : SGEGameApplicationVG.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#ifndef __SGEGAMEAPPLICATIONVG_H__
#define __SGEGAMEAPPLICATIONVG_H__

// INCLUDES
#include <SGEApplication/SGEGameApplicationGL.h>

/**
 *  CSGEGameApplicationVG
 * 
 */
class CSGEGameApplicationVG : public CSGEGameApplicationGL
    {

public:

    /*
     * This functions returns EGL rendering context for OpenGL.
     * @return egl rc only when SupportGL() function returns true else NULL
     */
    IMPORT_C CEglRc *EglRenderingContext() const;
    
    /*
     * This functions returns EGL rendering context for OpenVG.
     * @return egl rc
     */
    IMPORT_C CEglRc *EglRenderingContextVG() const;
    
    /*
     * This functions makes current rendering context
     * created for window surface for OpenGL.
     * Only when SupportGL() function returns true
     * @return KErrNone or system error
     */
    IMPORT_C TInt MakeCurrentWindowEglContext();

    /*
     * This functions makes current rendering context
     * created for window surface for OpenVG.
     * @return KErrNone or system error
     */
    IMPORT_C TInt MakeCurrentWindowEglContextVG();

public: // derive in client class
    
    /*
     * If return true, then also OpenGL ES rendering is supported.
     * By default returns EFalse. Should be implemented by derived class.
     */
    IMPORT_C virtual TBool SupportGL() const;

public: // internal functions
    
    IMPORT_C CSGESystemAppView* CreateAppViewL( TRect aRect );

private: // data

    };

#endif // __SGEGAMEAPPLICATIONVG_H__
