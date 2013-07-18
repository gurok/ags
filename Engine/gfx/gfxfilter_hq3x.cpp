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

#include "gfx/bitmap.h"
#include "gfx/gfxfilter_hq3x.h"
#include "gfx/hq2x3x.h"

namespace AGS
{
namespace Engine
{

namespace BitmapHelper = Common::BitmapHelper;

const char* Hq3xGFXFilter::Initialize(int width, int height, int colDepth)
{
    if (colDepth < 32)
        return "Only supports 32-bit colour games";

    return AllegroGFXFilter::Initialize(width, height, colDepth);
}

Bitmap* Hq3xGFXFilter::ScreenInitialized(Bitmap *screen, int virtualWidth, int virtualHeight, Placement placement)
{
    Bitmap *virtual_screen = AllegroGFXFilter::ScreenInitialized(screen, virtualWidth, virtualHeight, placement);
    hq3xScalingBuffer = BitmapHelper::CreateBitmap(virtualWidth * BASE_MULTIPLIER, virtualHeight * BASE_MULTIPLIER);
    InitLUTs();
    return virtual_screen;
}

Bitmap *Hq3xGFXFilter::ShutdownAndReturnRealScreen(Bitmap *currentScreen)
{
    Bitmap *real_screen = AllegroGFXFilter::ShutdownAndReturnRealScreen(currentScreen);
    delete hq3xScalingBuffer;
    hq3xScalingBuffer = NULL;
    return real_screen;
}

const char *Hq3xGFXFilter::GetVersionBoxText()
{
    return "Hq3x filter (32-bit only)[";
}

const char *Hq3xGFXFilter::GetFilterID()
{
    return "Hq3x";
}

Bitmap *Hq3xGFXFilter::PreRenderPass(Bitmap *toRender, int x, int y)
{
    hq3xScalingBuffer->Acquire();
    hq3x_32(toRender->GetDataForWriting(), hq3xScalingBuffer->GetDataForWriting(),
        toRender->GetWidth(), toRender->GetHeight(),
        hq3xScalingBuffer->GetWidth() * BYTES_PER_PIXEL(hq3xScalingBuffer->GetColorDepth()));
    hq3xScalingBuffer->Release();
    return hq3xScalingBuffer;
}

} // namespace Engine
} // namespace AGS
