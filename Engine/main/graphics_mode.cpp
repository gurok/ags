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
// Graphics initialization
//

#include "main/mainheader.h"
#include "gfx/ali3d.h"
#include "ac/common.h"
#include "ac/display.h"
#include "ac/draw.h"
#include "ac/gamesetup.h"
#include "ac/gamesetupstruct.h"
#include "ac/walkbehind.h"
#include "debug/debug_log.h"
#include "debug/debugger.h"
#include "debug/out.h"
#include "font/fonts.h"
#include "gui/guiinv.h"
#include "gui/guimain.h"
#include "main/graphics_mode.h"
#include "platform/base/agsplatformdriver.h"
#include "gfx/graphicsdriver.h"
#include "gfx/bitmap.h"

using AGS::Common::Bitmap;
namespace BitmapHelper = AGS::Common::BitmapHelper;
namespace Out = AGS::Common::Out;

extern GameSetup usetup;
extern GameSetupStruct game;
extern int proper_exit;
extern GUIMain*guis;
extern int psp_gfx_renderer; // defined in ali3dogl
extern WalkBehindMethodEnum walkBehindMethod;
extern DynamicArray<GUIInv> guiinv;
extern int numguiinv;
extern int current_screen_resolution_multiplier;
extern char force_gfxfilter[50];
extern AGSPlatformDriver *platform;
extern int force_16bit;
extern IGraphicsDriver *gfxDriver;
extern volatile int timerloop;
extern IDriverDependantBitmap *blankImage;
extern IDriverDependantBitmap *blankSidebarImage;
extern Bitmap *_old_screen;
extern int _places_r, _places_g, _places_b;

struct ColorDepthOption
{
    int32_t First;
    int32_t Second;

    ColorDepthOption()
        : First(0)
        , Second(0)
    {
    }

    ColorDepthOption(int32_t first, int32_t second)
        : First(first)
        , Second(second)
    {
    }
};

Size              GameSize;
DisplayResolution GameResolution;

// set to 0 once successful
int working_gfx_mode_status = -1;
int debug_15bit_mode = 0, debug_24bit_mode = 0;
int convert_16bit_bgr = 0;

int ff; // whatever!

int adjust_pixel_size_for_loaded_data(int size, int filever)
{
    if (filever < kGameVersion_300)
    {
        return multiply_up_coordinate(size);
    }
    return size;
}

void adjust_pixel_sizes_for_loaded_data(int *x, int *y, int filever)
{
    x[0] = adjust_pixel_size_for_loaded_data(x[0], filever);
    y[0] = adjust_pixel_size_for_loaded_data(y[0], filever);
}

void adjust_sizes_for_resolution(int filever)
{
    int ee;
    for (ee = 0; ee < game.numcursors; ee++) 
    {
        game.mcurs[ee].hotx = adjust_pixel_size_for_loaded_data(game.mcurs[ee].hotx, filever);
        game.mcurs[ee].hoty = adjust_pixel_size_for_loaded_data(game.mcurs[ee].hoty, filever);
    }

    for (ee = 0; ee < game.numinvitems; ee++) 
    {
        adjust_pixel_sizes_for_loaded_data(&game.invinfo[ee].hotx, &game.invinfo[ee].hoty, filever);
    }

    for (ee = 0; ee < game.numgui; ee++) 
    {
        GUIMain*cgp=&guis[ee];
        adjust_pixel_sizes_for_loaded_data(&cgp->x, &cgp->y, filever);
        if (cgp->wid < 1)
            cgp->wid = 1;
        if (cgp->hit < 1)
            cgp->hit = 1;
        // Temp fix for older games
        if (cgp->wid == usetup.base_width - 1)
            cgp->wid = usetup.base_width;

        adjust_pixel_sizes_for_loaded_data(&cgp->wid, &cgp->hit, filever);

        cgp->popupyp = adjust_pixel_size_for_loaded_data(cgp->popupyp, filever);

        for (ff = 0; ff < cgp->numobjs; ff++) 
        {
            adjust_pixel_sizes_for_loaded_data(&cgp->objs[ff]->x, &cgp->objs[ff]->y, filever);
            adjust_pixel_sizes_for_loaded_data(&cgp->objs[ff]->wid, &cgp->objs[ff]->hit, filever);
            cgp->objs[ff]->activated=0;
        }
    }

    if ((filever >= 37) && (game.options[OPT_NATIVECOORDINATES] == 0) &&
        (game.IsHiRes()))
    {
        // New 3.1 format game file, but with Use Native Coordinates off

        for (ee = 0; ee < game.numcharacters; ee++) 
        {
            game.chars[ee].x /= 2;
            game.chars[ee].y /= 2;
        }

        for (ee = 0; ee < numguiinv; ee++)
        {
            guiinv[ee].itemWidth /= 2;
            guiinv[ee].itemHeight /= 2;
        }
    }

}

