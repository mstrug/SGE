/*
 ============================================================================
 Name		: GlTextureManagerScripter.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : GlTextureManagerScripter declaration
 ============================================================================
 */

#include "GlTextureManagerScripter.h"

/* example script:

[SGEGL_texture_load]
file_name=file
texture_id=1
generate_mipmap=0
gray_as_alpha=0

[SGEGL_texture_as_mipmap_load]
file_name=file
texture_id=1
mipmap_level=1
gray_as_alpa=0

[SGEGL_texture_atlas_add]
texture_atlas_id=2
texture_id=1
rect=1,1,100,100

[SGEGL_textures_loading_start]

*/


const TUint32 KGroup_SGEGL_texture_load = 0x7000;
const TUint32 KGroup_SGEGL_texture_as_mipmap_load = 0x7001;
const TUint32 KGroup_SGEGL_texture_atlas_add = 0x7002;
const TUint32 KGroup_SGEGL_textures_loading_start = 0x7003;

const TUint32 KTag_file_name = 0x0001;
const TUint32 KTag_texture_id = 0x0002;
const TUint32 KTag_generate_mipmap = 0x0003;
const TUint32 KTag_gray_as_alpha = 0x0004;
const TUint32 KTag_mipmap_level = 0x0005;
const TUint32 KTag_texture_atlas_id = 0x0006;
const TUint32 KTag_rect = 0x0007;

#define DEF_G( name ) { _L8( #name ), KGroup_ ## name }
#define DEF_T( name ) { _L8( #name ), KTag_ ## name }
#define DEF_GT_END { KNullDesC8(), 0 }



CGlTextureManagerScripter::CGlTextureManagerScripter( CGlTextureManager& aTextureManager, CSGEUtilsScriptEngine& aScriptEngine ) :
	iTextureManager( aTextureManager ), iScriptEngine( aScriptEngine )
	{
	}

CGlTextureManagerScripter::~CGlTextureManagerScripter()
	{
	iScriptEngine.UnregisterClient( *this );
	iGroupDefinition.Close();
	iTagDefinition.Close();
	iAtlasDefinition.Close();
	}

CGlTextureManagerScripter* CGlTextureManagerScripter::NewL( CGlTextureManager& aTextureManager, CSGEUtilsScriptEngine& aScriptEngine )
	{
	CGlTextureManagerScripter* self = CGlTextureManagerScripter::NewLC( aTextureManager, aScriptEngine );
	CleanupStack::Pop( self );
	return self;
	}

