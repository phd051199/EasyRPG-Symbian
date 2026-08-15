#include "rgss/core/RgssDownscale.h"

namespace rgss {

namespace {

inline void Expand565(unsigned short aPixel, int* aRed, int* aGreen, int* aBlue)
{
    int r = (aPixel >> 11) & 31;
    int g = (aPixel >> 5) & 63;
    int b = aPixel & 31;
    *aRed = (r << 3) | (r >> 2);
    *aGreen = (g << 2) | (g >> 4);
    *aBlue = (b << 3) | (b >> 2);
}

inline unsigned short Pack565(int aRed, int aGreen, int aBlue)
{
    if (aRed < 0) aRed = 0;
    if (aRed > 255) aRed = 255;
    if (aGreen < 0) aGreen = 0;
    if (aGreen > 255) aGreen = 255;
    if (aBlue < 0) aBlue = 0;
    if (aBlue > 255) aBlue = 255;
    return static_cast<unsigned short>(((aRed >> 3) << 11) |
                                       ((aGreen >> 2) << 5) |
                                       (aBlue >> 3));
}

} // namespace

void RgssDownscaleRgb565(const unsigned short* aSrc, int aSrcWidth, int aSrcHeight,
                         unsigned short* aDst, int aDstWidth, int aDstHeight)
{
    if (!aSrc || !aDst || aSrcWidth <= 0 || aSrcHeight <= 0 ||
        aDstWidth <= 0 || aDstHeight <= 0)
        return;

    if (aSrcWidth == 2 * aDstWidth && aSrcHeight == 2 * aDstHeight) {
        /* Exact 2:1 box average: (a + b + c + d + 2) / 4 per channel. */
        for (int y = 0; y < aDstHeight; ++y) {
            const unsigned short* row0 = aSrc + (2 * y) * aSrcWidth;
            const unsigned short* row1 = row0 + aSrcWidth;
            unsigned short* dstRow = aDst + y * aDstWidth;
            for (int x = 0; x < aDstWidth; ++x) {
                int r[4], g[4], b[4];
                Expand565(row0[2 * x], &r[0], &g[0], &b[0]);
                Expand565(row0[2 * x + 1], &r[1], &g[1], &b[1]);
                Expand565(row1[2 * x], &r[2], &g[2], &b[2]);
                Expand565(row1[2 * x + 1], &r[3], &g[3], &b[3]);
                dstRow[x] = Pack565((r[0] + r[1] + r[2] + r[3] + 2) / 4,
                                    (g[0] + g[1] + g[2] + g[3] + 2) / 4,
                                    (b[0] + b[1] + b[2] + b[3] + 2) / 4);
            }
        }
        return;
    }

    /* Nearest-neighbor with Bresenham source stepping. */
    int sy = 0;
    int errY = 0;
    for (int dy = 0; dy < aDstHeight; ++dy) {
        int sx = 0;
        int errX = 0;
        unsigned short* dstRow = aDst + dy * aDstWidth;
        for (int dx = 0; dx < aDstWidth; ++dx) {
            dstRow[dx] = aSrc[sy * aSrcWidth + sx];
            errX += aSrcWidth;
            while (errX >= aDstWidth) {
                errX -= aDstWidth;
                ++sx;
            }
        }
        errY += aSrcHeight;
        while (errY >= aDstHeight) {
            errY -= aDstHeight;
            ++sy;
        }
    }
}

} // namespace rgss
