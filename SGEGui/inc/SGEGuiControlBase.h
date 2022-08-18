/*
 ============================================================================
 Name		: SGEGuiControlBase.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEGuiControlBase declaration
 ============================================================================
 */

#ifndef SGEGUICONTROLBASE_H
#define SGEGUICONTROLBASE_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <SGEEngine/SGEObjectInterfaces.h>

// FORWARD DECLARATIONS
class CSGEGuiControlBase;

// CLASS DECLARATION

/**
 *  MSGEGuiControlObserver
 * 
 */
class MSGEGuiControlObserver
    {
public:
    
    enum TSGEGuiControlEventType
        {
        EPushed,
        EUnpushed,
        EUnpushedOutside,
        EFocused
        };
    
    virtual void GuiControlEvent( CSGEGuiControlBase *aControl, 
                                  MSGEGuiControlObserver::TSGEGuiControlEventType aType,
                                  TPoint aPosition = TPoint() ) = 0;
    
    virtual void GuiControlFeedback() { };
    };
 
/**
 *  CSGEGuiControlBase
 * 
 */
class CSGEGuiControlBase : public CBase, public MSGEUpdatableObject, public MSGEDrawableObject
    {
protected:
    
    enum TSGEGuiControlState
        {
        ENormal,
        EPushed,
        EFocused
        };
    
public:

    /**
     * Destructor.
     */
    IMPORT_C ~CSGEGuiControlBase();

    IMPORT_C void SetObserver( MSGEGuiControlObserver *aObserver );
    
    IMPORT_C MSGEGuiControlObserver* Observer() const;
    
    IMPORT_C TBool Focused() const;

    IMPORT_C virtual void SetFocused( TBool aFocused );

    IMPORT_C virtual void SetFocused( TBool aFocused, TPoint aPosition );

    IMPORT_C TBool Pushed() const;

    IMPORT_C virtual void SetPushed( TBool aPushed );

    IMPORT_C virtual void SetPushed( TBool aFocused, TPoint aPosition );

    IMPORT_C TBool Hidden() const;

    IMPORT_C virtual void SetHidden( TBool aHidden );
    
    IMPORT_C TRect Rect() const;
    
    IMPORT_C void SetRect( TRect aRect );
    
    IMPORT_C TUint Id() const;
    
    IMPORT_C virtual CSGEGuiControlBase* GetControl( TUint aControlId ) const;

    IMPORT_C void SetPointerEventMargins( TInt aLeft, TInt aTop, TInt aRight, TInt aBottom );

    IMPORT_C TRect PointerEventMargins() const;
    
    IMPORT_C TBool CheckPointerEvent( TPoint aPosition );

    IMPORT_C virtual void SetNeedsRedraw( TBool aVal );

    IMPORT_C virtual TBool NeedsRedraw() const;
    
    IMPORT_C virtual TRect RedrawRect() const;
    
    IMPORT_C virtual void ResetState();

    IMPORT_C void SetMinimumPushedEventTime( TInt aMicroSeconds );
    
    IMPORT_C TInt MinimumPushedEventTime() const;
    
    IMPORT_C void InvokeFeedback();
    
protected:

    /**
     * Constructor for performing 1st stage construction
     */
    IMPORT_C CSGEGuiControlBase( TUint aControlId, TRect aControlRect );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    IMPORT_C void BaseConstructL();

public: // from MSGEUpdatableObject
    
    virtual TBool Update( const TTimeIntervalMicroSeconds32 aTimeIntercal ); 
    
public: // from MSGEDrawableObject
    
    //    must be implemented in derived class:
    // virtual void Draw( CBitmapContext* aGc, const TRect aDrawRect );

protected: // data
    
    MSGEGuiControlObserver *iObserver;
    
    TSGEGuiControlState iControlState;

    TBool iNeedsRedraw;

    TBool iHidden;

    TUint iControlId;
    
    TRect iControlRect;
    
    TRect iPointerEventMargin;

    TInt iMinimumPushedEventTime;
    
    };

#endif // SGEGUICONTROLBASE_H
