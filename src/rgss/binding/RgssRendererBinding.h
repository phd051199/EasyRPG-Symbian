#ifndef RGSS_RENDERER_BINDING_H
#define RGSS_RENDERER_BINDING_H

#include "ruby.h"

namespace rgss {
class RgssBitmapCore;
class RgssScene;
class ViewportElement;
}

rgss::RgssScene* RgssRendererScene();
rgss::RgssBitmapCore* RgssBitmapBindingCore(VALUE aObject);
rgss::ViewportElement* RgssViewportBindingCore(VALUE aObject);

#endif
