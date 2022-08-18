/*
 ============================================================================
 Name		: SGEBitmapUtils.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEBitmapUtils implementation
 ============================================================================
 */

#include "SGEBitmapUtils.h"
#include "SGEBitmapItem.h"
#include "SGEBitmapItemCreator.h"
#include "SGEBitmapLoader.h"
#include "SGEBitmapLoaderObserver.h"
#include <FBS.H>


// -----------------------------------------------------------------------------
// SGEBitmapUtils::DuplicateFbsBitmapL()
// -----------------------------------------------------------------------------
//
EXPORT_C CFbsBitmap* SGEBitmapUtils::DuplicateFbsBitmapL( const CFbsBitmap &aBitmap )
    {
    CFbsBitmap *bitmap = new (ELeave) CFbsBitmap();
    TSize size = aBitmap.SizeInPixels();
    User::LeaveIfError( bitmap->Create( size, aBitmap.DisplayMode() ) );
    TInt scanLineLength = CFbsBitmap::ScanLineLength( size.iWidth, aBitmap.DisplayMode() );
#ifndef _S60_50_
    bitmap->LockHeap();
    aBitmap.LockHeap();
#endif
    Mem::Copy( (TAny*) bitmap->DataAddress(), (TAny*) aBitmap.DataAddress(), size.iHeight * scanLineLength );
#ifndef _S60_50_
    bitmap->UnlockHeap();
    aBitmap.UnlockHeap();
#endif
    return bitmap;
    }


// -----------------------------------------------------------------------------
// SGEBitmapUtils::TransformCurves()
// -----------------------------------------------------------------------------
//
EXPORT_C void SGEBitmapUtils::TransformCurves( TSGEBitmapFunctionCurves aFunction, CFbsBitmap& aBitmap )
    {
    TSize size = aBitmap.SizeInPixels();
    TInt scanLineLength = CFbsBitmap::ScanLineLength( size.iWidth, aBitmap.DisplayMode() );
#ifndef _S60_50_
    aBitmap.LockHeap();
#endif
    TUint8 *data = (TUint8*) aBitmap.DataAddress();
    for ( TInt i = 0; i < size.iHeight * scanLineLength; i += 3 )
        {
        aFunction( data[ i + 2 ], data[ i + 1 ], data[ i ] );
        }
#ifndef _S60_50_
    aBitmap.UnlockHeap();
#endif
    }


// -----------------------------------------------------------------------------
// SGEBitmapUtils::TransformCurvesParam()
// -----------------------------------------------------------------------------
//
EXPORT_C void SGEBitmapUtils::TransformCurvesParam( TSGEBitmapFunctionCurvesParam aFunction, 
                                                    CFbsBitmap& aBitmap, TAny *aParam )
    {
    TSize size = aBitmap.SizeInPixels();
    TInt scanLineLength = CFbsBitmap::ScanLineLength( size.iWidth, aBitmap.DisplayMode() );
#ifndef _S60_50_
    aBitmap.LockHeap();
#endif
    TUint8 *data = (TUint8*) aBitmap.DataAddress();
    for ( TInt i = 0; i < size.iHeight * scanLineLength; i += 3 )
        {
        aFunction( data[ i + 2 ], data[ i + 1 ], data[ i ], aParam );
        }
#ifndef _S60_50_
    aBitmap.UnlockHeap();
#endif
    }


