/*
 ============================================================================
 Name		: VgSvgParser.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CVgSvgParser implementation
 ============================================================================
 */

#include "VgSvgParser.h"
#include "VgSvgParserDef.h"
#include "VgSvgImage.h"
#include <xml/documentparameters.h>
#include <xml/stringdictionarycollection.h>
#include <xml/parserfeature.h>
#include <xml/utils/xmlengmem.h>
#include <xml/matchdata.h> 
#include <e32debug.h>
#include <vg/openvg.h>
#include <vg/vgu.h>

_LIT8(KSvgMimeType, "text/xml");
_LIT8(KParser, "libxml2");



#include <e32cons.h>			// Console
GLREF_D CConsoleBase* console; // write all messages to this
//#define CON_WRITE( x ) console->Write( x )
//#define CON_WRITE8( x ) { RBuf b; b.Create( x.Length() ); b.Copy( x ); console->Write( b ); b.Close(); }
//#define CON_PRINTF( x ) console->Printf( x )
#define CON_WRITE( x ) RDebug::RawPrint( x )
#define CON_WRITE8( x ) RDebug::RawPrint( x )




CVgSvgParser::CVgSvgParser( CVgSvgImage& aImage ) : iImage( aImage ), iDpi( 96 ), iCnvMulX( 1 ), iCnvMulY( 1 ), iCnvDivX( 1 ), iCnvDivY( 1 )
	{
	}

CVgSvgParser::~CVgSvgParser()
	{
	delete iParser;
	delete iParserPlugin;
	iStack.Close();
	
	for ( TInt i = 0; i < iDefs.Count(); i++ )
		{
		vgDestroyPath( iDefs[ i ].iPath );
		if ( !iDefs[ i ].iUsed )
			{
			vgDestroyPaint( iDefs[ i ].iFill );
			vgDestroyPaint( iDefs[ i ].iStroke );
			}
		iDefs[ i ].iId.Close();
		}
	iDefs.Close();
	iCurrentDefId.Close();
	}

CVgSvgParser* CVgSvgParser::NewLC( CVgSvgImage& aImage, TBool aUseLibXml2Parser )
	{
	CVgSvgParser* self = new (ELeave) CVgSvgParser( aImage );
	CleanupStack::PushL(self);
	self->ConstructL( aUseLibXml2Parser );
	return self;
	}

CVgSvgParser* CVgSvgParser::NewL( CVgSvgImage& aImage, TBool aUseLibXml2Parser )
	{
	CVgSvgParser* self = CVgSvgParser::NewLC( aImage, aUseLibXml2Parser );
	CleanupStack::Pop(); // self;
	return self;
	}

void CVgSvgParser::ConstructL( TBool aUseLibXml2Parser )
	{
	if ( aUseLibXml2Parser )
		{
		iParserPlugin = CMatchData::NewL();
		iParserPlugin->SetMimeTypeL( KSvgMimeType() );
		iParserPlugin->SetVariantL( KParser() );
		TRAPD( err, iParser = CParser::NewL( *iParserPlugin, *this ) );
		if ( err != KErrNone )
			{
			iParser = CParser::NewL( KSvgMimeType(), *this );
			}
		}
	else
		{
		iParser = CParser::NewL( KSvgMimeType(), *this );
		}
	}

void CVgSvgParser::ParseL( const TDesC8& aData )
	{
	iParser->StringPool().OpenL( KVgSvgParserStringTable );
	iParser->ParseBeginL();
	iParser->ParseL( aData );
//	TInt start = 0;
//	const TInt KLen = 15 * 1024;
//	TInt len = KLen;
//	TPtrC8 ptr;
//	
//	RBuf8 tmp;
//	tmp.CreateMaxL( 100 * 1024 );
//	iParser->ParseL( tmp );
//	tmp.Close();
//	
//	do
//		{
//		if ( start + len > aData.Length() )
//			{
//			len = aData.Length() - start;
//			}
//		ptr.Set( aData.Mid( start, len ) );
//		iParser->ParseL( ptr );
//		start += len;
//		}
//	while ( len == KLen );
	iParser->ParseEndL();
	}

TVgRect CVgSvgParser::ViewBox() const
	{
	return iViewBox;
	}

TReal CVgSvgParser::UnitCoef( const TDesC8& aUnit )
    {
    if ( aUnit.Length() == 0 || aUnit.CompareC( _L8("px") ) == 0 )
        { // px by default
        return 1; //aValue;
        }
    else if ( aUnit.CompareC( _L8("cm") ) == 0 )
        {
        // 1 cm = 0.393700787401575 in ; 1 in = 96 px
        // 1 in = 2.54 cm
        return iDpi /** aValue*/ / 2.54;
        }
    else if ( aUnit.CompareC( _L8("mm") ) == 0 )
        {
        // 1 cm = 10 mm
        // 1 in = 2.54 cm = 25.4 mm
        return iDpi /** aValue*/ / 25.4;
        }
    else if ( aUnit.CompareC( _L8("dm") ) == 0 )
        {
        // 1 cm = 0.1 dm
        // 1 in = 2.54 cm = 0.254 dm
        return iDpi /** aValue*/ / 0.254;
        }
    else if ( aUnit.CompareC( _L8("m") ) == 0 )
        {
        // 1 cm = 0.01 m
        // 1 in = 2.54 cm = 0.0254 m
        return iDpi /** aValue*/ / 0.0254;
        }
    else if ( aUnit.CompareC( _L8("in") ) == 0 )
        {
        return iDpi /** aValue*/;
        }
    else if ( aUnit.CompareC( _L8("ft") ) == 0 )
        {
        // 1 ft = 12 in
        return iDpi /** aValue*/ / 12;
        }
    else if ( aUnit.CompareC( _L8("pt") ) == 0 )
        {
        return /** aValue*/ iDpi / 72;
        }
    return -1;
    }

