//=============================================================================
//
// Adventure Game Studio (AGS)
//
// Copyright (C) 1999-2011 Chris Jones and 2011-20xx others
// The full list of copyright holders can be found in the Copyright.txt
// file, which is part of this source code distribution.
//
// The AGS source code is provided under the Artistic License 2.0.
// A copy of this license can be found in the file License.txt and at
// http://www.opensource.org/licenses/artistic-license-2.0.php
//
//=============================================================================
//
// Basic software scaling filter
//
//=============================================================================
#ifndef __AC_ALLEGROGFXFILTER_H
#define __AC_ALLEGROGFXFILTER_H

#include "gfx/gfxfilter_scaling.h"

namespace AGS
{

namespace Common { class Bitmap; }

namespace Engine
{

using Common::Bitmap;

class AllegroGFXFilter : public ScalingGFXFilter
{
protected:
    // pointer to real screen bitmap
    Bitmap *realScreen;
    // bitmap the size of game resolution
    Bitmap *virtualScreen;
    // buffer for making a copy of video memory before stretching
    // for screen capture
    Bitmap *realScreenSizedBuffer;
    Bitmap *lastBlitFrom;

public:
    AllegroGFXFilter(bool justCheckingForSetup);
    AllegroGFXFilter(int multiplier, bool justCheckingForSetup);

    virtual Bitmap *ScreenInitialized(Common::Bitmap *screen, int virtualWidth, int virtualHeight, Placement placement);
    virtual Bitmap *ShutdownAndReturnRealScreen(Common::Bitmap *currentScreen);
    virtual void RenderScreen(Common::Bitmap *toRender, int x, int y);
    virtual void RenderScreenFlipped(Common::Bitmap *toRender, int x, int y, int flipType);
    virtual void ClearRect(int x1, int y1, int x2, int y2, int color);
    virtual void GetCopyOfScreenIntoBitmap(Common::Bitmap *copyBitmap);
    virtual void GetCopyOfScreenIntoBitmap(Common::Bitmap *copyBitmap, bool copyWithYOffset);

protected:
    virtual Bitmap *PreRenderPass(Bitmap *toRender, int x, int y);
};

} // namespace Engine
} // namespace AGS

#endif // __AC_ALLEGROGFXFILTER_H
