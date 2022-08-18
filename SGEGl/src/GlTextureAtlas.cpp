/*
 ============================================================================
 Name		: GlTextureAtlas.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : TGlTextureAtlas implementation
 ============================================================================
 */

#include "GlTextureAtlas.h"
#include "GlTexture.h"


TGlTextureAtlas::TGlTextureAtlas( TUint aTextureId, TRect aRect ) : iTextureId( aTextureId ), iTexture( NULL ), iRect( aRect ) 
    {
    }

void TGlTextureAtlas::Use( GLenum aTextureUnitNumber )
    {
    ASSERT( iTexture );
    
    iTexture->Use( iRect, aTextureUnitNumber );
    }

void TGlTextureAtlas::Discard()
    {
    ASSERT( iTexture );
    
    iTexture->Discard();
    }

TUint TGlTextureAtlas::TextureId() const
    {
    return iTextureId;
    }

TGlTexture* TGlTextureAtlas::Texture() const
    {
    return iTexture;
    }

void TGlTextureAtlas::SetTexture( TGlTexture* aTexture )
    {
    iTexture = aTexture;
    }

MGlTexture::TGlTextureType TGlTextureAtlas::TextureType() const
    {
    return MGlTexture::EGlTextureTypeTextureAtlas;
    }

TSize TGlTextureAtlas::Size() const
    {
    return iRect.Size();
    }