void engine_init_screen_settings(ColorDepthOption &color_depths)
{
    Out::FPrint("Initializing screen settings");

    // default shifts for how we store the sprite data

#if defined(PSP_VERSION)
    // PSP: Switch b<>r for 15/16 bit.
    _rgb_r_shift_32 = 16;
    _rgb_g_shift_32 = 8;
    _rgb_b_shift_32 = 0;
    _rgb_b_shift_16 = 11;
    _rgb_g_shift_16 = 5;
    _rgb_r_shift_16 = 0;
    _rgb_b_shift_15 = 10;
    _rgb_g_shift_15 = 5;
    _rgb_r_shift_15 = 0;
#else
    _rgb_r_shift_32 = 16;
    _rgb_g_shift_32 = 8;
    _rgb_b_shift_32 = 0;
    _rgb_r_shift_16 = 11;
    _rgb_g_shift_16 = 5;
    _rgb_b_shift_16 = 0;
    _rgb_r_shift_15 = 10;
    _rgb_g_shift_15 = 5;
    _rgb_b_shift_15 = 0;
#endif

    usetup.base_width = 320;
    usetup.base_height = 200;

    GameResolutionType game_res = game.GetDefaultResolution();
    switch (game_res)
    {
    case kGameResolution_800x600:
    case kGameResolution_1024x768:
        if (game_res >= kGameResolution_1024x768)
        {
            // 1024x768
            usetup.base_width = 512;
            usetup.base_height = 384;
        }
        else
        {
            // 800x600
            usetup.base_width = 400;
            usetup.base_height = 300;
        }
        GameSize.Width = usetup.base_width * 2;
        GameSize.Height = usetup.base_height * 2;
        wtext_multiply = 2;
        break;
    case kGameResolution_640x480:
        GameSize.Width = 640;
        GameSize.Height = 480;
        wtext_multiply = 2;
        break;
    case kGameResolution_640x400:
        GameSize.Width = 640;
        GameSize.Height = 400;
        wtext_multiply = 2;
        break;
    case kGameResolution_320x240:
        GameSize.Width = 320;
        GameSize.Height = 240;
        wtext_multiply = 1;
        break;
    case kGameResolution_320x200:
        GameSize.Width = 320;
        GameSize.Height = 200;
        wtext_multiply = 1;
        break;
    default:
        GameSize.Width = usetup.base_width;
        GameSize.Height = usetup.base_height;
        wtext_multiply = 1;
    }

    usetup.textheight = wgetfontheight(0) + 1;
    current_screen_resolution_multiplier = GameSize.Width / usetup.base_width;

    if ((game.IsHiRes()) &&
        (game.options[OPT_NATIVECOORDINATES]))
    {
        usetup.base_width *= 2;
        usetup.base_height *= 2;
    }

    // don't allow them to force a 256-col game to hi-color
    if (game.color_depth < 2)
        usetup.force_hicolor_mode = 0;

    color_depths.First = 8;
    color_depths.Second = 8;
    if ((game.color_depth == 2) || (force_16bit) || (usetup.force_hicolor_mode)) {
        color_depths.First = 16;
        color_depths.Second = 15;
    }
    else if (game.color_depth > 2) {
        color_depths.First = 32;
        color_depths.Second = 24;
    }

    adjust_sizes_for_resolution(loaded_game_file_version);
}

int initialize_graphics_filter(const char *filterID, const int colDepth)
{
    int idx = 0;
    GFXFilter **filterList;

    if (stricmp(usetup.gfxDriverID, "D3D9") == 0)
    {
        filterList = get_d3d_gfx_filter_list(false);
    }
    else
    {
        filterList = get_allegro_gfx_filter_list(false);
    }

    // by default, select No Filter
    gfxFilter = filterList[0];

    GFXFilter *thisFilter = filterList[idx];
    while (thisFilter != NULL) {

        if ((filterID != NULL) &&
            (strcmp(thisFilter->GetFilterID(), filterID) == 0))
            gfxFilter = thisFilter;
        else if (idx > 0)
            delete thisFilter;

        idx++;
        thisFilter = filterList[idx];
    }

    const char *filterError = gfxFilter->Initialize(GameSize.Width, GameSize.Height, colDepth);
    if (filterError != NULL) {
        proper_exit = 1;
        platform->DisplayAlert("Unable to initialize the graphics filter. It returned the following error:\n'%s'\n\nTry running Setup and selecting a different graphics filter.", filterError);
        return -1;
    }

    return 0;
}

