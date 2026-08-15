#include "rgss/core/RgssPlaneCore.h"

#include "rgss/core/RgssBitmapCore.h"

namespace rgss {

namespace {

int PositiveMod(int aValue, int aDivisor)
{
    if (aDivisor <= 0)
        return 0;
    int value = aValue % aDivisor;
    return value < 0 ? value + aDivisor : value;
}

} // namespace

RgssPlaneCore::RgssPlaneCore()
    : bitmap(0), ox(0), oy(0), zoomX(1.0), zoomY(1.0), opacity(255),
      blendType(KBlendNormal), color(), tone(), scratch(0)
{
}

RgssPlaneCore::~RgssPlaneCore()
{
    delete scratch;
}

RgssBitmapCore* RgssPlaneCore::Scratch(int aWidth, int aHeight)
{
    if (aWidth <= 0 || aHeight <= 0)
        return 0;
    if (!scratch)
        scratch = new RgssBitmapCore;
    if (!scratch->IsValid() || scratch->Width() != aWidth ||
        scratch->Height() != aHeight)
    {
        if (!scratch->CreateTransparent(aWidth, aHeight))
            return 0;
    }
    else
    {
        scratch->ClearTransparent();
    }
    return scratch;
}

void RgssPlaneCore::Draw(RgssBitmapCore& aDest, int aViewportX, int aViewportY,
                         const Rect& aClip)
{
    if (!bitmap || !bitmap->IsValid() || opacity <= 0 ||
        zoomX <= 0.0 || zoomY <= 0.0 || aClip.IsEmpty())
        return;

    RgssBitmapCore* target = Scratch(aClip.width, aClip.height);
    if (!target)
        return;

    const int tileWidth = static_cast<int>(bitmap->Width() * zoomX + 0.5);
    const int tileHeight = static_cast<int>(bitmap->Height() * zoomY + 0.5);
    if (tileWidth <= 0 || tileHeight <= 0)
        return;

    const int patternX = aViewportX - static_cast<int>(ox * zoomX + 0.5);
    const int patternY = aViewportY - static_cast<int>(oy * zoomY + 0.5);
    const int firstX = -PositiveMod(aClip.x - patternX, tileWidth);
    const int firstY = -PositiveMod(aClip.y - patternY, tileHeight);
    const Rect source(0, 0, bitmap->Width(), bitmap->Height());

    for (int y = firstY; y < aClip.height; y += tileHeight)
    {
        for (int x = firstX; x < aClip.width; x += tileWidth)
        {
            target->StretchBlt(bitmap, source, x, y, zoomX, zoomY, 255,
                               false, blendType);
        }
    }

    aDest.Blit(target, Rect(0, 0, aClip.width, aClip.height),
               aClip.x, aClip.y, opacity);
}

} // namespace rgss
