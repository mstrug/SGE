/*
 ============================================================================
 Name		: VgFontManager.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CVgFontManager implementation
 ============================================================================
 */

#include "VgFontManager.h"



CVgFontManager::CVgFontManager()
    {
    }

CVgFontManager::~CVgFontManager()
    {
	iFonts.ResetAndDestroy();
    }

CVgFontManager* CVgFontManager::NewLC()
	{
	CVgFontManager* self = new (ELeave) CVgFontManager();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CVgFontManager* CVgFontManager::NewL()
	{
	CVgFontManager* self = CVgFontManager::NewLC();
	CleanupStack::Pop(); // self;
	return self;
	}

void CVgFontManager::ConstructL()
	{
	}

void CVgFontManager::AddFontL( TUint aFontId, CVgFont* aFont )
	{
	TUint *key = new (ELeave) TUint;
	*key = aFontId;
	iFonts.Insert( key, aFont );
	}

CVgFont* CVgFontManager::Get( TUint aFontId )
	{
	return iFonts.Find( aFontId );
	}


