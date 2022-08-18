/*
 ============================================================================
  Name        : SGEImeiGetter.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#ifndef __SGEIMEI_GETTER_H__
#define __SGEIMEI_GETTER_H__

#include <e32base.h>
#include <F32FILE.H>
#include <d32dbms.h>
#include <BADESCA.H>
#include <Etel3rdParty.h>


 
class MSGEImeiGetterObserver
	{
public:
	virtual void GotImeiL(const TDesC& aIMEI,TInt aError) = 0;
	};


NONSHARABLE_CLASS( CSGEImeiGetter ) : public CActive
	{
public:

    IMPORT_C static CSGEImeiGetter* NewL( MSGEImeiGetterObserver* aObserver, TInt aPriority = CActive::EPriorityStandard );

    IMPORT_C static CSGEImeiGetter* NewLC( MSGEImeiGetterObserver* aObserver, TInt aPriority = CActive::EPriorityStandard );
	
    IMPORT_C ~CSGEImeiGetter();
	
private:
	
	void DoCancel();
	
	void RunL();
	
private:
	
	CSGEImeiGetter( MSGEImeiGetterObserver* aObserver, TInt aPriority );
	
	void ConstructL();
	
private:
	
	MSGEImeiGetterObserver* iObserver;
	
	CTelephony* iTelephony;
	
	CTelephony::TPhoneIdV1 iIdV1;
	
	CTelephony::TPhoneIdV1Pckg iIdV1Pkg;
	
	};

#endif // __SGEIMEI_GETTER_H__

