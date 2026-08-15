#include "RgssRun.h"
#include "RgssBootstrap.h"

#include "binding/RgssBindingUtil.h"
#include "core/RgssVfs.h"
#include "RgssHost.h"

#include "ruby.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

extern "C" void rb_call_builtin_inits(void);
/* Static libruby ships dmyenc.c, so register the encodings we rely on
 * directly (same set as the Symbian lane's Init_enc in Ruby3Platform.cpp). */
extern "C" void rb_encdb_declare(const char* aName);
extern "C" int rb_encdb_alias(const char* aAlias, const char* aOriginal);

extern const char module_rpg1[];
extern const unsigned int module_rpg1_len;
extern const char module_rpg2[];
extern const unsigned int module_rpg2_len;
extern const char module_rpg3[];
extern const unsigned int module_rpg3_len;

int RgssBindingInit(int aGeneration);

namespace {

const int KMaxInflatedScriptBytes = 8 * 1024 * 1024;
const int KInflateChunkBytes = 16 * 1024;

int gInitialized = 0;
const RgssHostVtable* gHostVtable = 0;

} // namespace

extern "C" void RgssHostSet(const RgssHostVtable* aVtable)
{
    gHostVtable = aVtable;
}

extern "C" const RgssHostVtable* RgssHost(void)
{
    return gHostVtable;
}

namespace {

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
    VALUE klass = rb_class_path(rb_obj_class(error));
    snprintf(aOutput, aCapacity, "%s%s: %s", prefix,
             RSTRING_PTR(klass), RSTRING_PTR(message));
    if (rb_funcall(error, rb_intern("respond_to?"), 1,
                   ID2SYM(rb_intern("backtrace"))) == Qtrue)
    {
        VALUE backtrace = rb_funcall(error, rb_intern("backtrace"), 0);
        if (RB_TYPE_P(backtrace, T_ARRAY) && RARRAY_LEN(backtrace) > 0)
        {
            VALUE top = rb_obj_as_string(rb_ary_entry(backtrace, 0));
            strncat(aOutput, " @ ", aCapacity - strlen(aOutput) - 1);
            strncat(aOutput, RSTRING_PTR(top), aCapacity - strlen(aOutput) - 1);
        }
    }
}

VALUE MarshalLoadProtect(VALUE aArg)
{
    return rb_marshal_load(aArg);
}

/* Same chunked inflate as the 2k-lane script VM (Ruby3Vm.cpp). */
int InflateScript(VALUE aCompressed, char** aOutput, int* aOutputSize)
{
    *aOutput = 0;
    *aOutputSize = 0;
    if (!RB_TYPE_P(aCompressed, T_STRING))
        return RgssRunCorrupt;

    z_stream stream;
    memset(&stream, 0, sizeof(stream));
    stream.next_in = reinterpret_cast<Bytef*>(RSTRING_PTR(aCompressed));
    stream.avail_in = static_cast<uInt>(RSTRING_LEN(aCompressed));
    if (inflateInit(&stream) != Z_OK)
        return RgssRunCorrupt;

    int capacity = KInflateChunkBytes;
    char* output = static_cast<char*>(malloc(capacity + 1));
    if (!output)
    {
        inflateEnd(&stream);
        return RgssRunNoMemory;
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
                return RgssRunNoMemory;
            }
            const int newCapacity = capacity + KInflateChunkBytes;
            char* grown = static_cast<char*>(realloc(output, newCapacity + 1));
            if (!grown)
            {
                free(output);
                inflateEnd(&stream);
                return RgssRunNoMemory;
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
        return RgssRunCorrupt;
    }
    output[length] = 0;
    *aOutput = output;
    *aOutputSize = length;
    return RgssRunOk;
}

#define RGSS_STAGE(msg) \
    do { const RgssHostVtable* h = RgssHost(); \
         if (h && h->Log) h->Log(h->ctx, msg); } while (0)

int EvaluateSource(const char* aSource, char* aError, int aCapacity,
                   int aScriptIndex)
{
    int state = 0;
    rb_eval_string_protect(aSource, &state);
    if (state != 0)
    {
        CopyRubyError(aError, aCapacity, aScriptIndex);
        return RgssRunScriptError;
    }
    return RgssRunOk;
}

const char* ScriptArchiveName(int aGeneration)
{
    switch (aGeneration)
    {
    case 2: return "Data/Scripts.rvdata";
    case 3: return "Data/Scripts.rvdata2";
    default: return "Data/Scripts.rxdata";
    }
}

} // namespace

