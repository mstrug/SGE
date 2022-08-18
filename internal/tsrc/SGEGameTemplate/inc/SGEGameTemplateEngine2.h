/*
 ============================================================================
  Name        : SGEGameTemplateEngine2.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#ifndef SGEGAMETEMPLATEENGINE2_H_
#define SGEGAMETEMPLATEENGINE2_H_

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <SGEEngine/SGEEngine.h>
#include <SGEEngine/SGEEngineObserver.h>


// FORWARD DECLARATIONS
class CSGEGameTemplateCommonObjects;

// CLASS DECLARATION

const TUint KSGEGameTemplateEngine2Id = 2;

/**
 * CSGEGameTemplateEngine2
 * 
 */
class CSGEGameTemplateEngine2 : public CSGEEngine
    {
public:

    /**
     * First phase constructor.
     */    
    static CSGEGameTemplateEngine2* NewL( MSGEEngineObserver& aObserver, CSGEEngineCommonObjects *aCommonObjects );

private:

    /**
     * Second phase constructor.
     */    
    void ConstructL();

    /**
     * Constructor.
     */    
    CSGEGameTemplateEngine2( MSGEEngineObserver& aObserver, CSGEEngineCommonObjects *aCommonObjects );

    CSGEGameTemplateCommonObjects& CommonObjects() const;
    
    static void BitmapTransformFunction( TUint8& aRed, TUint8& aGreen, TUint8& aBlue );
    
public: // from base class

    TUint Id() const;

    void Draw( CBitmapContext* aGc, const TRect aDrawRect );

    TBool Update( const TTimeIntervalMicroSeconds32 aTimeIntercal );

    TBool KeyEvent( const TKeyEvent &aKeyEvent );

    void SetPause( TBool aPause );
    
private: // data

    TBool iPause;
    
    };

#endif // SGEGAMETEMPLATEENGINE2_H_
