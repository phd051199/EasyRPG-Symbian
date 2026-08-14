#include "RgssBitmapBridge.h"
#include "RgssBootstrap.h"
#include "RgssGraphicsBridge.h"
#include "RgssInputBridge.h"
#include "RgssPlaneBridge.h"
#include "RgssSpriteBridge.h"
#include "RgssTableBridge.h"
#include "RgssTilemapBridge.h"
#include "RgssViewportBridge.h"
#include "RgssVmBridge.h"

#include "ruby.h"
#include "ruby/encoding.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

extern "C" void rb_call_builtin_inits(void);
extern "C" void EasyRpgRgssVmTrace(const char* aText);
extern "C" void EasyRpgRgssVmTraceHeap(const char* aText);

namespace
{
enum THandleType
{
    EHandleBitmap = 1,
    EHandleSprite,
    EHandleViewport,
    EHandlePlane,
    EHandleTable,
    EHandleTilemap
};

struct THandleToken
{
    int type;
    int handle;
};

struct TVfsInput
{
    char* path;
    int size;
    int position;
    int bufferStart;
    int bufferLength;
    unsigned char buffer[4096];
};

struct TAnimationEntry
{
    int offset;
    int objectBase;
    int objectCount;
    int symbolBase;
};

struct TAnimationSymbol
{
    char* name;
    int length;
};

struct TAnimationArchive
{
    char* path;
    int size;
    int selected;
    TAnimationEntry* entries;
    int entryCount;
    TAnimationSymbol* symbols;
    int symbolCount;
    int symbolCapacity;
};

EasyRpgRgssVmHost gHost;
int gVersion = 0;
int gInitialized = 0;
VALUE gNativeModule = Qnil;
VALUE gHandleClass = Qnil;
VALUE gVfsInputClass = Qnil;
VALUE gAnimationArchiveClass = Qnil;
int gDeferredBattleScripts[128];
int gDeferredBattleScriptCount = 0;
int gDeferredBattleLoaded = 0;
int gDeferredBattleLoading = 0;
int gDeferredOptionalScripts[64];
int gDeferredOptionalScriptCount = 0;
int gDeferredOptionalLoaded = 0;
int gDeferredOptionalLoading = 0;
int gLastErrorScript = -1;
char gLastError[768];
unsigned int gLastGraphicsTick = 0;
unsigned int gSystemDeltaUs = 25000;
int gHaveGraphicsTick = 0;

VALUE NLoadDeferredBattle(VALUE);
VALUE NLoadDeferredOptional(VALUE);
int EvalProtected(const char* source, int scriptIndex);

void ResetHost()
{
    gHost.context = 0;
    gHost.file_size = 0;
    gHost.file_read = 0;
    gHost.ticks = 0;
    gHost.input = 0;
    gHost.bitmap_load = 0;
    gHost.text_decode = 0;
    gHost.frame_sync = 0;
    gHost.script_count = 0;
    gHost.script_info = 0;
    gHost.script_name = 0;
    gHost.script_inflate = 0;
    gHost.release = 0;
    gHost.dirty = 0;
    gHost.audio = 0;
    gVersion = 0;
    gInitialized = 0;
    gNativeModule = Qnil;
    gHandleClass = Qnil;
    gVfsInputClass = Qnil;
    gAnimationArchiveClass = Qnil;
    gDeferredBattleScriptCount = 0;
    gDeferredBattleLoaded = 0;
    gDeferredBattleLoading = 0;
    gDeferredOptionalScriptCount = 0;
    gDeferredOptionalLoaded = 0;
    gDeferredOptionalLoading = 0;
    gLastErrorScript = -1;
    gLastError[0] = 0;
    gLastGraphicsTick = 0;
    gSystemDeltaUs = 25000;
    gHaveGraphicsTick = 0;
}

void DestroyNativeHandle(THandleToken* token)
{
    if (!token || token->handle == 0)
        return;
    switch (token->type)
    {
    case EHandleBitmap:
        EasyRpgRgssBitmapDestroy(token->handle);
        break;
    case EHandleSprite:
        EasyRpgRgssSpriteDestroy(token->handle);
        break;
    case EHandleViewport:
        EasyRpgRgssViewportDestroy(token->handle);
        break;
    case EHandlePlane:
        EasyRpgRgssPlaneDestroy(token->handle);
        break;
    case EHandleTable:
        EasyRpgRgssTableDestroy(token->handle);
        break;
    case EHandleTilemap:
        EasyRpgRgssTilemapDestroy(token->handle);
        break;
    default:
        break;
    }
    token->handle = 0;
}

void HandleFree(void* memory)
{
    THandleToken* token = static_cast<THandleToken*>(memory);
    DestroyNativeHandle(token);
    ruby_xfree(token);
}

VALUE NewHandle(int type, int handle)
{
    THandleToken* token = ALLOC(THandleToken);
    token->type = type;
    token->handle = handle;
    return Data_Wrap_Struct(gHandleClass, 0, HandleFree, token);
}

THandleToken* Token(VALUE value)
{
    THandleToken* token = 0;
    Data_Get_Struct(value, THandleToken, token);
    if (!token)
        rb_raise(rb_eTypeError, "invalid native handle");
    return token;
}

int Handle(VALUE value, int type)
{
    THandleToken* token = Token(value);
    if (token->type != type)
        rb_raise(rb_eTypeError, "native handle type mismatch");
    if (token->handle == 0)
        rb_raise(rb_eRuntimeError, "disposed object");
    return token->handle;
}

int OptionalHandle(VALUE value, int type)
{
    return NIL_P(value) ? 0 : Handle(value, type);
}

VALUE RaiseResult(const char* operation, int result)
{
    rb_raise(rb_eRuntimeError, "%s failed (%d)", operation, result);
    return Qnil;
}

VALUE BoolValue(int value)
{
    return value ? Qtrue : Qfalse;
}

VALUE NHandleDispose(VALUE, VALUE value)
{
    DestroyNativeHandle(Token(value));
    return Qnil;
}

VALUE NHandleDisposed(VALUE, VALUE value)
{
    return BoolValue(Token(value)->handle == 0);
}

VALUE NGraphicsUpdate(VALUE)
{
    const int result = EasyRpgRgssGraphicsUpdate();
    if (result != ERgssGraphicsOk)
        return RaiseResult("Graphics.update", result);

    // mkxp-z exposes System.delta as the elapsed time of the previous frame
    // in microseconds, not an absolute clock. Pokemon Essentials builds
    // Graphics.delta_s on top of this value for splash timers, fades, weather,
    // etc. Keep a stable first-frame fallback and clamp long focus/pause gaps so
    // event timers do not jump several seconds in one update.
    const unsigned int now = gHost.ticks(gHost.context);
    unsigned int delta = 0;
    if (gHaveGraphicsTick)
        delta = now - gLastGraphicsTick; // unsigned subtraction handles wraparound
    if (!gHaveGraphicsTick || delta == 0)
    {
        const int frameRate = EasyRpgRgssGraphicsFrameRate();
        delta = frameRate > 0 ? static_cast<unsigned int>(1000000 / frameRate) : 25000u;
    }
    if (delta < 1000u)
        delta = 1000u;
    else if (delta > 250000u)
        delta = 250000u;
    gLastGraphicsTick = now;
    gSystemDeltaUs = delta;
    gHaveGraphicsTick = 1;
    return Qnil;
}

VALUE NGraphicsFrameRate(VALUE)
{
    const int value = EasyRpgRgssGraphicsFrameRate();
    return value >= 0 ? INT2NUM(value) : RaiseResult("Graphics.frame_rate", value);
}

VALUE NGraphicsSetFrameRate(VALUE, VALUE value)
{
    const int frameRate = NUM2INT(value);
    const int result = EasyRpgRgssGraphicsSetFrameRate(frameRate);
    return result == ERgssGraphicsOk ? value : RaiseResult("Graphics.frame_rate=", result);
}

VALUE NGraphicsFrameCount(VALUE)
{
    const int value = EasyRpgRgssGraphicsFrameCount();
    return value >= 0 ? INT2NUM(value) : RaiseResult("Graphics.frame_count", value);
}

VALUE NGraphicsSetFrameCount(VALUE, VALUE value)
{
    const int frameCount = NUM2INT(value);
    const int result = EasyRpgRgssGraphicsSetFrameCount(frameCount);
    return result == ERgssGraphicsOk ? value : RaiseResult("Graphics.frame_count=", result);
}

VALUE NGraphicsWidth(VALUE)
{
    EasyRpgRgssConfig config;
    const int result = EasyRpgRgssCoreGetConfig(&config);
    return result == ERgssOk ? INT2NUM(config.logical_width) : RaiseResult("Graphics.width", result);
}

VALUE NGraphicsHeight(VALUE)
{
    EasyRpgRgssConfig config;
    const int result = EasyRpgRgssCoreGetConfig(&config);
    return result == ERgssOk ? INT2NUM(config.logical_height) : RaiseResult("Graphics.height", result);
}

VALUE NGraphicsResize(VALUE, VALUE width, VALUE height)
{
    const int result = EasyRpgRgssCoreResizeLogical(NUM2INT(width), NUM2INT(height));
    if (result != ERgssOk)
        return RaiseResult("Graphics.resize_screen", result);
    EasyRpgRgssTilemapInvalidateAll();
    return Qnil;
}

VALUE NInputUpdate(VALUE)
{
    const unsigned int mask = gHost.input(gHost.context, 0);
    EasyRpgRgssInputSetMask(mask);
    const int result = EasyRpgRgssInputUpdate();
    return result == ERgssInputOk ? Qnil : RaiseResult("Input.update", result);
}

VALUE NInputPress(VALUE, VALUE mask)
{
    return BoolValue(EasyRpgRgssInputPress(static_cast<unsigned int>(NUM2ULONG(mask))));
}

VALUE NInputTrigger(VALUE, VALUE mask)
{
    return BoolValue(EasyRpgRgssInputTrigger(static_cast<unsigned int>(NUM2ULONG(mask))));
}

VALUE NInputRepeat(VALUE, VALUE mask)
{
    return BoolValue(EasyRpgRgssInputRepeat(static_cast<unsigned int>(NUM2ULONG(mask))));
}

VALUE NSystemDelta(VALUE)
{
    return ULONG2NUM(gSystemDeltaUs);
}

VALUE NTraceScene(VALUE, VALUE name)
{
    if (!NIL_P(name))
    {
        const char* text = StringValuePtr(name);
        char line[256];
        int length = 0;
        while (text[length] && length < static_cast<int>(sizeof(line)) - 8)
            ++length;
        memcpy(line, "scene ", 6);
        memcpy(line + 6, text, length);
        line[6 + length] = 0;
        EasyRpgRgssVmTrace(line);
    }
    return Qnil;
}

VALUE NZlibInflate(VALUE, VALUE input)
{
    StringValue(input);
    z_stream stream;
    memset(&stream, 0, sizeof(stream));
    stream.next_in = reinterpret_cast<Bytef*>(RSTRING_PTR(input));
    stream.avail_in = static_cast<uInt>(RSTRING_LEN(input));
    if (inflateInit(&stream) != Z_OK)
        rb_raise(rb_eRuntimeError, "zlib inflate init failed");
    VALUE output = rb_str_new(0, 0);
    unsigned char buffer[8192];
    int result = Z_OK;
    while (result == Z_OK)
    {
        stream.next_out = buffer;
        stream.avail_out = sizeof(buffer);
        result = inflate(&stream, Z_NO_FLUSH);
        const int produced = static_cast<int>(sizeof(buffer) - stream.avail_out);
        if (produced > 0)
            rb_str_cat(output, reinterpret_cast<const char*>(buffer), produced);
    }
    inflateEnd(&stream);
    if (result != Z_STREAM_END)
        rb_raise(rb_eRuntimeError, "zlib inflate failed (%d)", result);
    return output;
}

VALUE NZlibDeflate(VALUE, VALUE input)
{
    StringValue(input);
    const uLong sourceLength = static_cast<uLong>(RSTRING_LEN(input));
    uLongf outputLength = sourceLength + sourceLength / 1000 + 13;
    VALUE output = rb_str_new(0, static_cast<long>(outputLength));
    const int result = compress2(reinterpret_cast<Bytef*>(RSTRING_PTR(output)),
                                 &outputLength,
                                 reinterpret_cast<const Bytef*>(RSTRING_PTR(input)),
                                 sourceLength,
                                 Z_DEFAULT_COMPRESSION);
    if (result != Z_OK)
        rb_raise(rb_eRuntimeError, "zlib deflate failed (%d)", result);
    rb_str_resize(output, static_cast<long>(outputLength));
    return output;
}

VALUE NFileSize(VALUE, VALUE path)
{
    const char* relative = StringValuePtr(path);
    const int size = gHost.file_size(gHost.context, relative);
    return size >= 0 ? INT2NUM(size) : Qnil;
}

VALUE NFileRead(VALUE, VALUE path)
{
    const char* relative = StringValuePtr(path);
    const int size = gHost.file_size(gHost.context, relative);
    if (size < 0)
        return RaiseResult("file read", size);
    if (size > 32 * 1024 * 1024)
        rb_raise(rb_eRuntimeError, "file too large");
    VALUE data = rb_str_new(0, size);
    if (size > 0)
    {
        const int read = gHost.file_read(gHost.context,
                                         relative,
                                         0,
                                         RSTRING_PTR(data),
                                         size);
        if (read != size)
            return RaiseResult("file read", read);
    }
    return data;
}

void VfsInputFree(void* memory)
{
    TVfsInput* input = static_cast<TVfsInput*>(memory);
    if (!input)
        return;
    ruby_xfree(input->path);
    ruby_xfree(input);
}

TVfsInput* VfsInput(VALUE self)
{
    TVfsInput* input = 0;
    Data_Get_Struct(self, TVfsInput, input);
    if (!input)
        rb_raise(rb_eRuntimeError, "invalid VFS input");
    return input;
}

VALUE NVfsInputCreate(VALUE, VALUE path)
{
    StringValue(path);
    const char* relative = RSTRING_PTR(path);
    const int size = gHost.file_size(gHost.context, relative);
    if (size < 0)
        return RaiseResult("VFS input", size);
    TVfsInput* input = ALLOC(TVfsInput);
    memset(input, 0, sizeof(TVfsInput));
    const int pathLength = RSTRING_LEN(path);
    input->path = ALLOC_N(char, pathLength + 1);
    memcpy(input->path, relative, pathLength);
    input->path[pathLength] = 0;
    input->size = size;
    input->bufferStart = -1;
    return Data_Wrap_Struct(gVfsInputClass, 0, VfsInputFree, input);
}

VALUE NVfsInputBinmode(VALUE self)
{
    return self;
}

VALUE NVfsInputGetbyte(VALUE self)
{
    TVfsInput* input = VfsInput(self);
    if (input->position >= input->size)
        return Qnil;

    if (input->bufferStart < 0 ||
        input->position < input->bufferStart ||
        input->position >= input->bufferStart + input->bufferLength)
    {
        const int count = input->size - input->position < static_cast<int>(sizeof(input->buffer))
            ? input->size - input->position
            : static_cast<int>(sizeof(input->buffer));
        const int read = gHost.file_read(gHost.context,
                                         input->path,
                                         input->position,
                                         input->buffer,
                                         count);
        if (read <= 0)
            return read == 0 ? Qnil : RaiseResult("VFS getbyte", read);
        input->bufferStart = input->position;
        input->bufferLength = read;
    }

    const int index = input->position - input->bufferStart;
    const unsigned char value = input->buffer[index];
    ++input->position;
    return INT2NUM(value);
}

VALUE NVfsInputRead(int argc, VALUE* argv, VALUE self)
{
    VALUE lengthValue = Qnil;
    rb_scan_args(argc, argv, "01", &lengthValue);
    TVfsInput* input = VfsInput(self);
    long requested = NIL_P(lengthValue)
        ? static_cast<long>(input->size - input->position)
        : NUM2LONG(lengthValue);
    if (requested < 0)
        rb_raise(rb_eArgError, "negative length");
    if (requested == 0)
        return rb_str_new(0, 0);
    if (input->position >= input->size)
        return Qnil;

    const int remaining = input->size - input->position;
    const int count = requested < remaining ? static_cast<int>(requested) : remaining;
    VALUE data = rb_str_new(0, count);
    const int read = gHost.file_read(gHost.context,
                                     input->path,
                                     input->position,
                                     RSTRING_PTR(data),
                                     count);
    if (read < 0)
        return RaiseResult("VFS read", read);
    if (read == 0)
        return Qnil;
    input->position += read;
    input->bufferStart = -1;
    input->bufferLength = 0;
    if (read != count)
        rb_str_resize(data, read);
    return data;
}

class TAnimationReader
{
public:
    TAnimationReader(TAnimationArchive* archive, int position)
        : iArchive(archive), iPosition(position), iBufferStart(-1), iBufferLength(0)
    {
    }