void CVgSvgParser::ExtractPair( TLex8& aLex, TReal32& aVal1, TReal32& aVal2 )
	{
	aLex.SkipSpace();
	aLex.Inc();  // (
	aLex.SkipSpace();
	aLex.Val( aVal1, '.' );
	aLex.SkipSpace();
    if ( aLex.Peek() == ',' )
    	{
    	aLex.Inc();
    	aLex.SkipSpace();
    	}
    if ( aLex.Peek() == ')' )
    	{ // no v2 specified
    	aLex.Inc();
    	}
    else
    	{
    	aLex.Val( aVal2, '.' );
    	aLex.SkipSpace();
    	aLex.Inc(); // )
    	}
	}

void CVgSvgParser::ExtractThree( TLex8& aLex, TReal32& aVal1, TReal32& aVal2, TReal32& aVal3 )
	{
	aLex.SkipSpace();
	aLex.Inc();  // (
	aLex.SkipSpace();
	aLex.Val( aVal1, '.' );
	aLex.SkipSpace();
    if ( aLex.Peek() == ',' )
    	{
    	aLex.Inc();
    	aLex.SkipSpace();
    	}
    if ( aLex.Peek() == ')' )
    	{ // no v2 specified
    	aLex.Inc();
    	}
    else
    	{
    	aLex.Val( aVal2, '.' );
    	aLex.SkipSpace();
        if ( aLex.Peek() == ',' )
        	{
        	aLex.Inc();
        	aLex.SkipSpace();
        	}
        if ( aLex.Peek() == ')' )
        	{ // no v3 specified
        	aLex.Inc();
        	}
        else
        	{
        	aLex.Val( aVal3, '.' );
        	aLex.SkipSpace();
        	aLex.Inc(); // )
        	}
    	}
	}
void CVgSvgParser::ParseTransformElement( const TDesC8& aValue )
	{
	TReal32 tx = 0, ty = 0, sx = 1, sy = 1, angle = 0, cx = 0, cy = 0; 
    TLex8 lex( aValue );
    lex.SkipSpace();
    CVgSvgParser::TStackItem& item = StackCurrentItem();
    vgLoadMatrix( item.iTransformation.Begin() );
    while ( !lex.Eos() )
    	{
		if ( lex.Remainder().Left( 9 ).CompareC( _L8("translate") ) == 0 )
			{
			lex.Inc( 9 );
			ExtractPair( lex, tx, ty );
		    vgTranslate( tx * iCnvMulX / iCnvDivX, ty * iCnvMulY / iCnvDivY );
			}
		else if ( lex.Remainder().Left( 5 ).CompareC( _L8("scale") ) == 0 )
			{
			lex.Inc( 5 );
			ExtractPair( lex, sx, sy );
		    vgScale( sx, sy );
			}
		else if ( lex.Remainder().Left( 6 ).CompareC( _L8("rotate") ) == 0 )
			{
			lex.Inc( 6 );
			ExtractThree( lex, angle, cx, cy );
			if ( cx == 0 && cy == 0 )
				{
				vgRotate( angle );
				}
			else
				{
			    vgTranslate( cx * iCnvMulX / iCnvDivX, cy * iCnvMulY / iCnvDivY );
				vgRotate( angle );
			    vgTranslate( -cx * iCnvMulX / iCnvDivX, -cy * iCnvMulY / iCnvDivY );
				}
			}
		else if ( lex.Remainder().Left( 6 ).CompareC( _L8("matrix") ) == 0 )
			{
			// todo
			}
		else
			{
		    lex.SkipCharacters();
			}
	    lex.SkipSpace();
    	}
    vgGetMatrix( item.iTransformation.Begin() );
	}

