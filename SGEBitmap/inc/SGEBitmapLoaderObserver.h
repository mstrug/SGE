/*
 ============================================================================
 Name       : SGEBitmapLoaderObserver.h
 Author   : Michal Strug
 Version     : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEBitmapLoader implementation
 ============================================================================
 */

#ifndef __SGEBITMAPLOADEROBSERVER_H__
#define __SGEBITMAPLOADEROBSERVER_H__

// INCLUDES
#include <e32base.h>
#include <e32std.h>
#include <w32stdgraphic.h>

// FORWARD DECLARATIONS
class CSGEBitmapItem;

// CLASS DECLARATION
/**
 *  MSGEBitmapLoaderObserver
 * Only for internal usage.
 */
class MSGEBitmapLoaderObserver
    {
public:
    
    virtual void BitmapLoaded( CSGEBitmapItem *aBitmapItem ) = 0;

    virtual void BitmapLoadError( TUint aBitmapId, TInt aError ) = 0;

    virtual void BitmapLoadedAll() = 0;

    virtual TBool BitmapLoadCustom( TUint aBitmapId, TFileName aFileName, RBuf8& aData ) = 0;

    virtual void BitmapLoadFrameInfo( TUint aBitmapId, TFrameInfo& aFrameInfo ) = 0; // output size can be adjusted 

    };

#endif // __SGEBITMAPLOADEROBSERVER_H__
