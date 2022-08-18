/*
 ============================================================================
 Name		: SGEGuiPageBase.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEGuiPageBase declaration
 ============================================================================
 */

#ifndef SGEGUIPAGEBASE_H
#define SGEGUIPAGEBASE_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <SGEEngine/SGEObjectInterfaces.h>
#include <SGEGui/SGEGuiControlBase.h>

// FORWARD DECLARATIONS
class CSGEGuiManagerControl;

// CLASS DECLARATION

/**
 *  CSGEGuiPageBase
 * 
 */
class CSGEGuiPageBase : public CBase, public MSGEDrawableObject, public MSGEUpdatableObject,
                        public MSGEKeyEventableObject, public MSGEPointerEventableObject,
                        public MSGEGuiControlObserver
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    IMPORT_C ~CSGEGuiPageBase();

    IMPORT_C virtual void ActivateL();

    IMPORT_C virtual void DeactivateL();

    IMPORT_C TRect GetRedrawRect( TRect aRect ) const;
    
    IMPORT_C TUint Id() const;
    
protected:

    /**
     * Constructor for performing 1st stage construction
     */
    IMPORT_C CSGEGuiPageBase( TUint aPageId, TSize aSize );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    IMPORT_C void BaseConstructL();

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
    
protected: // from MGuiControlEventObserver

//    must be implemented in derived class:
//    void GuiControlEvent( CGuiControl *aControl, 
//                          MGuiControlEventObserver::TGuiControlEventType aType );
    
protected: // data
    
    CSGEGuiManagerControl *iControlManager; // owned

    TUint iPageId;
    
    TSize iSize;
    
    };

#endif // SGEGUIPAGEBASE_H