TUint32 CVgSvgParser::ParseColorValue( TLex8& aLex )
	{
	TUint32 color;
	if ( aLex.Peek() == '#' )
		{
		aLex.Inc();
		aLex.Mark();
		aLex.Val( color, EHex );
		TPtrC8 number = aLex.MarkedToken();
		TInt l = number.Length();
		if ( l == 3 )
			{ // color short form
			TBuf8<10> buf;
			buf.Append( number[ 0 ] );
			buf.Append( number[ 0 ] );
			buf.Append( number[ 1 ] );
			buf.Append( number[ 1 ] );
			buf.Append( number[ 2 ] );
			buf.Append( number[ 2 ] );
			TLex8 l2( buf );
			l2.Val( color, EHex );
			}
		}
	else if ( aLex.Remainder().Left( 3 ).CompareC( _L8("rgb") ) == 0 )
		{
		TReal32 v1, v2, v3;
		aLex.Inc( 3 );
		if ( aLex.Remainder().Locate( '%' ) != KErrNotFound )
			{
			RBuf8 bf;
			bf.Create( aLex.Remainder() );
			TInt idx = bf.Locate( '%' );
			do
				{
				bf[ idx ] = ' ';
				idx = bf.Locate( '%' );
				}
			while ( idx != KErrNotFound );
			
			TLex8 l2( bf );
			ExtractThree( l2, v1, v2, v3 );
			
			v1 = 255 * v1 / 100;
			v2 = 255 * v2 / 100;
			v3 = 255 * v3 / 100;
			bf.Close();
			}
		else
			{
			ExtractThree( aLex, v1, v2, v3 );
			}
		TUint32 r = v1, g = v2, b = v3;
		r &= 0xff;
		g &= 0xff;
		b &= 0xff;
		color = ( r << 16 ) | ( g << 8 ) | ( b ); 
		}
	else 
		{
		TPtrC8 cname = aLex.Remainder();
		for ( TInt i = 0; i < KVgSvgParserColorsMapCount; i++ )
			{
			if ( cname.CompareC( KVgSvgParserColorsMap[ i ].iColorName ) == 0 )
				{
				color = KVgSvgParserColorsMap[ i ].iColor;
				break;
				}
			}
		}
	return color;
	}

void CVgSvgParser::ParseFillElement(const RAttributeArray& aAttributes)
    {
	TUint32 color;
	//StackCurrentItem().iFill.iSpecified = EFalse;
    CVgSvgParser::TStackItem& item = StackCurrentItem();
    TReal32 alpha = item.iFill.iColor.Alpha();
    for ( TInt i = 0; i < aAttributes.Count(); i++ )
        {
		TInt idx = aAttributes[ i ].Attribute().LocalName().Index( KVgSvgParserStringTable );
        const TDesC8& value = aAttributes[ i ].Value().DesC();
        TLex8 lex( value );
        lex.SkipSpace();
        switch ( idx )
			{
			case ESvgParserString_fill:
					if ( lex.Remainder().CompareC( _L8("none") ) == 0 )
						{
						item.iFill.iSpecified = EFalse;
						}
					else
						{
						item.iFill.iSpecified = ETrue;
						color = ParseColorValue( lex );
					    item.iFill.iColor = TVgColor( color << 8 );
						}
				break;
			case ESvgParserString_fill_rule:
				break;
			case ESvgParserString_fill_opacity:
					lex.Val( alpha, '.' );
				break;
			default:
				break;
			}
        }
    item.iFill.iColor.SetAlpha( alpha );
    }

void CVgSvgParser::ParseStrokeElement(const RAttributeArray& aAttributes)
    {
	TUint32 color;
	//StackCurrentItem().iStroke.iSpecified = EFalse;
    CVgSvgParser::TStackItem& item = StackCurrentItem();
    TReal32 width = item.iStroke.iProperties.iWidth;
    TReal32 mitterLimit = item.iStroke.iProperties.iMitterLimit;
    TReal32 alpha = item.iStroke.iColor.Alpha();
    for ( TInt i = 0; i < aAttributes.Count(); i++ )
        {
		TInt idx = aAttributes[ i ].Attribute().LocalName().Index( KVgSvgParserStringTable );
        const TDesC8& value = aAttributes[ i ].Value().DesC();
        TLex8 lex( value );
        lex.SkipSpace();
		switch ( idx )
			{
			case ESvgParserString_stroke:
					if ( lex.Remainder().CompareC( _L8("none") ) == 0 )
						{
						item.iStroke.iSpecified = EFalse;
						}
					else
						{
					    item.iStroke.iSpecified = ETrue;
						color = ParseColorValue( lex );
					    item.iStroke.iColor = TVgColor( color << 8 );
						}
				break;
			case ESvgParserString_stroke_width:
					lex.Val( width, '.' );
					width = iCnvMulX * width / iCnvDivX;
				    item.iStroke.iProperties.iWidth = width;
				break;
			case ESvgParserString_stroke_linecap:
                    if ( lex.Remainder().CompareC( _L8("butt") ) == 0 )
                        {
                        item.iStroke.iProperties.iCapStyle = VG_CAP_BUTT;
                        }
                    else if ( lex.Remainder().CompareC( _L8("round") ) == 0 )
                        {
                        item.iStroke.iProperties.iCapStyle = VG_CAP_ROUND;
                        }
                    else if ( lex.Remainder().CompareC( _L8("square") ) == 0 )
                        {
                        item.iStroke.iProperties.iCapStyle = VG_CAP_SQUARE;
                        }
				break;
			case ESvgParserString_stroke_linejoin:
                    if ( lex.Remainder().CompareC( _L8("miter") ) == 0 )
                        {
                        item.iStroke.iProperties.iJoinStyle = VG_JOIN_MITER;
                        }
                    else if ( lex.Remainder().CompareC( _L8("round") ) == 0 )
                        {
                        item.iStroke.iProperties.iJoinStyle = VG_JOIN_ROUND;
                        }
                    else if ( lex.Remainder().CompareC( _L8("bevel") ) == 0 )
                        {
                        item.iStroke.iProperties.iJoinStyle = VG_JOIN_BEVEL;
                        }
				break;
			case ESvgParserString_stroke_miterlimit:
                    lex.Val( mitterLimit, '.' );
                    mitterLimit = iCnvMulX * mitterLimit / iCnvDivX;
                    item.iStroke.iProperties.iMitterLimit = mitterLimit;
				break;
			case ESvgParserString_stroke_dasharray:
				break;
			case ESvgParserString_stroke_dashoffse:
				break;
			case ESvgParserString_stroke_opacity:
					lex.Val( alpha, '.' );
				break;
			default:
				break;
			}
        }
    item.iStroke.iColor.SetAlpha( alpha );
    }

