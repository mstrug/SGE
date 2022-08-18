/*
 ============================================================================
  Name        : SGEBitmapProvider.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#ifndef __SGEBITMAPPROVIDER_H__
#define __SGEBITMAPPROVIDER_H__

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <GDI.H>
#include <SGEBitmap/SGEBitmapLoaderObserver.h>

// FORWARD DECLARATIONS
class CSGEBitmapLoader;
class CSGEBitmapSet;
class CSGEBitmapItem;
class CSGEFile;
class CSGEBitmapItemCreator;

// CLASS DECLARATION

/**
 *  MSGEBitmapProviderObserver
 * 
 */
class MSGEBitmapProviderObserver
    {
public:
    
    virtual void BitmapLoaded( TUint aBitmapId, TInt aError ) = 0;

    virtual void BitmapsLoadedAll() = 0;
    
    virtual TBool BitmapLoadCustom( TUint /*aBitmapId*/, TFileName /*aFileName*/, RBuf8& /*aData*/ ) { return EFalse; };

    virtual void BitmapLoadFrameInfo( TUint /*aBitmapId*/, TFrameInfo& /*aFrameInfo*/ ) { }; // output size can be adjusted
    
    };

/**
 *  CSGEBitmapProvider
 * 
 */
NONSHARABLE_CLASS( CSGEBitmapProvider ) : public CBase, public MSGEBitmapLoaderObserver
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    IMPORT_C ~CSGEBitmapProvider();

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CSGEBitmapProvider* NewL( MSGEBitmapProviderObserver *aObserver );

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CSGEBitmapProvider* NewLC( MSGEBitmapProviderObserver *aObserver );

    IMPORT_C void SetObserver( MSGEBitmapProviderObserver *aObserver );

    IMPORT_C void CreateBitmapSetL( TUint aBitmapSetId );

    // return EFalse when set with specified ID wasn't found
    IMPORT_C TBool SelectBitmapSet( TUint aBitmapSetId );
    
    IMPORT_C TUint CurrentBitmapSet() const;

    IMPORT_C void RemoveBitmapSet( TUint aBitmapSetId );

    IMPORT_C void RemoveBitmapSetAll();

    IMPORT_C CSGEBitmapItem* BitmapItem( TUint aBitmapId );
    
    IMPORT_C void AddImageToLoadL( const TDesC& aFileName, TUint aBitmapId );

    IMPORT_C void AddImageToLoadL( CSGEFile &aSGEFile, const TDesC& aFileName, TUint aBitmapId );

    IMPORT_C void AddImageToLoadL( CSGEBitmapItemCreator *aBitmapItemCreator );

    IMPORT_C void StartLoadingImagesL();
    
    IMPORT_C void AddBitmapItemL( CSGEBitmapItem *aBitmapItem );
    
    IMPORT_C void RemoveBitmapItem( TUint aBitmapId );
    
    IMPORT_C void RemoveBitmapItemAll();
    
    IMPORT_C TBool BitBlt( CBitmapContext &aGc, TPoint aPoint, TUint aBitmapItem );
    
    IMPORT_C TBool BitBlt( CBitmapContext &aGc, TPoint aPoint, TRect aSourceRect, 
                           TUint aBitmapItem, TBool aInvertMask = EFalse );

    // leaves KErrNotReady if not all bitmaps are loaded
    IMPORT_C void ExternalizeL( RWriteStream& aStream ) const;

    // leaves KErrArgument if passed stream don't have propper data
    // leaves KErrNotSupported if passed stream have data in not supported version
    // leaves KErrAlreadyExists if there is bitmap item with same id in one bitmap set
    // leaves KErrNotFound if saved current bitmap set cannot be set after internalizaion
    IMPORT_C void InternalizeL( RReadStream& aStream );
    
private:

    /**
     * Constructor for performing 1st stage construction
     */
    CSGEBitmapProvider( MSGEBitmapProviderObserver *aObserver );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();
    
    TInt GetBitmapSetIdx( TUint aBitmapSetId );
    
    void CheckCurrentBitmapSetAndCreateL();

private: // from MSGEBitmapLoaderObserver
    
    void BitmapLoaded( CSGEBitmapItem *aBitmapItem );

    void BitmapLoadError( TUint aBitmapId, TInt aError );

    void BitmapLoadedAll();

    TBool BitmapLoadCustom( TUint aBitmapId, TFileName aFileName, RBuf8& aData );
    
    void BitmapLoadFrameInfo( TUint aBitmapId, TFrameInfo& aFrameInfo );
    
private: // data

    MSGEBitmapProviderObserver *iObserver;
    
    RPointerArray< CSGEBitmapSet > iBitmapSets;

    CSGEBitmapSet *iCurrentBitmapSet; // not owned
    
    CSGEBitmapLoader *iBitmapLoader; // owned
    
    };

#endif // __SGEBITMAPPROVIDER_H__
