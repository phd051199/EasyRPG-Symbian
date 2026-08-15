#include "RgssTypes.h"

#include <cstdlib>
#include <cstring>

/* All target hosts (x86_64/arm64 macOS, ARMv5 E72) are little-endian with
 * IEEE754 doubles, so memcpy of raw bytes is the wire format. */
namespace rgss {

void WriteLe32(unsigned char* aBuffer, int aValue)
{
    aBuffer[0] = static_cast<unsigned char>(aValue);
    aBuffer[1] = static_cast<unsigned char>(aValue >> 8);
    aBuffer[2] = static_cast<unsigned char>(aValue >> 16);
    aBuffer[3] = static_cast<unsigned char>(aValue >> 24);
}

int ReadLe32(const unsigned char* aBuffer)
{
    return static_cast<int>(aBuffer[0]) |
           (static_cast<int>(aBuffer[1]) << 8) |
           (static_cast<int>(aBuffer[2]) << 16) |
           (static_cast<int>(aBuffer[3]) << 24);
}

void WriteLeDouble(unsigned char* aBuffer, double aValue)
{
    memcpy(aBuffer, &aValue, 8);
}

double ReadLeDouble(const unsigned char* aBuffer)
{
    double value = 0.0;
    memcpy(&value, aBuffer, 8);
    return value;
}

Table::Table() : xsize(0), ysize(0), zsize(0), data(0) {}

Table::Table(int aXsize, int aYsize, int aZsize)
    : xsize(0), ysize(0), zsize(0), data(0)
{
    Resize(aXsize, aYsize, aZsize);
}

Table::~Table()
{
    free(data);
}

void Table::Allocate()
{
    free(data);
    data = 0;
    if (xsize <= 0 || ysize <= 0 || zsize <= 0)
        return;
    const size_t count =
        static_cast<size_t>(xsize) * ysize * zsize;
    data = static_cast<short*>(malloc(count * sizeof(short)));
    if (data)
        memset(data, 0, count * sizeof(short));
}

void Table::Resize(int aXsize, int aYsize, int aZsize)
{
    if (aXsize < 0) aXsize = 0;
    if (aYsize < 0) aYsize = 0;
    if (aZsize < 0) aZsize = 0;

    short* old = data;
    const int oldX = xsize, oldY = ysize, oldZ = zsize;
    data = 0;
    xsize = aXsize; ysize = aYsize; zsize = aZsize;
    Allocate();

    if (old && data)
    {
        const int copyX = xsize < oldX ? xsize : oldX;
        const int copyY = ysize < oldY ? ysize : oldY;
        const int copyZ = zsize < oldZ ? zsize : oldZ;
        for (int z = 0; z < copyZ; ++z)
            for (int y = 0; y < copyY; ++y)
                memcpy(data + Index(0, y, z, xsize, ysize),
                       old + Index(0, y, z, oldX, oldY),
                       copyX * sizeof(short));
    }
    free(old);
}

bool Table::Get(int aX, int aY, int aZ, short* aOut) const
{
    if (!data || aX < 0 || aY < 0 || aZ < 0 ||
        aX >= xsize || aY >= ysize || aZ >= zsize)
        return false;
    *aOut = data[Index(aX, aY, aZ, xsize, ysize)];
    return true;
}

void Table::Set(int aX, int aY, int aZ, short aValue)
{
    if (!data || aX < 0 || aY < 0 || aZ < 0 ||
        aX >= xsize || aY >= ysize || aZ >= zsize)
        return;
    data[Index(aX, aY, aZ, xsize, ysize)] = aValue;
}

size_t Table::SerializedSize() const
{
    const size_t count =
        static_cast<size_t>(xsize) * ysize * zsize;
    return 20 + count * 2;
}

void Table::Serialize(unsigned char* aBuffer) const
{
    const int dim = zsize > 1 ? 3 : (ysize > 1 ? 2 : 1);
    const int count = xsize * ysize * zsize;
    WriteLe32(aBuffer + 0, dim);
    WriteLe32(aBuffer + 4, xsize);
    WriteLe32(aBuffer + 8, ysize);
    WriteLe32(aBuffer + 12, zsize);
    WriteLe32(aBuffer + 16, count);
    if (data && count > 0)
    {
        /* Host is little-endian; short is 16 bits on all targets. */
        memcpy(aBuffer + 20, data, static_cast<size_t>(count) * 2);
    }
}

bool Table::Deserialize(const unsigned char* aData, size_t aSize)
{
    if (aSize < 20)
        return false;

    const int inX = ReadLe32(aData + 4);
    const int inY = ReadLe32(aData + 8);
    const int inZ = ReadLe32(aData + 12);
    const int count = ReadLe32(aData + 16);

    if (inX < 0 || inY < 0 || inZ < 0)
        return false;
    if (count != inX * inY * inZ)
        return false;
    if (aSize != 20 + static_cast<size_t>(count) * 2)
        return false;

    xsize = inX; ysize = inY; zsize = inZ;
    Allocate();
    if (!data && count > 0)
        return false; /* out of memory */
    if (data && count > 0)
        memcpy(data, aData + 20, static_cast<size_t>(count) * 2);
    return true;
}

void SerializeRect(unsigned char* aBuffer, const Rect& aRect)
{
    WriteLe32(aBuffer + 0, aRect.x);
    WriteLe32(aBuffer + 4, aRect.y);
    WriteLe32(aBuffer + 8, aRect.width);
    WriteLe32(aBuffer + 12, aRect.height);
}

bool DeserializeRect(const unsigned char* aData, size_t aSize, Rect* aOut)
{
    if (aSize != 16)
        return false;
    aOut->x = ReadLe32(aData + 0);
    aOut->y = ReadLe32(aData + 4);
    aOut->width = ReadLe32(aData + 8);
    aOut->height = ReadLe32(aData + 12);
    return true;
}

void SerializeColor(unsigned char* aBuffer, const Color& aColor)
{
    WriteLeDouble(aBuffer + 0, aColor.red);
    WriteLeDouble(aBuffer + 8, aColor.green);
    WriteLeDouble(aBuffer + 16, aColor.blue);
    WriteLeDouble(aBuffer + 24, aColor.alpha);
}

bool DeserializeColor(const unsigned char* aData, size_t aSize, Color* aOut)
{
    if (aSize != 32)
        return false;
    aOut->red = ReadLeDouble(aData + 0);
    aOut->green = ReadLeDouble(aData + 8);
    aOut->blue = ReadLeDouble(aData + 16);
    aOut->alpha = ReadLeDouble(aData + 24);
    return true;
}

void SerializeTone(unsigned char* aBuffer, const Tone& aTone)
{
    WriteLeDouble(aBuffer + 0, aTone.red);
    WriteLeDouble(aBuffer + 8, aTone.green);
    WriteLeDouble(aBuffer + 16, aTone.blue);
    WriteLeDouble(aBuffer + 24, aTone.gray);
}

bool DeserializeTone(const unsigned char* aData, size_t aSize, Tone* aOut)
{
    if (aSize != 32)
        return false;
    aOut->red = ReadLeDouble(aData + 0);
    aOut->green = ReadLeDouble(aData + 8);
    aOut->blue = ReadLeDouble(aData + 16);
    aOut->gray = ReadLeDouble(aData + 24);
    return true;
}

} // namespace rgss
