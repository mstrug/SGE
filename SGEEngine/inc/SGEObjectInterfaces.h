/*
 ============================================================================
  Name        : SGEObjectInterfaces.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#ifndef __SGEOBJECTINTERFACES_H__
#define __SGEOBJECTINTERFACES_H__

// INCLUDES
#include <e32std.h>
#include <W32STD.H>
#include <GDI.H> 


// CLASS DECLARATIONS

/**
 * MSGEDrawableObject
 * Interface for object which needs to be drawn.
 */
class MSGEDrawableObject
    {
public:
    virtual void Draw( CBitmapContext* aGc, const TRect aDrawRect ) = 0; 
    };

/**
 * MSGEGlDrawableObject
 * Interface for object which needs to be drawn on Open GL canvas.
 */
class MSGEGlDrawableObject
    {
public:
    virtual void Draw( const TRect aDrawRect ) = 0; 
    };

/**
 * MSGEUpdatableObject
 * Interface for object which needs to be updated in each game loop event.
 */
class MSGEUpdatableObject
    {
public:
    // @return ETrue if object was updated
    virtual TBool Update( const TTimeIntervalMicroSeconds32 aTimeInterval ) = 0; 
    };

/**
 * MSGEKeyEventableObject
 * Interface for object which needs to get key events.
 */
class MSGEKeyEventableObject
    {
public:
    // @return ETrue if key was processed
    virtual TBool KeyDownEvent( const TKeyEvent &aKeyEvent ) = 0; 

    // @return ETrue if key was processed
    virtual TBool KeyUpEvent( const TKeyEvent &aKeyEvent ) = 0; 

    // @return ETrue if key was processed
    virtual TBool KeyEvent( const TKeyEvent &aKeyEvent ) = 0;
    };

/**
 * MSGEPointerEventableObject
 * Interface for object which needs to get pointer events.
 */
class MSGEPointerEventableObject
    {
public:
    // @return ETrue if event was processed
    virtual TBool PointerEvent( const TPointerEvent &aPointerEvent ) = 0; 
    };


#endif // __SGEOBJECTINTERFACES_H__
