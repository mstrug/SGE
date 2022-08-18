/*
 * MyApplication.h
 *
 *  Created on: 2009-10-12
 *      Author: michal.strug
 *   Copyright: Copyright (c) 2022 Michal Strug. All rights reserved.
 */

#ifndef CMYAPPLICATION_H_
#define CMYAPPLICATION_H_

#include <SGEApplication.h>

class CMyApplication : public CSGEApplication
    {
public:
    
    CMyApplication();
    
    ~CMyApplication();

    TUid ApplicationUid();

    void HandleCommandL( TInt aCommand );

    TKeyResponse HandleKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );

    static CApaApplication* NewApplication();

    void Draw( const TRect& aRect ) const;

    };

#endif /* CMYAPPLICATION_H_ */
