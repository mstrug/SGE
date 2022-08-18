/*
 ============================================================================
 Name		: SGEBitmapTransparent.h
 Author	  : Michal Strug
 Version	 : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEBitmapTransparent declaration
 ============================================================================
 */

#ifndef __SGEBITMAPTRANSPARENT_H__
#define __SGEBITMAPTRANSPARENT_H__

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <GDI.H>

// FORWARD DECLARATIONS
class CFbsBitmap;
class CBitmapContext;
class CFbsBitmapDevice; 
class CFbsBitGc;
class CSGEBitmapItem;

// CLASS DECLARATION
/**
 *  CSGEBitmapTransparent
 * 
 */
NONSHARABLE_CLASS( CSGEBitmapTransparent ) : public CBase
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    IMPORT_C ~CSGEBitmapTransparent();

    /**
     * Two-phased constructor.
     * aTransparency - 255 no transparency, 0 - full transparenct
     */
    IMPORT_C static CSGEBitmapTransparent* NewL( TSize aSize, TRgb aColor, TUint8 aTransparency );

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CSGEBitmapTransparent* NewLC( TSize aSize, TRgb aColor, TUint8 aTransparency );

    IMPORT_C void BitBlt( CBitmapContext *aGc, TRect aRect );

    IMPORT_C void BitBlt( CBitmapContext *aGc, TPoint aPoint, TRect aSourceRect, TBool aInvertMask = EFalse );

    IMPORT_C CFbsBitGc* BitmapGc() const;

    IMPORT_C CFbsBitGc* BitmapMaskGc() const;
    
    IMPORT_C void CopyBitmapToMask();
    
    // transforms this object to the CSGEBitmapItem object
    // deletes itself
    IMPORT_C CSGEBitmapItem* TransformToBitmapItemLD( TUint aBitmapId );
    
private:

    /**
     * Constructor for performing 1st stage construction
     */
    CSGEBitmapTransparent();

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL( TSize aSize, TRgb aColor, TUint8 aTransparency );

private: // data
    
    CFbsBitmap *iBitmap;
    CFbsBitGc *iBitmapGc;
    CFbsBitmapDevice *iBitmapDevice;

    CFbsBitmap *iBitmapMask;
    CFbsBitGc *iBitmapMaskGc;
    CFbsBitmapDevice *iBitmapMaskDevice;

    };

#endif // __SGEBITMAPTRANSPARENT_H__
