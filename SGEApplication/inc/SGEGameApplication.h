/*
 ============================================================================
  Name        : SGEGameApplication.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#ifndef __SGEGAMEAPPLICATION_H__
#define __SGEGAMEAPPLICATION_H__

// INCLUDES
#include <SGEApplication/SGEApplication.h>

// CLASS DECLARATION
/**
 *  CSGEGameApplication
 * 
 */
class CSGEGameApplication : public CSGEApplication
    {
public: // creation and destruction
    
    IMPORT_C CSGEGameApplication();
    
    IMPORT_C ~CSGEGameApplication();
    
public: // new functions
    
    /*
     * This function is intended to create all necessary objects.
     * It has same role as ConstructL() function.
     * It is called by framework.
     */
    IMPORT_C virtual void InitL();

    // return ETure if application should block simultaneous key presses, default is EFalse
    IMPORT_C virtual TBool BlockKeys() const;

    // return ETure if application should go to the background when user presses red headphone,
    // default is EFalse
    IMPORT_C virtual TBool GoBackgroundOnEndKey() const;

    /*
     * This function stops game loop.
     */
    IMPORT_C virtual void StopGameLoop();

    /*
     * This function starts game loop.
     * It also can be used to restart game loop after specyfing max FPS.
     */
    IMPORT_C virtual void StartGameLoop();

    /*
     * Sets game loop time interval indirectly by specifying maximum frames per seconds.
     * If game loop should run with maximum refresh rate specify as a parameter 0.
     * This function not starts game loop.
     * @param aMaxFPS maximum frames per second 
     */
    IMPORT_C virtual void SetGameLoopMaxFPS( TUint aMaxFPS ); 

    /*
     * Returns current maximum frames per seconds.
     * Default value is 30.
     * @return maximum frames per second 
     */
    IMPORT_C virtual TUint GameLoopMaxFPS() const; 

    /*
     * Returns current frames per seconds.
     * @return frames per second 
     */
    IMPORT_C virtual TUint GameLoopFPS() const; 

    /*
     * Returns current frames counter. Counter is set to 0 each 1 second.
     * @return counter
     */
    IMPORT_C virtual TUint GameLoopFPSCounter() const; 

    /*
     * Checks if game loop is started.
     * @return ETrue if game loop is started 
     */
    IMPORT_C virtual TBool GameLoopStarted(); 

protected: // from base class

    IMPORT_C virtual void HandleForegroundEventL( TBool aForeground );

protected: // abstract functions

    virtual void DoGameLoopEvent( TTimeIntervalMicroSeconds32 aTimeInterval ) = 0;
    
private: // new functions
    
    static TInt GameLoopEvent( TAny *aSelf );

protected: // new functions
    
    TTimeIntervalMicroSeconds32 CalculateGameLoopInterval();
    
public: // internal functions

    IMPORT_C virtual void GameLoopEvent();
    
    IMPORT_C CSGESystemAppUi* CreateAppUiL();

private: // data

    CPeriodic *iGameLoop; // owned

    TInt iNanokernelTickPeriod;
    
    TUint32 iNanokernelTickCount;

    TUint iMaxFPS;
    
    TUint iFPSTime;
    TUint iFPSCounter;
    TUint iFPS;
    
    };

#endif // __SGEGAMEAPPLICATION_H__
