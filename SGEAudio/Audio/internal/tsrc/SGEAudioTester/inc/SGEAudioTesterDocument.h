/*
========================================================================
 Name        : SGEAudioTesterDocument.h
 Author      : Michal Strug
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : 
========================================================================
*/
#ifndef SGEAUDIOTESTERDOCUMENT_H
#define SGEAUDIOTESTERDOCUMENT_H

#include <akndoc.h>
        
class CEikAppUi;

/**
* @class    CSGEAudioTesterDocument SGEAudioTesterDocument.h
* @brief    A CAknDocument-derived class is required by the S60 application 
*           framework. It is responsible for creating the AppUi object. 
*/
class CSGEAudioTesterDocument : public CAknDocument
    {
public: 
    // constructor
    static CSGEAudioTesterDocument* NewL( CEikApplication& aApp );

private: 
    // constructors
    CSGEAudioTesterDocument( CEikApplication& aApp );
    void ConstructL();
    
public: 
    // from base class CEikDocument
    CEikAppUi* CreateAppUiL();
    };
#endif // SGEAUDIOTESTERDOCUMENT_H
