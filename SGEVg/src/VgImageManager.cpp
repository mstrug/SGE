/*
 ============================================================================
 Name		: VgImageManager
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CVgImageManager implementation
 ============================================================================
 */

#include "VgImageManager.h"
#include <SGEBitmap/SGEBitmapItem.h>
#include <SGEBitmap/SGEBitmapUtils.h>
#include <SGEBitmap/SGEBitmapItemCreator.h>
#include <FBS.H> 
#include <SGEVg/VgSvgImage.h>


CVgImageManager::CVgImageManager( MVgImageManagerObserver *aObserver ) : iObserver( aObserver ), iCurrentFileToLoadId( 1 )
    {
    }

CVgImageManager::~CVgImageManager()
    {
	delete iBitmapProvider;
	RHashMap< TUint, SImage >::TIter iter( iImages );
	iter.Reset();
	iter.NextValue();
	while ( iter.CurrentValue() )
	    {
		TBool owned = iter.CurrentValue()->iOwned;
		if ( owned )
			{
			MVgImage* img = iter.CurrentValue()->iImage;
			if ( img->ImageType() == MVgImage::EVgImageTypeImage )
				{
				((TVgImage*)img)->DeleteImage();
				delete ((TVgImage*)img);
				}
			else if ( img->ImageType() == MVgImage::EVgImageTypeSvg )
				{
				delete ((CVgSvgImage*)img);
				}
			}
	    iter.NextValue();
	    }
	iImages.Close();
    iImageItemsToLoad.Close();
    iFs.Close();
    }

