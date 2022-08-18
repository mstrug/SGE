/*
 ============================================================================
 Name		: CGlObject.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CGlObject implementation
 ============================================================================
 */

#include "GlObject.h"
#include "GlPrimitive.h"
#include "GlPrimitiveType.h"


CGlObject::CGlObject() 
    {
    }

CGlObject::CGlObject( CGlPrimitive& aPrimitive, TGlPrimitiveType& aPrimitiveType ) 
    : iPrimitive( &aPrimitive ), iPrimitiveType( &aPrimitiveType ), 
      iPrimitiveOwner( ETrue ), iPrimitiveTypeOwner( ETrue ) 
    {
    }

CGlObject::~CGlObject()
    {
    if ( iPrimitiveOwner )
        {
        delete iPrimitive;
        }
    if ( iPrimitiveTypeOwner )
        {
        delete iPrimitiveType;
        }
    }

CGlObject* CGlObject::NewLC( CGlPrimitive& aPrimitive, TGlPrimitiveType& aPrimitiveType )
    {
    CGlObject* self = new ( ELeave ) CGlObject( aPrimitive, aPrimitiveType );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

CGlObject* CGlObject::NewL( CGlPrimitive& aPrimitive, TGlPrimitiveType& aPrimitiveType )
    {
    CGlObject* self = CGlObject::NewLC( aPrimitive, aPrimitiveType );
    CleanupStack::Pop(); // self;
    return self;
    }

void CGlObject::ConstructL()
    {
    }

void CGlObject::Draw()
    {
    iPrimitiveType->Use();    
    iPrimitive->Draw( iPrimitiveType->Type() );
    }

void CGlObject::Redraw()
    {
    iPrimitive->Redraw( iPrimitiveType->Type() );
    }

CGlPrimitive& CGlObject::Primitive() const
    {
    return *iPrimitive;
    }

TGlPrimitiveType& CGlObject::PrimitiveTypeBase() const
    {
    return *iPrimitiveType; 
    }

void CGlObject::SetPrimitiveType( TGlPrimitiveType& aPrimitiveType, TBool aOwner )
    {
    if ( iPrimitiveTypeOwner )
        {
        delete iPrimitiveType;
        }
    iPrimitiveType = &aPrimitiveType;
    iPrimitiveTypeOwner = aOwner;
    }



