
#include "util/geometry.h"

namespace AGS
{
namespace Common
{

namespace Math
{

void PlaceInRect(const Rect &place, Rect &item, Placement &placement)
{
    switch (placement)
    {
    case kPlaceCenter:
        item = RectWH((place.GetWidth() >> 1) - (item.GetWidth() >> 1),
                      (place.GetHeight() >> 1) - (item.GetHeight() >> 1),
                      item.GetWidth(), item.GetHeight());
        break;
    case kPlaceStretch:
        item = place;
        break;
    case kPlaceStretchProportional:
        {
            const int place_w = place.GetWidth();
            const int place_h = place.GetHeight();
            const int item_w  = item.GetWidth();
            const int item_h  = item.GetHeight();

            int width  = place_w;
            int height = place_w * item_h / item_w;
            if (height > place_h)
            {
                width  = place_h * item_w / item_h;
                height = place_h;
            }
            item = RectWH((place_w - width) >> 1, (place_h - height) >> 1, width, height);
        }
        break;
    default:
        item = RectWH(place.Left + item.Left, place.Top + item.Top, item.GetWidth(), item.GetHeight());
        break;
    }
}

} // namespace Math

} // namespace Common
} // namespace AGS
