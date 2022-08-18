/*
 ============================================================================
  Name        : $(baseName)Application.h
  Author      : $(author)
  Company     : $(company)
  Version     : $(version)
  Copyright   : $(copyright)
  Description : Declares main application class.
 ============================================================================
 */

#ifndef __$(baseNameUpper)APPLICATION_H__
#define __$(baseNameUpper)APPLICATION_H__

// INCLUDES
#include <SGEApplication/SGEGameApplicationVG.h>

// CONSTANTS
// UID of the application
const TUid KUid$(baseName) = { $(uid3) };

// CLASS DECLARATION
/**
 * C$(baseName)Application
 *
 */
class C$(baseName)Application : public CSGEGameApplicationVG
    {

public:

    C$(baseName)Application();

    ~C$(baseName)Application();

    void InitL();

    static CApaApplication* NewApplication();

    TUid ApplicationUid();

    void HandleCommandL( TInt aCommand );

    void Draw( const TRect& aRect ) const;

    TBool FullScreen() const;

    TInt SampleBuffers();

    TInt SampleSize();

    TInt DepthSize();

private:

    void DoGameLoopEvent( TTimeIntervalMicroSeconds32 aTimeInterval );

    TBool UpdateScene( TTimeIntervalMicroSeconds32 aTimeInterval );

    void DrawScene();

private: // data

    };

#endif // __$(baseNameUpper)APPLICATION_H__
