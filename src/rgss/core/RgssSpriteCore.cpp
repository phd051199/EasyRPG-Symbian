#include "rgss/core/RgssSpriteCore.h"

#include "rgss/core/RgssBitmapCore.h"
#include <math.h>

namespace rgss {

RgssSpriteCore::RgssSpriteCore()
    : bitmap(0), srcRect(), x(0), y(0), ox(0), oy(0), zoomX(1.0), zoomY(1.0),
      opacity(255), bushTop(0), bushOpacity(128), waveAmp(0), waveLength(180),
      wavePhase(0.0), waveSpeed(360), blendType(KBlendNormal), mirror(false)
{
}

namespace {

const double KSpriteWavePi = 3.14159265358979323846;

Rect SpriteIntersectRect(const Rect& aLeft, const Rect& aRight)
{
    const int x1 = aLeft.x > aRight.x ? aLeft.x : aRight.x;
    const int y1 = aLeft.y > aRight.y ? aLeft.y : aRight.y;
    const int leftRight = aLeft.x + aLeft.width;
    const int rightRight = aRight.x + aRight.width;
    const int leftBottom = aLeft.y + aLeft.height;
    const int rightBottom = aRight.y + aRight.height;
    const int x2 = leftRight < rightRight ? leftRight : rightRight;
    const int y2 = leftBottom < rightBottom ? leftBottom : rightBottom;
    return x2 > x1 && y2 > y1 ? Rect(x1, y1, x2 - x1, y2 - y1) : Rect();
}

} // namespace

void RgssSpriteCore::Draw(RgssBitmapCore& aDest, int aViewportX,
                          int aViewportY, const Rect& aClip)
{
    if (!bitmap || !bitmap->IsValid() || aClip.IsEmpty())
        return;

    const int sourceHeight = srcRect.height > 0 ? srcRect.height : bitmap->Height();
    if (sourceHeight <= 0)
        return;
    const double absZoomY = zoomY < 0.0 ? -zoomY : zoomY;
    const double absZoomX = zoomX < 0.0 ? -zoomX : zoomX;
    const int screenHeight = static_cast<int>(sourceHeight * absZoomY + 0.5);
    if (screenHeight <= 0)
        return;

    const int originalX = x;
    const int originalOpacity = opacity;
    const int top = aViewportY + y - static_cast<int>(oy * zoomY + (zoomY >= 0.0 ? 0.5 : -0.5));
    int bushBoundary = top + screenHeight;
    if (bushTop > 0 && bushOpacity < 255)
    {
        int depth = bushTop;
        if (depth > sourceHeight) depth = sourceHeight;
        bushBoundary = top + static_cast<int>((sourceHeight - depth) * absZoomY + 0.5);
    }

    const bool useBush = bushTop > 0 && bushOpacity < 255;
    const bool useWave = waveAmp != 0 && waveLength > 0;
    if (!useBush && !useWave)
    {
        DrawRaw(aDest, aViewportX, aViewportY, aClip);
        return;
    }

    if (!useWave)
    {
        Rect upperClip = SpriteIntersectRect(aClip,
            Rect(aClip.x, aClip.y, aClip.width, bushBoundary - aClip.y));
        if (!upperClip.IsEmpty())
            DrawRaw(aDest, aViewportX, aViewportY, upperClip);

        Rect lowerClip = SpriteIntersectRect(aClip,
            Rect(aClip.x, bushBoundary, aClip.width,
                 aClip.y + aClip.height - bushBoundary));
        if (!lowerClip.IsEmpty())
        {
            opacity = originalOpacity * bushOpacity / 255;
            DrawRaw(aDest, aViewportX, aViewportY, lowerClip);
            opacity = originalOpacity;
        }
        return;
    }

    const int bandHeight = absZoomY >= 2.0 ? 2 : 3;
    const int bottom = top + screenHeight;
    for (int bandTop = top; bandTop < bottom; bandTop += bandHeight)
    {
        const int height = bottom - bandTop < bandHeight ? bottom - bandTop : bandHeight;
        Rect bandClip = SpriteIntersectRect(aClip,
            Rect(aClip.x, bandTop, aClip.width, height));
        if (bandClip.IsEmpty())
            continue;

        const double sourceY = absZoomY > 0.000001 ?
            static_cast<double>(bandTop - top) / absZoomY : 0.0;
        const double degrees = wavePhase + sourceY * 360.0 / waveLength;
        const double offset = sin(degrees * KSpriteWavePi / 180.0) *
                              waveAmp * absZoomX;
        x = originalX + static_cast<int>(offset + (offset >= 0.0 ? 0.5 : -0.5));
        opacity = (useBush && bandTop >= bushBoundary) ?
                  originalOpacity * bushOpacity / 255 : originalOpacity;
        DrawRaw(aDest, aViewportX, aViewportY, bandClip);
    }
    x = originalX;
    opacity = originalOpacity;
}

void RgssSpriteCore::DrawRaw(RgssBitmapCore& aDest, int aViewportX, int aViewportY,
                          const Rect& aClip)
{
    if (!bitmap || !bitmap->IsValid() || srcRect.IsEmpty())
        return;
    double zx = zoomX > 0.0 ? zoomX : 0.0;
    double zy = zoomY > 0.0 ? zoomY : 0.0;
    bool identity = zx == 1.0 && zy == 1.0;
    int dstX = aViewportX + x - ox;
    int dstY = aViewportY + y - oy;
    int dstW = identity ? srcRect.width
                        : static_cast<int>(srcRect.width * zx + 0.5);
    int dstH = identity ? srcRect.height
                        : static_cast<int>(srcRect.height * zy + 0.5);
    if (dstW <= 0 || dstH <= 0)
        return;

    /* Clip the drawn rect to the viewport. */
    int vx1 = dstX > aClip.x ? dstX : aClip.x;
    int vy1 = dstY > aClip.y ? dstY : aClip.y;
    int vx2 = dstX + dstW < aClip.x + aClip.width ? dstX + dstW
                                                  : aClip.x + aClip.width;
    int vy2 = dstY + dstH < aClip.y + aClip.height ? dstY + dstH
                                                   : aClip.y + aClip.height;
    if (vx1 >= vx2 || vy1 >= vy2)
        return;
    int visX = vx1, visY = vy1, visW = vx2 - vx1, visH = vy2 - vy1;

    if (identity && blendType == KBlendNormal && !mirror) {
        const int sx = srcRect.x + (visX - dstX);
        const int sy = srcRect.y + (visY - dstY);
        aDest.Blit(bitmap, Rect(sx, sy, visW, visH), visX, visY, opacity);
        return;
    }

    /* SLOW PATH: scaled and/or mirrored and/or add/sub blend. The visible
     * region maps back into source space with the same scale. */
    Rect srcSub(static_cast<int>((visX - dstX) / zx) + srcRect.x,
                static_cast<int>((visY - dstY) / zy) + srcRect.y,
                static_cast<int>(visW / zx + 0.5),
                static_cast<int>(visH / zy + 0.5));
    if (srcSub.width <= 0 || srcSub.height <= 0)
        return;
    aDest.StretchBlt(bitmap, srcSub, visX, visY, zx, zy, opacity, mirror,
                     blendType);
}

} // namespace rgss
