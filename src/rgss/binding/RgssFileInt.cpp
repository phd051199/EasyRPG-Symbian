#include "RgssBindingUtil.h"
#include "../core/RgssVfs.h"
#include "RgssHost.h"

#include <cerrno>
#include <cstdlib>

namespace {

/* rb_marshal_load / rb_marshal_dump can raise; wrap for rb_protect. */
VALUE MarshalLoadProtect(VALUE aArg)
{
    return rb_marshal_load(aArg);
}

VALUE MarshalDumpProtect(VALUE aArg)
{
    return rb_marshal_dump(aArg, Qnil);
}

/* load_data(filename) -> Object (Marshal-loaded, strings forced to UTF-8) */
VALUE LoadData(VALUE aSelf, VALUE aFilename)
{
    const char* path = StringValueCStr(aFilename);

    unsigned char* data = 0;
    const long size = RgssVfsRead(path, &data);
    if (size < 0 || !data)
    {
        errno = ENOENT;
        rb_sys_fail_str(aFilename); /* Errno::ENOENT, like RGSS */
    }

    /* Copy into a Ruby string first so the buffer can be freed before
     * marshal load (which may raise). */
    VALUE bytes = rb_str_new(reinterpret_cast<const char*>(data),
                             static_cast<long>(size));
    free(data);

    int state = 0;
    VALUE result = rb_protect(MarshalLoadProtect, bytes, &state);
    if (state)
        rb_jump_tag(state);

    /* Recursive ASCII-8BIT -> UTF-8 fix (RgssInternal.fix_encoding,
     * defined in the bootstrap). Games written for Ruby 1.8 expect
     * these strings to be UTF-8-ish. */
    static VALUE fixer = Qnil;
    if (NIL_P(fixer))
        fixer = rb_define_module("RgssInternal");
    return rb_funcall(fixer, rb_intern("fix_encoding"), 1, result);
}

/* save_data(obj, filename) */
VALUE SaveData(VALUE aSelf, VALUE aObject, VALUE aFilename)
{
    int state = 0;
    VALUE dumped = rb_protect(MarshalDumpProtect, aObject, &state);
    if (state)
        rb_jump_tag(state);

    const RgssHostVtable* host = RgssHost();
    if (!host || !host->FileWrite ||
        host->FileWrite(host->ctx, StringValueCStr(aFilename),
                        reinterpret_cast<const unsigned char*>(RSTRING_PTR(dumped)),
                        RSTRING_LEN(dumped)) != RSTRING_LEN(dumped))
    {
        errno = EIO;
        rb_sys_fail_str(aFilename);
    }
    return Qnil;
}

VALUE VfsExists(VALUE, VALUE aFilename)
{
    const char* path = StringValueCStr(aFilename);
    return RgssVfsExists(path) ? Qtrue : Qfalse;
}

VALUE VfsRead(VALUE, VALUE aFilename)
{
    const char* path = StringValueCStr(aFilename);
    unsigned char* data = 0;
    const long size = RgssVfsRead(path, &data);
    if (size < 0 || (!data && size != 0))
    {
        free(data);
        errno = ENOENT;
        rb_sys_fail_str(aFilename);
    }
    VALUE bytes = rb_str_new(reinterpret_cast<const char*>(data), size);
    free(data);
    return bytes;
}

VALUE VfsWrite(VALUE, VALUE aFilename, VALUE aBytes)
{
    StringValue(aBytes);
    const RgssHostVtable* host = RgssHost();
    const long size = RSTRING_LEN(aBytes);
    if (!host || !host->FileWrite ||
        host->FileWrite(host->ctx, StringValueCStr(aFilename),
                        reinterpret_cast<const unsigned char*>(RSTRING_PTR(aBytes)),
                        size) != size)
    {
        errno = EIO;
        rb_sys_fail_str(aFilename);
    }
    return LONG2NUM(size);
}

VALUE VfsDelete(VALUE, VALUE aFilename)
{
    const RgssHostVtable* host = RgssHost();
    if (!host || !host->FileDelete)
        return Qfalse;
    if (host->FileDelete(host->ctx, StringValueCStr(aFilename)) != 0)
    {
        errno = ENOENT;
        rb_sys_fail_str(aFilename);
    }
    return Qtrue;
}

VALUE VfsRename(VALUE, VALUE aOldFilename, VALUE aNewFilename)
{
    const RgssHostVtable* host = RgssHost();
    if (!host || !host->FileRename)
        return Qfalse;
    if (host->FileRename(host->ctx, StringValueCStr(aOldFilename),
                         StringValueCStr(aNewFilename)) != 0)
    {
        errno = EIO;
        rb_sys_fail_str(aOldFilename);
    }
    return Qtrue;
}

} // namespace

void RgssFileIntBindingInit()
{
    rb_define_private_method(rb_mKernel, "load_data", RUBY_METHOD_FUNC(LoadData), 1);
    rb_define_private_method(rb_mKernel, "save_data", RUBY_METHOD_FUNC(SaveData), 2);

    VALUE internal = rb_define_module("RgssInternal");
    rb_define_module_function(internal, "__vfs_exist?", RUBY_METHOD_FUNC(VfsExists), 1);
    rb_define_module_function(internal, "__vfs_read", RUBY_METHOD_FUNC(VfsRead), 1);
    rb_define_module_function(internal, "__vfs_write", RUBY_METHOD_FUNC(VfsWrite), 2);
    rb_define_module_function(internal, "__vfs_delete", RUBY_METHOD_FUNC(VfsDelete), 1);
    rb_define_module_function(internal, "__vfs_rename", RUBY_METHOD_FUNC(VfsRename), 2);
}