    int Position() const
    {
        return iPosition;
    }

    int ReadByte(unsigned char& value)
    {
        if (!iArchive || iPosition < 0 || iPosition >= iArchive->size)
            return -1;
        if (iBufferStart < 0 || iPosition < iBufferStart ||
            iPosition >= iBufferStart + iBufferLength)
        {
            if (Fill() != 0)
                return -1;
        }
        value = iBuffer[iPosition - iBufferStart];
        ++iPosition;
        return 0;
    }

    int ReadRaw(void* output, int bytes)
    {
        if (bytes < 0 || !iArchive || iPosition < 0 ||
            iPosition > iArchive->size || bytes > iArchive->size - iPosition)
            return -1;
        unsigned char* destination = static_cast<unsigned char*>(output);
        int remaining = bytes;
        while (remaining > 0)
        {
            if (iBufferStart < 0 || iPosition < iBufferStart ||
                iPosition >= iBufferStart + iBufferLength)
            {
                if (Fill() != 0)
                    return -1;
            }
            const int offset = iPosition - iBufferStart;
            const int available = iBufferLength - offset;
            const int count = remaining < available ? remaining : available;
            if (destination)
            {
                memcpy(destination, iBuffer + offset, count);
                destination += count;
            }
            iPosition += count;
            remaining -= count;
        }
        return 0;
    }

    int Skip(int bytes)
    {
        if (bytes < 0 || !iArchive || iPosition < 0 ||
            iPosition > iArchive->size || bytes > iArchive->size - iPosition)
            return -1;
        iPosition += bytes;
        return 0;
    }

