/*
 ============================================================================
 Name		: GlRenderer2D.h
 Author	  : Michal Strug
 Version	 : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CGlRenderer2D declaration
 ============================================================================
 */

#ifndef GLRENDERER2D_H
#define GLRENDERER2D_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <SGEEngine/SGEEngineRenderer.h>

// CLASS DECLARATION

enum TGlRenderingPhase
	{
	EGlRenderingFirstPhase,
	EGlRenderingSecondPhase
	};

/**
 * MGlRenderer2DRenderingObject
 *
 */

class MGlRenderer2DRenderingObject : public MSGEEngineRendererObject
	{
public:

	virtual void Draw( TGlRenderingPhase aRenderingPhase ) = 0;

	virtual TRect BoundingBox() { }

	};


/**
 * CGlRenderer2D
 *
 */
class CGlRenderer2D: public CSGEEngineRenderer
	{
public:

	~CGlRenderer2D();

	static CGlRenderer2D* NewL( TInt aLayersCount, TBool aDoublePass, TBool aBoundingBoxTest );

	static CGlRenderer2D* NewLC( TInt aLayersCount, TBool aDoublePass, TBool aBoundingBoxTest );

	// layer 0 is background layer max is foreground
	TInt AddObject( TInt aLayer, MGlRenderer2DRenderingObject* aObject );

	void Draw( TRect aRect );

private:	

	CGlRenderer2D( TBool aDoublePass, TBool aBoundingBoxTest );

	void ConstructL( TInt aLayersCount );

	MGlRenderer2DRenderingObject* Object( TInt aLayer, TInt aIdx );

private: // data

	TBool iDoublePass;

	TBool iBoundingBoxTest;

	};


#endif // GLRENDERER2D_H
