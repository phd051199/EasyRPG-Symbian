#include "RgssBindingUtil.h"
#include "RgssEtcBinding.h"
#include "RgssRendererBinding.h"
#include "RgssTableBinding.h"

#include "../core/RgssBitmapCore.h"
#include "../core/RgssPlaneCore.h"
#include "../core/RgssTilemapCore.h"
#include "../core/RgssWindowCore.h"

#include <vector>

namespace {

using rgss::Color;
using rgss::RgssBitmapCore;
using rgss::RgssPlaneCore;
using rgss::RgssScene;
using rgss::RgssTilemapCore;
using rgss::RgssWindowCore;
using rgss::Tone;
using rgss::ViewportElement;

VALUE cPlane = Qnil;
VALUE cWindow = Qnil;
VALUE cTilemap = Qnil;
int gGeneration = 1;

int ClampByte(int aValue)
{
    if (aValue < 0) return 0;
    if (aValue > 255) return 255;
    return aValue;
}

template<typename T>
void RemovePointer(std::vector<T*>& aList, T* aValue)
{
    for (size_t i = 0; i < aList.size(); ++i)
    {
        if (aList[i] == aValue)
        {
            aList.erase(aList.begin() + i);
            return;
        }
    }
}

template<typename T>
void RebindElement(T* aData, VALUE aViewport)
{
    RgssScene* scene = RgssRendererScene();
    if (aData->registered)
        scene->RemoveElement(&aData->core);
    aData->viewportRef = aViewport;
    aData->core.viewport = RgssViewportBindingCore(aViewport);
    if (aData->registered)
        scene->AddElement(&aData->core);
}

/* ---------------------------------------------------------------------
 * Plane
 * ------------------------------------------------------------------ */

struct PlaneData
{
    PlaneData()
        : bitmapRef(Qnil), viewportRef(Qnil), colorRef(Qnil), toneRef(Qnil),
          registered(false)
    {
    }