CVgImageManager* CVgImageManager::NewLC( MVgImageManagerObserver *aObserver )
    {
	CVgImageManager *self = new (ELeave) CVgImageManager( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

CVgImageManager* CVgImageManager::NewL( MVgImageManagerObserver *aObserver )
    {
	CVgImageManager *self = CVgImageManager::NewLC( aObserver );
    CleanupStack::Pop(); // self;
    return self;
    }

void CVgImageManager::ConstructL()
    {
    User::LeaveIfError( iFs.Connect() );
    iBitmapProvider = CSGEBitmapProvider::NewL( this );
    }

MVgImageManagerObserver* CVgImageManager::SetObserver( MVgImageManagerObserver *aObserver )
    {
	MVgImageManagerObserver* old = iObserver;
    iObserver = aObserver;
    return old;
    }

void CVgImageManager::AddImageL( TUint aImageId, TVgImage aImage )
    {
	if ( iImages.Find( aImageId ) )
        {
        User::Leave( KErrAlreadyExists );
        }
	TVgImage* img = new(ELeave) TVgImage( aImage );
	SImage simg = { img, ETrue };
	iImages.InsertL( aImageId, simg );
    }

void CVgImageManager::AddImageL( TUint aImageId, CVgSvgImage* aImage, TBool aTakeOwnership )
	{
	if ( iImages.Find( aImageId ) )
        {
        User::Leave( KErrAlreadyExists );
        }
	SImage simg = { aImage, aTakeOwnership };
	iImages.InsertL( aImageId, simg );
	}

void CVgImageManager::RemoveImage( TUint aImageId )
	{
	SImage* simg = iImages.Find( aImageId );
	if ( simg )
		{
		if ( simg->iOwned )
			{
			MVgImage *img = simg->iImage;
			if ( img->ImageType() == MVgImage::EVgImageTypeImage )
				{
				((TVgImage*)img)->DeleteImage();
				delete ((TVgImage*)img);
				}
			else if ( img->ImageType() == MVgImage::EVgImageTypeSvg )
				{
				delete ((CVgSvgImage*)img);
				}
			}
		iImages.Remove( aImageId );
		}
	}

MVgImage* CVgImageManager::Get( TUint aImageId )
    {
	SImage* simg = iImages.Find( aImageId );
	if ( !simg )
		{
		return NULL;
		}
	return simg->iImage;
    }

void CVgImageManager::AddImageToLoadL( const TDesC& aFileName, TUint aImageId, VGbitfield aAllowedQuality, TSize aMaxSize )
    {
    SImageItem item = { iCurrentFileToLoadId, aImageId, aAllowedQuality, aMaxSize };
    iImageItemsToLoad.AppendL( item );
    
    CSGEBitmapItemCreator *bitem = new (ELeave) CSGEBitmapItemCreator( aFileName, iCurrentFileToLoadId );
    CleanupStack::PushL( bitem );
    bitem->AddImageDecoderOptions( (CImageDecoder::TOptions) ( CImageDecoder::EOptionAlwaysThread ) );
//    CSGEBitmapItemCreatorChild* child = new (ELeave) CSGEBitmapItemCreatorChild( TSize( 360, 360 ), NULL, iCurrentFileToLoadId );
//    bitem->AddChildActionL( child );
    iBitmapProvider->AddImageToLoadL( bitem );
    CleanupStack::Pop( bitem );
    
//    iBitmapProvider->AddImageToLoadL( aFileName, iCurrentFileToLoadId );
    iCurrentFileToLoadId++;
    }
	
void CVgImageManager::AddImageToLoadL( CSGEFile &aSGEFile, const TDesC& aFileName, TUint aImageId, VGbitfield aAllowedQuality, TSize aMaxSize )
    {
    SImageItem item = { iCurrentFileToLoadId, aImageId, aAllowedQuality, aMaxSize };
    iImageItemsToLoad.AppendL( item );
    
    CSGEBitmapItemCreator *bitem = new (ELeave) CSGEBitmapItemCreator( aSGEFile, aFileName, iCurrentFileToLoadId );
    CleanupStack::PushL( bitem );
    bitem->AddImageDecoderOptions( (CImageDecoder::TOptions) ( CImageDecoder::EOptionAlwaysThread ) );
    iBitmapProvider->AddImageToLoadL( bitem );
    CleanupStack::Pop( bitem );
    
    //iBitmapProvider->AddImageToLoadL( aSGEFile, aFileName, iCurrentFileToLoadId );    
    iCurrentFileToLoadId++;
    }

void CVgImageManager::StartLoadingImagesL()
    {
    ASSERT( iBitmapProvider );
    
    iBitmapProvider->StartLoadingImagesL();
    }

void CVgImageManager::BitmapLoaded( TUint aBitmapId, TInt aError )
    {
    TInt idx = KErrNotFound;
    for ( TInt i = 0; i < iImageItemsToLoad.Count(); i++ )
        {
        if ( iImageItemsToLoad[ i ].iFileId == aBitmapId )
            {
            idx = i;
            break;
            }
        }
    if ( idx == KErrNotFound && aError == KErrNone )
        {
        aError = KErrNotFound;
        }
    if ( idx == KErrNotFound && aBitmapId == KSGEBitmapItemNotDefined )
        {
        return;
        }

    if ( aError == KErrNone )
        {
    	TVgImage image;
        CSGEBitmapItem *bi = iBitmapProvider->BitmapItem( aBitmapId );
        if ( bi && bi->Bitmap() && bi->BitmapMask() )
            {
            CFbsBitmap *bmp = NULL;
            TRAP( aError, bmp = SGEBitmapUtils::CombineBitmapsL( *bi->Bitmap(), *bi->BitmapMask(), EFalse, ETrue ) );
            if ( aError == KErrNone )
                {
                if ( bmp )
                    {
					aError = image.CreateImage( bmp, iImageItemsToLoad[ idx ].iAllowedQuality );
                    }
                else
                    {
                    aError = KErrGeneral;
                    }
                }        
            delete bmp;
            }
        else if ( bi && bi->Bitmap() )
            {
            CFbsBitmap *bmp = NULL;
            TRAP( aError, bmp = SGEBitmapUtils::ConvertTo16MAL( *bi->Bitmap(), EFalse, ETrue ) );
            if ( aError == KErrNone )
                {
                if ( bmp )
                    {
                    aError = image.CreateImage( bmp, iImageItemsToLoad[ idx ].iAllowedQuality );
                    
                    /*if ( aError == KErrGeneral )
                        {
                        //TSize s = bmp->SizeInPixels();
                        //if ( s.iWidth < s.iHeight ) 
                        
                        CFbsBitmap *bmpSmall = new(ELeave) CFbsBitmap();
                        bmpSmall->Create( TSize( 400, 400 ), EColor16MA );
                        CFbsBitmapDevice* bmpdev = CFbsBitmapDevice::NewL( bmpSmall );
                        CFbsBitGc* bmpgc;
                        bmpdev->CreateContext( bmpgc );
                        bmpgc->DrawBitmap( TRect( TPoint(), TSize( 400, 400 ) ), bmp );
                        delete bmpgc;
                        delete bmpdev;
                        delete bmp;
                        bmp = bmpSmall;

                        Logger::LogStrNum( _L("c:\\data\\egal.txt"), _L("CVgImageManager::BitmapLoaded rec "), 1 );
                        aError = image.CreateImage( bmp, iImageItemsToLoad[ idx ].iAllowedQuality );
                      
                        }*/
                    }
                else
                    {
                    aError = KErrGeneral;
                    }
                }        
            delete bmp;
            }
        else
            {
            aError = KErrGeneral;
            }
            
        if ( aError == KErrNone )
            {
        	TRAP( aError, AddImageL( iImageItemsToLoad[ idx ].iImageId, image ) );
            if ( aError != KErrNone )
                {
            	image.DeleteImage();
                }
            }
        
        aBitmapId = iImageItemsToLoad[ idx ].iImageId;
        iImageItemsToLoad.Remove( idx );
        }

    if ( iObserver )
        {
        iObserver->VgImageLoaded( aBitmapId, aError );
        }
    }

void CVgImageManager::BitmapsLoadedAll()
    {
    iBitmapProvider->RemoveBitmapSetAll(); // bitmaps are not used because textures are created
    
    if ( iObserver )
        {
        iObserver->VgAllImagesLoaded();
        }
    }

TBool CVgImageManager::BitmapLoadCustom( TUint aBitmapId, TFileName aFileName, RBuf8& aData )
    {
    _LIT( KSvgExt, ".svg" );
    TParse p;
    p.Set( aFileName, NULL, NULL );
    TPtrC ext = p.Ext();
    
    if ( ext.CompareC( KSvgExt() ) == 0 )
        {
        TInt idx = KErrNotFound;
        for ( TInt i = 0; i < iImageItemsToLoad.Count(); i++ )
            {
            if ( iImageItemsToLoad[ i ].iFileId == aBitmapId )
                {
                idx = i;
                break;
                }
            }
        if ( idx == KErrNotFound )
            {
            return EFalse;
            }

        CVgSvgImage* svgi = NULL;
        TInt err = KErrNone;
        TUint tId = iImageItemsToLoad[ idx ].iImageId;
        if ( aData.Length() > 0 )
            {
            TRAP( err, svgi = CVgSvgImage::NewL( aData ) )
            aData.Close();
            }
        else
            {
            TRAP( err, svgi = CVgSvgImage::NewL( iFs, aFileName ) );
            }
        if ( err != KErrNone )
            {
            delete svgi;
            svgi = NULL;
            }
        else
            {
        	SImage simg = { svgi, ETrue };
            err = iImages.Insert( tId, simg );
            if ( err != KErrNone )
                {
                delete svgi;
                svgi = NULL;
                }
            else
                {
                iImageItemsToLoad.Remove( idx );
                }
            }
        
        if ( iObserver )
            {
            iObserver->VgImageLoaded( tId, err );
            }
        
        return ETrue;
        }
    return EFalse;
    }

void CVgImageManager::BitmapLoadFrameInfo( TUint aBitmapId, TFrameInfo& aFrameInfo )
    {
    for ( TInt i = 0; i < iImageItemsToLoad.Count(); i++ )
        {
        if ( iImageItemsToLoad[ i ].iFileId == aBitmapId )
            {
            if ( iImageItemsToLoad[ i ].iMaxSize != TSize() )
                {
            	TBool larg = ETrue;
            	if ( aFrameInfo.iOverallSizeInPixels.iHeight <= iImageItemsToLoad[ i ].iMaxSize.iHeight )
            		{
            		aFrameInfo.iOverallSizeInPixels.iHeight = iImageItemsToLoad[ i ].iMaxSize.iHeight;
            		larg = EFalse;
            		}
            	if ( aFrameInfo.iOverallSizeInPixels.iWidth <= iImageItemsToLoad[ i ].iMaxSize.iWidth )
            		{
            		aFrameInfo.iOverallSizeInPixels.iWidth = iImageItemsToLoad[ i ].iMaxSize.iWidth;
            		larg = EFalse;
            		}
            	
				if ( larg )
					{
					for ( TInt j = 2; j <= 64; j *= 2 )
						{
						if ( aFrameInfo.iOverallSizeInPixels.iHeight > aFrameInfo.iOverallSizeInPixels.iWidth )
							{
							if ( aFrameInfo.iOverallSizeInPixels.iWidth / j <= iImageItemsToLoad[ i ].iMaxSize.iWidth )
								{
								aFrameInfo.iOverallSizeInPixels.iHeight /= j / 2;
								aFrameInfo.iOverallSizeInPixels.iWidth /= j / 2;
								return;
								}
							}
						else
							{
							if ( aFrameInfo.iOverallSizeInPixels.iHeight / j <= iImageItemsToLoad[ i ].iMaxSize.iHeight )
								{
								aFrameInfo.iOverallSizeInPixels.iHeight /= j / 2;
								aFrameInfo.iOverallSizeInPixels.iWidth /= j / 2;
								return;
								}
							}
						}
                    }
                }
            return;
            }
        }
    }


// End of file
