/*
 ============================================================================
 Name		: GlMaterialManager.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CGlMaterialManager implementation
 ============================================================================
 */

#include "GlMaterialManager.h"

CGlMaterialManager::CGlMaterialManager()
    {
    // No implementation required
    }

CGlMaterialManager::~CGlMaterialManager()
    {
    }

CGlMaterialManager* CGlMaterialManager::NewLC()
    {
    CGlMaterialManager* self = new (ELeave) CGlMaterialManager();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

CGlMaterialManager* CGlMaterialManager::NewL()
    {
    CGlMaterialManager* self = CGlMaterialManager::NewLC();
    CleanupStack::Pop(); // self;
    return self;
    }

void CGlMaterialManager::ConstructL()
    {

    }