    RgssPlaneCore core;
    VALUE bitmapRef;
    VALUE viewportRef;
    VALUE colorRef;
    VALUE toneRef;
    bool registered;
};

std::vector<PlaneData*> gPlanes;

void PlaneMark(void* aPointer)
{
    PlaneData* data = static_cast<PlaneData*>(aPointer);
    if (!data) return;
    rb_gc_mark(data->bitmapRef);
    rb_gc_mark(data->viewportRef);
    rb_gc_mark(data->colorRef);
    rb_gc_mark(data->toneRef);
}

void PlaneFree(void* aPointer)
{
    PlaneData* data = static_cast<PlaneData*>(aPointer);
    if (!data) return;
    if (data->registered && RgssRendererScene())
        RgssRendererScene()->RemoveElement(&data->core);
    RemovePointer(gPlanes, data);
    delete data;
}

const rb_data_type_t PlaneType = {
    "Plane", {PlaneMark, PlaneFree, 0, 0}, 0, 0,
    RUBY_TYPED_FREE_IMMEDIATELY
};

VALUE PlaneAlloc(VALUE aKlass)
{
    return TypedData_Wrap_Struct(aKlass, &PlaneType, new PlaneData);
}

PlaneData* GetPlane(VALUE aSelf)
{
    PlaneData* data = 0;
    TypedData_Get_Struct(aSelf, PlaneData, &PlaneType, data);
    if (!data) rgssRaise("uninitialized Plane");
    return data;
}

VALUE PlaneInitialize(int aArgc, VALUE* aArgv, VALUE aSelf)
{
    VALUE viewport = Qnil;
    rb_scan_args(aArgc, aArgv, "01", &viewport);
    PlaneData* data = GetPlane(aSelf);
    data->viewportRef = viewport;
    data->core.viewport = RgssViewportBindingCore(viewport);
    data->colorRef = RgssColorObject(Color(0, 0, 0, 0));
    data->toneRef = RgssToneObject(Tone(0, 0, 0, 0));
    data->core.disposed = false;
    if (!data->registered)
    {
        RgssRendererScene()->AddElement(&data->core);
        data->registered = true;
        gPlanes.push_back(data);
    }
    return aSelf;
}

VALUE PlaneDispose(VALUE aSelf)
{
    PlaneData* data = GetPlane(aSelf);
    if (!data->core.disposed)
    {
        data->core.disposed = true;
        if (data->registered)
        {
            RgssRendererScene()->RemoveElement(&data->core);
            data->registered = false;
        }
    }
    return Qnil;
}

VALUE PlaneDisposed(VALUE aSelf)
{
    return GetPlane(aSelf)->core.disposed ? Qtrue : Qfalse;
}
VALUE PlaneViewport(VALUE aSelf) { return GetPlane(aSelf)->viewportRef; }
VALUE PlaneSetViewport(VALUE aSelf, VALUE aValue)
{
    RebindElement(GetPlane(aSelf), aValue);
    return aValue;
}
VALUE PlaneBitmap(VALUE aSelf) { return GetPlane(aSelf)->bitmapRef; }
VALUE PlaneSetBitmap(VALUE aSelf, VALUE aValue)
{
    PlaneData* data = GetPlane(aSelf);
    data->bitmapRef = aValue;
    data->core.bitmap = RgssBitmapBindingCore(aValue);
    return aValue;
}

#define PLANE_INT_PROP(Name, member)                                     \
    VALUE Plane##Name(VALUE aSelf)                                       \
    { return INT2FIX(GetPlane(aSelf)->core.member); }                    \
    VALUE PlaneSet##Name(VALUE aSelf, VALUE aValue)                      \
    { GetPlane(aSelf)->core.member = NUM2INT(aValue); return aValue; }

PLANE_INT_PROP(Z, z)
PLANE_INT_PROP(Ox, ox)
PLANE_INT_PROP(Oy, oy)

VALUE PlaneZoomX(VALUE aSelf) { return DBL2NUM(GetPlane(aSelf)->core.zoomX); }
VALUE PlaneSetZoomX(VALUE aSelf, VALUE aValue)
{
    GetPlane(aSelf)->core.zoomX = NUM2DBL(aValue);
    return aValue;
}
VALUE PlaneZoomY(VALUE aSelf) { return DBL2NUM(GetPlane(aSelf)->core.zoomY); }
VALUE PlaneSetZoomY(VALUE aSelf, VALUE aValue)
{
    GetPlane(aSelf)->core.zoomY = NUM2DBL(aValue);
    return aValue;
}
VALUE PlaneOpacity(VALUE aSelf) { return INT2FIX(GetPlane(aSelf)->core.opacity); }
VALUE PlaneSetOpacity(VALUE aSelf, VALUE aValue)
{
    GetPlane(aSelf)->core.opacity = ClampByte(NUM2INT(aValue));
    return aValue;
}
VALUE PlaneBlendType(VALUE aSelf) { return INT2FIX(GetPlane(aSelf)->core.blendType); }
VALUE PlaneSetBlendType(VALUE aSelf, VALUE aValue)
{
    int type = NUM2INT(aValue);
    if (type < rgss::KBlendNormal || type > rgss::KBlendSubtractive)
        type = rgss::KBlendNormal;
    GetPlane(aSelf)->core.blendType = type;
    return aValue;
}
VALUE PlaneVisible(VALUE aSelf) { return GetPlane(aSelf)->core.visible ? Qtrue : Qfalse; }
VALUE PlaneSetVisible(VALUE aSelf, VALUE aValue)
{
    GetPlane(aSelf)->core.visible = RTEST(aValue);
    return aValue;
}
VALUE PlaneColor(VALUE aSelf) { return GetPlane(aSelf)->colorRef; }
VALUE PlaneSetColor(VALUE aSelf, VALUE aValue)
{
    PlaneData* data = GetPlane(aSelf);
    data->core.color = RgssColorValue(aValue);
    data->colorRef = aValue;
    return aValue;
}
VALUE PlaneTone(VALUE aSelf) { return GetPlane(aSelf)->toneRef; }
VALUE PlaneSetTone(VALUE aSelf, VALUE aValue)
{
    PlaneData* data = GetPlane(aSelf);
    data->core.tone = RgssToneValue(aValue);
    data->toneRef = aValue;
    return aValue;
}

/* ---------------------------------------------------------------------
 * Window
 * ------------------------------------------------------------------ */

struct WindowData
{
    WindowData()
        : windowskinRef(Qnil), contentsRef(Qnil), cursorRectRef(Qnil), toneRef(Qnil),
          viewportRef(Qnil), registered(false)
    {
    }

