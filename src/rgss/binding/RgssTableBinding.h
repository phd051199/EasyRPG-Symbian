#ifndef RGSS_TABLE_BINDING_H
#define RGSS_TABLE_BINDING_H

#include "ruby.h"

namespace rgss { class Table; }

rgss::Table* RgssTableBindingCore(VALUE aObject);

#endif
