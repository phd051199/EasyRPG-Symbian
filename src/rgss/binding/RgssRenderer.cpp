#include "RgssBindingUtil.h"
#include "RgssEtcBinding.h"
#include "RgssRendererBinding.h"

#include "../core/RgssBitmapCore.h"
#include "../core/RgssGraphicsCore.h"
#include "../core/RgssFontCore.h"
#include "../core/RgssScene.h"
#include "../core/RgssSpriteCore.h"
#include "../core/RgssVfs.h"
#include "RgssHost.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>

void RgssInputBindingSetFrameRate(int aFramesPerSecond);
void RgssM4BindingSync();

namespace {

using rgss::Color;
using rgss::Rect;
using rgss::RgssBitmapCore;
using rgss::RgssGraphicsCore;
using rgss::RgssScene;
using rgss::RgssSpriteCore;
using rgss::Tone;
using rgss::ViewportElement;

VALUE cBitmap = Qnil;
VALUE cSprite = Qnil;
VALUE cViewport = Qnil;
VALUE mGraphics = Qnil;

RgssScene* gScene = 0;
RgssGraphicsCore* gGraphics = 0;

int ClampByte(int aValue)
{
    if (aValue < 0)
        return 0;
    if (aValue > 255)
        return 255;
    return aValue;
}

/* -------------------------------------------------------------------------
 * Bitmap
 * ---------------------------------------------------------------------- */

struct BitmapData
{
    BitmapData() : core(new RgssBitmapCore), disposed(false) {}
    ~BitmapData() { delete core; }

