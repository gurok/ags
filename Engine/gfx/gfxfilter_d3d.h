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
// Standard 3D-accelerated filter
//
//=============================================================================
#ifndef __AC_D3DGFXFILTER_H
#define __AC_D3DGFXFILTER_H

#include "gfx/gfxfilter_scaling.h"

namespace AGS
{
namespace Engine
{

class D3DGFXFilter : public ScalingGFXFilter
{
public:
    D3DGFXFilter(bool justCheckingForSetup);
    D3DGFXFilter(int multiplier, bool justCheckingForSetup);

    virtual void InitTargetFrame(int realWidth, int realHeight, int virtualWidth, int virtualHeight, Placement placement);
    virtual void SetSamplerStateForStandardSprite(void *direct3ddevice9);
    virtual bool NeedToColourEdgeLines();
};

} // namespace Engine
} // namespace AGS

AGS::Engine::GFXFilter **get_allegro_gfx_filter_list(bool checkingForSetup);
AGS::Engine::GFXFilter **get_d3d_gfx_filter_list(bool checkingForSetup);

#endif // __AC_D3DGFXFILTER_H
