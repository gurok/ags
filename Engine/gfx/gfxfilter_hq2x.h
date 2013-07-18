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
//
//
//=============================================================================
#ifndef __AC_HQ2XGFXFILTER_H
#define __AC_HQ2XGFXFILTER_H

#include "gfx/gfxfilter_allegro.h"

namespace AGS
{
namespace Engine
{

class Hq2xGFXFilter : public AllegroGFXFilter
{
private:
    Common::Bitmap *hq2xScalingBuffer;

public:
    Hq2xGFXFilter(bool justCheckingForSetup) : AllegroGFXFilter(2, justCheckingForSetup) { }

    virtual const char* Initialize(int width, int height, int colDepth);
    virtual Common::Bitmap *ScreenInitialized(Common::Bitmap *screen, int virtualWidth, int virtualHeight, Placement placement);
    virtual Common::Bitmap *ShutdownAndReturnRealScreen(Common::Bitmap *currentScreen);
    virtual const char *GetVersionBoxText();
    virtual const char *GetFilterID();

protected:
    virtual Bitmap *PreRenderPass(Bitmap *toRender, int x, int y);
};

} // namespace Engine
} // namespace AGS

#endif // __AC_HQ2XGFXFILTER_H