    RgssBitmapCore* core;
    bool disposed;
};

void BitmapFree(void* aData)
{
    delete static_cast<BitmapData*>(aData);
}

size_t BitmapMemsize(const void* aData)
{
    const BitmapData* data = static_cast<const BitmapData*>(aData);
    if (!data || !data->core)
        return sizeof(BitmapData);
    return sizeof(BitmapData) +
           static_cast<size_t>(data->core->StrideShorts()) *
               data->core->Height() * sizeof(unsigned short) +
           (data->core->AlphaMask()
                ? static_cast<size_t>(data->core->Width()) * data->core->Height()
                : 0);
}

const rb_data_type_t BitmapType = {
    "Bitmap",
    {0, BitmapFree, BitmapMemsize, 0},
    0, 0, RUBY_TYPED_FREE_IMMEDIATELY
};

VALUE BitmapAlloc(VALUE aKlass)
{
    return TypedData_Wrap_Struct(aKlass, &BitmapType, new BitmapData);
}

BitmapData* GetBitmap(VALUE aSelf)
{
    BitmapData* data = 0;
    TypedData_Get_Struct(aSelf, BitmapData, &BitmapType, data);
    if (!data || !data->core)
        rgssRaise("uninitialized Bitmap");
    return data;
}

RgssBitmapCore* GetLiveBitmap(VALUE aSelf)
{
    BitmapData* data = GetBitmap(aSelf);
    if (data->disposed || !data->core->IsValid())
        rgssRaise("disposed Bitmap");
    return data->core;
}

bool HasPathExtension(const char* aPath)
{
    const char* slash = strrchr(aPath, '/');
    const char* backslash = strrchr(aPath, '\\');
    if (!slash || (backslash && backslash > slash))
        slash = backslash;
    const char* dot = strrchr(aPath, '.');
    return dot && (!slash || dot > slash);
}

bool CopyNormalizedPath(const char* aInput, char* aOutput, int aCapacity)
{
    if (!aInput || !aOutput || aCapacity <= 1)
        return false;
    const size_t length = strlen(aInput);
    if (length == 0 || length >= static_cast<size_t>(aCapacity))
        return false;
    for (size_t i = 0; i < length; ++i)
        aOutput[i] = aInput[i] == '\\' ? '/' : aInput[i];
    aOutput[length] = 0;
    return true;
}

bool ResolveBitmapPath(const char* aInput, char* aOutput, int aCapacity)
{
    char base[1024];
    if (!CopyNormalizedPath(aInput, base, sizeof(base)))
        return false;

    if (RgssVfsExists(base))
    {
        if (strlen(base) >= static_cast<size_t>(aCapacity))
            return false;
        strcpy(aOutput, base);
        return true;
    }
    if (HasPathExtension(base))
        return false;

    static const char* kExtensions[] =
        {".png", ".PNG", ".bmp", ".BMP", ".xyz", ".XYZ"};
    for (unsigned i = 0; i < sizeof(kExtensions) / sizeof(kExtensions[0]); ++i)
    {
        const size_t baseLength = strlen(base);
        const size_t extLength = strlen(kExtensions[i]);
        if (baseLength + extLength >= sizeof(base) ||
            baseLength + extLength >= static_cast<size_t>(aCapacity))
            continue;
        char candidate[1024];
        strcpy(candidate, base);
        strcat(candidate, kExtensions[i]);
        if (!RgssVfsExists(candidate))
            continue;
        strcpy(aOutput, candidate);
        return true;
    }
    return false;
}

bool LoadBitmapFile(BitmapData* aData, const char* aPath)
{
    char resolved[1024];
    if (!ResolveBitmapPath(aPath, resolved, sizeof(resolved)))
        return false;

    unsigned char* bytes = 0;
    const long size = RgssVfsRead(resolved, &bytes);
    if (size <= 0 || !bytes)
    {
        free(bytes);
        return false;
    }

    RgssBitmapCore* decoded = rgss::RgssDecodeImageToRgb565(bytes, size);
    free(bytes);
    if (!decoded)
        return false;

    delete aData->core;
    aData->core = decoded;
    aData->disposed = false;
    return true;
}

VALUE BitmapInitialize(int aArgc, VALUE* aArgv, VALUE aSelf)
{
    BitmapData* data = GetBitmap(aSelf);
    data->disposed = false;

    if (aArgc == 1)
    {
        VALUE filename = aArgv[0];
        StringValue(filename);
        const char* path = StringValueCStr(filename);
        if (!LoadBitmapFile(data, path))
        {
            VALUE errnoModule = rb_const_get(rb_cObject, rb_intern("Errno"));
            VALUE enoent = rb_const_get(errnoModule, rb_intern("ENOENT"));
            rb_raise(enoent, "No such file or unsupported bitmap - %s", path);
        }
        return aSelf;
    }
    if (aArgc == 2)
    {
        const int width = NUM2INT(aArgv[0]);
        const int height = NUM2INT(aArgv[1]);
        if (width <= 0 || height <= 0 || width > 4096 || height > 4096)
            rb_raise(rb_eArgError, "invalid Bitmap size");
        if (!data->core->CreateTransparent(width, height))
            rgssRaise("failed to allocate Bitmap");
        return aSelf;
    }

    rb_raise(rb_eArgError, "wrong number of arguments");
    return Qnil;
}

VALUE BitmapDispose(VALUE aSelf)
{
    BitmapData* data = GetBitmap(aSelf);
    if (!data->disposed)
    {
        data->core->Create(0, 0);
        data->disposed = true;
    }
    return Qnil;
}

VALUE BitmapDisposed(VALUE aSelf)
{
    return GetBitmap(aSelf)->disposed ? Qtrue : Qfalse;
}

VALUE BitmapWidth(VALUE aSelf)
{
    return INT2FIX(GetLiveBitmap(aSelf)->Width());
}

VALUE BitmapHeight(VALUE aSelf)
{
    return INT2FIX(GetLiveBitmap(aSelf)->Height());
}

VALUE BitmapRect(VALUE aSelf)
{
    RgssBitmapCore* core = GetLiveBitmap(aSelf);
    return RgssRectObject(Rect(0, 0, core->Width(), core->Height()));
}

VALUE BitmapBlt(int aArgc, VALUE* aArgv, VALUE aSelf)
{
    VALUE x, y, srcObject, srcRectObject, opacityObject;
    rb_scan_args(aArgc, aArgv, "41", &x, &y, &srcObject, &srcRectObject,
                 &opacityObject);
    const int opacity = NIL_P(opacityObject) ? 255 : ClampByte(NUM2INT(opacityObject));
    GetLiveBitmap(aSelf)->Blit(GetLiveBitmap(srcObject),
                               RgssRectValue(srcRectObject),
                               NUM2INT(x), NUM2INT(y), opacity);
    return Qnil;
}

VALUE BitmapStretchBlt(int aArgc, VALUE* aArgv, VALUE aSelf)
{
    VALUE dstRectObject, srcObject, srcRectObject, opacityObject;
    rb_scan_args(aArgc, aArgv, "31", &dstRectObject, &srcObject,
                 &srcRectObject, &opacityObject);
    const Rect dst = RgssRectValue(dstRectObject);
    const Rect src = RgssRectValue(srcRectObject);
    const int opacity = NIL_P(opacityObject) ? 255 : ClampByte(NUM2INT(opacityObject));
    if (dst.width > 0 && dst.height > 0 && src.width > 0 && src.height > 0)
    {
        GetLiveBitmap(aSelf)->StretchBlt(
            GetLiveBitmap(srcObject), src, dst.x, dst.y,
            static_cast<double>(dst.width) / src.width,
            static_cast<double>(dst.height) / src.height,
            opacity, false, rgss::KBlendNormal);
    }
    return Qnil;
}

VALUE BitmapFillRect(int aArgc, VALUE* aArgv, VALUE aSelf)
{
    Rect rect;
    VALUE colorObject = Qnil;
    if (aArgc == 2)
    {
        rect = RgssRectValue(aArgv[0]);
        colorObject = aArgv[1];
    }
    else if (aArgc == 5)
    {
        rect.Set(NUM2INT(aArgv[0]), NUM2INT(aArgv[1]),
                 NUM2INT(aArgv[2]), NUM2INT(aArgv[3]));
        colorObject = aArgv[4];
    }
    else
    {
        rb_raise(rb_eArgError, "wrong number of arguments");
    }
    const Color color = RgssColorValue(colorObject);
    GetLiveBitmap(aSelf)->FillRect(rect,
        ClampByte(static_cast<int>(color.red + 0.5)),
        ClampByte(static_cast<int>(color.green + 0.5)),
        ClampByte(static_cast<int>(color.blue + 0.5)),
        ClampByte(static_cast<int>(color.alpha + 0.5)));
    return Qnil;
}

VALUE BitmapClear(VALUE aSelf)
{
    GetLiveBitmap(aSelf)->ClearTransparent();
    return Qnil;
}

VALUE BitmapClearRect(int aArgc, VALUE* aArgv, VALUE aSelf)
{
    Rect rect;
    if (aArgc == 1)
        rect = RgssRectValue(aArgv[0]);
    else if (aArgc == 4)
        rect.Set(NUM2INT(aArgv[0]), NUM2INT(aArgv[1]),
                 NUM2INT(aArgv[2]), NUM2INT(aArgv[3]));
    else
        rb_raise(rb_eArgError, "wrong number of arguments");
    GetLiveBitmap(aSelf)->ClearRectTransparent(rect);
    return Qnil;
}

VALUE BitmapGetPixel(VALUE aSelf, VALUE aX, VALUE aY)
{
    RgssBitmapCore* core = GetLiveBitmap(aSelf);
    const int x = NUM2INT(aX);
    const int y = NUM2INT(aY);
    int red = 0, green = 0, blue = 0;
    if (!core->GetPixel(x, y, &red, &green, &blue))
        return RgssColorObject(Color(0, 0, 0, 0));
    return RgssColorObject(Color(red, green, blue, core->AlphaAt(x, y)));
}

VALUE BitmapSetPixel(VALUE aSelf, VALUE aX, VALUE aY, VALUE aColor)
{
    const Color color = RgssColorValue(aColor);
    GetLiveBitmap(aSelf)->SetPixelRgba(NUM2INT(aX), NUM2INT(aY),
        ClampByte(static_cast<int>(color.red + 0.5)),
        ClampByte(static_cast<int>(color.green + 0.5)),
        ClampByte(static_cast<int>(color.blue + 0.5)),
        ClampByte(static_cast<int>(color.alpha + 0.5)));
    return Qnil;
}

VALUE BitmapHueChange(VALUE aSelf, VALUE aHue)
{
    GetLiveBitmap(aSelf)->HueChange(NUM2INT(aHue));
    return Qnil;
}

VALUE BitmapFontObject(VALUE aSelf)
{
    VALUE font = rb_iv_get(aSelf, "@font");
    if (!NIL_P(font))
        return font;
    const ID fontId = rb_intern("Font");
    if (!rb_const_defined(rb_cObject, fontId))
        return Qnil;
    VALUE klass = rb_const_get(rb_cObject, fontId);
    font = rb_class_new_instance(0, 0, klass);
    rb_iv_set(aSelf, "@font", font);
    return font;
}

rgss::RgssFontStyle BitmapFontStyle(VALUE aSelf)
{
    rgss::RgssFontStyle style;
    VALUE font = BitmapFontObject(aSelf);
    if (NIL_P(font))
        return style;

    if (rb_respond_to(font, rb_intern("size")))
        style.size = NUM2INT(rb_funcall(font, rb_intern("size"), 0));
    if (rb_respond_to(font, rb_intern("bold")))
        style.bold = RTEST(rb_funcall(font, rb_intern("bold"), 0));
    if (rb_respond_to(font, rb_intern("italic")))
        style.italic = RTEST(rb_funcall(font, rb_intern("italic"), 0));
    if (rb_respond_to(font, rb_intern("shadow")))
        style.shadow = RTEST(rb_funcall(font, rb_intern("shadow"), 0));
    if (rb_respond_to(font, rb_intern("outline")))
        style.outline = RTEST(rb_funcall(font, rb_intern("outline"), 0));
    if (rb_respond_to(font, rb_intern("color")))
    {
        VALUE color = rb_funcall(font, rb_intern("color"), 0);
        if (!NIL_P(color))
            style.color = RgssColorValue(color);
    }
    if (rb_respond_to(font, rb_intern("out_color")))
    {
        VALUE color = rb_funcall(font, rb_intern("out_color"), 0);
        if (!NIL_P(color))
            style.outColor = RgssColorValue(color);
    }
    return style;
}

VALUE BitmapGradientFillRect(int aArgc, VALUE* aArgv, VALUE aSelf)
{
    RgssBitmapCore* bitmap = GetLiveBitmap(aSelf);
    Rect rect;
    VALUE firstColor = Qnil;
    VALUE secondColor = Qnil;
    VALUE verticalValue = Qfalse;
    if (aArgc == 3 || aArgc == 4)
    {
        rect = RgssRectValue(aArgv[0]);
        firstColor = aArgv[1];
        secondColor = aArgv[2];
        if (aArgc == 4) verticalValue = aArgv[3];
    }
    else if (aArgc == 6 || aArgc == 7)
    {
        rect.Set(NUM2INT(aArgv[0]), NUM2INT(aArgv[1]),
                 NUM2INT(aArgv[2]), NUM2INT(aArgv[3]));
        firstColor = aArgv[4];
        secondColor = aArgv[5];
        if (aArgc == 7) verticalValue = aArgv[6];
    }
    else
    {
        rb_raise(rb_eArgError, "wrong number of arguments");
    }

    const Color first = RgssColorValue(firstColor);
    const Color second = RgssColorValue(secondColor);
    const bool vertical = RTEST(verticalValue);
    const int length = vertical ? rect.height : rect.width;
    if (length <= 0)
        return Qnil;
    for (int index = 0; index < length; ++index)
    {
        const double t = length <= 1 ? 0.0 :
                         static_cast<double>(index) / (length - 1);
        const int red = static_cast<int>(first.red + (second.red - first.red) * t + 0.5);
        const int green = static_cast<int>(first.green + (second.green - first.green) * t + 0.5);
        const int blue = static_cast<int>(first.blue + (second.blue - first.blue) * t + 0.5);
        const int alpha = static_cast<int>(first.alpha + (second.alpha - first.alpha) * t + 0.5);
        const Rect strip = vertical ? Rect(rect.x, rect.y + index, rect.width, 1)
                                    : Rect(rect.x + index, rect.y, 1, rect.height);
        bitmap->FillRect(strip, red, green, blue, alpha);
    }
    return Qnil;
}

void BitmapReplaceWith(RgssBitmapCore* aBitmap, RgssBitmapCore& aReplacement)
{
    aBitmap->ClearTransparent();
    aBitmap->Blit(&aReplacement,
                  Rect(0, 0, aReplacement.Width(), aReplacement.Height()),
                  0, 0, 255);
}

VALUE BitmapBlur(VALUE aSelf)
{
    RgssBitmapCore* bitmap = GetLiveBitmap(aSelf);
    RgssBitmapCore output;
    if (!output.CreateTransparent(bitmap->Width(), bitmap->Height()))
        rb_raise(rb_eNoMemError, "Bitmap blur allocation failed");

    for (int y = 0; y < bitmap->Height(); ++y)
    {
        for (int x = 0; x < bitmap->Width(); ++x)
        {
            int redSum = 0, greenSum = 0, blueSum = 0, alphaSum = 0, count = 0;
            for (int dy = -1; dy <= 1; ++dy)
            {
                const int sy = y + dy;
                if (sy < 0 || sy >= bitmap->Height()) continue;
                for (int dx = -1; dx <= 1; ++dx)
                {
                    const int sx = x + dx;
                    if (sx < 0 || sx >= bitmap->Width()) continue;
                    int red = 0, green = 0, blue = 0;
                    if (!bitmap->GetPixel(sx, sy, &red, &green, &blue)) continue;
                    redSum += red;
                    greenSum += green;
                    blueSum += blue;
                    alphaSum += bitmap->AlphaAt(sx, sy);
                    ++count;
                }
            }
            if (count > 0)
                output.FillRect(Rect(x, y, 1, 1), redSum / count, greenSum / count,
                                blueSum / count, alphaSum / count);
        }
    }
    BitmapReplaceWith(bitmap, output);
    return Qnil;
}

static void RotateSample(const RgssBitmapCore& aBitmap, double aCenterX,
                         double aCenterY, int aX, int aY, double aRadians,
                         int* aRed, int* aGreen, int* aBlue, int* aAlpha)
{
    const double dx = aX - aCenterX;
    const double dy = aY - aCenterY;
    const double cosine = cos(aRadians);
    const double sine = sin(aRadians);
    const int sx = static_cast<int>(aCenterX + dx * cosine - dy * sine + 0.5);
    const int sy = static_cast<int>(aCenterY + dx * sine + dy * cosine + 0.5);
    if (sx < 0 || sy < 0 || sx >= aBitmap.Width() || sy >= aBitmap.Height())
    {
        *aRed = *aGreen = *aBlue = *aAlpha = 0;
        return;
    }
    if (!aBitmap.GetPixel(sx, sy, aRed, aGreen, aBlue))
        *aRed = *aGreen = *aBlue = 0;
    *aAlpha = aBitmap.AlphaAt(sx, sy);
}

VALUE BitmapRadialBlur(VALUE aSelf, VALUE aAngle, VALUE aDivision)
{
    RgssBitmapCore* bitmap = GetLiveBitmap(aSelf);
    double angle = NUM2DBL(aAngle);
    int division = NUM2INT(aDivision);
    if (angle < 0.0) angle = -angle;
    if (angle > 360.0) angle = 360.0;
    if (division < 2) division = 2;
    if (division > 32) division = 32;

    RgssBitmapCore output;
    if (!output.CreateTransparent(bitmap->Width(), bitmap->Height()))
        rb_raise(rb_eNoMemError, "Bitmap radial blur allocation failed");
    const double centerX = (bitmap->Width() - 1) * 0.5;
    const double centerY = (bitmap->Height() - 1) * 0.5;
    const double radians = angle * 3.14159265358979323846 / 180.0;

    for (int y = 0; y < bitmap->Height(); ++y)
    {
        for (int x = 0; x < bitmap->Width(); ++x)
        {
            int redSum = 0, greenSum = 0, blueSum = 0, alphaSum = 0;
            for (int sample = 0; sample < division; ++sample)
            {
                const double t = division <= 1 ? 0.0 :
                                 static_cast<double>(sample) / (division - 1) - 0.5;
                int red = 0, green = 0, blue = 0, alpha = 0;
                RotateSample(*bitmap, centerX, centerY, x, y, radians * t,
                             &red, &green, &blue, &alpha);
                redSum += red;
                greenSum += green;
                blueSum += blue;
                alphaSum += alpha;
            }
            output.FillRect(Rect(x, y, 1, 1), redSum / division,
                            greenSum / division, blueSum / division,
                            alphaSum / division);
        }
    }
    BitmapReplaceWith(bitmap, output);
    return Qnil;
}

VALUE BitmapHueChange(VALUE aSelf, VALUE aHue)
{
    RgssBitmapCore* bitmap = GetLiveBitmap(aSelf);
    double shift = NUM2DBL(aHue);
    while (shift >= 360.0) shift -= 360.0;
    while (shift < 0.0) shift += 360.0;
    if (shift == 0.0)
        return Qnil;

    RgssBitmapCore output;
    if (!output.CreateTransparent(bitmap->Width(), bitmap->Height()))
        rb_raise(rb_eNoMemError, "Bitmap hue allocation failed");
    for (int y = 0; y < bitmap->Height(); ++y)
    {
        for (int x = 0; x < bitmap->Width(); ++x)
        {
            int red = 0, green = 0, blue = 0;
            if (!bitmap->GetPixel(x, y, &red, &green, &blue)) continue;
            const int alpha = bitmap->AlphaAt(x, y);
            const double r = red / 255.0;
            const double g = green / 255.0;
            const double b = blue / 255.0;
            double maxValue = r;
            if (g > maxValue) maxValue = g;
            if (b > maxValue) maxValue = b;
            double minValue = r;
            if (g < minValue) minValue = g;
            if (b < minValue) minValue = b;
            const double delta = maxValue - minValue;
            double hue = 0.0;
            if (delta > 0.000001)
            {
                if (maxValue == r) hue = 60.0 * ((g - b) / delta);
                else if (maxValue == g) hue = 60.0 * (2.0 + (b - r) / delta);
                else hue = 60.0 * (4.0 + (r - g) / delta);
                while (hue < 0.0) hue += 360.0;
            }
            hue += shift;
            while (hue >= 360.0) hue -= 360.0;
            const double saturation = maxValue <= 0.0 ? 0.0 : delta / maxValue;
            const double chroma = maxValue * saturation;
            const double h = hue / 60.0;
            const double hmod2 = h - 2.0 * floor(h / 2.0);
            const double secondary = chroma * (1.0 - fabs(hmod2 - 1.0));
            double rr = 0.0, gg = 0.0, bb = 0.0;
            const int sector = static_cast<int>(floor(hue / 60.0)) % 6;
            switch (sector)
            {
            case 0: rr = chroma; gg = secondary; break;
            case 1: rr = secondary; gg = chroma; break;
            case 2: gg = chroma; bb = secondary; break;
            case 3: gg = secondary; bb = chroma; break;
            case 4: rr = secondary; bb = chroma; break;
            default: rr = chroma; bb = secondary; break;
            }
            const double match = maxValue - chroma;
            output.FillRect(Rect(x, y, 1, 1),
                            static_cast<int>((rr + match) * 255.0 + 0.5),
                            static_cast<int>((gg + match) * 255.0 + 0.5),
                            static_cast<int>((bb + match) * 255.0 + 0.5), alpha);
        }
    }
    BitmapReplaceWith(bitmap, output);
    return Qnil;
}

VALUE BitmapDrawText(int aArgc, VALUE* aArgv, VALUE aSelf)
{
    RgssBitmapCore* bitmap = GetLiveBitmap(aSelf);
    Rect bounds;
    VALUE text = Qnil;
    VALUE alignValue = Qnil;

    if (aArgc == 2 || aArgc == 3)
    {
        bounds = RgssRectValue(aArgv[0]);
        text = aArgv[1];
        if (aArgc == 3)
            alignValue = aArgv[2];
    }
    else if (aArgc == 5 || aArgc == 6)
    {
        bounds.Set(NUM2INT(aArgv[0]), NUM2INT(aArgv[1]),
                   NUM2INT(aArgv[2]), NUM2INT(aArgv[3]));
        text = aArgv[4];
        if (aArgc == 6)
            alignValue = aArgv[5];
    }
    else
    {
        rb_raise(rb_eArgError, "wrong number of arguments");
    }

    text = rb_obj_as_string(text);
    int align = NIL_P(alignValue) ? 0 : NUM2INT(alignValue);
    if (align < 0 || align > 2)
        align = 0;
    const rgss::RgssFontStyle style = BitmapFontStyle(aSelf);
    rgss::RgssDrawTextUtf8(*bitmap, bounds, RSTRING_PTR(text),
                           static_cast<size_t>(RSTRING_LEN(text)), style, align);
    return Qnil;
}

VALUE BitmapTextSize(VALUE aSelf, VALUE aText)
{
    GetLiveBitmap(aSelf);
    VALUE text = rb_obj_as_string(aText);
    const rgss::RgssFontStyle style = BitmapFontStyle(aSelf);
    return RgssRectObject(rgss::RgssMeasureTextUtf8(
        RSTRING_PTR(text), static_cast<size_t>(RSTRING_LEN(text)), style));
}

VALUE BitmapFont(VALUE aSelf)
{
    GetLiveBitmap(aSelf);
    return BitmapFontObject(aSelf);
}

VALUE BitmapSetFont(VALUE aSelf, VALUE aFont)
{
    GetLiveBitmap(aSelf);
    rb_iv_set(aSelf, "@font", aFont);
    return aFont;
}

VALUE BitmapMaxSize(VALUE)
{
    return INT2FIX(4096);
}

/* -------------------------------------------------------------------------
 * Viewport and Sprite wrappers. Ruby owns reference-valued properties;
 * the portable core receives snapshots immediately before composition.
 * ---------------------------------------------------------------------- */

struct ViewportData
{
    ViewportData()
        : core(Rect()), rectRef(Qnil), colorRef(Qnil), toneRef(Qnil),
          registered(false)
    {
    }

