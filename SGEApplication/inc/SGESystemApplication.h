/*
 ============================================================================
  Name        : SGESystemApplication.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#ifndef __SGESYSTEMAPPLICATION_H__
#define __SGESYSTEMAPPLICATION_H__

// INCLUDES
#include <aknapp.h>

// FORWARD DECLARATIONS
class CSGEApplication;

// CLASS DECLARATION
/**
 * CSGESystemApplication
 * 
 */
NONSHARABLE_CLASS( CSGESystemApplication ) : public CAknApplication
    {
public:
    
    CSGESystemApplication( CSGEApplication *aApplication );

    ~CSGESystemApplication();

protected: // from base class

    TUid AppDllUid() const;

    CApaDocument* CreateDocumentL();
    
private: // data
    
    CSGEApplication *iApplication; // owned
    
    };

#endif // __APPTESTAPPLICATION_H__
