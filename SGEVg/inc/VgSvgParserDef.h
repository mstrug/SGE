/*
 ============================================================================
 Name		: VgSvgParserDef.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CVgSvgParser declaration
 ============================================================================
 */

#ifndef VGSVGPARSERDEF_H
#define VGSVGPARSERDEF_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <vg/openvg.h>


_LIT8(KVgSvgParserStr_1, "svg");
_LIT8(KVgSvgParserStr_2, "path");
_LIT8(KVgSvgParserStr_3, "rect");
_LIT8(KVgSvgParserStr_4, "circle");
_LIT8(KVgSvgParserStr_5, "ellipse");
_LIT8(KVgSvgParserStr_6, "line");
_LIT8(KVgSvgParserStr_7, "polyline");
_LIT8(KVgSvgParserStr_8, "polygon");
_LIT8(KVgSvgParserStr_9, "width");
_LIT8(KVgSvgParserStr_10, "height");
_LIT8(KVgSvgParserStr_11, "x");
_LIT8(KVgSvgParserStr_12, "y");
_LIT8(KVgSvgParserStr_13, "rx");
_LIT8(KVgSvgParserStr_14, "ry");
_LIT8(KVgSvgParserStr_15, "cx");
_LIT8(KVgSvgParserStr_16, "cy");
_LIT8(KVgSvgParserStr_17, "r");
_LIT8(KVgSvgParserStr_18, "x1");
_LIT8(KVgSvgParserStr_19, "y1");
_LIT8(KVgSvgParserStr_20, "x2");
_LIT8(KVgSvgParserStr_21, "y2");
_LIT8(KVgSvgParserStr_22, "points");
_LIT8(KVgSvgParserStr_23, "d");
_LIT8(KVgSvgParserStr_24, "fill");
_LIT8(KVgSvgParserStr_25, "fill-rule");
_LIT8(KVgSvgParserStr_26, "fill-opacity");
_LIT8(KVgSvgParserStr_27, "stroke");
_LIT8(KVgSvgParserStr_28, "stroke-width");
_LIT8(KVgSvgParserStr_29, "stroke-linecap");
_LIT8(KVgSvgParserStr_30, "stroke-linejoin");
_LIT8(KVgSvgParserStr_31, "stroke-miterlimit");
_LIT8(KVgSvgParserStr_32, "stroke-dasharray");
_LIT8(KVgSvgParserStr_33, "stroke-dashoffset");
_LIT8(KVgSvgParserStr_34, "stroke-opacity");
_LIT8(KVgSvgParserStr_35, "display");
_LIT8(KVgSvgParserStr_36, "visibility");
_LIT8(KVgSvgParserStr_37, "g");
_LIT8(KVgSvgParserStr_38, "transform");     /* http://www.w3.org/TR/SVG/coords.html */
_LIT8(KVgSvgParserStr_39, "viewBox");
_LIT8(KVgSvgParserStr_40, "defs");
_LIT8(KVgSvgParserStr_41, "style");
_LIT8(KVgSvgParserStr_42, "use");
_LIT8(KVgSvgParserStr_43, "href");
_LIT8(KVgSvgParserStr_44, "id");


const void * const KVgSvgParserStringPointers[] =
{
(const void*)&KVgSvgParserStr_1,
(const void*)&KVgSvgParserStr_2,
(const void*)&KVgSvgParserStr_3,
(const void*)&KVgSvgParserStr_4,
(const void*)&KVgSvgParserStr_5,
(const void*)&KVgSvgParserStr_6,
(const void*)&KVgSvgParserStr_7,
(const void*)&KVgSvgParserStr_8,
(const void*)&KVgSvgParserStr_9,
(const void*)&KVgSvgParserStr_10,
(const void*)&KVgSvgParserStr_11,
(const void*)&KVgSvgParserStr_12,
(const void*)&KVgSvgParserStr_13,
(const void*)&KVgSvgParserStr_14,
(const void*)&KVgSvgParserStr_15,
(const void*)&KVgSvgParserStr_16,
(const void*)&KVgSvgParserStr_17,
(const void*)&KVgSvgParserStr_18,
(const void*)&KVgSvgParserStr_19,
(const void*)&KVgSvgParserStr_20,
(const void*)&KVgSvgParserStr_21,
(const void*)&KVgSvgParserStr_22,
(const void*)&KVgSvgParserStr_23,
(const void*)&KVgSvgParserStr_24,
(const void*)&KVgSvgParserStr_25,
(const void*)&KVgSvgParserStr_26,
(const void*)&KVgSvgParserStr_27,
(const void*)&KVgSvgParserStr_28,
(const void*)&KVgSvgParserStr_29,
(const void*)&KVgSvgParserStr_30,
(const void*)&KVgSvgParserStr_31,
(const void*)&KVgSvgParserStr_32,
(const void*)&KVgSvgParserStr_33,
(const void*)&KVgSvgParserStr_34,
(const void*)&KVgSvgParserStr_35,
(const void*)&KVgSvgParserStr_36,
(const void*)&KVgSvgParserStr_37,
(const void*)&KVgSvgParserStr_38,
(const void*)&KVgSvgParserStr_39,
(const void*)&KVgSvgParserStr_40,
(const void*)&KVgSvgParserStr_41,
(const void*)&KVgSvgParserStr_42,
(const void*)&KVgSvgParserStr_43,
(const void*)&KVgSvgParserStr_44
};
const TStringTable KVgSvgParserStringTable = { 44, KVgSvgParserStringPointers, ETrue };

