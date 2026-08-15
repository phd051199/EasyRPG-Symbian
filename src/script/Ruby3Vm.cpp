#include "ScriptVmBridge.h"

#include "ruby.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

extern "C" void rb_call_builtin_inits(void);

extern const char module_rpg1[];
extern const unsigned int module_rpg1_len;
extern const char module_rpg2[];
extern const unsigned int module_rpg2_len;
extern const char module_rpg3[];
extern const unsigned int module_rpg3_len;

namespace
{
const int KMaxInflatedScriptBytes = 8 * 1024 * 1024;
const int KInflateChunkBytes = 16 * 1024;

const char KCompatibilityBootstrap[] =
    "Object.const_set(:Fixnum, Integer) unless Object.const_defined?(:Fixnum)\n"
    "Object.const_set(:Bignum, Integer) unless Object.const_defined?(:Bignum)\n"
    "module Kernel\n"
    "  def rgss_main\n"
    "    yield\n"
    "  end\n"
    "end\n";

VALUE gScripts = Qnil;
int gGeneration = 0;
int gInitialized = 0;
int gScriptsRegistered = 0;

void CopyError(char* aOutput, int aCapacity, const char* aText)
{
    if (!aOutput || aCapacity <= 0)
        return;
    if (!aText)
        aText = "Ruby error";
    int length = static_cast<int>(strlen(aText));
    if (length >= aCapacity)
        length = aCapacity - 1;
    if (length > 0)
        memcpy(aOutput, aText, length);
    aOutput[length] = 0;
}

void CopyRubyError(char* aOutput, int aCapacity, int aScriptIndex)
{
    char prefix[40];
    prefix[0] = 0;
    if (aScriptIndex >= 0)
        snprintf(prefix, sizeof(prefix), "script %d: ", aScriptIndex);

    VALUE error = rb_errinfo();
    if (NIL_P(error))
    {
        CopyError(aOutput, aCapacity, "Ruby error");
        return;
    }
    VALUE message = rb_obj_as_string(error);
    const char* text = RSTRING_PTR(message);
    const long textLength = RSTRING_LEN(message);
    const int prefixLength = static_cast<int>(strlen(prefix));
    if (!aOutput || aCapacity <= 0)
        return;
    int copyLength = static_cast<int>(textLength);
    if (copyLength > aCapacity - prefixLength - 1)
        copyLength = aCapacity - prefixLength - 1;
    if (copyLength < 0)
        copyLength = 0;
    int usedPrefix = prefixLength;
    if (usedPrefix >= aCapacity)
        usedPrefix = aCapacity - 1;
    memcpy(aOutput, prefix, usedPrefix);
    if (copyLength > 0)
        memcpy(aOutput + usedPrefix, text, copyLength);
    aOutput[usedPrefix + copyLength] = 0;
}

VALUE LoadMarshal(VALUE aBytes)
{
    return rb_marshal_load(aBytes);
}

void DestroyVm()
{
    if (gScriptsRegistered)
    {
        rb_gc_unregister_address(&gScripts);
        gScriptsRegistered = 0;
    }
    gScripts = Qnil;
    gGeneration = 0;
    if (gInitialized)
    {
        ruby_cleanup(0);
        gInitialized = 0;
    }
}

int EvaluateModule(int aGeneration, char* aError, int aErrorCapacity)
{
    const char* source = module_rpg1;
    unsigned int length = module_rpg1_len;
    if (aGeneration == 2)
    {
        source = module_rpg2;
        length = module_rpg2_len;
    }
    else if (aGeneration == 3)
    {
        source = module_rpg3;
        length = module_rpg3_len;
    }
    if (length == 0 || source[length] != 0)
        return EScriptVmCorrupt;

    int state = 0;
    rb_eval_string_protect(source, &state);
    if (state != 0)
    {
        CopyRubyError(aError, aErrorCapacity, -1);
        return EScriptVmCorrupt;
    }
    return EScriptVmOk;
}

int InflateScript(VALUE aCompressed, char** aOutput, int* aOutputSize)
{
    *aOutput = 0;
    *aOutputSize = 0;
    if (!RB_TYPE_P(aCompressed, T_STRING))
        return EScriptVmCorrupt;

    z_stream stream;
    memset(&stream, 0, sizeof(stream));
    stream.next_in = reinterpret_cast<Bytef*>(RSTRING_PTR(aCompressed));
    stream.avail_in = static_cast<uInt>(RSTRING_LEN(aCompressed));
    if (inflateInit(&stream) != Z_OK)
        return EScriptVmCorrupt;

    int capacity = KInflateChunkBytes;
    char* output = static_cast<char*>(malloc(capacity + 1));
    if (!output)
    {
        inflateEnd(&stream);
        return EScriptVmNoMemory;
    }

    int length = 0;
    int status = Z_OK;
    while (status == Z_OK)
    {
        if (length == capacity)
        {
            if (capacity > KMaxInflatedScriptBytes - KInflateChunkBytes)
            {
                free(output);
                inflateEnd(&stream);
                return EScriptVmNoMemory;
            }
            const int newCapacity = capacity + KInflateChunkBytes;
            char* grown = static_cast<char*>(realloc(output, newCapacity + 1));
            if (!grown)
            {
                free(output);
                inflateEnd(&stream);
                return EScriptVmNoMemory;
            }
            output = grown;
            capacity = newCapacity;
        }
        stream.next_out = reinterpret_cast<Bytef*>(output + length);
        stream.avail_out = capacity - length;
        status = inflate(&stream, Z_NO_FLUSH);
        length = capacity - stream.avail_out;
    }
    inflateEnd(&stream);
    if (status != Z_STREAM_END)
    {
        free(output);
        return EScriptVmCorrupt;
    }
    output[length] = 0;
    *aOutput = output;
    *aOutputSize = length;
    return EScriptVmOk;
}
}