void CVgSvgParser::ParseSvgElement(const RAttributeArray& aAttributes)
    {
    VGfloat w, h, uw = 1, uh = 1, vx, vy, vw, vh, ucx, ucy;
    for ( TInt i = 0; i < aAttributes.Count(); i++ )
        {
        TInt idx = aAttributes[ i ].Attribute().LocalName().Index( KVgSvgParserStringTable );
        const TDesC8& value = aAttributes[ i ].Value().DesC();
        TLex8 lex( value );
        lex.SkipSpace();
        switch ( idx )
            {
            case ESvgParserString_width:
                    lex.Val( w, '.' );
                    uw = UnitCoef( lex.Remainder() );
                    //w *= uw;
                break;
            case ESvgParserString_height:
                    lex.Val( h, '.' );
                    uh = UnitCoef( lex.Remainder() );
                    //h *= uh;
                break;
            case ESvgParserString_viewBox:
                    lex.Val( vx, '.' );
                    lex.SkipCharacters(); // unit
                    lex.SkipSpace();
                    lex.Val( vy, '.' );
                    lex.SkipCharacters(); // unit
                    lex.SkipSpace();
                    lex.Val( vw, '.' );
                    lex.SkipCharacters(); // unit
                    lex.SkipSpace();
                    lex.Val( vh, '.' );
                break;
            default:
                break;
            }
        }
    iCnvMulX = 96;
    iCnvDivX = 72;
    iCnvMulY = 96;
    iCnvDivY = 72;
    if ( vw != 0 && vh != 0 ) 
        {
        ucx = w * uw / vw; 
        ucy = h * uh / vh; 
        iCnvMulX = w * uw;
        iCnvDivX = vw;
        iCnvMulY = h * uh;
        iCnvDivY = vh;
        }
    ParseFillElement( aAttributes );
    ParseStrokeElement( aAttributes );
    
    iViewBox.iTl.iX = iCnvMulX * vx / iCnvDivX;
    iViewBox.iTl.iY = iCnvMulY * ( vy + vh ) / iCnvDivY;
    iViewBox.iBr.iX = iCnvMulX * ( vx + vw ) / iCnvDivX;
    iViewBox.iBr.iY = iCnvMulY * vy / iCnvDivY;
    }

void CVgSvgParser::AddImagePathL( VGPath aPath )
	{
    VGPath pathT = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 4, 4 * 2, VG_PATH_CAPABILITY_ALL );
    CVgSvgParser::TStackItem& item = StackCurrentItem();
    vgLoadMatrix( item.iTransformation.Begin() );
    vgTransformPath( pathT, aPath );
    vgDestroyPath( aPath );
    VGPaint fill = 0, stroke = 0;
    if ( item.iFill.iSpecified )
        {
        fill = item.iFill.iColor.CreateVgPaint();
        }
    if ( item.iStroke.iSpecified )
        {
    	stroke = item.iStroke.iColor.CreateVgPaint();
        }
    if ( iDefsMode )
    	{
    	TDefItem di;
    	di.iFill = fill;
    	di.iStroke = stroke;
    	di.iStrokeProperties = item.iStroke.iProperties;
    	di.iPath = pathT;
    	di.iId.CreateL( iCurrentDefId );
    	di.iUsed = EFalse;
    	iDefs.AppendL( di );
    	}
    else
    	{
    	iImage.AddPathL( pathT, fill, stroke, item.iStroke.iProperties );
    	}
	}

void CVgSvgParser::AddImagePathFromDefsL( TInt aIdx )
	{
    VGPath pathT = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 4, 4 * 2, VG_PATH_CAPABILITY_ALL );
    CVgSvgParser::TStackItem& item = StackCurrentItem();
    vgLoadMatrix( item.iTransformation.Begin() );
    vgTransformPath( pathT, iDefs[ aIdx ].iPath );
    iDefs[ aIdx ].iUsed = ETrue;
	iImage.AddPathL( pathT, iDefs[ aIdx ].iFill, iDefs[ aIdx ].iStroke, iDefs[ aIdx ].iStrokeProperties );
	}

