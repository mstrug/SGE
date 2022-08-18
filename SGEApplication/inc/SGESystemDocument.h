/*
 ============================================================================
  Name        : SGESystemDocument.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#ifndef __SGESYSTEMDOCUMENT_H__
#define __SGESYSTEMDOCUMENT_H__

// INCLUDES
#include <akndoc.h>

// FORWARD DECLARATIONS
class CEikAppUi;
class CEikApplication;
class CSGEApplication;

// CLASS DECLARATION
/**
 * CSGESystemDocument
 *
 */
NONSHARABLE_CLASS( CSGESystemDocument ) : public CAknDocument
    {
public:

    static CSGESystemDocument* NewL( CEikApplication& aEikApp, CSGEApplication& aApplication );

    static CSGESystemDocument* NewLC( CEikApplication& aEikApp, CSGEApplication& aApplication );

    virtual ~CSGESystemDocument();

public: // from base class

    CEikAppUi* CreateAppUiL();

private:

    void ConstructL();

    CSGESystemDocument( CEikApplication& aEikApp, CSGEApplication& aApplication );
    
private: // data
    
    CSGEApplication& iApplication;

    };

#endif // __SGESYSTEMDOCUMENT_H__
