/*
 ============================================================================
 Name		: SGEBitmapLoader.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEBitmapLoader declaration
 ============================================================================
 */

#ifndef SGEBITMAPLOADER_H
#define SGEBITMAPLOADER_H

// INCLUDES
#include <e32base.h>
#include <e32std.h>
#include <f32file.h>
#include "SGEBitmapLoaderObserver.h"

// FORWARD DECLARATIONS
class CImageDecoder;
class CBitmapScaler;
class CBitmapRotator; 
class CSGEBitmapItemCreator;
class CSGEBitmapItemCreatorChild;
class CSGEBitmapItem;
class CSGEFile;

// CLASS DECLARATION

/**
 *  CSGEBitmapLoader
 * 
 */
NONSHARABLE_CLASS( CSGEBitmapLoader ) : public CActive
    {
   
public:
    // Cancel and destroy
    ~CSGEBitmapLoader();

    // Two-phased constructor.
    static CSGEBitmapLoader* NewL( MSGEBitmapLoaderObserver &aObserver );

    // Two-phased constructor.
    static CSGEBitmapLoader* NewLC( MSGEBitmapLoaderObserver &aObserver );

    void AddBitmapToLoadL( CSGEBitmapItemCreator *aItem );
    
    void StartLoadingBitmapsL();

    TBool AllBitmapsLoaded() const;
    
private:
    // C++ constructor
    CSGEBitmapLoader( MSGEBitmapLoaderObserver &aObserver );

    // Second-phase constructor
    void ConstructL();

    void LoadBitmapFromSGEFileL( CSGEFile &aSGEFile, const TDesC& aFileName );

    void LoadNextBitmapL();
    
private: // from CActive

    void RunL();

    void DoCancel();

    TInt RunError(TInt aError);

private:
    
    enum TSGEBitmapLoaderState 
        { 
        EIdle, 
        ELoadingImage, 
        EStartActions, 
        ENextAction, 
        EScalingImage1, 
        EScalingImage2, 
        ERotatingImage1, 
        ERotatingImage2, 
        ERotateAgain, 
        ECroopImage, 
        EFinished 
        };

private: // data

    MSGEBitmapLoaderObserver &iObserver;
    
    TSGEBitmapLoaderState iState;

    RFs iFs;
    
    CImageDecoder *iImageDecoder; // owned

    CBitmapScaler *iBitmapScaler; // owned
    
    CBitmapRotator *iBitmapRotator; // owned

    RPointerArray< CSGEBitmapItemCreator > iItemsToLoad;
    
    CSGEBitmapItemCreatorChild *iActualAction; // not owned

    CSGEBitmapItem *iBitmapItemActual;
    
    RBuf8 iFileData;
    };

#endif // SGEBITMAPLOADER_H