int engine_init_gfx_filters(int color_depth)
{
    Out::FPrint("Init gfx filters");

    char *gfxfilter = NULL;

    if (force_gfxfilter[0]) {
        gfxfilter = force_gfxfilter;
    }
    else if (usetup.gfxFilterID) {
        gfxfilter = usetup.gfxFilterID;
    }
#if defined (WINDOWS_VERSION) || defined (LINUX_VERSION)
    else {
        int desktopWidth, desktopHeight;
        if (get_desktop_resolution(&desktopWidth, &desktopHeight) == 0)
        {
            if (usetup.windowed > 0)
                desktopHeight -= 100;

            int xratio = desktopWidth / GameSize.Width;
            int yratio = desktopHeight / GameSize.Height;
            int min_ratio = xratio < yratio ? xratio : yratio;

            if (min_ratio > 1)
            {
                if (min_ratio > 8)
                    min_ratio = 8;
                char filterID[12];
                sprintf(filterID, "StdScale%d", min_ratio);
                gfxfilter = filterID;
            }
        }
        else
        {
            Out::FPrint("Automatic scaling: disabled (unable to obtain desktop resolution)");
        }
    }
#endif

    if (initialize_graphics_filter(gfxfilter, color_depth))
    {
        return EXIT_NORMAL;
    }

    return RETURN_CONTINUE;
}

void create_gfx_driver() 
{
#ifdef WINDOWS_VERSION
    if (stricmp(usetup.gfxDriverID, "D3D9") == 0)
        gfxDriver = GetD3DGraphicsDriver(gfxFilter);
    else
#endif
    {
#if defined(IOS_VERSION) || defined(ANDROID_VERSION) || defined(WINDOWS_VERSION)
        if ((psp_gfx_renderer > 0) && (game.color_depth != 1))
            gfxDriver = GetOGLGraphicsDriver(gfxFilter);
        else
#endif
            gfxDriver = GetSoftwareGraphicsDriver(gfxFilter);
    }

    gfxDriver->SetCallbackOnInit(GfxDriverOnInitCallback);
    gfxDriver->SetTintMethod(TintReColourise);
}

int init_gfx_mode(const int width, const int height, const int color_depth) {

    // a mode has already been initialized, so abort
    if (working_gfx_mode_status == 0) return 0;

    GameResolution.Width = width;
    GameResolution.Height = height;
    GameResolution.ColorDepth = color_depth;

    if (debug_15bit_mode)
        GameResolution.ColorDepth = 15;
    else if (debug_24bit_mode)
        GameResolution.ColorDepth = 24;

    Out::FPrint("Attempt to switch gfx mode to %d x %d (%d-bit)", GameResolution.Width, GameResolution.Height, GameResolution.ColorDepth);

    if (usetup.refresh >= 50)
        request_refresh_rate(usetup.refresh);

    if (game.color_depth == 1) {
        GameResolution.ColorDepth = 8;
    }
    else {
        set_color_depth(GameResolution.ColorDepth);
    }

    working_gfx_mode_status =
        (gfxDriver->Init(GameSize.Width, GameSize.Height, GameResolution.Width, GameResolution.Height, kPlaceStretch,
                         GameResolution.ColorDepth, usetup.windowed > 0, &timerloop) ? 0 : -1);

    if (working_gfx_mode_status == 0) 
        Out::FPrint("Succeeded. Using gfx mode %d x %d (%d-bit)", GameResolution.Width, GameResolution.Height, GameResolution.ColorDepth);
    else
        Out::FPrint("Failed, resolution not supported");

    if ((working_gfx_mode_status < 0) && (usetup.windowed > 0) && (editor_debugging_enabled == 0)) {
        usetup.windowed ++;
        if (usetup.windowed > 2) usetup.windowed = 0;
        return init_gfx_mode(width, height, color_depth);
    }
    return working_gfx_mode_status;    
}

