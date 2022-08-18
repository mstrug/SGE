/*
 ============================================================================
 Name		: VgFontManager.h
 Author	  : Michal Strug
 Version	 : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CVgFontManager declaration
 ============================================================================
 */

#ifndef VGFONTMANAGER_H
#define VGFONTMANAGER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <vg/openvg.h>
#include <SGE/SGEVg/VgFont.h>
#include <e32hashtab.h> 


/**
 *  CVgFontManager
 */
class CVgFontManager : public CBase
    {

public:
    
	~CVgFontManager();
	
	static CVgFontManager* NewL(); 

	static CVgFontManager* NewLC();
    
	void AddFontL( TUint aFontId, CVgFont* aFont );
	
	CVgFont* Get( TUint aFontId );
	
private:
	
	CVgFontManager();
	
	void ConstructL();

private:

	RPtrHashMap< TUint, CVgFont > iFonts;
	
    };

#endif // VGFONTMANAGER_H