    ViewportElement core;
    VALUE rectRef;
    VALUE colorRef;
    VALUE toneRef;
    bool registered;
};

struct SpriteData
{
    SpriteData()
        : bitmapRef(Qnil), viewportRef(Qnil), srcRectRef(Qnil),
          colorRef(Qnil), toneRef(Qnil), registered(false), angle(0.0)
    {
    }

    RgssSpriteCore core;
    VALUE bitmapRef;
    VALUE viewportRef;
    VALUE srcRectRef;
    VALUE colorRef;
    VALUE toneRef;
    bool registered;
    double angle;
};

std::vector<ViewportData*> gViewports;
std::vector<SpriteData*> gSprites;

void RemoveViewportFromList(ViewportData* aData)
{
    for (size_t i = 0; i < gViewports.size(); ++i)
    {
        if (gViewports[i] == aData)
        {
            gViewports.erase(gViewports.begin() + i);
            return;
        }
    }
}

void RemoveSpriteFromList(SpriteData* aData)
{
    for (size_t i = 0; i < gSprites.size(); ++i)
    {
        if (gSprites[i] == aData)
        {
            gSprites.erase(gSprites.begin() + i);
            return;
        }
    }
}

void ViewportMark(void* aData)
{
    ViewportData* data = static_cast<ViewportData*>(aData);
    if (!data)
        return;
    rb_gc_mark(data->rectRef);
    rb_gc_mark(data->colorRef);
    rb_gc_mark(data->toneRef);
}

void ViewportFree(void* aData)
{
    ViewportData* data = static_cast<ViewportData*>(aData);
    if (!data)
        return;
    if (gScene && data->registered)
        gScene->RemoveViewport(&data->core);
    RemoveViewportFromList(data);
    delete data;
}

const rb_data_type_t ViewportType = {
    "Viewport",
    {ViewportMark, ViewportFree, 0, 0},
    0, 0, RUBY_TYPED_FREE_IMMEDIATELY
};

VALUE ViewportAlloc(VALUE aKlass)
{
    return TypedData_Wrap_Struct(aKlass, &ViewportType, new ViewportData);
}

ViewportData* GetViewportData(VALUE aSelf)
{
    ViewportData* data = 0;
    TypedData_Get_Struct(aSelf, ViewportData, &ViewportType, data);
    if (!data)
        rgssRaise("uninitialized Viewport");
    return data;
}

ViewportElement* GetViewportCore(VALUE aObject)
{
    if (NIL_P(aObject))
        return 0;
    return &GetViewportData(aObject)->core;
}

void SpriteMark(void* aData)
{
    SpriteData* data = static_cast<SpriteData*>(aData);
    if (!data)
        return;
    rb_gc_mark(data->bitmapRef);
    rb_gc_mark(data->viewportRef);
    rb_gc_mark(data->srcRectRef);
    rb_gc_mark(data->colorRef);
    rb_gc_mark(data->toneRef);
}

void SpriteFree(void* aData)
{
    SpriteData* data = static_cast<SpriteData*>(aData);
    if (!data)
        return;
    if (gScene && data->registered)
        gScene->RemoveElement(&data->core);
    RemoveSpriteFromList(data);
    delete data;
}

const rb_data_type_t SpriteType = {
    "Sprite",
    {SpriteMark, SpriteFree, 0, 0},
    0, 0, RUBY_TYPED_FREE_IMMEDIATELY
};

VALUE SpriteAlloc(VALUE aKlass)
{
    return TypedData_Wrap_Struct(aKlass, &SpriteType, new SpriteData);
}

SpriteData* GetSpriteData(VALUE aSelf)
{
    SpriteData* data = 0;
    TypedData_Get_Struct(aSelf, SpriteData, &SpriteType, data);
    if (!data)
        rgssRaise("uninitialized Sprite");
    return data;
}

void SyncRenderState()
{
    for (size_t i = 0; i < gViewports.size(); ++i)
    {
        ViewportData* data = gViewports[i];
        if (!NIL_P(data->rectRef))
            data->core.rect = RgssRectValue(data->rectRef);
        if (!NIL_P(data->colorRef))
            data->core.color = RgssColorValue(data->colorRef);
        if (!NIL_P(data->toneRef))
            data->core.tone = RgssToneValue(data->toneRef);
    }
    for (size_t i = 0; i < gSprites.size(); ++i)
    {
        SpriteData* data = gSprites[i];
        if (!NIL_P(data->srcRectRef))
            data->core.srcRect = RgssRectValue(data->srcRectRef);
        data->core.bitmap = NIL_P(data->bitmapRef)
                                ? 0 : GetBitmap(data->bitmapRef)->core;
    }
}

VALUE ViewportInitialize(int aArgc, VALUE* aArgv, VALUE aSelf)
{
    ViewportData* data = GetViewportData(aSelf);
    Rect rect;
    if (aArgc == 0)
        rect.Set(0, 0, gGraphics ? gGraphics->Width() : 640,
                 gGraphics ? gGraphics->Height() : 480);
    else if (aArgc == 1)
        rect = RgssRectValue(aArgv[0]);
    else if (aArgc == 4)
        rect.Set(NUM2INT(aArgv[0]), NUM2INT(aArgv[1]),
                 NUM2INT(aArgv[2]), NUM2INT(aArgv[3]));
    else
        rb_raise(rb_eArgError, "wrong number of arguments");

    data->rectRef = RgssRectObject(rect);
    data->colorRef = RgssColorObject(Color(0, 0, 0, 0));
    data->toneRef = RgssToneObject(Tone(0, 0, 0, 0));
    data->core.rect = rect;
    data->core.disposed = false;
    if (!data->registered)
    {
        gScene->AddViewport(&data->core);
        data->registered = true;
        gViewports.push_back(data);
    }
    return aSelf;
}

VALUE ViewportDispose(VALUE aSelf)
{
    ViewportData* data = GetViewportData(aSelf);
    if (!data->core.disposed)
    {
        data->core.disposed = true;
        if (data->registered)
        {
            gScene->RemoveViewport(&data->core);
            data->registered = false;
        }
    }
    return Qnil;
}

VALUE ViewportDisposed(VALUE aSelf)
{
    return GetViewportData(aSelf)->core.disposed ? Qtrue : Qfalse;
}

VALUE ViewportRect(VALUE aSelf) { return GetViewportData(aSelf)->rectRef; }
VALUE ViewportSetRect(VALUE aSelf, VALUE aValue)
{
    ViewportData* data = GetViewportData(aSelf);
    data->core.rect = RgssRectValue(aValue);
    data->rectRef = aValue;
    return aValue;
}

#define VIEWPORT_INT_PROP(Name, member)                                  \
    VALUE Viewport##Name(VALUE aSelf)                                    \
    { return INT2FIX(GetViewportData(aSelf)->core.member); }             \
    VALUE ViewportSet##Name(VALUE aSelf, VALUE aValue)                   \
    { GetViewportData(aSelf)->core.member = NUM2INT(aValue); return aValue; }

VIEWPORT_INT_PROP(Z, z)
VIEWPORT_INT_PROP(Ox, ox)
VIEWPORT_INT_PROP(Oy, oy)

VALUE ViewportVisible(VALUE aSelf)
{
    return GetViewportData(aSelf)->core.visible ? Qtrue : Qfalse;
}
VALUE ViewportSetVisible(VALUE aSelf, VALUE aValue)
{
    GetViewportData(aSelf)->core.visible = RTEST(aValue);
    return aValue;
}

VALUE ViewportColor(VALUE aSelf) { return GetViewportData(aSelf)->colorRef; }
VALUE ViewportSetColor(VALUE aSelf, VALUE aValue)
{
    ViewportData* data = GetViewportData(aSelf);
    data->core.color = RgssColorValue(aValue);
    data->colorRef = aValue;
    return aValue;
}
VALUE ViewportTone(VALUE aSelf) { return GetViewportData(aSelf)->toneRef; }
VALUE ViewportSetTone(VALUE aSelf, VALUE aValue)
{
    ViewportData* data = GetViewportData(aSelf);
    data->core.tone = RgssToneValue(aValue);
    data->toneRef = aValue;
    return aValue;
}

VALUE ViewportFlash(VALUE aSelf, VALUE aColor, VALUE)
{
    ViewportData* data = GetViewportData(aSelf);
    data->core.flashColor = NIL_P(aColor) ? Color(0, 0, 0, 0)
                                          : RgssColorValue(aColor);
    return Qnil;
}
VALUE ViewportUpdate(VALUE) { return Qnil; }

VALUE SpriteInitialize(int aArgc, VALUE* aArgv, VALUE aSelf)
{
    VALUE viewport = Qnil;
    rb_scan_args(aArgc, aArgv, "01", &viewport);
    SpriteData* data = GetSpriteData(aSelf);
    data->viewportRef = viewport;
    data->core.viewport = GetViewportCore(viewport);
    data->srcRectRef = RgssRectObject(Rect());
    data->colorRef = RgssColorObject(Color(0, 0, 0, 0));
    data->toneRef = RgssToneObject(Tone(0, 0, 0, 0));
    data->core.disposed = false;
    if (!data->registered)
    {
        gScene->AddElement(&data->core);
        data->registered = true;
        gSprites.push_back(data);
    }
    return aSelf;
}

VALUE SpriteDispose(VALUE aSelf)
{
    SpriteData* data = GetSpriteData(aSelf);
    if (!data->core.disposed)
    {
        data->core.disposed = true;
        if (data->registered)
        {
            gScene->RemoveElement(&data->core);
            data->registered = false;
        }
    }
    return Qnil;
}

VALUE SpriteDisposed(VALUE aSelf)
{
    return GetSpriteData(aSelf)->core.disposed ? Qtrue : Qfalse;
}

VALUE SpriteViewport(VALUE aSelf) { return GetSpriteData(aSelf)->viewportRef; }
VALUE SpriteSetViewport(VALUE aSelf, VALUE aValue)
{
    SpriteData* data = GetSpriteData(aSelf);
    ViewportElement* viewport = GetViewportCore(aValue);
    if (data->registered)
        gScene->RemoveElement(&data->core);
    data->core.viewport = viewport;
    data->viewportRef = aValue;
    if (data->registered)
        gScene->AddElement(&data->core);
    return aValue;
}

VALUE SpriteBitmap(VALUE aSelf) { return GetSpriteData(aSelf)->bitmapRef; }
VALUE SpriteSetBitmap(VALUE aSelf, VALUE aValue)
{
    SpriteData* data = GetSpriteData(aSelf);
    if (NIL_P(aValue))
    {
        data->bitmapRef = Qnil;
        data->core.bitmap = 0;
        return aValue;
    }
    BitmapData* bitmap = GetBitmap(aValue);
    data->bitmapRef = aValue;
    data->core.bitmap = bitmap->core;
    Rect current = RgssRectValue(data->srcRectRef);
    if (current.width == 0 && current.height == 0 && !bitmap->disposed &&
        bitmap->core->IsValid())
    {
        current.Set(0, 0, bitmap->core->Width(), bitmap->core->Height());
        data->srcRectRef = RgssRectObject(current);
        data->core.srcRect = current;
    }
    return aValue;
}

VALUE SpriteSrcRect(VALUE aSelf) { return GetSpriteData(aSelf)->srcRectRef; }
VALUE SpriteSetSrcRect(VALUE aSelf, VALUE aValue)
{
    SpriteData* data = GetSpriteData(aSelf);
    data->core.srcRect = RgssRectValue(aValue);
    data->srcRectRef = aValue;
    return aValue;
}

#define SPRITE_INT_PROP(Name, member)                                    \
    VALUE Sprite##Name(VALUE aSelf)                                      \
    { return INT2FIX(GetSpriteData(aSelf)->core.member); }               \
    VALUE SpriteSet##Name(VALUE aSelf, VALUE aValue)                     \
    { GetSpriteData(aSelf)->core.member = NUM2INT(aValue); return aValue; }

SPRITE_INT_PROP(X, x)
SPRITE_INT_PROP(Y, y)
SPRITE_INT_PROP(Z, z)
SPRITE_INT_PROP(Ox, ox)
SPRITE_INT_PROP(Oy, oy)

VALUE SpriteZoomX(VALUE aSelf) { return DBL2NUM(GetSpriteData(aSelf)->core.zoomX); }
VALUE SpriteSetZoomX(VALUE aSelf, VALUE aValue)
{
    GetSpriteData(aSelf)->core.zoomX = NUM2DBL(aValue);
    return aValue;
}
VALUE SpriteZoomY(VALUE aSelf) { return DBL2NUM(GetSpriteData(aSelf)->core.zoomY); }
VALUE SpriteSetZoomY(VALUE aSelf, VALUE aValue)
{
    GetSpriteData(aSelf)->core.zoomY = NUM2DBL(aValue);
    return aValue;
}
VALUE SpriteAngle(VALUE aSelf) { return DBL2NUM(GetSpriteData(aSelf)->angle); }
VALUE SpriteSetAngle(VALUE aSelf, VALUE aValue)
{
    GetSpriteData(aSelf)->angle = NUM2DBL(aValue);
    return aValue;
}

VALUE SpriteMirror(VALUE aSelf)
{
    return GetSpriteData(aSelf)->core.mirror ? Qtrue : Qfalse;
}
VALUE SpriteSetMirror(VALUE aSelf, VALUE aValue)
{
    GetSpriteData(aSelf)->core.mirror = RTEST(aValue);
    return aValue;
}
VALUE SpriteVisible(VALUE aSelf)
{
    return GetSpriteData(aSelf)->core.visible ? Qtrue : Qfalse;
}
VALUE SpriteSetVisible(VALUE aSelf, VALUE aValue)
{
    GetSpriteData(aSelf)->core.visible = RTEST(aValue);
    return aValue;
}
VALUE SpriteOpacity(VALUE aSelf)
{
    return INT2FIX(GetSpriteData(aSelf)->core.opacity);
}
VALUE SpriteSetOpacity(VALUE aSelf, VALUE aValue)
{
    GetSpriteData(aSelf)->core.opacity = ClampByte(NUM2INT(aValue));
    return aValue;
}
VALUE SpriteBushDepth(VALUE aSelf)
{
    return INT2FIX(GetSpriteData(aSelf)->core.bushTop);
}
VALUE SpriteSetBushDepth(VALUE aSelf, VALUE aValue)
{
    int depth = NUM2INT(aValue);
    GetSpriteData(aSelf)->core.bushTop = depth < 0 ? 0 : depth;
    return aValue;
}
VALUE SpriteBushOpacity(VALUE aSelf)
{
    return INT2FIX(GetSpriteData(aSelf)->core.bushOpacity);
}
VALUE SpriteSetBushOpacity(VALUE aSelf, VALUE aValue)
{
    GetSpriteData(aSelf)->core.bushOpacity = ClampByte(NUM2INT(aValue));
    return aValue;
}
VALUE SpriteWaveAmp(VALUE aSelf) { return INT2FIX(GetSpriteData(aSelf)->core.waveAmp); }
VALUE SpriteSetWaveAmp(VALUE aSelf, VALUE aValue)
{
    GetSpriteData(aSelf)->core.waveAmp = NUM2INT(aValue);
    return aValue;
}
VALUE SpriteWaveLength(VALUE aSelf) { return INT2FIX(GetSpriteData(aSelf)->core.waveLength); }
VALUE SpriteSetWaveLength(VALUE aSelf, VALUE aValue)
{
    int value = NUM2INT(aValue);
    GetSpriteData(aSelf)->core.waveLength = value < 1 ? 1 : value;
    return aValue;
}
VALUE SpriteWaveSpeed(VALUE aSelf) { return INT2FIX(GetSpriteData(aSelf)->core.waveSpeed); }
VALUE SpriteSetWaveSpeed(VALUE aSelf, VALUE aValue)
{
    GetSpriteData(aSelf)->core.waveSpeed = NUM2INT(aValue);
    return aValue;
}
VALUE SpriteWavePhase(VALUE aSelf) { return DBL2NUM(GetSpriteData(aSelf)->core.wavePhase); }
VALUE SpriteSetWavePhase(VALUE aSelf, VALUE aValue)
{
    GetSpriteData(aSelf)->core.wavePhase = NUM2DBL(aValue);
    return aValue;
}
VALUE SpriteWidth(VALUE aSelf)
{
    return INT2FIX(GetSpriteData(aSelf)->core.srcRect.width);
}
VALUE SpriteHeight(VALUE aSelf)
{
    return INT2FIX(GetSpriteData(aSelf)->core.srcRect.height);
}
VALUE SpriteBlendType(VALUE aSelf)
{
    return INT2FIX(GetSpriteData(aSelf)->core.blendType);
}
VALUE SpriteSetBlendType(VALUE aSelf, VALUE aValue)
{
    int blend = NUM2INT(aValue);
    if (blend < rgss::KBlendNormal || blend > rgss::KBlendSubtractive)
        blend = rgss::KBlendNormal;
    GetSpriteData(aSelf)->core.blendType = blend;
    return aValue;
}
VALUE SpriteColor(VALUE aSelf) { return GetSpriteData(aSelf)->colorRef; }
VALUE SpriteSetColor(VALUE aSelf, VALUE aValue)
{
    RgssColorValue(aValue);
    GetSpriteData(aSelf)->colorRef = aValue;
    return aValue;
}
VALUE SpriteTone(VALUE aSelf) { return GetSpriteData(aSelf)->toneRef; }
VALUE SpriteSetTone(VALUE aSelf, VALUE aValue)
{
    RgssToneValue(aValue);
    GetSpriteData(aSelf)->toneRef = aValue;
    return aValue;
}
VALUE SpriteFlash(VALUE, VALUE, VALUE) { return Qnil; }
VALUE SpriteUpdate(VALUE aSelf)
{
    SpriteData* data = GetSpriteData(aSelf);
    data->core.wavePhase += static_cast<double>(data->core.waveSpeed) / 180.0;
    while (data->core.wavePhase >= 360.0) data->core.wavePhase -= 360.0;
    while (data->core.wavePhase < 0.0) data->core.wavePhase += 360.0;
    return Qnil;
}

/* -------------------------------------------------------------------------
 * Graphics
 * ---------------------------------------------------------------------- */

void PaceFrame(unsigned int aStartMs)
{
    const RgssHostVtable* host = RgssHost();
    if (!host || !host->TicksMs || !host->DelayMs || !gGraphics ||
        gGraphics->frameRate <= 0)
        return;
    const unsigned int frameMs =
        static_cast<unsigned int>(1000 / gGraphics->frameRate);
    if (frameMs == 0)
        return;
    const unsigned int elapsed = host->TicksMs(host->ctx) - aStartMs;
    if (elapsed < frameMs)
        host->DelayMs(host->ctx, frameMs - elapsed);
}

VALUE GraphicsUpdate(VALUE)
{
    const RgssHostVtable* host = RgssHost();
    const unsigned int start = host && host->TicksMs ? host->TicksMs(host->ctx) : 0;
    RgssM4BindingSync();
    SyncRenderState();
    gGraphics->Update();
    if (host && host->Present)
        host->Present(host->ctx, gGraphics->GetFrame(),
                      gGraphics->Width(), gGraphics->Height());
    if (host && host->AudioUpdate)
        host->AudioUpdate(host->ctx);
    PaceFrame(start);
    return Qnil;
}

VALUE GraphicsFreeze(VALUE)
{
    RgssM4BindingSync();
    SyncRenderState();
    gGraphics->Update();
    gGraphics->Freeze();
    return Qnil;
}

VALUE GraphicsTransition(int aArgc, VALUE* aArgv, VALUE)
{
    VALUE duration = Qnil, filename = Qnil, vague = Qnil;
    rb_scan_args(aArgc, aArgv, "03", &duration, &filename, &vague);
    (void)filename;
    gGraphics->Transition(NIL_P(duration) ? 8 : NUM2INT(duration),
                          NIL_P(vague) ? 40 : NUM2INT(vague));
    return Qnil;
}

VALUE GraphicsWait(VALUE, VALUE aDuration)
{
    int count = NUM2INT(aDuration);
    if (count < 0)
        count = 0;
    for (int i = 0; i < count; ++i)
        GraphicsUpdate(Qnil);
    return Qnil;
}
VALUE GraphicsFadeOut(VALUE, VALUE aDuration)
{
    gGraphics->FadeOut(NUM2INT(aDuration));
    return Qnil;
}
VALUE GraphicsFadeIn(VALUE, VALUE aDuration)
{
    gGraphics->FadeIn(NUM2INT(aDuration));
    return Qnil;
}
VALUE GraphicsFrameCount(VALUE) { return INT2NUM(gGraphics->frameCount); }
VALUE GraphicsSetFrameCount(VALUE, VALUE aValue)
{
    gGraphics->frameCount = NUM2INT(aValue);
    return aValue;
}
VALUE GraphicsFrameRate(VALUE) { return INT2FIX(gGraphics->frameRate); }
VALUE GraphicsSetFrameRate(VALUE, VALUE aValue)
{
    int rate = NUM2INT(aValue);
    if (rate <= 0)
        rb_raise(rb_eArgError, "frame_rate must be positive");
    gGraphics->frameRate = rate;
    RgssInputBindingSetFrameRate(rate);
    return aValue;
}
VALUE GraphicsFrameReset(VALUE)
{
    gGraphics->frameCount = 0;
    return Qnil;
}
VALUE GraphicsBrightness(VALUE) { return INT2FIX(gGraphics->Brightness()); }
VALUE GraphicsWidth(VALUE) { return INT2FIX(gGraphics->Width()); }
VALUE GraphicsHeight(VALUE) { return INT2FIX(gGraphics->Height()); }
VALUE GraphicsResizeScreen(VALUE, VALUE aWidth, VALUE aHeight)
{
    const int width = NUM2INT(aWidth);
    const int height = NUM2INT(aHeight);
    if (width <= 0 || height <= 0 || width > 4096 || height > 4096)
        rb_raise(rb_eArgError, "invalid screen size");
    if (!gGraphics->SetSize(width, height))
        rgssRaise("failed to resize Graphics");
    const RgssHostVtable* host = RgssHost();
    if (host && host->Init)
        host->Init(host->ctx, width, height);
    return Qnil;
}
VALUE GraphicsSnapToBitmap(VALUE)
{
    VALUE object = rb_obj_alloc(cBitmap);
    BitmapData* data = GetBitmap(object);
    const int width = gGraphics->Width();
    const int height = gGraphics->Height();
    if (!data->core->Create(width, height))
        rgssRaise("failed to allocate Bitmap");
    data->core->Blit(&gGraphics->BackBuffer(), Rect(0, 0, width, height),
                     0, 0, 255);
    data->disposed = false;
    return object;
}

void DefineBitmapBinding()
{
    cBitmap = rb_define_class("Bitmap", rb_cObject);
    rb_define_alloc_func(cBitmap, BitmapAlloc);
    rb_define_singleton_method(cBitmap, "max_size", RUBY_METHOD_FUNC(BitmapMaxSize), 0);
    rb_define_method(cBitmap, "initialize", RUBY_METHOD_FUNC(BitmapInitialize), -1);
    rb_define_method(cBitmap, "dispose", RUBY_METHOD_FUNC(BitmapDispose), 0);
    rb_define_method(cBitmap, "disposed?", RUBY_METHOD_FUNC(BitmapDisposed), 0);
    rb_define_method(cBitmap, "width", RUBY_METHOD_FUNC(BitmapWidth), 0);
    rb_define_method(cBitmap, "height", RUBY_METHOD_FUNC(BitmapHeight), 0);
    rb_define_method(cBitmap, "rect", RUBY_METHOD_FUNC(BitmapRect), 0);
    rb_define_method(cBitmap, "blt", RUBY_METHOD_FUNC(BitmapBlt), -1);
    rb_define_method(cBitmap, "stretch_blt", RUBY_METHOD_FUNC(BitmapStretchBlt), -1);
    rb_define_method(cBitmap, "fill_rect", RUBY_METHOD_FUNC(BitmapFillRect), -1);
    rb_define_method(cBitmap, "gradient_fill_rect", RUBY_METHOD_FUNC(BitmapGradientFillRect), -1);
    rb_define_method(cBitmap, "blur", RUBY_METHOD_FUNC(BitmapBlur), 0);
    rb_define_method(cBitmap, "radial_blur", RUBY_METHOD_FUNC(BitmapRadialBlur), 2);
    rb_define_method(cBitmap, "hue_change", RUBY_METHOD_FUNC(BitmapHueChange), 1);
    rb_define_method(cBitmap, "clear", RUBY_METHOD_FUNC(BitmapClear), 0);
    rb_define_method(cBitmap, "clear_rect", RUBY_METHOD_FUNC(BitmapClearRect), -1);
    rb_define_method(cBitmap, "get_pixel", RUBY_METHOD_FUNC(BitmapGetPixel), 2);
    rb_define_method(cBitmap, "set_pixel", RUBY_METHOD_FUNC(BitmapSetPixel), 3);
    rb_define_method(cBitmap, "hue_change", RUBY_METHOD_FUNC(BitmapHueChange), 1);
    rb_define_method(cBitmap, "draw_text", RUBY_METHOD_FUNC(BitmapDrawText), -1);
    rb_define_method(cBitmap, "text_size", RUBY_METHOD_FUNC(BitmapTextSize), 1);
    rb_define_method(cBitmap, "font", RUBY_METHOD_FUNC(BitmapFont), 0);
    rb_define_method(cBitmap, "font=", RUBY_METHOD_FUNC(BitmapSetFont), 1);
}

void DefineViewportBinding()
{
    cViewport = rb_define_class("Viewport", rb_cObject);
    rb_define_alloc_func(cViewport, ViewportAlloc);
    rb_define_method(cViewport, "initialize", RUBY_METHOD_FUNC(ViewportInitialize), -1);
    rb_define_method(cViewport, "dispose", RUBY_METHOD_FUNC(ViewportDispose), 0);
    rb_define_method(cViewport, "disposed?", RUBY_METHOD_FUNC(ViewportDisposed), 0);
    rb_define_method(cViewport, "rect", RUBY_METHOD_FUNC(ViewportRect), 0);
    rb_define_method(cViewport, "rect=", RUBY_METHOD_FUNC(ViewportSetRect), 1);
    rb_define_method(cViewport, "z", RUBY_METHOD_FUNC(ViewportZ), 0);
    rb_define_method(cViewport, "z=", RUBY_METHOD_FUNC(ViewportSetZ), 1);
    rb_define_method(cViewport, "ox", RUBY_METHOD_FUNC(ViewportOx), 0);
    rb_define_method(cViewport, "ox=", RUBY_METHOD_FUNC(ViewportSetOx), 1);
    rb_define_method(cViewport, "oy", RUBY_METHOD_FUNC(ViewportOy), 0);
    rb_define_method(cViewport, "oy=", RUBY_METHOD_FUNC(ViewportSetOy), 1);
    rb_define_method(cViewport, "visible", RUBY_METHOD_FUNC(ViewportVisible), 0);
    rb_define_method(cViewport, "visible=", RUBY_METHOD_FUNC(ViewportSetVisible), 1);
    rb_define_method(cViewport, "color", RUBY_METHOD_FUNC(ViewportColor), 0);
    rb_define_method(cViewport, "color=", RUBY_METHOD_FUNC(ViewportSetColor), 1);
    rb_define_method(cViewport, "tone", RUBY_METHOD_FUNC(ViewportTone), 0);
    rb_define_method(cViewport, "tone=", RUBY_METHOD_FUNC(ViewportSetTone), 1);
    rb_define_method(cViewport, "flash", RUBY_METHOD_FUNC(ViewportFlash), 2);
    rb_define_method(cViewport, "update", RUBY_METHOD_FUNC(ViewportUpdate), 0);
}

void DefineSpriteBinding()
{
    cSprite = rb_define_class("Sprite", rb_cObject);
    rb_define_alloc_func(cSprite, SpriteAlloc);
    rb_define_method(cSprite, "initialize", RUBY_METHOD_FUNC(SpriteInitialize), -1);
    rb_define_method(cSprite, "dispose", RUBY_METHOD_FUNC(SpriteDispose), 0);
    rb_define_method(cSprite, "disposed?", RUBY_METHOD_FUNC(SpriteDisposed), 0);
    rb_define_method(cSprite, "viewport", RUBY_METHOD_FUNC(SpriteViewport), 0);
    rb_define_method(cSprite, "viewport=", RUBY_METHOD_FUNC(SpriteSetViewport), 1);
    rb_define_method(cSprite, "bitmap", RUBY_METHOD_FUNC(SpriteBitmap), 0);
    rb_define_method(cSprite, "bitmap=", RUBY_METHOD_FUNC(SpriteSetBitmap), 1);
    rb_define_method(cSprite, "src_rect", RUBY_METHOD_FUNC(SpriteSrcRect), 0);
    rb_define_method(cSprite, "src_rect=", RUBY_METHOD_FUNC(SpriteSetSrcRect), 1);
    rb_define_method(cSprite, "x", RUBY_METHOD_FUNC(SpriteX), 0);
    rb_define_method(cSprite, "x=", RUBY_METHOD_FUNC(SpriteSetX), 1);
    rb_define_method(cSprite, "y", RUBY_METHOD_FUNC(SpriteY), 0);
    rb_define_method(cSprite, "y=", RUBY_METHOD_FUNC(SpriteSetY), 1);
    rb_define_method(cSprite, "z", RUBY_METHOD_FUNC(SpriteZ), 0);
    rb_define_method(cSprite, "z=", RUBY_METHOD_FUNC(SpriteSetZ), 1);
    rb_define_method(cSprite, "ox", RUBY_METHOD_FUNC(SpriteOx), 0);
    rb_define_method(cSprite, "ox=", RUBY_METHOD_FUNC(SpriteSetOx), 1);
    rb_define_method(cSprite, "oy", RUBY_METHOD_FUNC(SpriteOy), 0);
    rb_define_method(cSprite, "oy=", RUBY_METHOD_FUNC(SpriteSetOy), 1);
    rb_define_method(cSprite, "zoom_x", RUBY_METHOD_FUNC(SpriteZoomX), 0);
    rb_define_method(cSprite, "zoom_x=", RUBY_METHOD_FUNC(SpriteSetZoomX), 1);
    rb_define_method(cSprite, "zoom_y", RUBY_METHOD_FUNC(SpriteZoomY), 0);
    rb_define_method(cSprite, "zoom_y=", RUBY_METHOD_FUNC(SpriteSetZoomY), 1);
    rb_define_method(cSprite, "angle", RUBY_METHOD_FUNC(SpriteAngle), 0);
    rb_define_method(cSprite, "angle=", RUBY_METHOD_FUNC(SpriteSetAngle), 1);
    rb_define_method(cSprite, "mirror", RUBY_METHOD_FUNC(SpriteMirror), 0);
    rb_define_method(cSprite, "mirror=", RUBY_METHOD_FUNC(SpriteSetMirror), 1);
    rb_define_method(cSprite, "visible", RUBY_METHOD_FUNC(SpriteVisible), 0);
    rb_define_method(cSprite, "visible=", RUBY_METHOD_FUNC(SpriteSetVisible), 1);
    rb_define_method(cSprite, "opacity", RUBY_METHOD_FUNC(SpriteOpacity), 0);
    rb_define_method(cSprite, "opacity=", RUBY_METHOD_FUNC(SpriteSetOpacity), 1);
    rb_define_method(cSprite, "bush_depth", RUBY_METHOD_FUNC(SpriteBushDepth), 0);
    rb_define_method(cSprite, "bush_depth=", RUBY_METHOD_FUNC(SpriteSetBushDepth), 1);
    rb_define_method(cSprite, "bush_opacity", RUBY_METHOD_FUNC(SpriteBushOpacity), 0);
    rb_define_method(cSprite, "bush_opacity=", RUBY_METHOD_FUNC(SpriteSetBushOpacity), 1);
    rb_define_method(cSprite, "wave_amp", RUBY_METHOD_FUNC(SpriteWaveAmp), 0);
    rb_define_method(cSprite, "wave_amp=", RUBY_METHOD_FUNC(SpriteSetWaveAmp), 1);
    rb_define_method(cSprite, "wave_length", RUBY_METHOD_FUNC(SpriteWaveLength), 0);
    rb_define_method(cSprite, "wave_length=", RUBY_METHOD_FUNC(SpriteSetWaveLength), 1);
    rb_define_method(cSprite, "wave_speed", RUBY_METHOD_FUNC(SpriteWaveSpeed), 0);
    rb_define_method(cSprite, "wave_speed=", RUBY_METHOD_FUNC(SpriteSetWaveSpeed), 1);
    rb_define_method(cSprite, "wave_phase", RUBY_METHOD_FUNC(SpriteWavePhase), 0);
    rb_define_method(cSprite, "wave_phase=", RUBY_METHOD_FUNC(SpriteSetWavePhase), 1);
    rb_define_method(cSprite, "width", RUBY_METHOD_FUNC(SpriteWidth), 0);
    rb_define_method(cSprite, "height", RUBY_METHOD_FUNC(SpriteHeight), 0);
    rb_define_method(cSprite, "blend_type", RUBY_METHOD_FUNC(SpriteBlendType), 0);
    rb_define_method(cSprite, "blend_type=", RUBY_METHOD_FUNC(SpriteSetBlendType), 1);
    rb_define_method(cSprite, "color", RUBY_METHOD_FUNC(SpriteColor), 0);
    rb_define_method(cSprite, "color=", RUBY_METHOD_FUNC(SpriteSetColor), 1);
    rb_define_method(cSprite, "tone", RUBY_METHOD_FUNC(SpriteTone), 0);
    rb_define_method(cSprite, "tone=", RUBY_METHOD_FUNC(SpriteSetTone), 1);
    rb_define_method(cSprite, "flash", RUBY_METHOD_FUNC(SpriteFlash), 2);
    rb_define_method(cSprite, "update", RUBY_METHOD_FUNC(SpriteUpdate), 0);
}

void DefineGraphicsBinding()
{
    mGraphics = rb_define_module("Graphics");
    rb_define_module_function(mGraphics, "update", RUBY_METHOD_FUNC(GraphicsUpdate), 0);
    rb_define_module_function(mGraphics, "freeze", RUBY_METHOD_FUNC(GraphicsFreeze), 0);
    rb_define_module_function(mGraphics, "transition", RUBY_METHOD_FUNC(GraphicsTransition), -1);
    rb_define_module_function(mGraphics, "wait", RUBY_METHOD_FUNC(GraphicsWait), 1);
    rb_define_module_function(mGraphics, "fadeout", RUBY_METHOD_FUNC(GraphicsFadeOut), 1);
    rb_define_module_function(mGraphics, "fadein", RUBY_METHOD_FUNC(GraphicsFadeIn), 1);
    rb_define_module_function(mGraphics, "frame_count", RUBY_METHOD_FUNC(GraphicsFrameCount), 0);
    rb_define_module_function(mGraphics, "frame_count=", RUBY_METHOD_FUNC(GraphicsSetFrameCount), 1);
    rb_define_module_function(mGraphics, "frame_rate", RUBY_METHOD_FUNC(GraphicsFrameRate), 0);
    rb_define_module_function(mGraphics, "frame_rate=", RUBY_METHOD_FUNC(GraphicsSetFrameRate), 1);
    rb_define_module_function(mGraphics, "frame_reset", RUBY_METHOD_FUNC(GraphicsFrameReset), 0);
    rb_define_module_function(mGraphics, "brightness", RUBY_METHOD_FUNC(GraphicsBrightness), 0);
    rb_define_module_function(mGraphics, "width", RUBY_METHOD_FUNC(GraphicsWidth), 0);
    rb_define_module_function(mGraphics, "height", RUBY_METHOD_FUNC(GraphicsHeight), 0);
    rb_define_module_function(mGraphics, "resize_screen", RUBY_METHOD_FUNC(GraphicsResizeScreen), 2);
    rb_define_module_function(mGraphics, "snap_to_bitmap", RUBY_METHOD_FUNC(GraphicsSnapToBitmap), 0);
}

} // namespace

rgss::RgssScene* RgssRendererScene()
{
    return gScene;
}

rgss::RgssBitmapCore* RgssBitmapBindingCore(VALUE aObject)
{
    if (NIL_P(aObject))
        return 0;
    return GetBitmap(aObject)->core;
}

rgss::ViewportElement* RgssViewportBindingCore(VALUE aObject)
{
    return GetViewportCore(aObject);
}

int RgssRendererBindingInit(int aGeneration)
{
    if (!gScene)
        gScene = new RgssScene;
    if (!gGraphics)
        gGraphics = new RgssGraphicsCore(*gScene);

    const int width = aGeneration == 1 ? 640 : 544;
    const int height = aGeneration == 1 ? 480 : 416;
    if (!gGraphics->SetSize(width, height))
        return -1;

    DefineBitmapBinding();
    DefineViewportBinding();
    DefineSpriteBinding();
    DefineGraphicsBinding();
    return 0;
}
