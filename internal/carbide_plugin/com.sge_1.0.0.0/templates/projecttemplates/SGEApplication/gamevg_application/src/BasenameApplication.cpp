/*
 ============================================================================
  Name        : $(baseName)Application.cpp
  Author      : $(author)
  Company     : $(company)
  Version     : $(version)
  Copyright   : $(copyright)
  Description : Main application class
 ============================================================================
 */

// INCLUDE FILES
#include "$(baseName)Application.h"
#include <VG/vgu.h>
#include <VG/openvg.h>


// -----------------------------------------------------------------------------
// C$(baseName)Application::C$(baseName)Application()
// -----------------------------------------------------------------------------
//
C$(baseName)Application::C$(baseName)Application()
    {
    }

// -----------------------------------------------------------------------------
// C$(baseName)Application::~C$(baseName)Application()
// -----------------------------------------------------------------------------
//
C$(baseName)Application::~C$(baseName)Application()
    {
    }

// -----------------------------------------------------------------------------
// C$(baseName)Application::InitL()
// -----------------------------------------------------------------------------
//
void C$(baseName)Application::InitL()
    {
    CSGEGameApplicationVG::InitL();
    }

// -----------------------------------------------------------------------------
// C$(baseName)Application::NewApplication()
// -----------------------------------------------------------------------------
//
CApaApplication* C$(baseName)Application::NewApplication()
    {
    return CSGEGameApplicationVG::NewApplication( new C$(baseName)Application );
    }

// -----------------------------------------------------------------------------
// C$(baseName)Application::ApplicationUid()
// -----------------------------------------------------------------------------
//
TUid C$(baseName)Application::ApplicationUid()
    {
    return KUid$(baseName);
    }

// -----------------------------------------------------------------------------
// C$(baseName)Application::HandleCommandL()
// -----------------------------------------------------------------------------
//
void C$(baseName)Application::HandleCommandL( TInt /*aCommand*/ )
    {
    Exit();
    }

// -----------------------------------------------------------------------------
// C$(baseName)Application::Draw()
// -----------------------------------------------------------------------------
//
void C$(baseName)Application::Draw( const TRect& /*aRect*/ ) const
    {
    const_cast< C$(baseName)Application* >( this )->DrawScene();
    }

// -----------------------------------------------------------------------------
// C$(baseName)Application::FullScreen()
// -----------------------------------------------------------------------------
//
TBool C$(baseName)Application::FullScreen() const
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// C$(baseName)Application::SampleBuffers()
// -----------------------------------------------------------------------------
//
TInt C$(baseName)Application::SampleBuffers()
    {
    return 0;
    }

// -----------------------------------------------------------------------------
// C$(baseName)Application::SampleSize()
// -----------------------------------------------------------------------------
//
TInt C$(baseName)Application::SampleSize()
    {
    return 0;
    }

// -----------------------------------------------------------------------------
// C$(baseName)Application::DepthSize()
// -----------------------------------------------------------------------------
//
TInt C$(baseName)Application::DepthSize()
    {
    return 0;
    }

// -----------------------------------------------------------------------------
// C$(baseName)Application::DoGameLoopEvent()
// -----------------------------------------------------------------------------
//
void C$(baseName)Application::DoGameLoopEvent( TTimeIntervalMicroSeconds32 aTimeInterval )
    {
    if ( UpdateScene( aTimeInterval ) )
        {
        DrawScene();
        }
    }

// -----------------------------------------------------------------------------
// C$(baseName)Application::UpdateScene()
// -----------------------------------------------------------------------------
//
TBool C$(baseName)Application::UpdateScene( TTimeIntervalMicroSeconds32 /*aTimeInterval*/ )
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// C$(baseName)Application::DrawScene()
// -----------------------------------------------------------------------------
//
void C$(baseName)Application::DrawScene()
    {
    }



// End of file
