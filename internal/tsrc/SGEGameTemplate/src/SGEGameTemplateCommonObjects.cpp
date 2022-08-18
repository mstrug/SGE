/*
 ============================================================================
 Name		: SGEGameTemplateCommonObjects.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEGameTemplateCommonObjects implementation
 ============================================================================
 */

#include "SGEGameTemplateCommonObjects.h"
#include "SGEGameTemplateSettings.h"


// -----------------------------------------------------------------------------
// CSGEGameTemplateCommonObjects::CSGEGameTemplateCommonObjects()
// -----------------------------------------------------------------------------
//
CSGEGameTemplateCommonObjects::CSGEGameTemplateCommonObjects()
    {
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateCommonObjects::~CSGEGameTemplateCommonObjects()
// -----------------------------------------------------------------------------
//
CSGEGameTemplateCommonObjects::~CSGEGameTemplateCommonObjects()
    {
    delete iSettings;
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateCommonObjects::SetSettings()
// -----------------------------------------------------------------------------
//
void CSGEGameTemplateCommonObjects::SetSettings( CSGEGameTemplateSettings *aSettings )
    {
    delete iSettings;
    iSettings = aSettings;
    }

// -----------------------------------------------------------------------------
// CSGEGameTemplateCommonObjects::Settings()
// -----------------------------------------------------------------------------
//
CSGEGameTemplateSettings* CSGEGameTemplateCommonObjects::Settings() const
    {
    return iSettings;
    }


// End of file