enum TVgSvgParserStringTable
	{
	ESvgParserString_svg,
	ESvgParserString_path,
	ESvgParserString_rect,
	ESvgParserString_circle,
	ESvgParserString_ellipse,
	ESvgParserString_line,
	ESvgParserString_polyline,
	ESvgParserString_polygon,
	ESvgParserString_width,
	ESvgParserString_height,
	ESvgParserString_x,
	ESvgParserString_y,
	ESvgParserString_rx,
	ESvgParserString_ry,
	ESvgParserString_cx,
	ESvgParserString_cy,
	ESvgParserString_r,
	ESvgParserString_x1,
	ESvgParserString_y1,
	ESvgParserString_x2,
	ESvgParserString_y2,
	ESvgParserString_points,
	ESvgParserString_d,
	ESvgParserString_fill,
	ESvgParserString_fill_rule,
	ESvgParserString_fill_opacity,
	ESvgParserString_stroke,
	ESvgParserString_stroke_width,
	ESvgParserString_stroke_linecap,
	ESvgParserString_stroke_linejoin,
	ESvgParserString_stroke_miterlimit,
	ESvgParserString_stroke_dasharray,
	ESvgParserString_stroke_dashoffse,
	ESvgParserString_stroke_opacity,
	ESvgParserString_display,
	ESvgParserString_visibility,
	ESvgParserString_g,
    ESvgParserString_transform,
    ESvgParserString_viewBox,
    ESvgParserString_defs,
    ESvgParserString_style,
    ESvgParserString_use,
    ESvgParserString_href,
    ESvgParserString_id
	};





const TInt KVgSvgParserPathCommandsMap[][ 2 ] =
{	/* VG path command, number of parameters */
		{ VG_SCCWARC_TO | VG_SCWARC_TO | VG_LCCWARC_TO | VG_LCWARC_TO, 7 },		// a	->	needs additional parsing!
		{ -1 },					// b
		{ VG_CUBIC_TO, 6 },		// c
		{ -1 },					// d
		{ -1 },					// e
		{ -1 },					// f
		{ -1 },					// g
		{ VG_HLINE_TO, 1 },		// h
		{ -1 },					// i
		{ -1 },					// j
		{ -1 },					// k
		{ VG_LINE_TO, 2 },		// l
		{ VG_MOVE_TO, 2 },		// m
		{ -1 },					// n
		{ -1 },					// o
		{ -1 },					// p
		{ VG_QUAD_TO, 4 },		// q
		{ -1 },					// r
		{ VG_SCUBIC_TO, 4 },	// s
		{ VG_SQUAD_TO, 2 },		// t
		{ -1 },					// u
		{ VG_VLINE_TO, 1 },		// v
		{ -1 },					// w
		{ -1 },					// x
		{ -1 },					// y
		{ VG_CLOSE_PATH, 0 }	// z
};



