/*
 ============================================================================
  Name        : SGEEngineRenderer.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#include "SGEEngineRenderer.h"



EXPORT_C CSGEEngineRenderer::CSGEEngineRenderer()
    {
    }

EXPORT_C CSGEEngineRenderer::~CSGEEngineRenderer()
    {
    for ( TInt i = 0; i < iLayers.Count(); i++ )
	{
	iLayers[ i ].Close();
	}
    iLayers.Close();
    }

EXPORT_C void CSGEEngineRenderer::BaseConstructL( TInt aLayersCount )
    {
    for ( TInt i = 0; i < aLayersCount; i++ )
	{
	iLayers.AppendL( RPointerArray< MSGEEngineRendererObject >() );
	}
    }

EXPORT_C TInt CSGEEngineRenderer::AddObject( TInt aLayer, MSGEEngineRendererObject* aObject )
    {
    if ( aLayer >= iLayers.Count() || aLayer < 0 )
	{
	return KErrArgument;
	}

    return iLayers[ aLayer ].Append( aObject );
    }

EXPORT_C TInt CSGEEngineRenderer::RemoveObject( MSGEEngineRendererObject* aObject )
    {
    for ( TInt i = 0; i < iLayers.Count(); i++ )
	{
	for ( TInt j = 0; j < iLayers[ i ].Count(); j++ )
	    {
	    if ( iLayers[ i ][ j ] == aObject )
		{
		iLayers[ i ].Remove( j );
		j--;
		}
	    }
	}
    return KErrNone;
    }

EXPORT_C TInt CSGEEngineRenderer::RemoveObject( TInt aLayer, MSGEEngineRendererObject* aObject )
    {
    if ( aLayer >= iLayers.Count() || aLayer < 0 )
	{
	return KErrArgument;
	}

    for ( TInt i = 0; i < iLayers[ aLayer ].Count(); i++ )
	{
	if ( iLayers[ aLayer ][ i ] == aObject )
	    {
	    iLayers[ aLayer ].Remove( i );
	    i--;
	    }
	}

    return KErrNone;
    }





// End of file
