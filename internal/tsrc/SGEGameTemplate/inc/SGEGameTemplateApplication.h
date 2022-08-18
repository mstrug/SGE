/*
 ============================================================================
  Name        : SGEGameTemplateApplication.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description : 
 ============================================================================
 */

#ifndef SGEGAMETEMPLATEAPPLICATION_H_
#define SGEGAMETEMPLATEAPPLICATION_H_

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <SGEApplication/SGEGameApplication.h>
#include <SGEEngine/SGEEngineControllerObserver.h>

// FORWARD DECLARATIONS
class CApaApplication;
class CSGEGameTemplateEngineController;

// CLASS DECLARATION
/**
 * CSGEGameTemplateApplication
 * 
 */
class CSGEGameTemplateApplication : public CSGEGameApplication, MSGEEngineControllerObserver
    {
public:

    /**
     * Constructor.
     */    
    CSGEGameTemplateApplication();

    /**
     * Destructor.
     */
    ~CSGEGameTemplateApplication();

    /**
     * Returns application UID3.
     */
    TUid ApplicationUid();

    /**
     * Creates new application.
     */
    static CApaApplication* NewApplication();

    /**
     * Initializes application.
     */
    void InitL();

    /**
     * Draw function.
     */
    void Draw( const TRect& aRect ) const;

    /**
     * Foreground event handler.
     */
    void HandleForegroundEventL( TBool aForeground );

    /**
     * Key event handler.
     */
    TKeyResponse HandleKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );

    /**
     * Pointer event handler.
     */
    TBool HandlePointerEventL( const TPointerEvent& aPointerEvent );

    /**
     * Returns ETrue when application is full screen.
     */
    TBool FullScreen() const;

    /**
     * Called by framework when application size has changed.
     */
    void SizeChanged();
    
private:

    /**
     * Called each game loop event.
     */
    void DoGameLoopEvent( TTimeIntervalMicroSeconds32 aTimeInterval );
    
private: // from MSGEEngineControllerObserver

    void EngineControllerClose( TInt aError );

private: // data
    
    // owned
    CSGEGameTemplateEngineController *iEngineController;
    
    };

#endif // SGEGAMETEMPLATEAPPLICATION_H_
