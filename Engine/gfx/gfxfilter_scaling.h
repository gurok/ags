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
// Base class for graphic filter which provides virtual screen scaling
//
//=============================================================================
#ifndef __AC_SCALINGGFXFILTER_H
#define __AC_SCALINGGFXFILTER_H

#include "gfx/gfxfilter.h"
#include "util/geometry.h"
#include "util/string.h"

namespace AGS
{
namespace Engine
{

using Common::String;
struct MouseGetPosCallbackImpl;

class ScalingGFXFilter : public GFXFilter
{
protected:
    enum
    {
        kMultiplierShift    = 10,
        kMultipliedUnit     = 1 << kMultiplierShift
    };

    unsigned BASE_MULTIPLIER;
    unsigned MULTIPLIER_X;
    unsigned MULTIPLIER_Y;
    unsigned RECT_CORRECTION_X;
    unsigned RECT_CORRECTION_Y;
    Rect TargetFrame;
    MouseGetPosCallbackImpl *mouseCallbackPtr;

    String filterName;
    String filterID;

    ScalingGFXFilter(int multiplier, bool justCheckingForSetup);

public:
    virtual const char* Initialize(int width, int height, int colDepth);
    virtual void UnInitialize();
    virtual void GetRealResolution(int *wid, int *hit);
    virtual Rect GetTargetFrame() { return TargetFrame; }
    virtual void SetMouseArea(int x1, int y1, int x2, int y2);
    virtual void SetMouseLimit(int x1, int y1, int x2, int y2);
    virtual void SetMousePosition(int x, int y);
    // Applies real->virtual coordinate transformation
    // (i.e. tells which in-game coords correspond to given real screen coords)
    virtual void AdjustPosition(int *x, int *y);
    virtual const char *GetVersionBoxText();
    virtual const char *GetFilterID();
    virtual ~ScalingGFXFilter();
};

} // namespace Engine
} // namespace AGS

#endif // __AC_SCALINGGFXFILTER_H
