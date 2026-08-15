#ifndef RGSS_TYPES_H
#define RGSS_TYPES_H

/*
 * RGSS value types with their exact Marshal _dump formats (little-endian,
 * verified against mkxp-z src/etc/table.cpp and src/etc/etc.cpp):
 *   Table: int32 dim, xsize, ysize, zsize, size + raw int16 elements (z-major)
 *   Color: 4x double (red, green, blue, alpha)
 *   Tone:  4x double (red, green, blue, gray)
 *   Rect:  4x int32 (x, y, width, height)
 * No Ruby headers here: host-unit-testable without a VM.
 */

#include <stddef.h>

namespace rgss {

class Rect
{
public:
    Rect() : x(0), y(0), width(0), height(0) {}
    Rect(int aX, int aY, int aWidth, int aHeight)
        : x(aX), y(aY), width(aWidth), height(aHeight) {}

    void Set(int aX, int aY, int aWidth, int aHeight)
    {
        x = aX; y = aY; width = aWidth; height = aHeight;
    }

    bool IsEmpty() const { return width <= 0 || height <= 0; }

    int x, y, width, height;
};

class Color
{
public:
    Color() : red(0), green(0), blue(0), alpha(0) {}
    Color(double aRed, double aGreen, double aBlue, double aAlpha)
        : red(aRed), green(aGreen), blue(aBlue), alpha(aAlpha) {}

    void Set(double aRed, double aGreen, double aBlue, double aAlpha)
    {
        red = aRed; green = aGreen; blue = aBlue; alpha = aAlpha;
    }

    double red, green, blue, alpha;
};

class Tone
{
public:
    Tone() : red(0), green(0), blue(0), gray(0) {}
    Tone(double aRed, double aGreen, double aBlue, double aGray)
        : red(aRed), green(aGreen), blue(aBlue), gray(aGray) {}

    void Set(double aRed, double aGreen, double aBlue, double aGray)
    {
        red = aRed; green = aGreen; blue = aBlue; gray = aGray;
    }

    double red, green, blue, gray;
};

class Table
{
public:
    Table();
    Table(int aXsize, int aYsize, int aZsize);
    ~Table();

    /* Resizing preserves the overlapping region (RPG Maker semantics). */
    void Resize(int aXsize, int aYsize, int aZsize);

    int Xsize() const { return xsize; }
    int Ysize() const { return ysize; }
    int Zsize() const { return zsize; }

    bool Get(int aX, int aY, int aZ, short* aOut) const;
    void Set(int aX, int aY, int aZ, short aValue); /* out of range is ignored */

    /* Serialization size: 20 + 2*x*y*z. */
    size_t SerializedSize() const;
    /* aBuffer must have SerializedSize() bytes. */
    void Serialize(unsigned char* aBuffer) const;
    /* Returns false (and leaves the table empty) on malformed input. */
    bool Deserialize(const unsigned char* aData, size_t aSize);

private:
    void Allocate();
    static size_t Index(int aX, int aY, int aZ, int aXsize, int aYsize)
    {
        return static_cast<size_t>(aX) +
               static_cast<size_t>(aY) * aXsize +
               static_cast<size_t>(aZ) * aXsize * aYsize;
    }

    int xsize, ysize, zsize;
    short* data; /* owned, malloc'd */
};

/* Little-endian helpers shared by the serialize/deserialize code. */
void WriteLe32(unsigned char* aBuffer, int aValue);
int ReadLe32(const unsigned char* aBuffer);
void WriteLeDouble(unsigned char* aBuffer, double aValue);
double ReadLeDouble(const unsigned char* aBuffer);

/* Rect/Color/Tone (de)serialization; fixed sizes 16/32/32 bytes. */
void SerializeRect(unsigned char* aBuffer, const Rect& aRect);
bool DeserializeRect(const unsigned char* aData, size_t aSize, Rect* aOut);
void SerializeColor(unsigned char* aBuffer, const Color& aColor);
bool DeserializeColor(const unsigned char* aData, size_t aSize, Color* aOut);
void SerializeTone(unsigned char* aBuffer, const Tone& aTone);
bool DeserializeTone(const unsigned char* aData, size_t aSize, Tone* aOut);

} // namespace rgss

#endif
