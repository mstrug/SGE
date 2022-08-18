/*
 ============================================================================
 Name		: SGEBitmapItemCreatorChild.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEBitmapItemCreatorChild declaration
 ============================================================================
 */

#ifndef SGEBITMAPITEMCREATORCHILD_H
#define SGEBITMAPITEMCREATORCHILD_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <BitmapTransforms.h>

// FORWARD DECLARATIONS
class CSGEBitmapItem;

const TUint KSGEBitmapItemNotDefined = 0;

// CLASS DECLARATION
/**
 *  CSGEBitmapItemCreatorChild
 * 
 */
NONSHARABLE_CLASS( CSGEBitmapItemCreatorChild ) : public CBase
    {
public:

    typedef CBitmapRotator::TRotationAngle TRotationAngle;

public:
    
    /**
     * Constructors
     */
    IMPORT_C CSGEBitmapItemCreatorChild( TSize aResize, CSGEBitmapItemCreatorChild *aParent = NULL, 
                                         TUint aBitmapId = KSGEBitmapItemNotDefined );
    
    IMPORT_C CSGEBitmapItemCreatorChild( TRotationAngle aRotate, CSGEBitmapItemCreatorChild *aParent = NULL, 
                                         TUint aBitmapId = KSGEBitmapItemNotDefined );
    
    /**
     * Destructor.
     */
    IMPORT_C ~CSGEBitmapItemCreatorChild();

    IMPORT_C void AddActionL( CSGEBitmapItemCreatorChild *aAction );
    
    CSGEBitmapItemCreatorChild* NextAction(); 

    CSGEBitmapItemCreatorChild* ParentAction() const;

    void SetParentAction( CSGEBitmapItemCreatorChild *aAction );
    
    TBool IsResize() const;
    
    TBool IsRotate() const;
    
    TSize Resize() const;
    
    TRotationAngle Rotate() const;
    
    TUint BitmapId() const;
    
    void SetBitmapItem( CSGEBitmapItem *aBitmapItem ); 

    CSGEBitmapItem* BitmapItem() const; 
    
    RPointerArray< CSGEBitmapItemCreatorChild >& ChildActions();
    
private: // data

    TBool iResizeSet;
    
    TBool iRotateSet;
    
    TUint iBitmapId;
    
    CSGEBitmapItem *iBitmapItem;
    
    TSize iResize;
    
    TRotationAngle iRotate;
    
    CSGEBitmapItemCreatorChild *iParentAction; // not owned
    
    RPointerArray< CSGEBitmapItemCreatorChild > iChildActions;
    
    TInt iCurrentChildAction;
    
    };

#endif // SGEBITMAPITEMCREATORCHILD_H
