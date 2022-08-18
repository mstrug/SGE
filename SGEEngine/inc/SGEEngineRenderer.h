/*
 ============================================================================
  Name        : SGEEngineRenderer.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#ifndef __SGEENGINERENDERER_H__
#define __SGEENGINERENDERER_H__

// INCLUDES
#include <e32std.h>
#include <e32base.h>

// FORWARD DECLARATIONS

// CLASS DECLARATIONS

/**
 * MSGEEngineRendererObject
 */
class MSGEEngineRendererObject
	{
public:
	};


/**
 * CSGEEngineRenderer
 */
class CSGEEngineRenderer : public CBase
    {
public:
    
    IMPORT_C ~CSGEEngineRenderer();

    IMPORT_C TInt AddObject( TInt aLayer, MSGEEngineRendererObject* aObject );

    IMPORT_C TInt RemoveObject( MSGEEngineRendererObject* aObject );

    IMPORT_C TInt RemoveObject( TInt aLayer, MSGEEngineRendererObject* aObject );

protected:

    IMPORT_C CSGEEngineRenderer();

    IMPORT_C void BaseConstructL( TInt aLayersCount );

protected: // data

    RArray< RPointerArray< MSGEEngineRendererObject > > iLayers;
    
    };


#endif // __SGEENGINERENDERER_H__