// -----------------------------------------------------------------------------
// SGEBitmapUtils::CombineBitmaps()
// -----------------------------------------------------------------------------
//
EXPORT_C CFbsBitmap* SGEBitmapUtils::CombineBitmapsL( CFbsBitmap& aBitmap, CFbsBitmap& aBitmapMask, TBool aSwapLeftRigth, TBool aSwapUpDown )
    {
    CFbsBitmap *bitmap = NULL;
    
    TSize size = aBitmap.SizeInPixels();
    //TInt scanLineLength = CFbsBitmap::ScanLineLength( size.iWidth, aBitmap.DisplayMode() );

    TSize sizeMask = aBitmapMask.SizeInPixels();
    //TInt scanLineLengthMask = CFbsBitmap::ScanLineLength( sizeMask.iWidth, aBitmapMask.DisplayMode() );
    
    if ( size != sizeMask )
        {
        User::Leave( KErrArgument );
        }
    
    if ( aBitmap.DisplayMode() == EColor16M && aBitmapMask.DisplayMode() == EGray256 )
        {
        bitmap = new (ELeave) CFbsBitmap();
        User::LeaveIfError( bitmap->Create( size, EColor16MA ) );
        TInt scanLineLength1 = CFbsBitmap::ScanLineLength( aBitmap.SizeInPixels().iWidth, aBitmap.DisplayMode() );
        TInt scanLineLength2 = CFbsBitmap::ScanLineLength( aBitmapMask.SizeInPixels().iWidth, aBitmapMask.DisplayMode() );
        //TInt scanLineLength3 = CFbsBitmap::ScanLineLength( size.iWidth, EColor16MA );

#ifndef _S60_50_
        // for S60 3.0:
        aBitmap.LockHeap();
        aBitmapMask.LockHeap();
        bitmap->LockHeap();
#endif
        TUint8 *data = (TUint8*) aBitmap.DataAddress();
        TUint8 *dataMask = (TUint8*) aBitmapMask.DataAddress();
        TUint32 *dataNew = bitmap->DataAddress();
        //TUint str1 = aBitmap.DataStride();
        //TUint str2 = aBitmapMask.DataStride();
        //TUint str3 = bitmap->DataStride();
        
        // 5px -> 5*rgb = 15bytes + 1allign   => scanlinelength = 16, readSize = 5, allign = 1
        // 5px -> 5*a = 5bytes + 3allign      => scanlinelength = 8, readSize = 5, allign = 3
        
        TUint readSize1 = aBitmapMask.SizeInPixels().iWidth * 3;
        TUint allignSize1 = scanLineLength1 - readSize1;
        TUint readSize2 = aBitmapMask.SizeInPixels().iWidth;
        TUint allignSize2 = scanLineLength2 - readSize2;
        if ( !aSwapUpDown )
        	{
        	if ( !aSwapLeftRigth )
        		{
				for ( TInt y = 0; y < size.iHeight; y++ )
					{
					for ( TInt x = 0; x < readSize1; x += 3 )
						{
						*dataNew = (*( data + 2 )) | ((*( data + 1 )) << 8 ) | ((*data) << 16 ) | ( (*dataMask) << 24 );
						data += 3;
						dataMask++;
						dataNew++;
						}
					data += allignSize1;
					dataMask += allignSize2;
					}
        		}
        	else
        		{
        		data += readSize1 - allignSize1 + 3;
        		dataMask += readSize2 - allignSize2 + 1;
				for ( TInt y = 0; y < size.iHeight; y++ )
					{
					for ( TInt x = 0; x < readSize1; x += 3 )
						{
						*dataNew = (*( data + 2 )) | ((*( data + 1 )) << 8 ) | ((*data) << 16 ) | ( (*dataMask) << 24 );
						data -= 3;
						dataMask--;
						dataNew++;
						}
					data += scanLineLength1 + readSize1;
					dataMask += scanLineLength2 + readSize2;
					}
        		}
        	}
        else
        	{
        	if ( !aSwapLeftRigth )
				{
				data += scanLineLength1 * ( size.iHeight - 1 );
				dataMask += scanLineLength2 * ( size.iHeight - 1 );
				for ( TInt y = 0; y < size.iHeight; y++ )
					{
					for ( TInt x = 0; x < readSize1; x += 3 )
						{
						*dataNew = (*( data + 2 )) | ((*( data + 1 )) << 8 ) | ((*data) << 16 ) | ( (*dataMask) << 24 );
						data += 3;
						dataMask++;
						dataNew++;
						}
					data += allignSize1 - 2 * scanLineLength1;
					dataMask += allignSize2 - 2 * scanLineLength2;
					}
				}
        	else
        		{
        		data += scanLineLength1 * ( size.iHeight - 1 ) + readSize1 - allignSize1 + 3;
        		dataMask += scanLineLength2 * ( size.iHeight - 1 ) + readSize2 - allignSize2 + 1;
				for ( TInt y = 0; y < size.iHeight; y++ )
					{
					for ( TInt x = 0; x < readSize1; x += 3 )
						{
						*dataNew = (*( data + 2 )) | ((*( data + 1 )) << 8 ) | ((*data) << 16 ) | ( (*dataMask) << 24 );
						data -= 3;
						dataMask--;
						dataNew++;
						}
					data -= allignSize1;
					dataMask -= allignSize2;
					}
        		}
        	}

/*        for ( TInt i = 0, j = 0, k = 0; i < size.iHeight * scanLineLengthNew / 4; i++, j += 3, k++ )
            {
            if ( i > 0 && ( 4 * i ) % scanLineLengthNew == 0 )
            	{
            	j += str1 - aBitmap.SizeInPixels().iWidth * 3;
            	k += str2 - aBitmapMask.SizeInPixels().iWidth;
            	}
			dataNew[ i ] = data[ j + 2 ] | ( data[ j + 1 ] << 8 ) | ( data[ j ] << 16 ) | ( dataMask[ k ] << 24 );
//            dataNew[ i ] = data[ j ] | ( data[ j + 1 ] << 8 ) | ( data[ j + 2 ] << 16 ) | ( dataMask[ i ] << 24 );
            }*/

#ifndef _S60_50_
        bitmap->UnlockHeap();
        aBitmap.UnlockHeap();
        aBitmapMask.UnlockHeap();
#endif
        }
    else if ( aBitmap.DisplayMode() == EColor16M && aBitmapMask.DisplayMode() == EGray2 )
        { // TODO
        bitmap = new (ELeave) CFbsBitmap();
        User::LeaveIfError( bitmap->Create( size, EColor16MA ) );
        TInt scanLineLengthNew = CFbsBitmap::ScanLineLength( size.iWidth, EColor16MA );
    
#ifndef _S60_50_
        aBitmap.LockHeap();
        aBitmapMask.LockHeap();
        bitmap->LockHeap();
#endif
        TUint8 *data = (TUint8*) aBitmap.DataAddress();
        TUint8 *dataMask = (TUint8*) aBitmapMask.DataAddress();
        TUint32 *dataNew = bitmap->DataAddress();
        
        for ( TInt i = 0, j = 0; i < size.iHeight * scanLineLengthNew / 4; i++, j += 3 )
            {
            dataNew[ i ] = data[ j + 2 ] | ( data[ j + 1 ] << 8 ) | ( data[ j ] << 16 ) | ( ( dataMask[ i ] ? 0xff : 0 ) << 24 );
//            dataNew[ i ] = data[ j ] | ( data[ j + 1 ] << 8 ) | ( data[ j + 2 ] << 16 ) | ( ( dataMask[ i ] ? 0xff : 0 ) << 24 );
            }
    
#ifndef _S60_50_
        bitmap->UnlockHeap();
        aBitmap.UnlockHeap();
        aBitmapMask.UnlockHeap();
#endif
        }
    else if ( aBitmap.DisplayMode() == EGray256 && aBitmapMask.DisplayMode() == EGray256 )
        { // TODO
        bitmap = new (ELeave) CFbsBitmap();
        User::LeaveIfError( bitmap->Create( size, EColor64K ) );
        TInt scanLineLengthNew = CFbsBitmap::ScanLineLength( size.iWidth, EColor64K );

#ifndef _S60_50_
        aBitmap.LockHeap();
        aBitmapMask.LockHeap();
        bitmap->LockHeap();
#endif
        TUint8 *data = (TUint8*) aBitmap.DataAddress();
        TUint8 *dataMask = (TUint8*) aBitmapMask.DataAddress();
        TUint16 *dataNew = (TUint16 *)bitmap->DataAddress();
        
        for ( TInt i = 0; i < size.iHeight * scanLineLengthNew / 2; i++ )
            {
            dataNew[ i ] = data[ i ] | ( dataMask[ i ] << 8 );
            }

#ifndef _S60_50_
        bitmap->UnlockHeap();
        aBitmap.UnlockHeap();
        aBitmapMask.UnlockHeap();
#endif
        }
    else if ( aBitmap.DisplayMode() == EColor64K && aBitmapMask.DisplayMode() == EGray256 )
        {
        bitmap = new (ELeave) CFbsBitmap();
        User::LeaveIfError( bitmap->Create( size, EColor16MA ) );
        TInt scanLineLength1 = CFbsBitmap::ScanLineLength( aBitmap.SizeInPixels().iWidth, aBitmap.DisplayMode() );
        TInt scanLineLength2 = CFbsBitmap::ScanLineLength( aBitmapMask.SizeInPixels().iWidth, aBitmapMask.DisplayMode() );
        //TInt scanLineLength3 = CFbsBitmap::ScanLineLength( size.iWidth, EColor16MA );

#ifndef _S60_50_
        // for S60 3.0:
        aBitmap.LockHeap();
        aBitmapMask.LockHeap();
        bitmap->LockHeap();
#endif
        TUint8 *data = (TUint8*) aBitmap.DataAddress();
        TUint16 *data16 = (TUint16*) aBitmap.DataAddress();
        TUint8 *dataMask = (TUint8*) aBitmapMask.DataAddress();
        TUint32 *dataNew = bitmap->DataAddress();
        //TUint str1 = aBitmap.DataStride();
        //TUint str2 = aBitmapMask.DataStride();
        //TUint str3 = bitmap->DataStride();
        
        // 5px -> 5*rgb = 15bytes + 1allign   => scanlinelength = 16, readSize = 5, allign = 1
        // 5px -> 5*a = 5bytes + 3allign      => scanlinelength = 8, readSize = 5, allign = 3
        
        // 16 bpp - RGB (rrrrrggggggbbbbb)
        //               2222222211111111
        
        TUint readSize1 = aBitmapMask.SizeInPixels().iWidth * 2;
        TUint allignSize1 = scanLineLength1 - readSize1;
        TUint readSize2 = aBitmapMask.SizeInPixels().iWidth;
        TUint allignSize2 = scanLineLength2 - readSize2;
        if ( !aSwapUpDown )
            {
            if ( !aSwapLeftRigth )
                {
                for ( TInt y = 0; y < size.iHeight; y++ )
                    {
                    for ( TInt x = 0; x < readSize1; x += 2 )
                        {
//                        TUint8 r = (* (data) ) & 0xF8;
//                        TUint8 g = 0; //( ( (*(data+1) ) & 0x7 ) << 3 ) | ( (*( data + 0 )) >> 5 );
//                        TUint8 b = (*( data + 1 )) << 3;
                        TUint16 v = (*data16) >> 8;
                        v |= (*data16) << 8;
                        v = (*data16);
                        TUint8 r = ( (v) & 0xF800 ) >> 8;
                        TUint8 g = ( (v) & 0x07E0 ) >> 3;
                        TUint8 b = ( (v) & 0x001F ) << 3;
                        TUint8 a = 0xff; //(*dataMask);
                        TUint32 rr = r;
                        TUint32 gg = g << 8;
                        TUint32 bb = b << 16;
                        TUint32 aa = a << 24;
                        
                        *dataNew = rr | gg | bb | aa;
                        
                        //*dataNew = ( (*data) >> 3 /*r*/ ) | ( ( ( ( (*data) & 0x7 ) << 3 /*g1*/ ) | ( (*( data + 1 )) >> 5 /*g2*/ ) ) << 8 ) | ( ( (*( data + 1 )) & 0x1F /*b*/ ) << 16 ) | ( 0xff/*(*dataMask)*/ << 24 );
                        data += 2;
                        data16++;
                        dataMask++;
                        dataNew++;
                        }
                    data += allignSize1;
                    dataMask += allignSize2;
                    }
                }
            else
                {
                data += readSize1 - allignSize1 + 3;
                dataMask += readSize2 - allignSize2 + 1;
                for ( TInt y = 0; y < size.iHeight; y++ )
                    {
                    for ( TInt x = 0; x < readSize1; x += 3 )
                        {
                        *dataNew = (*( data + 2 )) | ((*( data + 1 )) << 8 ) | ((*data) << 16 ) | ( (*dataMask) << 24 );
                        data -= 3;
                        dataMask--;
                        dataNew++;
                        }
                    data += scanLineLength1 + readSize1;
                    dataMask += scanLineLength2 + readSize2;
                    }
                }
            }
        else
            {
            if ( !aSwapLeftRigth )
                {
                data += scanLineLength1 * ( size.iHeight - 1 );
                dataMask += scanLineLength2 * ( size.iHeight - 1 );
                for ( TInt y = 0; y < size.iHeight; y++ )
                    {
                    for ( TInt x = 0; x < readSize1; x += 3 )
                        {
                        TUint16 v = (*data16) >> 8;
                        v |= (*data16) << 8;
                        v = (*data16);
                        TUint8 r = ( (v) & 0xF800 ) >> 8;
                        TUint8 g = ( (v) & 0x07E0 ) >> 3;
                        TUint8 b = ( (v) & 0x001F ) << 3;
                        TUint8 a = 0xff; //(*dataMask);
                        TUint32 rr = r;
                        TUint32 gg = g << 8;
                        TUint32 bb = b << 16;
                        TUint32 aa = a << 24;
                        
                        *dataNew = rr | gg | bb | aa;
                        
                        //*dataNew = (*( data + 2 )) | ((*( data + 1 )) << 8 ) | ((*data) << 16 ) | ( (*dataMask) << 24 );
                        data += 2;
                        data16++;
                        dataMask++;
                        dataNew++;
                        }
                    data += allignSize1 - 2 * scanLineLength1;
                    dataMask += allignSize2 - 2 * scanLineLength2;
                    }
                }
            else
                {
                data += scanLineLength1 * ( size.iHeight - 1 ) + readSize1 - allignSize1 + 3;
                dataMask += scanLineLength2 * ( size.iHeight - 1 ) + readSize2 - allignSize2 + 1;
                for ( TInt y = 0; y < size.iHeight; y++ )
                    {
                    for ( TInt x = 0; x < readSize1; x += 3 )
                        {
                        *dataNew = (*( data + 2 )) | ((*( data + 1 )) << 8 ) | ((*data) << 16 ) | ( (*dataMask) << 24 );
                        data -= 3;
                        dataMask--;
                        dataNew++;
                        }
                    data -= allignSize1;
                    dataMask -= allignSize2;
                    }
                }
            }

/*        for ( TInt i = 0, j = 0, k = 0; i < size.iHeight * scanLineLengthNew / 4; i++, j += 3, k++ )
            {
            if ( i > 0 && ( 4 * i ) % scanLineLengthNew == 0 )
                {
                j += str1 - aBitmap.SizeInPixels().iWidth * 3;
                k += str2 - aBitmapMask.SizeInPixels().iWidth;
                }
            dataNew[ i ] = data[ j + 2 ] | ( data[ j + 1 ] << 8 ) | ( data[ j ] << 16 ) | ( dataMask[ k ] << 24 );
//            dataNew[ i ] = data[ j ] | ( data[ j + 1 ] << 8 ) | ( data[ j + 2 ] << 16 ) | ( dataMask[ i ] << 24 );
            }*/

#ifndef _S60_50_
        bitmap->UnlockHeap();
        aBitmap.UnlockHeap();
        aBitmapMask.UnlockHeap();
#endif
        }
    else
        {
        User::Leave( KErrNotSupported );
        }

    return bitmap;
    }


