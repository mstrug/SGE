/*
 ============================================================================
 Name		: GlRttPbuffer.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CGlRttPbuffer implementation
 ============================================================================
 */

#include "GlRttPbuffer.h"
#include "GlObjectRect.h"
#include "GlPrimitive.h"
#include "GlVertexArray.h"
#include "GlUtils.h"
#include <SGEEGl/EglDevice.h>
#include <SGEEGl/EglRc.h>
#include <SGEEGl/EglSurface.h>
#include <SGEEGl/EglSurfacePbuffer.h>



CGlRttPbuffer::CGlRttPbuffer( CEglDevice& aDevice, CEglRc& aWindowRc, CEglSurface& aWindowSurface ) : iDevice( aDevice ), iWindowRc( aWindowRc ), iWindowSurface( aWindowSurface )
    {
    }

CGlRttPbuffer::~CGlRttPbuffer()
    {
    delete iPbuffer;
    iTexture.DeleteTexture();
    delete iRect;
    }

CGlRttPbuffer* CGlRttPbuffer::NewLC( CEglDevice& aDevice, CEglRc& aWindowRc, CEglSurface& aWindowSurface, TSize aSize, TBool aUseAlpha )
    {
    CGlRttPbuffer *self = new (ELeave) CGlRttPbuffer( aDevice, aWindowRc, aWindowSurface );
    CleanupStack::PushL( self );
    self->ConstructL( aSize, aUseAlpha );
    return self;
    }

CGlRttPbuffer* CGlRttPbuffer::NewL( CEglDevice& aDevice, CEglRc& aWindowRc, CEglSurface& aWindowSurface, TSize aSize, TBool aUseAlpha )
    {
    CGlRttPbuffer *self = CGlRttPbuffer::NewLC( aDevice, aWindowRc, aWindowSurface, aSize, aUseAlpha );
    CleanupStack::Pop(); // self;
    return self;
    }

void CGlRttPbuffer::ConstructL( TSize aSize, TBool aUseAlpha )
    {
    iTexture.CreateTexture( aSize, ( aUseAlpha ? EColor16MA : EColor16M ), NULL );
    iTexture.Use();

    iPbuffer = CEglSurfacePbuffer::NewL( iDevice, aSize, ETrue, aUseAlpha, EFalse, 0, iWindowRc.RC() );

    // initialize PBuffer
    // switch rendering context to PBuffer
    User::LeaveIfError( MakeCurrentPbufferContext() );

    GlUtils::SetSceneSizeAsScreen( aSize );

    // switch rendering context back to window
    User::LeaveIfError( MakeCurrentDefaultContext() );

    iTexture.Discard();

    const TInt KVertexCount = 4;
    const TInt KTexCoordSize = 2;
    GLfixed texCoords[ KVertexCount * KTexCoordSize ] =
        {
        0, ( 1 << 16 ),
        ( 1 << 16 ), ( 1 << 16 ),
        ( 1 << 16 ), 0,
        0, 0,
        };
    iRect = CGlObjectRect::NewL();
    iRect->Primitive().VertexArray( 0 )->SetTexCoordData( 0, (TAny*) texCoords, KVertexCount * KTexCoordSize * sizeof( GLfixed ) );
    }

TInt CGlRttPbuffer::MakeCurrentPbufferContext()
    {
    return iPbuffer->MakeCurrent();
    }

TInt CGlRttPbuffer::MakeCurrentDefaultContext()
    {
    return iWindowRc.MakeCurrent( iWindowSurface );
    }

MGlTexture::TGlTextureType CGlRttPbuffer::TextureType() const
    {
    return EGlTextureTypePbuffer;
    }
    
void CGlRttPbuffer::Use( GLenum aTextureUnitNumber )
    {
    iTexture.Use( aTextureUnitNumber );
    iPbuffer->BindToTexture();
    }
    
void CGlRttPbuffer::Discard()
    {
    iPbuffer->ReleaseFromTexture();
    iTexture.Discard();
    }

void CGlRttPbuffer::Draw()
    {
    Use();
    glPushMatrix();
    GlUtils::Scale2D( iTexture.Size() );
    iRect->Draw();
    glPopMatrix();
    Discard();
    }

TSize CGlRttPbuffer::Size() const
    {
    return iPbuffer->Size();
    }




