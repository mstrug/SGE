/*
 ============================================================================
  Name        : SGEEngineController.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#ifndef __SGEENGINECONTROLLER_H__
#define __SGEENGINECONTROLLER_H__

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <W32STD.H>
#include <COEDEF.H>
#include <SGEEngine/SGEEngineObserver.h>

// FORWARD DECLARATIONS
class CSGEEngineCommonObjects;
class CSGEEngine;
class MSGEEngineControllerObserver;

// CLASS DECLARATION
/**
 * CSGEEngineController
 * Contrls engines.   @todo: add iSize set by constructor
 */
class CSGEEngineController : public CBase, public MSGEEngineObserver
    {
public: // destruction
    
    IMPORT_C ~CSGEEngineController();

public: // new functions 
    
    IMPORT_C TKeyResponse HandleKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );

    IMPORT_C TBool HandlePointerEventL( const TPointerEvent &aPointerEvent ); 
    
    IMPORT_C TBool Update( CBitmapContext* aGc, TRect aDrawRect, TTimeIntervalMicroSeconds32 aTimeInterval );

    IMPORT_C void SizeChanged( TSize aNewSize );

    IMPORT_C void HandleForegroundEventL( TBool aForeground );

    // this function is only for special usage
    IMPORT_C void Draw( CBitmapContext* aGc, const TRect aDrawRect ) const;

    IMPORT_C CSGEEngineCommonObjects& CommonObjects() const;

protected: // construction
    
    IMPORT_C CSGEEngineController( MSGEEngineControllerObserver& aObserver, TSize aSize );

    IMPORT_C void BaseConstructL();
    
    IMPORT_C virtual CSGEEngineCommonObjects* CreateCommonObjectsL();

protected: // new functions

    /*
     * Adds engine to the iEngines array.
     * @param aEngine engine to add
     * @return KErrNone if successfull otherwise system error (incl. KErrNoMemory)
     */
    IMPORT_C TInt AddEngine( CSGEEngine *aEngine );

    /*
     * Checks if engine with specified id is in the iEngines array.
     * @param aId engine id
     * @return ETrue if engine was added to the array
     */
    IMPORT_C TBool EngineAdded( TUint aId ) const;

    /*
     * Returns engine with speficied ID.
     * @param aId engine id
     * @return Engine or NULL
     */
    IMPORT_C CSGEEngine* Engine( TUint aId ) const;

    /*
     * Sets engine with specified id as current engine.
     * On current engine function Activate() is called;
     * On engine which was current function Deactivate() is called;
     * Engine should be added to the iEngines array already.
     * @param aId engine id
     * @return KErrNone if successfull
     */
    IMPORT_C TInt SetCurrentEngine( TUint aId );

    /*
     * Gets current engine.
     * @return engine or NULL if current engine not set
     */
    IMPORT_C CSGEEngine* CurrentEngine() const;

    /*
     * Function called from SetCurrentEngine() function intended
     * to be overriden in derived class to do updates after
     * current engine has been chaned.
     * Default empty implementation.
     */
    IMPORT_C virtual void DoSetCurrentEngine();

protected: // from MSGEEngineObserver
    
    // default implementation closes application
    IMPORT_C virtual void EngineClose( TUint aId, TInt aError );
    
    // default empty implementation
    IMPORT_C virtual void EngineSwitch( TUint aId );

protected: // data
    
    MSGEEngineControllerObserver& iObserver;
    
    CSGEEngineCommonObjects *iCommonObjects; // owned

    TSize iSize;
    
private: //data

    RPointerArray<CSGEEngine> iEngines;
    
    CSGEEngine *iCurrentEngine; // not owned
    
    };


#endif // __SGEENGINECONTROLLER_H__