extern "C" int EasyRpgScriptVmCreate(const unsigned char* aArchive,
                                       int aArchiveSize,
                                       int aGeneration,
                                       char* aError,
                                       int aErrorCapacity)
{
    if (aError && aErrorCapacity > 0)
        aError[0] = 0;
    if (!aArchive || aArchiveSize <= 0 || aGeneration < 1 || aGeneration > 3 ||
        gInitialized)
        return EScriptVmGeneral;

    ruby_init();
    gInitialized = 1;
    rb_call_builtin_inits();
    ruby_script("EasyRPGScript");

    int state = 0;
    rb_eval_string_protect(KCompatibilityBootstrap, &state);
    if (state != 0)
    {
        CopyRubyError(aError, aErrorCapacity, -1);
        DestroyVm();
        return EScriptVmCorrupt;
    }

    int result = EvaluateModule(aGeneration, aError, aErrorCapacity);
    if (result != EScriptVmOk)
    {
        DestroyVm();
        return result;
    }

    VALUE bytes = rb_str_new(reinterpret_cast<const char*>(aArchive), aArchiveSize);
    gScripts = rb_protect(LoadMarshal, bytes, &state);
    if (state != 0 || !RB_TYPE_P(gScripts, T_ARRAY))
    {
        CopyRubyError(aError, aErrorCapacity, -1);
        DestroyVm();
        return EScriptVmCorrupt;
    }

    rb_gc_register_address(&gScripts);
    gScriptsRegistered = 1;
    gGeneration = aGeneration;
    rb_gv_set("$RGSS_SCRIPTS", gScripts);
    rb_define_global_const("RGSS_VERSION", INT2FIX(gGeneration));
    return EScriptVmOk;
}

extern "C" int EasyRpgScriptVmExecute(char* aError, int aErrorCapacity)
{
    if (aError && aErrorCapacity > 0)
        aError[0] = 0;
    if (!gInitialized || !RB_TYPE_P(gScripts, T_ARRAY))
        return EScriptVmGeneral;

    const long count = RARRAY_LEN(gScripts);
    for (long index = 0; index < count; ++index)
    {
        VALUE entry = rb_ary_entry(gScripts, index);
        if (!RB_TYPE_P(entry, T_ARRAY) || RARRAY_LEN(entry) < 3)
        {
            CopyError(aError, aErrorCapacity, "invalid script archive entry");
            return EScriptVmCorrupt;
        }

        char* source = 0;
        int sourceSize = 0;
        const int inflateResult = InflateScript(
            rb_ary_entry(entry, 2), &source, &sourceSize);
        if (inflateResult != EScriptVmOk)
        {
            CopyError(aError, aErrorCapacity, "cannot inflate script");
            return inflateResult;
        }

        int state = 0;
        rb_eval_string_protect(source, &state);
        free(source);
        if (state != 0)
        {
            CopyRubyError(aError, aErrorCapacity, static_cast<int>(index));
            return EScriptVmGeneral;
        }
    }
    return EScriptVmOk;
}

extern "C" void EasyRpgScriptVmDestroy(void)
{
    DestroyVm();
}
