#include "RgssBindingUtil.h"
#include "RgssEtcBinding.h"

using rgss::Color;
using rgss::Tone;
using rgss::Rect;

RGSS_DEFINE_TYPE(Color)
RGSS_DEFINE_TYPE(Tone)
RGSS_DEFINE_TYPE(Rect)

namespace {

VALUE cColor, cTone, cRect;

/* --- Color --- */

void ColorArgs(int aArgc, VALUE* aArgv, double* aOut)
{
    VALUE r = Qnil, g = Qnil, b = Qnil, a = Qnil;
    rb_scan_args(aArgc, aArgv, "31", &r, &g, &b, &a);
    aOut[0] = NUM2DBL(r);
    aOut[1] = NUM2DBL(g);
    aOut[2] = NUM2DBL(b);
    aOut[3] = NIL_P(a) ? 255.0 : NUM2DBL(a);
}

VALUE ColorInitialize(int aArgc, VALUE* aArgv, VALUE aSelf)
{
    double v[4];
    ColorArgs(aArgc, aArgv, v);
    RGSS_GET(aSelf, Color)->Set(v[0], v[1], v[2], v[3]);
    return aSelf;
}

VALUE ColorSet(int aArgc, VALUE* aArgv, VALUE aSelf)
{
    double v[4];
    ColorArgs(aArgc, aArgv, v);
    RGSS_GET(aSelf, Color)->Set(v[0], v[1], v[2], v[3]);
    return aSelf;
}

#define COLOR_PROP(Name, member)                                        \
    VALUE Color##Name(VALUE aSelf)                                      \
    {                                                                   \
        return DBL2NUM(RGSS_GET(aSelf, Color)->member);                 \
    }                                                                   \
    VALUE ColorSet##Name(VALUE aSelf, VALUE aValue)                     \
    {                                                                   \
        RGSS_GET(aSelf, Color)->member = NUM2DBL(aValue);               \
        return aValue;                                                  \
    }

COLOR_PROP(Red, red)
COLOR_PROP(Green, green)
COLOR_PROP(Blue, blue)
COLOR_PROP(Alpha, alpha)

VALUE ColorDump(VALUE aSelf, VALUE aLimit)
{
    VALUE result = rb_str_new(0, 32);
    rgss::SerializeColor(reinterpret_cast<unsigned char*>(RSTRING_PTR(result)),
                         *RGSS_GET(aSelf, Color));
    return result;
}

VALUE ColorLoad(VALUE aKlass, VALUE aData)
{
    rb_check_type(aData, T_STRING);

    Color* core = new Color;
    if (!rgss::DeserializeColor(
            reinterpret_cast<const unsigned char*>(RSTRING_PTR(aData)),
            RSTRING_LEN(aData), core))
    {
        delete core;
        rgssRaise("Color._load: corrupted marshal data");
    }
    VALUE object = rb_obj_alloc(aKlass);
    rgssAttach<Color>(object, core, &ColorType);
    return object;
}

/* --- Tone --- */

void ToneArgs(int aArgc, VALUE* aArgv, double* aOut)
{
    VALUE r = Qnil, g = Qnil, b = Qnil, gray = Qnil;
    rb_scan_args(aArgc, aArgv, "04", &r, &g, &b, &gray);
    aOut[0] = NIL_P(r) ? 0.0 : NUM2DBL(r);
    aOut[1] = NIL_P(g) ? 0.0 : NUM2DBL(g);
    aOut[2] = NIL_P(b) ? 0.0 : NUM2DBL(b);
    aOut[3] = NIL_P(gray) ? 0.0 : NUM2DBL(gray);
}

VALUE ToneInitialize(int aArgc, VALUE* aArgv, VALUE aSelf)
{
    double v[4];
    ToneArgs(aArgc, aArgv, v);
    RGSS_GET(aSelf, Tone)->Set(v[0], v[1], v[2], v[3]);
    return aSelf;
}

VALUE ToneSet(int aArgc, VALUE* aArgv, VALUE aSelf)
{
    double v[4];
    ToneArgs(aArgc, aArgv, v);
    RGSS_GET(aSelf, Tone)->Set(v[0], v[1], v[2], v[3]);
    return aSelf;
}

#define TONE_PROP(Name, member)                                         \
    VALUE Tone##Name(VALUE aSelf)                                       \
    {                                                                   \
        return DBL2NUM(RGSS_GET(aSelf, Tone)->member);                  \
    }                                                                   \
    VALUE ToneSet##Name(VALUE aSelf, VALUE aValue)                      \
    {                                                                   \
        RGSS_GET(aSelf, Tone)->member = NUM2DBL(aValue);                \
        return aValue;                                                  \
    }

TONE_PROP(Red, red)
TONE_PROP(Green, green)
TONE_PROP(Blue, blue)
TONE_PROP(Gray, gray)

VALUE ToneDump(VALUE aSelf, VALUE aLimit)
{
    VALUE result = rb_str_new(0, 32);
    rgss::SerializeTone(reinterpret_cast<unsigned char*>(RSTRING_PTR(result)),
                        *RGSS_GET(aSelf, Tone));
    return result;
}

VALUE ToneLoad(VALUE aKlass, VALUE aData)
{
    rb_check_type(aData, T_STRING);

    Tone* core = new Tone;
    if (!rgss::DeserializeTone(
            reinterpret_cast<const unsigned char*>(RSTRING_PTR(aData)),
            RSTRING_LEN(aData), core))
    {
        delete core;
        rgssRaise("Tone._load: corrupted marshal data");
    }
    VALUE object = rb_obj_alloc(aKlass);
    rgssAttach<Tone>(object, core, &ToneType);
    return object;
}

/* --- Rect --- */

void RectArgs(int aArgc, VALUE* aArgv, int* aOut)
{
    VALUE x = Qnil, y = Qnil, w = Qnil, h = Qnil;
    rb_scan_args(aArgc, aArgv, "04", &x, &y, &w, &h);
    aOut[0] = NIL_P(x) ? 0 : NUM2INT(x);
    aOut[1] = NIL_P(y) ? 0 : NUM2INT(y);
    aOut[2] = NIL_P(w) ? 0 : NUM2INT(w);
    aOut[3] = NIL_P(h) ? 0 : NUM2INT(h);
}

VALUE RectInitialize(int aArgc, VALUE* aArgv, VALUE aSelf)
{
    int v[4];
    RectArgs(aArgc, aArgv, v);
    RGSS_GET(aSelf, Rect)->Set(v[0], v[1], v[2], v[3]);
    return aSelf;
}

VALUE RectSet(int aArgc, VALUE* aArgv, VALUE aSelf)
{
    int v[4];
    RectArgs(aArgc, aArgv, v);
    RGSS_GET(aSelf, Rect)->Set(v[0], v[1], v[2], v[3]);
    return aSelf;
}

#define RECT_PROP(Name, member)                                         \
    VALUE Rect##Name(VALUE aSelf)                                       \
    {                                                                   \
        return INT2FIX(RGSS_GET(aSelf, Rect)->member);                  \
    }                                                                   \
    VALUE RectSet##Name(VALUE aSelf, VALUE aValue)                      \
    {                                                                   \
        RGSS_GET(aSelf, Rect)->member = NUM2INT(aValue);                \
        return aValue;                                                  \
    }

RECT_PROP(X, x)
RECT_PROP(Y, y)
RECT_PROP(Width, width)
RECT_PROP(Height, height)

VALUE RectEmpty(VALUE aSelf)
{
    return RGSS_GET(aSelf, Rect)->IsEmpty() ? Qtrue : Qfalse;
}

VALUE RectDump(VALUE aSelf, VALUE aLimit)
{
    VALUE result = rb_str_new(0, 16);
    rgss::SerializeRect(reinterpret_cast<unsigned char*>(RSTRING_PTR(result)),
                        *RGSS_GET(aSelf, Rect));
    return result;
}

VALUE RectLoad(VALUE aKlass, VALUE aData)
{
    rb_check_type(aData, T_STRING);

    Rect* core = new Rect;
    if (!rgss::DeserializeRect(
            reinterpret_cast<const unsigned char*>(RSTRING_PTR(aData)),
            RSTRING_LEN(aData), core))
    {
        delete core;
        rgssRaise("Rect._load: corrupted marshal data");
    }
    VALUE object = rb_obj_alloc(aKlass);
    rgssAttach<Rect>(object, core, &RectType);
    return object;
}

} // namespace

