/*
 ============================================================================
  Name        : SGEGameAppViewGL.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#ifndef __SGEGAMEAPPVIEWGL_H__
#define __SGEGAMEAPPVIEWGL_H__

// INCLUDES
#include "SGESystemAppView.h"


// FORWARD DECLARATIONS
class CSGEGameApplication;
class CEglDevice;
class CEglSurface;
class CEglRc;

const TInt KDepthSize = 16;
const TInt KSampleBuffers = 1;
const TInt KSampleSize = 4;

// CLASS DECLARATION
/**
 *  CSGEGameAppViewGL
 * 
 */
NONSHARABLE_CLASS( CSGEGameAppViewGL ) : public CSGESystemAppView
    {
public:
    
    enum TEglSurfaceType
        {
        EEglSurfaceWindow,
        EEglSurfacePixmap,
        EEglSurfacePbuffer
        };
    
public:

    static CSGEGameAppViewGL* NewL( const TRect& aRect, TEglSurfaceType aEglSurfaceType, 
                                    CSGEGameApplication& aApplication );

    static CSGEGameAppViewGL* NewLC( const TRect& aRect, TEglSurfaceType aEglSurfaceType,
                                     CSGEGameApplication& aApplication );

    static CSGEGameAppViewGL* NewL( const TRect& aRect, TEglSurfaceType aEglSurfaceType, TInt aDepthSize, TInt aSampleBuffers, TInt aSampleSize,
                                    CSGEGameApplication& aApplication );

    virtual ~CSGEGameAppViewGL();

    void SwapBuffers();

    CEglDevice *EglDevice() const;
    
    CEglSurface *EglSurface() const;

    CEglRc *EglRenderingContext() const;
    
private: // from base class
    
    void SizeChanged();

protected:

    void ConstructL( const TRect& aRect, TBool aOpenVGSupport );

    CSGEGameAppViewGL( TEglSurfaceType aEglSurfaceType, TInt aDepthSize, TInt aSampleBuffers, TInt aSampleSize, CSGEGameApplication& aApplication );
    
    void ConstructGLL( TBool aOpenVGSupport );
    
    virtual void InitViewport();

protected:
    
    CEglDevice *iDevice;

    CEglSurface *iSurface;
    
    CEglRc *iRc;
    
    TEglSurfaceType iEglSurfaceType;
    
    /**
     * Flag that indicates if OpenGL ES has been initialized or not.
     * Used to check if SizeChanged() can react to incoming notifications.
     */
    TBool iOpenGlInitialized;

    TInt iDepthSize;

    TInt iSampleBuffers;

    TInt iSampleSize;

    };

#endif // __SGEGAMEAPPVIEWGL_H__
