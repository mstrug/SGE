/*
 ============================================================================
 Name		: SGESensorBase.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGESensorBase implementation
 ============================================================================
 */

#ifndef __S60_3X__

#include "SGESensorBase.h"
#include <sensrvchannelfinder.h> 
#include <sensrvchannel.h> 



CSGESensorBase::CSGESensorBase()
    {
    // No implementation required
    }

EXPORT_C CSGESensorBase::~CSGESensorBase()
    {
    if ( iSensrvChannel )
        {
        iSensrvChannel->StopDataListening();
        iSensrvChannel->CloseChannel();
        }
    delete iSensrvChannel;
    }

void CSGESensorBase::BaseConstructL( TSensrvChannelTypeId aChannelType )
    {
    CSensrvChannelFinder *SensrvChannelFinder = CSensrvChannelFinder::NewLC(); 
    RSensrvChannelInfoList ChannelInfoList;
    CleanupClosePushL( ChannelInfoList );
 
    TSensrvChannelInfo mySearchConditions;
    mySearchConditions.iChannelType = aChannelType;
    SensrvChannelFinder->FindChannelsL( ChannelInfoList, mySearchConditions );

    if ( ChannelInfoList.Count() == 0 )
    {
        User::Leave( KErrNotFound );
    }

    iSensrvChannel = CSensrvChannel::NewL( ChannelInfoList[ 0 ] );

    CleanupStack::PopAndDestroy( &ChannelInfoList );
    CleanupStack::PopAndDestroy( SensrvChannelFinder );

    iSensrvChannel->OpenChannelL();
    }

EXPORT_C void CSGESensorBase::StartListeningL()
    {
    iSensrvChannel->StartDataListeningL( this, 1, 1, 0 );
    }

EXPORT_C void CSGESensorBase::StopListening()
    {
    iSensrvChannel->StopDataListening();
    }

void CSGESensorBase::GetDataListenerInterfaceL( TUid /*aInterfaceUid*/, TAny*& aInterface )
    {
    aInterface = NULL;
    }


#endif // __S60_3X__

// End of file