    RgssWindowCore core;
    VALUE windowskinRef;
    VALUE contentsRef;
    VALUE cursorRectRef;
    VALUE toneRef;
    VALUE viewportRef;
    bool registered;
};

std::vector<WindowData*> gWindows;

void WindowMark(void* aPointer)
{
    WindowData* data = static_cast<WindowData*>(aPointer);
    if (!data) return;
    rb_gc_mark(data->windowskinRef);
    rb_gc_mark(data->contentsRef);
    rb_gc_mark(data->cursorRectRef);
    rb_gc_mark(data->toneRef);
    rb_gc_mark(data->viewportRef);
}

void WindowFree(void* aPointer)
{
    WindowData* data = static_cast<WindowData*>(aPointer);
    if (!data) return;
    if (data->registered && RgssRendererScene())
        RgssRendererScene()->RemoveElement(&data->core);
    RemovePointer(gWindows, data);
    delete data;
}

const rb_data_type_t WindowType = {
    "Window", {WindowMark, WindowFree, 0, 0}, 0, 0,
    RUBY_TYPED_FREE_IMMEDIATELY
};

VALUE WindowAlloc(VALUE aKlass)
{
    return TypedData_Wrap_Struct(aKlass, &WindowType, new WindowData);
}

WindowData* GetWindow(VALUE aSelf)
{
    WindowData* data = 0;
    TypedData_Get_Struct(aSelf, WindowData, &WindowType, data);
    if (!data) rgssRaise("uninitialized Window");
    return data;
}

VALUE WindowInitialize(int aArgc, VALUE* aArgv, VALUE aSelf)
{
    VALUE viewport = Qnil;
    rb_scan_args(aArgc, aArgv, "01", &viewport);
    WindowData* data = GetWindow(aSelf);
    data->viewportRef = viewport;
    data->core.viewport = RgssViewportBindingCore(viewport);
    data->core.generation = gGeneration;
    data->core.padding = gGeneration >= 3 ? 12 : 16;
    data->core.paddingBottom = data->core.padding;
    data->cursorRectRef = RgssRectObject(rgss::Rect());
    data->toneRef = RgssToneObject(Tone(0, 0, 0, 0));
    data->core.tone = RgssToneValue(data->toneRef);
    data->core.disposed = false;
    if (!data->registered)
    {
        RgssRendererScene()->AddElement(&data->core);
        data->registered = true;
        gWindows.push_back(data);
    }
    return aSelf;
}

VALUE WindowDispose(VALUE aSelf)
{
    WindowData* data = GetWindow(aSelf);
    if (!data->core.disposed)
    {
        data->core.disposed = true;
        if (data->registered)
        {
            RgssRendererScene()->RemoveElement(&data->core);
            data->registered = false;
        }
    }
    return Qnil;
}
VALUE WindowDisposed(VALUE aSelf) { return GetWindow(aSelf)->core.disposed ? Qtrue : Qfalse; }
VALUE WindowViewport(VALUE aSelf) { return GetWindow(aSelf)->viewportRef; }
VALUE WindowSetViewport(VALUE aSelf, VALUE aValue)
{
    RebindElement(GetWindow(aSelf), aValue);
    return aValue;
}
VALUE WindowWindowskin(VALUE aSelf) { return GetWindow(aSelf)->windowskinRef; }
VALUE WindowSetWindowskin(VALUE aSelf, VALUE aValue)
{
    WindowData* data = GetWindow(aSelf);
    data->windowskinRef = aValue;
    data->core.windowskin = RgssBitmapBindingCore(aValue);
    return aValue;
}
VALUE WindowContents(VALUE aSelf) { return GetWindow(aSelf)->contentsRef; }
VALUE WindowSetContents(VALUE aSelf, VALUE aValue)
{
    WindowData* data = GetWindow(aSelf);
    data->contentsRef = aValue;
    data->core.contents = RgssBitmapBindingCore(aValue);
    return aValue;
}
VALUE WindowCursorRect(VALUE aSelf) { return GetWindow(aSelf)->cursorRectRef; }
VALUE WindowSetCursorRect(VALUE aSelf, VALUE aValue)
{
    WindowData* data = GetWindow(aSelf);
    data->core.cursorRect = RgssRectValue(aValue);
    data->cursorRectRef = aValue;
    return aValue;
}

#define WINDOW_INT_PROP(Name, member)                                    \
    VALUE Window##Name(VALUE aSelf)                                      \
    { return INT2FIX(GetWindow(aSelf)->core.member); }                   \
    VALUE WindowSet##Name(VALUE aSelf, VALUE aValue)                     \
    { GetWindow(aSelf)->core.member = NUM2INT(aValue); return aValue; }

WINDOW_INT_PROP(X, x)
WINDOW_INT_PROP(Y, y)
WINDOW_INT_PROP(Width, width)
WINDOW_INT_PROP(Height, height)
WINDOW_INT_PROP(Z, z)
WINDOW_INT_PROP(Ox, ox)
WINDOW_INT_PROP(Oy, oy)

VALUE WindowOpacity(VALUE aSelf) { return INT2FIX(GetWindow(aSelf)->core.opacity); }
VALUE WindowSetOpacity(VALUE aSelf, VALUE aValue)
{
    GetWindow(aSelf)->core.opacity = ClampByte(NUM2INT(aValue)); return aValue;
}
VALUE WindowBackOpacity(VALUE aSelf) { return INT2FIX(GetWindow(aSelf)->core.backOpacity); }
VALUE WindowSetBackOpacity(VALUE aSelf, VALUE aValue)
{
    GetWindow(aSelf)->core.backOpacity = ClampByte(NUM2INT(aValue)); return aValue;
}
VALUE WindowTone(VALUE aSelf)
{
    return GetWindow(aSelf)->toneRef;
}

VALUE WindowSetTone(VALUE aSelf, VALUE aValue)
{
    WindowData* data = GetWindow(aSelf);
    data->core.tone = RgssToneValue(aValue);
    data->toneRef = aValue;
    return aValue;
}

VALUE WindowContentsOpacity(VALUE aSelf) { return INT2FIX(GetWindow(aSelf)->core.contentsOpacity); }
VALUE WindowSetContentsOpacity(VALUE aSelf, VALUE aValue)
{
    GetWindow(aSelf)->core.contentsOpacity = ClampByte(NUM2INT(aValue)); return aValue;
}
VALUE WindowOpenness(VALUE aSelf) { return INT2FIX(GetWindow(aSelf)->core.openness); }
VALUE WindowSetOpenness(VALUE aSelf, VALUE aValue)
{
    GetWindow(aSelf)->core.openness = ClampByte(NUM2INT(aValue)); return aValue;
}
VALUE WindowPadding(VALUE aSelf) { return INT2FIX(GetWindow(aSelf)->core.padding); }
VALUE WindowSetPadding(VALUE aSelf, VALUE aValue)
{
    int value = NUM2INT(aValue);
    GetWindow(aSelf)->core.padding = value < 0 ? 0 : value;
    return aValue;
}
VALUE WindowPaddingBottom(VALUE aSelf) { return INT2FIX(GetWindow(aSelf)->core.paddingBottom); }
VALUE WindowSetPaddingBottom(VALUE aSelf, VALUE aValue)
{
    int value = NUM2INT(aValue);
    GetWindow(aSelf)->core.paddingBottom = value < 0 ? 0 : value;
    return aValue;
}
VALUE WindowOpenQ(VALUE aSelf) { return GetWindow(aSelf)->core.openness >= 255 ? Qtrue : Qfalse; }
VALUE WindowCloseQ(VALUE aSelf) { return GetWindow(aSelf)->core.openness <= 0 ? Qtrue : Qfalse; }
VALUE WindowMove(VALUE aSelf, VALUE aX, VALUE aY, VALUE aWidth, VALUE aHeight)
{
    WindowData* data = GetWindow(aSelf);
    data->core.x = NUM2INT(aX);
    data->core.y = NUM2INT(aY);
    data->core.width = NUM2INT(aWidth);
    data->core.height = NUM2INT(aHeight);
    return aSelf;
}

#define WINDOW_BOOL_PROP(Name, member)                                   \
    VALUE Window##Name(VALUE aSelf)                                      \
    { return GetWindow(aSelf)->core.member ? Qtrue : Qfalse; }           \
    VALUE WindowSet##Name(VALUE aSelf, VALUE aValue)                     \
    { GetWindow(aSelf)->core.member = RTEST(aValue); return aValue; }

WINDOW_BOOL_PROP(Visible, visible)
WINDOW_BOOL_PROP(Active, active)
WINDOW_BOOL_PROP(Pause, pause)
WINDOW_BOOL_PROP(Stretch, stretch)
WINDOW_BOOL_PROP(ArrowsVisible, arrowsVisible)
VALUE WindowUpdate(VALUE) { return Qnil; }

/* ---------------------------------------------------------------------
 * Tilemap
 * ------------------------------------------------------------------ */

struct TilemapData
{
    TilemapData()
        : tilesetRef(Qnil), autotilesRef(Qnil), bitmapsRef(Qnil), mapDataRef(Qnil),
          flashDataRef(Qnil), prioritiesRef(Qnil), flagsRef(Qnil), viewportRef(Qnil),
          registered(false)
    {
    }

