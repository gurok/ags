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
// Graphics filter base class; does no processing
//
//=============================================================================
#ifndef __AC_GFXFILTER_H
#define __AC_GFXFILTER_H

#define SCR_HFLIP  1
#define SCR_VFLIP  2
#define SCR_VHFLIP 3

#define BYTES_PER_PIXEL(bpp)     (((int)(bpp) + 7) / 8)

namespace AGS
{
namespace Engine
{

class GFXFilter
{
public:
    virtual const char *Initialize(int width, int height, int colDepth);
    virtual void UnInitialize();
    // Multiplies values by corresponding X/Y factors
    virtual void GetRealResolution(int *wid, int *hit);
    // Set actual OS cursor position on screen; applies virtual->real
    // coordinates transformation
    virtual void SetMousePosition(int x, int y);
    // Limits the area beyond which the standard OS cursor is shown;
    // applies virtual->real coordinates transformation
    virtual void SetMouseArea(int x1, int y1, int x2, int y2);
    // Limits the area where the game cursor can move on virtual screen;
    // applies virtual->real coordinates transformation
    virtual void SetMouseLimit(int x1, int y1, int x2, int y2);
    virtual const char *GetVersionBoxText();
    virtual const char *GetFilterID();
    virtual ~GFXFilter();
};

} // namespace Engine
} // namespace AGS

AGS::Engine::GFXFilter **get_allegro_gfx_filter_list(bool checkingForSetup);
AGS::Engine::GFXFilter **get_d3d_gfx_filter_list(bool checkingForSetup);


extern AGS::Engine::GFXFilter *gfxFilter;

extern AGS::Engine::GFXFilter *gfxFilterList[11];
extern AGS::Engine::GFXFilter *gfxFilterListD3D[16];

#endif // __AC_GFXFILTER_H
