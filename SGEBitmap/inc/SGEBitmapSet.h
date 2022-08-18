/*
 ============================================================================
 Name		: SGEBitmapSet.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEBitmapSet declaration
 ============================================================================
 */

#ifndef SGEBITMAPSET_H
#define SGEBITMAPSET_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <S32STRM.H>

// FORWARD DECLARATIONS
class CSGEBitmapItem;

// CLASS DECLARATION
/**
 *  CSGEBitmapSet
 * 
 */
NONSHARABLE_CLASS( CSGEBitmapSet ) : public CBase
    {
public:

    /**
     * Constructor.
     */
    CSGEBitmapSet( TUint aBitmapSetId );
    
    /**
     * Destructor.
     */
    ~CSGEBitmapSet();

    RPointerArray< CSGEBitmapItem >& Items();

    TUint Id() const;
    
    CSGEBitmapItem* GetBitmapItem( TUint aBitmapId ) const;
    
    void RemoveBitmapItem( TUint aBitmapId );
    
    void ExternalizeL( RWriteStream& aStream ) const;

    void InternalizeL( RReadStream& aStream );

private:
    
    static TBool ItemsCompare( const TUint *aId, const CSGEBitmapItem &aItem );

private: // data
    
    RPointerArray< CSGEBitmapItem > iItems; // owned
    
    TUint iBitmapSetId;
    
    };

#endif // SGEBITMAPSET_H
