/*
 ============================================================================
 Name		: SGEBitmapItemCreator.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEBitmapItemCreator implementation
 ============================================================================
 */

#include "SGEBitmapItemCreator.h"
#include "SGEBitmapItem.h"


// -----------------------------------------------------------------------------
// CSGEBitmapItemCreator::CSGEBitmapItemCreator()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEBitmapItemCreator::CSGEBitmapItemCreator( const TDesC& aFileName, TUint aBitmapId )
    : iBitmapId( aBitmapId ), iFileName( aFileName )
    {
    }

// -----------------------------------------------------------------------------
// CSGEBitmapItemCreator::CSGEBitmapItemCreator()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEBitmapItemCreator::CSGEBitmapItemCreator( CSGEFile& aSGEFile, const TDesC& aFileName, TUint aBitmapId )
    : iBitmapId( aBitmapId ), iSGEFile( &aSGEFile ), iFileName( aFileName )
    {
    }

// -----------------------------------------------------------------------------
// CSGEBitmapItemCreator::~CSGEBitmapItemCreator()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEBitmapItemCreator::~CSGEBitmapItemCreator()
    {
    delete iBitmapItem;
    iChildActions.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CSGEBitmapItemCreator::AddChildActionL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEBitmapItemCreator::AddChildActionL( CSGEBitmapItemCreatorChild *aAction )
    {
    delete iBitmapItem;
    iBitmapItem = NULL;
    iChildActions.AppendL( aAction );
    }

// -----------------------------------------------------------------------------
// CSGEBitmapItemCreator::AddImageDecoderOptions()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEBitmapItemCreator::AddImageDecoderOptions( CImageDecoder::TOptions aDecodingOptions )
	{
	iDecodingOptions = aDecodingOptions;
	}

// -----------------------------------------------------------------------------
// CSGEBitmapItemCreator::FileName()
// -----------------------------------------------------------------------------
//
CImageDecoder::TOptions CSGEBitmapItemCreator::ImageDecoderAdditionalOptions() const
	{
	return iDecodingOptions;
	}

// -----------------------------------------------------------------------------
// CSGEBitmapItemCreator::FileName()
// -----------------------------------------------------------------------------
//
void CSGEBitmapItemCreator::RemoveFirstChildAction()
    {
    delete iChildActions[ 0 ];
    iChildActions.Remove( 0 );
    }

// -----------------------------------------------------------------------------
// CSGEBitmapItemCreator::FileName()
// -----------------------------------------------------------------------------
//
const TDesC& CSGEBitmapItemCreator::FileName() const
    {
    return iFileName;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapItemCreator::BitmapId()
// -----------------------------------------------------------------------------
//
TUint CSGEBitmapItemCreator::BitmapId() const
    {
    return iBitmapId;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapItemCreator::SetBitmapItem()
// -----------------------------------------------------------------------------
//
void CSGEBitmapItemCreator::SetBitmapItem( CSGEBitmapItem *aBitmapItem )
    {
    delete iBitmapItem;
    iBitmapItem = aBitmapItem;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapItemCreator::BitmapItem()
// -----------------------------------------------------------------------------
//
CSGEBitmapItem* CSGEBitmapItemCreator::BitmapItem() const
    {
    return iBitmapItem;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapItemCreator::BitmapItem()
// -----------------------------------------------------------------------------
//
RPointerArray< CSGEBitmapItemCreatorChild >& CSGEBitmapItemCreator::ChildActions()
    {
    return iChildActions;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapItemCreator::SGEFile()
// -----------------------------------------------------------------------------
//
CSGEFile* CSGEBitmapItemCreator::SGEFile() const
    {
    return iSGEFile;
    }


// end of file
