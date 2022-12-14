/*
========================================================================
 Name        : SGEAudioTesterSettingItemList.h
 Author      : Michal Strug
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : 
========================================================================
*/
#ifndef SGEAUDIOTESTERSETTINGITEMLIST_H
#define SGEAUDIOTESTERSETTINGITEMLIST_H

// [[[ begin generated region: do not modify [Generated Includes]
#include <aknsettingitemlist.h>
// ]]] end generated region [Generated Includes]


// [[[ begin [Event Handler Includes]
// ]]] end [Event Handler Includes]

// [[[ begin generated region: do not modify [Generated Forward Declarations]
class MEikCommandObserver;
class TSGEAudioTesterSettingItemListSettings;
// ]]] end generated region [Generated Forward Declarations]

/**
 * @class    CSGEAudioTesterSettingItemList SGEAudioTesterSettingItemList.h
 */
class CSGEAudioTesterSettingItemList : public CAknSettingItemList
    {
public: // constructors and destructor

    CSGEAudioTesterSettingItemList( 
            TSGEAudioTesterSettingItemListSettings& settings, 
            MEikCommandObserver* aCommandObserver );
    virtual ~CSGEAudioTesterSettingItemList();

public:

    // from CCoeControl
    void HandleResourceChange( TInt aType );

    // overrides of CAknSettingItemList
    CAknSettingItem* CreateSettingItemL( TInt id );
    void EditItemL( TInt aIndex, TBool aCalledFromMenu );
    TKeyResponse OfferKeyEventL( 
            const TKeyEvent& aKeyEvent, 
            TEventCode aType );

public:
    // utility function for menu
    void ChangeSelectedItemL();

    void LoadSettingValuesL();
    void SaveSettingValuesL();
        
private:
    // override of CAknSettingItemList
    void SizeChanged();

private:
    // current settings values
    TSGEAudioTesterSettingItemListSettings& iSettings;
    MEikCommandObserver* iCommandObserver;
    // [[[ begin generated region: do not modify [Generated Methods]
public: 
    // ]]] end generated region [Generated Methods]
    
    // [[[ begin generated region: do not modify [Generated Type Declarations]
public: 
    // ]]] end generated region [Generated Type Declarations]
    
    // [[[ begin generated region: do not modify [Generated Instance Variables]
private: 
    // ]]] end generated region [Generated Instance Variables]
    
    
    // [[[ begin [Overridden Methods]
protected: 
    // ]]] end [Overridden Methods]
    
    
    // [[[ begin [User Handlers]
protected: 
    // ]]] end [User Handlers]
    
    };
#endif // SGEAUDIOTESTERSETTINGITEMLIST_H