// -----------------------------------------------------------------------------
// SGEBitmapUtils::LoadImage()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEBitmapItem* SGEBitmapUtils::LoadImageL( TFileName aFileName )
	{
	class TObserver : public MSGEBitmapLoaderObserver
		{
	public:
		
	    virtual void BitmapLoaded( CSGEBitmapItem *aBitmapItem ) { iBitmapItem = aBitmapItem; }

	    virtual void BitmapLoadError( TUint /*aBitmapId*/, TInt /*aError*/ ) { }

	    virtual void BitmapLoadedAll() { iAs->AsyncStop(); }

	    virtual TBool BitmapLoadCustom( TUint /*aBitmapId*/, TFileName /*aFileName*/, RBuf8& /*aData*/ ) { return EFalse; }

	    virtual void BitmapLoadFrameInfo( TUint /*aBitmapId*/, TFrameInfo& /*aFrameInfo*/ ) { }; // output size can be adjusted

	    CSGEBitmapItem* iBitmapItem;
	    
	    CActiveSchedulerWait* iAs;
		} observer;

	observer.iBitmapItem = NULL;
	observer.iAs = new (ELeave) CActiveSchedulerWait();

	CleanupStack::PushL( observer.iAs );
	
	CSGEBitmapItemCreator* item = new (ELeave) CSGEBitmapItemCreator( aFileName, 0xffee );
	CleanupStack::PushL( item );

	CSGEBitmapLoader* loader = CSGEBitmapLoader::NewLC( observer );
	loader->AddBitmapToLoadL( item );
	CleanupStack::Pop( loader );
	CleanupStack::Pop( item );
	CleanupStack::PushL( loader );
	loader->StartLoadingBitmapsL();
	observer.iAs->Start();

	CleanupStack::PopAndDestroy( loader );
	CleanupStack::PopAndDestroy( observer.iAs );

	return observer.iBitmapItem;
	}