void CVgSvgParser::ParsePathElement(const RAttributeArray& aAttributes)
	{ // http://www.w3.org/TR/SVG/paths.html
	RBuf8 segments;
	CArrayFixFlat< VGfloat > *data = new CArrayFixFlat< VGfloat >( 50 );
	for ( TInt i = 0; i < aAttributes.Count(); i++ )
		{
		TInt idx = aAttributes[ i ].Attribute().LocalName().Index( KVgSvgParserStringTable );
        const TDesC8& value = aAttributes[ i ].Value().DesC();
		switch ( idx )
			{
			case ESvgParserString_d:
				{
				segments.Create( value.Length() );
				TLex8 lex( value );
				while ( !lex.Eos() )
					{
					lex.SkipSpace();
					TInt cmd = 0;
					TInt pointsCountToExtract = 0;
					TChar ccmd = lex.Get();
					TUint ccmdi = ccmd.GetLowerCase();
					if ( ccmdi == 'a' )
						{
						//VGfloat rx, ry, x_rot, x, y;
						TBool large_arc, seep_flag;
						for ( TInt p = 0; p < KVgSvgParserPathCommandsMap[ ccmdi - 'a' ][ 1 ]; p++ )
							{
							lex.SkipSpace();
							TReal32 out;
							lex.Val( out, '.' );
							if ( p == 3 )
								{
								large_arc = ( out != 0 );
								}
							else if ( p == 4 )
								{
								seep_flag = ( out != 0 );
								}
							else 
								{
								if ( p != 2 )
									{
									out = iCnvMulX * out / iCnvDivX;  // todo x or y ?
									}
								data->AppendL( out );
								}
							lex.SkipSpace();
							if ( lex.Peek() == ',' )
								{
								lex.Inc();
								lex.SkipSpace();
								}
							}
						if ( large_arc && seep_flag )
							{
							cmd = VG_LCCWARC_TO;
							}
						else if ( large_arc && !seep_flag )
							{
							cmd = VG_LCWARC_TO;
							}
						else if ( !large_arc && seep_flag )
							{
							cmd = VG_SCCWARC_TO;
							}
						else if ( !large_arc && !seep_flag )
							{
							cmd = VG_SCWARC_TO;
							}
						cmd |= ( ccmd < 'a' ? VG_ABSOLUTE : VG_RELATIVE );
						segments.Append( cmd );
						}
					else if ( ccmdi > 'a' && ccmdi <= 'z' )
						{
						TInt cmd = KVgSvgParserPathCommandsMap[ ccmdi - 'a' ][ 0 ];
						pointsCountToExtract = KVgSvgParserPathCommandsMap[ ccmdi - 'a' ][ 1 ];
						if ( cmd != VG_CLOSE_PATH )
							{
							cmd |= ( ccmd < 'a' ? VG_ABSOLUTE : VG_RELATIVE );
							}
						segments.Append( cmd );
						}
					for ( TInt p = 0; p < pointsCountToExtract; p++ )
						{
						lex.SkipSpace();
						TReal32 out;
						lex.Val( out, '.' );
						out = iCnvMulX * out / iCnvDivX;  // todo x or y ?
						data->AppendL( out );
						lex.SkipSpace();
						if ( lex.Peek() == ',' )
							{
							lex.Inc();
							lex.SkipSpace();
							}
						}
					lex.SkipSpace();
					}
				}
				break;
            case ESvgParserString_transform:
                    ParseTransformElement( value );
                break;
			default:
				break;
			}
		}
	if ( segments.Length() == 0 || data->Count() == 0 )
		{
	    segments.Close();
	    delete data;
	    return;
		}
    ParseFillElement( aAttributes );
    ParseStrokeElement( aAttributes );
    
    VGPath path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, segments.Length(), data->Count(), VG_PATH_CAPABILITY_ALL ); //VG_PATH_CAPABILITY_APPEND_TO | VG_PATH_CAPABILITY_MODIFY | VG_PATH_CAPABILITY_TRANSFORM_TO );
    vgAppendPathData( path, segments.Length(), segments.Ptr(), data->Back( 0 ) );
    AddImagePathL( path );
    
    segments.Close();
    delete data;
	}

void CVgSvgParser::ParseRectElement(const RAttributeArray& aAttributes)
    {
    VGfloat x = 0, y = 0, w, h, rx = -1, ry = -1;
    for ( TInt i = 0; i < aAttributes.Count(); i++ )
        {
        TInt idx = aAttributes[ i ].Attribute().LocalName().Index( KVgSvgParserStringTable );
        const TDesC8& value = aAttributes[ i ].Value().DesC();
        TLex8 lex( value );
        lex.SkipSpace();
        switch ( idx )
            {
            case ESvgParserString_x:
                    lex.Val( x, '.' );
                    x = iCnvMulX * x / iCnvDivX;  // todo x or y ?
                break;
            case ESvgParserString_y:
                    lex.Val( y, '.' );
                    y = iCnvMulY * y / iCnvDivY;  // todo x or y ?
                break;
            case ESvgParserString_width:
                    lex.Val( w, '.' );
                    w = iCnvMulX * w / iCnvDivX;  // todo x or y ?
                break;
            case ESvgParserString_height:
                    lex.Val( h, '.' );
                    h = iCnvMulY * h / iCnvDivY;  // todo x or y ?
                break;
            case ESvgParserString_rx:
                    lex.Val( rx, '.' );
                    rx = iCnvMulX * rx / iCnvDivX;  // todo x or y ?
                    if ( ry == -1 )
                        {
                        ry = rx;
                        }
                break;
            case ESvgParserString_ry:
                    lex.Val( ry, '.' );
                    ry = iCnvMulY * ry / iCnvDivY;  // todo x or y ?
                break;
            case ESvgParserString_transform:
                    ParseTransformElement( value );
                break;
            default:
                break;
            }
        }
    ParseFillElement( aAttributes );
    ParseStrokeElement( aAttributes );
    
    VGPath path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 4, 4 * 2, VG_PATH_CAPABILITY_ALL ); //VG_PATH_CAPABILITY_APPEND_TO | VG_PATH_CAPABILITY_MODIFY | VG_PATH_CAPABILITY_TRANSFORM_TO );
    if ( ( rx == -1 && ry == -1 ) ||
         ( rx == 0 && ry == 0 ) )
        {
        vguRect( path, x, y, w, h );
        }
    else
        {
        vguRoundRect( path, x, y, w, h, rx, ry );
        }
    AddImagePathL( path );
    }

