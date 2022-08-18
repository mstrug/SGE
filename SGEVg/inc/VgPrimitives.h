/*
 ============================================================================
 Name		: VgPrimitives.h
 Author	  : Michal Strug
 Version	 : 1.0.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : VgPrimitives.h declaration
 ============================================================================
 */

#ifndef VGPRIMITIVES_H
#define VGPRIMITIVES_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <gdi.h>
#include <vg/openvg.h>

class TVgPoint;
class TVgSize;
class TVgRect;

/**
 *  OpenVG screen coords orientation:
 *  
 *         w,h
 *     -----
 *     |   |
 *     |   |
 *     -----
 *  0,0
 */


/**
 *  TVgPoint
 */
class TVgPoint
    {

public:

	TVgPoint();
	
	TVgPoint( VGfloat aX, VGfloat aY );
	
	TVgPoint( const TPoint& aPoint );
	
	TVgSize AsVgSize() const;
	
	TPoint AsPoint() const;
	
	TSize AsSize() const;
	
	void SetXY( VGfloat aX, VGfloat aY );
	
	TBool operator==( const TVgPoint &aPoint ) const;
	
	operator TPoint();
	
	TVgPoint& operator=( const TPoint &aPoint );

	TVgPoint operator+( const TVgPoint &aPoint ) const;
    
public:
	
	VGfloat iX;
	
	VGfloat iY;
	
    };

/**
 *  TVgSize
 */
class TVgSize
    {

public:

	TVgSize();
	
	TVgSize( VGfloat aWidth, VGfloat aHeight );
	
	TVgSize( const TSize& aSize );
	
	TVgPoint AsVgPoint() const;
	
	TSize AsSize() const;
	
	TPoint AsPoint() const;
	
	void SetSize( VGfloat aWidth, VGfloat aHeight );
	
	TBool operator==( const TVgSize &aSize ) const;
	
	operator TSize();
	
	TVgSize& operator=( const TSize &aSize );

public:
	
	VGfloat iWidth;
	
	VGfloat iHeight;
	
    };

/**
 *  TVgRect
 */
class TVgRect
    {

public:
	
	TVgRect();

	TVgRect( VGfloat aXtl, VGfloat aYtl, VGfloat aXbr, VGfloat aYbr );

	TVgRect( TVgPoint aTopLeft, TVgSize aSize );

	TVgRect( TVgPoint aTopLeft, TVgPoint aBottomRight );

	TVgRect( TPoint aTopLeft, TSize aSize );

	TVgRect( TPoint aTopLeft, TPoint aBottomRight );

	TVgRect( const TRect &aRect );
	
	static TVgRect RectFromBottomLeft( TVgPoint aBottomLeft, TVgSize aSize );

	TVgSize Size() const;
	
	VGfloat Width() const;

	VGfloat Height() const;
	
	TVgPoint BottomLeft() const;
	
	TVgPoint TopRight() const;

	TBool IsEmpty() const;
	
	void Move( VGfloat aDx, VGfloat aDy );
	
	void Move( const TVgPoint &aPoint );
	
	void Resize( VGfloat aDx, VGfloat aDy );
	
	void Resize( const TVgSize &aSize );
	
	void ResizeByCenter( VGfloat aDx, VGfloat aDy );
	
	void ResizeByCenter( const TVgSize &aSize );
	
	void Shrink( VGfloat aDx, VGfloat aDy );
	
	void Shrink( const TVgSize &aSize );
	
	void Grow( VGfloat aDx, VGfloat aDy );
	
	void Grow( const TVgSize &aSize );
	
	TBool Contains(const TVgPoint &aPoint) const;
	
	TBool Intersects(const TVgRect &aRect) const;
	
	void BoundingRect(const TVgRect &aRect);
	
	TVgPoint Center() const;
	
	TBool operator==( const TVgRect &aRect ) const;
	
	operator TRect();
	
	TVgRect& operator=( const TRect &aRect );
	
	TVgRect operator+( const TVgRect &aRect ) const;
	
	void AddVguRect( VGPath aPath ) const;
	
public:
	
	TVgPoint iTl;
	
	TVgPoint iBr;
	
    };


/**
 *  TVgVector
 */
class TVgVector : public TVgPoint
    {

public:

    TVgVector();
    
    TVgVector( VGfloat aX, VGfloat aY );
    
    TVgVector( const TVgPoint& aPoint );
    
    TReal Length() const;
    
    TReal LengthPow2() const;
    
    void MakeUnitVector();
    
    TVgVector& operator=( const TVgVector &aVector );

    TVgVector operator+( const TVgVector &aVector ) const;
    
    };


/**
 *  TVgColor
 */
class TVgColor
    {
public:

	TVgColor();
	
	TVgColor( VGfloat aRed, VGfloat aGreen, VGfloat aBlue );
	
	TVgColor( VGfloat aRed, VGfloat aGreen, VGfloat aBlue, VGfloat aAlpha );
	
	TVgColor( TRgb aColor );

	TVgColor( TUint32 aColor );	// RGBA
	
	void SetAlpha( VGfloat aAlpha );
	
	VGfloat Alpha() const;
	
	const VGfloat* v() const;
	
	VGPaint CreateVgPaint() const;
	
	TUint32 AsUint32() const;  // RGBA
	
public:
	
	TFixedArray< VGfloat, 4 > iColor;	// RGBA

    };

#define KVgColorBlack		TVgColor(0x000000ff)
#define KVgColorWhite		TVgColor(0xffffffff)
#define KVgColorRed			TVgColor(0x0000ffff)
#define KVgColorGreen		TVgColor(0x00ff00ff)
#define KVgColorYellow		TVgColor(0x00ffffff)
#define KVgColorBlue		TVgColor(0xff0000ff)
#define KVgColorMagenta		TVgColor(0xff00ffff)
#define KVgColorCyan		TVgColor(0xffff00ff)
#define KVgColorGray		TVgColor(0xaaaaaaff)
#define KVgColorTransparent	TVgColor(0x00000000)



/**
 *  TVgSvgImageStrokeProperties
 */
class TVgSvgImageStrokeProperties
    {
public:
    TVgSvgImageStrokeProperties();
    void Use() const;
    VGfloat iWidth;
    VGCapStyle iCapStyle;
    VGJoinStyle iJoinStyle;
    VGfloat iMitterLimit;
    };


#endif // VGPRIMITIVES_H
