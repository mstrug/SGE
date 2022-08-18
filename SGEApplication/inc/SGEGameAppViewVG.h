/*
 ============================================================================
  Name        : SGEGameAppViewVG.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#ifndef __SGEGAMEAPPVIEWVG_H__
#define __SGEGAMEAPPVIEWVG_H__

// INCLUDES
#include "SGEGameAppViewGL.h"


// CLASS DECLARATION
/**
 *  CSGEGameAppViewVG
 * 
 */
NONSHARABLE_CLASS( CSGEGameAppViewVG ) : public CSGEGameAppViewGL
    {
    
public:

    static CSGEGameAppViewVG* NewL( const TRect& aRect, TEglSurfaceType aEglSurfaceType, 
                                    CSGEGameApplication& aApplication );

    static CSGEGameAppViewVG* NewLC( const TRect& aRect, TEglSurfaceType aEglSurfaceType,
                                     CSGEGameApplication& aApplication );

    static CSGEGameAppViewVG* NewL( const TRect& aRect, TEglSurfaceType aEglSurfaceType, TInt aDepthSize, TInt aSampleBuffers, TInt aSampleSize,
                                    CSGEGameApplication& aApplication );

    virtual ~CSGEGameAppViewVG();

    CEglRc *EglRenderingContextVG() const;
    
private:

    void ConstructL( const TRect& aRect );

    CSGEGameAppViewVG( TEglSurfaceType aEglSurfaceType, TInt aDepthSize, TInt aSampleBuffers, TInt aSampleSize, CSGEGameApplication& aApplication );
    
    void InitViewport();

private:
    
    CEglRc *iRcVG;
    
    };

#endif // __SGEGAMEAPPVIEWVG_H__