void CVgSvgParser::ParseCircleElement(const RAttributeArray& aAttributes)
    {
    VGfloat cx = 0, cy = 0, r;
    for ( TInt i = 0; i < aAttributes.Count(); i++ )
        {
        TInt idx = aAttributes[ i ].Attribute().LocalName().Index( KVgSvgParserStringTable );
        const TDesC8& value = aAttributes[ i ].Value().DesC();
        TLex8 lex( value );
        lex.SkipSpace();
        switch ( idx )
            {
            case ESvgParserString_cx:
                    lex.Val( cx, '.' );
                    cx = iCnvMulX * cx / iCnvDivX;  // todo x or y ?
                break;
            case ESvgParserString_cy:
                    lex.Val( cy, '.' );
                    cy = iCnvMulY * cy / iCnvDivY;  // todo x or y ?
                break;
            case ESvgParserString_r:
                    lex.Val( r, '.' );
                    r = iCnvMulX * r / iCnvDivX;  // todo x or y ?
                break;
            case ESvgParserString_transform:
                    ParseTransformElement( value );
                break;
            default:
                break;
            }
        }
    ParseFillElement( aAttributes );
    ParseStrokeElement( aAttributes );
    
    VGPath path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 4, 4 * 2, VG_PATH_CAPABILITY_ALL ); //VG_PATH_CAPABILITY_APPEND_TO | VG_PATH_CAPABILITY_MODIFY | VG_PATH_CAPABILITY_TRANSFORM_TO );
    vguEllipse( path, cx, cy, 2 * r, 2 * r );
    AddImagePathL( path );
    }

void CVgSvgParser::ParseEllipseElement(const RAttributeArray& aAttributes)
	{
	VGfloat cx = 0, cy = 0, rx, ry;
	for ( TInt i = 0; i < aAttributes.Count(); i++ )
		{
		TInt idx = aAttributes[ i ].Attribute().LocalName().Index( KVgSvgParserStringTable );
		const TDesC8& value = aAttributes[ i ].Value().DesC();
		TLex8 lex( value );
		lex.SkipSpace();
		switch ( idx )
			{
			case ESvgParserString_cx:
					lex.Val( cx, '.' );
					cx = iCnvMulX * cx / iCnvDivX;  // todo x or y ?
				break;
			case ESvgParserString_cy:
					lex.Val( cy, '.' );
					cy = iCnvMulY * cy / iCnvDivY;  // todo x or y ?
				break;
			case ESvgParserString_rx:
					lex.Val( rx, '.' );
					rx = iCnvMulX * rx / iCnvDivX;  // todo x or y ?
				break;
			case ESvgParserString_ry:
					lex.Val( ry, '.' );
					ry = iCnvMulY * ry / iCnvDivY;  // todo x or y ?
				break;
			case ESvgParserString_transform:
					ParseTransformElement( value );
				break;
			default:
				break;
			}
		}
	ParseFillElement( aAttributes );
	ParseStrokeElement( aAttributes );
	
	VGPath path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 4, 4 * 2, VG_PATH_CAPABILITY_ALL ); //VG_PATH_CAPABILITY_APPEND_TO | VG_PATH_CAPABILITY_MODIFY | VG_PATH_CAPABILITY_TRANSFORM_TO );
	vguEllipse( path, cx, cy, 2 * rx, 2 * ry );
    AddImagePathL( path );
	}

void CVgSvgParser::ParseLineElement(const RAttributeArray& aAttributes)
	{
	VGfloat x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	for ( TInt i = 0; i < aAttributes.Count(); i++ )
		{
		TInt idx = aAttributes[ i ].Attribute().LocalName().Index( KVgSvgParserStringTable );
		const TDesC8& value = aAttributes[ i ].Value().DesC();
		TLex8 lex( value );
		lex.SkipSpace();
		switch ( idx )
			{
			case ESvgParserString_x1:
					lex.Val( x1, '.' );
					x1 = iCnvMulX * x1 / iCnvDivX;  // todo x or y ?
				break;
			case ESvgParserString_y1:
					lex.Val( y1, '.' );
					y1 = iCnvMulY * y1 / iCnvDivY;  // todo x or y ?
				break;
			case ESvgParserString_x2:
					lex.Val( x2, '.' );
					x2 = iCnvMulX * x2 / iCnvDivX;  // todo x or y ?
				break;
			case ESvgParserString_y2:
					lex.Val( y2, '.' );
					y2 = iCnvMulY * y2 / iCnvDivY;  // todo x or y ?
				break;
			case ESvgParserString_transform:
					ParseTransformElement( value );
				break;
			default:
				break;
			}
		}
	ParseFillElement( aAttributes );
	ParseStrokeElement( aAttributes );
	
	VGPath path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 4, 4 * 2, VG_PATH_CAPABILITY_ALL ); //VG_PATH_CAPABILITY_APPEND_TO | VG_PATH_CAPABILITY_MODIFY | VG_PATH_CAPABILITY_TRANSFORM_TO );
	vguLine( path, x1, y1, x2, y2 );
    AddImagePathL( path );
	}