extern "C" int RgssRun(int aGeneration, char* aError, int aErrorCapacity)
{
    if (aError && aErrorCapacity > 0)
        aError[0] = 0;
    if (aGeneration < 1 || aGeneration > 3 || gInitialized)
        return RgssRunGeneral;

    const RgssHostVtable* host = RgssHost();
    if (!host || !host->FileExists || !host->FileRead)
    {
        CopyError(aError, aErrorCapacity, "RGSS host filesystem is not initialized");
        return RgssRunGeneral;
    }
    const int nativeWidth = aGeneration == 1 ? 640 : 544;
    const int nativeHeight = aGeneration == 1 ? 480 : 416;
    if (host->Init && host->Init(host->ctx, nativeWidth, nativeHeight) != 0)
    {
        CopyError(aError, aErrorCapacity, "RGSS host display initialization failed");
        return RgssRunGeneral;
    }

    RgssVfsMountRgssad();

    ruby_init();
    gInitialized = 1;
    rb_call_builtin_inits();
    rb_encdb_declare("ASCII-8BIT");
    rb_encdb_declare("US-ASCII");
    rb_encdb_declare("UTF-8");
    rb_encdb_alias("BINARY", "ASCII-8BIT");
    rb_encdb_alias("ASCII", "US-ASCII");
    ruby_script("EasyRPGScript");

    RGSS_STAGE("stage: bindings");
    if (RgssBindingInit(aGeneration) != 0)
    {
        CopyError(aError, aErrorCapacity, "RGSS renderer initialization failed");
        return RgssRunNoMemory;
    }
    RGSS_STAGE("stage: bootstrap");
    if (RgssEvaluateBootstrap() != 0)
    {
        CopyRubyError(aError, aErrorCapacity, -1);
        return RgssRunCorrupt;
    }

    RGSS_STAGE("stage: module_rpg");
    const char* moduleSource = module_rpg1;
    unsigned int moduleLength = module_rpg1_len;
    if (aGeneration == 2)
    {
        moduleSource = module_rpg2;
        moduleLength = module_rpg2_len;
    }
    else if (aGeneration == 3)
    {
        moduleSource = module_rpg3;
        moduleLength = module_rpg3_len;
    }
    if (moduleLength == 0 || moduleSource[moduleLength] != 0)
        return RgssRunCorrupt;

    int result = EvaluateSource(moduleSource, aError, aErrorCapacity, -1);
    if (result != RgssRunOk)
        return result;

    RGSS_STAGE("stage: scripts read");
    unsigned char* archiveBytes = 0;
    const long archiveSize =
        RgssVfsRead(ScriptArchiveName(aGeneration), &archiveBytes);
    if (archiveSize <= 0 || !archiveBytes)
    {
        CopyError(aError, aErrorCapacity, "Scripts archive not found");
        return RgssRunCorrupt;
    }

    VALUE bytes = rb_str_new(reinterpret_cast<const char*>(archiveBytes),
                             static_cast<long>(archiveSize));
    free(archiveBytes);

    int state = 0;
    VALUE scripts = rb_protect(MarshalLoadProtect, bytes, &state);
    if (state != 0 || !RB_TYPE_P(scripts, T_ARRAY))
    {
        CopyRubyError(aError, aErrorCapacity, -1);
        return RgssRunCorrupt;
    }

    RGSS_STAGE("stage: scripts eval");
    rb_gv_set("$RGSS_SCRIPTS", scripts);
    rb_define_global_const("RGSS_VERSION", INT2FIX(aGeneration));

    const long count = RARRAY_LEN(scripts);
    for (long index = 0; index < count; ++index)
    {
        VALUE entry = rb_ary_entry(scripts, index);
        if (!RB_TYPE_P(entry, T_ARRAY) || RARRAY_LEN(entry) < 3)
        {
            CopyError(aError, aErrorCapacity, "invalid script archive entry");
            return RgssRunCorrupt;
        }

        char* source = 0;
        int sourceSize = 0;
        const int inflateResult = InflateScript(
            rb_ary_entry(entry, 2), &source, &sourceSize);
        if (inflateResult != RgssRunOk)
        {
            CopyError(aError, aErrorCapacity, "cannot inflate script");
            return inflateResult;
        }

        result = EvaluateSource(source, aError, aErrorCapacity,
                                static_cast<int>(index));
        free(source);
        if (result != RgssRunOk)
            return result;
    }
    return RgssRunOk;
}

extern "C" int RgssEval(const char* aCode, char* aError, int aErrorCapacity)
{
    if (aError && aErrorCapacity > 0)
        aError[0] = 0;
    if (!gInitialized)
        return RgssRunGeneral;
    return EvaluateSource(aCode, aError, aErrorCapacity, -1);
}
