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
#include <e32debug.h>
#include <vg/openvg.h>
#include <vg/vgu.h>

_LIT8(KSvgMimeType, "text/xml");
//_LIT8(KSvgMimeType, "image/svg+xml");



#include <e32cons.h>			// Console
GLREF_D CConsoleBase* console; // write all messages to this
//#define CON_WRITE( x ) console->Write( x )
//#define CON_WRITE8( x ) { RBuf b; b.Create( x.Length() ); b.Copy( x ); console->Write( b ); b.Close(); }
//#define CON_PRINTF( x ) console->Printf( x )
#define CON_WRITE( x ) RDebug::RawPrint( x )
#define CON_WRITE8( x ) RDebug::RawPrint( x )




CVgSvgParser::CVgSvgParser( CVgSvgImage& aImage ) : iImage( aImage ), iDpi( 96 ), iCnvMulX( 1 ), iCnvDivX( 1 ), iCnvMulY( 1 ), iCnvDivY( 1 )
	{
	}

CVgSvgParser::~CVgSvgParser()
	{
	delete iParser;
	iStack.Close();
	}

CVgSvgParser* CVgSvgParser::NewLC( CVgSvgImage& aImage )
	{
	CVgSvgParser* self = new (ELeave) CVgSvgParser( aImage );
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CVgSvgParser* CVgSvgParser::NewL( CVgSvgImage& aImage )
	{
	CVgSvgParser* self = CVgSvgParser::NewLC( aImage );
	CleanupStack::Pop(); // self;
	return self;
	}

void CVgSvgParser::ConstructL()
	{
	iParser = CParser::NewL( KSvgMimeType(), *this );
	}

void CVgSvgParser::ParseL( const TDesC8& aData )
	{
	iParser->StringPool().OpenL( KVgSvgParserStringTable );
	iParser->ParseL( aData );
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
    vgLoadMatrix( StackCurrentItem().iTransformation.Begin() );
    while ( lex.Eos() )
    	{
		if ( lex.Remainder().Left( 9 ).CompareC( _L8("translate") ) == 0 )
			{
			lex.Inc( 9 );
			ExtractPair( lex, tx, ty );
		    vgTranslate( tx, ty );
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
			    vgTranslate( cx, cy );
				vgRotate( angle );
			    vgTranslate( -cx, -cy );
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
    vgGetMatrix( StackCurrentItem().iTransformation.Begin() );
	}

void CVgSvgParser::ParseFillElement(const RAttributeArray& aAttributes)
    {
	TUint32 color;
	TReal32 alpha = 1;
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
						StackCurrentItem().iFill.iSpecified = EFalse;
						}
					else
						{
						StackCurrentItem().iFill.iSpecified = ETrue;
						}
					if ( lex.Peek() == '#' )
						{
						lex.Inc();
						lex.Val( color, EHex );
						}
					else if ( lex.Remainder().Left( 3 ).CompareC( _L8("rgb") ) == 0 )
						{
						
						}
					else if ( lex.Remainder().CompareC( _L8("red") ) == 0 )
						{
						color = 0xFF0000;
						}
					else if ( lex.Remainder().CompareC( _L8("blue") ) == 0 )
						{
						color = 0x0000FF;
						}
					else if ( lex.Remainder().CompareC( _L8("green") ) == 0 )
						{
						color = 0x00FF00;
						}
					else if ( lex.Remainder().CompareC( _L8("black") ) == 0 )
						{
						color = 0x000000;
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
	StackCurrentItem().iFill.iColor.SetInternal( color );
	StackCurrentItem().iFill.iAlpha = alpha;
    }

void CVgSvgParser::ParseStrokeElement(const RAttributeArray& aAttributes)
    {
	TUint32 color;
	TReal32 alpha = 1, width = 1;
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
						StackCurrentItem().iStroke.iSpecified = EFalse;
						}
					else
						{
						StackCurrentItem().iStroke.iSpecified = ETrue;
						}
					if ( lex.Peek() == '#' )
						{
						lex.Inc();
						lex.Val( color, EHex );
						}
					else if ( lex.Remainder().Left( 3 ).CompareC( _L8("rgb") ) == 0 )
						{
						
						}
					else if ( lex.Remainder().CompareC( _L8("red") ) == 0 )
						{
						color = 0xFF0000;
						}
					else if ( lex.Remainder().CompareC( _L8("blue") ) == 0 )
						{
						color = 0x0000FF;
						}
					else if ( lex.Remainder().CompareC( _L8("green") ) == 0 )
						{
						color = 0x00FF00;
						}
					else if ( lex.Remainder().CompareC( _L8("black") ) == 0 )
						{
						color = 0x000000;
						}
				break;
			case ESvgParserString_stroke_width:
					lex.Val( width, '.' );
				break;
			case ESvgParserString_stroke_linecap:
				break;
			case ESvgParserString_stroke_linejoin:
				break;
			case ESvgParserString_stroke_miterlimit:
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
	StackCurrentItem().iStroke.iColor.SetInternal( color );
	StackCurrentItem().iStroke.iAlpha = alpha;
	StackCurrentItem().iStroke.iWidth = width;
    }

void CVgSvgParser::ParseSvgElement(const RAttributeArray& aAttributes)
    {
    VGfloat w, h, uw, uh, vx, vy, vw, vh, ucx, ucy;
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
        }
    ParseFillElement( aAttributes );
    ParseStrokeElement( aAttributes );
    }

void CVgSvgParser::ParsePathElement(const RAttributeArray& aAttributes)
	{ // http://www.w3.org/TR/SVG/paths.html
	RBuf8 segments;
	//RArray< VGfloat > data;
	CArrayFixFlat< VGfloat > *data = new CArrayFixFlat< VGfloat >( 50 );
	for ( TInt i = 0; i < aAttributes.Count(); i++ )
		{
		TInt idx = aAttributes[ i ].Attribute().LocalName().Index( KVgSvgParserStringTable );
		switch ( idx )
			{
			case ESvgParserString_d:
				{
				const TDesC8& value = aAttributes[ i ].Value().DesC();
				segments.Create( value.Length() );
				TLex8 lex( value );
				while ( !lex.Eos() )
					{
					lex.SkipSpace();
					TInt pointsCountToExtract = 0;
					TChar ccmd = lex.Get();
					TUint ccmdi = ccmd.GetLowerCase();
					if ( ccmdi == 'a' )
						{ // todo: parse ARC
						
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
				}
				break;
			default:
				break;
			}
		}
    ParseFillElement( aAttributes );
    ParseStrokeElement( aAttributes );
    
    VGPath path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, segments.Length(), data->Count(), VG_PATH_CAPABILITY_APPEND_TO | VG_PATH_CAPABILITY_MODIFY | VG_PATH_CAPABILITY_TRANSFORM_TO );
    vgAppendPathData( path, segments.Length(), segments.Ptr(), data->Back( 0 ) );
    vgTransformPath( path, path );
    VGPaint fill = 0, stroke = 0;
    if ( StackCurrentItem().iFill.iSpecified )
        {
        fill = vgCreatePaint();
        vgSetParameteri( fill, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR );
        StackCurrentItem().iFill.iColor.SetAlpha( StackCurrentItem().iFill.iAlpha * 255 );
        vgSetColor( fill, StackCurrentItem().iFill.iColor.Internal() );
        }
    if ( StackCurrentItem().iStroke.iSpecified )
        {
        stroke = vgCreatePaint();
        vgSetParameteri( stroke, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR );
        StackCurrentItem().iStroke.iColor.SetAlpha( StackCurrentItem().iStroke.iAlpha * 255 );
        vgSetColor( stroke, StackCurrentItem().iStroke.iColor.Internal() );
        }
    iImage.AddPathL( path, fill, stroke, StackCurrentItem().iStroke.iWidth );
    
    segments.Close();
    delete data;
	}

void CVgSvgParser::ParseRectElement(const RAttributeArray& aAttributes)
    {
    VGfloat x, y, w, h, rx = 0, ry = 0;
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
                break;
            case ESvgParserString_ry:
                    lex.Val( ry, '.' );
                    ry = iCnvMulY * ry / iCnvDivY;  // todo x or y ?
                break;
            default:
                break;
            }
        }
    ParseFillElement( aAttributes );
    ParseStrokeElement( aAttributes );
    
    VGPath path = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 4, 4 * 2, VG_PATH_CAPABILITY_APPEND_TO | VG_PATH_CAPABILITY_MODIFY | VG_PATH_CAPABILITY_TRANSFORM_TO );
    if ( rx == 0 && ry == 0 )
        {
        vguRect( path, x, y, w, h );
        }
    else
        {
        vguRoundRect( path, x, y, w, h, rx, ry );
        }
    vgTransformPath( path, path );
    VGPaint fill = 0, stroke = 0;
    if ( StackCurrentItem().iFill.iSpecified )
        {
        fill = vgCreatePaint();
        vgSetParameteri( fill, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR );
        StackCurrentItem().iFill.iColor.SetAlpha( StackCurrentItem().iFill.iAlpha * 255 );
        vgSetColor( fill, StackCurrentItem().iFill.iColor.Internal() );
        }
    if ( StackCurrentItem().iStroke.iSpecified )
        {
        stroke = vgCreatePaint();
        vgSetParameteri( stroke, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR );
        StackCurrentItem().iStroke.iColor.SetAlpha( StackCurrentItem().iStroke.iAlpha * 255 );
        vgSetColor( stroke, StackCurrentItem().iStroke.iColor.Internal() );
        }
    iImage.AddPathL( path, fill, stroke, StackCurrentItem().iStroke.iWidth );
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
            default:
                break;
            }
        }
    ParseFillElement( aAttributes );
    ParseStrokeElement( aAttributes );
    }

