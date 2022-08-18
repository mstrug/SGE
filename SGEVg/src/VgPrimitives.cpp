/*
 ============================================================================
 Name		: VgPrimitives.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : VgPrimitives implementation
 ============================================================================
 */

#include "VgPrimitives.h"
#include <e32math.h>
#include <vg/openvg.h>
#include <vg/vgu.h>



TVgPoint::TVgPoint() : iX( 0 ), iY( 0 )
	{
	}

TVgPoint::TVgPoint( VGfloat aX, VGfloat aY ) : iX( aX ), iY( aY )
	{
	}

TVgPoint::TVgPoint( const TPoint& aPoint ) : iX( aPoint.iX ), iY( aPoint.iY )
	{
	}

TVgSize TVgPoint::AsVgSize() const
	{
	return TVgSize( iX, iY );
	}

TPoint TVgPoint::AsPoint() const
	{
	return TPoint( iX, iY );
	}

TSize TVgPoint::AsSize() const
	{
	return TSize( iX, iY );
	}

void TVgPoint::SetXY( VGfloat aX, VGfloat aY )
	{
	iX = aX;
	iY = aY;
	}

TBool TVgPoint::operator==(const TVgPoint &aPoint) const
	{
	return ( iX == aPoint.iX && iY == aPoint.iY );
	}

TVgPoint::operator TPoint()
	{
	return TPoint( iX, iY );
	}

TVgPoint& TVgPoint::operator=(const TPoint &aPoint)
	{
	iX = aPoint.iX;
	iY = aPoint.iY;
	return *this;
	}

TVgPoint TVgPoint::operator+( const TVgPoint &aPoint ) const
    {
    return TVgPoint( iX + aPoint.iX, iY + aPoint.iY );
    }




TVgSize::TVgSize() : iWidth( 0 ), iHeight( 0 )
	{
	}

TVgSize::TVgSize( VGfloat aWidth, VGfloat aHeight ) : iWidth( aWidth ), iHeight( aHeight )
	{
	}

TVgSize::TVgSize( const TSize& aSize ) : iWidth( aSize.iWidth ), iHeight( aSize.iHeight )
	{	
	}

TVgPoint TVgSize::AsVgPoint() const
	{
	return TVgPoint( iWidth, iHeight );
	}

TSize TVgSize::AsSize() const
	{
	return TSize( iWidth, iHeight );
	}

TPoint TVgSize:: AsPoint() const
	{
	return TPoint( iWidth, iHeight );
	}

void TVgSize::SetSize( VGfloat aWidth, VGfloat aHeight )
	{
	iWidth = aWidth;
	iHeight = aHeight;
	}

TBool TVgSize::operator==( const TVgSize &aSize ) const
	{
	return ( iWidth == aSize.iWidth && iHeight == aSize.iHeight );
	}

TVgSize::operator TSize()
	{
	return TSize( iWidth, iHeight );
	}

TVgSize& TVgSize::operator=( const TSize &aSize )
	{
	iWidth = aSize.iWidth;
	iHeight = aSize.iHeight;
	return *this;
	}




TVgRect::TVgRect()
	{	
	}

TVgRect::TVgRect( VGfloat aXtl, VGfloat aYtl, VGfloat aXbr, VGfloat aYbr ) : iTl( aXtl, aYtl ), iBr( aXbr, aYbr )
	{
	ASSERT( iTl.iX <= iBr.iX );
	ASSERT( iTl.iY >= iBr.iY );
	}

TVgRect::TVgRect( TVgPoint aTopLeft, TVgSize aSize ) : iTl( aTopLeft ), iBr( aTopLeft.iX + aSize.iWidth, aTopLeft.iY - aSize.iHeight )
	{
	ASSERT( iTl.iX <= iBr.iX );
	ASSERT( iTl.iY >= iBr.iY );
	}

TVgRect::TVgRect( TVgPoint aTopLeft, TVgPoint aBottomRight ) : iTl( aTopLeft ), iBr( aBottomRight )
	{	
	ASSERT( iTl.iX <= iBr.iX );
	ASSERT( iTl.iY >= iBr.iY );
	}

TVgRect::TVgRect( TPoint aTopLeft, TSize aSize ) : iTl( aTopLeft ), iBr( aTopLeft.iX + aSize.iWidth, aTopLeft.iY - aSize.iHeight )
	{
	ASSERT( iTl.iX <= iBr.iX );
	ASSERT( iTl.iY >= iBr.iY );
	}

TVgRect::TVgRect( TPoint aTopLeft, TPoint aBottomRight ) : iTl( aTopLeft ), iBr( aBottomRight )
	{	
	ASSERT( iTl.iX <= iBr.iX );
	ASSERT( iTl.iY >= iBr.iY );
	}

