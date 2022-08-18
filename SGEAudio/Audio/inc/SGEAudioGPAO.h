/*
 ============================================================================
 Name		: SGEAudioGPAO.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CSGEAudioGPAO declaration
 ============================================================================
 */

#ifndef SGEAUDIOGPAO_H
#define SGEAUDIOGPAO_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

// CLASS DECLARATION

class CSGEAudioGPAO;

/**
 *  MSGEAudioGPAOObserver
 */
class MSGEAudioGPAOObserver
    {
public:
    
    virtual void GPAONotifierCancel( CSGEAudioGPAO *aGPAO ) = 0;
    
    virtual void GPAONotifierL( CSGEAudioGPAO *aGPAO, TInt aStatus ) = 0;
    
    virtual TInt GPAONotifierError( CSGEAudioGPAO *aGPAO, TInt aError ) = 0;

    };

/**
 *  CSGEAudioGPAO
 * 
 * @brief General purpose active object
 * 
 */
NONSHARABLE_CLASS( CSGEAudioGPAO ) : public CActive
    {
public:
    // Constructors and destructor

    /**
     * Constructor for performing 1st stage construction
     */
    CSGEAudioGPAO( MSGEAudioGPAOObserver &aObserver, TInt aPriority = EPriorityStandard );

    /**
     * Destructor.
     */
    ~CSGEAudioGPAO();

    void SetActive();
    
private: // from CActive
    
    void DoCancel();
    
    void RunL();
    
    TInt RunError( TInt aError );

private: // data
    
    MSGEAudioGPAOObserver &iObserver;
    
    };

#endif // SGEAUDIOGPAO_H
