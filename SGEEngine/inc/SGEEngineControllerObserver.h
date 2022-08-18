/*
 ============================================================================
  Name        : SGEEngineControllerObserver.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#ifndef __SGEENGINECONTROLLEROBSERVER_H__
#define __SGEENGINECONTROLLEROBSERVER_H__


// CLASS DECLARATION

/**
 * MSGEEngineControllerObserver
 * 
 */
class MSGEEngineControllerObserver
    {
public:
    /**
     * Called by CSSGEEngineController when engine controller is to be closed.
     * @param aError KErrNone if close is caused by normal program flow
     *        otherwise system error which causes to close engine controller
     */
    virtual void EngineControllerClose( TInt aError ) = 0;
    
    /**
     * Used by engine controller to invoke actions on the owner.
     */
    virtual void EngineControllerAction( TInt /*aAction*/, TInt /*aParameter*/ ) { };
    };


#endif // __SGEENGINECONTROLLEROBSERVER_H__
