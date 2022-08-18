/*
 ============================================================================
 Name		: GlRenderer2D.cpp
 Author	  : Michal Strug
 Version	 : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CGlRenderer2D declaration
 ============================================================================
 */

#include "GlRenderer2D.h"



CGlRenderer2D::CGlRenderer2D( TBool aDoublePass, TBool aBoundingBoxTest ) : iDoublePass( aDoublePass ), iBoundingBoxTest( aBoundingBoxTest )
	{
	}

CGlRenderer2D::~CGlRenderer2D()
	{
	}

CGlRenderer2D* CGlRenderer2D::NewL( TInt aLayersCount, TBool aDoublePass, TBool aBoundingBoxTest )
	{
	CGlRenderer2D* self = CGlRenderer2D::NewLC( aLayersCount, aDoublePass, aBoundingBoxTest );
	CleanupStack::Pop( self );
	return self;
	}

CGlRenderer2D* CGlRenderer2D::NewLC( TInt aLayersCount, TBool aDoublePass, TBool aBoundingBoxTest )
	{
	CGlRenderer2D* self = new (ELeave) CGlRenderer2D( aDoublePass, aBoundingBoxTest );
	CleanupStack::PushL( self );
	self->ConstructL( aLayersCount );
	return self;
	}

void CGlRenderer2D::ConstructL( TInt aLayersCount )
	{
	BaseConstructL( aLayersCount );
	}

MGlRenderer2DRenderingObject* CGlRenderer2D::Object( TInt aLayer, TInt aIdx )
	{
	return (MGlRenderer2DRenderingObject*)iLayers[ aLayer ][ aIdx ];
	}

TInt CGlRenderer2D::AddObject( TInt aLayer, MGlRenderer2DRenderingObject* aObject )
	{
	return CSGEEngineRenderer::AddObject( aLayer, aObject );
	}

void CGlRenderer2D::Draw( TRect aRect )
	{
	TGlRenderingPhase secondPhase = EGlRenderingFirstPhase;

	// phase 1
	if ( iDoublePass )
		{
		secondPhase = EGlRenderingSecondPhase;
		for ( TInt i = iLayers.Count() - 1; i >= 0; i++ )
			{
			for ( TInt j = 0; j < iLayers[ i ].Count(); j++ )
				{
				MGlRenderer2DRenderingObject* obj = Object( i,  j );
				if ( iBoundingBoxTest && aRect.Intersects( obj->BoundingBox() ) )
					{
					obj->Draw( EGlRenderingFirstPhase );
					}
				else if ( !iBoundingBoxTest )
					{
					obj->Draw( EGlRenderingFirstPhase );
					}
				}
			}
		}

	// phase 2
	for ( TInt i = 0; i < iLayers.Count(); i++ )
		{
		for ( TInt j = 0; j < iLayers[ i ].Count(); j++ )
			{
			MGlRenderer2DRenderingObject* obj = Object( i,  j );
			if ( iBoundingBoxTest && aRect.Intersects( obj->BoundingBox() ) )
				{
				obj->Draw( secondPhase );
				}
			else if ( !iBoundingBoxTest )
				{
				obj->Draw( EGlRenderingFirstPhase );
				}
			}
		}
	}


