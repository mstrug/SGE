/*
 ============================================================================
  Name        : SGEUtilsGPTimerObserver.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#ifndef __SGEUTILSGPTIMEROBSERVER_H__
#define __SGEUTILSGPTIMEROBSERVER_H__


// CLASS DECLARATION
class CSGEUtilsGPTimer;

/**
 * MSGEUtilsGPTimerObserver
 * 
 */
class MSGEUtilsGPTimerObserver
    {
public:
    /**
     * Called by CSSGEUtilsGPTimer when specified time elapse.
     * @param aTimer timer object
     */
    virtual void GPTimerEvent( CSGEUtilsGPTimer* aTimer ) = 0;
    };


#endif // __SGEUTILSGPTIMEROBSERVER_H__
