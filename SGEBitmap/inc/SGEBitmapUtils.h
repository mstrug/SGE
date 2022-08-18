/*
 ============================================================================
 Name		: SGEBitmapUtils.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEBitmapUtils declaration
 ============================================================================
 */

#ifndef __SGEBITMAPUTILS_H__
#define __SGEBITMAPUTILS_H__

// INCLUDES
#include <e32std.h>
#include <e32base.h>

// FORWARD DECLARATIONS
class CFbsBitmap;
class CSGEBitmapItem;

// CLASS DECLARATION

typedef void (*TSGEBitmapFunctionCurves)( TUint8 &aRed, TUint8 &aGreen, TUint8 &aBlue ); 

typedef void (*TSGEBitmapFunctionCurvesParam)( TUint8 &aRed, TUint8 &aGreen, TUint8 &aBlue, TAny *aParam ); 

/**
 *  CSGEBitmapUtils
 * 
 */
NONSHARABLE_CLASS( SGEBitmapUtils )
    {
public:

    IMPORT_C static CFbsBitmap* DuplicateFbsBitmapL( const CFbsBitmap& aBitmap );

    IMPORT_C static void TransformCurves( TSGEBitmapFunctionCurves aFunction, CFbsBitmap& aBitmap );
    
    IMPORT_C static void TransformCurvesParam( TSGEBitmapFunctionCurvesParam aFunction, 
                                               CFbsBitmap& aBitmap, TAny *aParam );
    
    IMPORT_C static CFbsBitmap* CombineBitmapsL( CFbsBitmap& aBitmap, CFbsBitmap& aBitmapMask, TBool aSwapLeftRigth = EFalse, TBool aSwapUpDown = EFalse );
    
    // function loads image using locally created bitmap loader, for testing purposes only (it is slow)
    IMPORT_C static CSGEBitmapItem* LoadImageL( TFileName aFileName );

    // converts display mode 16M to 16MA
    IMPORT_C static CFbsBitmap* ConvertTo16MAL( CFbsBitmap& aBitmap, TBool aSwapLeftRigth = EFalse, TBool aSwapUpDown = EFalse );

    };

#endif // __SGEBITMAPUTILS_H__
