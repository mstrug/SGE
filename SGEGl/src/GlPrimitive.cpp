/*
 ============================================================================
 Name		: GlPrimitive.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CGlPrimitive implementation
 ============================================================================
 */

#include "GlPrimitive.h"

CGlPrimitive::CGlPrimitive()
    {
    // No implementation required
    }

CGlPrimitive::~CGlPrimitive()
    {
    iChildPrimitives.Close();
    iTransformations.ResetAndDestroy();
    iVertexArray.Close();
    iMaterials.Close();
    }

CGlPrimitive* CGlPrimitive::NewLC()
    {
    CGlPrimitive* self = new (ELeave) CGlPrimitive();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

CGlPrimitive* CGlPrimitive::NewL()
    {
    CGlPrimitive* self = CGlPrimitive::NewLC();
    CleanupStack::Pop(); // self;
    return self;
    }

void CGlPrimitive::ConstructL()
    {
    glGetIntegerv( GL_MAX_TEXTURE_UNITS, &iMaxTextureUnits );
    
    ASSERT( iMaxTextureUnits > 0 );
    }

void CGlPrimitive::AddChildPrimitiveL( CGlPrimitive *aPrimitive )
    {
    iChildPrimitives.AppendL( aPrimitive );
    }

void CGlPrimitive::PushTransformationL( TGlTransformation *aTransformation )
    {
    iTransformations.AppendL( aTransformation );
    }

void CGlPrimitive::PopTransformation()
    {
    delete iTransformations[ iTransformations.Count() - 1 ];
    iTransformations.Remove( iTransformations.Count() - 1 );
    }

void CGlPrimitive::AddVertexArrayL( CGlVertexArray *aVertexArray )
    {
    iVertexArray.AppendL( aVertexArray );
    }

void CGlPrimitive::AddMaterialL( TGlMaterial aMaterial )
    {
    iMaterials.AppendL( aMaterial );
    }

void CGlPrimitive::Draw( GLenum aType )
    {
    glPushMatrix();
    
    Transform();
    
    for ( TInt i = 0; i < iVertexArray.Count(); i++ )
        {
        if ( i < iMaterials.Count() )
            {
            iMaterials[ i ].Use();
            }
        iVertexArray[ i ]->Draw( aType );
        }

    for ( TInt i = 0; i < iChildPrimitives.Count(); i++ )
        {
        iChildPrimitives[ i ]->Draw( aType );
        }
    
    glPopMatrix();
    }

void CGlPrimitive::Redraw( GLenum aType )
    {
    glPushMatrix();
    
    //Transform();
    
    for ( TInt i = 0; i < iVertexArray.Count(); i++ )
        {
        /*if ( i < iMaterials.Count() )
            {
            iMaterials[ i ].Use();
            }*/
        iVertexArray[ i ]->Redraw( aType );
        }

    for ( TInt i = 0; i < iChildPrimitives.Count(); i++ )
        {
        iChildPrimitives[ i ]->Redraw( aType );
        }
    
    glPopMatrix();
    }

CGlVertexArray* CGlPrimitive::VertexArray( TInt aIndex ) const
	{
	if ( aIndex < 0 || aIndex >= iVertexArray.Count() )
		{
		return NULL;
		}
	return iVertexArray[ aIndex ];
	}

void CGlPrimitive::Transform()
    {
    glMatrixMode( GL_MODELVIEW );
    
    for ( TInt i = 0; i < iTransformations.Count(); i++ )
        {
        iTransformations[ i ]->Transform();
        }
    }


