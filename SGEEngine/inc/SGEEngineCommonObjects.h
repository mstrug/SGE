/*
 ============================================================================
  Name        : SGEEngineCommonObjects.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#ifndef __SGEENGINECOMMONOBJECTS_H__
#define __SGEENGINECOMMONOBJECTS_H__

// INCLUDES
#include <e32std.h>
#include <e32base.h>

// FORWARD DECLARATIONS
class CSGEFileProvider;
class CSGEBitmapProvider;
class CSGEFontProvider;
class CSGEStringProvider;
class CSGEAudioProvider;
class CSGEApplication;

// CLASS DECLARATION

/**
 * CSGEEngineCommonObjectsBase
 * Empty class for derivation
 */
class CSGEEngineCommonObjectsBase : public CBase
    {
public:
    virtual CSGEBitmapProvider* BitmapProvider() const;
    
    virtual CSGEAudioProvider* AudioProvider() const;
    };


/**
 * CSGEEngineCommonObjects
 * Common objects used by all engines.
 */
class CSGEEngineCommonObjects : public CSGEEngineCommonObjectsBase
    {
public: // construction and destruction
    
    IMPORT_C CSGEEngineCommonObjects();
    
    IMPORT_C ~CSGEEngineCommonObjects();

public: // new functions

    IMPORT_C void SetFileProvider( CSGEFileProvider *aFileProvider );
    
    IMPORT_C void SetBitmapProvider( CSGEBitmapProvider *aBitmapProvider );
    
    IMPORT_C void SetFontProvider( CSGEFontProvider *aFontProvider );
    
    IMPORT_C void SetStringProvider( CSGEStringProvider *aStringProvider );
    
    IMPORT_C void SetAudioProvider( CSGEAudioProvider *aAudioProvider );

    IMPORT_C void SetApplication( CSGEApplication *aApplication );
    
    IMPORT_C CSGEFileProvider* FileProvider() const;
    
    IMPORT_C CSGEBitmapProvider* BitmapProvider() const;
    
    IMPORT_C CSGEFontProvider* FontProvider() const;

    IMPORT_C CSGEStringProvider* StringProvider() const;
    
    IMPORT_C CSGEAudioProvider* AudioProvider() const;

    IMPORT_C CSGEApplication* Application() const;
    
protected: // data owned
    
    CSGEFileProvider *iFileProvider;

    CSGEBitmapProvider *iBitmapProvider;

    CSGEFontProvider *iFontProvider;

    CSGEStringProvider *iStringProvider;
    
    CSGEAudioProvider *iAudioProvider;

protected: // not owned
    
    CSGEApplication *iApplication;
    
    };


#endif // __SGEENGINECOMMONOBJECTS_H__
