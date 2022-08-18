/*
 ============================================================================
 Name		: GlTextureSet.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CGlTextureSet implementation
 ============================================================================
 */

#include "GlTextureSet.h"

CGlTextureSet::CGlTextureSet( TUint aTextureSetId ) : iTextureSetId( aTextureSetId )
    {
    // No implementation required
    }

CGlTextureSet::~CGlTextureSet()
    {
    iItems.Close();
    iAtlasItems.Close();
    }

RHashMap< TUint, TGlTexture >& CGlTextureSet::Items()
    {
    return iItems;
    }

TUint CGlTextureSet::Id() const
    {
    return iTextureSetId;
    }

TGlTexture* CGlTextureSet::Texture( TUint aTextureId )
    {
    return iItems.Find( aTextureId );
    }

TGlTextureAtlas* CGlTextureSet::TextureAtlas( TUint aTextureId )
    {
    TGlTextureAtlas *atlas = iAtlasItems.Find( aTextureId );
    if ( atlas )
        {
        TGlTexture* tex = iItems.Find( atlas->TextureId() );
        if ( tex )
            {
            atlas->SetTexture( tex );
            
            return atlas;
            }
        }
    return NULL;
    }

void CGlTextureSet::RemoveTextureItem( TUint aTextureId )
    {
    TGlTexture *tex = iItems.Find( aTextureId );
    if ( tex )
        {
//        THashMapIter< TUint, TGlTextureAtlas > iter( iAtlasItems );
//        iter.Reset();
//        iter.NextKey();
//
//        while ( iter.CurrentKey() != NULL )
//            {
//            TGlTextureAtlas *atlas = iter.CurrentValue();
//            if ( atlas && atlas->Texture().TextureId() == tex->TextureId() )
//                {
//                iAtlasItems.Remove( *iter.CurrentKey() );
//                }
//            else
//                {
//                iter.NextKey();
//                }
//            }
        
        tex->DeleteTexture();
        }
    iItems.Remove( aTextureId );
    }

void CGlTextureSet::AddTextureAtlasL( TUint aTextureAtlasId , TUint aTextureId, TRect aRect )
    {
    if ( iItems.Find( aTextureAtlasId ) )
        {
        User::Leave( KErrAlreadyExists );
        }
    TGlTexture* texture = iItems.Find( aTextureId );
    if ( !texture )
        {
        User::Leave( KErrNotFound );
        }
    
    TGlTextureAtlas atlas( aTextureId, aRect );
    
    iAtlasItems.InsertL( aTextureAtlasId, atlas );    
    }

TInt CGlTextureSet::AddTextureAtlas( TUint aTextureAtlasId , TUint aTextureId, TRect aRect )
    {
    if ( iItems.Find( aTextureAtlasId ) )
        {
        return KErrAlreadyExists;
        }
    TGlTexture* texture = iItems.Find( aTextureId );
    if ( !texture )
        {
        return KErrNotFound;
        }
    
    TGlTextureAtlas atlas( aTextureId, aRect );
    
    return iAtlasItems.Insert( aTextureAtlasId, atlas );    
    }

