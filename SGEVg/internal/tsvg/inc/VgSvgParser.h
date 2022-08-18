/*
 ============================================================================
 Name		: VgSvgParser.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CVgSvgParser declaration
 ============================================================================
 */

#ifndef VGSVGPARSER_H
#define VGSVGPARSER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <w32std.h>
#include <xml/parser.h>
#include <xml/contenthandler.h>
//#include <xml/XmlFrameworkConstants.h>

// CLASS DECLARATION
using namespace Xml;
class CVgSvgImage;
/**
 *  CVgSvgParser
 * 
 */
class CVgSvgParser : public CBase, private MContentHandler
	{
	
	typedef TFixedArray< TReal32, 9 > TTransformation;

	typedef struct
		{
		TBool iSpecified;
		TRgb iColor;
		TReal32 iAlpha;
		} TFill;
	
	typedef struct
		{
		TBool iSpecified;
		TRgb iColor;
		TReal32 iAlpha;
		TReal32 iWidth;
		} TStroke;
		
	typedef struct
		{
		TTransformation iTransformation;
		TFill iFill;
		TStroke iStroke;
		} TStackItem;
		
public:

	~CVgSvgParser();

	static CVgSvgParser* NewL( CVgSvgImage& aImage );

	static CVgSvgParser* NewLC( CVgSvgImage& aImage );
	
	void ParseL( const TDesC8& aData );

private:

	CVgSvgParser( CVgSvgImage& aImage );

	void ConstructL();
    
	TReal UnitCoef( const TDesC8& aUnit );
    void ExtractPair( TLex8& aLex, TReal32& aVal1, TReal32& aVal2 );
    void ExtractThree( TLex8& aLex, TReal32& aVal1, TReal32& aVal2, TReal32& aVal3 );

    void ParseTransformElement( const TDesC8& aValue );
    void ParseFillElement(const RAttributeArray& aAttributes);
    void ParseStrokeElement(const RAttributeArray& aAttributes);
    void ParseSvgElement(const RAttributeArray& aAttributes);
    void ParsePathElement(const RAttributeArray& aAttributes);
    void ParseRectElement(const RAttributeArray& aAttributes);
    void ParseGroupElement(const RAttributeArray& aAttributes);
    
    void StackPushCopy();
    void StackPop();
    TStackItem& StackCurrentItem();
	
private: // MContentHandler
	
	void OnStartDocumentL(const RDocumentParameters& aDocParam, TInt aErrorCode);
	void OnEndDocumentL(TInt aErrorCode);
	void OnStartElementL(const RTagInfo& aElement, const RAttributeArray& aAttributes,TInt aErrorCode);
	void OnEndElementL(const RTagInfo& aElement, TInt aErrorCode);
	void OnContentL(const TDesC8& aBytes, TInt aErrorCode);
	void OnStartPrefixMappingL(const RString& aPrefix, const RString& aUri, TInt aErrorCode);
	void OnEndPrefixMappingL(const RString& aPrefix, TInt aErrorCode);
	void OnIgnorableWhiteSpaceL(const TDesC8& aBytes, TInt aErrorCode);
	void OnSkippedEntityL(const RString& aName, TInt aErrorCode);
	void OnProcessingInstructionL(const TDesC8& aTarget, const TDesC8& aData, TInt aErrorCode);
	void OnError(TInt aErrorCode);
	TAny* GetExtendedInterface(const TInt32 aUid);

private:

	CVgSvgImage& iImage;
	
	CParser* iParser;
	
	TReal iDpi;
    
    TReal iCnvMulX;
    TReal iCnvMulY;
    
    TReal iCnvDivX;
    TReal iCnvDivY;
			
	RArray< TStackItem > iStack;
	
	};

#endif // VGSVGPARSER_H
