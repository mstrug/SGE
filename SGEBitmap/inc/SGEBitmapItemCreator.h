/*
 ============================================================================
 Name		: SGEBitmapItemCreator.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEBitmapItemCreator declaration
 ============================================================================
 */

#ifndef SGEBITMAPITEMCREATOR_H
#define SGEBITMAPITEMCREATOR_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <SGEBitmap/SGEBitmapItemCreatorChild.h>
#include <ImageConversion.h>

// FORWARD DECLARATIONS
class CSGEBitmapItem;
class CSGEFile;

// CLASS DECLARATION
/**
 *  CSGEBitmapItemCreator
 * 
 */
NONSHARABLE_CLASS( CSGEBitmapItemCreator ) : public CBase
    {
public:

    /**
     * Constructor.
     */
    IMPORT_C CSGEBitmapItemCreator( const TDesC& aFileName, TUint aBitmapId = KSGEBitmapItemNotDefined );

    /**
     * Constructor.
     */
    IMPORT_C CSGEBitmapItemCreator( CSGEFile& aSGEFile, const TDesC& aFileName, TUint aBitmapId = KSGEBitmapItemNotDefined );

    /**
     * Destructor.
     */
    IMPORT_C ~CSGEBitmapItemCreator();
    
    IMPORT_C void AddChildActionL( CSGEBitmapItemCreatorChild *aAction );
    
    IMPORT_C void AddImageDecoderOptions( CImageDecoder::TOptions aDecodingOptions );

    void RemoveFirstChildAction();

    CImageDecoder::TOptions ImageDecoderAdditionalOptions() const;
    
    const TDesC& FileName() const;

    TUint BitmapId() const;
    
    void SetBitmapItem( CSGEBitmapItem *aBitmapItem ); 

    CSGEBitmapItem* BitmapItem() const; 
    
    RPointerArray< CSGEBitmapItemCreatorChild >& ChildActions();

    CSGEFile* SGEFile() const; 
    
private:

    TUint iBitmapId;
    
    CSGEFile *iSGEFile; // not owned
    
    TFileName iFileName;

    CSGEBitmapItem *iBitmapItem;

    RPointerArray< CSGEBitmapItemCreatorChild > iChildActions;

    CImageDecoder::TOptions iDecodingOptions;
    
    };


#endif // SGEBITMAPITEMCREATOR_H