    int ReadLong(int& value)
    {
        unsigned char firstByte = 0;
        if (ReadByte(firstByte) != 0)
            return -1;
        int first = static_cast<int>(firstByte);
        if (first >= 128)
            first -= 256;
        if (first == 0)
        {
            value = 0;
            return 0;
        }
        if (first > 4)
        {
            value = first - 5;
            return 0;
        }
        if (first < -4)
        {
            value = first + 5;
            return 0;
        }
        if (first > 0)
        {
            unsigned int raw = 0;
            for (int index = 0; index < first; ++index)
            {
                unsigned char byte = 0;
                if (ReadByte(byte) != 0)
                    return -1;
                raw |= static_cast<unsigned int>(byte) << (index * 8);
            }
            value = static_cast<int>(raw);
            return 0;
        }
        const int length = -first;
        unsigned int raw = 0xffffffffu;
        for (int index = 0; index < length; ++index)
        {
            unsigned char byte = 0;
            if (ReadByte(byte) != 0)
                return -1;
            const unsigned int shift = static_cast<unsigned int>(index * 8);
            raw &= ~(0xffu << shift);
            raw |= static_cast<unsigned int>(byte) << shift;
        }
        value = static_cast<int>(raw);
        return 0;
    }

private:
    int Fill()
    {
        if (!iArchive || !gHost.file_read || iPosition < 0 || iPosition >= iArchive->size)
            return -1;
        int count = iArchive->size - iPosition;
        if (count > static_cast<int>(sizeof(iBuffer)))
            count = static_cast<int>(sizeof(iBuffer));
        const int read = gHost.file_read(gHost.context,
                                         iArchive->path,
                                         iPosition,
                                         iBuffer,
                                         count);
        if (read != count)
            return -1;
        iBufferStart = iPosition;
        iBufferLength = read;
        return 0;
    }

private:
    TAnimationArchive* iArchive;
    int iPosition;
    int iBufferStart;
    int iBufferLength;
    unsigned char iBuffer[4096];
};

struct TAnimationScanState
{
    int objectCount;
};

int AnimationAddSymbol(TAnimationArchive* archive, TAnimationReader& reader, int length)
{
    if (!archive || length < 0 || length > 4096)
        return -1;
    if (archive->symbolCount >= archive->symbolCapacity)
    {
        int capacity = archive->symbolCapacity == 0 ? 32 : archive->symbolCapacity * 2;
        REALLOC_N(archive->symbols, TAnimationSymbol, capacity);
        archive->symbolCapacity = capacity;
    }
    TAnimationSymbol& symbol = archive->symbols[archive->symbolCount];
    symbol.name = ALLOC_N(char, length + 1);
    symbol.length = length;
    if (reader.ReadRaw(symbol.name, length) != 0)
    {
        ruby_xfree(symbol.name);
        symbol.name = 0;
        return -1;
    }
    symbol.name[length] = 0;
    ++archive->symbolCount;
    return archive->symbolCount - 1;
}

int AnimationReadSymbol(TAnimationArchive* archive, TAnimationReader& reader, int& symbolId)
{
    unsigned char marker = 0;
    if (reader.ReadByte(marker) != 0)
        return -1;
    if (marker == ':')
    {
        int length = 0;
        if (reader.ReadLong(length) != 0)
            return -1;
        symbolId = AnimationAddSymbol(archive, reader, length);
        return symbolId >= 0 ? 0 : -1;
    }
    if (marker == ';')
    {
        if (reader.ReadLong(symbolId) != 0 || symbolId < 0 || symbolId >= archive->symbolCount)
            return -1;
        return 0;
    }
    return -1;
}

int AnimationScanObject(TAnimationArchive* archive,
                        TAnimationReader& reader,
                        TAnimationScanState& state);

int AnimationScanIvars(TAnimationArchive* archive,
                       TAnimationReader& reader,
                       TAnimationScanState& state)
{
    int count = 0;
    if (reader.ReadLong(count) != 0 || count < 0 || count > 100000)
        return -1;
    for (int index = 0; index < count; ++index)
    {
        if (AnimationScanObject(archive, reader, state) != 0 ||
            AnimationScanObject(archive, reader, state) != 0)
            return -1;
    }
    return 0;
}

int AnimationScanSizedBytes(TAnimationReader& reader)
{
    int length = 0;
    if (reader.ReadLong(length) != 0 || length < 0)
        return -1;
    return reader.Skip(length);
}

int AnimationScanArray(TAnimationArchive* archive,
                       TAnimationReader& reader,
                       TAnimationScanState& state)
{
    int count = 0;
    if (reader.ReadLong(count) != 0 || count < 0 || count > 10000000)
        return -1;
    ++state.objectCount;
    for (int index = 0; index < count; ++index)
    {
        if (AnimationScanObject(archive, reader, state) != 0)
            return -1;
    }
    return 0;
}

int AnimationScanObject(TAnimationArchive* archive,
                        TAnimationReader& reader,
                        TAnimationScanState& state)
{
    unsigned char marker = 0;
    if (reader.ReadByte(marker) != 0)
        return -1;
    switch (marker)
    {
    case '0':
    case 'T':
    case 'F':
        return 0;
    case 'i':
    {
        int value = 0;
        return reader.ReadLong(value);
    }
    case ':':
    {
        int length = 0;
        if (reader.ReadLong(length) != 0)
            return -1;
        return AnimationAddSymbol(archive, reader, length) >= 0 ? 0 : -1;
    }
    case ';':
    {
        int symbol = 0;
        return reader.ReadLong(symbol) == 0 && symbol >= 0 && symbol < archive->symbolCount ? 0 : -1;
    }
    case '"':
        ++state.objectCount;
        return AnimationScanSizedBytes(reader);
    case 'f':
        if (AnimationScanSizedBytes(reader) != 0)
            return -1;
        ++state.objectCount;
        return 0;
    case '[':
        return AnimationScanArray(archive, reader, state);
    case '{':
    case '}':
    {
        int count = 0;
        if (reader.ReadLong(count) != 0 || count < 0 || count > 1000000)
            return -1;
        ++state.objectCount;
        for (int index = 0; index < count; ++index)
        {
            if (AnimationScanObject(archive, reader, state) != 0 ||
                AnimationScanObject(archive, reader, state) != 0)
                return -1;
        }
        return marker == '}' ? AnimationScanObject(archive, reader, state) : 0;
    }
    case 'I':
        if (AnimationScanObject(archive, reader, state) != 0)
            return -1;
        return AnimationScanIvars(archive, reader, state);
    case 'C':
    case 'e':
    {
        int symbol = 0;
        if (AnimationReadSymbol(archive, reader, symbol) != 0)
            return -1;
        return AnimationScanObject(archive, reader, state);
    }
    case 'o':
    {
        ++state.objectCount;
        int symbol = 0;
        if (AnimationReadSymbol(archive, reader, symbol) != 0)
            return -1;
        return AnimationScanIvars(archive, reader, state);
    }
    case 'u':
    {
        int symbol = 0;
        if (AnimationReadSymbol(archive, reader, symbol) != 0 ||
            AnimationScanSizedBytes(reader) != 0)
            return -1;
        ++state.objectCount;
        return 0;
    }
    case '@':
    {
        int link = 0;
        return reader.ReadLong(link) == 0 && link >= 0 && link < state.objectCount ? 0 : -1;
    }
    case 'l':
    {
        unsigned char sign = 0;
        int words = 0;
        if (reader.ReadByte(sign) != 0 || (sign != '+' && sign != '-') ||
            reader.ReadLong(words) != 0 || words < 0 || words > 0x3fffffff ||
            reader.Skip(words * 2) != 0)
            return -1;
        ++state.objectCount;
        return 0;
    }
    case '/':
    {
        if (AnimationScanSizedBytes(reader) != 0)
            return -1;
        unsigned char options = 0;
        if (reader.ReadByte(options) != 0)
            return -1;
        ++state.objectCount;
        return 0;
    }
    default:
        return -1;
    }
}

int AnimationIndexArchive(TAnimationArchive* archive)
{
    if (!archive || archive->size < 8)
        return -1;
    TAnimationReader reader(archive, 0);
    unsigned char major = 0;
    unsigned char minor = 0;
    unsigned char marker = 0;
    if (reader.ReadByte(major) != 0 || reader.ReadByte(minor) != 0 ||
        major != 4 || minor > 8 || reader.ReadByte(marker) != 0 || marker != 'I' ||
        reader.ReadByte(marker) != 0 || marker != 'C')
        return -1;

    int classSymbol = 0;
    if (AnimationReadSymbol(archive, reader, classSymbol) != 0 ||
        strcmp(archive->symbols[classSymbol].name, "PBAnimations") != 0 ||
        reader.ReadByte(marker) != 0 || marker != '[')
        return -1;

    TAnimationScanState state;
    state.objectCount = 1;
    int baseCount = 0;
    if (reader.ReadLong(baseCount) != 0 || baseCount < 0 || baseCount > 1000)
        return -1;
    for (int index = 0; index < baseCount; ++index)
    {
        if (AnimationScanObject(archive, reader, state) != 0)
            return -1;
    }

    int ivarCount = 0;
    if (reader.ReadLong(ivarCount) != 0 || ivarCount < 0 || ivarCount > 64)
        return -1;
    int foundArray = 0;
    for (int ivar = 0; ivar < ivarCount; ++ivar)
    {
        int keySymbol = 0;
        if (AnimationReadSymbol(archive, reader, keySymbol) != 0)
            return -1;
        const char* key = archive->symbols[keySymbol].name;
        if (strcmp(key, "@array") != 0)
        {
            if (AnimationScanObject(archive, reader, state) != 0)
                return -1;
            continue;
        }

        if (reader.ReadByte(marker) != 0 || marker != '[')
            return -1;
        ++state.objectCount;
        int count = 0;
        if (reader.ReadLong(count) != 0 || count <= 0 || count > 100000)
            return -1;
        archive->entries = ALLOC_N(TAnimationEntry, count);
        archive->entryCount = count;
        for (int index = 0; index < count; ++index)
        {
            TAnimationEntry& entry = archive->entries[index];
            entry.offset = reader.Position();
            entry.objectBase = state.objectCount;
            entry.symbolBase = archive->symbolCount;
            if (AnimationScanObject(archive, reader, state) != 0)
                return -1;
            entry.objectCount = state.objectCount - entry.objectBase;
            if (entry.objectCount <= 0)
                return -1;
        }
        foundArray = 1;
    }
    return foundArray ? 0 : -1;
}

void AnimationArchiveFree(void* memory)
{
    TAnimationArchive* archive = static_cast<TAnimationArchive*>(memory);
    if (!archive)
        return;
    if (archive->symbols)
    {
        for (int index = 0; index < archive->symbolCount; ++index)
            ruby_xfree(archive->symbols[index].name);
        ruby_xfree(archive->symbols);
    }
    ruby_xfree(archive->entries);
    ruby_xfree(archive->path);
    ruby_xfree(archive);
}

TAnimationArchive* AnimationArchive(VALUE self)
{
    TAnimationArchive* archive = 0;
    Data_Get_Struct(self, TAnimationArchive, archive);
    if (!archive)
        rb_raise(rb_eRuntimeError, "invalid animation archive");
    return archive;
}

struct TAnimationDecodeState
{
    TAnimationArchive* archive;
    TAnimationReader* reader;
    int objectBase;
    int nextSymbol;
    VALUE objects;
};

int AnimationDecodeSymbol(TAnimationDecodeState& state)
{
    unsigned char marker = 0;
    if (state.reader->ReadByte(marker) != 0)
        rb_raise(rb_eArgError, "invalid animation symbol");
    if (marker == ':')
    {
        int length = 0;
        if (state.reader->ReadLong(length) != 0 || length < 0 ||
            state.nextSymbol < 0 || state.nextSymbol >= state.archive->symbolCount ||
            length != state.archive->symbols[state.nextSymbol].length ||
            state.reader->Skip(length) != 0)
            rb_raise(rb_eArgError, "invalid animation symbol definition");
        return state.nextSymbol++;
    }
    if (marker == ';')
    {
        int symbol = 0;
        if (state.reader->ReadLong(symbol) != 0 || symbol < 0 || symbol >= state.archive->symbolCount)
            rb_raise(rb_eArgError, "invalid animation symbol link");
        return symbol;
    }
    rb_raise(rb_eArgError, "invalid animation symbol marker");
    return -1;
}

VALUE AnimationDecodeObject(TAnimationDecodeState& state);

VALUE AnimationDecodeRawString(TAnimationDecodeState& state)
{
    int length = 0;
    if (state.reader->ReadLong(length) != 0 || length < 0 || length > state.archive->size)
        rb_raise(rb_eArgError, "invalid animation string");
    VALUE string = rb_str_new(0, length);
    if (length > 0 && state.reader->ReadRaw(RSTRING_PTR(string), length) != 0)
        rb_raise(rb_eArgError, "truncated animation string");
    return string;
}

double AnimationLoadMantissa(double value, const char* buffer, long length)
{
    if (length <= 0)
        return value;
    --length;
    if (length <= 0 || *buffer++ != 0)
        return value;

    int exponent = 0;
    const int negative = value < 0.0;
    int digit = 0;
    double integerPart = 0.0;
    modf(ldexp(frexp(fabs(value), &exponent), 37), &integerPart);
    value = integerPart;
    do
    {
        unsigned long mantissa = 0;
        switch (length)
        {
        default: mantissa = static_cast<unsigned char>(*buffer++);
        case 3: mantissa = (mantissa << 8) | static_cast<unsigned char>(*buffer++);
        case 2: mantissa = (mantissa << 8) | static_cast<unsigned char>(*buffer++);
        case 1: mantissa = (mantissa << 8) | static_cast<unsigned char>(*buffer++);
        }
        digit -= length < 4 ? static_cast<int>(8 * length) : 32;
        value += ldexp(static_cast<double>(mantissa), digit);
    } while ((length -= 4) > 0);
    value = ldexp(value, exponent - 37);
    return negative ? -value : value;
}

void AnimationRegisterObject(TAnimationDecodeState& state, VALUE object)
{
    rb_ary_push(state.objects, object);
}

void AnimationDecodeIvars(TAnimationDecodeState& state, VALUE object)
{
    int count = 0;
    if (state.reader->ReadLong(count) != 0 || count < 0 || count > 100000)
        rb_raise(rb_eArgError, "invalid animation ivars");
    for (int index = 0; index < count; ++index)
    {
        const int keySymbol = AnimationDecodeSymbol(state);
        VALUE value = AnimationDecodeObject(state);
        const TAnimationSymbol& key = state.archive->symbols[keySymbol];
        if (key.length > 1 && key.name[0] == '@')
        {
            rb_ivar_set(object, rb_intern(key.name), value);
            continue;
        }
        if (RB_TYPE_P(object, T_STRING) && key.length == 1 && key.name[0] == 'E')
        {
            if (value == Qtrue)
                rb_enc_associate(object, rb_utf8_encoding());
            else if (value == Qfalse)
                rb_enc_associate(object, rb_usascii_encoding());
        }
    }
}

VALUE AnimationDecodeArray(TAnimationDecodeState& state, VALUE klass)
{
    int count = 0;
    if (state.reader->ReadLong(count) != 0 || count < 0 || count > 10000000)
        rb_raise(rb_eArgError, "invalid animation array");
    VALUE array = NIL_P(klass) ? rb_ary_new2(count) : rb_obj_alloc(klass);
    AnimationRegisterObject(state, array);
    for (int index = 0; index < count; ++index)
        rb_ary_push(array, AnimationDecodeObject(state));
    return array;
}

VALUE AnimationDecodeObject(TAnimationDecodeState& state)
{
    unsigned char marker = 0;
    if (state.reader->ReadByte(marker) != 0)
        rb_raise(rb_eArgError, "truncated animation object");
    switch (marker)
    {
    case '0': return Qnil;
    case 'T': return Qtrue;
    case 'F': return Qfalse;
    case 'i':
    {
        int value = 0;
        if (state.reader->ReadLong(value) != 0)
            rb_raise(rb_eArgError, "invalid animation integer");
        return INT2NUM(value);
    }
    case ':':
    {
        int length = 0;
        if (state.reader->ReadLong(length) != 0 || length < 0 ||
            state.nextSymbol < 0 || state.nextSymbol >= state.archive->symbolCount ||
            length != state.archive->symbols[state.nextSymbol].length ||
            state.reader->Skip(length) != 0)
            rb_raise(rb_eArgError, "invalid animation symbol definition");
        const TAnimationSymbol& name = state.archive->symbols[state.nextSymbol++];
        return ID2SYM(rb_intern(name.name));
    }
    case ';':
    {
        int symbol = 0;
        if (state.reader->ReadLong(symbol) != 0 || symbol < 0 || symbol >= state.archive->symbolCount)
            rb_raise(rb_eArgError, "invalid animation symbol link");
        return ID2SYM(rb_intern(state.archive->symbols[symbol].name));
    }
    case '"':
    {
        VALUE string = AnimationDecodeRawString(state);
        AnimationRegisterObject(state, string);
        return string;
    }
    case 'f':
    {
        VALUE text = AnimationDecodeRawString(state);
        char* end = 0;
        double value = strtod(RSTRING_PTR(text), &end);
        value = AnimationLoadMantissa(value,
                                      end,
                                      RSTRING_LEN(text) - static_cast<long>(end - RSTRING_PTR(text)));
        VALUE number = rb_float_new(value);
        AnimationRegisterObject(state, number);
        return number;
    }
    case '[':
        return AnimationDecodeArray(state, Qnil);
    case 'I':
    {
        VALUE object = AnimationDecodeObject(state);
        AnimationDecodeIvars(state, object);
        return object;
    }
    case 'C':
    {
        const int classSymbol = AnimationDecodeSymbol(state);
        const TAnimationSymbol& className = state.archive->symbols[classSymbol];
        VALUE klass = rb_path2class(className.name);
        if (state.reader->ReadByte(marker) != 0 || marker != '[')
            rb_raise(rb_eArgError, "unsupported animation user class");
        return AnimationDecodeArray(state, klass);
    }
    case 'o':
    {
        const int classSymbol = AnimationDecodeSymbol(state);
        VALUE klass = rb_path2class(state.archive->symbols[classSymbol].name);
        VALUE object = rb_obj_alloc(klass);
        AnimationRegisterObject(state, object);
        AnimationDecodeIvars(state, object);
        return object;
    }
    case 'u':
    {
        const int classSymbol = AnimationDecodeSymbol(state);
        VALUE klass = rb_path2class(state.archive->symbols[classSymbol].name);
        VALUE data = AnimationDecodeRawString(state);
        VALUE object = rb_funcall(klass, rb_intern("_load"), 1, data);
        AnimationRegisterObject(state, object);
        return object;
    }
    case '@':
    {
        int link = 0;
        if (state.reader->ReadLong(link) != 0)
            rb_raise(rb_eArgError, "invalid animation object link");
        const int local = link - state.objectBase;
        if (local < 0 || local >= RARRAY_LEN(state.objects))
            rb_raise(rb_eArgError, "external animation object link");
        return rb_ary_entry(state.objects, local);
    }
    default:
        rb_raise(rb_eArgError, "unsupported animation marshal type 0x%02x", marker);
        return Qnil;
    }
}

VALUE AnimationDecodeEntry(TAnimationArchive* archive, int index)
{
    if (!archive || index < 0 || index >= archive->entryCount)
        return Qnil;
    const TAnimationEntry& entry = archive->entries[index];
    TAnimationReader reader(archive, entry.offset);
    TAnimationDecodeState state;
    state.archive = archive;
    state.reader = &reader;
    state.objectBase = entry.objectBase;
    state.nextSymbol = entry.symbolBase;
    state.objects = rb_ary_new2(entry.objectCount);
    VALUE object = AnimationDecodeObject(state);
    if (RARRAY_LEN(state.objects) != entry.objectCount)
        rb_raise(rb_eArgError, "animation object index mismatch");
    return object;
}

VALUE NAnimationArchiveCreate(VALUE, VALUE path)
{
    StringValue(path);
    const char* relative = RSTRING_PTR(path);
    const int size = gHost.file_size(gHost.context, relative);
    if (size < 0)
        return RaiseResult("animation archive", size);
    TAnimationArchive* archive = ALLOC(TAnimationArchive);
    memset(archive, 0, sizeof(TAnimationArchive));
    const int pathLength = RSTRING_LEN(path);
    archive->path = ALLOC_N(char, pathLength + 1);
    memcpy(archive->path, relative, pathLength);
    archive->path[pathLength] = 0;
    archive->size = size;
    if (AnimationIndexArchive(archive) != 0)
    {
        AnimationArchiveFree(archive);
        rb_raise(rb_eArgError, "unsupported PkmnAnimations archive");
    }
    return Data_Wrap_Struct(gAnimationArchiveClass, 0, AnimationArchiveFree, archive);
}

VALUE NAnimationArchiveLength(VALUE self)
{
    return INT2NUM(AnimationArchive(self)->entryCount);
}

VALUE NAnimationArchiveGet(VALUE self, VALUE indexValue)
{
    TAnimationArchive* archive = AnimationArchive(self);
    int index = NUM2INT(indexValue);
    if (index < 0)
        index += archive->entryCount;
    return AnimationDecodeEntry(archive, index);
}

VALUE NAnimationArchiveEach(VALUE self)
{
    TAnimationArchive* archive = AnimationArchive(self);
    for (int index = 0; index < archive->entryCount; ++index)
        rb_yield(AnimationDecodeEntry(archive, index));
    return self;
}

VALUE NAnimationArchiveSelected(VALUE self)
{
    return INT2NUM(AnimationArchive(self)->selected);
}

VALUE NAnimationArchiveSetSelected(VALUE self, VALUE value)
{
    AnimationArchive(self)->selected = NUM2INT(value);
    return value;
}

int DecodeText(VALUE text, unsigned int*& codepoints)
{
    StringValue(text);
    const int length = RSTRING_LEN(text);
    codepoints = ALLOC_N(unsigned int, length > 0 ? length : 1);
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(RSTRING_PTR(text));
    int ascii = 1;
    for (int index = 0; index < length; ++index)
    {
        if (bytes[index] >= 0x80)
        {
            ascii = 0;
            break;
        }
    }
    if (ascii)
    {
        for (int index = 0; index < length; ++index)
            codepoints[index] = bytes[index];
        return length;
    }
    const int count = gHost.text_decode(gHost.context,
                                        bytes,
                                        length,
                                        codepoints,
                                        length);
    if (count < 0)
    {
        ruby_xfree(codepoints);
        codepoints = 0;
        RaiseResult("text decode", count);
    }
    return count;
}

VALUE NBitmapCreate(VALUE, VALUE width, VALUE height)
{
    int handle = 0;
    const int result = EasyRpgRgssBitmapCreate(NUM2INT(width), NUM2INT(height), &handle);
    return result == ERgssBitmapOk ? NewHandle(EHandleBitmap, handle) : RaiseResult("Bitmap.new", result);
}

VALUE NBitmapLoad(VALUE, VALUE path)
{
    int handle = 0;
    const int result = gHost.bitmap_load(gHost.context, StringValuePtr(path), &handle);
    return result == ERgssVmOk ? NewHandle(EHandleBitmap, handle) : RaiseResult("Bitmap.new", result);
}

VALUE NBitmapWidth(VALUE, VALUE token)
{
    const int result = EasyRpgRgssBitmapWidth(Handle(token, EHandleBitmap));
    return result >= 0 ? INT2NUM(result) : RaiseResult("Bitmap.width", result);
}

VALUE NBitmapHeight(VALUE, VALUE token)
{
    const int result = EasyRpgRgssBitmapHeight(Handle(token, EHandleBitmap));
    return result >= 0 ? INT2NUM(result) : RaiseResult("Bitmap.height", result);
}

VALUE NBitmapClear(VALUE, VALUE token)
{
    const int result = EasyRpgRgssBitmapClear(Handle(token, EHandleBitmap));
    return result == ERgssBitmapOk ? Qnil : RaiseResult("Bitmap.clear", result);
}

VALUE NBitmapFillRect(int argc, VALUE* argv, VALUE)
{
    if (argc != 9)
        rb_raise(rb_eArgError, "wrong number of arguments");
    const int result = EasyRpgRgssBitmapFillRect(Handle(argv[0], EHandleBitmap),
                                                 NUM2INT(argv[1]),
                                                 NUM2INT(argv[2]),
                                                 NUM2INT(argv[3]),
                                                 NUM2INT(argv[4]),
                                                 NUM2INT(argv[5]),
                                                 NUM2INT(argv[6]),
                                                 NUM2INT(argv[7]),
                                                 NUM2INT(argv[8]));
    return result == ERgssBitmapOk ? Qnil : RaiseResult("Bitmap.fill_rect", result);
}

VALUE NBitmapSetPixel(int argc, VALUE* argv, VALUE)
{
    if (argc != 7)
        rb_raise(rb_eArgError, "wrong number of arguments");
    const int result = EasyRpgRgssBitmapSetPixel(Handle(argv[0], EHandleBitmap),
                                                 NUM2INT(argv[1]),
                                                 NUM2INT(argv[2]),
                                                 NUM2INT(argv[3]),
                                                 NUM2INT(argv[4]),
                                                 NUM2INT(argv[5]),
                                                 NUM2INT(argv[6]));
    return result == ERgssBitmapOk ? Qnil : RaiseResult("Bitmap.set_pixel", result);
}

VALUE NBitmapGetPixel(VALUE, VALUE token, VALUE x, VALUE y)
{
    int red = 0;
    int green = 0;
    int blue = 0;
    int alpha = 0;
    const int result = EasyRpgRgssBitmapGetPixel(Handle(token, EHandleBitmap),
                                                 NUM2INT(x),
                                                 NUM2INT(y),
                                                 &red,
                                                 &green,
                                                 &blue,
                                                 &alpha);
    if (result != ERgssBitmapOk)
        return RaiseResult("Bitmap.get_pixel", result);
    return rb_ary_new3(4, INT2NUM(red), INT2NUM(green), INT2NUM(blue), INT2NUM(alpha));
}

VALUE NBitmapBlt(int argc, VALUE* argv, VALUE)
{
    if (argc != 9)
        rb_raise(rb_eArgError, "wrong number of arguments");
    EasyRpgRgssRect source;
    source.x = NUM2INT(argv[4]);
    source.y = NUM2INT(argv[5]);
    source.width = NUM2INT(argv[6]);
    source.height = NUM2INT(argv[7]);
    const int result = EasyRpgRgssBitmapBlt(Handle(argv[0], EHandleBitmap),
                                            NUM2INT(argv[1]),
                                            NUM2INT(argv[2]),
                                            Handle(argv[3], EHandleBitmap),
                                            &source,
                                            NUM2INT(argv[8]));
    return result == ERgssBitmapOk ? Qnil : RaiseResult("Bitmap.blt", result);
}

VALUE NBitmapStretchBlt(int argc, VALUE* argv, VALUE)
{
    if (argc != 11)
        rb_raise(rb_eArgError, "wrong number of arguments");
    EasyRpgRgssRect destination;
    destination.x = NUM2INT(argv[1]);
    destination.y = NUM2INT(argv[2]);
    destination.width = NUM2INT(argv[3]);
    destination.height = NUM2INT(argv[4]);
    EasyRpgRgssRect source;
    source.x = NUM2INT(argv[6]);
    source.y = NUM2INT(argv[7]);
    source.width = NUM2INT(argv[8]);
    source.height = NUM2INT(argv[9]);
    const int result = EasyRpgRgssBitmapStretchBlt(Handle(argv[0], EHandleBitmap),
                                                   &destination,
                                                   Handle(argv[5], EHandleBitmap),
                                                   &source,
                                                   NUM2INT(argv[10]));
    return result == ERgssBitmapOk ? Qnil : RaiseResult("Bitmap.stretch_blt", result);
}

VALUE NBitmapHueChange(VALUE, VALUE token, VALUE hue)
{
    const int result = EasyRpgRgssBitmapHueChange(Handle(token, EHandleBitmap), NUM2INT(hue));
    return result == ERgssBitmapOk ? Qnil : RaiseResult("Bitmap.hue_change", result);
}

VALUE NBitmapTextSize(int argc, VALUE* argv, VALUE)
{
    if (argc != 4)
        rb_raise(rb_eArgError, "wrong number of arguments");
    unsigned int* codepoints = 0;
    const int count = DecodeText(argv[1], codepoints);
    EasyRpgRgssTextStyle style;
    style.font_size = NUM2INT(argv[2]);
    style.red = 255;
    style.green = 255;
    style.blue = 255;
    style.alpha = 255;
    style.bold = RTEST(argv[3]) ? 1 : 0;
    style.italic = 0;
    style.shadow = 0;
    style.align = 0;
    int width = 0;
    int height = 0;
    const int result = EasyRpgRgssBitmapTextSize(Handle(argv[0], EHandleBitmap),
                                                 codepoints,
                                                 count,
                                                 &style,
                                                 &width,
                                                 &height);
    ruby_xfree(codepoints);
    if (result != ERgssBitmapOk)
        return RaiseResult("Bitmap.text_size", result);
    return rb_ary_new3(2, INT2NUM(width), INT2NUM(height));
}

VALUE NBitmapDrawText(int argc, VALUE* argv, VALUE)
{
    if (argc != 15)
        rb_raise(rb_eArgError, "wrong number of arguments");
    EasyRpgRgssRect rect;
    rect.x = NUM2INT(argv[1]);
    rect.y = NUM2INT(argv[2]);
    rect.width = NUM2INT(argv[3]);
    rect.height = NUM2INT(argv[4]);
    unsigned int* codepoints = 0;
    const int count = DecodeText(argv[5], codepoints);
    EasyRpgRgssTextStyle style;
    style.font_size = NUM2INT(argv[6]);
    style.red = NUM2INT(argv[7]);
    style.green = NUM2INT(argv[8]);
    style.blue = NUM2INT(argv[9]);
    style.alpha = NUM2INT(argv[10]);
    style.bold = RTEST(argv[11]) ? 1 : 0;
    style.italic = RTEST(argv[12]) ? 1 : 0;
    style.shadow = RTEST(argv[13]) ? 1 : 0;
    style.align = NUM2INT(argv[14]);
    const int result = EasyRpgRgssBitmapDrawText(Handle(argv[0], EHandleBitmap),
                                                 &rect,
                                                 codepoints,
                                                 count,
                                                 &style);
    ruby_xfree(codepoints);
    return result == ERgssBitmapOk ? Qnil : RaiseResult("Bitmap.draw_text", result);
}

VALUE NViewportCreate(int argc, VALUE* argv, VALUE)
{
    if (argc != 4)
        rb_raise(rb_eArgError, "wrong number of arguments");
    EasyRpgRgssRect rect;
    rect.x = NUM2INT(argv[0]);
    rect.y = NUM2INT(argv[1]);
    rect.width = NUM2INT(argv[2]);
    rect.height = NUM2INT(argv[3]);
    int handle = 0;
    const int result = EasyRpgRgssViewportCreate(&rect, &handle);
    return result == ERgssViewportOk ? NewHandle(EHandleViewport, handle) : RaiseResult("Viewport.new", result);
}

VALUE NViewportSetRect(int argc, VALUE* argv, VALUE)
{
    if (argc != 5)
        rb_raise(rb_eArgError, "wrong number of arguments");
    EasyRpgRgssRect rect;
    rect.x = NUM2INT(argv[1]);
    rect.y = NUM2INT(argv[2]);
    rect.width = NUM2INT(argv[3]);
    rect.height = NUM2INT(argv[4]);
    const int result = EasyRpgRgssViewportSetRect(Handle(argv[0], EHandleViewport), &rect);
    return result == ERgssViewportOk ? Qnil : RaiseResult("Viewport.rect=", result);
}

VALUE NViewportSetOrigin(VALUE, VALUE token, VALUE x, VALUE y)
{
    const int result = EasyRpgRgssViewportSetOrigin(Handle(token, EHandleViewport), NUM2INT(x), NUM2INT(y));
    return result == ERgssViewportOk ? Qnil : RaiseResult("Viewport.origin", result);
}

VALUE NViewportSetZ(VALUE, VALUE token, VALUE z)
{
    const int result = EasyRpgRgssViewportSetZ(Handle(token, EHandleViewport), NUM2INT(z));
    return result == ERgssViewportOk ? Qnil : RaiseResult("Viewport.z=", result);
}

VALUE NViewportSetVisible(VALUE, VALUE token, VALUE visible)
{
    const int result = EasyRpgRgssViewportSetVisible(Handle(token, EHandleViewport), RTEST(visible) ? 1 : 0);
    return result == ERgssViewportOk ? Qnil : RaiseResult("Viewport.visible=", result);
}

VALUE NPlaneCreate(VALUE)
{
    int handle = 0;
    const int result = EasyRpgRgssPlaneCreate(&handle);
    return result == ERgssPlaneOk ? NewHandle(EHandlePlane, handle) : RaiseResult("Plane.new", result);
}

VALUE NPlaneSetBitmap(VALUE, VALUE token, VALUE bitmap)
{
    const int result = EasyRpgRgssPlaneSetBitmap(Handle(token, EHandlePlane), OptionalHandle(bitmap, EHandleBitmap));
    return result == ERgssPlaneOk ? Qnil : RaiseResult("Plane.bitmap=", result);
}

VALUE NPlaneSetViewport(VALUE, VALUE token, VALUE viewport)
{
    const int result = EasyRpgRgssPlaneSetViewport(Handle(token, EHandlePlane), OptionalHandle(viewport, EHandleViewport));
    return result == ERgssPlaneOk ? Qnil : RaiseResult("Plane.viewport=", result);
}

VALUE NPlaneSetZ(VALUE, VALUE token, VALUE z)
{
    const int result = EasyRpgRgssPlaneSetZ(Handle(token, EHandlePlane), NUM2INT(z));
    return result == ERgssPlaneOk ? Qnil : RaiseResult("Plane.z=", result);
}

VALUE NPlaneSetOrigin(VALUE, VALUE token, VALUE x, VALUE y)
{
    const int result = EasyRpgRgssPlaneSetOrigin(Handle(token, EHandlePlane), NUM2INT(x), NUM2INT(y));
    return result == ERgssPlaneOk ? Qnil : RaiseResult("Plane.origin", result);
}

VALUE NPlaneSetZoom(VALUE, VALUE token, VALUE x, VALUE y)
{
    const int result = EasyRpgRgssPlaneSetZoom(Handle(token, EHandlePlane), NUM2INT(x), NUM2INT(y));
    return result == ERgssPlaneOk ? Qnil : RaiseResult("Plane.zoom", result);
}

VALUE NPlaneSetOpacity(VALUE, VALUE token, VALUE opacity)
{
    const int result = EasyRpgRgssPlaneSetOpacity(Handle(token, EHandlePlane), NUM2INT(opacity));
    return result == ERgssPlaneOk ? Qnil : RaiseResult("Plane.opacity=", result);
}

VALUE NPlaneSetVisible(VALUE, VALUE token, VALUE visible)
{
    const int result = EasyRpgRgssPlaneSetVisible(Handle(token, EHandlePlane), RTEST(visible) ? 1 : 0);
    return result == ERgssPlaneOk ? Qnil : RaiseResult("Plane.visible=", result);
}

VALUE NPlaneSetBlendType(VALUE, VALUE token, VALUE blend)
{
    const int result = EasyRpgRgssPlaneSetBlendType(Handle(token, EHandlePlane), NUM2INT(blend));
    return result == ERgssPlaneOk ? Qnil : RaiseResult("Plane.blend_type=", result);
}

VALUE NSpriteCreate(VALUE)
{
    int handle = 0;
    const int result = EasyRpgRgssSpriteCreate(&handle);
    return result == ERgssSpriteOk ? NewHandle(EHandleSprite, handle) : RaiseResult("Sprite.new", result);
}

VALUE NSpriteSetBitmap(VALUE, VALUE token, VALUE bitmap)
{
    const int result = EasyRpgRgssSpriteSetBitmap(Handle(token, EHandleSprite), OptionalHandle(bitmap, EHandleBitmap));
    return result == ERgssSpriteOk ? Qnil : RaiseResult("Sprite.bitmap=", result);
}

VALUE NSpriteSetViewport(VALUE, VALUE token, VALUE viewport)
{
    const int result = EasyRpgRgssSpriteSetViewport(Handle(token, EHandleSprite), OptionalHandle(viewport, EHandleViewport));
    return result == ERgssSpriteOk ? Qnil : RaiseResult("Sprite.viewport=", result);
}

VALUE NSpriteSetSourceRect(int argc, VALUE* argv, VALUE)
{
    if (argc != 5)
        rb_raise(rb_eArgError, "wrong number of arguments");
    EasyRpgRgssRect rect;
    rect.x = NUM2INT(argv[1]);
    rect.y = NUM2INT(argv[2]);
    rect.width = NUM2INT(argv[3]);
    rect.height = NUM2INT(argv[4]);
    const int result = EasyRpgRgssSpriteSetSourceRect(Handle(argv[0], EHandleSprite), &rect);
    return result == ERgssSpriteOk ? Qnil : RaiseResult("Sprite.src_rect=", result);
}

VALUE NSpriteSetPosition(VALUE, VALUE token, VALUE x, VALUE y, VALUE z)
{
    const int result = EasyRpgRgssSpriteSetPosition(Handle(token, EHandleSprite), NUM2INT(x), NUM2INT(y), NUM2INT(z));
    return result == ERgssSpriteOk ? Qnil : RaiseResult("Sprite.position", result);
}

VALUE NSpriteSetOrigin(VALUE, VALUE token, VALUE x, VALUE y)
{
    const int result = EasyRpgRgssSpriteSetOrigin(Handle(token, EHandleSprite), NUM2INT(x), NUM2INT(y));
    return result == ERgssSpriteOk ? Qnil : RaiseResult("Sprite.origin", result);
}

VALUE NSpriteSetZoom(VALUE, VALUE token, VALUE x, VALUE y)
{
    const int result = EasyRpgRgssSpriteSetZoom(Handle(token, EHandleSprite), NUM2INT(x), NUM2INT(y));
    return result == ERgssSpriteOk ? Qnil : RaiseResult("Sprite.zoom", result);
}

VALUE NSpriteSetOpacity(VALUE, VALUE token, VALUE opacity)
{
    const int result = EasyRpgRgssSpriteSetOpacity(Handle(token, EHandleSprite), NUM2INT(opacity));
    return result == ERgssSpriteOk ? Qnil : RaiseResult("Sprite.opacity=", result);
}

VALUE NSpriteSetVisible(VALUE, VALUE token, VALUE visible)
{
    const int result = EasyRpgRgssSpriteSetVisible(Handle(token, EHandleSprite), RTEST(visible) ? 1 : 0);
    return result == ERgssSpriteOk ? Qnil : RaiseResult("Sprite.visible=", result);
}

VALUE NSpriteSetMirror(VALUE, VALUE token, VALUE mirror)
{
    const int result = EasyRpgRgssSpriteSetMirror(Handle(token, EHandleSprite), RTEST(mirror) ? 1 : 0);
    return result == ERgssSpriteOk ? Qnil : RaiseResult("Sprite.mirror=", result);
}

VALUE NSpriteSetBlendType(VALUE, VALUE token, VALUE blend)
{
    const int result = EasyRpgRgssSpriteSetBlendType(Handle(token, EHandleSprite), NUM2INT(blend));
    return result == ERgssSpriteOk ? Qnil : RaiseResult("Sprite.blend_type=", result);
}

VALUE NSpriteSetTone(int argc, VALUE* argv, VALUE)
{
    if (argc != 5)
        rb_raise(rb_eArgError, "wrong number of arguments");
    const int result = EasyRpgRgssSpriteSetTone(Handle(argv[0], EHandleSprite),
                                                NUM2INT(argv[1]),
                                                NUM2INT(argv[2]),
                                                NUM2INT(argv[3]),
                                                NUM2INT(argv[4]));
    return result == ERgssSpriteOk ? Qnil : RaiseResult("Sprite.tone=", result);
}

VALUE NSpriteSetColor(int argc, VALUE* argv, VALUE)
{
    if (argc != 5)
        rb_raise(rb_eArgError, "wrong number of arguments");
    const int result = EasyRpgRgssSpriteSetColor(Handle(argv[0], EHandleSprite),
                                                 NUM2INT(argv[1]),
                                                 NUM2INT(argv[2]),
                                                 NUM2INT(argv[3]),
                                                 NUM2INT(argv[4]));
    return result == ERgssSpriteOk ? Qnil : RaiseResult("Sprite.color=", result);
}

VALUE NSpriteSetBush(VALUE, VALUE token, VALUE depth, VALUE opacity)
{
    const int result = EasyRpgRgssSpriteSetBush(Handle(token, EHandleSprite), NUM2INT(depth), NUM2INT(opacity));
    return result == ERgssSpriteOk ? Qnil : RaiseResult("Sprite.bush", result);
}

VALUE NTableCreate(int argc, VALUE* argv, VALUE)
{
    if (argc < 1 || argc > 3)
        rb_raise(rb_eArgError, "wrong number of arguments");
    const int dimensions = argc;
    const int x = NUM2INT(argv[0]);
    const int y = argc >= 2 ? NUM2INT(argv[1]) : 1;
    const int z = argc >= 3 ? NUM2INT(argv[2]) : 1;
    int handle = 0;
    const int result = EasyRpgRgssTableCreate(dimensions, x, y, z, &handle);
    return result == ERgssTableOk ? NewHandle(EHandleTable, handle) : RaiseResult("Table.new", result);
}

VALUE NTableDimensions(VALUE, VALUE token)
{
    const int result = EasyRpgRgssTableDimensions(Handle(token, EHandleTable));
    return result >= 0 ? INT2NUM(result) : RaiseResult("Table.dimensions", result);
}

VALUE NTableXSize(VALUE, VALUE token)
{
    const int result = EasyRpgRgssTableXSize(Handle(token, EHandleTable));
    return result >= 0 ? INT2NUM(result) : RaiseResult("Table.xsize", result);
}

VALUE NTableYSize(VALUE, VALUE token)
{
    const int result = EasyRpgRgssTableYSize(Handle(token, EHandleTable));
    return result >= 0 ? INT2NUM(result) : RaiseResult("Table.ysize", result);
}

VALUE NTableZSize(VALUE, VALUE token)
{
    const int result = EasyRpgRgssTableZSize(Handle(token, EHandleTable));
    return result >= 0 ? INT2NUM(result) : RaiseResult("Table.zsize", result);
}

VALUE NTableGet(int argc, VALUE* argv, VALUE)
{
    if (argc < 2 || argc > 4)
        rb_raise(rb_eArgError, "wrong number of arguments");
    int value = 0;
    const int result = EasyRpgRgssTableGet(Handle(argv[0], EHandleTable),
                                           NUM2INT(argv[1]),
                                           argc >= 3 ? NUM2INT(argv[2]) : 0,
                                           argc >= 4 ? NUM2INT(argv[3]) : 0,
                                           &value);
    if (result == ERgssTableErrorOutOfBounds)
        return Qnil;
    return result == ERgssTableOk ? INT2NUM(value) : RaiseResult("Table.[]", result);
}

VALUE NTableSet(int argc, VALUE* argv, VALUE)
{
    if (argc < 3 || argc > 5)
        rb_raise(rb_eArgError, "wrong number of arguments");
    int x = NUM2INT(argv[1]);
    int y = 0;
    int z = 0;
    VALUE value = Qnil;
    if (argc == 3)
        value = argv[2];
    else if (argc == 4)
    {
        y = NUM2INT(argv[2]);
        value = argv[3];
    }
    else
    {
        y = NUM2INT(argv[2]);
        z = NUM2INT(argv[3]);
        value = argv[4];
    }
    const int result = EasyRpgRgssTableSet(Handle(argv[0], EHandleTable), x, y, z, NUM2INT(value));
    if (result == ERgssTableErrorOutOfBounds)
        return Qnil;
    return result == ERgssTableOk ? value : RaiseResult("Table.[]=", result);
}

VALUE NTableResize(int argc, VALUE* argv, VALUE)
{
    if (argc < 2 || argc > 4)
        rb_raise(rb_eArgError, "wrong number of arguments");
    const int dimensions = argc - 1;
    const int result = EasyRpgRgssTableResize(Handle(argv[0], EHandleTable),
                                              dimensions,
                                              NUM2INT(argv[1]),
                                              argc >= 3 ? NUM2INT(argv[2]) : 1,
                                              argc >= 4 ? NUM2INT(argv[3]) : 1);
    return result == ERgssTableOk ? Qnil : RaiseResult("Table.resize", result);
}

void Write32(unsigned char* output, int value)
{
    const unsigned int v = static_cast<unsigned int>(value);
    output[0] = static_cast<unsigned char>(v & 0xffu);
    output[1] = static_cast<unsigned char>((v >> 8) & 0xffu);
    output[2] = static_cast<unsigned char>((v >> 16) & 0xffu);
    output[3] = static_cast<unsigned char>((v >> 24) & 0xffu);
}

int Read32(const unsigned char* input)
{
    const unsigned int value = static_cast<unsigned int>(input[0]) |
        (static_cast<unsigned int>(input[1]) << 8) |
        (static_cast<unsigned int>(input[2]) << 16) |
        (static_cast<unsigned int>(input[3]) << 24);
    return static_cast<int>(value);
}

VALUE NTableDump(VALUE, VALUE token)
{
    const int handle = Handle(token, EHandleTable);
    const int dimensions = EasyRpgRgssTableDimensions(handle);
    const int x = EasyRpgRgssTableXSize(handle);
    const int y = EasyRpgRgssTableYSize(handle);
    const int z = EasyRpgRgssTableZSize(handle);
    short* data = 0;
    int count = 0;
    const int lockResult = EasyRpgRgssTableLock(handle, &data, &count);
    if (dimensions < 1 || x < 1 || y < 1 || z < 1 || lockResult != ERgssTableOk)
        return RaiseResult("Table._dump", lockResult);
    if (count > (0x7fffffff - 20) / 2)
        rb_raise(rb_eRuntimeError, "table too large");
    VALUE output = rb_str_new(0, 20 + count * 2);
    unsigned char* bytes = reinterpret_cast<unsigned char*>(RSTRING_PTR(output));
    Write32(bytes + 0, dimensions);
    Write32(bytes + 4, x);
    Write32(bytes + 8, y);
    Write32(bytes + 12, z);
    Write32(bytes + 16, count);
    for (int index = 0; index < count; ++index)
    {
        const unsigned short value = static_cast<unsigned short>(data[index]);
        bytes[20 + index * 2] = static_cast<unsigned char>(value & 0xffu);
        bytes[21 + index * 2] = static_cast<unsigned char>((value >> 8) & 0xffu);
    }
    return output;
}

VALUE NTableLoad(VALUE, VALUE packed)
{
    StringValue(packed);
    const int length = RSTRING_LEN(packed);
    if (length < 20)
        rb_raise(rb_eArgError, "invalid table data");
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(RSTRING_PTR(packed));
    const int dimensions = Read32(bytes + 0);
    const int x = Read32(bytes + 4);
    const int y = Read32(bytes + 8);
    const int z = Read32(bytes + 12);
    const int count = Read32(bytes + 16);
    if (dimensions < 1 || dimensions > 3 || x <= 0 || y <= 0 || z <= 0 ||
        count < 0 || count > (0x7fffffff - 20) / 2 || length != 20 + count * 2)
        rb_raise(rb_eArgError, "invalid table data");
    int handle = 0;
    int result = EasyRpgRgssTableCreate(dimensions, x, y, z, &handle);
    if (result != ERgssTableOk)
        return RaiseResult("Table._load", result);
    short* data = ALLOC_N(short, count > 0 ? count : 1);
    for (int index = 0; index < count; ++index)
    {
        const unsigned short value = static_cast<unsigned short>(bytes[20 + index * 2]) |
            static_cast<unsigned short>(static_cast<unsigned short>(bytes[21 + index * 2]) << 8);
        data[index] = static_cast<short>(value);
    }
    result = EasyRpgRgssTableLoadRaw(handle, data, count);
    ruby_xfree(data);
    if (result != ERgssTableOk)
    {
        EasyRpgRgssTableDestroy(handle);
        return RaiseResult("Table._load", result);
    }
    return NewHandle(EHandleTable, handle);
}

VALUE NTilemapCreate(VALUE, VALUE viewport)
{
    int handle = 0;
    const int result = EasyRpgRgssTilemapCreate(OptionalHandle(viewport, EHandleViewport), &handle);
    return result == ERgssTilemapOk ? NewHandle(EHandleTilemap, handle) : RaiseResult("Tilemap.new", result);
}

VALUE NTilemapSetViewport(VALUE, VALUE token, VALUE viewport)
{
    const int result = EasyRpgRgssTilemapSetViewport(Handle(token, EHandleTilemap), OptionalHandle(viewport, EHandleViewport));
    return result == ERgssTilemapOk ? Qnil : RaiseResult("Tilemap.viewport=", result);
}

VALUE NTilemapSetTileset(VALUE, VALUE token, VALUE bitmap)
{
    const int result = EasyRpgRgssTilemapSetTileset(Handle(token, EHandleTilemap), OptionalHandle(bitmap, EHandleBitmap));
    return result == ERgssTilemapOk ? Qnil : RaiseResult("Tilemap.tileset=", result);
}

VALUE NTilemapSetAutotile(VALUE, VALUE token, VALUE index, VALUE bitmap)
{
    const int result = EasyRpgRgssTilemapSetAutotile(Handle(token, EHandleTilemap), NUM2INT(index), OptionalHandle(bitmap, EHandleBitmap));
    return result == ERgssTilemapOk ? Qnil : RaiseResult("Tilemap.autotiles[]=", result);
}

VALUE NTilemapSetMapData(VALUE, VALUE token, VALUE table)
{
    const int result = EasyRpgRgssTilemapSetMapData(Handle(token, EHandleTilemap), OptionalHandle(table, EHandleTable));
    return result == ERgssTilemapOk ? Qnil : RaiseResult("Tilemap.map_data=", result);
}

VALUE NTilemapSetFlashData(VALUE, VALUE token, VALUE table)
{
    const int result = EasyRpgRgssTilemapSetFlashData(Handle(token, EHandleTilemap), OptionalHandle(table, EHandleTable));
    return result == ERgssTilemapOk ? Qnil : RaiseResult("Tilemap.flash_data=", result);
}

VALUE NTilemapSetPriorities(VALUE, VALUE token, VALUE table)
{
    const int result = EasyRpgRgssTilemapSetPriorities(Handle(token, EHandleTilemap), OptionalHandle(table, EHandleTable));
    return result == ERgssTilemapOk ? Qnil : RaiseResult("Tilemap.priorities=", result);
}

VALUE NTilemapSetOrigin(VALUE, VALUE token, VALUE x, VALUE y)
{
    const int result = EasyRpgRgssTilemapSetOrigin(Handle(token, EHandleTilemap), NUM2INT(x), NUM2INT(y));
    return result == ERgssTilemapOk ? Qnil : RaiseResult("Tilemap.origin", result);
}

VALUE NTilemapSetVisible(VALUE, VALUE token, VALUE visible)
{
    const int result = EasyRpgRgssTilemapSetVisible(Handle(token, EHandleTilemap), RTEST(visible) ? 1 : 0);
    return result == ERgssTilemapOk ? Qnil : RaiseResult("Tilemap.visible=", result);
}

VALUE NTilemapUpdate(VALUE, VALUE token)
{
    const int result = EasyRpgRgssTilemapUpdate(Handle(token, EHandleTilemap));
    return result == ERgssTilemapOk ? Qnil : RaiseResult("Tilemap.update", result);
}

VALUE NAudioCommand(VALUE, VALUE command, VALUE path, VALUE volume, VALUE pitch, VALUE duration)
{
    const char* audioPath = 0;
    if (!NIL_P(path))
    {
        StringValue(path);
        audioPath = RSTRING_PTR(path);
    }
    const int result = gHost.audio(gHost.context,
                                   NUM2INT(command),
                                   audioPath,
                                   NUM2INT(volume),
                                   NUM2INT(pitch),
                                   NUM2INT(duration));
    return result == ERgssVmOk ? Qnil : RaiseResult("Audio", result);
}

void DefineNativeMethod(const char* name, VALUE (*method)(ANYARGS), int arity)
{
    switch (arity)
    {
    case -1: rb_define_module_function(gNativeModule, name, method, -1); break;
    case 0: rb_define_module_function(gNativeModule, name, method, 0); break;
    case 1: rb_define_module_function(gNativeModule, name, method, 1); break;
    case 2: rb_define_module_function(gNativeModule, name, method, 2); break;
    case 3: rb_define_module_function(gNativeModule, name, method, 3); break;
    case 4: rb_define_module_function(gNativeModule, name, method, 4); break;
    case 5: rb_define_module_function(gNativeModule, name, method, 5); break;
    default: rb_raise(rb_eArgError, "unsupported native arity");
    }
}

void BindNative()
{
    gNativeModule = rb_define_module("EasyRPGNative");
    gHandleClass = rb_define_class_under(gNativeModule, "Handle", rb_cObject);
    gVfsInputClass = rb_define_class_under(gNativeModule, "VfsInput", rb_cObject);
    gAnimationArchiveClass = rb_define_class_under(gNativeModule, "AnimationArchive", rb_cObject);
    rb_define_method(gVfsInputClass, "binmode", RUBY_METHOD_FUNC(NVfsInputBinmode), 0);
    rb_define_method(gVfsInputClass, "getbyte", RUBY_METHOD_FUNC(NVfsInputGetbyte), 0);
    rb_define_method(gVfsInputClass, "read", RUBY_METHOD_FUNC(NVfsInputRead), -1);
    rb_define_method(gAnimationArchiveClass, "length", RUBY_METHOD_FUNC(NAnimationArchiveLength), 0);
    rb_define_method(gAnimationArchiveClass, "size", RUBY_METHOD_FUNC(NAnimationArchiveLength), 0);
    rb_define_method(gAnimationArchiveClass, "[]", RUBY_METHOD_FUNC(NAnimationArchiveGet), 1);
    rb_define_method(gAnimationArchiveClass, "each", RUBY_METHOD_FUNC(NAnimationArchiveEach), 0);
    rb_define_method(gAnimationArchiveClass, "selected", RUBY_METHOD_FUNC(NAnimationArchiveSelected), 0);
    rb_define_method(gAnimationArchiveClass, "selected=", RUBY_METHOD_FUNC(NAnimationArchiveSetSelected), 1);

    DefineNativeMethod("vfs_input", RUBY_METHOD_FUNC(NVfsInputCreate), 1);
    DefineNativeMethod("animation_archive", RUBY_METHOD_FUNC(NAnimationArchiveCreate), 1);
    DefineNativeMethod("load_deferred_battle", RUBY_METHOD_FUNC(NLoadDeferredBattle), 0);
    DefineNativeMethod("load_deferred_optional", RUBY_METHOD_FUNC(NLoadDeferredOptional), 0);
    DefineNativeMethod("handle_dispose", RUBY_METHOD_FUNC(NHandleDispose), 1);
    DefineNativeMethod("handle_disposed", RUBY_METHOD_FUNC(NHandleDisposed), 1);
    DefineNativeMethod("graphics_update", RUBY_METHOD_FUNC(NGraphicsUpdate), 0);
    DefineNativeMethod("graphics_frame_rate", RUBY_METHOD_FUNC(NGraphicsFrameRate), 0);
    DefineNativeMethod("graphics_frame_rate_set", RUBY_METHOD_FUNC(NGraphicsSetFrameRate), 1);
    DefineNativeMethod("graphics_frame_count", RUBY_METHOD_FUNC(NGraphicsFrameCount), 0);
    DefineNativeMethod("graphics_frame_count_set", RUBY_METHOD_FUNC(NGraphicsSetFrameCount), 1);
    DefineNativeMethod("graphics_width", RUBY_METHOD_FUNC(NGraphicsWidth), 0);
    DefineNativeMethod("graphics_height", RUBY_METHOD_FUNC(NGraphicsHeight), 0);
    DefineNativeMethod("graphics_resize", RUBY_METHOD_FUNC(NGraphicsResize), 2);
    DefineNativeMethod("input_update", RUBY_METHOD_FUNC(NInputUpdate), 0);
    DefineNativeMethod("input_press", RUBY_METHOD_FUNC(NInputPress), 1);
    DefineNativeMethod("input_trigger", RUBY_METHOD_FUNC(NInputTrigger), 1);
    DefineNativeMethod("input_repeat", RUBY_METHOD_FUNC(NInputRepeat), 1);
    DefineNativeMethod("system_delta", RUBY_METHOD_FUNC(NSystemDelta), 0);
    DefineNativeMethod("trace_scene", RUBY_METHOD_FUNC(NTraceScene), 1);
    DefineNativeMethod("zlib_inflate", RUBY_METHOD_FUNC(NZlibInflate), 1);
    DefineNativeMethod("zlib_deflate", RUBY_METHOD_FUNC(NZlibDeflate), 1);
    DefineNativeMethod("file_size", RUBY_METHOD_FUNC(NFileSize), 1);
    DefineNativeMethod("file_read", RUBY_METHOD_FUNC(NFileRead), 1);
    DefineNativeMethod("bitmap_create", RUBY_METHOD_FUNC(NBitmapCreate), 2);
    DefineNativeMethod("bitmap_load", RUBY_METHOD_FUNC(NBitmapLoad), 1);
    DefineNativeMethod("bitmap_width", RUBY_METHOD_FUNC(NBitmapWidth), 1);
    DefineNativeMethod("bitmap_height", RUBY_METHOD_FUNC(NBitmapHeight), 1);
    DefineNativeMethod("bitmap_clear", RUBY_METHOD_FUNC(NBitmapClear), 1);
    DefineNativeMethod("bitmap_fill_rect", RUBY_METHOD_FUNC(NBitmapFillRect), -1);
    DefineNativeMethod("bitmap_set_pixel", RUBY_METHOD_FUNC(NBitmapSetPixel), -1);
    DefineNativeMethod("bitmap_get_pixel", RUBY_METHOD_FUNC(NBitmapGetPixel), 3);
    DefineNativeMethod("bitmap_blt", RUBY_METHOD_FUNC(NBitmapBlt), -1);
    DefineNativeMethod("bitmap_stretch_blt", RUBY_METHOD_FUNC(NBitmapStretchBlt), -1);
    DefineNativeMethod("bitmap_hue_change", RUBY_METHOD_FUNC(NBitmapHueChange), 2);
    DefineNativeMethod("bitmap_text_size", RUBY_METHOD_FUNC(NBitmapTextSize), -1);
    DefineNativeMethod("bitmap_draw_text", RUBY_METHOD_FUNC(NBitmapDrawText), -1);
    DefineNativeMethod("viewport_create", RUBY_METHOD_FUNC(NViewportCreate), -1);
    DefineNativeMethod("viewport_set_rect", RUBY_METHOD_FUNC(NViewportSetRect), -1);
    DefineNativeMethod("viewport_set_origin", RUBY_METHOD_FUNC(NViewportSetOrigin), 3);
    DefineNativeMethod("viewport_set_z", RUBY_METHOD_FUNC(NViewportSetZ), 2);
    DefineNativeMethod("viewport_set_visible", RUBY_METHOD_FUNC(NViewportSetVisible), 2);
    DefineNativeMethod("plane_create", RUBY_METHOD_FUNC(NPlaneCreate), 0);
    DefineNativeMethod("plane_set_bitmap", RUBY_METHOD_FUNC(NPlaneSetBitmap), 2);
    DefineNativeMethod("plane_set_viewport", RUBY_METHOD_FUNC(NPlaneSetViewport), 2);
    DefineNativeMethod("plane_set_z", RUBY_METHOD_FUNC(NPlaneSetZ), 2);
    DefineNativeMethod("plane_set_origin", RUBY_METHOD_FUNC(NPlaneSetOrigin), 3);
    DefineNativeMethod("plane_set_zoom", RUBY_METHOD_FUNC(NPlaneSetZoom), 3);
    DefineNativeMethod("plane_set_opacity", RUBY_METHOD_FUNC(NPlaneSetOpacity), 2);
    DefineNativeMethod("plane_set_visible", RUBY_METHOD_FUNC(NPlaneSetVisible), 2);
    DefineNativeMethod("plane_set_blend_type", RUBY_METHOD_FUNC(NPlaneSetBlendType), 2);
    DefineNativeMethod("sprite_create", RUBY_METHOD_FUNC(NSpriteCreate), 0);
    DefineNativeMethod("sprite_set_bitmap", RUBY_METHOD_FUNC(NSpriteSetBitmap), 2);
    DefineNativeMethod("sprite_set_viewport", RUBY_METHOD_FUNC(NSpriteSetViewport), 2);
    DefineNativeMethod("sprite_set_source_rect", RUBY_METHOD_FUNC(NSpriteSetSourceRect), -1);
    DefineNativeMethod("sprite_set_position", RUBY_METHOD_FUNC(NSpriteSetPosition), 4);
    DefineNativeMethod("sprite_set_origin", RUBY_METHOD_FUNC(NSpriteSetOrigin), 3);
    DefineNativeMethod("sprite_set_zoom", RUBY_METHOD_FUNC(NSpriteSetZoom), 3);
    DefineNativeMethod("sprite_set_opacity", RUBY_METHOD_FUNC(NSpriteSetOpacity), 2);
    DefineNativeMethod("sprite_set_visible", RUBY_METHOD_FUNC(NSpriteSetVisible), 2);
    DefineNativeMethod("sprite_set_mirror", RUBY_METHOD_FUNC(NSpriteSetMirror), 2);
    DefineNativeMethod("sprite_set_blend_type", RUBY_METHOD_FUNC(NSpriteSetBlendType), 2);
    DefineNativeMethod("sprite_set_tone", RUBY_METHOD_FUNC(NSpriteSetTone), -1);
    DefineNativeMethod("sprite_set_color", RUBY_METHOD_FUNC(NSpriteSetColor), -1);
    DefineNativeMethod("sprite_set_bush", RUBY_METHOD_FUNC(NSpriteSetBush), 3);
    DefineNativeMethod("table_create", RUBY_METHOD_FUNC(NTableCreate), -1);
    DefineNativeMethod("table_dimensions", RUBY_METHOD_FUNC(NTableDimensions), 1);
    DefineNativeMethod("table_xsize", RUBY_METHOD_FUNC(NTableXSize), 1);
    DefineNativeMethod("table_ysize", RUBY_METHOD_FUNC(NTableYSize), 1);
    DefineNativeMethod("table_zsize", RUBY_METHOD_FUNC(NTableZSize), 1);
    DefineNativeMethod("table_get", RUBY_METHOD_FUNC(NTableGet), -1);
    DefineNativeMethod("table_set", RUBY_METHOD_FUNC(NTableSet), -1);
    DefineNativeMethod("table_resize", RUBY_METHOD_FUNC(NTableResize), -1);
    DefineNativeMethod("table_dump", RUBY_METHOD_FUNC(NTableDump), 1);
    DefineNativeMethod("table_load", RUBY_METHOD_FUNC(NTableLoad), 1);
    DefineNativeMethod("tilemap_create", RUBY_METHOD_FUNC(NTilemapCreate), 1);
    DefineNativeMethod("tilemap_set_viewport", RUBY_METHOD_FUNC(NTilemapSetViewport), 2);
    DefineNativeMethod("tilemap_set_tileset", RUBY_METHOD_FUNC(NTilemapSetTileset), 2);
    DefineNativeMethod("tilemap_set_autotile", RUBY_METHOD_FUNC(NTilemapSetAutotile), 3);
    DefineNativeMethod("tilemap_set_map_data", RUBY_METHOD_FUNC(NTilemapSetMapData), 2);
    DefineNativeMethod("tilemap_set_flash_data", RUBY_METHOD_FUNC(NTilemapSetFlashData), 2);
    DefineNativeMethod("tilemap_set_priorities", RUBY_METHOD_FUNC(NTilemapSetPriorities), 2);
    DefineNativeMethod("tilemap_set_origin", RUBY_METHOD_FUNC(NTilemapSetOrigin), 3);
    DefineNativeMethod("tilemap_set_visible", RUBY_METHOD_FUNC(NTilemapSetVisible), 2);
    DefineNativeMethod("tilemap_update", RUBY_METHOD_FUNC(NTilemapUpdate), 1);
    DefineNativeMethod("audio_command", RUBY_METHOD_FUNC(NAudioCommand), 5);
}

int EvalProtected(const char* source, int scriptIndex)
{
    int state = 0;
    rb_eval_string_protect(source, &state);
    if (state == 0)
        return ERgssVmOk;
    VALUE error = rb_errinfo();
    gLastErrorScript = scriptIndex;
    if (!NIL_P(error))
    {
        VALUE message = rb_obj_as_string(error);
        const char* klass = rb_obj_classname(error);
        const char* text = StringValuePtr(message);
        snprintf(gLastError, sizeof(gLastError), "%s: %s", klass ? klass : "Exception", text ? text : "");
        gLastError[sizeof(gLastError) - 1] = 0;
        VALUE backtrace = rb_funcall(error, rb_intern("backtrace"), 0);
        if (RB_TYPE_P(backtrace, T_ARRAY) && RARRAY_LEN(backtrace) > 0)
        {
            VALUE line = rb_ary_entry(backtrace, 0);
            StringValue(line);
            const size_t used = strlen(gLastError);
            if (used + 2 < sizeof(gLastError))
            {
                snprintf(gLastError + used, sizeof(gLastError) - used, "\n%s", RSTRING_PTR(line));
                gLastError[sizeof(gLastError) - 1] = 0;
            }
        }
    }
    else
    {
        strncpy(gLastError, "Ruby script error", sizeof(gLastError) - 1);
        gLastError[sizeof(gLastError) - 1] = 0;
    }
    {
        char prefix[64];
        snprintf(prefix, sizeof(prefix), "ruby error script=%d:", scriptIndex);
        EasyRpgRgssVmTraceHeap(prefix);
        EasyRpgRgssVmTrace(gLastError);
    }
    return ERgssVmErrorScript;
}
}

