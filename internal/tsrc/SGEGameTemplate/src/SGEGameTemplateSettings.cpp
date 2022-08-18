/*
 ============================================================================
 Name		: SGEGameTemplateSettings.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEGameTemplateSettings implementation
 ============================================================================
 */

#include "SGEGameTemplateSettings.h"

// -----------------------------------------------------------------------------
// CSGEGameTemplateSettings::CSGEGameTemplateSettings()
// -----------------------------------------------------------------------------
//
CSGEGameTemplateSettings::CSGEGameTemplateSettings()
    {
    // No implementation required
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateSettings::~CSGEGameTemplateSettings()
// -----------------------------------------------------------------------------
//
CSGEGameTemplateSettings::~CSGEGameTemplateSettings()
    {
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateSettings::NewLC()
// -----------------------------------------------------------------------------
//
CSGEGameTemplateSettings* CSGEGameTemplateSettings::NewLC()
    {
    CSGEGameTemplateSettings *self = new (ELeave) CSGEGameTemplateSettings();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateSettings::NewL()
// -----------------------------------------------------------------------------
//
CSGEGameTemplateSettings* CSGEGameTemplateSettings::NewL()
    {
    CSGEGameTemplateSettings *self = CSGEGameTemplateSettings::NewLC();
    CleanupStack::Pop(); // self;
    return self;
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateSettings::ConstructL()
// -----------------------------------------------------------------------------
//
void CSGEGameTemplateSettings::ConstructL()
    {
    ReadSettingsL();
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateSettings::WriteSettingsL()
// -----------------------------------------------------------------------------
//
void CSGEGameTemplateSettings::ReadSettingsL()
    {    
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateSettings::WriteSettingsL()
// -----------------------------------------------------------------------------
//
void CSGEGameTemplateSettings::WriteSettingsL()
    {    
    }


// End of file
