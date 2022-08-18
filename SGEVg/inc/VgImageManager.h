/*
 ============================================================================
 Name		: VgImageManager.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CVgImageManager declaration
 ============================================================================
 */

#ifndef VGIMAGEMANAGER_H
#define VGIMAGEMANAGER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <e32hashtab.h>
#include <SGEVg/VGImage.h>
#include <SGEBitmap/SGEBitmapProvider.h>

// FORWARD DECLARATIONS
class CSGEFile;
class CVgSvgImage;

// CLASS DECLARATION

/**
 *  MVgImageManagerObserver
 * 
 */
class MVgImageManagerObserver
    {
public:
    
    virtual void VgImageLoaded( TUint aImageId, TInt aError ) = 0;

    virtual void VgAllImagesLoaded() = 0;
    
    };

/**
 *  CVgImageManager
 * 
 */
class CVgImageManager : public CBase, public MSGEBitmapProviderObserver
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CVgImageManager();

    /**
     * Two-phased constructor.
     */
    static CVgImageManager* NewL( MVgImageManagerObserver *aObserver );

    /**
     * Two-phased constructor.
     */
    static CVgImageManager* NewLC( MVgImageManagerObserver *aObserver );

    // returns old observer or NULL if none
    MVgImageManagerObserver* SetObserver( MVgImageManagerObserver *aObserver );

    void AddImageL( TUint aImageId, TVgImage aImage );

    void AddImageL( TUint aImageId, CVgSvgImage* aImage, TBool aTakeOwnership = ETrue );

    void RemoveImage( TUint aImageId );

    MVgImage* Get( TUint aImageId );

    void AddImageToLoadL( const TDesC& aFileName, TUint aImageId, VGbitfield aAllowedQuality, TSize aMaxSize = TSize() );

    void AddImageToLoadL( CSGEFile &aSGEFile, const TDesC& aFileName, TUint aImageId, VGbitfield aAllowedQuality, TSize aMaxSize = TSize() );

    void StartLoadingImagesL();

private:

    /**
     * Constructor for performing 1st stage construction
     */
    CVgImageManager( MVgImageManagerObserver *aObserver );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();

private: // from MSGEBitmapProviderObserver
    
    void BitmapLoaded( TUint aBitmapId, TInt aError );

    void BitmapsLoadedAll();

    TBool BitmapLoadCustom( TUint aBitmapId, TFileName aFileName, RBuf8& aData );

    void BitmapLoadFrameInfo( TUint aBitmapId, TFrameInfo& aFrameInfo );

private:
    
    RFs iFs;
    
    MVgImageManagerObserver *iObserver;
    
    CSGEBitmapProvider *iBitmapProvider; // owned

    struct SImage
    	{
    	MVgImage* iImage;
    	TBool iOwned;
    	};
    
    RHashMap< TUint, SImage > iImages;

    struct SImageItem
        {
        TUint iFileId;
        TUint iImageId;
        VGbitfield iAllowedQuality;
        TSize iMaxSize;
        };
    
    RArray< SImageItem > iImageItemsToLoad;

    TInt iCurrentFileToLoadId;
    
    };

#endif // GLTEXTUREMANAGER_H
