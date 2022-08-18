/*
 ============================================================================
 Name		: SGEStringFileBase.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEStringFileBase implementation
 ============================================================================
 */

#include "SGEStringFileBase.h"
#include "SGEStringFileItem.h"


CSGEStringFileBase::CSGEStringFileBase()
    {
    }

CSGEStringFileBase::~CSGEStringFileBase()
    {
    iStringItems.ResetAndDestroy();
    }

void CSGEStringFileBase::BaseConstructL()
    {
    }

void CSGEStringFileBase::UnloadStrings()
    {
    iStringItems.ResetAndDestroy();
    }

// end of file