typedef struct
{
	TBuf8<24> iColorName;	
	TUint32 iColor;
} TVgSvgParserColorEntry;
//#define rgb( r, g, b ) TVgColor( TRgb( r, g, b ) )
#define rgb( r, g, b ) ( ( r << 16 ) | ( g << 8 ) | ( b ) )
const TInt KVgSvgParserColorsMapCount = 149;
const TVgSvgParserColorEntry KVgSvgParserColorsMap[ KVgSvgParserColorsMapCount ] =
{
	{ _L8("aliceblue"), rgb(240, 248, 255) }, 
	{ _L8("aliceblue"), rgb(240, 248, 255) }, 
	{ _L8("aliceblue"), rgb(240, 248, 255) }, 
	{ _L8("antiquewhite"), rgb(250, 235, 215) },
	{ _L8("aqua"), rgb( 0, 255, 255) },
	{ _L8("aquamarine"), rgb(127, 255, 212) },
	{ _L8("azure"), rgb(240, 255, 255) },
	{ _L8("beige"), rgb(245, 245, 220) },
	{ _L8("bisque"), rgb(255, 228, 196) },
	{ _L8("black"), rgb( 0, 0, 0) },
	{ _L8("blanchedalmond"), rgb(255, 235, 205) },
	{ _L8("blue"), rgb( 0, 0, 255) },
	{ _L8("blueviolet"), rgb(138, 43, 226) },
	{ _L8("brown"), rgb(165, 42, 42) },
	{ _L8("burlywood"), rgb(222, 184, 135) },
	{ _L8("cadetblue"), rgb( 95, 158, 160) },
	{ _L8("chartreuse"), rgb(127, 255, 0) },
	{ _L8("chocolate"), rgb(210, 105, 30) },
	{ _L8("coral"), rgb(255, 127, 80) },
	{ _L8("cornflowerblue"), rgb(100, 149, 237) },
	{ _L8("cornsilk"), rgb(255, 248, 220) },
	{ _L8("crimson"), rgb(220, 20, 60) },
	{ _L8("cyan"), rgb( 0, 255, 255) },
	{ _L8("darkblue"), rgb( 0, 0, 139) },
	{ _L8("darkcyan"), rgb( 0, 139, 139) },
	{ _L8("darkgoldenrod"), rgb(184, 134, 11) },
	{ _L8("darkgray"), rgb(169, 169, 169) },
	{ _L8("darkgreen"), rgb( 0, 100, 0) },
	{ _L8("darkgrey"), rgb(169, 169, 169) },
	{ _L8("darkkhaki"), rgb(189, 183, 107) },
	{ _L8("darkmagenta"), rgb(139, 0, 139) },
	{ _L8("darkolivegreen"), rgb( 85, 107, 47) },
	{ _L8("darkorange"), rgb(255, 140, 0) },
	{ _L8("darkorchid"), rgb(153, 50, 204) },
	{ _L8("darkred"), rgb(139, 0, 0) },
	{ _L8("darksalmon"), rgb(233, 150, 122) },
	{ _L8("darkseagreen"), rgb(143, 188, 143) },
	{ _L8("darkslateblue"), rgb( 72, 61, 139) },
	{ _L8("darkslategray"), rgb( 47, 79, 79) },
	{ _L8("darkslategrey"), rgb( 47, 79, 79) },
	{ _L8("darkturquoise"), rgb( 0, 206, 209) },
	{ _L8("darkviolet"), rgb(148, 0, 211) },
	{ _L8("deeppink"), rgb(255, 20, 147) },
	{ _L8("deepskyblue"), rgb( 0, 191, 255) },
	{ _L8("dimgray"), rgb(105, 105, 105) },
	{ _L8("dimgrey"), rgb(105, 105, 105) },
	{ _L8("dodgerblue"), rgb( 30, 144, 255) },
	{ _L8("firebrick"), rgb(178, 34, 34) },
	{ _L8("floralwhite"), rgb(255, 250, 240) },
	{ _L8("forestgreen"), rgb( 34, 139, 34) },
	{ _L8("fuchsia"), rgb(255, 0, 255) },
	{ _L8("gainsboro"), rgb(220, 220, 220) },
	{ _L8("ghostwhite"), rgb(248, 248, 255) },
	{ _L8("gold"), rgb(255, 215, 0) },
	{ _L8("goldenrod"), rgb(218, 165, 32) },
	{ _L8("gray"), rgb(128, 128, 128) },
	{ _L8("grey"), rgb(128, 128, 128) },
	{ _L8("green"), rgb( 0, 128, 0) },
	{ _L8("greenyellow"), rgb(173, 255, 47) },
	{ _L8("honeydew"), rgb(240, 255, 240) },
	{ _L8("hotpink"), rgb(255, 105, 180) },
	{ _L8("indianred"), rgb(205, 92, 92) },
	{ _L8("indigo"), rgb( 75, 0, 130) },
	{ _L8("ivory"), rgb(255, 255, 240) },
	{ _L8("khaki"), rgb(240, 230, 140) },
	{ _L8("lavender"), rgb(230, 230, 250) },
	{ _L8("lavenderblush"), rgb(255, 240, 245) },
	{ _L8("lawngreen"), rgb(124, 252, 0) },
	{ _L8("lemonchiffon"), rgb(255, 250, 205) },
	{ _L8("lightblue"), rgb(173, 216, 230) },
	{ _L8("lightcoral"), rgb(240, 128, 128) },
	{ _L8("lightcyan"), rgb(224, 255, 255) },
	{ _L8("lightgoldenrodyellow"), rgb(250, 250, 210) },
	{ _L8("lightgray"), rgb(211, 211, 211) },
	{ _L8("lightgreen"), rgb(144, 238, 144) },
	{ _L8("lightgrey"), rgb(211, 211, 211) },
	{ _L8("lightpink"), rgb(255, 182, 193) },
	{ _L8("lightsalmon"), rgb(255, 160, 122) },
	{ _L8("lightseagreen"), rgb( 32, 178, 170) },
	{ _L8("lightskyblue"), rgb(135, 206, 250) },
	{ _L8("lightslategray"), rgb(119, 136, 153) },
	{ _L8("lightslategrey"), rgb(119, 136, 153) },
	{ _L8("lightsteelblue"), rgb(176, 196, 222) },
	{ _L8("lightyellow"), rgb(255, 255, 224) },
	{ _L8("lime"), rgb( 0, 255, 0) },
	{ _L8("limegreen"), rgb( 50, 205, 50) },
	{ _L8("linen"), rgb(250, 240, 230) },
	{ _L8("magenta"), rgb(255, 0, 255) },
	{ _L8("maroon"), rgb(128, 0, 0) },
	{ _L8("mediumaquamarine"), rgb(102, 205, 170) },
	{ _L8("mediumblue"), rgb( 0, 0, 205) },
	{ _L8("mediumorchid"), rgb(186, 85, 211) },
	{ _L8("mediumpurple"), rgb(147, 112, 219) },
	{ _L8("mediumseagreen"), rgb( 60, 179, 113) },
	{ _L8("mediumslateblue"), rgb(123, 104, 238) },
	{ _L8("mediumspringgreen"), rgb( 0, 250, 154) },
	{ _L8("mediumturquoise"), rgb( 72, 209, 204) },
	{ _L8("mediumvioletred"), rgb(199, 21, 133) },
	{ _L8("midnightblue"), rgb( 25, 25, 112) },
	{ _L8("mintcream"), rgb(245, 255, 250) },
	{ _L8("mistyrose"), rgb(255, 228, 225) },
	{ _L8("moccasin"), rgb(255, 228, 181) },
	{ _L8("navajowhite"), rgb(255, 222, 173) },
	{ _L8("navy"), rgb( 0, 0, 128) },
	{ _L8("oldlace"), rgb(253, 245, 230) },
	{ _L8("olive"), rgb(128, 128, 0) },
	{ _L8("olivedrab"), rgb(107, 142, 35) },
	{ _L8("orange"), rgb(255, 165, 0) },
	{ _L8("orangered"), rgb(255, 69, 0) },
	{ _L8("orchid"), rgb(218, 112, 214) },
	{ _L8("palegoldenrod"), rgb(238, 232, 170) },
	{ _L8("palegreen"), rgb(152, 251, 152) },
	{ _L8("paleturquoise"), rgb(175, 238, 238) },
	{ _L8("palevioletred"), rgb(219, 112, 147) },
	{ _L8("papayawhip"), rgb(255, 239, 213) },
	{ _L8("peachpuff"), rgb(255, 218, 185) },
	{ _L8("peru"), rgb(205, 133, 63) },
	{ _L8("pink"), rgb(255, 192, 203) },
	{ _L8("plum"), rgb(221, 160, 221) },
	{ _L8("powderblue"), rgb(176, 224, 230) },
	{ _L8("purple"), rgb(128, 0, 128) },
	{ _L8("red"), rgb(255, 0, 0) },
	{ _L8("rosybrown"), rgb(188, 143, 143) },
	{ _L8("royalblue"), rgb( 65, 105, 225) },
	{ _L8("saddlebrown"), rgb(139, 69, 19) },
	{ _L8("salmon"), rgb(250, 128, 114) },
	{ _L8("sandybrown"), rgb(244, 164, 96) },
	{ _L8("seagreen"), rgb( 46, 139, 87) },
	{ _L8("seashell"), rgb(255, 245, 238) },
	{ _L8("sienna"), rgb(160, 82, 45) },
	{ _L8("silver"), rgb(192, 192, 192) },
	{ _L8("skyblue"), rgb(135, 206, 235) },
	{ _L8("slateblue"), rgb(106, 90, 205) },
	{ _L8("slategray"), rgb(112, 128, 144) },
	{ _L8("slategrey"), rgb(112, 128, 144) },
	{ _L8("snow"), rgb(255, 250, 250) },
	{ _L8("springgreen"), rgb( 0, 255, 127) },
	{ _L8("steelblue"), rgb( 70, 130, 180) },
	{ _L8("tan"), rgb(210, 180, 140) },
	{ _L8("teal"), rgb( 0, 128, 128) },
	{ _L8("thistle"), rgb(216, 191, 216) },
	{ _L8("tomato"), rgb(255, 99, 71) },
	{ _L8("turquoise"), rgb( 64, 224, 208) },
	{ _L8("violet"), rgb(238, 130, 238) },
	{ _L8("wheat"), rgb(245, 222, 179) },
	{ _L8("white"), rgb(255, 255, 255) },
	{ _L8("whitesmoke"), rgb(245, 245, 245) },
	{ _L8("yellow"), rgb(255, 255, 0) },
	{ _L8("yellowgreen"), rgb(154, 205, 50) }
};
#undef rgb

#endif // VGSVGPARSERDEF_H
