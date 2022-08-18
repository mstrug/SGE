/*
========================================================================
 Name        : SGEAudioTesterApplication.cpp
 Author      : Michal Strug
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : 
========================================================================
*/
// [[[ begin generated region: do not modify [Generated System Includes]
// ]]] end generated region [Generated System Includes]

// [[[ begin generated region: do not modify [Generated Includes]
#include "SGEAudioTesterApplication.h"
#include "SGEAudioTesterDocument.h"
#ifdef EKA2
#include <eikstart.h>
#endif
// ]]] end generated region [Generated Includes]

/**
 * @brief Returns the application's UID (override from CApaApplication::AppDllUid())
 * @return UID for this application (KUidSGEAudioTesterApplication)
 */
TUid CSGEAudioTesterApplication::AppDllUid() const
    {
    return KUidSGEAudioTesterApplication;
    }

/**
 * @brief Creates the application's document (override from CApaApplication::CreateDocumentL())
 * @return Pointer to the created document object (CSGEAudioTesterDocument)
 */
CApaDocument* CSGEAudioTesterApplication::CreateDocumentL()
    {
    return CSGEAudioTesterDocument::NewL( *this );
    }

#ifdef EKA2

/**
 *    @brief Called by the application framework to construct the application object
 *  @return The application (CSGEAudioTesterApplication)
 */    
LOCAL_C CApaApplication* NewApplication()
    {
    return new CSGEAudioTesterApplication;
    }

/**
* @brief This standard export is the entry point for all Series 60 applications
* @return error code
 */    
GLDEF_C TInt E32Main()
    {
    RThread thr;
    thr.SetPriority( EPriorityAbsoluteLow );
    thr.Close();
    
    return EikStart::RunApplication( NewApplication );
    }
    
#else     // Series 60 2.x main DLL program code

/**
* @brief This standard export constructs the application object.
* @return The application (CSGEAudioTesterApplication)
*/
EXPORT_C CApaApplication* NewApplication()
    {
    return new CSGEAudioTesterApplication;
    }

/**
* @brief This standard export is the entry point for all Series 60 applications
* @return error code
*/
GLDEF_C TInt E32Dll(TDllReason /*reason*/)
    {
    return KErrNone;
    }

#endif // EKA2