int try_widescreen_bordered_graphics_mode_if_appropriate(const int width, const int height, const int color_depth)
{
    if (working_gfx_mode_status == 0) return 0;
    if (usetup.enable_side_borders == 0)
    {
        Out::FPrint("Widescreen side borders: disabled in Setup");
        return 1;
    }
    if (usetup.windowed > 0)
    {
        Out::FPrint("Widescreen side borders: disabled (windowed mode)");
        return 1;
    }

    int failed = 1;
    int desktopWidth, desktopHeight;
    if (get_desktop_resolution(&desktopWidth, &desktopHeight) == 0)
    {
        int screenRatio = (desktopWidth * 1000) / desktopHeight;
        int gameRatio = (width * 1000) / height;
        // 1250 = 1280x1024 
        // 1333 = 640x480, 800x600, 1024x768, 1152x864, 1280x960
        // 1600 = 640x400, 960x600, 1280x800, 1680x1050
        // 1666 = 1280x768

        Out::FPrint("Widescreen side borders: game resolution: %d x %d; desktop resolution: %d x %d", width, height, desktopWidth, desktopHeight);

        if ((screenRatio > 1500) && (gameRatio < 1500))
        {
            int tryWidth = (width * screenRatio) / gameRatio;
            int supportedRes = gfxDriver->FindSupportedResolutionWidth(tryWidth, height, color_depth, 110);
            if (supportedRes > 0)
            {
                tryWidth = supportedRes;
                Out::FPrint("Widescreen side borders: enabled, attempting resolution %d x %d", tryWidth, height);
            }
            else
            {
                Out::FPrint("Widescreen side borders: gfx card does not support suitable resolution. will attempt %d x %d anyway", tryWidth, height);
            }
            failed = init_gfx_mode(tryWidth, height, color_depth);
        }
        else
        {
            Out::FPrint("Widescreen side borders: disabled (not necessary, game and desktop aspect ratios match)", width, height, desktopWidth, desktopHeight);
        }
    }
    else 
    {
        Out::FPrint("Widescreen side borders: disabled (unable to obtain desktop resolution)");
    }
    return failed;
}

int switch_to_graphics_mode(const ColorDepthOption color_depths) 
{
    int failed;
    int initasyLetterbox = (GameSize.Height * 12) / 10;

    // first of all, try 16-bit normal then letterboxed
    failed = try_widescreen_bordered_graphics_mode_if_appropriate(GameSize.Width, GameSize.Height, color_depths.First);
    failed = init_gfx_mode(GameSize.Width, GameSize.Height, color_depths.First);
    failed = try_widescreen_bordered_graphics_mode_if_appropriate(GameSize.Width, initasyLetterbox, color_depths.First);
    failed = init_gfx_mode(GameSize.Width, initasyLetterbox, color_depths.First);

    if (color_depths.Second != color_depths.First) {
        // now, try 15-bit normal then letterboxed
        failed = try_widescreen_bordered_graphics_mode_if_appropriate(GameSize.Width, GameSize.Height, color_depths.Second);
        failed = init_gfx_mode(GameSize.Width, GameSize.Height, color_depths.Second);
        failed = try_widescreen_bordered_graphics_mode_if_appropriate(GameSize.Width, initasyLetterbox, color_depths.Second);
        failed = init_gfx_mode(GameSize.Width, initasyLetterbox, color_depths.Second);
    }

    if (failed)
        return -1;

    return 0;
}

void engine_init_gfx_driver()
{
    Out::FPrint("Init gfx driver");

    create_gfx_driver();
}

int engine_init_graphics_mode(const ColorDepthOption color_depths)
{
    Out::FPrint("Switching to graphics mode");

    if (switch_to_graphics_mode(color_depths))
    {
        proper_exit=1;
        platform->FinishedUsingGraphicsMode();

        // make sure the error message displays the true resolution
        int game_width = GameSize.Width;
        int game_height = GameSize.Height;

        if (gfxFilter != NULL)
            gfxFilter->GetRealResolution(&game_width, &game_height);

        platform->DisplayAlert("There was a problem initializing graphics mode %d x %d (%d-bit).\n"
            "(Problem: '%s')\n"
            "Try to correct the problem, or seek help from the AGS homepage.\n"
            "\nPossible causes:\n* your graphics card drivers do not support this resolution. "
            "Run the game setup program and try the other resolution.\n"
            "* the graphics driver you have selected does not work. Try switching between Direct3D and DirectDraw.\n"
            "* the graphics filter you have selected does not work. Try another filter.",
            game_width, game_height, color_depths.First, allegro_error);
        return EXIT_NORMAL;
    }

    return RETURN_CONTINUE;
}

void CreateBlankImage()
{
    // this is the first time that we try to use the graphics driver,
    // so it's the most likey place for a crash
    try
    {
        Bitmap *blank = BitmapHelper::CreateBitmap(16, 16, GameResolution.ColorDepth);
        blank = gfxDriver->ConvertBitmapToSupportedColourDepth(blank);
        blank->Clear();
        blankImage = gfxDriver->CreateDDBFromBitmap(blank, false, true);
        blankSidebarImage = gfxDriver->CreateDDBFromBitmap(blank, false, true);
        delete blank;
    }
    catch (Ali3DException gfxException)
    {
        quit((char*)gfxException._message);
    }

}

