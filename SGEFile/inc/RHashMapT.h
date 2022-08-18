#ifndef RHASHMAPT_H_
#define RHASHMAPT_H_

#include <e32hashtab.h>
#include <S32FILE.H> 



template< TInt T >
NONSHARABLE_CLASS( RHashMapT ) : public RHashMap< TBuf<T>, TUid > 
	{
public:
	RHashMapT(): RHashMap< TBuf<T>, TUid >( &HBufCHash, &HBufCIdent ) 
		{
		}
	
	inline void InternalizeL( RReadStream& aStream )
		{
		TInt count = aStream.ReadInt32L();
		
		for ( TInt i = 0; i < count; i++ )
			{
			TUid uid = TUid::Uid( aStream.ReadInt32L() );
			TInt32 len = aStream.ReadInt16L();

			TBuf<T> buf;
			aStream.ReadL( buf, len );
			
			this->Insert( buf, uid );
			}
		}
	
	inline void ExternalizeL( RWriteStream& aStream ) const
		{
		THashMapIter< TBuf<T>, TUid > iterator( *this );
		iterator.Reset();
		
		aStream.WriteInt32L( this->Count() );
	
		TBuf<T> *buf = (TBuf<T>*) iterator.NextKey();
		TUid *uid = iterator.CurrentValue();
		
		while ( uid )
			{		
			aStream.WriteInt32L( uid->iUid );
			aStream.WriteInt16L( buf->Length() );
			aStream.WriteL( *buf, buf->Length() );
			
			buf = (TBuf<64>*) iterator.NextKey();
			uid = iterator.CurrentValue();
			}
		}


/*	RArray< TBuf<T> > GetValues()
		{
		RArray< TBuf<T> > ret;
		THashMapIter< TBuf<T>, TUid > iterator( *this );
		iterator.Reset();

		TBuf<T> *buf = (TBuf<T>*) ( iterator.NextKey() );
		TUid *uid = iterator.CurrentValue();
		
		while ( uid )
			{		
			ret.Append( *buf );
			
			buf = (TBuf<T>*) iterator.NextKey();
			uid = iterator.CurrentValue();
			}
		
		return ret;
		}*/

		
	void GetDataAsTEntry( RArray< TEntry > &ret, TUint aAtt = KEntryAttNormal, TBool aFillUids = ETrue )
		{
		THashMapIter< TBuf<T>, TUid > iterator( *this );
		iterator.Reset();

		TBuf<T> *buf = (TBuf<T>*) ( iterator.NextKey() );
		TUid *uid = iterator.CurrentValue();
		
		while ( uid )
			{
			TEntry entry;
			entry.iName = *buf;
			entry.iAtt = aAtt;
			if ( aFillUids )
				{
				entry.iType = *uid;
				}
			ret.Append( entry );
			
			buf = (TBuf<T>*) iterator.NextKey();
			uid = iterator.CurrentValue();
			}
		}
		
private:
/*	TUint32 TUidHash( const TUid& uid )
		{
		return DefaultHash::Integer( uid.iUid );		
		} 
	
	TBool TUidIdent( const TUid& aL, const TUid& aR )
		{
		return DefaultIdentity::Integer( aL.iUid, aR.iUid );
		} 	

	TUint32 TBufCHash( const TBufC<24>& buf )
		{
		return DefaultHash::Des16( buf );		
		} 
	
	TBool TBufCIdent( const TBufC<24>& aL, const TBufC<24>& aR )
		{
		return DefaultIdentity::Des16( aL, aR );
		} 	*/
	
	static TUint32 HBufCHash( TBuf<T> const &aBuf )
		{
		return DefaultHash::Des16(aBuf);
		}
	static TBool HBufCIdent( TBuf<T> const &aL, TBuf<T> const &aR )
		{
		return DefaultIdentity::Des16(aL, aR);
		} 
	
	};

NONSHARABLE_CLASS( RHashMap2 ) : public RHashMapT<64>
	{
	};


NONSHARABLE_CLASS( RHashMapS ) : public RHashMap< TUid, TInt > 
	{
public:
	RHashMapS(): RHashMap< TUid, TInt >( &TUidHash, &TUidIdent ) 
		{
		}
	
	inline void InternalizeL( RReadStream& aStream )
		{
		TInt count = aStream.ReadInt32L();
		
		for ( TInt i = 0; i < count; i++ )
			{
			TUid uid = TUid::Uid( aStream.ReadInt32L() );
			TInt size = aStream.ReadInt32L();
		
			this->Insert( uid, size );
			}
		}
	
	inline void ExternalizeL( RWriteStream& aStream ) const
		{
		THashMapIter< TUid, TInt > iterator( *this );
		iterator.Reset();
		
		aStream.WriteInt32L( this->Count() );
	
		TUid *uid = (TUid *) iterator.NextKey();
		TInt *size = iterator.CurrentValue();
		
		while ( uid )
			{		
			aStream.WriteInt32L( uid->iUid );
			aStream.WriteInt32L( *size );
			
			uid = (TUid *) iterator.NextKey();
			size = iterator.CurrentValue();
			}
		}

private:
	static TUint32 TUidHash( TUid const &uid )
		{
		return DefaultHash::Integer( uid.iUid );		
		} 
	
	static TBool TUidIdent( TUid const &aL, TUid const &aR )
		{
		return DefaultIdentity::Integer( aL.iUid, aR.iUid );
		}
	};

#endif /*RHASHMAPT_H_*/
