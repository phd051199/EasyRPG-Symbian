#include "RgssBitmapBridge.h"
#include "RgssBootstrap.h"
#include "RgssGraphicsBridge.h"
#include "RgssInputBridge.h"
#include "RgssPlaneBridge.h"
#include "RgssSpriteBridge.h"
#include "RgssTableBridge.h"
#include "RgssTilemapBridge.h"
#include "RgssViewportBridge.h"
#include "RgssVmBridge.h"

#include "ruby.h"

namespace
{
enum THandleType
{
    EHandleBitmap = 1,
    EHandleSprite,
    EHandleViewport,
    EHandlePlane,
    EHandleTable,
    EHandleTilemap
};

struct THandleToken
{
    int type;
    int handle;
};

EasyRpgRgssVmHost gHost;
int gVersion = 0;
int gInitialized = 0;
VALUE gNativeModule = Qnil;
VALUE gHandleClass = Qnil;

void ResetHost()
{
    gHost.context = 0;
    gHost.file_size = 0;
    gHost.file_read = 0;
    gHost.ticks = 0;
    gHost.input = 0;
    gHost.bitmap_load = 0;
    gHost.text_decode = 0;
    gHost.frame_sync = 0;
    gHost.script_count = 0;
    gHost.script_info = 0;
    gHost.script_name = 0;
    gHost.script_inflate = 0;
    gHost.release = 0;
    gHost.dirty = 0;
    gHost.audio = 0;
    gVersion = 0;
    gInitialized = 0;
    gNativeModule = Qnil;
    gHandleClass = Qnil;
}

void DestroyNativeHandle(THandleToken* token)
{
    if (!token || token->handle == 0)
        return;
    switch (token->type)
    {
    case EHandleBitmap:
        EasyRpgRgssBitmapDestroy(token->handle);
        break;
    case EHandleSprite:
        EasyRpgRgssSpriteDestroy(token->handle);
        break;
    case EHandleViewport:
        EasyRpgRgssViewportDestroy(token->handle);
        break;
    case EHandlePlane:
        EasyRpgRgssPlaneDestroy(token->handle);
        break;
    case EHandleTable:
        EasyRpgRgssTableDestroy(token->handle);
        break;
    case EHandleTilemap:
        EasyRpgRgssTilemapDestroy(token->handle);
        break;
    default:
        break;
    }
    token->handle = 0;
}

void HandleFree(void* memory)
{
    THandleToken* token = static_cast<THandleToken*>(memory);
    DestroyNativeHandle(token);
    ruby_xfree(token);
}

VALUE NewHandle(int type, int handle)
{
    THandleToken* token = ALLOC(THandleToken);
    token->type = type;
    token->handle = handle;
    return Data_Wrap_Struct(gHandleClass, 0, HandleFree, token);
}

THandleToken* Token(VALUE value)
{
    THandleToken* token = 0;
    Data_Get_Struct(value, THandleToken, token);
    if (!token)
        rb_raise(rb_eTypeError, "invalid native handle");
    return token;
}

int Handle(VALUE value, int type)
{
    THandleToken* token = Token(value);
    if (token->type != type)
        rb_raise(rb_eTypeError, "native handle type mismatch");
    if (token->handle == 0)
        rb_raise(rb_eRuntimeError, "disposed object");
    return token->handle;
}

int OptionalHandle(VALUE value, int type)
{
    return NIL_P(value) ? 0 : Handle(value, type);
}

VALUE RaiseResult(const char* operation, int result)
{
    rb_raise(rb_eRuntimeError, "%s failed (%d)", operation, result);
    return Qnil;
}

VALUE BoolValue(int value)
{
    return value ? Qtrue : Qfalse;
}

VALUE NHandleDispose(VALUE, VALUE value)
{
    DestroyNativeHandle(Token(value));
    return Qnil;
}

VALUE NHandleDisposed(VALUE, VALUE value)
{
    return BoolValue(Token(value)->handle == 0);
}

VALUE NGraphicsUpdate(VALUE)
{
    const int result = EasyRpgRgssGraphicsUpdate();
    return result == ERgssGraphicsOk ? Qnil : RaiseResult("Graphics.update", result);
}

VALUE NGraphicsFrameRate(VALUE)
{
    const int value = EasyRpgRgssGraphicsFrameRate();
    return value >= 0 ? INT2NUM(value) : RaiseResult("Graphics.frame_rate", value);
}

VALUE NGraphicsSetFrameRate(VALUE, VALUE value)
{
    const int frameRate = NUM2INT(value);
    const int result = EasyRpgRgssGraphicsSetFrameRate(frameRate);
    return result == ERgssGraphicsOk ? value : RaiseResult("Graphics.frame_rate=", result);
}

VALUE NGraphicsFrameCount(VALUE)
{
    const int value = EasyRpgRgssGraphicsFrameCount();
    return value >= 0 ? INT2NUM(value) : RaiseResult("Graphics.frame_count", value);
}

VALUE NGraphicsSetFrameCount(VALUE, VALUE value)
{
    const int frameCount = NUM2INT(value);
    const int result = EasyRpgRgssGraphicsSetFrameCount(frameCount);
    return result == ERgssGraphicsOk ? value : RaiseResult("Graphics.frame_count=", result);
}

VALUE NGraphicsWidth(VALUE)
{
    EasyRpgRgssConfig config;
    const int result = EasyRpgRgssCoreGetConfig(&config);
    return result == ERgssOk ? INT2NUM(config.logical_width) : RaiseResult("Graphics.width", result);
}

VALUE NGraphicsHeight(VALUE)
{
    EasyRpgRgssConfig config;
    const int result = EasyRpgRgssCoreGetConfig(&config);
    return result == ERgssOk ? INT2NUM(config.logical_height) : RaiseResult("Graphics.height", result);
}

VALUE NGraphicsResize(VALUE, VALUE width, VALUE height)
{
    const int result = EasyRpgRgssCoreResizeLogical(NUM2INT(width), NUM2INT(height));
    if (result != ERgssOk)
        return RaiseResult("Graphics.resize_screen", result);
    EasyRpgRgssTilemapInvalidateAll();
    return Qnil;
}

VALUE NInputUpdate(VALUE)
{
    const unsigned int mask = gHost.input(gHost.context, 0);
    EasyRpgRgssInputSetMask(mask);
    const int result = EasyRpgRgssInputUpdate();
    return result == ERgssInputOk ? Qnil : RaiseResult("Input.update", result);
}

VALUE NInputPress(VALUE, VALUE mask)
{
    return BoolValue(EasyRpgRgssInputPress(static_cast<unsigned int>(NUM2ULONG(mask))));
}

VALUE NInputTrigger(VALUE, VALUE mask)
{
    return BoolValue(EasyRpgRgssInputTrigger(static_cast<unsigned int>(NUM2ULONG(mask))));
}

VALUE NInputRepeat(VALUE, VALUE mask)
{
    return BoolValue(EasyRpgRgssInputRepeat(static_cast<unsigned int>(NUM2ULONG(mask))));
}

VALUE NFileSize(VALUE, VALUE path)
{
    const char* relative = StringValuePtr(path);
    const int size = gHost.file_size(gHost.context, relative);
    return size >= 0 ? INT2NUM(size) : Qnil;
}

VALUE NFileRead(VALUE, VALUE path)
{
    const char* relative = StringValuePtr(path);
    const int size = gHost.file_size(gHost.context, relative);
    if (size < 0)
        return RaiseResult("file read", size);
    if (size > 32 * 1024 * 1024)
        rb_raise(rb_eRuntimeError, "file too large");
    VALUE data = rb_str_new(0, size);
    if (size > 0)
    {
        const int read = gHost.file_read(gHost.context,
                                         relative,
                                         0,
                                         RSTRING_PTR(data),
                                         size);
        if (read != size)
            return RaiseResult("file read", read);
    }
    return data;
}

int DecodeText(VALUE text, unsigned int*& codepoints)
{
    StringValue(text);
    const int length = RSTRING_LEN(text);
    codepoints = ALLOC_N(unsigned int, length > 0 ? length : 1);
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(RSTRING_PTR(text));
    int ascii = 1;
    for (int index = 0; index < length; ++index)
    {
        if (bytes[index] >= 0x80)
        {
            ascii = 0;
            break;
        }
    }
    if (ascii)
    {
        for (int index = 0; index < length; ++index)
            codepoints[index] = bytes[index];
        return length;
    }
    const int count = gHost.text_decode(gHost.context,
                                        bytes,
                                        length,
                                        codepoints,
                                        length);
    if (count < 0)
    {
        ruby_xfree(codepoints);
        codepoints = 0;
        RaiseResult("text decode", count);
    }
    return count;
}

VALUE NBitmapCreate(VALUE, VALUE width, VALUE height)
{
    int handle = 0;
    const int result = EasyRpgRgssBitmapCreate(NUM2INT(width), NUM2INT(height), &handle);
    return result == ERgssBitmapOk ? NewHandle(EHandleBitmap, handle) : RaiseResult("Bitmap.new", result);
}

VALUE NBitmapLoad(VALUE, VALUE path)
{
    int handle = 0;
    const int result = gHost.bitmap_load(gHost.context, StringValuePtr(path), &handle);
    return result == ERgssVmOk ? NewHandle(EHandleBitmap, handle) : RaiseResult("Bitmap.new", result);
}

VALUE NBitmapWidth(VALUE, VALUE token)
{
    const int result = EasyRpgRgssBitmapWidth(Handle(token, EHandleBitmap));
    return result >= 0 ? INT2NUM(result) : RaiseResult("Bitmap.width", result);
}

VALUE NBitmapHeight(VALUE, VALUE token)
{
    const int result = EasyRpgRgssBitmapHeight(Handle(token, EHandleBitmap));
    return result >= 0 ? INT2NUM(result) : RaiseResult("Bitmap.height", result);
}

VALUE NBitmapClear(VALUE, VALUE token)
{
    const int result = EasyRpgRgssBitmapClear(Handle(token, EHandleBitmap));
    return result == ERgssBitmapOk ? Qnil : RaiseResult("Bitmap.clear", result);
}

VALUE NBitmapFillRect(int argc, VALUE* argv, VALUE)
{
    if (argc != 9)
        rb_raise(rb_eArgError, "wrong number of arguments");
    const int result = EasyRpgRgssBitmapFillRect(Handle(argv[0], EHandleBitmap),
                                                 NUM2INT(argv[1]),
                                                 NUM2INT(argv[2]),
                                                 NUM2INT(argv[3]),
                                                 NUM2INT(argv[4]),
                                                 NUM2INT(argv[5]),
                                                 NUM2INT(argv[6]),
                                                 NUM2INT(argv[7]),
                                                 NUM2INT(argv[8]));
    return result == ERgssBitmapOk ? Qnil : RaiseResult("Bitmap.fill_rect", result);
}

VALUE NBitmapSetPixel(int argc, VALUE* argv, VALUE)
{
    if (argc != 7)
        rb_raise(rb_eArgError, "wrong number of arguments");
    const int result = EasyRpgRgssBitmapSetPixel(Handle(argv[0], EHandleBitmap),
                                                 NUM2INT(argv[1]),
                                                 NUM2INT(argv[2]),
                                                 NUM2INT(argv[3]),
                                                 NUM2INT(argv[4]),
                                                 NUM2INT(argv[5]),
                                                 NUM2INT(argv[6]));
    return result == ERgssBitmapOk ? Qnil : RaiseResult("Bitmap.set_pixel", result);
}

VALUE NBitmapGetPixel(VALUE, VALUE token, VALUE x, VALUE y)
{
    int red = 0;
    int green = 0;
    int blue = 0;
    int alpha = 0;
    const int result = EasyRpgRgssBitmapGetPixel(Handle(token, EHandleBitmap),
                                                 NUM2INT(x),
                                                 NUM2INT(y),
                                                 &red,
                                                 &green,
                                                 &blue,
                                                 &alpha);
    if (result != ERgssBitmapOk)
        return RaiseResult("Bitmap.get_pixel", result);
    return rb_ary_new3(4, INT2NUM(red), INT2NUM(green), INT2NUM(blue), INT2NUM(alpha));
}

VALUE NBitmapBlt(int argc, VALUE* argv, VALUE)
{
    if (argc != 9)
        rb_raise(rb_eArgError, "wrong number of arguments");
    EasyRpgRgssRect source;
    source.x = NUM2INT(argv[4]);
    source.y = NUM2INT(argv[5]);
    source.width = NUM2INT(argv[6]);
    source.height = NUM2INT(argv[7]);
    const int result = EasyRpgRgssBitmapBlt(Handle(argv[0], EHandleBitmap),
                                            NUM2INT(argv[1]),
                                            NUM2INT(argv[2]),
                                            Handle(argv[3], EHandleBitmap),
                                            &source,
                                            NUM2INT(argv[8]));
    return result == ERgssBitmapOk ? Qnil : RaiseResult("Bitmap.blt", result);
}

VALUE NBitmapStretchBlt(int argc, VALUE* argv, VALUE)
{
    if (argc != 11)
        rb_raise(rb_eArgError, "wrong number of arguments");
    EasyRpgRgssRect destination;
    destination.x = NUM2INT(argv[1]);
    destination.y = NUM2INT(argv[2]);
    destination.width = NUM2INT(argv[3]);
    destination.height = NUM2INT(argv[4]);
    EasyRpgRgssRect source;
    source.x = NUM2INT(argv[6]);
    source.y = NUM2INT(argv[7]);
    source.width = NUM2INT(argv[8]);
    source.height = NUM2INT(argv[9]);
    const int result = EasyRpgRgssBitmapStretchBlt(Handle(argv[0], EHandleBitmap),
                                                   &destination,
                                                   Handle(argv[5], EHandleBitmap),
                                                   &source,
                                                   NUM2INT(argv[10]));
    return result == ERgssBitmapOk ? Qnil : RaiseResult("Bitmap.stretch_blt", result);
}

VALUE NBitmapHueChange(VALUE, VALUE token, VALUE hue)
{
    const int result = EasyRpgRgssBitmapHueChange(Handle(token, EHandleBitmap), NUM2INT(hue));
    return result == ERgssBitmapOk ? Qnil : RaiseResult("Bitmap.hue_change", result);
}

VALUE NBitmapTextSize(int argc, VALUE* argv, VALUE)
{
    if (argc != 4)
        rb_raise(rb_eArgError, "wrong number of arguments");
    unsigned int* codepoints = 0;
    const int count = DecodeText(argv[1], codepoints);
    EasyRpgRgssTextStyle style;
    style.font_size = NUM2INT(argv[2]);
    style.red = 255;
    style.green = 255;
    style.blue = 255;
    style.alpha = 255;
    style.bold = RTEST(argv[3]) ? 1 : 0;
    style.italic = 0;
    style.shadow = 0;
    style.align = 0;
    int width = 0;
    int height = 0;
    const int result = EasyRpgRgssBitmapTextSize(Handle(argv[0], EHandleBitmap),
                                                 codepoints,
                                                 count,
                                                 &style,
                                                 &width,
                                                 &height);
    ruby_xfree(codepoints);
    if (result != ERgssBitmapOk)
        return RaiseResult("Bitmap.text_size", result);
    return rb_ary_new3(2, INT2NUM(width), INT2NUM(height));
}

VALUE NBitmapDrawText(int argc, VALUE* argv, VALUE)
{
    if (argc != 15)
        rb_raise(rb_eArgError, "wrong number of arguments");
    EasyRpgRgssRect rect;
    rect.x = NUM2INT(argv[1]);
    rect.y = NUM2INT(argv[2]);
    rect.width = NUM2INT(argv[3]);
    rect.height = NUM2INT(argv[4]);
    unsigned int* codepoints = 0;
    const int count = DecodeText(argv[5], codepoints);
    EasyRpgRgssTextStyle style;
    style.font_size = NUM2INT(argv[6]);
    style.red = NUM2INT(argv[7]);
    style.green = NUM2INT(argv[8]);
    style.blue = NUM2INT(argv[9]);
    style.alpha = NUM2INT(argv[10]);
    style.bold = RTEST(argv[11]) ? 1 : 0;
    style.italic = RTEST(argv[12]) ? 1 : 0;
    style.shadow = RTEST(argv[13]) ? 1 : 0;
    style.align = NUM2INT(argv[14]);
    const int result = EasyRpgRgssBitmapDrawText(Handle(argv[0], EHandleBitmap),
                                                 &rect,
                                                 codepoints,
                                                 count,
                                                 &style);
    ruby_xfree(codepoints);
    return result == ERgssBitmapOk ? Qnil : RaiseResult("Bitmap.draw_text", result);
}

VALUE NViewportCreate(int argc, VALUE* argv, VALUE)
{
    if (argc != 4)
        rb_raise(rb_eArgError, "wrong number of arguments");
    EasyRpgRgssRect rect;
    rect.x = NUM2INT(argv[0]);
    rect.y = NUM2INT(argv[1]);
    rect.width = NUM2INT(argv[2]);
    rect.height = NUM2INT(argv[3]);
    int handle = 0;
    const int result = EasyRpgRgssViewportCreate(&rect, &handle);
    return result == ERgssViewportOk ? NewHandle(EHandleViewport, handle) : RaiseResult("Viewport.new", result);
}

VALUE NViewportSetRect(int argc, VALUE* argv, VALUE)
{
    if (argc != 5)
        rb_raise(rb_eArgError, "wrong number of arguments");
    EasyRpgRgssRect rect;
    rect.x = NUM2INT(argv[1]);
    rect.y = NUM2INT(argv[2]);
    rect.width = NUM2INT(argv[3]);
    rect.height = NUM2INT(argv[4]);
    const int result = EasyRpgRgssViewportSetRect(Handle(argv[0], EHandleViewport), &rect);
    return result == ERgssViewportOk ? Qnil : RaiseResult("Viewport.rect=", result);
}

VALUE NViewportSetOrigin(VALUE, VALUE token, VALUE x, VALUE y)
{
    const int result = EasyRpgRgssViewportSetOrigin(Handle(token, EHandleViewport), NUM2INT(x), NUM2INT(y));
    return result == ERgssViewportOk ? Qnil : RaiseResult("Viewport.origin", result);
}

VALUE NViewportSetZ(VALUE, VALUE token, VALUE z)
{
    const int result = EasyRpgRgssViewportSetZ(Handle(token, EHandleViewport), NUM2INT(z));
    return result == ERgssViewportOk ? Qnil : RaiseResult("Viewport.z=", result);
}

VALUE NViewportSetVisible(VALUE, VALUE token, VALUE visible)
{
    const int result = EasyRpgRgssViewportSetVisible(Handle(token, EHandleViewport), RTEST(visible) ? 1 : 0);
    return result == ERgssViewportOk ? Qnil : RaiseResult("Viewport.visible=", result);
}

VALUE NPlaneCreate(VALUE)
{
    int handle = 0;
    const int result = EasyRpgRgssPlaneCreate(&handle);
    return result == ERgssPlaneOk ? NewHandle(EHandlePlane, handle) : RaiseResult("Plane.new", result);
}

VALUE NPlaneSetBitmap(VALUE, VALUE token, VALUE bitmap)
{
    const int result = EasyRpgRgssPlaneSetBitmap(Handle(token, EHandlePlane), OptionalHandle(bitmap, EHandleBitmap));
    return result == ERgssPlaneOk ? Qnil : RaiseResult("Plane.bitmap=", result);
}

VALUE NPlaneSetViewport(VALUE, VALUE token, VALUE viewport)
{
    const int result = EasyRpgRgssPlaneSetViewport(Handle(token, EHandlePlane), OptionalHandle(viewport, EHandleViewport));
    return result == ERgssPlaneOk ? Qnil : RaiseResult("Plane.viewport=", result);
}

VALUE NPlaneSetZ(VALUE, VALUE token, VALUE z)
{
    const int result = EasyRpgRgssPlaneSetZ(Handle(token, EHandlePlane), NUM2INT(z));
    return result == ERgssPlaneOk ? Qnil : RaiseResult("Plane.z=", result);
}

VALUE NPlaneSetOrigin(VALUE, VALUE token, VALUE x, VALUE y)
{
    const int result = EasyRpgRgssPlaneSetOrigin(Handle(token, EHandlePlane), NUM2INT(x), NUM2INT(y));
    return result == ERgssPlaneOk ? Qnil : RaiseResult("Plane.origin", result);
}

VALUE NPlaneSetZoom(VALUE, VALUE token, VALUE x, VALUE y)
{
    const int result = EasyRpgRgssPlaneSetZoom(Handle(token, EHandlePlane), NUM2INT(x), NUM2INT(y));
    return result == ERgssPlaneOk ? Qnil : RaiseResult("Plane.zoom", result);
}

VALUE NPlaneSetOpacity(VALUE, VALUE token, VALUE opacity)
{
    const int result = EasyRpgRgssPlaneSetOpacity(Handle(token, EHandlePlane), NUM2INT(opacity));
    return result == ERgssPlaneOk ? Qnil : RaiseResult("Plane.opacity=", result);
}

VALUE NPlaneSetVisible(VALUE, VALUE token, VALUE visible)
{
    const int result = EasyRpgRgssPlaneSetVisible(Handle(token, EHandlePlane), RTEST(visible) ? 1 : 0);
    return result == ERgssPlaneOk ? Qnil : RaiseResult("Plane.visible=", result);
}

VALUE NPlaneSetBlendType(VALUE, VALUE token, VALUE blend)
{
    const int result = EasyRpgRgssPlaneSetBlendType(Handle(token, EHandlePlane), NUM2INT(blend));
    return result == ERgssPlaneOk ? Qnil : RaiseResult("Plane.blend_type=", result);
}

VALUE NSpriteCreate(VALUE)
{
    int handle = 0;
    const int result = EasyRpgRgssSpriteCreate(&handle);
    return result == ERgssSpriteOk ? NewHandle(EHandleSprite, handle) : RaiseResult("Sprite.new", result);
}

VALUE NSpriteSetBitmap(VALUE, VALUE token, VALUE bitmap)
{
    const int result = EasyRpgRgssSpriteSetBitmap(Handle(token, EHandleSprite), OptionalHandle(bitmap, EHandleBitmap));
    return result == ERgssSpriteOk ? Qnil : RaiseResult("Sprite.bitmap=", result);
}

VALUE NSpriteSetViewport(VALUE, VALUE token, VALUE viewport)
{
    const int result = EasyRpgRgssSpriteSetViewport(Handle(token, EHandleSprite), OptionalHandle(viewport, EHandleViewport));
    return result == ERgssSpriteOk ? Qnil : RaiseResult("Sprite.viewport=", result);
}

VALUE NSpriteSetSourceRect(int argc, VALUE* argv, VALUE)
{
    if (argc != 5)
        rb_raise(rb_eArgError, "wrong number of arguments");
    EasyRpgRgssRect rect;
    rect.x = NUM2INT(argv[1]);
    rect.y = NUM2INT(argv[2]);
    rect.width = NUM2INT(argv[3]);
    rect.height = NUM2INT(argv[4]);
    const int result = EasyRpgRgssSpriteSetSourceRect(Handle(argv[0], EHandleSprite), &rect);
    return result == ERgssSpriteOk ? Qnil : RaiseResult("Sprite.src_rect=", result);
}

VALUE NSpriteSetPosition(VALUE, VALUE token, VALUE x, VALUE y, VALUE z)
{
    const int result = EasyRpgRgssSpriteSetPosition(Handle(token, EHandleSprite), NUM2INT(x), NUM2INT(y), NUM2INT(z));
    return result == ERgssSpriteOk ? Qnil : RaiseResult("Sprite.position", result);
}

VALUE NSpriteSetOrigin(VALUE, VALUE token, VALUE x, VALUE y)
{
    const int result = EasyRpgRgssSpriteSetOrigin(Handle(token, EHandleSprite), NUM2INT(x), NUM2INT(y));
    return result == ERgssSpriteOk ? Qnil : RaiseResult("Sprite.origin", result);
}

VALUE NSpriteSetZoom(VALUE, VALUE token, VALUE x, VALUE y)
{
    const int result = EasyRpgRgssSpriteSetZoom(Handle(token, EHandleSprite), NUM2INT(x), NUM2INT(y));
    return result == ERgssSpriteOk ? Qnil : RaiseResult("Sprite.zoom", result);
}

VALUE NSpriteSetOpacity(VALUE, VALUE token, VALUE opacity)
{
    const int result = EasyRpgRgssSpriteSetOpacity(Handle(token, EHandleSprite), NUM2INT(opacity));
    return result == ERgssSpriteOk ? Qnil : RaiseResult("Sprite.opacity=", result);
}

VALUE NSpriteSetVisible(VALUE, VALUE token, VALUE visible)
{
    const int result = EasyRpgRgssSpriteSetVisible(Handle(token, EHandleSprite), RTEST(visible) ? 1 : 0);
    return result == ERgssSpriteOk ? Qnil : RaiseResult("Sprite.visible=", result);
}

VALUE NSpriteSetMirror(VALUE, VALUE token, VALUE mirror)
{
    const int result = EasyRpgRgssSpriteSetMirror(Handle(token, EHandleSprite), RTEST(mirror) ? 1 : 0);
    return result == ERgssSpriteOk ? Qnil : RaiseResult("Sprite.mirror=", result);
}

VALUE NSpriteSetBlendType(VALUE, VALUE token, VALUE blend)
{
    const int result = EasyRpgRgssSpriteSetBlendType(Handle(token, EHandleSprite), NUM2INT(blend));
    return result == ERgssSpriteOk ? Qnil : RaiseResult("Sprite.blend_type=", result);
}

VALUE NSpriteSetTone(int argc, VALUE* argv, VALUE)
{
    if (argc != 5)
        rb_raise(rb_eArgError, "wrong number of arguments");
    const int result = EasyRpgRgssSpriteSetTone(Handle(argv[0], EHandleSprite),
                                                NUM2INT(argv[1]),
                                                NUM2INT(argv[2]),
                                                NUM2INT(argv[3]),
                                                NUM2INT(argv[4]));
    return result == ERgssSpriteOk ? Qnil : RaiseResult("Sprite.tone=", result);
}

VALUE NSpriteSetColor(int argc, VALUE* argv, VALUE)
{
    if (argc != 5)
        rb_raise(rb_eArgError, "wrong number of arguments");
    const int result = EasyRpgRgssSpriteSetColor(Handle(argv[0], EHandleSprite),
                                                 NUM2INT(argv[1]),
                                                 NUM2INT(argv[2]),
                                                 NUM2INT(argv[3]),
                                                 NUM2INT(argv[4]));
    return result == ERgssSpriteOk ? Qnil : RaiseResult("Sprite.color=", result);
}

VALUE NSpriteSetBush(VALUE, VALUE token, VALUE depth, VALUE opacity)
{
    const int result = EasyRpgRgssSpriteSetBush(Handle(token, EHandleSprite), NUM2INT(depth), NUM2INT(opacity));
    return result == ERgssSpriteOk ? Qnil : RaiseResult("Sprite.bush", result);
}

VALUE NTableCreate(int argc, VALUE* argv, VALUE)
{
    if (argc < 1 || argc > 3)
        rb_raise(rb_eArgError, "wrong number of arguments");
    const int dimensions = argc;
    const int x = NUM2INT(argv[0]);
    const int y = argc >= 2 ? NUM2INT(argv[1]) : 1;
    const int z = argc >= 3 ? NUM2INT(argv[2]) : 1;
    int handle = 0;
    const int result = EasyRpgRgssTableCreate(dimensions, x, y, z, &handle);
    return result == ERgssTableOk ? NewHandle(EHandleTable, handle) : RaiseResult("Table.new", result);
}

VALUE NTableDimensions(VALUE, VALUE token)
{
    const int result = EasyRpgRgssTableDimensions(Handle(token, EHandleTable));
    return result >= 0 ? INT2NUM(result) : RaiseResult("Table.dimensions", result);
}

VALUE NTableXSize(VALUE, VALUE token)
{
    const int result = EasyRpgRgssTableXSize(Handle(token, EHandleTable));
    return result >= 0 ? INT2NUM(result) : RaiseResult("Table.xsize", result);
}

VALUE NTableYSize(VALUE, VALUE token)
{
    const int result = EasyRpgRgssTableYSize(Handle(token, EHandleTable));
    return result >= 0 ? INT2NUM(result) : RaiseResult("Table.ysize", result);
}

VALUE NTableZSize(VALUE, VALUE token)
{
    const int result = EasyRpgRgssTableZSize(Handle(token, EHandleTable));
    return result >= 0 ? INT2NUM(result) : RaiseResult("Table.zsize", result);
}

VALUE NTableGet(int argc, VALUE* argv, VALUE)
{
    if (argc < 2 || argc > 4)
        rb_raise(rb_eArgError, "wrong number of arguments");
    int value = 0;
    const int result = EasyRpgRgssTableGet(Handle(argv[0], EHandleTable),
                                           NUM2INT(argv[1]),
                                           argc >= 3 ? NUM2INT(argv[2]) : 0,
                                           argc >= 4 ? NUM2INT(argv[3]) : 0,
                                           &value);
    if (result == ERgssTableErrorOutOfBounds)
        return Qnil;
    return result == ERgssTableOk ? INT2NUM(value) : RaiseResult("Table.[]", result);
}

VALUE NTableSet(int argc, VALUE* argv, VALUE)
{
    if (argc < 3 || argc > 5)
        rb_raise(rb_eArgError, "wrong number of arguments");
    int x = NUM2INT(argv[1]);
    int y = 0;
    int z = 0;
    VALUE value = Qnil;
    if (argc == 3)
        value = argv[2];
    else if (argc == 4)
    {
        y = NUM2INT(argv[2]);
        value = argv[3];
    }
    else
    {
        y = NUM2INT(argv[2]);
        z = NUM2INT(argv[3]);
        value = argv[4];
    }
    const int result = EasyRpgRgssTableSet(Handle(argv[0], EHandleTable), x, y, z, NUM2INT(value));
    if (result == ERgssTableErrorOutOfBounds)
        return Qnil;
    return result == ERgssTableOk ? value : RaiseResult("Table.[]=", result);
}

VALUE NTableResize(int argc, VALUE* argv, VALUE)
{
    if (argc < 2 || argc > 4)
        rb_raise(rb_eArgError, "wrong number of arguments");
    const int dimensions = argc - 1;
    const int result = EasyRpgRgssTableResize(Handle(argv[0], EHandleTable),
                                              dimensions,
                                              NUM2INT(argv[1]),
                                              argc >= 3 ? NUM2INT(argv[2]) : 1,
                                              argc >= 4 ? NUM2INT(argv[3]) : 1);
    return result == ERgssTableOk ? Qnil : RaiseResult("Table.resize", result);
}

void Write32(unsigned char* output, int value)
{
    const unsigned int v = static_cast<unsigned int>(value);
    output[0] = static_cast<unsigned char>(v & 0xffu);
    output[1] = static_cast<unsigned char>((v >> 8) & 0xffu);
    output[2] = static_cast<unsigned char>((v >> 16) & 0xffu);
    output[3] = static_cast<unsigned char>((v >> 24) & 0xffu);
}

int Read32(const unsigned char* input)
{
    const unsigned int value = static_cast<unsigned int>(input[0]) |
        (static_cast<unsigned int>(input[1]) << 8) |
        (static_cast<unsigned int>(input[2]) << 16) |
        (static_cast<unsigned int>(input[3]) << 24);
    return static_cast<int>(value);
}

VALUE NTableDump(VALUE, VALUE token)
{
    const int handle = Handle(token, EHandleTable);
    const int dimensions = EasyRpgRgssTableDimensions(handle);
    const int x = EasyRpgRgssTableXSize(handle);
    const int y = EasyRpgRgssTableYSize(handle);
    const int z = EasyRpgRgssTableZSize(handle);
    short* data = 0;
    int count = 0;
    const int lockResult = EasyRpgRgssTableLock(handle, &data, &count);
    if (dimensions < 1 || x < 1 || y < 1 || z < 1 || lockResult != ERgssTableOk)
        return RaiseResult("Table._dump", lockResult);
    if (count > (0x7fffffff - 20) / 2)
        rb_raise(rb_eRuntimeError, "table too large");
    VALUE output = rb_str_new(0, 20 + count * 2);
    unsigned char* bytes = reinterpret_cast<unsigned char*>(RSTRING_PTR(output));
    Write32(bytes + 0, dimensions);
    Write32(bytes + 4, x);
    Write32(bytes + 8, y);
    Write32(bytes + 12, z);
    Write32(bytes + 16, count);
    for (int index = 0; index < count; ++index)
    {
        const unsigned short value = static_cast<unsigned short>(data[index]);
        bytes[20 + index * 2] = static_cast<unsigned char>(value & 0xffu);
        bytes[21 + index * 2] = static_cast<unsigned char>((value >> 8) & 0xffu);
    }
    return output;
}

VALUE NTableLoad(VALUE, VALUE packed)
{
    StringValue(packed);
    const int length = RSTRING_LEN(packed);
    if (length < 20)
        rb_raise(rb_eArgError, "invalid table data");
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(RSTRING_PTR(packed));
    const int dimensions = Read32(bytes + 0);
    const int x = Read32(bytes + 4);
    const int y = Read32(bytes + 8);
    const int z = Read32(bytes + 12);
    const int count = Read32(bytes + 16);
    if (dimensions < 1 || dimensions > 3 || x <= 0 || y <= 0 || z <= 0 ||
        count < 0 || count > (0x7fffffff - 20) / 2 || length != 20 + count * 2)
        rb_raise(rb_eArgError, "invalid table data");
    int handle = 0;
    int result = EasyRpgRgssTableCreate(dimensions, x, y, z, &handle);
    if (result != ERgssTableOk)
        return RaiseResult("Table._load", result);
    short* data = ALLOC_N(short, count > 0 ? count : 1);
    for (int index = 0; index < count; ++index)
    {
        const unsigned short value = static_cast<unsigned short>(bytes[20 + index * 2]) |
            static_cast<unsigned short>(static_cast<unsigned short>(bytes[21 + index * 2]) << 8);
        data[index] = static_cast<short>(value);
    }
    result = EasyRpgRgssTableLoadRaw(handle, data, count);
    ruby_xfree(data);
    if (result != ERgssTableOk)
    {
        EasyRpgRgssTableDestroy(handle);
        return RaiseResult("Table._load", result);
    }
    return NewHandle(EHandleTable, handle);
}

VALUE NTilemapCreate(VALUE, VALUE viewport)
{
    int handle = 0;
    const int result = EasyRpgRgssTilemapCreate(OptionalHandle(viewport, EHandleViewport), &handle);
    return result == ERgssTilemapOk ? NewHandle(EHandleTilemap, handle) : RaiseResult("Tilemap.new", result);
}

VALUE NTilemapSetViewport(VALUE, VALUE token, VALUE viewport)
{
    const int result = EasyRpgRgssTilemapSetViewport(Handle(token, EHandleTilemap), OptionalHandle(viewport, EHandleViewport));
    return result == ERgssTilemapOk ? Qnil : RaiseResult("Tilemap.viewport=", result);
}

VALUE NTilemapSetTileset(VALUE, VALUE token, VALUE bitmap)
{
    const int result = EasyRpgRgssTilemapSetTileset(Handle(token, EHandleTilemap), OptionalHandle(bitmap, EHandleBitmap));
    return result == ERgssTilemapOk ? Qnil : RaiseResult("Tilemap.tileset=", result);
}

VALUE NTilemapSetAutotile(VALUE, VALUE token, VALUE index, VALUE bitmap)
{
    const int result = EasyRpgRgssTilemapSetAutotile(Handle(token, EHandleTilemap), NUM2INT(index), OptionalHandle(bitmap, EHandleBitmap));
    return result == ERgssTilemapOk ? Qnil : RaiseResult("Tilemap.autotiles[]=", result);
}

VALUE NTilemapSetMapData(VALUE, VALUE token, VALUE table)
{
    const int result = EasyRpgRgssTilemapSetMapData(Handle(token, EHandleTilemap), OptionalHandle(table, EHandleTable));
    return result == ERgssTilemapOk ? Qnil : RaiseResult("Tilemap.map_data=", result);
}

VALUE NTilemapSetFlashData(VALUE, VALUE token, VALUE table)
{
    const int result = EasyRpgRgssTilemapSetFlashData(Handle(token, EHandleTilemap), OptionalHandle(table, EHandleTable));
    return result == ERgssTilemapOk ? Qnil : RaiseResult("Tilemap.flash_data=", result);
}

VALUE NTilemapSetPriorities(VALUE, VALUE token, VALUE table)
{
    const int result = EasyRpgRgssTilemapSetPriorities(Handle(token, EHandleTilemap), OptionalHandle(table, EHandleTable));
    return result == ERgssTilemapOk ? Qnil : RaiseResult("Tilemap.priorities=", result);
}

VALUE NTilemapSetOrigin(VALUE, VALUE token, VALUE x, VALUE y)
{
    const int result = EasyRpgRgssTilemapSetOrigin(Handle(token, EHandleTilemap), NUM2INT(x), NUM2INT(y));
    return result == ERgssTilemapOk ? Qnil : RaiseResult("Tilemap.origin", result);
}

VALUE NTilemapSetVisible(VALUE, VALUE token, VALUE visible)
{
    const int result = EasyRpgRgssTilemapSetVisible(Handle(token, EHandleTilemap), RTEST(visible) ? 1 : 0);
    return result == ERgssTilemapOk ? Qnil : RaiseResult("Tilemap.visible=", result);
}

VALUE NTilemapUpdate(VALUE, VALUE token)
{
    const int result = EasyRpgRgssTilemapUpdate(Handle(token, EHandleTilemap));
    return result == ERgssTilemapOk ? Qnil : RaiseResult("Tilemap.update", result);
}

VALUE NAudioCommand(VALUE, VALUE command, VALUE path, VALUE volume, VALUE pitch, VALUE duration)
{
    const char* audioPath = 0;
    if (!NIL_P(path))
    {
        StringValue(path);
        audioPath = RSTRING_PTR(path);
    }
    const int result = gHost.audio(gHost.context,
                                   NUM2INT(command),
                                   audioPath,
                                   NUM2INT(volume),
                                   NUM2INT(pitch),
                                   NUM2INT(duration));
    return result == ERgssVmOk ? Qnil : RaiseResult("Audio", result);
}

void DefineNativeMethod(const char* name, VALUE (*method)(ANYARGS), int arity)
{
    rb_define_module_function(gNativeModule, name, method, arity);
}

void BindNative()
{
    gNativeModule = rb_define_module("EasyRPGNative");
    gHandleClass = rb_define_class_under(gNativeModule, "Handle", rb_cObject);

    DefineNativeMethod("handle_dispose", RUBY_METHOD_FUNC(NHandleDispose), 1);
    DefineNativeMethod("handle_disposed", RUBY_METHOD_FUNC(NHandleDisposed), 1);
    DefineNativeMethod("graphics_update", RUBY_METHOD_FUNC(NGraphicsUpdate), 0);
    DefineNativeMethod("graphics_frame_rate", RUBY_METHOD_FUNC(NGraphicsFrameRate), 0);
    DefineNativeMethod("graphics_frame_rate_set", RUBY_METHOD_FUNC(NGraphicsSetFrameRate), 1);
    DefineNativeMethod("graphics_frame_count", RUBY_METHOD_FUNC(NGraphicsFrameCount), 0);
    DefineNativeMethod("graphics_frame_count_set", RUBY_METHOD_FUNC(NGraphicsSetFrameCount), 1);
    DefineNativeMethod("graphics_width", RUBY_METHOD_FUNC(NGraphicsWidth), 0);
    DefineNativeMethod("graphics_height", RUBY_METHOD_FUNC(NGraphicsHeight), 0);
    DefineNativeMethod("graphics_resize", RUBY_METHOD_FUNC(NGraphicsResize), 2);
    DefineNativeMethod("input_update", RUBY_METHOD_FUNC(NInputUpdate), 0);
    DefineNativeMethod("input_press", RUBY_METHOD_FUNC(NInputPress), 1);
    DefineNativeMethod("input_trigger", RUBY_METHOD_FUNC(NInputTrigger), 1);
    DefineNativeMethod("input_repeat", RUBY_METHOD_FUNC(NInputRepeat), 1);
    DefineNativeMethod("file_size", RUBY_METHOD_FUNC(NFileSize), 1);
    DefineNativeMethod("file_read", RUBY_METHOD_FUNC(NFileRead), 1);
    DefineNativeMethod("bitmap_create", RUBY_METHOD_FUNC(NBitmapCreate), 2);
    DefineNativeMethod("bitmap_load", RUBY_METHOD_FUNC(NBitmapLoad), 1);
    DefineNativeMethod("bitmap_width", RUBY_METHOD_FUNC(NBitmapWidth), 1);
    DefineNativeMethod("bitmap_height", RUBY_METHOD_FUNC(NBitmapHeight), 1);
    DefineNativeMethod("bitmap_clear", RUBY_METHOD_FUNC(NBitmapClear), 1);
    DefineNativeMethod("bitmap_fill_rect", RUBY_METHOD_FUNC(NBitmapFillRect), -1);
    DefineNativeMethod("bitmap_set_pixel", RUBY_METHOD_FUNC(NBitmapSetPixel), -1);
    DefineNativeMethod("bitmap_get_pixel", RUBY_METHOD_FUNC(NBitmapGetPixel), 3);
    DefineNativeMethod("bitmap_blt", RUBY_METHOD_FUNC(NBitmapBlt), -1);
    DefineNativeMethod("bitmap_stretch_blt", RUBY_METHOD_FUNC(NBitmapStretchBlt), -1);
    DefineNativeMethod("bitmap_hue_change", RUBY_METHOD_FUNC(NBitmapHueChange), 2);
    DefineNativeMethod("bitmap_text_size", RUBY_METHOD_FUNC(NBitmapTextSize), -1);
    DefineNativeMethod("bitmap_draw_text", RUBY_METHOD_FUNC(NBitmapDrawText), -1);
    DefineNativeMethod("viewport_create", RUBY_METHOD_FUNC(NViewportCreate), -1);
    DefineNativeMethod("viewport_set_rect", RUBY_METHOD_FUNC(NViewportSetRect), -1);
    DefineNativeMethod("viewport_set_origin", RUBY_METHOD_FUNC(NViewportSetOrigin), 3);
    DefineNativeMethod("viewport_set_z", RUBY_METHOD_FUNC(NViewportSetZ), 2);
    DefineNativeMethod("viewport_set_visible", RUBY_METHOD_FUNC(NViewportSetVisible), 2);
    DefineNativeMethod("plane_create", RUBY_METHOD_FUNC(NPlaneCreate), 0);
    DefineNativeMethod("plane_set_bitmap", RUBY_METHOD_FUNC(NPlaneSetBitmap), 2);
    DefineNativeMethod("plane_set_viewport", RUBY_METHOD_FUNC(NPlaneSetViewport), 2);
    DefineNativeMethod("plane_set_z", RUBY_METHOD_FUNC(NPlaneSetZ), 2);
    DefineNativeMethod("plane_set_origin", RUBY_METHOD_FUNC(NPlaneSetOrigin), 3);
    DefineNativeMethod("plane_set_zoom", RUBY_METHOD_FUNC(NPlaneSetZoom), 3);
    DefineNativeMethod("plane_set_opacity", RUBY_METHOD_FUNC(NPlaneSetOpacity), 2);
    DefineNativeMethod("plane_set_visible", RUBY_METHOD_FUNC(NPlaneSetVisible), 2);
    DefineNativeMethod("plane_set_blend_type", RUBY_METHOD_FUNC(NPlaneSetBlendType), 2);
    DefineNativeMethod("sprite_create", RUBY_METHOD_FUNC(NSpriteCreate), 0);
    DefineNativeMethod("sprite_set_bitmap", RUBY_METHOD_FUNC(NSpriteSetBitmap), 2);
    DefineNativeMethod("sprite_set_viewport", RUBY_METHOD_FUNC(NSpriteSetViewport), 2);
    DefineNativeMethod("sprite_set_source_rect", RUBY_METHOD_FUNC(NSpriteSetSourceRect), -1);
    DefineNativeMethod("sprite_set_position", RUBY_METHOD_FUNC(NSpriteSetPosition), 4);
    DefineNativeMethod("sprite_set_origin", RUBY_METHOD_FUNC(NSpriteSetOrigin), 3);
    DefineNativeMethod("sprite_set_zoom", RUBY_METHOD_FUNC(NSpriteSetZoom), 3);
    DefineNativeMethod("sprite_set_opacity", RUBY_METHOD_FUNC(NSpriteSetOpacity), 2);
    DefineNativeMethod("sprite_set_visible", RUBY_METHOD_FUNC(NSpriteSetVisible), 2);
    DefineNativeMethod("sprite_set_mirror", RUBY_METHOD_FUNC(NSpriteSetMirror), 2);
    DefineNativeMethod("sprite_set_blend_type", RUBY_METHOD_FUNC(NSpriteSetBlendType), 2);
    DefineNativeMethod("sprite_set_tone", RUBY_METHOD_FUNC(NSpriteSetTone), -1);
    DefineNativeMethod("sprite_set_color", RUBY_METHOD_FUNC(NSpriteSetColor), -1);
    DefineNativeMethod("sprite_set_bush", RUBY_METHOD_FUNC(NSpriteSetBush), 3);
    DefineNativeMethod("table_create", RUBY_METHOD_FUNC(NTableCreate), -1);
    DefineNativeMethod("table_dimensions", RUBY_METHOD_FUNC(NTableDimensions), 1);
    DefineNativeMethod("table_xsize", RUBY_METHOD_FUNC(NTableXSize), 1);
    DefineNativeMethod("table_ysize", RUBY_METHOD_FUNC(NTableYSize), 1);
    DefineNativeMethod("table_zsize", RUBY_METHOD_FUNC(NTableZSize), 1);
    DefineNativeMethod("table_get", RUBY_METHOD_FUNC(NTableGet), -1);
    DefineNativeMethod("table_set", RUBY_METHOD_FUNC(NTableSet), -1);
    DefineNativeMethod("table_resize", RUBY_METHOD_FUNC(NTableResize), -1);
    DefineNativeMethod("table_dump", RUBY_METHOD_FUNC(NTableDump), 1);
    DefineNativeMethod("table_load", RUBY_METHOD_FUNC(NTableLoad), 1);
    DefineNativeMethod("tilemap_create", RUBY_METHOD_FUNC(NTilemapCreate), 1);
    DefineNativeMethod("tilemap_set_viewport", RUBY_METHOD_FUNC(NTilemapSetViewport), 2);
    DefineNativeMethod("tilemap_set_tileset", RUBY_METHOD_FUNC(NTilemapSetTileset), 2);
    DefineNativeMethod("tilemap_set_autotile", RUBY_METHOD_FUNC(NTilemapSetAutotile), 3);
    DefineNativeMethod("tilemap_set_map_data", RUBY_METHOD_FUNC(NTilemapSetMapData), 2);
    DefineNativeMethod("tilemap_set_flash_data", RUBY_METHOD_FUNC(NTilemapSetFlashData), 2);
    DefineNativeMethod("tilemap_set_priorities", RUBY_METHOD_FUNC(NTilemapSetPriorities), 2);
    DefineNativeMethod("tilemap_set_origin", RUBY_METHOD_FUNC(NTilemapSetOrigin), 3);
    DefineNativeMethod("tilemap_set_visible", RUBY_METHOD_FUNC(NTilemapSetVisible), 2);
    DefineNativeMethod("tilemap_update", RUBY_METHOD_FUNC(NTilemapUpdate), 1);
    DefineNativeMethod("audio_command", RUBY_METHOD_FUNC(NAudioCommand), 5);
}

int EvalProtected(const char* source)
{
    int state = 0;
    rb_eval_string_protect(source, &state);
    return state == 0 ? ERgssVmOk : ERgssVmErrorScript;
}
}