    RgssTilemapCore core;
    VALUE tilesetRef;
    VALUE autotilesRef;
    VALUE bitmapsRef;
    VALUE mapDataRef;
    VALUE flashDataRef;
    VALUE prioritiesRef;
    VALUE flagsRef;
    VALUE viewportRef;
    bool registered;
};

std::vector<TilemapData*> gTilemaps;

void TilemapMark(void* aPointer)
{
    TilemapData* data = static_cast<TilemapData*>(aPointer);
    if (!data) return;
    rb_gc_mark(data->tilesetRef);
    rb_gc_mark(data->autotilesRef);
    rb_gc_mark(data->bitmapsRef);
    rb_gc_mark(data->mapDataRef);
    rb_gc_mark(data->flashDataRef);
    rb_gc_mark(data->prioritiesRef);
    rb_gc_mark(data->flagsRef);
    rb_gc_mark(data->viewportRef);
}

void TilemapFree(void* aPointer)
{
    TilemapData* data = static_cast<TilemapData*>(aPointer);
    if (!data) return;
    if (data->registered && RgssRendererScene())
        RgssRendererScene()->RemoveElement(&data->core);
    RemovePointer(gTilemaps, data);
    delete data;
}

const rb_data_type_t TilemapType = {
    "Tilemap", {TilemapMark, TilemapFree, 0, 0}, 0, 0,
    RUBY_TYPED_FREE_IMMEDIATELY
};

VALUE TilemapAlloc(VALUE aKlass)
{
    return TypedData_Wrap_Struct(aKlass, &TilemapType, new TilemapData);
}

TilemapData* GetTilemap(VALUE aSelf)
{
    TilemapData* data = 0;
    TypedData_Get_Struct(aSelf, TilemapData, &TilemapType, data);
    if (!data) rgssRaise("uninitialized Tilemap");
    return data;
}

VALUE TilemapInitialize(int aArgc, VALUE* aArgv, VALUE aSelf)
{
    VALUE viewport = Qnil;
    rb_scan_args(aArgc, aArgv, "01", &viewport);
    TilemapData* data = GetTilemap(aSelf);
    data->viewportRef = viewport;
    data->core.viewport = RgssViewportBindingCore(viewport);
    data->core.generation = gGeneration;
    data->autotilesRef = rb_ary_new2(7);
    for (int i = 0; i < 7; ++i)
        rb_ary_push(data->autotilesRef, Qnil);
    data->bitmapsRef = rb_ary_new2(9);
    for (int i = 0; i < 9; ++i)
        rb_ary_push(data->bitmapsRef, Qnil);
    data->core.disposed = false;
    if (!data->registered)
    {
        RgssRendererScene()->AddElement(&data->core);
        data->registered = true;
        gTilemaps.push_back(data);
    }
    return aSelf;
}

VALUE TilemapDispose(VALUE aSelf)
{
    TilemapData* data = GetTilemap(aSelf);
    if (!data->core.disposed)
    {
        data->core.disposed = true;
        if (data->registered)
        {
            RgssRendererScene()->RemoveElement(&data->core);
            data->registered = false;
        }
    }
    return Qnil;
}
VALUE TilemapDisposed(VALUE aSelf) { return GetTilemap(aSelf)->core.disposed ? Qtrue : Qfalse; }
VALUE TilemapViewport(VALUE aSelf) { return GetTilemap(aSelf)->viewportRef; }
VALUE TilemapSetViewport(VALUE aSelf, VALUE aValue)
{
    RebindElement(GetTilemap(aSelf), aValue);
    return aValue;
}
VALUE TilemapTileset(VALUE aSelf) { return GetTilemap(aSelf)->tilesetRef; }
VALUE TilemapSetTileset(VALUE aSelf, VALUE aValue)
{
    TilemapData* data = GetTilemap(aSelf);
    data->tilesetRef = aValue;
    data->core.tileset = RgssBitmapBindingCore(aValue);
    return aValue;
}
VALUE TilemapAutotiles(VALUE aSelf) { return GetTilemap(aSelf)->autotilesRef; }
VALUE TilemapBitmaps(VALUE aSelf) { return GetTilemap(aSelf)->bitmapsRef; }
VALUE TilemapMapData(VALUE aSelf) { return GetTilemap(aSelf)->mapDataRef; }
VALUE TilemapSetMapData(VALUE aSelf, VALUE aValue)
{
    TilemapData* data = GetTilemap(aSelf);
    data->mapDataRef = aValue;
    data->core.mapData = RgssTableBindingCore(aValue);
    return aValue;
}
VALUE TilemapFlashData(VALUE aSelf) { return GetTilemap(aSelf)->flashDataRef; }
VALUE TilemapSetFlashData(VALUE aSelf, VALUE aValue)
{
    TilemapData* data = GetTilemap(aSelf);
    data->flashDataRef = aValue;
    data->core.flashData = RgssTableBindingCore(aValue);
    return aValue;
}
VALUE TilemapPriorities(VALUE aSelf) { return GetTilemap(aSelf)->prioritiesRef; }
VALUE TilemapSetPriorities(VALUE aSelf, VALUE aValue)
{
    TilemapData* data = GetTilemap(aSelf);
    data->prioritiesRef = aValue;
    data->core.priorities = RgssTableBindingCore(aValue);
    return aValue;
}
VALUE TilemapFlags(VALUE aSelf) { return GetTilemap(aSelf)->flagsRef; }
VALUE TilemapSetFlags(VALUE aSelf, VALUE aValue)
{
    TilemapData* data = GetTilemap(aSelf);
    data->flagsRef = aValue;
    data->core.flags = RgssTableBindingCore(aValue);
    return aValue;
}
VALUE TilemapVisible(VALUE aSelf) { return GetTilemap(aSelf)->core.visible ? Qtrue : Qfalse; }
VALUE TilemapSetVisible(VALUE aSelf, VALUE aValue)
{
    GetTilemap(aSelf)->core.visible = RTEST(aValue); return aValue;
}
VALUE TilemapOx(VALUE aSelf) { return INT2FIX(GetTilemap(aSelf)->core.ox); }
VALUE TilemapSetOx(VALUE aSelf, VALUE aValue)
{
    GetTilemap(aSelf)->core.ox = NUM2INT(aValue); return aValue;
}
VALUE TilemapOy(VALUE aSelf) { return INT2FIX(GetTilemap(aSelf)->core.oy); }
VALUE TilemapSetOy(VALUE aSelf, VALUE aValue)
{
    GetTilemap(aSelf)->core.oy = NUM2INT(aValue); return aValue;
}
VALUE TilemapUpdate(VALUE aSelf)
{
    GetTilemap(aSelf)->core.UpdateAnimation();
    return Qnil;
}

void DefinePlane()
{
    cPlane = rb_define_class("Plane", rb_cObject);
    rb_define_alloc_func(cPlane, PlaneAlloc);
    rb_define_method(cPlane, "initialize", RUBY_METHOD_FUNC(PlaneInitialize), -1);
    rb_define_method(cPlane, "dispose", RUBY_METHOD_FUNC(PlaneDispose), 0);
    rb_define_method(cPlane, "disposed?", RUBY_METHOD_FUNC(PlaneDisposed), 0);
    rb_define_method(cPlane, "viewport", RUBY_METHOD_FUNC(PlaneViewport), 0);
    rb_define_method(cPlane, "viewport=", RUBY_METHOD_FUNC(PlaneSetViewport), 1);
    rb_define_method(cPlane, "bitmap", RUBY_METHOD_FUNC(PlaneBitmap), 0);
    rb_define_method(cPlane, "bitmap=", RUBY_METHOD_FUNC(PlaneSetBitmap), 1);
    rb_define_method(cPlane, "z", RUBY_METHOD_FUNC(PlaneZ), 0);
    rb_define_method(cPlane, "z=", RUBY_METHOD_FUNC(PlaneSetZ), 1);
    rb_define_method(cPlane, "ox", RUBY_METHOD_FUNC(PlaneOx), 0);
    rb_define_method(cPlane, "ox=", RUBY_METHOD_FUNC(PlaneSetOx), 1);
    rb_define_method(cPlane, "oy", RUBY_METHOD_FUNC(PlaneOy), 0);
    rb_define_method(cPlane, "oy=", RUBY_METHOD_FUNC(PlaneSetOy), 1);
    rb_define_method(cPlane, "zoom_x", RUBY_METHOD_FUNC(PlaneZoomX), 0);
    rb_define_method(cPlane, "zoom_x=", RUBY_METHOD_FUNC(PlaneSetZoomX), 1);
    rb_define_method(cPlane, "zoom_y", RUBY_METHOD_FUNC(PlaneZoomY), 0);
    rb_define_method(cPlane, "zoom_y=", RUBY_METHOD_FUNC(PlaneSetZoomY), 1);
    rb_define_method(cPlane, "opacity", RUBY_METHOD_FUNC(PlaneOpacity), 0);
    rb_define_method(cPlane, "opacity=", RUBY_METHOD_FUNC(PlaneSetOpacity), 1);
    rb_define_method(cPlane, "blend_type", RUBY_METHOD_FUNC(PlaneBlendType), 0);
    rb_define_method(cPlane, "blend_type=", RUBY_METHOD_FUNC(PlaneSetBlendType), 1);
    rb_define_method(cPlane, "visible", RUBY_METHOD_FUNC(PlaneVisible), 0);
    rb_define_method(cPlane, "visible=", RUBY_METHOD_FUNC(PlaneSetVisible), 1);
    rb_define_method(cPlane, "color", RUBY_METHOD_FUNC(PlaneColor), 0);
    rb_define_method(cPlane, "color=", RUBY_METHOD_FUNC(PlaneSetColor), 1);
    rb_define_method(cPlane, "tone", RUBY_METHOD_FUNC(PlaneTone), 0);
    rb_define_method(cPlane, "tone=", RUBY_METHOD_FUNC(PlaneSetTone), 1);
}

void DefineWindow()
{
    cWindow = rb_define_class("Window", rb_cObject);
    rb_define_alloc_func(cWindow, WindowAlloc);
    rb_define_method(cWindow, "initialize", RUBY_METHOD_FUNC(WindowInitialize), -1);
    rb_define_method(cWindow, "dispose", RUBY_METHOD_FUNC(WindowDispose), 0);
    rb_define_method(cWindow, "disposed?", RUBY_METHOD_FUNC(WindowDisposed), 0);
    rb_define_method(cWindow, "viewport", RUBY_METHOD_FUNC(WindowViewport), 0);
    rb_define_method(cWindow, "viewport=", RUBY_METHOD_FUNC(WindowSetViewport), 1);
    rb_define_method(cWindow, "windowskin", RUBY_METHOD_FUNC(WindowWindowskin), 0);
    rb_define_method(cWindow, "windowskin=", RUBY_METHOD_FUNC(WindowSetWindowskin), 1);
    rb_define_method(cWindow, "contents", RUBY_METHOD_FUNC(WindowContents), 0);
    rb_define_method(cWindow, "contents=", RUBY_METHOD_FUNC(WindowSetContents), 1);
    rb_define_method(cWindow, "cursor_rect", RUBY_METHOD_FUNC(WindowCursorRect), 0);
    rb_define_method(cWindow, "cursor_rect=", RUBY_METHOD_FUNC(WindowSetCursorRect), 1);
    rb_define_method(cWindow, "x", RUBY_METHOD_FUNC(WindowX), 0);
    rb_define_method(cWindow, "x=", RUBY_METHOD_FUNC(WindowSetX), 1);
    rb_define_method(cWindow, "y", RUBY_METHOD_FUNC(WindowY), 0);
    rb_define_method(cWindow, "y=", RUBY_METHOD_FUNC(WindowSetY), 1);
    rb_define_method(cWindow, "width", RUBY_METHOD_FUNC(WindowWidth), 0);
    rb_define_method(cWindow, "width=", RUBY_METHOD_FUNC(WindowSetWidth), 1);
    rb_define_method(cWindow, "height", RUBY_METHOD_FUNC(WindowHeight), 0);
    rb_define_method(cWindow, "height=", RUBY_METHOD_FUNC(WindowSetHeight), 1);
    rb_define_method(cWindow, "z", RUBY_METHOD_FUNC(WindowZ), 0);
    rb_define_method(cWindow, "z=", RUBY_METHOD_FUNC(WindowSetZ), 1);
    rb_define_method(cWindow, "ox", RUBY_METHOD_FUNC(WindowOx), 0);
    rb_define_method(cWindow, "ox=", RUBY_METHOD_FUNC(WindowSetOx), 1);
    rb_define_method(cWindow, "oy", RUBY_METHOD_FUNC(WindowOy), 0);
    rb_define_method(cWindow, "oy=", RUBY_METHOD_FUNC(WindowSetOy), 1);
    rb_define_method(cWindow, "opacity", RUBY_METHOD_FUNC(WindowOpacity), 0);
    rb_define_method(cWindow, "opacity=", RUBY_METHOD_FUNC(WindowSetOpacity), 1);
    rb_define_method(cWindow, "back_opacity", RUBY_METHOD_FUNC(WindowBackOpacity), 0);
    rb_define_method(cWindow, "back_opacity=", RUBY_METHOD_FUNC(WindowSetBackOpacity), 1);
    rb_define_method(cWindow, "contents_opacity", RUBY_METHOD_FUNC(WindowContentsOpacity), 0);
    rb_define_method(cWindow, "contents_opacity=", RUBY_METHOD_FUNC(WindowSetContentsOpacity), 1);
    rb_define_method(cWindow, "tone", RUBY_METHOD_FUNC(WindowTone), 0);
    rb_define_method(cWindow, "tone=", RUBY_METHOD_FUNC(WindowSetTone), 1);
    rb_define_method(cWindow, "openness", RUBY_METHOD_FUNC(WindowOpenness), 0);
    rb_define_method(cWindow, "openness=", RUBY_METHOD_FUNC(WindowSetOpenness), 1);
    rb_define_method(cWindow, "padding", RUBY_METHOD_FUNC(WindowPadding), 0);
    rb_define_method(cWindow, "padding=", RUBY_METHOD_FUNC(WindowSetPadding), 1);
    rb_define_method(cWindow, "padding_bottom", RUBY_METHOD_FUNC(WindowPaddingBottom), 0);
    rb_define_method(cWindow, "padding_bottom=", RUBY_METHOD_FUNC(WindowSetPaddingBottom), 1);
    rb_define_method(cWindow, "open?", RUBY_METHOD_FUNC(WindowOpenQ), 0);
    rb_define_method(cWindow, "close?", RUBY_METHOD_FUNC(WindowCloseQ), 0);
    rb_define_method(cWindow, "move", RUBY_METHOD_FUNC(WindowMove), 4);
    rb_define_method(cWindow, "visible", RUBY_METHOD_FUNC(WindowVisible), 0);
    rb_define_method(cWindow, "visible=", RUBY_METHOD_FUNC(WindowSetVisible), 1);
    rb_define_method(cWindow, "active", RUBY_METHOD_FUNC(WindowActive), 0);
    rb_define_method(cWindow, "active=", RUBY_METHOD_FUNC(WindowSetActive), 1);
    rb_define_method(cWindow, "pause", RUBY_METHOD_FUNC(WindowPause), 0);
    rb_define_method(cWindow, "pause=", RUBY_METHOD_FUNC(WindowSetPause), 1);
    rb_define_method(cWindow, "stretch", RUBY_METHOD_FUNC(WindowStretch), 0);
    rb_define_method(cWindow, "stretch=", RUBY_METHOD_FUNC(WindowSetStretch), 1);
    rb_define_method(cWindow, "arrows_visible", RUBY_METHOD_FUNC(WindowArrowsVisible), 0);
    rb_define_method(cWindow, "arrows_visible=", RUBY_METHOD_FUNC(WindowSetArrowsVisible), 1);
    rb_define_method(cWindow, "update", RUBY_METHOD_FUNC(WindowUpdate), 0);
}

void DefineTilemap()
{
    cTilemap = rb_define_class("Tilemap", rb_cObject);
    rb_define_alloc_func(cTilemap, TilemapAlloc);
    rb_define_method(cTilemap, "initialize", RUBY_METHOD_FUNC(TilemapInitialize), -1);
    rb_define_method(cTilemap, "dispose", RUBY_METHOD_FUNC(TilemapDispose), 0);
    rb_define_method(cTilemap, "disposed?", RUBY_METHOD_FUNC(TilemapDisposed), 0);
    rb_define_method(cTilemap, "viewport", RUBY_METHOD_FUNC(TilemapViewport), 0);
    rb_define_method(cTilemap, "viewport=", RUBY_METHOD_FUNC(TilemapSetViewport), 1);
    rb_define_method(cTilemap, "tileset", RUBY_METHOD_FUNC(TilemapTileset), 0);
    rb_define_method(cTilemap, "tileset=", RUBY_METHOD_FUNC(TilemapSetTileset), 1);
    rb_define_method(cTilemap, "autotiles", RUBY_METHOD_FUNC(TilemapAutotiles), 0);
    rb_define_method(cTilemap, "bitmaps", RUBY_METHOD_FUNC(TilemapBitmaps), 0);
    rb_define_method(cTilemap, "map_data", RUBY_METHOD_FUNC(TilemapMapData), 0);
    rb_define_method(cTilemap, "map_data=", RUBY_METHOD_FUNC(TilemapSetMapData), 1);
    rb_define_method(cTilemap, "flash_data", RUBY_METHOD_FUNC(TilemapFlashData), 0);
    rb_define_method(cTilemap, "flash_data=", RUBY_METHOD_FUNC(TilemapSetFlashData), 1);
    rb_define_method(cTilemap, "priorities", RUBY_METHOD_FUNC(TilemapPriorities), 0);
    rb_define_method(cTilemap, "priorities=", RUBY_METHOD_FUNC(TilemapSetPriorities), 1);
    rb_define_method(cTilemap, "flags", RUBY_METHOD_FUNC(TilemapFlags), 0);
    rb_define_method(cTilemap, "flags=", RUBY_METHOD_FUNC(TilemapSetFlags), 1);
    rb_define_method(cTilemap, "passages", RUBY_METHOD_FUNC(TilemapFlags), 0);
    rb_define_method(cTilemap, "passages=", RUBY_METHOD_FUNC(TilemapSetFlags), 1);
    rb_define_method(cTilemap, "visible", RUBY_METHOD_FUNC(TilemapVisible), 0);
    rb_define_method(cTilemap, "visible=", RUBY_METHOD_FUNC(TilemapSetVisible), 1);
    rb_define_method(cTilemap, "ox", RUBY_METHOD_FUNC(TilemapOx), 0);
    rb_define_method(cTilemap, "ox=", RUBY_METHOD_FUNC(TilemapSetOx), 1);
    rb_define_method(cTilemap, "oy", RUBY_METHOD_FUNC(TilemapOy), 0);
    rb_define_method(cTilemap, "oy=", RUBY_METHOD_FUNC(TilemapSetOy), 1);
    rb_define_method(cTilemap, "update", RUBY_METHOD_FUNC(TilemapUpdate), 0);
}

} // namespace

