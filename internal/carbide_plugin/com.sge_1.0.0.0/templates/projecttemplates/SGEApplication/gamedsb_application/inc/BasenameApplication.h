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
#include <SGEApplication.h>

// CONSTANTS
// UID of the application
const TUid KUid$(baseName) = { $(uid3) };

// CLASS DECLARATION
/**
 * C$(baseName)Application
 *
 */
class C$(baseName)Application : public CSGEApplication
    {

public:

    /*
     * Application Uid getter
     * @return Application Uid
     */
    TUid ApplicationUid();

    /*
     * Command handler
     * @param aCommand Command identifier
     */
    void HandleCommandL( TInt aCommand );

    /*
     * Function used to launch application
     * @return Newly created system application object
     */
    static CApaApplication* NewApplication();

    /*
     * Draws application background
     * @param aRect drawing rectangle
     */
    void Draw( const TRect& aRect ) const;

    };

#endif // __$(baseNameUpper)APPLICATION_H__
