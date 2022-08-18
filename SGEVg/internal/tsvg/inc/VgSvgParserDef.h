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
(const void*)&KVgSvgParserStr_39
};
const TStringTable KVgSvgParserStringTable = { 39, KVgSvgParserStringPointers, ETrue };

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
    ESvgParserString_viewBox	
	};





const TInt KVgSvgParserPathCommandsMap[][ 2 ] =
{	/* VG path command, number of parameters */
		{ VG_SCCWARC_TO | VG_SCWARC_TO | VG_LCCWARC_TO | VG_LCWARC_TO, 0 },		// a	->	needs additional parsing!
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




#endif // VGSVGPARSERDEF_H
