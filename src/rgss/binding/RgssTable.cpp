#include "RgssBindingUtil.h"
#include "RgssTableBinding.h"
#include "../core/RgssTypes.h"

using rgss::Table;

RGSS_DEFINE_TYPE(Table)

namespace {

VALUE cTable;

/* Table.new(xsize, ysize=1, zsize=1) */
VALUE TableInitialize(int aArgc, VALUE* aArgv, VALUE aSelf)
{
    VALUE x = Qnil, y = Qnil, z = Qnil;
    rb_scan_args(aArgc, aArgv, "12", &x, &y, &z);
    Table* table = RGSS_GET(aSelf, Table);
    table->Resize(NUM2INT(x), NIL_P(y) ? 1 : NUM2INT(y),
                  NIL_P(z) ? 1 : NUM2INT(z));
    return aSelf;
}

VALUE TableResize(VALUE aSelf, VALUE aX, VALUE aY, VALUE aZ)
{
    RGSS_GET(aSelf, Table)->Resize(NUM2INT(aX), NUM2INT(aY), NUM2INT(aZ));
    return aSelf;
}

VALUE TableXsize(VALUE aSelf) { return INT2FIX(RGSS_GET(aSelf, Table)->Xsize()); }
VALUE TableYsize(VALUE aSelf) { return INT2FIX(RGSS_GET(aSelf, Table)->Ysize()); }
VALUE TableZsize(VALUE aSelf) { return INT2FIX(RGSS_GET(aSelf, Table)->Zsize()); }

VALUE TableGet3(VALUE aX, VALUE aY, VALUE aZ, Table* aTable)
{
    short value = 0;
    if (aTable->Get(NUM2INT(aX), NUM2INT(aY), NUM2INT(aZ), &value))
        return INT2FIX(value);
    return Qnil;
}

/* table[x, y, z] */
VALUE TableAref(int aArgc, VALUE* aArgv, VALUE aSelf)
{
    VALUE x = Qnil, y = Qnil, z = Qnil;
    rb_scan_args(aArgc, aArgv, "12", &x, &y, &z);
    Table* table = RGSS_GET(aSelf, Table);
    return TableGet3(x, NIL_P(y) ? INT2FIX(0) : y,
                     NIL_P(z) ? INT2FIX(0) : z, table);
}

/* table[x, y, z] = value; out-of-range writes are ignored */
VALUE TableAset(int aArgc, VALUE* aArgv, VALUE aSelf)
{
    VALUE x = Qnil, y = Qnil, z = Qnil, value = Qnil;
    rb_scan_args(aArgc, aArgv, "22", &x, &y, &z, &value);
    if (NIL_P(y)) y = INT2FIX(0);
    if (NIL_P(z)) z = INT2FIX(0);
    RGSS_GET(aSelf, Table)->Set(NUM2INT(x), NUM2INT(y), NUM2INT(z),
                                static_cast<short>(NUM2INT(value)));
    return value;
}

VALUE TableDump(VALUE aSelf, VALUE aLimit)
{
    Table* table = RGSS_GET(aSelf, Table);
    VALUE result = rb_str_new(0, static_cast<long>(table->SerializedSize()));
    table->Serialize(reinterpret_cast<unsigned char*>(RSTRING_PTR(result)));
    return result;
}

/* Table._load(data) */
VALUE TableLoad(VALUE aKlass, VALUE aData)
{
    rb_check_type(aData, T_STRING);

    Table* table = new Table;
    if (!table->Deserialize(reinterpret_cast<const unsigned char*>(RSTRING_PTR(aData)),
                            RSTRING_LEN(aData)))
    {
        delete table;
        rgssRaise("Table._load: corrupted marshal data");
    }

    VALUE object = rb_obj_alloc(aKlass);
    RGSS_ATTACH(object, table, Table);
    return object;
}

} // namespace

rgss::Table* RgssTableBindingCore(VALUE aObject)
{
    if (NIL_P(aObject))
        return 0;
    return RGSS_GET(aObject, Table);
}

void RgssTableBindingInit()
{
    cTable = rb_define_class("Table", rb_cObject);
    rb_define_alloc_func(cTable, TableAlloc);
    rb_define_method(cTable, "initialize",
                     RUBY_METHOD_FUNC(TableInitialize), -1);
    rb_define_method(cTable, "resize",
                     RUBY_METHOD_FUNC(TableResize), 3);
    rb_define_method(cTable, "xsize",
                     RUBY_METHOD_FUNC(TableXsize), 0);
    rb_define_method(cTable, "ysize",
                     RUBY_METHOD_FUNC(TableYsize), 0);
    rb_define_method(cTable, "zsize",
                     RUBY_METHOD_FUNC(TableZsize), 0);
    rb_define_method(cTable, "[]",
                     RUBY_METHOD_FUNC(TableAref), -1);
    rb_define_method(cTable, "[]=",
                     RUBY_METHOD_FUNC(TableAset), -1);
    rb_define_method(cTable, "_dump",
                     RUBY_METHOD_FUNC(TableDump), 1);
    rb_define_singleton_method(cTable, "_load",
                               RUBY_METHOD_FUNC(TableLoad), 1);
}