void CVgSvgParser::ParsePolylineElement(const RAttributeArray& aAttributes, TBool aPolygon)
	{
	RBuf8 segments;
	CArrayFixFlat< VGfloat > *data = new CArrayFixFlat< VGfloat >( 50 );

	for ( TInt i = 0; i < aAttributes.Count(); i++ )
		{
		TInt idx = aAttributes[ i ].Attribute().LocalName().Index( KVgSvgParserStringTable );
		const TDesC8& value = aAttributes[ i ].Value().DesC();
		TLex8 lex( value );
		lex.SkipSpace();
		switch ( idx )
			{
			case ESvgParserString_points:
				{
				segments.Create( value.Length() );
				
				TLex8 lex( value );
				TInt i = 0;
				while ( !lex.Eos() )
					{
					lex.SkipSpace();
					if ( i == 0 )
						{
						segments.Append( VG_MOVE_TO_ABS );
						}
					else 
						{
						segments.Append( VG_LINE_TO_ABS );
						}
					i++;
					for ( TInt p = 0; p < 2; p++ )
						{
						lex.SkipSpace();
						TReal32 out;
						lex.Val( out, '.' );
						out = iCnvMulX * out / iCnvDivX;  // todo x or y ?
						data->AppendL( out );
						//data.Append( out );
						lex.SkipSpace();
						if ( lex.Peek() == ',' )
							{
							lex.Inc();
							lex.SkipSpace();
							}
						}
					lex.SkipSpace();
					}
				if ( aPolygon )
					{
					segments.Append( VG_CLOSE_PATH );
					}
				}
				break;
			case ESvgParserString_transform:
					ParseTransformElement( value );
				break;
			default:
				break;
			}
		}
	if ( segments.Length() == 0 || data->Count() == 0 )
		{
	    segments.Close();
	    delete data;
	    return;
		}
	ParseFillElement( aAttributes );
	ParseStrokeElement( aAttributes );
	
    VGPath path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, segments.Length(), data->Count(), VG_PATH_CAPABILITY_ALL ); //VG_PATH_CAPABILITY_APPEND_TO | VG_PATH_CAPABILITY_MODIFY | VG_PATH_CAPABILITY_TRANSFORM_TO );
    vgAppendPathData( path, segments.Length(), segments.Ptr(), data->Back( 0 ) );
    AddImagePathL( path );
    
    segments.Close();
    delete data;
	}

void CVgSvgParser::ParseGroupElement(const RAttributeArray& aAttributes)
    {
    for ( TInt i = 0; i < aAttributes.Count(); i++ )
        {
        TInt idx = aAttributes[ i ].Attribute().LocalName().Index( KVgSvgParserStringTable );
        switch ( idx )
            {
            case ESvgParserString_transform:
					ParseTransformElement( aAttributes[ i ].Value().DesC() );
                break;
            case ESvgParserString_display:
                break;
            case ESvgParserString_visibility:
                break;
            case ESvgParserString_id:
					if ( iDefsMode )
						{
						iCurrentDefId.Close();
						iCurrentDefId.Create( aAttributes[ i ].Value().DesC() );
						}
                break;
            default:
                break;
            }
        }
    ParseFillElement( aAttributes );
    ParseStrokeElement( aAttributes );
    }

void CVgSvgParser::ParseUseElement(const RAttributeArray& aAttributes)
    {
	RBuf8 href;
    for ( TInt i = 0; i < aAttributes.Count(); i++ )
        {
        TInt idx = aAttributes[ i ].Attribute().LocalName().Index( KVgSvgParserStringTable );
        switch ( idx )
            {
            case ESvgParserString_transform:
					//ParseTransformElement( aAttributes[ i ].Value().DesC() );
                break;
            case ESvgParserString_href:
					href.Create( aAttributes[ i ].Value().DesC() );
                break;
            default:
                break;
            }
        }
    //ParseFillElement( aAttributes );
    //ParseStrokeElement( aAttributes );
    
    if ( href.Length() > 0 )
    	{
    	if ( href[ 0 ] == '#' )
    		{
    		href.Delete( 0, 1 );
    		}
		for ( TInt i = 0; i < iDefs.Count(); i++ )
			{
			if ( iDefs[ i ].iId.Compare( href ) == 0 )
				{
				AddImagePathFromDefsL( i );
				}
			}
    	}
    href.Close();
    }

void CVgSvgParser::StackPushCopy()
	{
	ASSERT( iStack.Count() > 0 );
	CVgSvgParser::TStackItem item = iStack[ iStack.Count() - 1 ];
	iStack.Append( item );
	}

void CVgSvgParser::StackPop()
	{
	ASSERT( iStack.Count() > 0 );
	iStack.Remove( iStack.Count() - 1 );
	}

CVgSvgParser::TStackItem& CVgSvgParser::StackCurrentItem()
	{
	return iStack[ iStack.Count() - 1 ];
	}

CVgSvgParser::TStackItem* CVgSvgParser::StackParentItem()
    {
    if ( iStack.Count() < 2 )
        {
        return NULL;
        }
    return &iStack[ iStack.Count() - 2 ];
    }

