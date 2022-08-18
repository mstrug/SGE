/*
 ============================================================================
  Name        : SGEEngine.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#ifndef __SGEENGINE_H__
#define __SGEENGINE_H__

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <W32STD.H>
#include <COEDEF.H>
#include <SGEEngine/SGEObjectInterfaces.h>
#include <SGEAudio/SGEAudioProvider.h>
#include <SGEBitmap/SGEBitmapProvider.h>
#include <SGEGl/GlTextureManager.h>
#include <SGEVg/VgImageManager.h>

// FORWARD DECLARATIONS
class CSGEEngineCommonObjectsBase;
class MSGEEngineObserver;

// CLASS DECLARATIONS
/**
 * CSGEEngine
 * @todo: add setting iSize by constructor
 */
class CSGEEngine : public CBase, public MSGEDrawableObject, public MSGEUpdatableObject, 
                   public MSGEKeyEventableObject, public MSGEPointerEventableObject,
                   public MSGEAudioProviderObserver, public MSGEBitmapProviderObserver,
                   public MGlTextureManagerObserver, public MVgImageManagerObserver
    {
public: // destruction
    
    IMPORT_C ~CSGEEngine();

public: // abstract functions
    
    virtual TUint Id() const = 0;

public: // new functions
    
    // empty implementation
    IMPORT_C virtual void Activate();

    // empty implementation
    IMPORT_C virtual void Deactivate();

    // empty implementation
    IMPORT_C virtual void SetPause( TBool aPause );

    // empty implementation
    IMPORT_C virtual void SizeChanged( TSize aNewSize );

protected: // construction
    
    IMPORT_C CSGEEngine( MSGEEngineObserver& aObserver, CSGEEngineCommonObjectsBase *aCommonObjects );

    IMPORT_C CSGEEngine( MSGEEngineObserver& aObserver, CSGEEngineCommonObjectsBase *aCommonObjects, 
                         TSize aSize, TDisplayMode aDisplayMode );

    IMPORT_C void BaseConstructL();

public: // from MSGEDrawableObject

    // should be implemented in derived class
    // virtual void Draw( CBitmapContext* aGc, const TRect aDrawRect ) = 0; 

public: // from MSGEUpdatableObject

    // should be implemented in derived class
    // virtual TBool Update( TTimeIntervalMicroSeconds32 aTimeInterval ) = 0; 

public: // from MSGEKeyEventableObject

    // empty implementation
    IMPORT_C virtual TBool KeyDownEvent( const TKeyEvent &aKeyEvent ); 

    // empty implementation
    IMPORT_C virtual TBool KeyUpEvent( const TKeyEvent &aKeyEvent ); 

    // empty implementation
    IMPORT_C virtual TBool KeyEvent( const TKeyEvent &aKeyEvent );

public: // from MSGEPointerEventableObject

    // empty implementation
    IMPORT_C virtual TBool PointerEvent( const TPointerEvent &aPointerEvent ); 

public: // from MSGEAudioProviderObserver
    
    // empty implementation
    IMPORT_C virtual void AudioFileLoaded( TInt aSoundSampleId, TInt aError );
    
    // empty implementation
    IMPORT_C virtual void AudioAllFilesLoaded();

    // empty implementation
    IMPORT_C virtual void AudioSoundTrackPlayStarted( TUint aTrackId );

    // empty implementation
    IMPORT_C virtual void AudioSoundTrackPlayFinished( TUint aTrackId );

    // empty implementation
    IMPORT_C virtual void AudioStreamCreated( TInt aError );

    // empty implementation
    IMPORT_C virtual void AudioStreamError( TInt aError );

public: // from MSGEBitmapProviderObserver
    
    // empty implementation
    IMPORT_C virtual void BitmapLoaded( TUint aBitmapId, TInt aError );

    // empty implementation
    IMPORT_C virtual void BitmapsLoadedAll();

public: // from MGlTextureManagerObserver

    // empty implementation
    IMPORT_C virtual void TextureCreated( TUint aTextureId, TInt aError );

    // empty implementation
    IMPORT_C virtual void TexturesCreatedAll();

public: // from MVgImageManagerObserver
    
    // empty implementation
    IMPORT_C virtual void VgImageLoaded( TUint aImageId, TInt aError );

    // empty implementation
    IMPORT_C virtual void VgAllImagesLoaded();
    
protected: // data
    
    MSGEEngineObserver &iObserver;
    
    CSGEEngineCommonObjectsBase *iCommonObjects; // not owned

    TSize iSize;

    TDisplayMode iDisplayMode;
    
    };


#endif // __SGEENGINECONTROLLER_H__