extern "C" int EasyRpgRgssVmAvailable(void)
{
    return 1;
}

extern "C" int EasyRpgRgssVmInitialize(int version,
                                        const EasyRpgRgssVmHost* host)
{
    if (version < 1 || version > 3 || !host ||
        !host->file_size || !host->file_read || !host->ticks ||
        !host->input || !host->bitmap_load || !host->text_decode ||
        !host->frame_sync || !host->script_count || !host->script_info ||
        !host->script_name || !host->script_inflate || !host->release ||
        !host->dirty || !host->audio)
        return ERgssVmErrorInvalidArgument;
    if (gInitialized)
        return ERgssVmErrorRuntime;

    ResetHost();
    gHost = *host;
    gVersion = version;
    RUBY_INIT_STACK;
    ruby_init();
    EasyRpgRgssVmTraceHeap("ruby init done");
    rb_define_const(rb_cEncoding, "ASCII_8BIT", rb_enc_from_encoding(rb_ascii8bit_encoding()));
    rb_define_const(rb_cEncoding, "UTF_8", rb_enc_from_encoding(rb_utf8_encoding()));
    rb_define_const(rb_cEncoding, "US_ASCII", rb_enc_from_encoding(rb_usascii_encoding()));
    rb_enc_alias("BINARY", "ASCII-8BIT");
    rb_enc_alias("ASCII", "US-ASCII");
    rb_call_builtin_inits();
    BindNative();
    const char* bootstrapChunks[] = {
        KEasyRpgRgssBootstrapCore,
        KEasyRpgRgssBootstrapGraphics,
        KEasyRpgRgssBootstrapWindow,
        KEasyRpgRgssBootstrapRuntime
    };
    for (int index = 0; index < 4; ++index)
    {
        const int bootstrapResult = EvalProtected(bootstrapChunks[index], -1);
        if (bootstrapResult != ERgssVmOk)
        {
            ResetHost();
            return bootstrapResult;
        }
    }
    EasyRpgRgssVmTraceHeap("bootstrap done");
    gInitialized = 1;
    return ERgssVmOk;
}

