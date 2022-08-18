/*
 ============================================================================
  Name        : SGEGameApplicationDSB.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#ifndef __SGEGAMEAPPLICATIONDSB_H__
#define __SGEGAMEAPPLICATIONDSB_H__

// INCLUDES
#include <SGEApplication/SGEGameApplicationDS.h>

// CLASS DECLARATION
/**
 *  CSGEGameApplicationDSB
 * 
 */
class CSGEGameApplicationDSB : public CSGEGameApplicationDS
    {

public: // internal functions

    IMPORT_C CSGESystemAppView* CreateAppViewL( TRect aRect );
    
private: // data

    };

#endif // __SGEGAMEAPPLICATIONDSB_H__