void CVgSvgParser::OnStartDocumentL(const RDocumentParameters& /*aDocParam*/, TInt /*aErrorCode*/)
	{
	CON_WRITE( _L("OnStartDocumentL") );
	
	ASSERT( iStack.Count() == 0 );
	TStackItem item;
    item.iFill.iSpecified = EFalse;
    item.iStroke.iSpecified = EFalse;
    vgSeti( VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE );
    vgLoadIdentity();
    vgGetMatrix( item.iTransformation.Begin() );
	iStack.Append( item );
	}

void CVgSvgParser::OnEndDocumentL(TInt /*aErrorCode*/)
	{
	CON_WRITE( _L("OnEndDocumentL") );
	
	iStack.Remove( iStack.Count() - 1 );
	ASSERT( iStack.Count() == 0 );
	}

void CVgSvgParser::OnStartElementL(const RTagInfo& aElement, const RAttributeArray& aAttributes,TInt /*aErrorCode*/)
	{ 
	CON_WRITE( _L("OnStartElementL") );
	CON_WRITE8( aElement.LocalName().DesC() );
	if ( iElementBypass )
	    {
	    return;
	    }
	TInt idx = aElement.LocalName().Index( KVgSvgParserStringTable );
	StackPushCopy();
	switch ( idx )
		{
        case ESvgParserString_svg:
                ParseSvgElement( aAttributes );
            break;
        /*case ESvgParserString_style:
        case ESvgParserString_defs:
				StackPop();
                iElementBypass++;
            break;*/
        case ESvgParserString_defs:
				iDefsMode = ETrue;
        	break;
        case ESvgParserString_path:
                ParsePathElement( aAttributes );
            break;
        case ESvgParserString_rect:
                ParseRectElement( aAttributes );
            break;
        case ESvgParserString_circle:
                ParseCircleElement( aAttributes );
            break;
        case ESvgParserString_ellipse:
                ParseEllipseElement( aAttributes );
            break;
        case ESvgParserString_line:
                ParseLineElement( aAttributes );
            break;
        case ESvgParserString_polyline:
                ParsePolylineElement( aAttributes, EFalse );
            break;
        case ESvgParserString_polygon:
				ParsePolylineElement( aAttributes, ETrue );
            break;
        case ESvgParserString_g:
                ParseGroupElement( aAttributes );
            break;
        case ESvgParserString_use:
                ParseUseElement( aAttributes );
            break;
		default:
			break;
		}
	for ( TInt i = 0; i < aAttributes.Count(); i++ )
		{
		CON_WRITE( _L("_ attr:") );
		CON_WRITE8( aAttributes[ i ].Attribute().LocalName().DesC() );
		CON_WRITE8( aAttributes[ i ].Value().DesC() );
		}
	}

void CVgSvgParser::OnEndElementL(const RTagInfo& aElement, TInt /*aErrorCode*/)
	{
	CON_WRITE( _L("OnEndElementL") );
	CON_WRITE8( aElement.LocalName().DesC() );
    TInt idx = aElement.LocalName().Index( KVgSvgParserStringTable );
	if ( iElementBypass )
	    {
	    switch ( idx )
	         {
	         /*case ESvgParserString_style:
	         case ESvgParserString_defs:
	                 iElementBypass--;
	             break;*/
	         default:
	             break;
	         }
	    return;
	    }
    switch ( idx )
         {
         case ESvgParserString_defs:
 				iDefsMode = EFalse;
         	break;
         default:
             break;
         }
	StackPop();
	}

void CVgSvgParser::OnContentL(const TDesC8& aBytes, TInt /*aErrorCode*/)
	{
	CON_WRITE( _L("OnContentL") );
	CON_WRITE8( aBytes );
	}

void CVgSvgParser::OnStartPrefixMappingL(const RString& /*aPrefix*/, const RString& /*aUri*/, TInt /*aErrorCode*/)
	{
	CON_WRITE( _L("OnStartPrefixMappingL") );
	
	}

void CVgSvgParser::OnEndPrefixMappingL(const RString& /*aPrefix*/, TInt /*aErrorCode*/)
	{
	CON_WRITE( _L("OnEndPrefixMappingL") );

	}

void CVgSvgParser::OnIgnorableWhiteSpaceL(const TDesC8& /*aBytes*/, TInt /*aErrorCode*/)
	{
	CON_WRITE( _L("OnIgnorableWhiteSpaceL") );

	}

void CVgSvgParser::OnSkippedEntityL(const RString& /*aName*/, TInt /*aErrorCode*/)
	{
	CON_WRITE( _L("OnSkippedEntityL") );

	}

void CVgSvgParser::OnProcessingInstructionL(const TDesC8& /*aTarget*/, const TDesC8& /*aData*/, TInt /*aErrorCode*/)
	{
	CON_WRITE( _L("OnProcessingInstructionL") );

	}

void CVgSvgParser::OnError(TInt aErrorCode)
	{
	TBuf<64> buf;
	buf.Copy(  _L("OnError: ") );
	buf.AppendNum( aErrorCode );
	CON_WRITE( buf );

	}

TAny* CVgSvgParser::GetExtendedInterface(const TInt32 /*aUid*/)
	{
	CON_WRITE( _L("GetExtendedInterface") );
	return NULL;
	}