namespace
{
int ModernScriptName(int index, char* name, int capacity)
{
    int id = 0;
    int length = 0;
    if (!name || capacity <= 1 ||
        gHost.script_info(gHost.context, index, &id, &length) != ERgssVmOk ||
        length <= 0 || length >= capacity)
        return 0;
    const int read = gHost.script_name(gHost.context, index, name, length);
    if (read != length)
        return 0;
    name[length] = 0;
    return 1;
}

int SkipModernReleaseScript(int index)
{
    char name[64];
    if (!ModernScriptName(index, name, sizeof(name)))
        return 0;
    return strncmp(name, "Editor", 6) == 0 ||
           strncmp(name, "AnimEditor_", 11) == 0 ||
           strncmp(name, "Debug_", 6) == 0 ||
           strcmp(name, "Compiler") == 0 ||
           strncmp(name, "Compiler_", 9) == 0;
}

int DeferModernBattleScript(int index)
{
    char name[64];
    if (!ModernScriptName(index, name, sizeof(name)))
        return 0;
    return strcmp(name, "PBEffects") == 0 ||
           strncmp(name, "BattleHandlers", 14) == 0 ||
           strncmp(name, "BallHandlers_", 13) == 0 ||
           strncmp(name, "PokeBattle_", 11) == 0 ||
           strncmp(name, "Battler_", 8) == 0 ||
           strncmp(name, "Move_Usage", 10) == 0 ||
           strncmp(name, "Move_Effects_", 13) == 0 ||
           strncmp(name, "Battle_", 7) == 0 ||
           strncmp(name, "AI_", 3) == 0 ||
           strcmp(name, "Scene_Initialize") == 0 ||
           strcmp(name, "Scene_Commands") == 0 ||
           strcmp(name, "Scene_Animations") == 0 ||
           strcmp(name, "ShadowPokemon_Other") == 0;
}

int DeferModernOptionalScript(int index)
{
    char name[64];
    if (!ModernScriptName(index, name, sizeof(name)))
        return 0;
    if (strncmp(name, "UI_", 3) == 0 &&
        strcmp(name, "UI_Load") != 0 &&
        strcmp(name, "UI_Options") != 0 &&
        strcmp(name, "UI_SplashesAndTitleScreen") != 0 &&
        strcmp(name, "UI_Controls") != 0)
        return 1;
    return strncmp(name, "UI_EggHatching", 15) == 0 ||
           strncmp(name, "UI_Evolution", 12) == 0 ||
           strncmp(name, "UI_Trading", 10) == 0 ||
           strncmp(name, "UI_HallOfFame", 13) == 0 ||
           strncmp(name, "UI_Credits", 10) == 0 ||
           strncmp(name, "Minigame_", 9) == 0 ||
           strcmp(name, "SafariZone") == 0 ||
           strcmp(name, "BugContest") == 0 ||
           strncmp(name, "Challenge_", 10) == 0 ||
           strcmp(name, "UI_BattleSwap") == 0 ||
           strncmp(name, "ChallengeGenerator_", 19) == 0;
}

int EvalScriptIndex(int index)
{
    unsigned char* source = 0;
    int sourceLength = 0;
    const int inflateResult = gHost.script_inflate(gHost.context,
                                                   index,
                                                   &source,
                                                   &sourceLength);
    if (inflateResult != ERgssVmOk || !source || sourceLength < 0)
        return ERgssVmErrorScript;
    const int evalResult = EvalProtected(reinterpret_cast<const char*>(source), index);
    gHost.release(gHost.context, source);
    return evalResult;
}

VALUE NLoadDeferredBattle(VALUE)
{
    if (gDeferredBattleLoaded)
        return Qtrue;
    if (gDeferredBattleLoading)
        return Qfalse;
    gDeferredBattleLoading = 1;
    for (int index = 0; index < gDeferredBattleScriptCount; ++index)
    {
        const int result = EvalScriptIndex(gDeferredBattleScripts[index]);
        if (result != ERgssVmOk)
        {
            gDeferredBattleLoading = 0;
            rb_raise(rb_eRuntimeError, "%s", gLastError[0] ? gLastError : "deferred battle load failed");
        }
    }
    gDeferredBattleLoaded = 1;
    gDeferredBattleLoading = 0;
    return Qtrue;
}

VALUE NLoadDeferredOptional(VALUE)
{
    if (gDeferredOptionalLoaded)
        return Qtrue;
    if (gDeferredOptionalLoading)
        return Qfalse;
    gDeferredOptionalLoading = 1;
    for (int index = 0; index < gDeferredOptionalScriptCount; ++index)
    {
        const int result = EvalScriptIndex(gDeferredOptionalScripts[index]);
        if (result != ERgssVmOk)
        {
            gDeferredOptionalLoading = 0;
            rb_raise(rb_eRuntimeError, "%s", gLastError[0] ? gLastError : "deferred optional load failed");
        }
    }
    gDeferredOptionalLoaded = 1;
    gDeferredOptionalLoading = 0;
    return Qtrue;
}
}

