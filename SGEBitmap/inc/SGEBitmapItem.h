/*
 ============================================================================
  Name        : SGEBitmapItem.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#ifndef __SGEBITMAPITEM_H__
#define __SGEBITMAPITEM_H__

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <GDI.H>
#include <SGEBitmap/SGEBitmapUtils.h>


// FORWARD DECLARATIONS
class CFbsBitmap;

// CLASS DECLARATIONS
/**
 * CSGEBitmapItem
 * 
 */
NONSHARABLE_CLASS( CSGEBitmapItem ) : public CBase
    {
public:
    // Constructors and destructor

    /**
     * Alternative constructor
     */
    IMPORT_C CSGEBitmapItem( CFbsBitmap *aBitmap, CFbsBitmap *aBitmapMask, TUint aBitmapId );
    
    /**
     * Destructor.
     */
    IMPORT_C ~CSGEBitmapItem();

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CSGEBitmapItem* NewL( TUint aBitmapId, TSize aSize, 
                                          TDisplayMode aMode, TDisplayMode aMaskMode = ENone );

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CSGEBitmapItem* NewLC( TUint aBitmapId, TSize aSize, 
                                           TDisplayMode aMode, TDisplayMode aMaskMode = ENone );
    
    IMPORT_C CSGEBitmapItem* DuplicateL() const;

    IMPORT_C CSGEBitmapItem* DuplicateL( TUint aNewBitmapId ) const;

    IMPORT_C CFbsBitmap* Bitmap() const;

    IMPORT_C CFbsBitmap* BitmapMask() const;    

    IMPORT_C TUint Id() const;
    
    IMPORT_C void TransformCurves( TSGEBitmapFunctionCurves aFunction );
    
    IMPORT_C void TransformCurvesMask( TSGEBitmapFunctionCurves aFunction );
    
    // if bitmap has no transparency mask this function can create empty
    // bitmap mask with specified display mode
    IMPORT_C void CreateBitmapMaskL( TDisplayMode aMaskMode );
    
private:

    /**
     * Constructor for performing 1st stage construction
     */
    CSGEBitmapItem( TUint aBitmapId );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL( TSize aSize, TDisplayMode aMode, TDisplayMode aMaskMode );
    
private: // data
    
    CFbsBitmap *iBitmap; // owned

    CFbsBitmap *iBitmapMask; // owned
    
    TUint iBitmapId;
    
    };

#endif // __SGEBITMAPITEM_H__