void engine_post_init_gfx_driver()
{
    //screen = _filter->ScreenInitialized(screen, GameResolution.Width, GameResolution.Height);
	_old_screen = BitmapHelper::GetScreenBitmap();

    if (gfxDriver->HasAcceleratedStretchAndFlip()) 
    {
        walkBehindMethod = DrawAsSeparateSprite;

        CreateBlankImage();
    }
}

void engine_prepare_screen()
{
    Out::FPrint("Preparing graphics mode screen");
    Out::FPrint("Screen resolution: %d x %d; game resolution %d x %d", GameResolution.Width, GameResolution.Height, GameSize.Width, GameSize.Height);

    // Most cards do 5-6-5 RGB, which is the format the files are saved in
    // Some do 5-6-5 BGR, or  6-5-5 RGB, in which case convert the gfx
    if ((GameResolution.ColorDepth == 16) && ((_rgb_b_shift_16 != 0) || (_rgb_r_shift_16 != 11))) {
        convert_16bit_bgr = 1;
        if (_rgb_r_shift_16 == 10) {
            // some very old graphics cards lie about being 16-bit when they
            // are in fact 15-bit ... get around this
            _places_r = 3;
            _places_g = 3;
        }
    }
    if (GameResolution.ColorDepth > 16) {
        // when we're using 32-bit colour, it converts hi-color images
        // the wrong way round - so fix that

#if defined(IOS_VERSION) || defined(ANDROID_VERSION) || defined(PSP_VERSION)
        _rgb_b_shift_16 = 0;
        _rgb_g_shift_16 = 5;
        _rgb_r_shift_16 = 11;

        _rgb_b_shift_15 = 0;
        _rgb_g_shift_15 = 5;
        _rgb_r_shift_15 = 10;

        _rgb_r_shift_32 = 0;
        _rgb_g_shift_32 = 8;
        _rgb_b_shift_32 = 16;
#else
        _rgb_r_shift_16 = 11;
        _rgb_g_shift_16 = 5;
        _rgb_b_shift_16 = 0;
#endif
    }
    else if (GameResolution.ColorDepth == 16) {
        // ensure that any 32-bit graphics displayed are converted
        // properly to the current depth
#if defined(PSP_VERSION)
        _rgb_r_shift_32 = 0;
        _rgb_g_shift_32 = 8;
        _rgb_b_shift_32 = 16;

        _rgb_b_shift_15 = 0;
        _rgb_g_shift_15 = 5;
        _rgb_r_shift_15 = 10;
#else
        _rgb_r_shift_32 = 16;
        _rgb_g_shift_32 = 8;
        _rgb_b_shift_32 = 0;
#endif
    }
    else if (GameResolution.ColorDepth < 16) {
        // ensure that any 32-bit graphics displayed are converted
        // properly to the current depth
#if defined (WINDOWS_VERSION)
        _rgb_r_shift_32 = 16;
        _rgb_g_shift_32 = 8;
        _rgb_b_shift_32 = 0;
#else
        _rgb_r_shift_32 = 0;
        _rgb_g_shift_32 = 8;
        _rgb_b_shift_32 = 16;

        _rgb_b_shift_15 = 0;
        _rgb_g_shift_15 = 5;
        _rgb_r_shift_15 = 10;
#endif
    }
}

void engine_set_gfx_driver_callbacks()
{
    gfxDriver->SetCallbackForPolling(update_polled_stuff_if_runtime);
    gfxDriver->SetCallbackToDrawScreen(draw_screen_callback);
    gfxDriver->SetCallbackForNullSprite(GfxDriverNullSpriteCallback);
}

void engine_set_color_conversions()
{
    Out::FPrint("Initializing colour conversion");

    set_color_conversion(COLORCONV_MOST | COLORCONV_EXPAND_256 | COLORCONV_REDUCE_16_TO_15);
}

int graphics_mode_init()
{
    ColorDepthOption color_depths;

    engine_init_screen_settings(color_depths);
    int res = engine_init_gfx_filters(color_depths.First);
    if (res != RETURN_CONTINUE) {
        return res;
    }
    engine_init_gfx_driver();
    res = engine_init_graphics_mode(color_depths);
    if (res != RETURN_CONTINUE) {
        return res;
    }

    engine_post_init_gfx_driver();
    engine_prepare_screen();
    platform->PostAllegroInit((usetup.windowed > 0) ? true : false);
    engine_set_gfx_driver_callbacks();
    engine_set_color_conversions();
    return RETURN_CONTINUE;
}