extern "C" int EasyRpgRgssVmLoadScripts(const char*)
{
    if (!gInitialized)
        return ERgssVmErrorNotInitialized;
    const int count = gHost.script_count(gHost.context);
    if (count <= 0)
        return ERgssVmErrorScript;

    for (int index = 0; index < count; ++index)
    {
        if (SkipModernReleaseScript(index))
            continue;
        if (DeferModernBattleScript(index))
        {
            if (gDeferredBattleScriptCount >= static_cast<int>(sizeof(gDeferredBattleScripts) / sizeof(gDeferredBattleScripts[0])))
                return ERgssVmErrorScript;
            gDeferredBattleScripts[gDeferredBattleScriptCount++] = index;
            continue;
        }
        if (DeferModernOptionalScript(index))
        {
            if (gDeferredOptionalScriptCount >= static_cast<int>(sizeof(gDeferredOptionalScripts) / sizeof(gDeferredOptionalScripts[0])))
                return ERgssVmErrorScript;
            gDeferredOptionalScripts[gDeferredOptionalScriptCount++] = index;
            continue;
        }
        const int evalResult = EvalScriptIndex(index);
        if (evalResult != ERgssVmOk)
            return evalResult;
    }
    if (gDeferredBattleScriptCount > 0)
    {
        const char* hook =
            "class Object\n"
            "  if private_method_defined?(:pbWildBattleCore) && !private_method_defined?(:__easyrpg_pbWildBattleCore)\n"
            "    alias_method :__easyrpg_pbWildBattleCore, :pbWildBattleCore\n"
            "    def pbWildBattleCore(*args); EasyRPGNative.load_deferred_battle; __easyrpg_pbWildBattleCore(*args); end\n"
            "    private :pbWildBattleCore, :__easyrpg_pbWildBattleCore\n"
            "  end\n"
            "  if private_method_defined?(:pbTrainerBattleCore) && !private_method_defined?(:__easyrpg_pbTrainerBattleCore)\n"
            "    alias_method :__easyrpg_pbTrainerBattleCore, :pbTrainerBattleCore\n"
            "    def pbTrainerBattleCore(*args); EasyRPGNative.load_deferred_battle; __easyrpg_pbTrainerBattleCore(*args); end\n"
            "    private :pbTrainerBattleCore, :__easyrpg_pbTrainerBattleCore\n"
            "  end\n"
            "end\n"
            "if defined?(PluginManager)\n"
            "  class << PluginManager\n"
            "    if method_defined?(:runPlugins) && !method_defined?(:__easyrpg_runPlugins)\n"
            "      alias_method :__easyrpg_runPlugins, :runPlugins\n"
            "      def runPlugins; result=__easyrpg_runPlugins; GC.start; result; end\n"
            "    end\n"
            "  end\n"
            "end\n"
            "end\n";
        const int hookResult = EvalProtected(hook, -2);
        if (hookResult != ERgssVmOk)
            return hookResult;
    }
    return ERgssVmOk;
}

extern "C" int EasyRpgRgssVmTick(void)
{
    return gInitialized ? ERgssVmOk : ERgssVmErrorNotInitialized;
}

extern "C" int EasyRpgRgssVmCollect(void)
{
    if (!gInitialized)
        return ERgssVmErrorNotInitialized;
    rb_gc_start();
    return ERgssVmOk;
}

extern "C" int EasyRpgRgssVmHeapBytes(void)
{
    return gInitialized ? 0 : ERgssVmErrorNotInitialized;
}

extern "C" int EasyRpgRgssVmLastError(char* buffer, int capacity, int* script_index)
{
    if (!buffer || capacity <= 0 || !script_index)
        return ERgssVmErrorInvalidArgument;
    *script_index = gLastErrorScript;
    const int length = static_cast<int>(strlen(gLastError));
    const int copyLength = length < capacity - 1 ? length : capacity - 1;
    if (copyLength > 0)
        memcpy(buffer, gLastError, copyLength);
    buffer[copyLength] = 0;
    return copyLength;
}

extern "C" void EasyRpgRgssVmShutdown(void)
{
    if (!gInitialized)
        return;
    ResetHost();
}
