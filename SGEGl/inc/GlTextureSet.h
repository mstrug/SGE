/*
 ============================================================================
 Name		: GlTextureSet.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CGlTextureSet declaration
 ============================================================================
 */

#ifndef GLTEXTURESET_H
#define GLTEXTURESET_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <e32hashtab.h>
#include <SGEGl/GlTexture.h>
#include <SGEGl/GlTextureAtlas.h>

// CLASS DECLARATION

/**
 *  CGlTextureSet
 * 
 */
class CGlTextureSet : public CBase
    {
public:

    /**
     * Constructor for performing 1st stage construction
     */
    CGlTextureSet( TUint aTextureSetId );

    /**
     * Destructor.
     */
    ~CGlTextureSet();

    RHashMap< TUint, TGlTexture >& Items();

    TUint Id() const;
    
    TGlTexture* Texture( TUint aTextureId );

    TGlTextureAtlas* TextureAtlas( TUint aTextureId );
    
    void RemoveTextureItem( TUint aTextureId );

    void AddTextureAtlasL( TUint aTextureAtlasId , TUint aTextureId, TRect aRect );

    TInt AddTextureAtlas( TUint aTextureAtlasId , TUint aTextureId, TRect aRect );
    
private: // data
    
    RHashMap< TUint, TGlTexture > iItems;

    TUint iTextureSetId;
    
    RHashMap< TUint, TGlTextureAtlas > iAtlasItems;
    
    };

#endif // GLTEXTURESET_H
