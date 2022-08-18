/*
 ============================================================================
  Name        : SGEGameApplicationDS.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#ifndef __SGEGAMEAPPLICATIONDS_H__
#define __SGEGAMEAPPLICATIONDS_H__

// INCLUDES
#include <SGEApplication/SGEGameApplication.h>

// CLASS DECLARATION
/**
 *  CSGEGameApplicationDS
 * 
 */
class CSGEGameApplicationDS : public CSGEGameApplication
    {

public:
    
    /*
     * This functions returns graphic context of the frame buffer used to draw scene.
     * If DSA is stopped this function returns NULL, so drawing should be not invoked
     * by the user.
     * @return graphic context 
     */
    IMPORT_C CFbsBitGc* Gc() const;

    /*
     * This functions returns size of the frame buffer.
     * @return size
     */
    IMPORT_C TSize Size() const;

    /*
     * This functions returns display mode of the frame buffer.
     * @return display mode
     */
    IMPORT_C TDisplayMode DisplayMode() const;

public: // internal functions

    IMPORT_C void GameLoopEvent();

    IMPORT_C CSGESystemAppView* CreateAppViewL( TRect aRect );
    
private: // data

    };

#endif // __SGEGAMEAPPLICATIONDS_H__