extern "C" int EasyRpgRgssVmAvailable(void)
{
    return 1;
}

extern "C" int EasyRpgRgssVmInitialize(int version,
                                        const EasyRpgRgssVmHost* host)
{
    if (version < 1 || version > 3 || !host ||
        !host->file_size || !host->file_read || !host->ticks ||
        !host->input || !host->bitmap_load || !host->text_decode ||
        !host->frame_sync || !host->script_count || !host->script_info ||
        !host->script_name || !host->script_inflate || !host->release ||
        !host->dirty || !host->audio)
        return ERgssVmErrorInvalidArgument;
    if (gInitialized)
        return ERgssVmErrorRuntime;

    ResetHost();
    gHost = *host;
    gVersion = version;
    ruby_init();
    BindNative();
    const int bootstrapResult = EvalProtected(KEasyRpgRgssBootstrap);
    if (bootstrapResult != ERgssVmOk)
    {
        ResetHost();
        return bootstrapResult;
    }
    gInitialized = 1;
    return ERgssVmOk;
}

extern "C" int EasyRpgRgssVmLoadScripts(const char*)
{
    if (!gInitialized)
        return ERgssVmErrorNotInitialized;
    const int count = gHost.script_count(gHost.context);
    if (count <= 0)
        return ERgssVmErrorScript;

    for (int index = 0; index < count; ++index)
    {
        unsigned char* source = 0;
        int sourceLength = 0;
        const int inflateResult = gHost.script_inflate(gHost.context,
                                                       index,
                                                       &source,
                                                       &sourceLength);
        if (inflateResult != ERgssVmOk || !source || sourceLength < 0)
            return ERgssVmErrorScript;
        const int evalResult = EvalProtected(reinterpret_cast<const char*>(source));
        gHost.release(gHost.context, source);
        if (evalResult != ERgssVmOk)
            return evalResult;
    }
    return ERgssVmOk;
}

extern "C" int EasyRpgRgssVmTick(void)
{
    return gInitialized ? ERgssVmOk : ERgssVmErrorNotInitialized;
}

extern "C" int EasyRpgRgssVmCollect(void)
{
    if (!gInitialized)
        return ERgssVmErrorNotInitialized;
    rb_gc_start();
    return ERgssVmOk;
}

extern "C" int EasyRpgRgssVmHeapBytes(void)
{
    return gInitialized ? 0 : ERgssVmErrorNotInitialized;
}

extern "C" void EasyRpgRgssVmShutdown(void)
{
    if (!gInitialized)
        return;
    ResetHost();
}