TVgRect::TVgRect( const TRect &aRect ) : iTl( aRect.iTl ), iBr( aRect.iBr )
	{
	ASSERT( iTl.iX <= iBr.iX );
	ASSERT( iTl.iY >= iBr.iY );
	}

TVgRect TVgRect::RectFromBottomLeft( TVgPoint aBottomLeft, TVgSize aSize )
    {
    return TVgRect( TVgPoint( aBottomLeft.iX, aBottomLeft.iY + aSize.iHeight ), aSize );
    }

TVgSize TVgRect::Size() const
	{
	return TVgSize( iBr.iX - iTl.iX, iTl.iY - iBr.iY );
	}

VGfloat TVgRect::Width() const
	{
	return iBr.iX - iTl.iX;
	}

VGfloat TVgRect::Height() const
	{
	return iTl.iY - iBr.iY;
	}

TVgPoint TVgRect::BottomLeft() const
	{
	return TVgPoint( iTl.iX, iBr.iY );
	}

TVgPoint TVgRect::TopRight() const
	{
	return TVgPoint( iBr.iX, iTl.iY );
	}

TBool TVgRect::IsEmpty() const
    {
    return !( iTl.iX < iBr.iX && iTl.iY > iBr.iY );
    }

void TVgRect::Move( VGfloat aDx, VGfloat aDy )
	{
    iTl.iX += aDx;
    iTl.iY += aDy;
    iBr.iX += aDx;
    iBr.iY += aDy;
	}

void TVgRect::Move( const TVgPoint &aPoint )
    {
	Move( aPoint.iX, aPoint.iY );
    }

void TVgRect::Resize( VGfloat aDx, VGfloat aDy )
	{
	iBr.iX += aDx;
	iBr.iY -= aDy;
	}

void TVgRect::Resize( const TVgSize &aSize )
	{
	iBr.iX += aSize.iWidth;
	iBr.iY -= aSize.iHeight;
	}

void TVgRect::ResizeByCenter( VGfloat aDx, VGfloat aDy )
	{
	TVgPoint c = Center();
	iTl = c;
	iBr = c;
	iTl.iX -= aDx / 2;
	iBr.iX += aDx / 2;
	iTl.iY += aDy / 2;
	iBr.iY -= aDy / 2;
	}

void TVgRect::ResizeByCenter( const TVgSize &aSize )
	{
	ResizeByCenter( aSize.iWidth, aSize.iHeight );
	}

void TVgRect::Shrink( VGfloat aDx, VGfloat aDy )
	{
	iTl.iX += aDx;
	iTl.iY -= aDy;
	iBr.iX -= aDx;
	iBr.iY += aDy;
	}

void TVgRect::Shrink( const TVgSize &aSize )
	{
	Shrink( aSize.iWidth, aSize.iHeight );
	}

void TVgRect::Grow( VGfloat aDx, VGfloat aDy )
	{
	iTl.iX -= aDx;
	iTl.iY += aDy;
	iBr.iX += aDx;
	iBr.iY -= aDy;
	}

void TVgRect::Grow( const TVgSize &aSize )
	{
	Grow( aSize.iWidth, aSize.iHeight );
	}

TBool TVgRect::Contains(const TVgPoint &aPoint) const
	{
	return ( aPoint.iX >= iTl.iX && aPoint.iX <= iBr.iX && aPoint.iY <= iTl.iY && aPoint.iY >= iBr.iY );
	}

TBool TVgRect::Intersects(const TVgRect &aRect) const
	{
	return ( Contains( aRect.iTl ) || Contains( aRect.iBr ) );
	}

void TVgRect::BoundingRect(const TVgRect &aRect)
	{
	*this = *this + aRect;
	}

TVgPoint TVgRect::Center() const
	{
	return TVgPoint( iTl.iX + ( iBr.iX - iTl.iX ) / 2, iBr.iY + ( iTl.iY - iBr.iY ) / 2 );
	}

TBool TVgRect::operator==( const TVgRect &aRect ) const
	{
	return ( iTl == aRect.iTl && iBr == aRect.iBr );
	}

TVgRect::operator TRect()
	{
	return TRect( iTl, iBr );
	}

TVgRect& TVgRect::operator=( const TRect &aRect )
	{
	iTl = aRect.iTl;
	iBr = aRect.iBr;
	return *this;
	}

TVgRect TVgRect::operator+( const TVgRect &aRect ) const
	{
    if ( aRect.IsEmpty() )
        {
        return *this;
        }
    else if ( IsEmpty() )
    	{
    	return aRect;
    	}
	TVgPoint tl, br;
	tl.iX = ( iTl.iX < aRect.iTl.iX ? iTl.iX : aRect.iTl.iX );
	tl.iY = ( iTl.iY > aRect.iTl.iY ? iTl.iY : aRect.iTl.iY );
	br.iX = ( iBr.iX > aRect.iBr.iX ? iBr.iX : aRect.iBr.iX );
	br.iY = ( iBr.iY < aRect.iBr.iY ? iBr.iY : aRect.iBr.iY );
	return TVgRect( tl, br );
	}

