/*
 ============================================================================
 Name		: SGEGameTemplateSettings.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEGameTemplateSettings declaration
 ============================================================================
 */

#ifndef SGEGAMETEMPLATESETTINGS_H
#define SGEGAMETEMPLATESETTINGS_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

// CLASS DECLARATION

/**
 *  CSGEGameTemplateSettings
 * 
 */
class CSGEGameTemplateSettings : public CBase
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CSGEGameTemplateSettings();

    /**
     * Two-phased constructor.
     */
    static CSGEGameTemplateSettings* NewL();

    /**
     * Two-phased constructor.
     */
    static CSGEGameTemplateSettings* NewLC();

    void ReadSettingsL();

    void WriteSettingsL();

private:

    /**
     * Constructor for performing 1st stage construction
     */
    CSGEGameTemplateSettings();

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();
    
    };

#endif // SGEGAMETEMPLATESETTINGS_H
