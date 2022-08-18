/*
 ============================================================================
 Name		: CGlObject.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CGlObject declaration
 ============================================================================
 */

#ifndef GLOBJECT_H
#define GLOBJECT_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <gles/gl.h>

// FORWARD DECLARATIONS

class CGlPrimitive;
class TGlPrimitiveType;

// CLASS DECLARATION

/**
 *  CGlObject
 * 
 */
class CGlObject : public CBase
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CGlObject();

	/**
	 * Two-phased constructor.
	 */
	static CGlObject* NewL( CGlPrimitive& aPrimitive, TGlPrimitiveType& aPrimitiveType );

	/**
	 * Two-phased constructor.
	 */
	static CGlObject* NewLC( CGlPrimitive& aPrimitive, TGlPrimitiveType& aPrimitiveType );

	// enables arrays and draws
	void Draw();

	// only draws
    void Redraw();

    CGlPrimitive& Primitive() const;

    TGlPrimitiveType& PrimitiveTypeBase() const;

    void SetPrimitiveType( TGlPrimitiveType& aPrimitiveType, TBool aOwner = ETrue );

protected:

    /**
     * Constructor for performing 1st stage construction
     */
    CGlObject();

    /**
     * Constructor for performing 1st stage construction
     */
    CGlObject( CGlPrimitive& aPrimitive, TGlPrimitiveType& aPrimitiveType );

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();

protected:
	
	CGlPrimitive *iPrimitive;

	TGlPrimitiveType *iPrimitiveType;

	TBool iPrimitiveOwner;

	TBool iPrimitiveTypeOwner;

	};

#endif // GLOBJECT_H
