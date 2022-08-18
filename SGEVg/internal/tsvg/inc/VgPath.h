/*
 ============================================================================
 Name		: VgPath.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CVgPath declaration
 ============================================================================
 */

#ifndef VGPATH_H
#define VGPATH_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

// CLASS DECLARATION

/**
 *  CVgPath
 * 
 */
class CVgPath : public CBase
	{
public:

	~CVgPath();

	static CVgPath* NewL();

	static CVgPath* NewLC();

private:

	CVgPath();

	void ConstructL();
	
private: // data
	
	};

#endif // VGPATH_H