void TVgRect::AddVguRect( VGPath aPath ) const
    {
    vguRect( aPath, iTl.iX, iBr.iY, iBr.iX - iTl.iX, iTl.iY - iBr.iY );
    }





TVgVector::TVgVector()
    {
    }

TVgVector::TVgVector( VGfloat aX, VGfloat aY ) : TVgPoint( aX, aY )
    {
    }

TVgVector::TVgVector( const TVgPoint& aPoint ) : TVgPoint( aPoint )
    {
    }

TReal TVgVector::Length() const
    {
    TReal out = iX * iX + iY * iY;
    Math::Sqrt( out, out );
    return out;
    }

TReal TVgVector::LengthPow2() const
    {
    return iX * iX + iY * iY;
    }

void TVgVector::MakeUnitVector()
    {
    TReal len = Length();
    iX /= len;
    iY /= len;
    }

TVgVector& TVgVector::operator=(const TVgVector &aVector)
    {
    iX = aVector.iX;
    iY = aVector.iY;
    return *this;
    }

TVgVector TVgVector::operator+( const TVgVector &aVector ) const
    {
    return TVgVector( iX + aVector.iX, iY + aVector.iY );
    }




TVgColor::TVgColor()
	{
	iColor[ 0 ] = 0;
	iColor[ 1 ] = 0;
	iColor[ 2 ] = 0;
	iColor[ 3 ] = 1;
	}

TVgColor::TVgColor( VGfloat aRed, VGfloat aGreen, VGfloat aBlue )
	{
	iColor[ 0 ] = aRed;
	iColor[ 1 ] = aGreen;
	iColor[ 2 ] = aBlue;
	iColor[ 3 ] = 1;
	}

TVgColor::TVgColor( VGfloat aRed, VGfloat aGreen, VGfloat aBlue, VGfloat aAlpha )
	{
	iColor[ 0 ] = aRed;
	iColor[ 1 ] = aGreen;
	iColor[ 2 ] = aBlue;
	iColor[ 3 ] = aAlpha;
	}

TVgColor::TVgColor( TRgb aColor )
	{
	iColor[ 0 ] = ((VGfloat)aColor.Red()) / 255.0;
	iColor[ 1 ] = ((VGfloat)aColor.Green()) / 255.0;
	iColor[ 2 ] = ((VGfloat)aColor.Blue()) / 255.0;
	iColor[ 3 ] = ((VGfloat)aColor.Alpha()) / 255.0;
	}

TVgColor::TVgColor( TUint32 aColor )
	{
	iColor[ 0 ] = ((VGfloat)((aColor >> 24) & 0xFF)) / 255.0;
	iColor[ 1 ] = ((VGfloat)((aColor >> 16) & 0xFF)) / 255.0;
	iColor[ 2 ] = ((VGfloat)((aColor >> 8) & 0xFF)) / 255.0;
	iColor[ 3 ] = ((VGfloat)(aColor & 0xFF)) / 255.0;
	}

void TVgColor::SetAlpha( VGfloat aAlpha )
	{
	iColor[ 3 ] = aAlpha;
	}

VGfloat TVgColor::Alpha() const
	{
	return iColor[ 3 ];
	}

const VGfloat* TVgColor::v() const
	{
	return iColor.Begin();
	}

VGPaint TVgColor::CreateVgPaint() const
	{
    VGPaint paint = vgCreatePaint();
    vgSetParameteri( paint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR );
    vgSetParameterfv( paint, VG_PAINT_COLOR, 4, iColor.Begin() );
    return paint;
	}

TUint32 TVgColor::AsUint32() const
    {
    TUint32 c;
    c |= ( ( (TUint32) ( iColor[ 0 ] * 255.0 ) ) & 0xFF ) << 24;
    c |= ( ( (TUint32) ( iColor[ 1 ] * 255.0 ) ) & 0xFF ) << 16;
    c |= ( ( (TUint32) ( iColor[ 2 ] * 255.0 ) ) & 0xFF ) << 8;
    c |= ( ( (TUint32) ( iColor[ 3 ] * 255.0 ) ) & 0xFF );
    return c;
    }







TVgSvgImageStrokeProperties::TVgSvgImageStrokeProperties() : iWidth( 1 ), iCapStyle( VG_CAP_BUTT ), iJoinStyle( VG_JOIN_MITER ), iMitterLimit( 4 )
    {
    }

void TVgSvgImageStrokeProperties::Use() const
    {
    vgSetf( VG_STROKE_LINE_WIDTH, iWidth );
    vgSeti( VG_STROKE_CAP_STYLE, iCapStyle );
    vgSeti( VG_STROKE_JOIN_STYLE, iJoinStyle );
    vgSetf( VG_STROKE_MITER_LIMIT, iMitterLimit );
    }

