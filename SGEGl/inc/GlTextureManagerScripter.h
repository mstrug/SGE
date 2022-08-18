/*
 ============================================================================
 Name		: GlTextureManagerScripter.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : GlTextureManagerScripter declaration
 ============================================================================
 */

#ifndef GLTEXTUREMANAGERSCRIPTER_H
#define GLTEXTUREMANAGERSCRIPTER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <SGEGl/GlTextureManager.h>
#include <SGEUtils/SGEUtilsScriptEngine.h>


// FORWARD DECLARATIONS


/**
 *  CGlTextureManagerScripter
 * 
 */
class CGlTextureManagerScripter : public CBase, public MSGEUtilsScriptEngineClient, MGlTextureManagerObserver
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CGlTextureManagerScripter();

    /**
     * Two-phased constructor.
     */
    static CGlTextureManagerScripter* NewL( CGlTextureManager& aTextureManager, CSGEUtilsScriptEngine& aScriptEngine );

    /**
     * Two-phased constructor.
     */
    static CGlTextureManagerScripter* NewLC( CGlTextureManager& aTextureManager, CSGEUtilsScriptEngine& aScriptEngine );

private:

    /**
     * Constructor for performing 1st stage construction
     */
    CGlTextureManagerScripter( CGlTextureManager& aTextureManager, CSGEUtilsScriptEngine& aScriptEngine );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();

    void ScriptEngineGroupDoL( TUint32 aGroupId, RArray< TSGEUtilsScriptEngineTag > aValue );

public: // from MSGEUtilsScriptEngineClient

    void ScriptEnginePrepare();

    TInt ScriptEngineGroup( TUint32 aGroupId, RArray< TSGEUtilsScriptEngineTag > aValue );

    void ScriptEngineCleanup();

private: // from MGlTextureManagerObserver

    void TextureCreated( TUint aTextureId, TInt aError );

    void TexturesCreatedAll();

private:
    
    CGlTextureManager& iTextureManager;

    CSGEUtilsScriptEngine& iScriptEngine;

    RArray< TSGEUtilsScriptEngineGroupDefinition > iGroupDefinition;

    RArray< TSGEUtilsScriptEngineTagDefinition > iTagDefinition;

    struct SAtlasDefinition
	{
	TUint32 iAtlasId;
	TUint32 iTextureId;
	TRect iRect;
	};

    RArray< SAtlasDefinition > iAtlasDefinition;

    MGlTextureManagerObserver* iOldObserver;
    
    };

#endif // GLTEXTUREMANAGERSCRIPTER_H
