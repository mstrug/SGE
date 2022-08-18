/*
 ============================================================================
 Name		: SGEBitmapItemCreatorChild.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEBitmapItemCreatorChild implementation
 ============================================================================
 */

#include "SGEBitmapItemCreatorChild.h"
#include "SGEBitmapItem.h"


// -----------------------------------------------------------------------------
// CSGEBitmapItemCreatorChild::CSGEBitmapItemCreatorChild()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEBitmapItemCreatorChild::CSGEBitmapItemCreatorChild( TSize aResize, 
                                                        CSGEBitmapItemCreatorChild *aParent, 
                                                        TUint aBitmapId )
    : iResizeSet( ETrue ), iBitmapId( aBitmapId ), iResize( aResize ), iParentAction( aParent )
    {
    }

// -----------------------------------------------------------------------------
// CSGEBitmapItemCreatorChild::CSGEBitmapItemCreatorChild()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEBitmapItemCreatorChild::CSGEBitmapItemCreatorChild( TRotationAngle aRotate, 
                                                        CSGEBitmapItemCreatorChild *aParent, 
                                                        TUint aBitmapId )
    : iRotateSet( ETrue ), iBitmapId( aBitmapId ), iRotate( aRotate ), iParentAction( aParent )
    {
    }

// -----------------------------------------------------------------------------
// CSGEBitmapItemCreatorChild::~CSGEBitmapItemCreatorChild()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEBitmapItemCreatorChild::~CSGEBitmapItemCreatorChild()
    {
    delete iBitmapItem;
    iChildActions.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CSGEBitmapItemCreatorChild::AddActionL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEBitmapItemCreatorChild::AddActionL( CSGEBitmapItemCreatorChild *aAction )
    {
    iChildActions.AppendL( aAction );
    aAction->SetParentAction( this );
    }

// -----------------------------------------------------------------------------
// CSGEBitmapItemCreatorChild::NextAction()
// -----------------------------------------------------------------------------
//
CSGEBitmapItemCreatorChild* CSGEBitmapItemCreatorChild::NextAction()
    {
    return ( iCurrentChildAction >= iChildActions.Count() ? NULL : iChildActions[ iCurrentChildAction++ ] ); 
    }

// -----------------------------------------------------------------------------
// CSGEBitmapItemCreatorChild::ParentAction()
// -----------------------------------------------------------------------------
//
CSGEBitmapItemCreatorChild* CSGEBitmapItemCreatorChild::ParentAction() const
    {
    return iParentAction;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapItemCreatorChild::SetParentAction()
// -----------------------------------------------------------------------------
//
void CSGEBitmapItemCreatorChild::SetParentAction( CSGEBitmapItemCreatorChild *aAction )
    {
    iParentAction = aAction;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapItemCreatorChild::IsResize()
// -----------------------------------------------------------------------------
//
TBool CSGEBitmapItemCreatorChild::IsResize() const
    {
    return iResizeSet;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapItemCreatorChild::IsRotate()
// -----------------------------------------------------------------------------
//
TBool CSGEBitmapItemCreatorChild::IsRotate() const
    {
    return iRotateSet;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapItemCreatorChild::Resize()
// -----------------------------------------------------------------------------
//
TSize CSGEBitmapItemCreatorChild::Resize() const
    {
    return iResize;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapItemCreatorChild::Rotate()
// -----------------------------------------------------------------------------
//
CSGEBitmapItemCreatorChild::TRotationAngle CSGEBitmapItemCreatorChild::Rotate() const
    {
    return iRotate;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapItemCreatorChild::BitmapId()
// -----------------------------------------------------------------------------
//
TUint CSGEBitmapItemCreatorChild::BitmapId() const
    {
    return iBitmapId;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapItemCreatorChild::SetBitmapItem()
// -----------------------------------------------------------------------------
//
void CSGEBitmapItemCreatorChild::SetBitmapItem( CSGEBitmapItem *aBitmapItem )
    {
    delete iBitmapItem;
    iBitmapItem = aBitmapItem;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapItemCreatorChild::BitmapItem()
// -----------------------------------------------------------------------------
//
CSGEBitmapItem* CSGEBitmapItemCreatorChild::BitmapItem() const
    {
    return iBitmapItem;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapItemCreatorChild::ChildActions()
// -----------------------------------------------------------------------------
//
RPointerArray< CSGEBitmapItemCreatorChild >& CSGEBitmapItemCreatorChild::ChildActions()
    {
    return iChildActions;
    }


// end of file