void CVgSvgParser::StackPushCopy()
	{
	ASSERT( iStack.Count() > 0 );
	iStack.Append( iStack[ iStack.Count() - 1 ] );
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

void CVgSvgParser::OnStartDocumentL(const RDocumentParameters& aDocParam, TInt aErrorCode)
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

void CVgSvgParser::OnEndDocumentL(TInt aErrorCode)
	{
	CON_WRITE( _L("OnEndDocumentL") );
	
	iStack.Remove( iStack.Count() - 1 );
	ASSERT( iStack.Count() == 0 );
	}

void CVgSvgParser::OnStartElementL(const RTagInfo& aElement, const RAttributeArray& aAttributes,TInt aErrorCode)
	{ 
	CON_WRITE( _L("OnStartElementL") );
	CON_WRITE8( aElement.LocalName().DesC() );
	TInt idx = aElement.LocalName().Index( KVgSvgParserStringTable );
	// todo: insert on stack: transform, fill, stroke (copy from top most)
	switch ( idx )
		{
        case ESvgParserString_svg:
                ParseSvgElement( aAttributes );
            break;
        case ESvgParserString_path:
                ParsePathElement( aAttributes );
            break;
        case ESvgParserString_rect:
                ParseRectElement( aAttributes );
            break;
        case ESvgParserString_g:
                ParseGroupElement( aAttributes );
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

void CVgSvgParser::OnEndElementL(const RTagInfo& aElement, TInt aErrorCode)
	{
	CON_WRITE( _L("OnEndElementL") );
	CON_WRITE8( aElement.LocalName().DesC() );
	
	// todo: pop from stack: transform, fill, stroke
	}

void CVgSvgParser::OnContentL(const TDesC8& aBytes, TInt aErrorCode)
	{
	CON_WRITE( _L("OnContentL") );
	CON_WRITE8( aBytes );
	}

void CVgSvgParser::OnStartPrefixMappingL(const RString& aPrefix, const RString& aUri, TInt aErrorCode)
	{
	CON_WRITE( _L("OnStartPrefixMappingL") );
	
	}

void CVgSvgParser::OnEndPrefixMappingL(const RString& aPrefix, TInt aErrorCode)
	{
	CON_WRITE( _L("OnEndPrefixMappingL") );

	}

void CVgSvgParser::OnIgnorableWhiteSpaceL(const TDesC8& aBytes, TInt aErrorCode)
	{
	CON_WRITE( _L("OnIgnorableWhiteSpaceL") );

	}

void CVgSvgParser::OnSkippedEntityL(const RString& aName, TInt aErrorCode)
	{
	CON_WRITE( _L("OnSkippedEntityL") );

	}

void CVgSvgParser::OnProcessingInstructionL(const TDesC8& aTarget, const TDesC8& aData, TInt aErrorCode)
	{
	CON_WRITE( _L("OnProcessingInstructionL") );

	}

void CVgSvgParser::OnError(TInt aErrorCode)
	{
	CON_WRITE( _L("OnError") );

	}

TAny* CVgSvgParser::GetExtendedInterface(const TInt32 aUid)
	{
	CON_WRITE( _L("GetExtendedInterface") );

	}


