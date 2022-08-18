/*
 ============================================================================
 Name		: GlTextureManager.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CGlTextureManager implementation
 ============================================================================
 */

#include "GlTextureManager.h"
#include "GlTextureFileCustom.h"
#include "GlUtils.h"
#include "GlVertexBuffer.h"
#include <SGEBitmap/SGEBitmapItem.h>
#include <SGEBitmap/SGEBitmapUtils.h>
//#include <gles/glext.h>


CGlTextureManager::CGlTextureManager( MGlTextureManagerObserver *aObserver, CSGEBitmapProvider *aBitmapProvider )
    : iObserver( aObserver ), iBitmapProvider( aBitmapProvider ), iCurrentFileToLoadId( 1 )
    {
    if ( aBitmapProvider )
        {
        iOwnedBitmapProvider = EFalse;
        }
    else
        {
        iOwnedBitmapProvider = ETrue;
        }
    }

CGlTextureManager::~CGlTextureManager()
    {
    if ( iOwnedBitmapProvider )
        {
        delete iBitmapProvider;
        }
    iTextureSets.ResetAndDestroy();
    iImageItemsToLoad.Close();
    delete iTextureDrawVBO;
    }

CGlTextureManager* CGlTextureManager::NewLC( MGlTextureManagerObserver *aObserver, CSGEBitmapProvider *aBitmapProvider )
    {
    CGlTextureManager *self = new (ELeave) CGlTextureManager( aObserver, aBitmapProvider );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

CGlTextureManager* CGlTextureManager::NewL( MGlTextureManagerObserver *aObserver, CSGEBitmapProvider *aBitmapProvider )
    {
    CGlTextureManager *self = CGlTextureManager::NewLC( aObserver, aBitmapProvider );
    CleanupStack::Pop(); // self;
    return self;
    }

void CGlTextureManager::ConstructL()
    {
    if ( !iBitmapProvider )
        {
        iBitmapProvider = CSGEBitmapProvider::NewL( this );
        iOwnedBitmapProvider = ETrue;
        }
    }

MGlTextureManagerObserver* CGlTextureManager::SetObserver( MGlTextureManagerObserver *aObserver )
    {
    MGlTextureManagerObserver* old = iObserver;
    iObserver = aObserver;
    return old;
    }

void CGlTextureManager::CreateTextureSetL( TUint aTextureSetId )
    {
    ASSERT( aTextureSetId > 0 ); // bitmap set ID should be greather than 0
    
    if ( iTextureSets.Find( aTextureSetId ) )
        {
        User::Leave( KErrAlreadyExists );
        }
    
    CGlTextureSet *textureSet = new (ELeave) CGlTextureSet( aTextureSetId );
    CleanupStack::PushL( textureSet );
    TUint *id = new (ELeave) TUint;
    *id = aTextureSetId;
    CleanupStack::PushL( id );
    iTextureSets.InsertL( id, textureSet );
    CleanupStack::Pop( id );
    CleanupStack::Pop( textureSet );
    
    if ( !iCurrentTextureSet )
        {
        iCurrentTextureSet = textureSet;
        }
    }

TBool CGlTextureManager::SelecTextureSet( TUint aTextureSetId )
    {
    CGlTextureSet *set = iTextureSets.Find( aTextureSetId );
    if ( set )
        {
        iCurrentTextureSet = set;
        return ETrue;
        }
    return EFalse;
    }

TUint CGlTextureManager::CurrentTextureSet() const
    {
    return ( iCurrentTextureSet ? iCurrentTextureSet->Id() : 0 );
    }

void CGlTextureManager::RemoveTextureSet( TUint aTextureSetId )
    {
    // todo: check if there is memory leak here
    iTextureSets.Remove( &aTextureSetId );
    }

void CGlTextureManager::RemoveTextureSetAll()
    {
    iTextureSets.ResetAndDestroy();
    }

void CGlTextureManager::CheckCurrentTextureSetAndCreateL()
    {
    if ( !iCurrentTextureSet && iTextureSets.Count() > 0 )
        {
        User::Leave( KErrNotReady );
        }
    else if ( !iCurrentTextureSet && iTextureSets.Count() == 0 )
        {
        const TUint KAutoTextureSetId = 1;
        CreateTextureSetL( KAutoTextureSetId );
        }
    }

void CGlTextureManager::AddTextureL( TUint aTextureId, TGlTexture &aTexture )
    {
    CheckCurrentTextureSetAndCreateL();
    
    if ( iCurrentTextureSet->Texture( aTextureId ) )
        {
        User::Leave( KErrAlreadyExists );
        }
    iCurrentTextureSet->Items().InsertL( aTextureId, aTexture );
    }

TInt CGlTextureManager::AddTextureAtlas( TUint aTextureAtlasId, TUint aTextureId, TRect aRect )
    {
    if ( !iCurrentTextureSet )
        {
        return KErrNotReady;
        }
        
    return iCurrentTextureSet->AddTextureAtlas( aTextureAtlasId, aTextureId, aRect );
    }

MGlTexture* CGlTextureManager::Get( TUint aTextureId )
    {
    ASSERT( iCurrentTextureSet );
    
    TGlTexture* tex = iCurrentTextureSet->Texture( aTextureId );
    if ( !tex )
        {
        TGlTextureAtlas* atlas = iCurrentTextureSet->TextureAtlas( aTextureId );
        return atlas;
        }
    return tex;
    }

TGlTexture* CGlTextureManager::Texture( TUint aTextureId )
    {
    ASSERT( iCurrentTextureSet );
    
    return iCurrentTextureSet->Texture( aTextureId );
    }

TGlTextureAtlas* CGlTextureManager::TextureAtlas( TUint aTextureId )
    {
    ASSERT( iCurrentTextureSet );
    
    return iCurrentTextureSet->TextureAtlas( aTextureId );
    }

void CGlTextureManager::AddImageToLoadL( const TDesC& aFileName, TUint aTextureId, TBool aGenerateMipMaps, TBool aGrayAsAlpha )
    {
    CheckCurrentTextureSetAndCreateL();
    
    if ( !iBitmapProvider )
        {
        iBitmapProvider = CSGEBitmapProvider::NewL( this );
        iOwnedBitmapProvider = ETrue;
        }
    
    SImageItem item = { iCurrentFileToLoadId, aTextureId, aGenerateMipMaps, EFalse, aGrayAsAlpha };
    iImageItemsToLoad.AppendL( item );
    
    iBitmapProvider->AddImageToLoadL( aFileName, iCurrentFileToLoadId );
    
    iCurrentFileToLoadId++;
    }

void CGlTextureManager::AddImageToLoadAsMipMapL( const TDesC& aFileName, TUint aMipMapTextureId, TInt aMipMapLevel, TBool aGrayAsAlpha )
	{
    CheckCurrentTextureSetAndCreateL();
    
    if ( !iBitmapProvider )
        {
        iBitmapProvider = CSGEBitmapProvider::NewL( this );
        iOwnedBitmapProvider = ETrue;
        }
    
    SImageItem item = { iCurrentFileToLoadId, aMipMapTextureId, EFalse /* N/A */, ETrue, aGrayAsAlpha, aMipMapLevel };
    iImageItemsToLoad.AppendL( item );

    iBitmapProvider->AddImageToLoadL( aFileName, iCurrentFileToLoadId );
    
    iCurrentFileToLoadId++;
	}
	
void CGlTextureManager::AddImageToLoadL( CSGEFile &aSGEFile, const TDesC& aFileName, TUint aTextureId, TBool aGenerateMipMaps, TBool aGrayAsAlpha )
    {
    CheckCurrentTextureSetAndCreateL();
    
    if ( !iBitmapProvider )
        {
        iBitmapProvider = CSGEBitmapProvider::NewL( this );
        iOwnedBitmapProvider = ETrue;
        }
    
    SImageItem item = { iCurrentFileToLoadId, aTextureId, aGenerateMipMaps, EFalse, aGrayAsAlpha };
    iImageItemsToLoad.AppendL( item );
    
    iBitmapProvider->AddImageToLoadL( aSGEFile, aFileName, iCurrentFileToLoadId );
    
    iCurrentFileToLoadId++;
    }

void CGlTextureManager::AddImageToLoadAsMipMapL( CSGEFile &aSGEFile, const TDesC& aFileName, TUint aMipMapTextureId, TInt aMipMapLevel, TBool aGrayAsAlpha )
    {
    CheckCurrentTextureSetAndCreateL();
    
    if ( !iBitmapProvider )
        {
        iBitmapProvider = CSGEBitmapProvider::NewL( this );
        iOwnedBitmapProvider = ETrue;
        }
    
    SImageItem item = { iCurrentFileToLoadId, aMipMapTextureId, EFalse /* N/A */, ETrue, aGrayAsAlpha, aMipMapLevel };
    iImageItemsToLoad.AppendL( item );

    iBitmapProvider->AddImageToLoadL( aSGEFile, aFileName, iCurrentFileToLoadId );
    
    iCurrentFileToLoadId++;
    }

void CGlTextureManager::AddImageToLoadL( CSGEBitmapItemCreator *aBitmapItemCreator )
    {
    User::Leave( KErrNotSupported ); // not supported due to usage of iImageItemsToLoad array
    
    CheckCurrentTextureSetAndCreateL();
    
    if ( !iBitmapProvider )
        {
        iBitmapProvider = CSGEBitmapProvider::NewL( this );
        iOwnedBitmapProvider = ETrue;
        }
    
    iBitmapProvider->AddImageToLoadL( aBitmapItemCreator );
    }

void CGlTextureManager::StartLoadingImagesL()
    {
    ASSERT( iBitmapProvider );
    
    iBitmapProvider->SetObserver( this );
    iBitmapProvider->StartLoadingImagesL();
    }

void CGlTextureManager::DeleteBitmapProvider()
    {
    if ( iOwnedBitmapProvider )
        {
        delete iBitmapProvider;
        iBitmapProvider = NULL;
        }
    }

TInt CGlTextureManager::CreateTextureDrawVBO()
    {
    if ( iTextureDrawVBO )
	{
	return KErrAlreadyExists;
	}

    const TInt KVertexSize = 2;
    const TInt KVertexCount = 4;
    const TInt KTexCoordSize = 2;
    static const GLshort KVertices[ KVertexCount * KVertexSize ] =
         {
          0,  0,
          1,  0,
          1,  1,
          0,  1,
         };
    static const GLshort KTexCoords[ KVertexCount * KTexCoordSize ] =
         {
          0,  1,
          1,  1,
          1,  0,
          0,  0,
         };

    TGlVertexArrayConfig cfg( EVertexTypeShort, ENormalTypeNone, EColorTypeNone, EPointSizeTypeNone, ETexCoordTypeShort,
                              EVertexSizeTwo, ENormalSizeNone, EColorSizeNone, EPointSizeNone, ETexCoordSizeTwo, 1 );

    TRAPD( err, iTextureDrawVBO = CGlVertexBuffer::NewL( cfg, KVertexCount ) );
    if ( err )
	{
	return err;
	}

    iTextureDrawVBO->SetVertexData( (TAny*) KVertices, KVertexCount * KVertexSize * sizeof( GLshort ) );
    iTextureDrawVBO->SetTexCoordData( 0, (TAny*) KTexCoords, KVertexCount * KTexCoordSize * sizeof( GLshort ) );
    return iTextureDrawVBO->CreateVertexBuffer();
    }

TInt CGlTextureManager::DrawTexture2D( TUint aTextureId, TBool aScale, TBool aDiscard, GLenum aTextureUnitNumber )
    {
    if ( !iTextureDrawVBO )
	{
	return KErrNotReady;
	}
    MGlTexture* tex = CGlTextureManager::Get( aTextureId );
    if ( tex )
	{
	tex->Use( aTextureUnitNumber );
	if ( aScale )
	    {
	    glPushMatrix();
	    GlUtils::Scale2D( tex->Size() );
	    }
	iTextureDrawVBO->Draw( GL_TRIANGLE_FAN );
	if ( aScale )
	    {
	    glPopMatrix();
	    }
	if ( aDiscard )
	    {
	    tex->Discard();
	    }
	return KErrNone;
	}
    return KErrNotFound;
    }

void CGlTextureManager::BitmapLoaded( TUint aBitmapId, TInt aError )
    {
    TInt idx = KErrNotFound;

    for ( TInt i = 0; i < iImageItemsToLoad.Count(); i++ )
        {
        if ( iImageItemsToLoad[ i ].iFileId == aBitmapId )
            {
            idx = i;
            break;
            }
        }
    if ( idx == KErrNotFound && aError == KErrNone )
        {
        aError = KErrNotFound;
        }

    if ( aError == KErrNotSupported && idx == KErrNotFound )
	{ // opened by custom texture file - just return
	return;
	}

    if ( aError == KErrNone )
        {
        TGlTexture texture;
	texture.SetGrayAsAlpha( iImageItemsToLoad[ idx ].iGrayAsAlpha );
        CSGEBitmapItem *bi = iBitmapProvider->BitmapItem( aBitmapId );
        if ( bi && bi->Bitmap() && bi->BitmapMask() )
            {
            CFbsBitmap *bmp = NULL;
            TRAP( aError, bmp = SGEBitmapUtils::CombineBitmapsL( *bi->Bitmap(), *bi->BitmapMask() ) );
            if ( aError == KErrNone )
                {
                if ( bmp )
                    {
                    if ( !iImageItemsToLoad[ idx ].iMipMap )
                        {
                        texture.iGenerateMipmap = ( iImageItemsToLoad[ idx ].iGenerateMipMap ? GL_TRUE : GL_FALSE );
                        
                        aError = texture.CreateTexture( bmp );
                        }
                    else
                        {
                        TGlTexture* tex = Texture( iImageItemsToLoad[ idx ].iTextureId );
                        if ( !tex )
                            {
                            aError = KErrNotFound;
                            }
                        else
                            {
                            aError = tex->CreateMipmap( bmp, iImageItemsToLoad[ idx ].iMipMapLevel );
                            }
                        }
                    }
                else
                    {
                    aError = KErrGeneral;
                    }
                }        
            delete bmp;
            }
        else if ( bi && bi->Bitmap() )
            {
            if ( !iImageItemsToLoad[ idx ].iMipMap )
                {
                texture.iGenerateMipmap = ( iImageItemsToLoad[ idx ].iGenerateMipMap ? GL_TRUE : GL_FALSE );

                aError = texture.CreateTexture( iBitmapProvider->BitmapItem( aBitmapId )->Bitmap() );
                }
            else
                {
                TGlTexture* tex = Texture( iImageItemsToLoad[ idx ].iTextureId );
                if ( !tex )
                    {
                    aError = KErrNotFound;
                    }
                else
                    {
                    aError = tex->CreateMipmap( iBitmapProvider->BitmapItem( aBitmapId )->Bitmap(), iImageItemsToLoad[ idx ].iMipMapLevel );
                    }
                }
            }
        else
            {
            aError = KErrGeneral;
            }
            
        if ( aError == KErrNone && !iImageItemsToLoad[ idx ].iMipMap )
            {
            aError = iCurrentTextureSet->Items().Insert( iImageItemsToLoad[ idx ].iTextureId, texture );
            if ( aError != KErrNone )
                {
                texture.DeleteTexture();
                }
            }
        
        aBitmapId = iImageItemsToLoad[ idx ].iTextureId;
        iImageItemsToLoad.Remove( idx );
        }

    if ( iObserver )
        {
        iObserver->TextureCreated( aBitmapId, aError );
        }
    }

void CGlTextureManager::BitmapsLoadedAll()
    {
    iBitmapProvider->RemoveBitmapSetAll(); // bitmaps are not used because textures are created
    
    if ( iObserver )
        {
        iObserver->TexturesCreatedAll();
        }
    }

TBool CGlTextureManager::BitmapLoadCustom( TUint aBitmapId, TFileName aFileName, RBuf8& aData )
    { // check if the file is a KTX of PVR texture
    TBool closeBuf = EFalse;
    TBool ret = EFalse;

    TInt idx = KErrNotFound;
    for ( TInt i = 0; i < iImageItemsToLoad.Count(); i++ )
        {
        if ( iImageItemsToLoad[ i ].iFileId == aBitmapId )
            {
            idx = i;
            break;
            }
        }
    if ( idx == KErrNotFound )
        {
        return EFalse;
        }

    if ( aData.Length() == 0 )
        { // use aFileName
        RFs fs;
        fs.Connect();
        RFile file;
        if ( file.Open( fs, aFileName, EFileRead ) != KErrNone )
            {
            fs.Close();
            return EFalse;
            }
        TInt size;
        file.Size( size );

        aData.Close();
        aData.Create( size );
        closeBuf = ETrue;

        file.Read( aData );

        file.Close();
        fs.Close();
        }

    if ( aData.Length() > 0 )
        { // use buffer
        GlTextureFileCustom::STextureCustom texData = { 0, 0, 0, 0, NULL };

        if ( GlTextureFileCustom::LoadKTX( aData, texData ) )
            {
#define GL_ETC1_RGB8_OES                                        0x8D64
            if ( texData.iFormat == GL_ETC1_RGB8_OES )
                {
#ifndef _DEBUG // only on HW builds
                TPtrC8 extptr( GlUtils::GetExtensions() );

                _LIT8( KString_GL_OES_compressed_ETC1_RGB8_texture, "GL_OES_compressed_ETC1_RGB8_texture" );

                if ( extptr.Find( KString_GL_OES_compressed_ETC1_RGB8_texture ) == KErrNotFound )
                    { // ETC1 compression is not supported
                    texData.iData = NULL;
                    }
#endif
                }
            }
        else if ( GlTextureFileCustom::LoadPVR( aData, texData ) )
            {

            }

        if ( texData.iData )
            {
            TInt error = KErrNone;
            TInt tId = iImageItemsToLoad[ idx ].iTextureId;
            TGlTexture texture;
            texture.SetGrayAsAlpha( iImageItemsToLoad[ idx ].iGrayAsAlpha );

            if ( !iImageItemsToLoad[ idx ].iMipMap )
                {
                texture.iGenerateMipmap = ( iImageItemsToLoad[ idx ].iGenerateMipMap ? GL_TRUE : GL_FALSE );

                if ( texData.iFormat == GL_ETC1_RGB8_OES )
                    {
                    error = texture.CreateCompressedTexture( texData.iWidth, texData.iHeight, texData.iFormat, texData.iDataSize, texData.iData );
                    }
                else
                    {
                    TDisplayMode mode = ENone;
                    if ( texData.iFormat == GL_ALPHA || texData.iFormat == GL_LUMINANCE )
                        {
                        mode = EGray256;
                        }
                    else if ( texData.iFormat == GL_LUMINANCE_ALPHA )
                        {
                        mode = EColor64K;
                        }
                    else if ( texData.iFormat == GL_RGB )
                        {
                        mode = EColor16M;
                        }
                    else if ( texData.iFormat == GL_RGBA )
                        {
                        mode = EColor16MA;
                        }

                    error = texture.CreateTexture( TSize( texData.iWidth, texData.iHeight ), mode, texData.iData );
                    }
                }
            else
                {
                error = KErrNotSupported;
                /*TGlTexture* tex = Texture( iImageItemsToLoad[ idx ].iTextureId );
                if ( !tex )
                    {
                    error = KErrNotFound;
                    }
                else
                    {
                    error = tex->CreateMipmap( iBitmapProvider->BitmapItem( aBitmapId )->Bitmap(), iImageItemsToLoad[ idx ].iMipMapLevel );
                    }*/
                }

            if ( error == KErrNone && !iImageItemsToLoad[ idx ].iMipMap )
                {
                error = iCurrentTextureSet->Items().Insert( iImageItemsToLoad[ idx ].iTextureId, texture );

                if ( error != KErrNone )
                    {
                    texture.DeleteTexture();
                    }
                }

            iImageItemsToLoad.Remove( idx );
            ret = ETrue;
            if ( iObserver )
                {
                iObserver->TextureCreated( tId, error );
                }
            }
        }

    if ( closeBuf )
        {
        aData.Close();
        }
    
    return ret;
    }


// End of file
