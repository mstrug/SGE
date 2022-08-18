/*
 ============================================================================
 Name		: GlTextureManager.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CGlTextureManager declaration
 ============================================================================
 */

#ifndef GLTEXTUREMANAGER_H
#define GLTEXTUREMANAGER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <e32hashtab.h>
#include <SGEGl/GlTextureSet.h>
#include <SGEBitmap/SGEBitmapProvider.h>

// FORWARD DECLARATIONS
class CSGEFile;
class CSGEBitmapItemCreator;
class CGlVertexBuffer;

// CLASS DECLARATION

/**
 *  MSGEBitmapProviderObserver
 * 
 */
class MGlTextureManagerObserver
    {
public:
    
    virtual void TextureCreated( TUint aTextureId, TInt aError ) = 0;

    virtual void TexturesCreatedAll() = 0;
    
    };

/**
 *  CGlTextureManager
 * 
 */
class CGlTextureManager : public CBase, public MSGEBitmapProviderObserver
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CGlTextureManager();

    /**
     * Two-phased constructor.
     */
    static CGlTextureManager* NewL( MGlTextureManagerObserver *aObserver, CSGEBitmapProvider *aBitmapProvider = NULL );

    /**
     * Two-phased constructor.
     */
    static CGlTextureManager* NewLC( MGlTextureManagerObserver *aObserver, CSGEBitmapProvider *aBitmapProvider = NULL );

    // returns old observer or NULL if none
    MGlTextureManagerObserver* SetObserver( MGlTextureManagerObserver *aObserver );

    void CreateTextureSetL( TUint aTextureSetId );

    // return EFalse when set with specified ID wasn't found
    TBool SelecTextureSet( TUint aTextureSetId );
    
    TUint CurrentTextureSet() const;

    void RemoveTextureSet( TUint aTextureSetId );

    void RemoveTextureSetAll();

    void AddTextureL( TUint aTextureId, TGlTexture &aTexture );

    TInt AddTextureAtlas( TUint aTextureAtlasId, TUint aTextureId, TRect aRect );

    MGlTexture* Get( TUint aTextureId );

    TGlTexture* Texture( TUint aTextureId );
    
    TGlTextureAtlas* TextureAtlas( TUint aTextureId );
    
    void AddImageToLoadL( const TDesC& aFileName, TUint aTextureId, TBool aGenerateMipMaps = EFalse, TBool aGrayAsAlpha = EFalse );

    void AddImageToLoadAsMipMapL( const TDesC& aFileName, TUint aMipMapTextureId, TInt aMipMapLevel, TBool aGrayAsAlpha = EFalse );

    void AddImageToLoadL( CSGEFile &aSGEFile, const TDesC& aFileName, TUint aTextureId, TBool aGenerateMipMaps = EFalse, TBool aGrayAsAlpha = EFalse );

    void AddImageToLoadAsMipMapL( CSGEFile &aSGEFile, const TDesC& aFileName, TUint aMipMapTextureId, TInt aMipMapLevel, TBool aGrayAsAlpha = EFalse );

    void AddImageToLoadL( CSGEBitmapItemCreator *aBitmapItemCreator );

    void StartLoadingImagesL();

    void DeleteBitmapProvider();

    // creates VBO for optimised 2D texture drawing
    TInt CreateTextureDrawVBO();    

    // optimised drawing of texture using VBO
    TInt DrawTexture2D( TUint aTextureId, TBool aScale = ETrue, TBool aDiscard = ETrue, GLenum aTextureUnitNumber = GL_TEXTURE0 );

private:

    /**
     * Constructor for performing 1st stage construction
     */
    CGlTextureManager( MGlTextureManagerObserver *aObserver, CSGEBitmapProvider *aBitmapProvider );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();
    
    void CheckCurrentTextureSetAndCreateL();

private: // from MSGEBitmapProviderObserver
    
    void BitmapLoaded( TUint aBitmapId, TInt aError );

    void BitmapsLoadedAll();

    TBool BitmapLoadCustom( TUint aBitmapId, TFileName aFileName, RBuf8& aData );

private:
    
    MGlTextureManagerObserver *iObserver;
    
    CSGEBitmapProvider *iBitmapProvider; // owned

    CGlVertexBuffer *iTextureDrawVBO; // owned

    TBool iOwnedBitmapProvider;
    
    RPtrHashMap< TUint, CGlTextureSet > iTextureSets;
    
    CGlTextureSet *iCurrentTextureSet; // not owned

    struct SImageItem
        {
        TUint iFileId;
        TUint iTextureId;
        TBool iGenerateMipMap;
        TBool iMipMap;
        TBool iGrayAsAlpha;
        TInt iMipMapLevel;
        };
    
    RArray< SImageItem > iImageItemsToLoad;

    TInt iCurrentFileToLoadId;
    
    };

#endif // GLTEXTUREMANAGER_H
