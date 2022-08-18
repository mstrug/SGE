/*
========================================================================
 Name        : SGEAudioTesterDocument.cpp
 Author      : Michal Strug
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : 
========================================================================
*/
// [[[ begin generated region: do not modify [Generated User Includes]
#include "SGEAudioTesterDocument.h"
#include "SGEAudioTesterAppUi.h"
// ]]] end generated region [Generated User Includes]

/**
 * @brief Constructs the document class for the application.
 * @param anApplication the application instance
 */
CSGEAudioTesterDocument::CSGEAudioTesterDocument( CEikApplication& anApplication )
    : CAknDocument( anApplication )
    {
    }

/**
 * @brief Completes the second phase of Symbian object construction. 
 * Put initialization code that could leave here.  
 */ 
void CSGEAudioTesterDocument::ConstructL()
    {
    }
    
/**
 * Symbian OS two-phase constructor.
 *
 * Creates an instance of CSGEAudioTesterDocument, constructs it, and
 * returns it.
 *
 * @param aApp the application instance
 * @return the new CSGEAudioTesterDocument
 */
CSGEAudioTesterDocument* CSGEAudioTesterDocument::NewL( CEikApplication& aApp )
    {
    CSGEAudioTesterDocument* self = new ( ELeave ) CSGEAudioTesterDocument( aApp );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

/**
 * @brief Creates the application UI object for this document.
 * @return the new instance
 */    
CEikAppUi* CSGEAudioTesterDocument::CreateAppUiL()
    {
    return new ( ELeave ) CSGEAudioTesterAppUi;
    }
                
