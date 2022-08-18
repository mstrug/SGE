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
// C$(baseName)Application::NewApplication()
// -----------------------------------------------------------------------------
//
CApaApplication* C$(baseName)Application::NewApplication()
    {
    return CSGEApplication::NewApplication( new C$(baseName)Application );
    }

// -----------------------------------------------------------------------------
// C$(baseName)Application::Draw()
// -----------------------------------------------------------------------------
//
void C$(baseName)Application::Draw( const TRect& aRect ) const
    {
    // Get the standard graphics context
    CWindowGc& gc = SystemGc();

    // Gets the control's extent
    TRect drawRect( aRect );

    // Clears the screen
    gc.Clear(drawRect);
    }


// End of file
