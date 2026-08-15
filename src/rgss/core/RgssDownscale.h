#ifndef RGSS_DOWNSCALE_H
#define RGSS_DOWNSCALE_H

/*
 * RGB565 downscale for the 320x240 device screen. Exact 2:1 box average
 * when the source is exactly twice the target; otherwise nearest-neighbor
 * with Bresenham source stepping. Channels are averaged in expanded
 * 8-bit space (5/6/5 bits bit-replicated).
 */

namespace rgss {

void RgssDownscaleRgb565(const unsigned short* aSrc, int aSrcWidth, int aSrcHeight,
                         unsigned short* aDst, int aDstWidth, int aDstHeight);

} // namespace rgss

#endif