void RgssM4BindingSync()
{
    for (size_t i = 0; i < gPlanes.size(); ++i)
    {
        PlaneData* data = gPlanes[i];
        data->core.bitmap = RgssBitmapBindingCore(data->bitmapRef);
        if (!NIL_P(data->colorRef))
            data->core.color = RgssColorValue(data->colorRef);
        if (!NIL_P(data->toneRef))
            data->core.tone = RgssToneValue(data->toneRef);
    }

    for (size_t i = 0; i < gWindows.size(); ++i)
    {
        WindowData* data = gWindows[i];
        data->core.windowskin = RgssBitmapBindingCore(data->windowskinRef);
        data->core.contents = RgssBitmapBindingCore(data->contentsRef);
        if (!NIL_P(data->cursorRectRef))
            data->core.cursorRect = RgssRectValue(data->cursorRectRef);
    }

    for (size_t i = 0; i < gTilemaps.size(); ++i)
    {
        TilemapData* data = gTilemaps[i];
        data->core.tileset = RgssBitmapBindingCore(data->tilesetRef);
        data->core.mapData = RgssTableBindingCore(data->mapDataRef);
        data->core.flashData = RgssTableBindingCore(data->flashDataRef);
        data->core.priorities = RgssTableBindingCore(data->prioritiesRef);
        data->core.flags = RgssTableBindingCore(data->flagsRef);
        for (int slot = 0; slot < 7; ++slot)
        {
            VALUE bitmap = rb_ary_entry(data->autotilesRef, slot);
            data->core.autotiles[slot] = RgssBitmapBindingCore(bitmap);
        }
        for (int slot = 0; slot < 9; ++slot)
        {
            VALUE bitmap = rb_ary_entry(data->bitmapsRef, slot);
            data->core.bitmaps[slot] = RgssBitmapBindingCore(bitmap);
        }
    }
}

void RgssM4BindingInit(int aGeneration)
{
    gGeneration = aGeneration >= 1 && aGeneration <= 3 ? aGeneration : 1;
    DefinePlane();
    DefineWindow();
    DefineTilemap();
}
