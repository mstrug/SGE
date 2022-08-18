/*
 ============================================================================
 Name		: SGESecurity.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : 
 ============================================================================
 */

#include "SGEImeiGetter.h"
#include <COEMAIN.H>
#include <BAUTILS.H>
#include <apmrec.h> 
#include <apgcli.h>


EXPORT_C CSGEImeiGetter* CSGEImeiGetter::NewL( MSGEImeiGetterObserver* aObserver, TInt aPriority )
    {
    CSGEImeiGetter* self = NewLC(aObserver);
    CleanupStack::Pop(self);
    return self;
    }
    
EXPORT_C CSGEImeiGetter* CSGEImeiGetter::NewLC( MSGEImeiGetterObserver* aObserver, TInt aPriority )
    {
    CSGEImeiGetter* self = new (ELeave) CSGEImeiGetter( aObserver, aPriority );
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }
    
  
CSGEImeiGetter::CSGEImeiGetter( MSGEImeiGetterObserver* aObserver, TInt aPriority ) : CActive( aPriority ), iObserver(aObserver), iIdV1Pkg(iIdV1)
    {
    }
 
EXPORT_C CSGEImeiGetter::~CSGEImeiGetter()
    {
	if ( iTelephony )
		{
		Cancel();
		delete iTelephony;	
		}
    }

 
void CSGEImeiGetter::ConstructL(void)
    {	
	CActiveScheduler::Add(this);
	
	iTelephony = CTelephony::NewL();
	iTelephony->GetPhoneId(iStatus,iIdV1Pkg);
	SetActive();	
    }
 
void CSGEImeiGetter::DoCancel()
    {
	iTelephony->CancelAsync(CTelephony::EGetPhoneIdCancel);
    }
 
void CSGEImeiGetter::RunL()
    {
    if ( iStatus.Int() == KErrNone )
        {
        TBuf<50> imei;
        imei.Copy( iIdV1.iSerialNumber );
//        imei.Copy( _L("35908101232327/2") );
//        imei.Copy( _L("357482010609641") );
//        imei.Copy( _L("358622010490539") );
        for ( TInt i = 0; i < imei.Length(); i++ )
            {
            if ( imei[ i ] < 0x30 || imei[ i ] > 0x39 )
                {
                imei.Delete( i, 1 );
                i--;
                }
            }
        
        iObserver->GotImeiL( imei, KErrNone );
        }
    else
        {
        iObserver->GotImeiL( iIdV1.iSerialNumber, iStatus.Int() );
        }
    }

