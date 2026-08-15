#ifndef RGSS_ETC_BINDING_H
#define RGSS_ETC_BINDING_H

#include "ruby.h"
#include "../core/RgssTypes.h"

rgss::Rect RgssRectValue(VALUE aObject);
VALUE RgssRectObject(const rgss::Rect& aRect);
rgss::Color RgssColorValue(VALUE aObject);
VALUE RgssColorObject(const rgss::Color& aColor);
rgss::Tone RgssToneValue(VALUE aObject);
VALUE RgssToneObject(const rgss::Tone& aTone);

#endif