CGlTextureManagerScripter* CGlTextureManagerScripter::NewLC( CGlTextureManager& aTextureManager, CSGEUtilsScriptEngine& aScriptEngine )
	{
	CGlTextureManagerScripter* self = new (ELeave) CGlTextureManagerScripter( aTextureManager, aScriptEngine );
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

void CGlTextureManagerScripter::ConstructL()
	{
	const TSGEUtilsScriptEngineGroupDefinition KGlGroupDefinition[ 5 ] = 
		{
		DEF_G( SGEGL_texture_load ),
		DEF_G( SGEGL_texture_as_mipmap_load ),
		DEF_G( SGEGL_texture_atlas_add ),
		DEF_G( SGEGL_textures_loading_start ),
		DEF_GT_END 
		};

	for ( TInt i = 0; i < 5; i++ )
		{
		iGroupDefinition.AppendL( KGlGroupDefinition[ i ] );
		}

	const TSGEUtilsScriptEngineTagDefinition KGlTagDefinition[ 8 ] = 
		{ 
		DEF_T( file_name ),
		DEF_T( texture_id ),
		DEF_T( generate_mipmap ),
		DEF_T( gray_as_alpha ),
		DEF_T( mipmap_level ),
		DEF_T( texture_atlas_id ),
		DEF_T( rect ),
		DEF_GT_END 
		};

	for ( TInt i = 0; i < 8; i++ )
		{
		iTagDefinition.AppendL( KGlTagDefinition[ i ] );
		}

	User::LeaveIfError( iScriptEngine.RegisterClient( *this, iGroupDefinition, iTagDefinition ) );
	}


void CGlTextureManagerScripter::ScriptEngineGroupDoL( TUint32 aGroupId, RArray< TSGEUtilsScriptEngineTag > aValues )
	{
	switch ( aGroupId )
		{
		case KGroup_SGEGL_texture_load:
				{
				TInt idx = CSGEUtilsScriptEngine::FindTag( aValues, KTag_file_name );
				if ( idx == KErrNotFound )
					{
					User::Leave( KErrArgument );
					}
				RBuf file_name;
				aValues[ idx ].ValueAsRBufLC( file_name );

				idx = CSGEUtilsScriptEngine::FindTag( aValues, KTag_texture_id );
				if ( idx == KErrNotFound )
					{
					User::Leave( KErrArgument );
					}
				TUint texId = aValues[ idx ].ValueAsTUint32();

				TBool genmipmap = EFalse;
				idx = CSGEUtilsScriptEngine::FindTag( aValues, KTag_generate_mipmap );
				if ( idx != KErrNotFound )
					{
					genmipmap = aValues[ idx ].ValueAsTBool();
					}
				
				TBool grayasalpha = EFalse;
				idx = CSGEUtilsScriptEngine::FindTag( aValues, KTag_gray_as_alpha );
				if ( idx != KErrNotFound )
					{
					grayasalpha = aValues[ idx ].ValueAsTBool();
					}
				
				iTextureManager.AddImageToLoadL( file_name, texId, genmipmap, grayasalpha );

				CleanupStack::PopAndDestroy( &file_name );
				}
			break;
		case KGroup_SGEGL_texture_as_mipmap_load:
				{
				TInt idx = CSGEUtilsScriptEngine::FindTag( aValues, KTag_file_name );
				if ( idx == KErrNotFound )
					{
					User::Leave( KErrArgument );
					}
				RBuf file_name;
				aValues[ idx ].ValueAsRBufLC( file_name );

				idx = CSGEUtilsScriptEngine::FindTag( aValues, KTag_texture_id );
				if ( idx == KErrNotFound )
					{
					User::Leave( KErrArgument );
					}
				TUint texId = aValues[ idx ].ValueAsTUint32();

				idx = CSGEUtilsScriptEngine::FindTag( aValues, KTag_mipmap_level );
				if ( idx == KErrNotFound )
					{
					User::Leave( KErrArgument );
					}
				TUint level = aValues[ idx ].ValueAsTUint32();
				
				TBool grayasalpha = EFalse;
				idx = CSGEUtilsScriptEngine::FindTag( aValues, KTag_gray_as_alpha );
				if ( idx != KErrNotFound )
					{
					grayasalpha = aValues[ idx ].ValueAsTBool();
					}
				
				iTextureManager.AddImageToLoadAsMipMapL( file_name, texId, level, grayasalpha );

				CleanupStack::PopAndDestroy( &file_name );
				}
			break;
		case KGroup_SGEGL_texture_atlas_add:
				{
				TInt idx = CSGEUtilsScriptEngine::FindTag( aValues, KTag_texture_id );
				if ( idx == KErrNotFound )
					{
					User::Leave( KErrArgument );
					}
				TUint texId = aValues[ idx ].ValueAsTUint32();

				idx = CSGEUtilsScriptEngine::FindTag( aValues, KTag_texture_atlas_id );
				if ( idx == KErrNotFound )
					{
					User::Leave( KErrArgument );
					}
				TUint atlas = aValues[ idx ].ValueAsTUint32();
				
				idx = CSGEUtilsScriptEngine::FindTag( aValues, KTag_rect );
				if ( idx == KErrNotFound )
					{
					User::Leave( KErrArgument );
					}
				TRect r = aValues[ idx ].ValueAsTRect();

				if ( iTextureManager.Get( texId ) )
					{
					User::LeaveIfError( iTextureManager.AddTextureAtlas( atlas, texId, r ) );
					}
				else
					{				
					SAtlasDefinition def = { atlas, texId, r };

					iAtlasDefinition.AppendL( def );
					}
				}
			break;
		case KGroup_SGEGL_textures_loading_start:
				{
				iTextureManager.StartLoadingImagesL();
				}
			break;
		default:
				User::Leave( KErrNotSupported );
			break;
		}
	}

void CGlTextureManagerScripter::ScriptEnginePrepare()
	{
	iOldObserver = iTextureManager.SetObserver( this );
	}

TInt CGlTextureManagerScripter::ScriptEngineGroup( TUint32 aGroupId, RArray< TSGEUtilsScriptEngineTag > aValues )
	{
	TRAPD( err, ScriptEngineGroupDoL( aGroupId, aValues ) );
	return err;
	}

void CGlTextureManagerScripter::ScriptEngineCleanup()
	{
	}

void CGlTextureManagerScripter::TextureCreated( TUint aTextureId, TInt aError )
	{
	if ( iOldObserver )
	    {
	    iOldObserver->TextureCreated( aTextureId, aError );
	    }
	}

void CGlTextureManagerScripter::TexturesCreatedAll()
	{
	for ( TInt i = 0; i < iAtlasDefinition.Count(); i++ )
	    {
	    TInt err = iTextureManager.AddTextureAtlas( iAtlasDefinition[ i ].iAtlasId, iAtlasDefinition[ i ].iTextureId, iAtlasDefinition[ i ].iRect );
	    if ( iOldObserver )
		{
		iOldObserver->TextureCreated( iAtlasDefinition[ i ].iAtlasId, err );
		}
	    }
	iAtlasDefinition.Reset();

	if ( iOldObserver )
	    {
	    iOldObserver->TexturesCreatedAll();
            iOldObserver = NULL;
	    }

	iTextureManager.SetObserver( iOldObserver );
	}




