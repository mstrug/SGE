/*
 ============================================================================
 Name		: SGEGameTemplateCommonObjects.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEGameTemplateCommonObjects declaration
 ============================================================================
 */

#ifndef SGEGAMETEMPLATECOMMONOBJECTS_H
#define SGEGAMETEMPLATECOMMONOBJECTS_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <SGEEngine/SGEEngineCommonObjects.h>

// FORWARD DECLARATIONS
class CSGEGameTemplateSettings;

// CLASS DECLARATION

/**
 *  CSGEGameTemplateCommonObjects
 * 
 */
class CSGEGameTemplateCommonObjects : public CSGEEngineCommonObjects
    {
public:
    
    /**
     * Constructor.
     */
    CSGEGameTemplateCommonObjects();
    
    /**
     * Destructor.
     */
    ~CSGEGameTemplateCommonObjects();

    /**
     * Sets settings.
     */
    void SetSettings( CSGEGameTemplateSettings *aSettings );
    
    /**
     * Returns settings.
     */
    CSGEGameTemplateSettings* Settings() const;
    
private: // data

    CSGEGameTemplateSettings *iSettings;
    
    };

#endif // SGEGAMETEMPLATECOMMONOBJECTS_H
