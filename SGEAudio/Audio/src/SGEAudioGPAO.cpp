/*
 ============================================================================
 Name		: SGEAudioGPAO.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEAudioGPAO implementation
 ============================================================================
 */

#include "SGEAudioGPAO.h"

CSGEAudioGPAO::CSGEAudioGPAO( MSGEAudioGPAOObserver &aObserver, TInt aPriority )
    : CActive( aPriority ), iObserver( aObserver )
    {
    CActiveScheduler::Add( this );
    }

CSGEAudioGPAO::~CSGEAudioGPAO()
    {
    Cancel();
    }

void CSGEAudioGPAO::SetActive()
    {
    return CActive::SetActive();
    }

void CSGEAudioGPAO::DoCancel()
    {
    iObserver.GPAONotifierCancel( this );
    }

void CSGEAudioGPAO::RunL()
    {
    iObserver.GPAONotifierL( this, iStatus.Int() );
    }

TInt CSGEAudioGPAO::RunError( TInt aError )
    {
    return iObserver.GPAONotifierError( this, aError );
    }
