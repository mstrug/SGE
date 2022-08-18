/*
 ============================================================================
 Name		: GlUtils.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : GlUtils declaration
 ============================================================================
 */

#ifndef GLUTILS_H
#define GLUTILS_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

// CLASS DECLARATION

/**
 *  GlUtils
 * 
 */
class GlUtils
    {
public:
    
    /**
     * TRANSPARENCY (blending)
     */
    static void EnableTransparency();

    static void DisableTransparency();
    
    /**
     * TRANSPARENCY (alpha test)
     */
    static void EnableAlphaTest();

    static void DisableAlphaTest();

    /**
     * MULTISAMPLING
     */
    static void EnableMultisampling();
    
    static void DisableMultisampling();

    static TBool IsMultisamplingSupported();

    /**
     * SHADE MODEL
     */
    static void SetShadeModelFlat();

    static void SetShadeModelSmooth();

    /**
     * HINTS
     */
    static void SetPerspectiveCorrectionFastest();

    static void SetPerspectiveCorrectionNicest();
    
    static void UnsetPerspectiveCorrection();

    /**
     * CLEARING
     */
    static void SetBlackClearColor();

    static void ClearColor();
    
    /**
     * TEXTURING
     */
    static void EnableTexturing();

    static void DisableTexturing();

    static void UnbindTexture( TBool aResetMatrix = EFalse );
    
    /**
     * VIEW
     */
    static void SetViewportAsScreen( TSize aScreenSize );

    static void SetSceneSizeAsScreen( TSize aScreenSize );
    
    /**
     * MATRIX
     */
    static void LoadIdentity();
    
    static void Translate2D( TPoint aVector );

    static void Scale2D( TSize aVector );

    /**
     * INFO
     */
    static TPtrC8 GetExtensions();

    /**
     * TEST DRAW
     * draws rectangle { ( 0, 0 ) ( 200, 200 ) }
     */
    static void TestDraw();

    static TBool TestBlendFunc( TInt aIdx );

    };

#endif // GLUTILS_H
