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

#include "gfx/gfxfilter_scaling.h"
#include "gfx/gfxfilterhelpers.h"

namespace AGS
{
namespace Engine
{

ScalingGFXFilter::ScalingGFXFilter(int multiplier, bool justCheckingForSetup) : GFXFilter()
{
    BASE_MULTIPLIER = multiplier;
    MULTIPLIER_X = BASE_MULTIPLIER << kMultiplierShift;
    MULTIPLIER_Y = BASE_MULTIPLIER << kMultiplierShift;
    RECT_CORRECTION_X = (MULTIPLIER_X - kMultipliedUnit) >> kMultiplierShift;
    RECT_CORRECTION_Y = (MULTIPLIER_Y - kMultipliedUnit) >> kMultiplierShift;
    mouseCallbackPtr = NULL;

    filterName.Format("%d" "x nearest-neighbour filter[", multiplier);
    filterID.Format("StdScale%d", multiplier);
}

const char* ScalingGFXFilter::Initialize(int width, int height, int colDepth)
{
    mouseCallbackPtr = new MouseGetPosCallbackImpl(this);
    msetcallback(mouseCallbackPtr);
    return NULL;
}

void ScalingGFXFilter::UnInitialize()
{
    msetcallback(NULL);
}

void ScalingGFXFilter::GetRealResolution(int *wid, int *hit)
{
    *wid = (*wid * MULTIPLIER_X) >> kMultiplierShift;
    *hit = (*hit * MULTIPLIER_Y) >> kMultiplierShift;
}

void ScalingGFXFilter::SetMouseArea(int x1, int y1, int x2, int y2)
{
    x1 = ((x1 * MULTIPLIER_X) >> kMultiplierShift) + TargetFrame.Left;
    y1 = ((y1 * MULTIPLIER_Y) >> kMultiplierShift) + TargetFrame.Top;
    x2 = ((x2 * MULTIPLIER_X) >> kMultiplierShift) + TargetFrame.Left;
    y2 = ((y2 * MULTIPLIER_Y) >> kMultiplierShift) + TargetFrame.Top;
    mgraphconfine(x1, y1, x2, y2);
}

void ScalingGFXFilter::SetMouseLimit(int x1, int y1, int x2, int y2)
{
    x1 = ((x1 * MULTIPLIER_X) >> kMultiplierShift) + RECT_CORRECTION_X + TargetFrame.Left;
    y1 = ((y1 * MULTIPLIER_Y) >> kMultiplierShift) + RECT_CORRECTION_Y + TargetFrame.Top;
    x2 = ((x2 * MULTIPLIER_X) >> kMultiplierShift) + RECT_CORRECTION_X + TargetFrame.Left;
    y2 = ((y2 * MULTIPLIER_Y) >> kMultiplierShift) + RECT_CORRECTION_Y + TargetFrame.Top;
    msetcursorlimit(x1, y1, x2, y2);
}

void ScalingGFXFilter::SetMousePosition(int x, int y)
{
    msetgraphpos(((x * MULTIPLIER_X) >> kMultiplierShift) + TargetFrame.Left, ((y * MULTIPLIER_Y) >> kMultiplierShift) + TargetFrame.Top);
}

void ScalingGFXFilter::AdjustPosition(int *x, int *y)
{
    *x = ((*x - TargetFrame.Left) << kMultiplierShift) / MULTIPLIER_X;
    *y = ((*y - TargetFrame.Top)  << kMultiplierShift) / MULTIPLIER_Y;
}

const char *ScalingGFXFilter::GetVersionBoxText()
{
    return filterName;
}

const char *ScalingGFXFilter::GetFilterID()
{
    return filterID;
}

ScalingGFXFilter::~ScalingGFXFilter()
{
    if (mouseCallbackPtr != NULL)
    {
        delete mouseCallbackPtr;
        mouseCallbackPtr = NULL;
    }
}

} // namespace Engine
} // namespace AGS
