/*
 ============================================================================
 Name		: SGEGuiManagerControl.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEGuiManagerControl declaration
 ============================================================================
 */

#ifndef SGEGUIMANAGERCONTROL_H
#define SGEGUIMANAGERCONTROL_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <SGEEngine/SGEObjectInterfaces.h>

// FORWARD DECLARATIONS
class CSGEGuiControlBase;

// CLASS DECLARATION

/**
 *  CSGEGuiManagerControl
 * 
 */
class CSGEGuiManagerControl : public CBase, public MSGEDrawableObject, public MSGEUpdatableObject,
                           public MSGEKeyEventableObject, public MSGEPointerEventableObject
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    IMPORT_C ~CSGEGuiManagerControl();

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CSGEGuiManagerControl* NewL();

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CSGEGuiManagerControl* NewLC();

    IMPORT_C void AddControlL( CSGEGuiControlBase *aControl );

    IMPORT_C CSGEGuiControlBase* GetControl( TUint aControlId ) const;

    IMPORT_C TRect GetRedrawRect( TRect aRect ) const;

    IMPORT_C void ResetControls();
    
private:

    /**
     * Constructor for performing 1st stage construction
     */
    IMPORT_C CSGEGuiManagerControl();

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    IMPORT_C void ConstructL();

public: // from MSGEDrawableObject
    
    virtual void Draw( CBitmapContext *aGc, const TRect aDrawRect ); 

public: // from MSGEUpdatableObject
    
    virtual TBool Update( const TTimeIntervalMicroSeconds32 aTimeIntercal ); 

public: // from MSGEKeyEventableObject
    
    virtual TBool KeyDownEvent( const TKeyEvent &aKeyEvent ); 

    virtual TBool KeyUpEvent( const TKeyEvent &aKeyEvent ); 

    virtual TBool KeyEvent( const TKeyEvent &aKeyEvent );

public: // from MSGEPointerEventableObject
    
    virtual TBool PointerEvent( const TPointerEvent &aPointerEvent ); 

private: // data
    
    RPointerArray< CSGEGuiControlBase > iControls;
    
    TTime iPushedEventTime;

    CSGEGuiControlBase *iPushedEventControl; // not owned
    
    };

#endif // SGEGUIMANAGERCONTROL_H
