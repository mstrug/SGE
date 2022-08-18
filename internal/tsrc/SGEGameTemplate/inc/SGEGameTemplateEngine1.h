/*
 ============================================================================
  Name        : SGEGameTemplateEngine1.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#ifndef SGEGAMETEMPLATEENGINE1_H_
#define SGEGAMETEMPLATEENGINE1_H_

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <SGEEngine/SGEEngine.h>
#include <SGEEngine/SGEEngineObserver.h>


// FORWARD DECLARATIONS
class CSGEGameTemplateCommonObjects;

// CLASS DECLARATION

const TUint KSGEGameTemplateEngine1Id = 1;

/**
 * CSGEGameTemplateEngine1
 * 
 */
class CSGEGameTemplateEngine1 : public CSGEEngine
    {
private:
    
    enum TEngine1States
        {
        ELoading,
        ENormal
        };
    
public:

    /**
     * First phase constructor.
     */    
    static CSGEGameTemplateEngine1* NewL( MSGEEngineObserver& aObserver, CSGEEngineCommonObjects *aCommonObjects );

private:

    /**
     * Second phase constructor.
     */    
    void ConstructL();
    
    /**
     * Second phase constructor. Called when all images are loaded successfully.
     */    
    void Construct2L();
    
    /**
     * Second phase constructor. Called when all audio files are loaded successfully.
     */    
    void Construct3L();

    /**
     * Constructor.
     */    
    CSGEGameTemplateEngine1( MSGEEngineObserver& aObserver, CSGEEngineCommonObjects *aCommonObjects );

    CSGEGameTemplateCommonObjects& CommonObjects() const;
    
public: // from base class

    TUint Id() const;

    void Draw( CBitmapContext* aGc, const TRect aDrawRect );

    TBool Update( const TTimeIntervalMicroSeconds32 aTimeIntercal );

    TBool KeyEvent( const TKeyEvent &aKeyEvent );

    void SetPause( TBool aPause );

public: // from base class (observer  MSGEAudioProviderObserver)

    void AudioAllFilesLoaded();
    
    void AudioStreamCreated( TInt aError );

public: // from base class (observer  MSGEBitmapProviderObserver)
    
    void BitmapLoaded( TUint aBitmapId, TInt aError );

    void BitmapsLoadedAll();
    
private: // data

    TEngine1States iEngineState;
    
    TBool iPause;
    
    };

#endif // SGEGAMETEMPLATEENGINE1_H_