// -----------------------------------------------------------------------------
// SGEBitmapUtils::ConvertTo16MAL()
// -----------------------------------------------------------------------------
//
EXPORT_C CFbsBitmap* SGEBitmapUtils::ConvertTo16MAL( CFbsBitmap& aBitmap, TBool aSwapLeftRigth, TBool aSwapUpDown )
    {
//    CFbsBitmap *bitmap = new(ELeave) CFbsBitmap();
//    bitmap->Create( aBitmap.SizeInPixels(), EColor16MA );
//    CFbsBitmapDevice* bmpdev = CFbsBitmapDevice::NewL( bitmap );
//    CFbsBitGc* bmpgc;
//    bmpdev->CreateContext( bmpgc );
//    bmpgc->BitBlt( TPoint(), &aBitmap );
//    delete bmpgc;
//    delete bmpdev;
//    return bitmap;
    
    CFbsBitmap *bitmapMask = new(ELeave) CFbsBitmap();
    bitmapMask->Create( aBitmap.SizeInPixels(), EGray256 );
    CFbsBitmapDevice* bmpdev = CFbsBitmapDevice::NewL( bitmapMask );
    CFbsBitGc* bmpgc;
    bmpdev->CreateContext( bmpgc );
    bmpgc->SetBrushColor( TRgb::Gray256( 255 ) );
    bmpgc->DrawRect( TRect( TPoint(), aBitmap.SizeInPixels() ) );
    delete bmpgc;
    delete bmpdev;
    CFbsBitmap* bitmap = SGEBitmapUtils::CombineBitmapsL( aBitmap, *bitmapMask, aSwapLeftRigth, aSwapUpDown );
    delete bitmapMask;
    return bitmap;
    }



// end of file
