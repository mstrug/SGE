/*
========================================================================
 Name        : SGEAudioTesterApplication.h
 Author      : Michal Strug
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : 
========================================================================
*/
#ifndef SGEAUDIOTESTERAPPLICATION_H
#define SGEAUDIOTESTERAPPLICATION_H

// [[[ begin generated region: do not modify [Generated Includes]
#include <aknapp.h>
// ]]] end generated region [Generated Includes]

// [[[ begin generated region: do not modify [Generated Constants]
const TUid KUidSGEAudioTesterApplication = { 0xE32B08C9 };
// ]]] end generated region [Generated Constants]

/**
 *
 * @class    CSGEAudioTesterApplication SGEAudioTesterApplication.h
 * @brief    A CAknApplication-derived class is required by the S60 application 
 *          framework. It is subclassed to create the application's document 
 *          object.
 */
class CSGEAudioTesterApplication : public CAknApplication
    {
private:
    TUid AppDllUid() const;
    CApaDocument* CreateDocumentL();
    
    };
            
#endif // SGEAUDIOTESTERAPPLICATION_H        