rgss::Rect RgssRectValue(VALUE aObject)
{
    return *RGSS_GET(aObject, Rect);
}

VALUE RgssRectObject(const rgss::Rect& aRect)
{
    VALUE object = rb_obj_alloc(cRect);
    *RGSS_GET(object, Rect) = aRect;
    return object;
}

rgss::Color RgssColorValue(VALUE aObject)
{
    return *RGSS_GET(aObject, Color);
}

VALUE RgssColorObject(const rgss::Color& aColor)
{
    VALUE object = rb_obj_alloc(cColor);
    *RGSS_GET(object, Color) = aColor;
    return object;
}

rgss::Tone RgssToneValue(VALUE aObject)
{
    return *RGSS_GET(aObject, Tone);
}

VALUE RgssToneObject(const rgss::Tone& aTone)
{
    VALUE object = rb_obj_alloc(cTone);
    *RGSS_GET(object, Tone) = aTone;
    return object;
}

void RgssColorBindingInit()
{
    cColor = rb_define_class("Color", rb_cObject);
    rb_define_alloc_func(cColor, ColorAlloc);
    rb_define_method(cColor, "initialize", RUBY_METHOD_FUNC(ColorInitialize), -1);
    rb_define_method(cColor, "set", RUBY_METHOD_FUNC(ColorSet), -1);
    rb_define_method(cColor, "red", RUBY_METHOD_FUNC(ColorRed), 0);
    rb_define_method(cColor, "red=", RUBY_METHOD_FUNC(ColorSetRed), 1);
    rb_define_method(cColor, "green", RUBY_METHOD_FUNC(ColorGreen), 0);
    rb_define_method(cColor, "green=", RUBY_METHOD_FUNC(ColorSetGreen), 1);
    rb_define_method(cColor, "blue", RUBY_METHOD_FUNC(ColorBlue), 0);
    rb_define_method(cColor, "blue=", RUBY_METHOD_FUNC(ColorSetBlue), 1);
    rb_define_method(cColor, "alpha", RUBY_METHOD_FUNC(ColorAlpha), 0);
    rb_define_method(cColor, "alpha=", RUBY_METHOD_FUNC(ColorSetAlpha), 1);
    rb_define_method(cColor, "_dump", RUBY_METHOD_FUNC(ColorDump), 1);
    rb_define_singleton_method(cColor, "_load", RUBY_METHOD_FUNC(ColorLoad), 1);
}

