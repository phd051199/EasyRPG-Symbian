#include "ruby.h"

#include <zlib.h>
#include <string.h>

/*
 * Minimal Zlib module over libz: our Ruby builds (host static and Symbian)
 * ship without the zlib extension, but game scripts (compressed saves,
 * Essentials plugins) call Zlib::Inflate.inflate / Zlib::Deflate.deflate.
 */

namespace {

VALUE ZlibRun(VALUE aString, int aInflate)
{
    StringValue(aString);

    VALUE result = rb_str_new(0, 0);

    z_stream stream;
    memset(&stream, 0, sizeof(stream));
    stream.next_in = reinterpret_cast<Bytef*>(RSTRING_PTR(aString));
    stream.avail_in = static_cast<uInt>(RSTRING_LEN(aString));

    if (aInflate ? inflateInit(&stream) != Z_OK
                 : deflateInit(&stream, Z_DEFAULT_COMPRESSION) != Z_OK)
        rb_raise(rb_eRuntimeError, "zlib init failed");

    int status = Z_OK;
    char scratch[16 * 1024];
    while (status == Z_OK)
    {
        stream.next_out = reinterpret_cast<Bytef*>(scratch);
        stream.avail_out = sizeof(scratch);
        status = aInflate ? inflate(&stream, Z_NO_FLUSH)
                          : deflate(&stream, Z_FINISH);
        const long produced = sizeof(scratch) - stream.avail_out;
        if (produced > 0)
            rb_str_cat(result, scratch, produced);
        if (RSTRING_LEN(result) > 64 * 1024 * 1024)
            break; /* hard cap */
    }
    aInflate ? inflateEnd(&stream) : deflateEnd(&stream);

    if ((aInflate && status != Z_STREAM_END) ||
        (!aInflate && status != Z_STREAM_END && status != Z_BUF_ERROR))
    {
        rb_raise(rb_eRuntimeError, "zlib stream error (%d)", status);
    }
    return result;
}

VALUE ZlibModuleInflate(VALUE aSelf, VALUE aString) { return ZlibRun(aString, 1); }
VALUE ZlibModuleDeflate(VALUE aSelf, VALUE aString) { return ZlibRun(aString, 0); }

} // namespace

void RgssZlibBindingInit()
{
    VALUE mZlib = rb_define_module("Zlib");
    rb_define_module_function(mZlib, "inflate", RUBY_METHOD_FUNC(ZlibModuleInflate), 1);
    rb_define_module_function(mZlib, "deflate", RUBY_METHOD_FUNC(ZlibModuleDeflate), 1);

    /* Games also use the class forms Zlib::Inflate.inflate / Zlib::Deflate.deflate. */
    VALUE cInflate = rb_define_class_under(mZlib, "Inflate", rb_cObject);
    rb_define_singleton_method(cInflate, "inflate",
                               RUBY_METHOD_FUNC(ZlibModuleInflate), 1);
    VALUE cDeflate = rb_define_class_under(mZlib, "Deflate", rb_cObject);
    rb_define_singleton_method(cDeflate, "deflate",
                               RUBY_METHOD_FUNC(ZlibModuleDeflate), 1);
}
