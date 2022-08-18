/*
 ============================================================================
  Name        : SGEEngine.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#include "SGEEngine.h"
#include "SGEEngineObserver.h"
#include "SGEEngineCommonObjects.h"


// -----------------------------------------------------------------------------
// CSGEEngine::CSGEEngine()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEEngine::CSGEEngine( MSGEEngineObserver& aObserver, CSGEEngineCommonObjectsBase *aCommonObjects )
        : iObserver( aObserver ), iCommonObjects( aCommonObjects )
    {
    }

// -----------------------------------------------------------------------------
// CSGEEngine::CSGEEngine()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEEngine::CSGEEngine( MSGEEngineObserver& aObserver, CSGEEngineCommonObjectsBase *aCommonObjects, 
                                 TSize aSize, TDisplayMode aDisplayMode )
        : iObserver( aObserver ), iCommonObjects( aCommonObjects ), iSize( aSize ), iDisplayMode( aDisplayMode )
    {
    }

// -----------------------------------------------------------------------------
// CSGEEngine::CSGEEngine()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEEngine::~CSGEEngine()
    {
    }

// -----------------------------------------------------------------------------
// CSGEEngine::BaseConstructL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEEngine::BaseConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CSGEEngine::Activate()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEEngine::Activate()
    {
    }

// -----------------------------------------------------------------------------
// CSGEEngine::Dectivate()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEEngine::Deactivate()
    {
    }

// -----------------------------------------------------------------------------
// CSGEEngine::SetPause()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEEngine::SetPause( TBool /*aPause*/ )
    {
    }

// -----------------------------------------------------------------------------
// CSGEEngine::SizeChanged()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEEngine::SizeChanged( TSize aNewSize )
    {
    iSize = aNewSize;
    }

// -----------------------------------------------------------------------------
// CSGEEngine::KeyDownEvent()
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSGEEngine::KeyDownEvent( const TKeyEvent& /*aKeyEvent*/ )
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CSGEEngine::KeyUpEvent()
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSGEEngine::KeyUpEvent( const TKeyEvent& /*aKeyEvent*/ )
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CSGEEngine::KeyEvent()
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSGEEngine::KeyEvent( const TKeyEvent& /*aKeyEvent*/ )
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CSGEEngine::PointerEvent()
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSGEEngine::PointerEvent( const TPointerEvent& /*aPointerEvent*/ )
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CSGEEngine::AudioFileLoaded()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEEngine::AudioFileLoaded( TInt /*aSoundSampleId*/, TInt /*aError*/ )
    {    
    }

// -----------------------------------------------------------------------------
// CSGEEngine::AudioAllFilesLoaded()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEEngine::AudioAllFilesLoaded()
    {    
    }

// -----------------------------------------------------------------------------
// CSGEEngine::AudioSoundTrackPlayStarted()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEEngine::AudioSoundTrackPlayStarted( TUint /*aTrackId*/ )
    {    
    }

// -----------------------------------------------------------------------------
// CSGEEngine::AudioSoundTrackPlayFinished()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEEngine::AudioSoundTrackPlayFinished( TUint /*aTrackId*/ )
    {    
    }

// -----------------------------------------------------------------------------
// CSGEEngine::AudioStreamCreated()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEEngine::AudioStreamCreated( TInt /*aError*/ )
    {    
    }

// -----------------------------------------------------------------------------
// CSGEEngine::AudioStreamError()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEEngine::AudioStreamError( TInt /*aError*/ )
    {    
    }

// -----------------------------------------------------------------------------
// CSGEEngine::BitmapLoaded()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEEngine::BitmapLoaded( TUint /*aBitmapId*/, TInt /*aError*/ )
    {
    }

// -----------------------------------------------------------------------------
// CSGEEngine::BitmapsLoadedAll()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEEngine::BitmapsLoadedAll()
    {
    }

// -----------------------------------------------------------------------------
// CSGEEngine::TextureCreated()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEEngine::TextureCreated( TUint /*aTextureId*/, TInt /*aError*/ )
    {
    }

// -----------------------------------------------------------------------------
// CSGEEngine::TexturesCreatedAll()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEEngine::TexturesCreatedAll()
    {
    }

// -----------------------------------------------------------------------------
// CSGEEngine::VgImageLoaded()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEEngine::VgImageLoaded( TUint /*aImageId*/, TInt /*aError*/ )
    {
    }

// -----------------------------------------------------------------------------
// CSGEEngine::VgAllImagesLoaded()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEEngine::VgAllImagesLoaded()
    {
    }


// End of file
