/*
 ============================================================================
 Name		: EglDevice.h
 Author	  : Michal Strug
 Version	 : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEglDevice declaration
 ============================================================================
 */

#ifndef EGLDEVICE_H
#define EGLDEVICE_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <GDI.H>
#include <gles/egl.h>


// CLASS DECLARATION

/**
 *  CEglDevice
 * 
 * Usage:
 * 
 * CEglDevice *device = CEglDevice::NewL();
 * device->Configure();
 * 
 * 
 */
NONSHARABLE_CLASS( CEglDevice ) : public CBase
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    EXPORT_C ~CEglDevice();

    /**
     * Two-phased constructor.
     */
    EXPORT_C static CEglDevice* NewL();

    /**
     * Two-phased constructor.
     */
    EXPORT_C static CEglDevice* NewLC();

    EXPORT_C EGLDisplay Display() const;

    EXPORT_C EGLConfig Config() const;
    
//    
// 3.4 Configuration Management
//
    /*
     * Chooses default config
     * @return KErrNone if successfull, otherwise system error 
     */
    EXPORT_C TInt Configure( TBool aWindow, TBool aPixmap, TBool aPBuffer );

    /*
     * @param aDepthBufferSize sets EGL attribute EGL_BUFFER_SIZE, colors bits 
     * @param aDepthSize sets EGL attribute EGL_DEPTH_SIZE, bits
     * @param aMultisampleBuffersCount sets EGL attribute EGL_SAMPLE_BUFFERS, 0 or 1
     * @param aSamplesPerPixel sets EGL attribute EGL_SAMPLES, bits
     * @param aStencilBufferSize sets EGL attribute EGL_STENCIL_SIZE, bits
     * @param aWindow EGL_WINDOW_BIT
     * @param aPixmap EGL_PIXMAP_BIT
     * @param aPBuffer EGL_PBUFFER_BIT
     * @param aBindToTexture EGL_BIND_TO_TEXTURE_RGB
     * @param aBindToTextureWithAlpha EGL_BIND_TO_TEXTURE_RGBA
     * @return KErrNone if successfull, otherwise system error
     */
    EXPORT_C TInt Configure( TInt aDepthBufferSize, TInt aDepthSize, 
                             TInt aMultisampleBuffersCount, TInt aSamplesPerPixel,
                             TInt aStencilBufferSize, TBool aWindow, TBool aPixmap, TBool aPBuffer,
                             TBool aBindToTexture, TBool aBindToTextureWithAlpha );

    /*
     * @param aConfig config passed to the eglChooseConfig() function
     * @param aAlternativeConfig config used in case of failing choose aConfig
     * @return KErrNone if successfull, otherwise system error
     */
    EXPORT_C TInt Configure( const EGLint *aConfig, const EGLint *aAlternativeConfig = NULL );

private:

    /**
     * Constructor for performing 1st stage construction
     */
    CEglDevice();

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();

private: // new functions
    
    TInt Initialize();      // 3.2 Initialization

    void Terminate();      // 3.2 Initialization

private: // data
    
    EGLDisplay iDisplay;

    EGLConfig iConfig;
    
    };

#endif // EGLDEVICE_H
