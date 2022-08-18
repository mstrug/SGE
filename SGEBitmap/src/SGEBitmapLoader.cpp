/*
 ============================================================================
 Name		: SGEBitmapLoader.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEBitmapLoader implementation
 ============================================================================
 */

#include "SGEBitmapLoader.h"
#include "SGEBitmapItemCreator.h"
#include "SGEBitmapItemCreatorChild.h"
#include "SGEBitmapItem.h"
#include <ImageConversion.h>
#include <BAUTILS.H> 
#include <SGEFile/SGEFile.h>


// -----------------------------------------------------------------------------
// CSGEBitmapLoader::CSGEBitmapLoader()
// -----------------------------------------------------------------------------
//
CSGEBitmapLoader::CSGEBitmapLoader( MSGEBitmapLoaderObserver &aObserver )
    : CActive( EPriorityStandard ),  iObserver( aObserver )
    {
    }

// -----------------------------------------------------------------------------
// CSGEBitmapLoader::NewLC()
// -----------------------------------------------------------------------------
//
CSGEBitmapLoader* CSGEBitmapLoader::NewLC( MSGEBitmapLoaderObserver &aObserver )
    {
    CSGEBitmapLoader *self = new (ELeave) CSGEBitmapLoader( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapLoader::NewL()
// -----------------------------------------------------------------------------
//
CSGEBitmapLoader* CSGEBitmapLoader::NewL( MSGEBitmapLoaderObserver &aObserver )
    {
    CSGEBitmapLoader *self = CSGEBitmapLoader::NewLC( aObserver );
    CleanupStack::Pop(); // self;
    return self;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapLoader::ConstructL()
// -----------------------------------------------------------------------------
//
void CSGEBitmapLoader::ConstructL()
    {
    User::LeaveIfError( iFs.Connect() );
    
    iBitmapScaler = CBitmapScaler::NewL();
    iBitmapScaler->SetQualityAlgorithm( CBitmapScaler::EMaximumQuality );
    
    iBitmapRotator = CBitmapRotator::NewL();
    
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CSGEBitmapLoader::~CSGEBitmapLoader()
// -----------------------------------------------------------------------------
//
CSGEBitmapLoader::~CSGEBitmapLoader()
    {
    Cancel();
    
    delete iImageDecoder;
    delete iBitmapScaler;
    delete iBitmapRotator; 

    iItemsToLoad.ResetAndDestroy();
    
    iFileData.Close();
    iFs.Close();
    }

// -----------------------------------------------------------------------------
// CSGEBitmapLoader::AddBitmapToLoadL()
// -----------------------------------------------------------------------------
//
void CSGEBitmapLoader::AddBitmapToLoadL( CSGEBitmapItemCreator *aItem )
    {
    iItemsToLoad.AppendL( aItem );
    }

// -----------------------------------------------------------------------------
// CSGEBitmapLoader::StartLoadingBitmapsL()
// -----------------------------------------------------------------------------
//
void CSGEBitmapLoader::StartLoadingBitmapsL()
    {
    if ( iItemsToLoad.Count() == 0 )
        {
        User::Leave( KErrNotFound );
        }
    LoadNextBitmapL();
    }

// -----------------------------------------------------------------------------
// CSGEBitmapLoader::AllBitmapsLoaded()
// -----------------------------------------------------------------------------
//
TBool CSGEBitmapLoader::AllBitmapsLoaded() const
    {
    return ( iItemsToLoad.Count() == 0 );
    }

// -----------------------------------------------------------------------------
// CSGEBitmapLoader::LoadBitmapFromSGEFileL()
// -----------------------------------------------------------------------------
//
void CSGEBitmapLoader::LoadBitmapFromSGEFileL( CSGEFile &aSGEFile, const TDesC& aFileName )
    {
    iFileData.Close();
    TInt size;
    
    RReadStream file = aSGEFile.OpenFileL( aFileName, &size );
    file.PushL();
    iFileData.CreateL( size );
    file.ReadL( iFileData, size );
    CleanupStack::PopAndDestroy( &file );
    }

// -----------------------------------------------------------------------------
// CSGEBitmapLoader::LoadNextBitmapL()
// -----------------------------------------------------------------------------
//
void CSGEBitmapLoader::LoadNextBitmapL()
    {
    if ( iImageDecoder )
        {
        iImageDecoder->Cancel();
        }
    delete iImageDecoder;
    iImageDecoder = NULL;
    
    if ( iItemsToLoad.Count() > 0 )
        {
        ASSERT( iState == EIdle || iState == EFinished );
    
        TParse parse;
        parse.Set( iItemsToLoad[ 0 ]->FileName(), NULL, NULL );
        
        _LIT( KExtPng, ".png" );
        _LIT( KExtGif, ".gif" );
        _LIT( KExtJpg, ".jpg" );
        
        CImageDecoder::TOptions decodingOptions = (CImageDecoder::TOptions) ( CImageDecoder::EOptionNoDither |
                                                  CImageDecoder::EPreferFastDecode |
                                                  CImageDecoder::EOptionIgnoreExifMetaData |
                                                  iItemsToLoad[ 0 ]->ImageDecoderAdditionalOptions() );
        
        TBool useSGEFile = ( iItemsToLoad[ 0 ]->SGEFile() != NULL );
        
        if ( useSGEFile )
            {
            LoadBitmapFromSGEFileL( *iItemsToLoad[ 0 ]->SGEFile(), iItemsToLoad[ 0 ]->FileName() );
            }
        
        if ( parse.Ext().Compare( KExtPng() ) == 0 )
            {
            _LIT8( KMimePng, "image/png" );
            if ( useSGEFile )
                {
                iImageDecoder = CImageDecoder::DataNewL( iFs, iFileData, KMimePng(), decodingOptions );
                }
            else
                {
                iImageDecoder = CImageDecoder::FileNewL( iFs, iItemsToLoad[ 0 ]->FileName(), 
                                                         KMimePng(), decodingOptions );
                }
            }
        else if ( parse.Ext().Compare( KExtGif() ) == 0 )
            {
            _LIT8( KMimeGif, "image/gif" );
            if ( useSGEFile )
                {
                iImageDecoder = CImageDecoder::DataNewL( iFs, iFileData, KMimeGif(), decodingOptions );
                }
            else
                {
                iImageDecoder = CImageDecoder::FileNewL( iFs, iItemsToLoad[ 0 ]->FileName(), 
                                                         KMimeGif(), decodingOptions );
                }
            }
        else if ( parse.Ext().Compare( KExtJpg() ) == 0 )
            {
            _LIT8( KMimeJpg, "image/jpeg" );
            if ( useSGEFile )
                {
                iImageDecoder = CImageDecoder::DataNewL( iFs, iFileData, KMimeJpg(), decodingOptions );
                }
            else
                {
                iImageDecoder = CImageDecoder::FileNewL( iFs, iItemsToLoad[ 0 ]->FileName(), 
                                                         KMimeJpg(), decodingOptions );
                }
            }
        else
            {
            TRAPD( err, 
            if ( useSGEFile )
                {
                iImageDecoder = CImageDecoder::DataNewL( iFs, iFileData, decodingOptions );
                }
            else
                {
                iImageDecoder = CImageDecoder::FileNewL( iFs, iItemsToLoad[ 0 ]->FileName(), decodingOptions );
                }
	        );
	        if ( err == KErrNotFound )
		        {
	            if ( useSGEFile || BaflUtils::FileExists( iFs, iItemsToLoad[ 0 ]->FileName() ) )
			        { // decoder doesn't exists
			        iState = ELoadingImage;
			        TRequestStatus *status = &iStatus;
			        User::RequestComplete( status, KErrNotSupported );
    		        SetActive();
		            return;
			        }
		        }
	        User::LeaveIfError( err );
            }
        
        iImageDecoder->SetDecoderThreadPriority( EPriorityMuchMore );

        TFrameInfo fi( iImageDecoder->FrameInfo() );
        iObserver.BitmapLoadFrameInfo( iItemsToLoad[ 0 ]->BitmapId(), fi );
        
        if ( fi.iFlags & fi.ETransparencyPossible )
            {
            CSGEBitmapItem *bmp = NULL;
            if ( fi.iFlags & fi.EAlphaChannel )
                {
                bmp = CSGEBitmapItem::NewL( iItemsToLoad[ 0 ]->BitmapId(), 
                                            fi.iOverallSizeInPixels,
                                            fi.iFrameDisplayMode,
                                            EGray256 );
                }
            else
                {
                bmp = CSGEBitmapItem::NewL( iItemsToLoad[ 0 ]->BitmapId(), 
                                            fi.iOverallSizeInPixels,
                                            fi.iFrameDisplayMode,
                                            EGray2 );
                }
            ASSERT( bmp );
            iItemsToLoad[ 0 ]->SetBitmapItem( bmp );

            iImageDecoder->Convert( &iStatus, *iItemsToLoad[ 0 ]->BitmapItem()->Bitmap(), 
                                              *iItemsToLoad[ 0 ]->BitmapItem()->BitmapMask() );
            }
        else
            {
            CSGEBitmapItem *bmp = CSGEBitmapItem::NewL( iItemsToLoad[ 0 ]->BitmapId(), 
                                                        fi.iOverallSizeInPixels,
                                                        fi.iFrameDisplayMode );
            iItemsToLoad[ 0 ]->SetBitmapItem( bmp );

            iImageDecoder->Convert( &iStatus, *iItemsToLoad[ 0 ]->BitmapItem()->Bitmap() );
            }

        iState = ELoadingImage;
        SetActive();
        }
    else
        {
        iFileData.Close();
    
        iState = EIdle;

        iObserver.BitmapLoadedAll();
        }
    }

// -----------------------------------------------------------------------------
// CSGEBitmapLoader::RunL()
// -----------------------------------------------------------------------------
//
void CSGEBitmapLoader::RunL()
    {
    if ( iStatus == KErrNone )
        {
        if ( iState == ELoadingImage )                          // image loaded successfully
            {
            if ( iItemsToLoad[ 0 ]->ChildActions().Count() > 0 )
                {
                iActualAction = iItemsToLoad[ 0 ]->ChildActions()[ 0 ];
                
                iActualAction->SetBitmapItem( iItemsToLoad[ 0 ]->BitmapItem()->DuplicateL() );
                
                iState = EStartActions;
                TRequestStatus *status = &iStatus;
                User::RequestComplete( status, KErrNone );
                SetActive();
                }
            else
                {
                iActualAction = NULL;
                CSGEBitmapItem *bmp = iItemsToLoad[ 0 ]->BitmapItem()->DuplicateL( iItemsToLoad[ 0 ]->BitmapId() );
                iObserver.BitmapLoaded( bmp );
                iState = EFinished;
                }
            }
        else if ( iState == EStartActions )
            {
            if ( iActualAction->IsResize() )
                {
                iBitmapScaler->Scale( &iStatus, *iActualAction->BitmapItem()->Bitmap(), iActualAction->Resize() );
                if ( iActualAction->BitmapItem()->BitmapMask() )
                    {
                    iState = EScalingImage1;
                    }
                else
                    {
                    iState = EScalingImage2;
                    }
                SetActive();
                }
            else if ( iActualAction->IsRotate() )
                {
                iBitmapRotator->Rotate( &iStatus, *iActualAction->BitmapItem()->Bitmap(), iActualAction->Rotate() );
                if ( iActualAction->BitmapItem()->BitmapMask() )
                    {
                    iState = ERotatingImage1;
                    }
                else
                    {
                    iState = ERotatingImage2;
                    }
                SetActive();
                }
            else
                {
                ASSERT( EFalse );
                }
            }
        else if ( iState == EScalingImage1 )            // start to scale phase 2
            {
            iBitmapScaler->Scale( &iStatus, *iActualAction->BitmapItem()->BitmapMask(), iActualAction->Resize() );
            iState = EScalingImage2;
            SetActive();
            }
        else if ( iState == ERotatingImage1 )           // start to rotate phase 2
            {
            iBitmapRotator->Rotate( &iStatus, *iActualAction->BitmapItem()->BitmapMask(), iActualAction->Rotate() );
            iState = ERotatingImage2;
            SetActive();
            }
        else if ( iState == EScalingImage2 || iState == ERotatingImage2 )
            {
            if ( iActualAction->BitmapId() != KSGEBitmapItemNotDefined )
                {
                CSGEBitmapItem *bmp = iActualAction->BitmapItem()->DuplicateL( iActualAction->BitmapId() );
                iObserver.BitmapLoaded( bmp );
                }
            
            iState = ENextAction;
            TRequestStatus *status = &iStatus;
            User::RequestComplete( status, KErrNone );
            SetActive();
            }
        else if ( iState == ENextAction )
            {
            CSGEBitmapItemCreatorChild *cc = iActualAction->NextAction();
            if ( cc )
                {
                cc->SetBitmapItem( iActualAction->BitmapItem()->DuplicateL() );
                iActualAction = cc;
                
                iState = EStartActions;
                TRequestStatus *status = &iStatus;
                User::RequestComplete( status, KErrNone );
                SetActive();
                }
            else
                {
                CSGEBitmapItemCreatorChild *parent = iActualAction->ParentAction();
                if ( parent )
                    {
                    iActualAction = parent;
                    
                    iState = EStartActions;
                    TRequestStatus *status = &iStatus;
                    User::RequestComplete( status, KErrNone );
                    SetActive();
                    }
                else
                    {
                    iItemsToLoad[ 0 ]->RemoveFirstChildAction();
                    iActualAction = NULL;
                    
                    iState = ELoadingImage;
                    TRequestStatus *status = &iStatus;
                    User::RequestComplete( status, KErrNone );
                    SetActive();
                    }
                }
            }
        }
    else
        {
        TBool consumed = EFalse;
        if ( iStatus == KErrNotSupported )
            {
            consumed = iObserver.BitmapLoadCustom( ( iActualAction ? iActualAction->BitmapId() : iItemsToLoad[ 0 ]->BitmapId() ), iItemsToLoad[ 0 ]->FileName(), iFileData );
            }
        if ( !consumed )
            {
            iObserver.BitmapLoadError( ( iActualAction ? iActualAction->BitmapId() : KSGEBitmapItemNotDefined ), iStatus.Int() );
            }
        iState = EFinished;
        }

    if ( iState == EFinished )
        {
        delete iItemsToLoad[ 0 ];
        iItemsToLoad.Remove( 0 );
        
        LoadNextBitmapL();
        }
    }

// -----------------------------------------------------------------------------
// CSGEBitmapLoader::DoCancel()
// -----------------------------------------------------------------------------
//
void CSGEBitmapLoader::DoCancel()
    {
    if ( iImageDecoder )
        {
        iImageDecoder->Cancel();
        }
    if ( iBitmapScaler )
        {
        iBitmapScaler->Cancel();
        }
    if ( iBitmapRotator )
        {
        iBitmapRotator->Cancel();
        }
    iState = EIdle;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapLoader::RunError()
// -----------------------------------------------------------------------------
//
TInt CSGEBitmapLoader::RunError( TInt aError )
    {
    iObserver.BitmapLoadError( ( iActualAction ? iActualAction->BitmapId() : KSGEBitmapItemNotDefined ), aError );

    return KErrNone;
    }


// end of file
