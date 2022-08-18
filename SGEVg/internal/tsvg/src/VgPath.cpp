/*
 ============================================================================
 Name		: VgPath.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CVgPath implementation
 ============================================================================
 */

#include "VgPath.h"


CVgPath::CVgPath()
	{
	}

CVgPath::~CVgPath()
	{
	}

CVgPath* CVgPath::NewLC()
	{
	CVgPath* self = new (ELeave) CVgPath();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CVgPath* CVgPath::NewL()
	{
	CVgPath* self = CVgPath::NewLC();
	CleanupStack::Pop(); // self;
	return self;
	}

void CVgPath::ConstructL()
	{
	}



