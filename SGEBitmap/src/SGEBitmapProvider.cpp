/*
 ============================================================================
  Name        : SGEBitmapProvider.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#include "SGEBitmapProvider.h"
#include "SGEBitmapLoader.h"
#include "SGEBitmapSet.h"
#include "SGEBitmapItem.h"
#include "SGEBitmapItemCreator.h"

    
const TUint32 KSGEBitmapProviderStreamId = 0x042454753;
const TUint16 KSGEBitmapProviderStreamVersion = 0x0001;


// -----------------------------------------------------------------------------
// CSGEBitmapProvider::CSGEBitmapProvider()
// -----------------------------------------------------------------------------
//
CSGEBitmapProvider::CSGEBitmapProvider( MSGEBitmapProviderObserver *aObserver )
    : iObserver( aObserver )
    {
    }

// -----------------------------------------------------------------------------
// CSGEBitmapProvider::~CSGEBitmapProvider()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEBitmapProvider::~CSGEBitmapProvider()
    {
    delete iBitmapLoader;
    iBitmapSets.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CSGEBitmapProvider::NewL()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEBitmapProvider* CSGEBitmapProvider::NewL( MSGEBitmapProviderObserver *aObserver )
    {
    CSGEBitmapProvider *self = CSGEBitmapProvider::NewLC( aObserver );
    CleanupStack::Pop(); // self;
    return self;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapProvider::NewLC()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEBitmapProvider* CSGEBitmapProvider::NewLC( MSGEBitmapProviderObserver *aObserver )
    {
    CSGEBitmapProvider *self = new (ELeave) CSGEBitmapProvider( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapProvider::NewLC()
// -----------------------------------------------------------------------------
//
void CSGEBitmapProvider::ConstructL()
    {
    iBitmapLoader = CSGEBitmapLoader::NewL( *this );
    }

// -----------------------------------------------------------------------------
// CSGEBitmapProvider::SetObserver()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEBitmapProvider::SetObserver( MSGEBitmapProviderObserver *aObserver )
    {
    iObserver = aObserver;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapProvider::CreateBitmapSetL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEBitmapProvider::CreateBitmapSetL( TUint aBitmapSetId )
    {
    ASSERT( aBitmapSetId > 0 ); // bitmap set ID should be greather than 0
    
    if ( GetBitmapSetIdx( aBitmapSetId ) >= 0 )
        {
        User::Leave( KErrAlreadyExists );
        }
    
    CSGEBitmapSet *bitmapSet = new (ELeave) CSGEBitmapSet( aBitmapSetId );
    CleanupStack::PushL( bitmapSet );
    iBitmapSets.AppendL( bitmapSet );
    CleanupStack::Pop( bitmapSet );
    
    if ( !iCurrentBitmapSet )
        {
        iCurrentBitmapSet = bitmapSet;
        }
    }

// -----------------------------------------------------------------------------
// CSGEBitmapProvider::SelectBitmapSet()
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSGEBitmapProvider::SelectBitmapSet( TUint aBitmapSetId )
    {
    TInt idx = GetBitmapSetIdx( aBitmapSetId );
    if ( idx >= 0 )
        {
        iCurrentBitmapSet = iBitmapSets[ idx ];
        return ETrue;
        }
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapProvider::CurrentBitmapSet()
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CSGEBitmapProvider::CurrentBitmapSet() const
    {
    return ( iCurrentBitmapSet ? iCurrentBitmapSet->Id() : 0 );
    }

// -----------------------------------------------------------------------------
// CSGEBitmapProvider::RemoveBitmapSet()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEBitmapProvider::RemoveBitmapSet( TUint aBitmapSetId )
    {
    TInt idx = GetBitmapSetIdx( aBitmapSetId );
    if ( idx >= 0 )
        {
        if ( iCurrentBitmapSet && iCurrentBitmapSet->Id() == aBitmapSetId )
            {
            iCurrentBitmapSet = NULL;
            }
        iBitmapSets.Remove( idx );
        }
    }

// -----------------------------------------------------------------------------
// CSGEBitmapProvider::RemoveBitmapSetAll()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEBitmapProvider::RemoveBitmapSetAll()
    {
    iBitmapSets.ResetAndDestroy();
    iCurrentBitmapSet = NULL;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapProvider::BitmapItem()
// -----------------------------------------------------------------------------
//
EXPORT_C CSGEBitmapItem* CSGEBitmapProvider::BitmapItem( TUint aBitmapId )
    {
    ASSERT( iCurrentBitmapSet );
    
    return iCurrentBitmapSet->GetBitmapItem( aBitmapId );
    }

// -----------------------------------------------------------------------------
// CSGEBitmapProvider::AddImageToLoadL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEBitmapProvider::AddImageToLoadL( const TDesC& aFileName, TUint aBitmapId )
    {
    CheckCurrentBitmapSetAndCreateL();
    
    CSGEBitmapItemCreator *item = new (ELeave) CSGEBitmapItemCreator( aFileName, aBitmapId );
    CleanupStack::PushL( item );
    iBitmapLoader->AddBitmapToLoadL( item );
    CleanupStack::Pop( item );
    }

// -----------------------------------------------------------------------------
// CSGEBitmapProvider::AddImageToLoadL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEBitmapProvider::AddImageToLoadL( CSGEFile &aSGEFile, const TDesC& aFileName, TUint aBitmapId )
    {
    CheckCurrentBitmapSetAndCreateL();
    
    CSGEBitmapItemCreator *item = new (ELeave) CSGEBitmapItemCreator( aSGEFile, aFileName, aBitmapId );
    CleanupStack::PushL( item );
    iBitmapLoader->AddBitmapToLoadL( item );
    CleanupStack::Pop( item );
    }

// -----------------------------------------------------------------------------
// CSGEBitmapProvider::AddImageToLoadL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEBitmapProvider::AddImageToLoadL( CSGEBitmapItemCreator *aBitmapItemCreator )
    {
    CheckCurrentBitmapSetAndCreateL();
    
    iBitmapLoader->AddBitmapToLoadL( aBitmapItemCreator );
    }

// -----------------------------------------------------------------------------
// CSGEBitmapProvider::StartLoadingImagesL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEBitmapProvider::StartLoadingImagesL()
    {
    iBitmapLoader->StartLoadingBitmapsL();
    }

// -----------------------------------------------------------------------------
// CSGEBitmapProvider::AddBitmapItemL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEBitmapProvider::AddBitmapItemL( CSGEBitmapItem *aBitmapItem )
    {
    ASSERT( aBitmapItem );
    CheckCurrentBitmapSetAndCreateL();
    
    if ( iCurrentBitmapSet->GetBitmapItem( aBitmapItem->Id() ) )
        {
        User::Leave( KErrAlreadyExists );
        }
    iCurrentBitmapSet->Items().AppendL( aBitmapItem );
    }

// -----------------------------------------------------------------------------
// CSGEBitmapProvider::RemoveBitmapItem()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEBitmapProvider::RemoveBitmapItem( TUint aBitmapId )
    {
    ASSERT( iCurrentBitmapSet );
    
    iCurrentBitmapSet->RemoveBitmapItem( aBitmapId );
    }

// -----------------------------------------------------------------------------
// CSGEBitmapProvider::RemoveBitmapItemAll()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEBitmapProvider::RemoveBitmapItemAll()
    {
    ASSERT( iCurrentBitmapSet );
    
    iCurrentBitmapSet->Items().ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CSGEBitmapProvider::BitBlt()
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSGEBitmapProvider::BitBlt( CBitmapContext &aGc, TPoint aPoint, TUint aBitmapItem )
    {
    ASSERT( iCurrentBitmapSet );
    
    CSGEBitmapItem *item = iCurrentBitmapSet->GetBitmapItem( aBitmapItem );
    if ( !item )
        {
        return EFalse;
        }
    
    if ( !item->BitmapMask() )
        {
        aGc.BitBlt( aPoint, item->Bitmap() );
        }
    else
        {
        TRect r( TPoint(), item->Bitmap()->SizeInPixels() );
        aGc.BitBltMasked( aPoint, item->Bitmap(), r, item->BitmapMask(), EFalse );
        }
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapProvider::BitBlt()
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSGEBitmapProvider::BitBlt( CBitmapContext &aGc, TPoint aPoint, TRect aSourceRect, 
                                           TUint aBitmapItem, TBool aInvertMask )
    {
    ASSERT( iCurrentBitmapSet );
    
    CSGEBitmapItem *item = iCurrentBitmapSet->GetBitmapItem( aBitmapItem );
    if ( !item )
        {
        return EFalse;
        }
    
    if ( !item->BitmapMask() )
        {
        aGc.BitBlt( aPoint, item->Bitmap() );
        }
    else
        {
        aGc.BitBltMasked( aPoint, item->Bitmap(), aSourceRect, item->BitmapMask(), aInvertMask );
        }
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapProvider::ExternalizeL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEBitmapProvider::ExternalizeL( RWriteStream &aStream ) const
    {
    if ( !iBitmapLoader->AllBitmapsLoaded() )
        {
        User::Leave( KErrNotReady );
        }

    aStream.WriteUint32L( KSGEBitmapProviderStreamId );
    aStream.WriteUint16L( KSGEBitmapProviderStreamVersion );
    aStream.WriteUint32L( CurrentBitmapSet() );
    aStream.WriteUint16L( iBitmapSets.Count() );
    
    for ( TInt i = 0; i < iBitmapSets.Count(); i++ )
        {
        aStream << *iBitmapSets[ i ];
        }
    }

// -----------------------------------------------------------------------------
// CSGEBitmapProvider::InternalizeL()
// -----------------------------------------------------------------------------
//
EXPORT_C void CSGEBitmapProvider::InternalizeL( RReadStream& aStream )
    {
    if ( aStream.ReadUint32L() != KSGEBitmapProviderStreamId )
        {
        User::Leave( KErrArgument );
        }
    if ( aStream.ReadUint16L() != KSGEBitmapProviderStreamVersion )
        {
        User::Leave( KErrNotSupported );
        }
    
    TUint32 curBitmapSetId = aStream.ReadUint32L();
    TInt bitmapSetCnt = aStream.ReadUint16L();
    
    for ( TInt i = 0; i < bitmapSetCnt; i++ )
        {
        CSGEBitmapSet *set = new (ELeave) CSGEBitmapSet( 0 );
        CleanupStack::PushL( set );
        
        aStream >> *set;

        TInt idx = GetBitmapSetIdx( set->Id() );
        if ( idx > KErrNotFound )
            {
            RPointerArray< CSGEBitmapItem > &items = set->Items();
            for ( TInt j = 0; j < items.Count(); j++ )
                {
                if ( iBitmapSets[ idx ]->GetBitmapItem( items[ j ]->Id() ) )
                    {
                    User::Leave( KErrAlreadyExists );
                    }
                
                iBitmapSets[ idx ]->Items().AppendL( items[ j ] );
                
                items.Remove( j-- );
                }
            CleanupStack::PopAndDestroy( set );
            }
        else
            {
            iBitmapSets.AppendL( set );
            CleanupStack::Pop( set );
            }
        }
    
    if ( !SelectBitmapSet( curBitmapSetId ) )
        {
        User::Leave( KErrNotFound );
        }
    }

// -----------------------------------------------------------------------------
// CSGEBitmapProvider::GetBitmapSetIdx()
// -----------------------------------------------------------------------------
//
TInt CSGEBitmapProvider::GetBitmapSetIdx( TUint aBitmapSetId )
    {
    for ( TInt i = 0; i < iBitmapSets.Count(); i++ )
        {
        if ( iBitmapSets[ i ]->Id() == aBitmapSetId )
            {
            return i;
            }
        }
    return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// CSGEBitmapProvider::CheckCurrentBitmapSetAndCreateL()
// -----------------------------------------------------------------------------
//
void CSGEBitmapProvider::CheckCurrentBitmapSetAndCreateL()
    {
    if ( !iCurrentBitmapSet && iBitmapSets.Count() > 0 )
        {
        User::Leave( KErrNotReady );
        }
    else if ( !iCurrentBitmapSet && iBitmapSets.Count() == 0 )
        {
        const TUint KAutoBitmapSetId = 1;
        CreateBitmapSetL( KAutoBitmapSetId );
        }
    }

// -----------------------------------------------------------------------------
// CSGEBitmapProvider::BitmapLoaded()
// -----------------------------------------------------------------------------
//
void CSGEBitmapProvider::BitmapLoaded( CSGEBitmapItem *aBitmapItem )
    {
    ASSERT( aBitmapItem );
    
    TInt err = iCurrentBitmapSet->Items().Append( aBitmapItem );
    
    if ( err != KErrNone )
        {
        iObserver->BitmapLoaded( aBitmapItem->Id(), err );
        delete aBitmapItem;
        }
    else
        {
        iObserver->BitmapLoaded( aBitmapItem->Id(), KErrNone );
        }
    }

// -----------------------------------------------------------------------------
// CSGEBitmapProvider::BitmapLoadError()
// -----------------------------------------------------------------------------
//
void CSGEBitmapProvider::BitmapLoadError( TUint aBitmapId, TInt aError )
    {
    iObserver->BitmapLoaded( aBitmapId, aError );
    }

// -----------------------------------------------------------------------------
// CSGEBitmapProvider::BitmapLoadedAll()
// -----------------------------------------------------------------------------
//
void CSGEBitmapProvider::BitmapLoadedAll()
    {
    iObserver->BitmapsLoadedAll();
    }


// -----------------------------------------------------------------------------
// CSGEBitmapProvider::BitmapLoadCustom()
// -----------------------------------------------------------------------------
//
TBool CSGEBitmapProvider::BitmapLoadCustom( TUint aBitmapId, TFileName aFileName, RBuf8& aData )
    {
    return iObserver->BitmapLoadCustom( aBitmapId, aFileName, aData );
    }

// -----------------------------------------------------------------------------
// CSGEBitmapProvider::BitmapLoadCustom()
// -----------------------------------------------------------------------------
//
void CSGEBitmapProvider::BitmapLoadFrameInfo( TUint aBitmapId, TFrameInfo& aFrameInfo )
    {
    iObserver->BitmapLoadFrameInfo( aBitmapId, aFrameInfo );
    }

// End of file
