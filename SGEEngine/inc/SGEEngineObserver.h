/*
 ============================================================================
  Name        : SGEEngineObserver.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#ifndef __SGEENGINEOBSERVER_H__
#define __SGEENGINEOBSERVER_H__


// CLASS DECLARATIONS

/**
 * MSGEEngineObserver
 * 
 */
class MSGEEngineObserver
    {
public:
    /**
     * Called by CSSGEEngine when engine is to be closed.
     * @param aId caller engine id
     * @param aError KErrNone if close is caused by normal program flow
     *        otherwise system error which causes to close engine
     */
    virtual void EngineClose( TUint aId, TInt aError ) = 0;
    
    /**
     * Called by CSSGEEngine when engines should be switched.
     * @param aId caller engine id
     */
    virtual void EngineSwitch( TUint aId ) = 0;
    
    /**
     * Used by engine to invoke actions on the owner.
     */
    virtual void EngineAction( TInt /*aAction*/, TInt /*aParameter*/ ) { };
    };


#endif // __SGEENGINEOBSERVER_H__
