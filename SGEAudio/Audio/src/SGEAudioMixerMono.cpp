/*
 ============================================================================
 Name		: SGEAudioMixerMono.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEAudioMixerMono implementation
 ============================================================================
 */

#include "SGEAudioMixerMono.h"

CSGEAudioMixerMono::CSGEAudioMixerMono( TSGEAudioConfig &aConfig, 
                                        RPointerArray< CSGEAudioSoundTrack > &aSoundTracks )
    : CSGEAudioMixerBase( aConfig, aSoundTracks )
    {
    // No implementation required
    }

CSGEAudioMixerMono::~CSGEAudioMixerMono()
    {
    }

CSGEAudioMixerMono* CSGEAudioMixerMono::NewLC( TSGEAudioConfig &aConfig, 
                                               RPointerArray< CSGEAudioSoundTrack > &aSoundTracks )
    {
    CSGEAudioMixerMono *self = new (ELeave) CSGEAudioMixerMono( aConfig, aSoundTracks );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

CSGEAudioMixerMono* CSGEAudioMixerMono::NewL( TSGEAudioConfig &aConfig, 
                                              RPointerArray< CSGEAudioSoundTrack > &aSoundTracks )
    {
    CSGEAudioMixerMono *self = CSGEAudioMixerMono::NewLC( aConfig, aSoundTracks );
    CleanupStack::Pop(); // self;
    return self;
    }

void CSGEAudioMixerMono::ConstructL()
    {
    BaseConstructL();
    }

void CSGEAudioMixerMono::MixTracks()
    {
    Mem::FillZ( iBufferMixed, iBufferMixedSize );

    ( iCurrentOutputBuffer == 0 ? iCurrentOutputBuffer = 1 : iCurrentOutputBuffer = 0 );
    
    TInt dataBufferSize = iConfig.SizeOfBufferDataType();
    
    for ( TInt i = 0; i < iSoundTracks.Count(); i++ )
        {
        if ( iSoundTracks[ i ]->AudioSample() && !iSoundTracks[ i ]->IsPaused() )
            {
            TInt dataLen = 0;
            TInt vol = iSoundTracks[ i ]->Volume();
            
            TAny* data = iSoundTracks[ i ]->GetCurrentDataAndAdvance( iConfig.iBufferSize, dataLen );
            
            if ( iSoundTracks[ i ]->IsMuted() )
                {
                continue;
                }
            else if ( vol == 0 )
                {
                if ( dataBufferSize == 2 )
                    {
                    for ( TInt j = 0; j < dataLen; j++ )
                        {
                        iBufferMixed[ j ] += ((TInt16*) data)[ j ];
                        }
                    }
                else if ( dataBufferSize == 1 )
                    {
                    for ( TInt j = 0; j < dataLen; j++ )
                        {
                        iBufferMixed[ j ] += ((TInt8*) data)[ j ];
                        }
                    }
                }
            else
                {
                if ( dataBufferSize == 2 )
                    {
                    if ( vol > 0 )
                        {
                        for ( TInt j = 0; j < dataLen; j++ )
                            {
                            iBufferMixed[ j ] += (((TInt16*) data)[ j ]) << vol;
                            }
                        }
                    else
                        {
                        for ( TInt j = 0; j < dataLen; j++ )
                            {
                            iBufferMixed[ j ] += (((TInt16*) data)[ j ]) >> (-vol);
                            }
                        }
                    }
                else if ( dataBufferSize == 1 )
                    {
                    if ( vol > 0 )
                        {
                        for ( TInt j = 0; j < dataLen; j++ )
                            {
                            iBufferMixed[ j ] += (((TInt8*) data)[ j ]) << vol;
                            }
                        }
                    else
                        {
                        for ( TInt j = 0; j < dataLen; j++ )
                            {
                            iBufferMixed[ j ] += (((TInt8*) data)[ j ]) >> (-vol);
                            }
                        }
                    }
                }
            }
        }

    if ( dataBufferSize == 2 )
        {
        const TInt32 KLowerBoundary = -32768; //-0x7fff;
        const TInt32 KUpperBoundary = 32767; //0x7fff - 1;

        TInt16 *buffer = (TInt16*) iBufferOutput[ iCurrentOutputBuffer ];
        
        for ( TUint i = 0; i < iConfig.iBufferSize; i++ )
            {
            if( iBufferMixed[ i ] < KLowerBoundary )
                {
                iBufferMixed[ i ] = KLowerBoundary;
                }
            else if( iBufferMixed[ i ] > KUpperBoundary )
                {
                iBufferMixed[ i ] = KUpperBoundary;
                }
            
            buffer[ i ] = (TInt16) iBufferMixed[ i ];
            }
        }
    else if ( dataBufferSize == 1 )
        {
        const TInt32 KLowerBoundary = -0x7f;
        const TInt32 KUpperBoundary = 0x7f - 1;

        TInt8 *buffer = (TInt8*) iBufferOutput[ iCurrentOutputBuffer ];
        
        for ( TUint i = 0; i < iConfig.iBufferSize; i++ )
            {
            if( iBufferMixed[ i ] < KLowerBoundary )
                {
                iBufferMixed[ i ] = KLowerBoundary;
                }
            else if( iBufferMixed[ i ] > KUpperBoundary )
                {
                iBufferMixed[ i ] = KUpperBoundary;
                }
            
            buffer[ i ] = (TInt8) iBufferMixed[ i ];
            }
        }
    }

void CSGEAudioMixerMono::FillBuffer( TAny *aBuffer, TInt aBufferSize )
    {
    ASSERT( aBufferSize == iBufferOutputSize );
    
    Mem::Copy( aBuffer, iBufferOutput[ iCurrentOutputBuffer ], 
               ( iBufferOutputSize > aBufferSize ? aBufferSize : iBufferOutputSize ) );
    }


