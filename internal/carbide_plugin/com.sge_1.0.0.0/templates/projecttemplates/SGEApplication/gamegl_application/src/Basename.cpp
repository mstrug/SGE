/*
 ============================================================================
  Name        : $(baseName).cpp
  Author      : $(author)
  Company     : $(company)
  Version     : $(version)
  Copyright   : $(copyright)
  Description : 
 ============================================================================
 */

// INCLUDE FILES
#include "$(baseName)Application.h"

// -----------------------------------------------------------------------------
// E32Main()
// -----------------------------------------------------------------------------
//
GLDEF_C TInt E32Main()
    {
    return C$(baseName)Application::Start( C$(baseName)Application::NewApplication );
    }

// End of file