void RgssToneBindingInit()
{
    cTone = rb_define_class("Tone", rb_cObject);
    rb_define_alloc_func(cTone, ToneAlloc);
    rb_define_method(cTone, "initialize", RUBY_METHOD_FUNC(ToneInitialize), -1);
    rb_define_method(cTone, "set", RUBY_METHOD_FUNC(ToneSet), -1);
    rb_define_method(cTone, "red", RUBY_METHOD_FUNC(ToneRed), 0);
    rb_define_method(cTone, "red=", RUBY_METHOD_FUNC(ToneSetRed), 1);
    rb_define_method(cTone, "green", RUBY_METHOD_FUNC(ToneGreen), 0);
    rb_define_method(cTone, "green=", RUBY_METHOD_FUNC(ToneSetGreen), 1);
    rb_define_method(cTone, "blue", RUBY_METHOD_FUNC(ToneBlue), 0);
    rb_define_method(cTone, "blue=", RUBY_METHOD_FUNC(ToneSetBlue), 1);
    rb_define_method(cTone, "gray", RUBY_METHOD_FUNC(ToneGray), 0);
    rb_define_method(cTone, "gray=", RUBY_METHOD_FUNC(ToneSetGray), 1);
    rb_define_method(cTone, "_dump", RUBY_METHOD_FUNC(ToneDump), 1);
    rb_define_singleton_method(cTone, "_load", RUBY_METHOD_FUNC(ToneLoad), 1);
}

void RgssRectBindingInit()
{
    cRect = rb_define_class("Rect", rb_cObject);
    rb_define_alloc_func(cRect, RectAlloc);
    rb_define_method(cRect, "initialize", RUBY_METHOD_FUNC(RectInitialize), -1);
    rb_define_method(cRect, "set", RUBY_METHOD_FUNC(RectSet), -1);
    rb_define_method(cRect, "x", RUBY_METHOD_FUNC(RectX), 0);
    rb_define_method(cRect, "x=", RUBY_METHOD_FUNC(RectSetX), 1);
    rb_define_method(cRect, "y", RUBY_METHOD_FUNC(RectY), 0);
    rb_define_method(cRect, "y=", RUBY_METHOD_FUNC(RectSetY), 1);
    rb_define_method(cRect, "width", RUBY_METHOD_FUNC(RectWidth), 0);
    rb_define_method(cRect, "width=", RUBY_METHOD_FUNC(RectSetWidth), 1);
    rb_define_method(cRect, "height", RUBY_METHOD_FUNC(RectHeight), 0);
    rb_define_method(cRect, "height=", RUBY_METHOD_FUNC(RectSetHeight), 1);
    rb_define_method(cRect, "empty?", RUBY_METHOD_FUNC(RectEmpty), 0);
    rb_define_method(cRect, "_dump", RUBY_METHOD_FUNC(RectDump), 1);
    rb_define_singleton_method(cRect, "_load", RUBY_METHOD_FUNC(RectLoad), 1);
}
