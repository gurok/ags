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
#include "gfx/gfxfilter_allegro.h"

namespace AGS
{
namespace Engine
{

namespace BitmapHelper = Common::BitmapHelper;

AllegroGFXFilter::AllegroGFXFilter(bool justCheckingForSetup) : ScalingGFXFilter(1, justCheckingForSetup)
{
    lastBlitFrom = NULL;
    filterName = "";
    filterID = "None";
}

AllegroGFXFilter::AllegroGFXFilter(int multiplier, bool justCheckingForSetup) : ScalingGFXFilter(multiplier, justCheckingForSetup)
{
}

Bitmap* AllegroGFXFilter::ScreenInitialized(Bitmap *screen, int virtualWidth, int virtualHeight, Placement placement)
{
    realScreen = screen;

    Rect screen_frame = RectWH(0, 0, realScreen->GetWidth(), realScreen->GetHeight());
    TargetFrame = RectWH(0, 0, virtualWidth * BASE_MULTIPLIER, virtualHeight * BASE_MULTIPLIER);
    Common::Math::PlaceInRect(screen_frame, TargetFrame, placement);
    MULTIPLIER_X = (TargetFrame.GetWidth() << kMultiplierShift) / virtualWidth;
    MULTIPLIER_Y = (TargetFrame.GetHeight() << kMultiplierShift) / virtualHeight;
    RECT_CORRECTION_X = MULTIPLIER_X > kMultipliedUnit ? ((MULTIPLIER_X - kMultipliedUnit) >> kMultiplierShift) : 0;
    RECT_CORRECTION_Y = MULTIPLIER_Y > kMultipliedUnit ? ((MULTIPLIER_Y - kMultipliedUnit) >> kMultiplierShift) : 0;

    if (BASE_MULTIPLIER == 1 && screen->GetSize() == Size(virtualWidth, virtualHeight))
    {
        // Speed up software rendering if no scaling is performed
        realScreenSizedBuffer = NULL;
        virtualScreen = realScreen;
    }
    else
    {
        realScreenSizedBuffer = BitmapHelper::CreateBitmap(screen->GetWidth(), screen->GetHeight(), screen->GetColorDepth());
        virtualScreen = BitmapHelper::CreateBitmap(virtualWidth, virtualHeight, screen->GetColorDepth());
    }
    return virtualScreen;
}

Bitmap *AllegroGFXFilter::ShutdownAndReturnRealScreen(Bitmap *currentScreen)
{
    if (virtualScreen != realScreen)
    {
        delete virtualScreen;
    }
    delete realScreenSizedBuffer;
    virtualScreen = NULL;
    realScreenSizedBuffer = NULL;
    return realScreen;
}

void AllegroGFXFilter::RenderScreen(Bitmap *toRender, int x, int y) {

    if (toRender != realScreen)
    {
        x = ((x * MULTIPLIER_X) >> kMultiplierShift) + TargetFrame.Left;
        y = ((y * MULTIPLIER_Y) >> kMultiplierShift) + TargetFrame.Top;

        Bitmap *render_src = PreRenderPass(toRender, x, y);

        if (render_src->GetSize() == TargetFrame.GetSize())
        {
            realScreen->Blit(render_src, x, y);
        }
        else
        {
            realScreen->StretchBlt(render_src, RectWH(x, y, TargetFrame.GetWidth(), TargetFrame.GetHeight()));
        }
    }
    lastBlitFrom = toRender;
}

void AllegroGFXFilter::RenderScreenFlipped(Bitmap *toRender, int x, int y, int flipType)
{
    if (toRender == virtualScreen)
        return;

    switch (flipType)
    {
    case SCR_HFLIP:
		virtualScreen->FlipBlt(toRender, 0, 0, Common::kBitmap_HFlip);
        break;
    case SCR_VFLIP:
        virtualScreen->FlipBlt(toRender, 0, 0, Common::kBitmap_VFlip);
        break;
    case SCR_VHFLIP:
        virtualScreen->FlipBlt(toRender, 0, 0, Common::kBitmap_HVFlip);
        break;
    default:
        virtualScreen->Blit(toRender, 0, 0);
        break;
    }

    RenderScreen(virtualScreen, x, y);
}

void AllegroGFXFilter::ClearRect(int x1, int y1, int x2, int y2, int color)
{
    x1 = ((x1 * MULTIPLIER_X) >> kMultiplierShift) + TargetFrame.Left;
    y1 = ((y1 * MULTIPLIER_Y) >> kMultiplierShift) + TargetFrame.Top;
    x2 = ((x2 * MULTIPLIER_X) >> kMultiplierShift) + RECT_CORRECTION_X + TargetFrame.Left;
    y2 = ((y2 * MULTIPLIER_Y) >> kMultiplierShift) + RECT_CORRECTION_Y + TargetFrame.Top;
    realScreen->FillRect(Rect(x1, y1, x2, y2), color);
}

void AllegroGFXFilter::GetCopyOfScreenIntoBitmap(Bitmap *copyBitmap) 
{
    GetCopyOfScreenIntoBitmap(copyBitmap, true);
}

void AllegroGFXFilter::GetCopyOfScreenIntoBitmap(Bitmap *copyBitmap, bool copyWithYOffset)
{
    if (!copyWithYOffset)
    {
        if (copyBitmap->GetSize() == TargetFrame.GetSize())
        {
            copyBitmap->Blit(realScreen, TargetFrame.Left, TargetFrame.Top, 0, 0, TargetFrame.GetWidth(), TargetFrame.GetHeight());
        }
        else
        {
            // Can't stretch_blit from Video Memory to normal memory,
            // so copy the screen to a buffer first.
            realScreenSizedBuffer->Blit(realScreen, 0, 0);
            copyBitmap->StretchBlt(realScreenSizedBuffer,
                TargetFrame,
                RectWH(0, 0, copyBitmap->GetWidth(), copyBitmap->GetHeight()));
        }
    }
    else if (lastBlitFrom == NULL)
    {
        copyBitmap->Fill(0);
    }
    else
    {
        copyBitmap->StretchBlt(lastBlitFrom,
		    RectWH(0, 0, lastBlitFrom->GetWidth(), lastBlitFrom->GetHeight()), 
            RectWH(0, 0, copyBitmap->GetWidth(), copyBitmap->GetHeight()));
    }
}

Bitmap *AllegroGFXFilter::PreRenderPass(Bitmap *toRender, int x, int y)
{
    // do nothing
    return toRender;
}

} // namespace Engine
} // namespace AGS
