/*
 ============================================================================
  Name        : SGEGameTemplateEngineController.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#ifndef SGEGAMETEMPLATEENGINECONTROLLER_H_
#define SGEGAMETEMPLATEENGINECONTROLLER_H_

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <SGEEngine/SGEEngineController.h>
#include <SGEEngine/SGEEngineControllerObserver.h>


// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
 * CSGEGameTemplateEngineController
 * 
 */
class CSGEGameTemplateEngineController : public CSGEEngineController
    {
public:

    /**
     * First phase constructor.
     */    
    static CSGEGameTemplateEngineController* NewL( MSGEEngineControllerObserver& aObserver, TSize aSize );
    
    void Update( CBitmapContext *aGc, TRect aDrawRect, TTimeIntervalMicroSeconds32 aTimeInterval );

private:

    /**
     * Second phase constructor.
     */    
    void ConstructL();
    
    /**
     * Constructor.
     */    
    CSGEGameTemplateEngineController( MSGEEngineControllerObserver& aObserver, TSize aSize );
    
    /**
     * Creating custom common controls.
     */    
    CSGEEngineCommonObjects* CreateCommonObjectsL();

private: // from base class (observer MSGEEngineObserver)

    void EngineClose( TUint aId, TInt aError );

    void EngineSwitch( TUint aId );
    
    };

#endif // SGEGAMETEMPLATEENGINECONTROLLER_H_
