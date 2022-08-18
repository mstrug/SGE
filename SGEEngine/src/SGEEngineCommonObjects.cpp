/*
 ============================================================================
  Name        : SGEEngineCommonObjects.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#include "SGEEngineCommonObjects.h"
#include <SGEBitmapProvider.h>
#include <SGEFontProvider.h>
#include <SGEFileProvider.h>
#include <SGEStringProvider.h>
#include <SGEAudioProvider.h>



CSGEBitmapProvider* CSGEEngineCommonObjectsBase::BitmapProvider() const
	{
	return NULL;
	}
    
CSGEAudioProvider* CSGEEngineCommonObjectsBase::AudioProvider() const
	{
	return NULL;
	}

    
    
// -----------------------------------------------------------------------------
// CSGEEngineCommonObjects::CSGEEngineCommonObjects()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEEngineCommonObjects::CSGEEngineCommonObjects()
    {
    }

// -----------------------------------------------------------------------------
// CSGEEngineCommonObjects::~CSGEEngineCommonObjects()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEEngineCommonObjects::~CSGEEngineCommonObjects()
    {
    delete iFileProvider;
    delete iBitmapProvider;
    delete iFontProvider;
    delete iStringProvider;
    delete iAudioProvider;
    }

// -----------------------------------------------------------------------------
// CSGEEngineCommonObjects::SetFileProvider()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEEngineCommonObjects::SetFileProvider( CSGEFileProvider *aFileProvider )
    {
    delete iFileProvider;
    iFileProvider = aFileProvider;
    }

// -----------------------------------------------------------------------------
// CSGEEngineCommonObjects::SetBitmapProvider()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEEngineCommonObjects::SetBitmapProvider( CSGEBitmapProvider *aBitmapProvider )
    {
    delete iBitmapProvider;
    iBitmapProvider = aBitmapProvider;
    }

// -----------------------------------------------------------------------------
// CSGEEngineCommonObjects::SetFontProvider()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEEngineCommonObjects::SetFontProvider( CSGEFontProvider *aFontProvider )
    {
    delete iFontProvider;
    iFontProvider = aFontProvider;
    }

// -----------------------------------------------------------------------------
// CSGEEngineCommonObjects::SetStringProvider()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEEngineCommonObjects::SetStringProvider( CSGEStringProvider *aStringProvider )
    {
    delete iStringProvider;
    iStringProvider = aStringProvider;
    }

// -----------------------------------------------------------------------------
// CSGEEngineCommonObjects::SetAudioProvider()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEEngineCommonObjects::SetAudioProvider( CSGEAudioProvider *aAudioProvider )
    {
    delete iAudioProvider;
    iAudioProvider = aAudioProvider;
    }

// -----------------------------------------------------------------------------
// CSGEEngineCommonObjects::SetApplication()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEEngineCommonObjects::SetApplication( CSGEApplication *aApplication )
    {
    iApplication = aApplication;
    }

// -----------------------------------------------------------------------------
// CSGEEngineCommonObjects::FileProvider()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEFileProvider* CSGEEngineCommonObjects::FileProvider() const
    {
    return iFileProvider;
    }

// -----------------------------------------------------------------------------
// CSGEEngineCommonObjects::BitmapProvider()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEBitmapProvider* CSGEEngineCommonObjects::BitmapProvider() const
    {
    return iBitmapProvider;
    }

// -----------------------------------------------------------------------------
// CSGEEngineCommonObjects::FontProvider()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEFontProvider* CSGEEngineCommonObjects::FontProvider() const
    {
    return iFontProvider;
    }

// -----------------------------------------------------------------------------
// CSGEEngineCommonObjects::StringProvider()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEStringProvider* CSGEEngineCommonObjects::StringProvider() const
    {
    return iStringProvider;
    }

// -----------------------------------------------------------------------------
// CSGEEngineCommonObjects::AudioProvider()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEAudioProvider* CSGEEngineCommonObjects::AudioProvider() const
    {
    return iAudioProvider;
    }

// -----------------------------------------------------------------------------
// CSGEEngineCommonObjects::Application()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEApplication* CSGEEngineCommonObjects::Application() const
    {
    return iApplication;
    }


// End of file
