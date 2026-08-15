#include "rgss/core/RgssWindowCore.h"

#include "rgss/core/RgssBitmapCore.h"

namespace rgss {

namespace {

Rect IntersectRect(const Rect& aLeft, const Rect& aRight)
{
    const int x1 = aLeft.x > aRight.x ? aLeft.x : aRight.x;
    const int y1 = aLeft.y > aRight.y ? aLeft.y : aRight.y;
    const int leftX2 = aLeft.x + aLeft.width;
    const int rightX2 = aRight.x + aRight.width;
    const int leftY2 = aLeft.y + aLeft.height;
    const int rightY2 = aRight.y + aRight.height;
    const int x2 = leftX2 < rightX2 ? leftX2 : rightX2;
    const int y2 = leftY2 < rightY2 ? leftY2 : rightY2;
    return x2 > x1 && y2 > y1 ? Rect(x1, y1, x2 - x1, y2 - y1)
                                : Rect();
}

void DrawFrame64(RgssBitmapCore& aDest, RgssBitmapCore& aSkin,
                 int aSourceX, int aSourceY, int aWidth, int aHeight,
                 int aOpacity)
{
    if (aWidth <= 0 || aHeight <= 0)
        return;
    if (aWidth < 32 || aHeight < 32)
    {
        aDest.StretchBlt(&aSkin, Rect(aSourceX, aSourceY, 64, 64), 0, 0,
                         static_cast<double>(aWidth) / 64.0,
                         static_cast<double>(aHeight) / 64.0,
                         aOpacity, false, KBlendNormal);
        return;
    }

    const int edgeWidth = aWidth - 32;
    const int edgeHeight = aHeight - 32;
    aDest.Blit(&aSkin, Rect(aSourceX, aSourceY, 16, 16), 0, 0, aOpacity);
    aDest.Blit(&aSkin, Rect(aSourceX + 48, aSourceY, 16, 16),
               aWidth - 16, 0, aOpacity);
    aDest.Blit(&aSkin, Rect(aSourceX, aSourceY + 48, 16, 16),
               0, aHeight - 16, aOpacity);
    aDest.Blit(&aSkin, Rect(aSourceX + 48, aSourceY + 48, 16, 16),
               aWidth - 16, aHeight - 16, aOpacity);

    if (edgeWidth > 0)
    {
        const double scaleX = static_cast<double>(edgeWidth) / 32.0;
        aDest.StretchBlt(&aSkin, Rect(aSourceX + 16, aSourceY, 32, 16),
                         16, 0, scaleX, 1.0, aOpacity, false, KBlendNormal);
        aDest.StretchBlt(&aSkin, Rect(aSourceX + 16, aSourceY + 48, 32, 16),
                         16, aHeight - 16, scaleX, 1.0,
                         aOpacity, false, KBlendNormal);
    }
    if (edgeHeight > 0)
    {
        const double scaleY = static_cast<double>(edgeHeight) / 32.0;
        aDest.StretchBlt(&aSkin, Rect(aSourceX, aSourceY + 16, 16, 32),
                         0, 16, 1.0, scaleY, aOpacity, false, KBlendNormal);
        aDest.StretchBlt(&aSkin, Rect(aSourceX + 48, aSourceY + 16, 16, 32),
                         aWidth - 16, 16, 1.0, scaleY,
                         aOpacity, false, KBlendNormal);
    }
}

void ApplyWindowTone(RgssBitmapCore& aBitmap, const Tone& aTone)
{
    const int toneRed = static_cast<int>(aTone.red + (aTone.red >= 0.0 ? 0.5 : -0.5));
    const int toneGreen = static_cast<int>(aTone.green + (aTone.green >= 0.0 ? 0.5 : -0.5));
    const int toneBlue = static_cast<int>(aTone.blue + (aTone.blue >= 0.0 ? 0.5 : -0.5));
    int gray = static_cast<int>(aTone.gray + 0.5);
    if (gray < 0) gray = 0;
    if (gray > 255) gray = 255;
    if (toneRed == 0 && toneGreen == 0 && toneBlue == 0 && gray == 0)
        return;

    RgssBitmapCore toned;
    if (!toned.CreateTransparent(aBitmap.Width(), aBitmap.Height()))
        return;
    for (int y = 0; y < aBitmap.Height(); ++y)
    {
        for (int x = 0; x < aBitmap.Width(); ++x)
        {
            const int alpha = aBitmap.AlphaAt(x, y);
            if (alpha <= 0)
                continue;
            int red = 0, green = 0, blue = 0;
            if (!aBitmap.GetPixel(x, y, &red, &green, &blue))
                continue;
            red += toneRed;
            green += toneGreen;
            blue += toneBlue;
            if (red < 0) red = 0; else if (red > 255) red = 255;
            if (green < 0) green = 0; else if (green > 255) green = 255;
            if (blue < 0) blue = 0; else if (blue > 255) blue = 255;
            if (gray > 0)
            {
                const int luminance = (red * 299 + green * 587 + blue * 114) / 1000;
                red = (red * (255 - gray) + luminance * gray) / 255;
                green = (green * (255 - gray) + luminance * gray) / 255;
                blue = (blue * (255 - gray) + luminance * gray) / 255;
            }
            toned.FillRect(Rect(x, y, 1, 1), red, green, blue, alpha);
        }
    }
    aBitmap.ClearTransparent();
    aBitmap.Blit(&toned, Rect(0, 0, toned.Width(), toned.Height()), 0, 0, 255);
}

} // namespace

RgssWindowCore::RgssWindowCore()
    : windowskin(0), contents(0), cursorRect(), x(0), y(0), width(0), height(0),
      ox(0), oy(0), opacity(255), backOpacity(255), contentsOpacity(255),
      openness(255), padding(16), paddingBottom(16), generation(1), tone(),
      active(true), pause(false), stretch(true), arrowsVisible(true), scratch(0)
{
}

RgssWindowCore::~RgssWindowCore()
{
    delete scratch;
}

RgssBitmapCore* RgssWindowCore::Scratch(int aWidth, int aHeight)
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

void RgssWindowCore::Draw(RgssBitmapCore& aDest, int aViewportX, int aViewportY,
                          const Rect& aClip)
{
    if (width <= 0 || height <= 0 || opacity <= 0 || aClip.IsEmpty())
        return;

    RgssBitmapCore* target = Scratch(width, height);
    if (!target)
        return;

    const Rect full(0, 0, width, height);
    if (windowskin && windowskin->IsValid())
    {
        /* RGSS1 standard windowskins are 192x128. The 128x128 left block is
         * the scalable background; the 64x64 top-right block contains the
         * frame artwork. Non-standard skins fall back to stretching the
         * complete bitmap, which is preferable to dropping custom UI. */
        if (generation <= 1 && windowskin->Width() >= 192 && windowskin->Height() >= 128)
        {
            const int innerX = width > 4 ? 2 : 0;
            const int innerY = height > 4 ? 2 : 0;
            const int innerW = width > 4 ? width - 4 : width;
            const int innerH = height > 4 ? height - 4 : height;
            if (innerW > 0 && innerH > 0)
            {
                target->StretchBlt(windowskin, Rect(0, 0, 128, 128),
                                   innerX, innerY,
                                   static_cast<double>(innerW) / 128.0,
                                   static_cast<double>(innerH) / 128.0,
                                   backOpacity, false, KBlendNormal);
            }
            DrawFrame64(*target, *windowskin, 128, 0,
                        width, height, opacity);
        }
        else if (generation >= 2 && windowskin->Width() >= 128 &&
                 windowskin->Height() >= 128)
        {
            const int innerX = width > 4 ? 2 : 0;
            const int innerY = height > 4 ? 2 : 0;
            const int innerW = width > 4 ? width - 4 : width;
            const int innerH = height > 4 ? height - 4 : height;
            if (innerW > 0 && innerH > 0)
            {
                target->StretchBlt(windowskin, Rect(0, 0, 64, 64),
                                   innerX, innerY,
                                   static_cast<double>(innerW) / 64.0,
                                   static_cast<double>(innerH) / 64.0,
                                   backOpacity, false, KBlendNormal);
            }
            DrawFrame64(*target, *windowskin, 64, 0,
                        width, height, opacity);
        }
        else
        {
            target->StretchBlt(windowskin,
                               Rect(0, 0, windowskin->Width(), windowskin->Height()),
                               0, 0,
                               static_cast<double>(width) / windowskin->Width(),
                               static_cast<double>(height) / windowskin->Height(),
                               backOpacity, false, KBlendNormal);
        }
    }
    else
    {
        target->FillRect(full, 0, 0, 0, backOpacity);
    }

    ApplyWindowTone(*target, tone);

    const int contentPadding = padding < 0 ? 0 : padding;
    const int bottomPadding = paddingBottom < 0 ? 0 : paddingBottom;
    const int contentWidth = width - contentPadding * 2;
    const int contentHeight = height - contentPadding - bottomPadding;
    if (contents && contents->IsValid() && contentsOpacity > 0 &&
        contentWidth > 0 && contentHeight > 0)
    {
        const Rect inner(contentPadding, contentPadding,
                         contentWidth, contentHeight);
        const Rect contentSource(ox, oy, inner.width, inner.height);
        target->Blit(contents, contentSource, inner.x, inner.y, contentsOpacity);
    }

    /* Minimal cursor highlight. The exact windowskin cursor nine-slice will
     * replace this once the full skin atlas path lands. */
    if (active && !cursorRect.IsEmpty())
    {
        Rect cursor(cursorRect.x + contentPadding - ox,
                    cursorRect.y + contentPadding - oy,
                    cursorRect.width, cursorRect.height);
        cursor = IntersectRect(cursor,
                               Rect(contentPadding, contentPadding,
                                    contentWidth > 0 ? contentWidth : 0,
                                    contentHeight > 0 ? contentHeight : 0));
        if (!cursor.IsEmpty())
        {
            const int cursorX = generation <= 1 ? 128 : 64;
            if (windowskin && windowskin->IsValid() &&
                windowskin->Width() >= cursorX + 32 && windowskin->Height() >= 96)
            {
                target->StretchBlt(windowskin, Rect(cursorX, 64, 32, 32),
                                   cursor.x, cursor.y,
                                   static_cast<double>(cursor.width) / 32.0,
                                   static_cast<double>(cursor.height) / 32.0,
                                   255, false, KBlendNormal);
            }
            else
            {
                target->FillRect(cursor, 255, 255, 255, 48);
            }
        }
    }

    if (openness <= 0)
        return;
    int openHeight = height;
    int openOffsetY = 0;
    if (openness < 255)
    {
        openHeight = height * openness / 255;
        if (openHeight <= 0)
            return;
        openOffsetY = (height - openHeight) / 2;
    }
    const Rect screenRect(aViewportX + x, aViewportY + y + openOffsetY,
                          width, openHeight);
    const Rect visibleRect = IntersectRect(screenRect, aClip);
    if (visibleRect.IsEmpty())
        return;
    const Rect source(visibleRect.x - screenRect.x,
                      visibleRect.y - screenRect.y + openOffsetY,
                      visibleRect.width, visibleRect.height);
    aDest.Blit(target, source, visibleRect.x, visibleRect.y, 255);
}

} // namespace rgss
