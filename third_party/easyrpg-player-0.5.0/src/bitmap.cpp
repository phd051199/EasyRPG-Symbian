/*
 * This file is part of EasyRPG Player.
 *
 * EasyRPG Player is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EasyRPG Player is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EasyRPG Player. If not, see <http://www.gnu.org/licenses/>.
 */

// Headers
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <iostream>

#include "utils.h"
#include "cache.h"
#include "bitmap.h"
#include "filefinder.h"
#include "options.h"
#include "data.h"
#include "output.h"
#include "image_xyz.h"
#include "image_bmp.h"
#include "image_png.h"
#include "font.h"
#include "output.h"
#include "util_macro.h"
#include "bitmap_hslrgb.h"
#ifdef EASYRPG_SYMBIAN
#include "baseui.h"
#include "RenderDamage.h"
#endif

#ifdef EASYRPG_SYMBIAN
namespace {

uint32_t gSymbianBitmapSeed = 1;

enum TSymbianDrawOp {
	ESymbianBlit = 1,
	ESymbianBlitFast,
	ESymbianTiledBlit,
	ESymbianStretchBlit,
	ESymbianTransformBlit,
	ESymbianWaverBlit,
	ESymbianFill,
	ESymbianFillRect,
	ESymbianClear,
	ESymbianClearRect,
	ESymbianToneBlit,
	ESymbianBlendBlit,
	ESymbianFlipBlit,
	ESymbianMaskedColor,
	ESymbianMaskedBitmap,
	ESymbianBlit2x
};

unsigned int SymbianHash(unsigned int aHash, unsigned int aValue) {
	return aHash ? SymbianRenderDamage::HashValue(aHash, aValue) : 0;
}

unsigned int SymbianHashInt(unsigned int aHash, int aValue) {
	return aHash ? SymbianHash(aHash, static_cast<unsigned int>(aValue)) : 0;
}

unsigned int SymbianHashRect(unsigned int aHash, const Rect& aRect) {
	if (!aHash)
		return 0;
	aHash = SymbianHashInt(aHash, aRect.x);
	aHash = SymbianHashInt(aHash, aRect.y);
	aHash = SymbianHashInt(aHash, aRect.width);
	return SymbianHashInt(aHash, aRect.height);
}

unsigned int SymbianHashDouble(unsigned int aHash, double aValue) {
	if (!aHash)
		return 0;
	unsigned long long bits = 0;
	std::memcpy(&bits, &aValue, sizeof(bits));
	aHash = SymbianHash(aHash, static_cast<unsigned int>(bits));
	return SymbianHash(aHash, static_cast<unsigned int>(bits >> 32));
}

unsigned int SymbianHashBitmap(unsigned int aHash, const Bitmap& aBitmap) {
	if (!aHash)
		return 0;
	aHash = SymbianHash(aHash, static_cast<unsigned int>(reinterpret_cast<size_t>(&aBitmap)));
	return SymbianHash(aHash, aBitmap.Revision());
}

unsigned int SymbianHashOpacity(unsigned int aHash, const Opacity& aOpacity) {
	if (!aHash)
		return 0;
	aHash = SymbianHashInt(aHash, aOpacity.top);
	aHash = SymbianHashInt(aHash, aOpacity.bottom);
	return SymbianHashInt(aHash, aOpacity.split);
}

unsigned int SymbianHashColor(unsigned int aHash, const Color& aColor) {
	if (!aHash)
		return 0;
	aHash = SymbianHashInt(aHash, aColor.red);
	aHash = SymbianHashInt(aHash, aColor.green);
	aHash = SymbianHashInt(aHash, aColor.blue);
	return SymbianHashInt(aHash, aColor.alpha);
}

unsigned int SymbianHashTone(unsigned int aHash, const Tone& aTone) {
	if (!aHash)
		return 0;
	aHash = SymbianHashInt(aHash, aTone.red);
	aHash = SymbianHashInt(aHash, aTone.green);
	aHash = SymbianHashInt(aHash, aTone.blue);
	return SymbianHashInt(aHash, aTone.gray);
}

bool SymbianIntersectsRect(const Rect& aFirst, const Rect& aSecond) {
	return aFirst.x < aSecond.x + aSecond.width && aSecond.x < aFirst.x + aFirst.width &&
		aFirst.y < aSecond.y + aSecond.height && aSecond.y < aFirst.y + aFirst.height;
}

bool SymbianRecordDraw(Bitmap* aDestination, unsigned int aSignature, const Rect& aRect) {
	const bool recording = SymbianRenderDamage::IsRecording();
	const Rect* paint_clip = SymbianRenderDamage::PaintClipRect();
	if (!recording && !paint_clip)
		return false;
	if (!DisplayUi || !DisplayUi->GetDisplaySurface() || DisplayUi->GetDisplaySurface().get() != aDestination)
		return false;
	if (recording) {
		SymbianRenderDamage::Record(aSignature, aRect);
		return true;
	}
	if (!SymbianIntersectsRect(aRect, *paint_clip))
		return true;
	return false;
}

unsigned int SymbianDrawSignature(int aOperation) {
	return SymbianRenderDamage::IsRecording()
		? SymbianRenderDamage::HashSeed(static_cast<unsigned int>(aOperation))
		: 0;
}

uint16_t SymbianColor565(const Color& aColor) {
	return static_cast<uint16_t>(
		((static_cast<unsigned int>(aColor.red) >> 3) << 11) |
		((static_cast<unsigned int>(aColor.green) >> 2) << 5) |
		(static_cast<unsigned int>(aColor.blue) >> 3));
}

bool SymbianUseA1555(const void* aPixels, int aWidth, int aHeight, uint32_t aFlags) {
	if (!aPixels || aWidth <= 0 || aHeight <= 0 || (aFlags & Bitmap::Flag_System))
		return false;
	const uint8_t* pixels = static_cast<const uint8_t*>(aPixels);
	const size_t count = static_cast<size_t>(aWidth) * static_cast<size_t>(aHeight);
	for (size_t index = 0; index < count; ++index) {
		const uint8_t alpha = pixels[index * 4 + 3];
		if (alpha != 0 && alpha != 255)
			return false;
	}
	return true;
}

DynamicFormat SymbianA1555Format() {
	return DynamicFormat(16, 5, 10, 5, 5, 5, 0, 1, 15, PF::Alpha);
}

void SymbianFill565Rect(Bitmap& aBitmap, const Rect& aRect, uint16_t aColor) {
	if (aRect.IsEmpty())
		return;
	uint8_t* base = static_cast<uint8_t*>(aBitmap.pixels());
	const int pitch = aBitmap.pitch();
	if (aColor == 0) {
		const size_t row_bytes = static_cast<size_t>(aRect.width) * 2u;
		if (aRect.x == 0 && row_bytes == static_cast<size_t>(pitch)) {
			std::memset(base + aRect.y * pitch, 0, row_bytes * static_cast<size_t>(aRect.height));
			return;
		}
		for (int y = aRect.y; y < aRect.y + aRect.height; ++y)
			std::memset(base + y * pitch + aRect.x * 2, 0, row_bytes);
		return;
	}
	for (int y = aRect.y; y < aRect.y + aRect.height; ++y) {
		uint16_t* row = reinterpret_cast<uint16_t*>(base + y * pitch) + aRect.x;
		for (int x = 0; x < aRect.width; ++x)
			row[x] = aColor;
	}
}

void SymbianFill565(Bitmap& aBitmap, const Rect& aInput, uint16_t aColor) {
	Rect rect = aInput;
	rect.Adjust(aBitmap.GetRect());
	if (rect.IsEmpty())
		return;
	const Rect* paint_clip = SymbianRenderDamage::PaintClipRect();
	if (paint_clip)
		rect.Adjust(*paint_clip);
	SymbianFill565Rect(aBitmap, rect, aColor);
}

unsigned int SymbianMul255(unsigned int aValue, unsigned int aAlpha) {
	const unsigned int value = aValue * aAlpha + 128u;
	return (value + (value >> 8)) >> 8;
}

uint16_t SymbianPack565(unsigned int aRed, unsigned int aGreen, unsigned int aBlue) {
	return static_cast<uint16_t>(((aRed >> 3) << 11) | ((aGreen >> 2) << 5) | (aBlue >> 3));
}

uint16_t SymbianA1555To565(uint16_t aPixel) {
	return static_cast<uint16_t>(((aPixel & 0x7fe0u) << 1) |
		((aPixel & 0x0200u) >> 4) |
		(aPixel & 0x001fu));
}

uint32_t SymbianA1555PairTo565(uint32_t aPixels) {
	return ((aPixels & 0x7fe07fe0u) << 1) |
		((aPixels & 0x02000200u) >> 4) |
		(aPixels & 0x001f001fu);
}

void SymbianCopyTileRow565(uint8_t* aDestination, const uint8_t* aSource) {
	if (((reinterpret_cast<size_t>(aDestination) | reinterpret_cast<size_t>(aSource)) & 3u) == 0) {
		uint32_t* destination = reinterpret_cast<uint32_t*>(aDestination);
		const uint32_t* source = reinterpret_cast<const uint32_t*>(aSource);
		for (int index = 0; index < TILE_SIZE / 2; ++index)
			destination[index] = source[index];
		return;
	}
	uint16_t* destination = reinterpret_cast<uint16_t*>(aDestination);
	const uint16_t* source = reinterpret_cast<const uint16_t*>(aSource);
	for (int index = 0; index < TILE_SIZE; ++index)
		destination[index] = source[index];
}

void SymbianExpand565(uint16_t aPixel, unsigned int& aRed, unsigned int& aGreen, unsigned int& aBlue) {
	aRed = (aPixel >> 11) & 31u;
	aGreen = (aPixel >> 5) & 63u;
	aBlue = aPixel & 31u;
	aRed = (aRed << 3) | (aRed >> 2);
	aGreen = (aGreen << 2) | (aGreen >> 4);
	aBlue = (aBlue << 3) | (aBlue >> 2);
}

bool SymbianBlitTo565(void* aDestinationPixels,
	int aDestinationPitch,
	int aDestinationWidth,
	int aDestinationHeight,
	const void* aSourcePixels,
	int aSourcePitch,
	int aSourceWidth,
	int aSourceHeight,
	pixman_format_code_t aSourceFormat,
	int aX,
	int aY,
	const Rect& aSourceRect,
	int aOpacity,
	Rect& aWrittenRect) {
	const bool source565 = aSourceFormat == PIXMAN_r5g6b5;
	const bool source1555 = aSourceFormat == PIXMAN_a1r5g5b5;
	const bool sourceArgb = aSourceFormat == PIXMAN_a8r8g8b8 || aSourceFormat == PIXMAN_x8r8g8b8;
	const bool sourceAbgr = aSourceFormat == PIXMAN_a8b8g8r8 || aSourceFormat == PIXMAN_x8b8g8r8;
	if (!source565 && !source1555 && !sourceArgb && !sourceAbgr)
		return false;

	int sx = aSourceRect.x;
	int sy = aSourceRect.y;
	int dx = aX;
	int dy = aY;
	int width = aSourceRect.width;
	int height = aSourceRect.height;
	if (sx < 0) { dx -= sx; width += sx; sx = 0; }
	if (sy < 0) { dy -= sy; height += sy; sy = 0; }
	if (dx < 0) { sx -= dx; width += dx; dx = 0; }
	if (dy < 0) { sy -= dy; height += dy; dy = 0; }
	width = std::min(width, aSourceWidth - sx);
	height = std::min(height, aSourceHeight - sy);
	width = std::min(width, aDestinationWidth - dx);
	height = std::min(height, aDestinationHeight - dy);
	const Rect* paint_clip = SymbianRenderDamage::PaintClipRect();
	if (paint_clip) {
		const Rect& dirty = *paint_clip;
		const int left = std::max(dx, dirty.x);
		const int top = std::max(dy, dirty.y);
		const int right = std::min(dx + width, dirty.x + dirty.width);
		const int bottom = std::min(dy + height, dirty.y + dirty.height);
		sx += left - dx;
		sy += top - dy;
		dx = left;
		dy = top;
		width = right - left;
		height = bottom - top;
	}
	if (width <= 0 || height <= 0 || aOpacity <= 0) {
		aWrittenRect = Rect();
		return true;
	}

	const unsigned int opacity = static_cast<unsigned int>(std::min(255, aOpacity));
	uint8_t* destinationBase = static_cast<uint8_t*>(aDestinationPixels);
	const uint8_t* sourceBase = static_cast<const uint8_t*>(aSourcePixels);
	if (source565 && opacity == 255u) {
		const size_t rowBytes = static_cast<size_t>(width) * 2u;
		if (dx == 0 && sx == 0 && rowBytes == static_cast<size_t>(aDestinationPitch) &&
			rowBytes == static_cast<size_t>(aSourcePitch)) {
			std::memcpy(destinationBase + dy * aDestinationPitch,
				sourceBase + sy * aSourcePitch,
				rowBytes * static_cast<size_t>(height));
		} else {
			for (int row = 0; row < height; ++row) {
				std::memcpy(destinationBase + (dy + row) * aDestinationPitch + dx * 2,
					sourceBase + (sy + row) * aSourcePitch + sx * 2,
					rowBytes);
			}
		}
		aWrittenRect.Set(dx, dy, width, height);
		return true;
	}
	if (source1555 && opacity == 255u) {
		for (int row = 0; row < height; ++row) {
			uint16_t* destination = reinterpret_cast<uint16_t*>(
				destinationBase + (dy + row) * aDestinationPitch) + dx;
			const uint16_t* source = reinterpret_cast<const uint16_t*>(
				sourceBase + (sy + row) * aSourcePitch) + sx;
			int column = 0;
			if ((reinterpret_cast<size_t>(destination) & 3u) == 0) {
				uint32_t* destination32 = reinterpret_cast<uint32_t*>(destination);
				const bool sourceAligned = (reinterpret_cast<size_t>(source) & 3u) == 0;
				const uint32_t* source32 = sourceAligned ? reinterpret_cast<const uint32_t*>(source) : NULL;
				const int pairs = width >> 1;
				for (int pair = 0; pair < pairs; ++pair) {
					const int pixel = pair << 1;
					const uint32_t pixels = sourceAligned
						? source32[pair]
						: static_cast<uint32_t>(source[pixel]) |
							(static_cast<uint32_t>(source[pixel + 1]) << 16);
					const uint32_t alpha = pixels & 0x80008000u;
					if (alpha == 0)
						continue;
					if (alpha == 0x80008000u) {
						destination32[pair] = SymbianA1555PairTo565(pixels);
						continue;
					}
					if ((pixels & 0x00008000u) != 0)
						destination[pixel] = SymbianA1555To565(static_cast<uint16_t>(pixels));
					if ((pixels & 0x80000000u) != 0)
						destination[pixel + 1] = SymbianA1555To565(static_cast<uint16_t>(pixels >> 16));
				}
				column = pairs << 1;
			}
			for (; column < width; ++column) {
				const uint16_t pixel = source[column];
				if ((pixel & 0x8000u) == 0)
					continue;
				destination[column] = SymbianA1555To565(pixel);
			}
		}
		aWrittenRect.Set(dx, dy, width, height);
		return true;
	}

	for (int row = 0; row < height; ++row) {
		uint16_t* destination = reinterpret_cast<uint16_t*>(destinationBase + (dy + row) * aDestinationPitch) + dx;
		if (source565 || source1555) {
			const uint16_t* source = reinterpret_cast<const uint16_t*>(sourceBase + (sy + row) * aSourcePitch) + sx;
			for (int column = 0; column < width; ++column) {
				const uint16_t sourcePixel = source[column];
				if (source1555 && (sourcePixel & 0x8000u) == 0)
					continue;
				unsigned int sourceRed;
				unsigned int sourceGreen;
				unsigned int sourceBlue;
				if (source565) {
					SymbianExpand565(sourcePixel, sourceRed, sourceGreen, sourceBlue);
				} else {
					const unsigned int red5 = (sourcePixel >> 10) & 31u;
					const unsigned int green5 = (sourcePixel >> 5) & 31u;
					const unsigned int blue5 = sourcePixel & 31u;
					sourceRed = (red5 << 3) | (red5 >> 2);
					sourceGreen = (green5 << 3) | (green5 >> 2);
					sourceBlue = (blue5 << 3) | (blue5 >> 2);
				}
				if (opacity == 255u) {
					destination[column] = SymbianPack565(sourceRed, sourceGreen, sourceBlue);
					continue;
				}
				unsigned int destinationRed;
				unsigned int destinationGreen;
				unsigned int destinationBlue;
				SymbianExpand565(destination[column], destinationRed, destinationGreen, destinationBlue);
				const unsigned int inverse = 255u - opacity;
				const unsigned int red = SymbianMul255(sourceRed, opacity) + SymbianMul255(destinationRed, inverse);
				const unsigned int green = SymbianMul255(sourceGreen, opacity) + SymbianMul255(destinationGreen, inverse);
				const unsigned int blue = SymbianMul255(sourceBlue, opacity) + SymbianMul255(destinationBlue, inverse);
				destination[column] = SymbianPack565(red, green, blue);
			}
		} else {
			const uint32_t* source = reinterpret_cast<const uint32_t*>(sourceBase + (sy + row) * aSourcePitch) + sx;
			const bool sourceOpaque = aSourceFormat == PIXMAN_x8r8g8b8 || aSourceFormat == PIXMAN_x8b8g8r8;
			for (int column = 0; column < width; ++column) {
				const uint32_t sourcePixel = source[column];
				unsigned int sourceAlpha = sourceOpaque ? 255u : ((sourcePixel >> 24) & 0xffu);
				unsigned int sourceRed = sourceArgb ? ((sourcePixel >> 16) & 0xffu) : (sourcePixel & 0xffu);
				unsigned int sourceGreen = (sourcePixel >> 8) & 0xffu;
				unsigned int sourceBlue = sourceArgb ? (sourcePixel & 0xffu) : ((sourcePixel >> 16) & 0xffu);
				if (opacity != 255u) {
					sourceAlpha = SymbianMul255(sourceAlpha, opacity);
					sourceRed = SymbianMul255(sourceRed, opacity);
					sourceGreen = SymbianMul255(sourceGreen, opacity);
					sourceBlue = SymbianMul255(sourceBlue, opacity);
				}
				if (sourceAlpha == 0)
					continue;
				if (sourceAlpha == 255u) {
					destination[column] = SymbianPack565(sourceRed, sourceGreen, sourceBlue);
					continue;
				}
				unsigned int destinationRed;
				unsigned int destinationGreen;
				unsigned int destinationBlue;
				SymbianExpand565(destination[column], destinationRed, destinationGreen, destinationBlue);
				const unsigned int inverse = 255u - sourceAlpha;
				const unsigned int red = sourceRed + SymbianMul255(destinationRed, inverse);
				const unsigned int green = sourceGreen + SymbianMul255(destinationGreen, inverse);
				const unsigned int blue = sourceBlue + SymbianMul255(destinationBlue, inverse);
				destination[column] = SymbianPack565(std::min(255u, red), std::min(255u, green), std::min(255u, blue));
			}
		}
	}

	aWrittenRect.Set(dx, dy, width, height);
	return true;
}

bool SymbianReadSourcePixel(const uint8_t* aBase,
	int aPitch,
	pixman_format_code_t aFormat,
	int aX,
	int aY,
	unsigned int& aAlpha,
	unsigned int& aRed,
	unsigned int& aGreen,
	unsigned int& aBlue) {
	if (aFormat == PIXMAN_r5g6b5) {
		const uint16_t pixel = reinterpret_cast<const uint16_t*>(aBase + aY * aPitch)[aX];
		aAlpha = 255u;
		SymbianExpand565(pixel, aRed, aGreen, aBlue);
		return true;
	}
	if (aFormat == PIXMAN_a1r5g5b5) {
		const uint16_t pixel = reinterpret_cast<const uint16_t*>(aBase + aY * aPitch)[aX];
		if ((pixel & 0x8000u) == 0) {
			aAlpha = aRed = aGreen = aBlue = 0;
			return true;
		}
		const unsigned int red5 = (pixel >> 10) & 31u;
		const unsigned int green5 = (pixel >> 5) & 31u;
		const unsigned int blue5 = pixel & 31u;
		aAlpha = 255u;
		aRed = (red5 << 3) | (red5 >> 2);
		aGreen = (green5 << 3) | (green5 >> 2);
		aBlue = (blue5 << 3) | (blue5 >> 2);
		return true;
	}
	const bool argb = aFormat == PIXMAN_a8r8g8b8 || aFormat == PIXMAN_x8r8g8b8;
	const bool abgr = aFormat == PIXMAN_a8b8g8r8 || aFormat == PIXMAN_x8b8g8r8;
	if (!argb && !abgr)
		return false;
	const uint32_t pixel = reinterpret_cast<const uint32_t*>(aBase + aY * aPitch)[aX];
	const bool opaque = aFormat == PIXMAN_x8r8g8b8 || aFormat == PIXMAN_x8b8g8r8;
	aAlpha = opaque ? 255u : ((pixel >> 24) & 0xffu);
	aRed = argb ? ((pixel >> 16) & 0xffu) : (pixel & 0xffu);
	aGreen = (pixel >> 8) & 0xffu;
	aBlue = argb ? (pixel & 0xffu) : ((pixel >> 16) & 0xffu);
	return true;
}

void SymbianBlendSource565(uint16_t& aDestination,
	unsigned int aAlpha,
	unsigned int aRed,
	unsigned int aGreen,
	unsigned int aBlue,
	unsigned int aOpacity) {
	if (aOpacity != 255u) {
		aAlpha = SymbianMul255(aAlpha, aOpacity);
		aRed = SymbianMul255(aRed, aOpacity);
		aGreen = SymbianMul255(aGreen, aOpacity);
		aBlue = SymbianMul255(aBlue, aOpacity);
	}
	if (aAlpha == 0)
		return;
	if (aAlpha == 255u) {
		aDestination = SymbianPack565(aRed, aGreen, aBlue);
		return;
	}
	unsigned int destinationRed;
	unsigned int destinationGreen;
	unsigned int destinationBlue;
	SymbianExpand565(aDestination, destinationRed, destinationGreen, destinationBlue);
	const unsigned int inverse = 255u - aAlpha;
	const unsigned int red = aRed + SymbianMul255(destinationRed, inverse);
	const unsigned int green = aGreen + SymbianMul255(destinationGreen, inverse);
	const unsigned int blue = aBlue + SymbianMul255(destinationBlue, inverse);
	aDestination = SymbianPack565(std::min(255u, red), std::min(255u, green), std::min(255u, blue));
}

bool SymbianScaleTo565(void* aDestinationPixels,
	int aDestinationPitch,
	int aDestinationWidth,
	int aDestinationHeight,
	const void* aSourcePixels,
	int aSourcePitch,
	int aSourceWidth,
	int aSourceHeight,
	pixman_format_code_t aSourceFormat,
	const Rect& aDestinationRect,
	const Rect& aSourceRect,
	int aOpacity,
	bool aFlipX,
	bool aFlipY,
	Rect& aWrittenRect) {
	if (!aDestinationPixels || !aSourcePixels || aOpacity <= 0 ||
		aDestinationRect.width <= 0 || aDestinationRect.height <= 0 ||
		aSourceRect.width <= 0 || aSourceRect.height <= 0 ||
		aSourceRect.x < 0 || aSourceRect.y < 0 ||
		aSourceRect.x + aSourceRect.width > aSourceWidth ||
		aSourceRect.y + aSourceRect.height > aSourceHeight)
		return false;

	unsigned int probeAlpha;
	unsigned int probeRed;
	unsigned int probeGreen;
	unsigned int probeBlue;
	if (!SymbianReadSourcePixel(static_cast<const uint8_t*>(aSourcePixels), aSourcePitch,
		aSourceFormat, aSourceRect.x, aSourceRect.y,
		probeAlpha, probeRed, probeGreen, probeBlue))
		return false;

	Rect destination = aDestinationRect;
	destination.Adjust(Rect(0, 0, aDestinationWidth, aDestinationHeight));
	const Rect* paint_clip = SymbianRenderDamage::PaintClipRect();
	if (paint_clip)
		destination.Adjust(*paint_clip);
	if (destination.IsEmpty()) {
		aWrittenRect = Rect();
		return true;
	}

	const unsigned int opacity = static_cast<unsigned int>(std::min(255, aOpacity));
	const int stepX = static_cast<int>((static_cast<long long>(aSourceRect.width) << 16) / aDestinationRect.width);
	const int stepY = static_cast<int>((static_cast<long long>(aSourceRect.height) << 16) / aDestinationRect.height);
	int sourceYFixed = (destination.y - aDestinationRect.y) * stepY + stepY / 2;
	uint8_t* destinationBase = static_cast<uint8_t*>(aDestinationPixels);
	const uint8_t* sourceBase = static_cast<const uint8_t*>(aSourcePixels);
	for (int destinationY = destination.y; destinationY < destination.y + destination.height; ++destinationY) {
		int sourceLocalY = sourceYFixed >> 16;
		if (sourceLocalY >= aSourceRect.height)
			sourceLocalY = aSourceRect.height - 1;
		if (aFlipY)
			sourceLocalY = aSourceRect.height - 1 - sourceLocalY;
		const int sourceY = aSourceRect.y + sourceLocalY;
		uint16_t* destinationRow = reinterpret_cast<uint16_t*>(destinationBase + destinationY * aDestinationPitch);
		int sourceXFixed = (destination.x - aDestinationRect.x) * stepX + stepX / 2;
		for (int destinationX = destination.x; destinationX < destination.x + destination.width; ++destinationX) {
			int sourceLocalX = sourceXFixed >> 16;
			if (sourceLocalX >= aSourceRect.width)
				sourceLocalX = aSourceRect.width - 1;
			if (aFlipX)
				sourceLocalX = aSourceRect.width - 1 - sourceLocalX;
			unsigned int alpha;
			unsigned int red;
			unsigned int green;
			unsigned int blue;
			SymbianReadSourcePixel(sourceBase, aSourcePitch, aSourceFormat,
				aSourceRect.x + sourceLocalX, sourceY, alpha, red, green, blue);
			SymbianBlendSource565(destinationRow[destinationX], alpha, red, green, blue, opacity);
			sourceXFixed += stepX;
		}
		sourceYFixed += stepY;
	}
	aWrittenRect = destination;
	return true;
}

}
#endif

const Opacity Opacity::opaque;

BitmapRef Bitmap::Create(int width, int height, const Color& color) {
    BitmapRef surface = Bitmap::Create(width, height, false);
	surface->Fill(color);
	return surface;
}

BitmapRef Bitmap::Create(const std::string& filename, bool transparent, uint32_t flags) {
	BitmapRef bmp = std::make_shared<Bitmap>(filename, transparent, flags);

	if (!bmp->pixels()) {
		return BitmapRef();
	}

	return bmp;
}

BitmapRef Bitmap::Create(const uint8_t* data, unsigned bytes, bool transparent, uint32_t flags) {
	BitmapRef bmp = std::make_shared<Bitmap>(data, bytes, transparent, flags);

	if (!bmp->pixels()) {
		return BitmapRef();
	}

	return bmp;
}

BitmapRef Bitmap::Create(Bitmap const& source, Rect const& src_rect, bool transparent) {
	return std::make_shared<Bitmap>(source, src_rect, transparent);
}

void Bitmap::InitBitmap() {
	editing = false;
	font = Font::Default();
#ifdef EASYRPG_SYMBIAN
	++gSymbianBitmapSeed;
	if (gSymbianBitmapSeed == 0)
		gSymbianBitmapSeed = 1;
	revision_counter = gSymbianBitmapSeed;
#endif
}

Bitmap::Bitmap() {
	InitBitmap();
}

Bitmap::~Bitmap() {
	if (bitmap) {
		pixman_image_unref(bitmap);
	}
}

bool Bitmap::WritePNG(std::ostream& os) const {
#ifdef SUPPORT_PNG
	size_t const width = GetWidth(), height = GetHeight();
	size_t const stride = width * 4;

	std::vector<uint32_t> data(width * height);

	std::shared_ptr<pixman_image_t> dst
		(pixman_image_create_bits(PIXMAN_a8r8g8b8, width, height, &data.front(), stride),
		 pixman_image_unref);
	pixman_image_composite32(PIXMAN_OP_SRC, bitmap, NULL, dst.get(),
							 0, 0, 0, 0, 0, 0, width, height);

	return ImagePNG::WritePNG(os, width, height, &data.front());
#else
	return false;
#endif
}

int Bitmap::GetWidth() const {
	return width();
}

int Bitmap::GetHeight() const {
	return height();
}

Rect Bitmap::GetRect() const {
	return Rect(0, 0, width(), height());
}

bool Bitmap::GetTransparent() const {
	return format.alpha_type != PF::NoAlpha;
}

Color Bitmap::GetTransparentColor() const {
	return Color();
}

void Bitmap::SetTransparentColor(Color /* color */) {
}

Bitmap::TileOpacity Bitmap::CheckOpacity(const Rect& rect) {
	if (!bitmap || !pixels())
		return Bitmap::Transparent;
	if (format.alpha_type != PF::Alpha || format.a.mask == 0)
		return Bitmap::Opaque;

	Rect clipped = rect;
	clipped.Adjust(GetRect());
	if (clipped.IsEmpty())
		return Bitmap::Transparent;

	bool all = true;
	bool any = false;
	const uint8_t* base = static_cast<const uint8_t*>(pixels());
	for (int y = clipped.y; y < clipped.y + clipped.height; ++y) {
		const uint8_t* row = base + y * pitch();
		for (int x = clipped.x; x < clipped.x + clipped.width; ++x) {
			uint32_t value = 0;
			if (format.bytes == 4)
				value = reinterpret_cast<const uint32_t*>(row)[x];
			else if (format.bytes == 2)
				value = reinterpret_cast<const uint16_t*>(row)[x];
			else if (format.bytes == 1)
				value = row[x];
			else
				return Bitmap::Partial;
			if ((value & format.a.mask) != 0)
				any = true;
			else
				all = false;
			if (any && !all)
				return Bitmap::Partial;
		}
	}

	return all ? Bitmap::Opaque : any ? Bitmap::Partial : Bitmap::Transparent;
}

void Bitmap::CheckPixels(uint32_t flags) {
	if ((flags & Flag_System) && bpp() == 4 && width() > 16 && height() > 32) {
		const uint8_t* base = static_cast<const uint8_t*>(pixels());
		const uint32_t* row = reinterpret_cast<const uint32_t*>(base + 32 * pitch());
		uint8_t r = 0;
		uint8_t g = 0;
		uint8_t b = 0;
		uint8_t a = 0;
		format.uint32_to_rgba(row[0], r, g, b, a);
		bg_color = Color(r, g, b, a);
		format.uint32_to_rgba(row[16], r, g, b, a);
		sh_color = Color(r, g, b, a);
	}

	if (flags & Flag_Chipset) {
		tile_opacity.clear();
		tile_opacity.resize(height() / 16);
		for (int row = 0; row < height() / 16; row++) {
			tile_opacity[row].resize(width() / 16);
			for (int col = 0; col < width() / 16; col++) {
				Rect rect(col * 16, row * 16, 16, 16);
				tile_opacity[row][col] = CheckOpacity(rect);
			}
		}
	}

	if (flags & Flag_ReadOnly) {
		read_only = true;

		opacity = CheckOpacity(GetRect());
	}
}

Bitmap::TileOpacity Bitmap::GetOpacity() const {
	return opacity;
}

Bitmap::TileOpacity Bitmap::GetTileOpacity(int row, int col) const {
	return !tile_opacity.empty() ? tile_opacity[row][col] : Partial;
}

Color Bitmap::GetBackgroundColor() const {
	return bg_color;
}

Color Bitmap::GetShadowColor() const {
	return sh_color;
}

int Bitmap::bytes() const {
	return format.bytes;
}

uint8_t* Bitmap::pointer(int x, int y) {
	return (uint8_t*) pixels() + y * pitch() + x * bytes();
}

uint8_t const* Bitmap::pointer(int x, int y) const {
	return (uint8_t const*) pixels() + y * pitch() + x * bytes();
}

BitmapRef Bitmap::Create(int width, int height, bool transparent, int bpp) {
#ifdef EASYRPG_SYMBIAN
	if (transparent && bpp == 16)
		return std::make_shared<Bitmap>((void*)NULL, width, height, 0, SymbianA1555Format());
#endif
	return std::make_shared<Bitmap>(width, height, transparent);
}

BitmapRef Bitmap::Create(void *pixels, int width, int height, int pitch, const DynamicFormat& format) {
	return std::make_shared<Bitmap>(pixels, width, height, pitch, format);
}

void Bitmap::HueChangeBlit(int x, int y, Bitmap const& src, Rect const& src_rect_, double hue_) {
	Rect dst_rect(x, y, 0, 0), src_rect = src_rect_;

	if (!Rect::AdjustRectangles(src_rect, dst_rect, src.GetRect()))
		return;
	if (!Rect::AdjustRectangles(dst_rect, src_rect, GetRect()))
		return;

	int hue  = (int) (hue_ / 60.0 * 0x100);
	if (hue < 0)
		hue += ((-hue + 0x5FF) / 0x600) * 0x600;
	else if (hue > 0x600)
		hue -= (hue / 0x600) * 0x600;

	DynamicFormat format(32,8,24,8,16,8,8,8,0,PF::Alpha);
	std::vector<uint32_t> pixels;
	pixels.resize(src_rect.width * src_rect.height);
	Bitmap bmp(reinterpret_cast<void*>(&pixels.front()), src_rect.width, src_rect.height, src_rect.width * 4, format);
	bmp.Blit(0, 0, src, src_rect, Opacity::opaque);

	for (std::vector<uint32_t>::iterator p = pixels.begin(); p != pixels.end(); ++p) {
		uint32_t pixel = *p;
		uint8_t r = (pixel>>24) & 0xFF;
		uint8_t g = (pixel>>16) & 0xFF;
		uint8_t b = (pixel>> 8) & 0xFF;
		uint8_t a = pixel & 0xFF;
		if (a > 0)
			RGB_adjust_HSL(r, g, b, hue);
		*p = ((uint32_t) r << 24) | ((uint32_t) g << 16) | ((uint32_t) b << 8) | (uint32_t) a;
	}

	Blit(dst_rect.x, dst_rect.y, bmp, bmp.GetRect(), Opacity::opaque);

	RefreshCallback();
}

void Bitmap::RefreshCallback() {
#ifdef EASYRPG_SYMBIAN
	++revision_counter;
	if (revision_counter == 0)
		revision_counter = 1;
#endif
}

FontRef const& Bitmap::GetFont() const {
	return font;
}

void Bitmap::SetFont(FontRef const& new_font) {
	font = new_font;
}

void Bitmap::TextDraw(int x, int y, int width, int /* height */, int color, std::string const& text, Text::Alignment align) {
	Rect rect = GetFont()->GetSize(text);
	int dx = width - rect.width;

	switch (align) {
	case Text::AlignLeft:
		TextDraw(x, y, color, text);
		break;
	case Text::AlignCenter:
		TextDraw(x + dx / 2, y, color, text);
		break;
	case Text::AlignRight:
		TextDraw(x + dx, y, color, text);
		break;
	default: assert(false);
	}
}

void Bitmap::TextDraw(Rect const& rect, int color, std::string const& text, Text::Alignment align) {
	TextDraw(rect.x, rect.y, rect.width, rect.height, color, text, align);
}

void Bitmap::TextDraw(int x, int y, int color, std::string const& text, Text::Alignment align) {
	Text::Draw(*this, x, y, color, text, align);
}

void Bitmap::TextDraw(int x, int y, int width, int /* height */, Color color, std::string const& text, Text::Alignment align) {
	Rect rect = GetFont()->GetSize(text);
	int dx = width - rect.width;

	switch (align) {
	case Text::AlignLeft:
		TextDraw(x, y, color, text);
		break;
	case Text::AlignCenter:
		TextDraw(x + dx / 2, y, color, text);
		break;
	case Text::AlignRight:
		TextDraw(x + dx, y, color, text);
		break;
	default: assert(false);
	}
}

void Bitmap::TextDraw(Rect const& rect, Color color, std::string const& text, Text::Alignment align) {
	TextDraw(rect.x, rect.y, rect.width, rect.height, color, text, align);
}

void Bitmap::TextDraw(int x, int y, Color color, std::string const& text) {
	Text::Draw(*this, x, y, color, text);
}

Rect Bitmap::TransformRectangle(const Matrix& m, const Rect& rect) {
	int sx0 = rect.x;
	int sy0 = rect.y;
	int sx1 = rect.x + rect.width;
	int sy1 = rect.y + rect.height;

	double x0, y0, x1, y1, x2, y2, x3, y3;
	m.Transform(sx0, sy0, x0, y0);
	m.Transform(sx1, sy0, x1, y1);
	m.Transform(sx1, sy1, x2, y2);
	m.Transform(sx0, sy1, x3, y3);

	double xmin = std::min(std::min(x0, x1), std::min(x2, x3));
	double ymin = std::min(std::min(y0, y1), std::min(y2, y3));
	double xmax = std::max(std::max(x0, x1), std::max(x2, x3));
	double ymax = std::max(std::max(y0, y1), std::max(y2, y3));

	int dx0 = (int) floor(xmin);
	int dy0 = (int) floor(ymin);
	int dx1 = (int) ceil(xmax);
	int dy1 = (int) ceil(ymax);

	return Rect(dx0, dy0, dx1 - dx0, dy1 - dy0);
}

Matrix Matrix::Setup(double angle,
					 double scale_x, double scale_y,
					 int src_pos_x, int src_pos_y,
					 int dst_pos_x, int dst_pos_y) {
	Matrix m = Matrix::Translation(-src_pos_x, -src_pos_y);
	m = m.PreMultiply(Matrix::Scale(scale_x, scale_y));
	m = m.PreMultiply(Matrix::Rotation(angle));
	m = m.PreMultiply(Matrix::Translation(dst_pos_x, dst_pos_y));
	return m;
}

bool Bitmap::formats_initialized = false;
std::map<int, pixman_format_code_t> Bitmap::formats_map;

void Bitmap::add_pair(pixman_format_code_t pcode, const DynamicFormat& format) {
	int dcode = format.code_alpha();
	formats_map[dcode] = pcode;
}

void Bitmap::initialize_formats() {
	if (formats_initialized)
		return;

	add_pair(PIXMAN_a8r8g8b8, DynamicFormat(32,8,16,8,8,8,0,8,24,PF::Alpha));
	add_pair(PIXMAN_x8r8g8b8, DynamicFormat(32,8,16,8,8,8,0,0,0,PF::NoAlpha));
	add_pair(PIXMAN_a8b8g8r8, DynamicFormat(32,8,0,8,8,8,16,8,24,PF::Alpha));
	add_pair(PIXMAN_x8b8g8r8, DynamicFormat(32,8,0,8,8,8,16,0,0,PF::NoAlpha));
	add_pair(PIXMAN_b8g8r8a8, DynamicFormat(32,8,8,8,16,8,24,8,0,PF::Alpha));
	add_pair(PIXMAN_b8g8r8x8, DynamicFormat(32,8,8,8,16,8,24,0,0,PF::NoAlpha));

	add_pair(PIXMAN_x14r6g6b6, DynamicFormat(32,6,12,6,6,6,0,0,0,PF::NoAlpha));
	add_pair(PIXMAN_x2r10g10b10, DynamicFormat(32,10,20,10,10,10,0,0,0,PF::NoAlpha));
	add_pair(PIXMAN_a2r10g10b10, DynamicFormat(32,10,20,10,10,10,0,2,30,PF::Alpha));
	add_pair(PIXMAN_x2b10g10r10, DynamicFormat(32,10,0,10,10,10,20,0,0,PF::NoAlpha));
	add_pair(PIXMAN_a2b10g10r10, DynamicFormat(32,10,0,10,10,10,20,2,30,PF::Alpha));

	add_pair(PIXMAN_r8g8b8a8, DynamicFormat(32,8,24,8,16,8,8,8,0,PF::Alpha));
	add_pair(PIXMAN_r8g8b8x8, DynamicFormat(32,8,24,8,16,8,8,8,0,PF::NoAlpha));

	add_pair(PIXMAN_r8g8b8, DynamicFormat(24,8,16,8,8,8,0,0,0,PF::NoAlpha));
	add_pair(PIXMAN_b8g8r8, DynamicFormat(24,8,0,8,8,8,16,0,0,PF::NoAlpha));

	add_pair(PIXMAN_r5g6b5, DynamicFormat(16,5,11,6,5,5,0,0,0,PF::NoAlpha));
	add_pair(PIXMAN_b5g6r5, DynamicFormat(16,5,0,6,5,5,11,0,0,PF::NoAlpha));
	add_pair(PIXMAN_a1r5g5b5, DynamicFormat(16,5,10,5,5,5,0,1,15,PF::Alpha));
	add_pair(PIXMAN_x1r5g5b5, DynamicFormat(16,5,10,5,5,5,0,0,0,PF::NoAlpha));
	add_pair(PIXMAN_a1b5g5r5, DynamicFormat(16,5,0,5,5,5,10,1,15,PF::Alpha));
	add_pair(PIXMAN_x1b5g5r5, DynamicFormat(16,5,0,5,5,5,10,0,0,PF::NoAlpha));
	add_pair(PIXMAN_a4r4g4b4, DynamicFormat(16,4,8,4,4,4,0,4,12,PF::Alpha));
	add_pair(PIXMAN_x4r4g4b4, DynamicFormat(16,4,8,4,4,4,0,0,0,PF::NoAlpha));
	add_pair(PIXMAN_a4b4g4r4, DynamicFormat(16,4,0,4,4,4,8,4,12,PF::Alpha));
	add_pair(PIXMAN_x4b4g4r4, DynamicFormat(16,4,0,4,4,4,8,0,0,PF::NoAlpha));
	add_pair(PIXMAN_a8, DynamicFormat(8,8,0,8,0,8,0,8,0,PF::Alpha));
	add_pair(PIXMAN_g8, DynamicFormat(8,8,0,8,0,8,0,0,0,PF::NoAlpha));

	formats_initialized = true;
}

pixman_format_code_t Bitmap::find_format(const DynamicFormat& format) {
	initialize_formats();
	int dcode = format.code_alpha();
	int pcode = formats_map[dcode];
	if (pcode == 0) {
		// To fix add a pair to initialize_formats that maps the outputted
		// DynamicFormat to a pixman format
		Output::Error("%s\nDynamicFormat(%d, %d, %d, %d, %d, %d, %d, %d, %d, %s)",
		"Couldn't find Pixman format for",
		format.bits,
		format.r.bits, format.r.shift,
		format.g.bits, format.g.shift,
		format.b.bits, format.b.shift,
		format.a.bits, format.a.shift,
		format.alpha_type == PF::Alpha ? "PF::Alpha" : "PF::NoAlpha");
	}
	return (pixman_format_code_t) pcode;
}

DynamicFormat Bitmap::pixel_format;
DynamicFormat Bitmap::opaque_pixel_format;
DynamicFormat Bitmap::image_format;
DynamicFormat Bitmap::opaque_image_format;

void Bitmap::SetFormat(const DynamicFormat& format) {
#ifdef EASYRPG_SYMBIAN
	if (format.bits == 16 && format.alpha_type == PF::NoAlpha) {
		pixel_format = format_B8G8R8A8_a().format();
		opaque_pixel_format = format;
	} else {
		pixel_format = format;
		opaque_pixel_format = format;
		opaque_pixel_format.alpha_type = PF::NoAlpha;
	}
#else
	pixel_format = format;
	opaque_pixel_format = format;
	opaque_pixel_format.alpha_type = PF::NoAlpha;
#endif
	image_format = format_R8G8B8A8_a().format();
	opaque_image_format = format_R8G8B8A8_n().format();
}

DynamicFormat Bitmap::ChooseFormat(const DynamicFormat& format) {
	uint32_t amask;
	amask = (format.a.mask == 0)
		? ((~0U >> (32 - format.bits)) ^ (format.r.mask | format.g.mask | format.b.mask))
		: format.a.mask;
	if (amask != 0)
		return DynamicFormat(format.bits,
							 format.r.mask, format.g.mask, format.b.mask,
							 amask, PF::Alpha);
	switch (format.bits) {
		case 16:
			return (format.r.shift > format.b.shift)
				? DynamicFormat(16,5,10,5,5,5,0,1,15,PF::Alpha)
				: DynamicFormat(16,5,0,5,5,5,10,1,15,PF::Alpha);
		case 24:
			return (format.r.shift > format.b.shift)
				? DynamicFormat(32,8,16,8,8,8,0,8,24,PF::Alpha)
				: DynamicFormat(32,8,0,8,8,8,16,8,24,PF::Alpha);
		default:
			return format_B8G8R8A8_a().format();
	}
}

static void destroy_func(pixman_image_t * /* image */, void *data) {
	free(data);
}

static pixman_indexed_t palette;
static bool palette_initialized = false;

static void initialize_palette() {
	if (palette_initialized)
		return;
	palette.color = false;
	palette.rgba[0] = 0U;
	for (int i = 1; i < PIXMAN_MAX_INDEXED; i++)
		palette.rgba[i] = ~0U;
	palette_initialized = true;
}

void Bitmap::Init(int width, int height, void* data, int pitch, bool destroy) {
	if (!pitch)
		pitch = width * bytes();

	bitmap = pixman_image_create_bits(pixman_format, width, height, (uint32_t*) data, pitch);

	if (bitmap == NULL) {
		Output::Error("Couldn't create %dx%d image.", width, height);
	}
#ifdef EASYRPG_SYMBIAN
	else {
		symbian_width = width;
		symbian_height = height;
		symbian_pitch = pixman_image_get_stride(bitmap);
		symbian_bpp = (pixman_image_get_depth(bitmap) + 7) / 8;
		symbian_pixels = pixman_image_get_data(bitmap);
	}
#endif

	if (format.bits == 8 && format.alpha_type == PF::NoAlpha) {
		initialize_palette();
		pixman_image_set_indexed(bitmap, &palette);
	}

	if (data != NULL && destroy)
		pixman_image_set_destroy_function(bitmap, destroy_func, data);
}

void Bitmap::ConvertImage(int& width, int& height, void*& pixels, bool transparent) {
	const DynamicFormat& img_format = transparent ? image_format : opaque_image_format;

	if (transparent) {
		const bool binary_alpha = format.bits == 16 && format.a.bits == 1;
		for (int y = 0; y < height; y++) {
			uint8_t* dst = (uint8_t*) pixels + y * width * 4;
			for (int x = 0; x < width; x++) {
				uint8_t &r = *dst++;
				uint8_t &g = *dst++;
				uint8_t &b = *dst++;
				uint8_t &a = *dst++;
				if (binary_alpha) {
					if (a == 0)
						r = g = b = 0;
				} else {
					MultiplyAlpha(r, g, b, a);
				}
			}
		}
	}

	Bitmap src(pixels, width, height, 0, img_format);
	Clear();
	Blit(0, 0, src, src.GetRect(), Opacity::opaque);
	free(pixels);
}

Bitmap::Bitmap(int width, int height, bool transparent) {
	InitBitmap();

	format = (transparent ? pixel_format : opaque_pixel_format);
	pixman_format = find_format(format);
	Init(width, height, (void *) NULL);
}

Bitmap::Bitmap(void *pixels, int width, int height, int pitch, const DynamicFormat& _format) {
	InitBitmap();

	format = _format;
	pixman_format = find_format(format);
	Init(width, height, pixels, pitch, false);
}

Bitmap::Bitmap(const std::string& filename, bool transparent, uint32_t flags) {
	InitBitmap();

	format = (transparent ? pixel_format : opaque_pixel_format);
	pixman_format = find_format(format);

	FILE* stream = FileFinder::fopenUTF8(filename, "rb");
	if (!stream) {
		Output::Error("Couldn't open image file %s", filename.c_str());
		return;
	}

	int w = 0;
	int h = 0;
	void* pixels;

	char data[4];
	size_t bytes = fread(&data, 1, 4, stream);
	fseek(stream, 0, SEEK_SET);

	bool img_okay = false;

#ifdef SUPPORT_XYZ
	if (bytes >= 4 && strncmp((char*)data, "XYZ1", 4) == 0)
		img_okay = ImageXYZ::ReadXYZ(stream, transparent, w, h, pixels);
	else
#endif
#ifdef SUPPORT_BMP
	if (bytes > 2 && strncmp((char*)data, "BM", 2) == 0)
		img_okay = ImageBMP::ReadBMP(stream, transparent, w, h, pixels);
	else
#endif
#ifdef SUPPORT_PNG
	if (bytes >= 4 && strncmp((char*)(data + 1), "PNG", 3) == 0)
		img_okay = ImagePNG::ReadPNG(stream, (void*)NULL, transparent, w, h, pixels);
	else
#endif
		Output::Warning("Unsupported image file %s", filename.c_str());

	fclose(stream);

	if (!img_okay) {
		free(pixels);

		pixels = nullptr;

		return;
	}

#ifdef EASYRPG_SYMBIAN
	if (transparent && SymbianUseA1555(pixels, w, h, flags)) {
		format = SymbianA1555Format();
		pixman_format = find_format(format);
	}
#endif
	Init(w, h, (void *) NULL);

	ConvertImage(w, h, pixels, transparent);

	CheckPixels(flags);
}

Bitmap::Bitmap(const uint8_t* data, unsigned bytes, bool transparent, uint32_t flags) {
	InitBitmap();

	format = (transparent ? pixel_format : opaque_pixel_format);
	pixman_format = find_format(format);

	int w = 0, h = 0;
	void* pixels;

	bool img_okay = false;

#ifdef SUPPORT_XYZ
	if (bytes > 4 && strncmp((char*) data, "XYZ1", 4) == 0)
		img_okay = ImageXYZ::ReadXYZ(data, bytes, transparent, w, h, pixels);
	else
#endif
#ifdef SUPPORT_BMP
	if (bytes > 2 && strncmp((char*) data, "BM", 2) == 0)
		img_okay = ImageBMP::ReadBMP(data, bytes, transparent, w, h, pixels);
	else
#endif
#ifdef SUPPORT_PNG
	if (bytes > 4 && strncmp((char*)(data + 1), "PNG", 3) == 0)
#ifdef EASYRPG_SYMBIAN
		img_okay = ImagePNG::ReadPNGBytes(data, bytes, transparent, w, h, pixels);
#else
		img_okay = ImagePNG::ReadPNG((FILE*) NULL, (const void*) data, transparent, w, h, pixels);
#endif
	else
#endif
		Output::Warning("Unsupported image");

	if (!img_okay) {
		return;
	}

#ifdef EASYRPG_SYMBIAN
	if (transparent && SymbianUseA1555(pixels, w, h, flags)) {
		format = SymbianA1555Format();
		pixman_format = find_format(format);
	}
#endif
	Init(w, h, (void *) NULL);

	ConvertImage(w, h, pixels, transparent);

	CheckPixels(flags);
}

Bitmap::Bitmap(Bitmap const& source, Rect const& src_rect, bool transparent) {
	InitBitmap();

	format = (transparent ? pixel_format : opaque_pixel_format);
	pixman_format = find_format(format);

	Init(src_rect.width, src_rect.height, (void *) NULL);

	Blit(0, 0, source, src_rect, Opacity::opaque);
}

void* Bitmap::pixels() {
#ifdef EASYRPG_SYMBIAN
	return symbian_pixels;
#else
	if (!bitmap) {
		return nullptr;
	}
	return (void*) pixman_image_get_data(bitmap);
#endif
}
void const* Bitmap::pixels() const {
#ifdef EASYRPG_SYMBIAN
	return symbian_pixels;
#else
	return (void const*) pixman_image_get_data(bitmap);
#endif
}

int Bitmap::bpp() const {
#ifdef EASYRPG_SYMBIAN
	return symbian_bpp;
#else
	return (pixman_image_get_depth(bitmap) + 7) / 8;
#endif
}

int Bitmap::width() const {
#ifdef EASYRPG_SYMBIAN
	return symbian_width;
#else
	return pixman_image_get_width(bitmap);
#endif
}

int Bitmap::height() const {
#ifdef EASYRPG_SYMBIAN
	return symbian_height;
#else
	return pixman_image_get_height(bitmap);
#endif
}

int Bitmap::pitch() const {
#ifdef EASYRPG_SYMBIAN
	return symbian_pitch;
#else
	return pixman_image_get_stride(bitmap);
#endif
}

#ifdef EASYRPG_SYMBIAN
uint32_t Bitmap::Revision() const {
	return revision_counter;
}

void Bitmap::Touch() {
	RefreshCallback();
}

void Bitmap::Touch(const Rect&) {
	++revision_counter;
	if (revision_counter == 0)
		revision_counter = 1;
}

bool Bitmap::EnsureSymbianOpaqueSpans() const {
	if (pixman_format != PIXMAN_a1r5g5b5 || !symbian_pixels || symbian_width <= 0 || symbian_height <= 0)
		return false;
	if (symbian_span_revision == revision_counter)
		return symbian_spans_usable;

	symbian_span_revision = revision_counter;
	symbian_spans_usable = false;
	symbian_span_offsets.clear();
	symbian_spans.clear();
	symbian_span_offsets.resize(static_cast<size_t>(symbian_height) + 1u);

	const int max_spans = std::max(32, (symbian_width * symbian_height) / 16);
	const uint8_t* base = static_cast<const uint8_t*>(symbian_pixels);
	int span_count = 0;
	for (int row = 0; row < symbian_height; ++row) {
		symbian_span_offsets[static_cast<size_t>(row)] = static_cast<int>(symbian_spans.size());
		const uint16_t* source = reinterpret_cast<const uint16_t*>(base + row * symbian_pitch);
		int column = 0;
		while (column < symbian_width) {
			while (column < symbian_width && (source[column] & 0x8000u) == 0)
				++column;
			const int start = column;
			while (column < symbian_width && (source[column] & 0x8000u) != 0)
				++column;
			if (start < column) {
				if (++span_count > max_spans) {
					symbian_span_offsets.clear();
					symbian_spans.clear();
					return false;
				}
				symbian_spans.push_back(start);
				symbian_spans.push_back(column);
			}
		}
	}
	symbian_span_offsets[static_cast<size_t>(symbian_height)] = static_cast<int>(symbian_spans.size());
	symbian_spans_usable = true;
	return true;
}

bool Bitmap::SymbianBlitA1555Spans565(int x, int y, Bitmap const& src,
	const Rect& src_rect, Rect& written) {
	written = Rect();
	if (this == &src || pixman_format != PIXMAN_r5g6b5 ||
		src.pixman_format != PIXMAN_a1r5g5b5 || !src.EnsureSymbianOpaqueSpans())
		return false;

	int sx = src_rect.x;
	int sy = src_rect.y;
	int dx = x;
	int dy = y;
	int copy_width = src_rect.width;
	int copy_height = src_rect.height;
	if (sx < 0) { dx -= sx; copy_width += sx; sx = 0; }
	if (sy < 0) { dy -= sy; copy_height += sy; sy = 0; }
	if (dx < 0) { sx -= dx; copy_width += dx; dx = 0; }
	if (dy < 0) { sy -= dy; copy_height += dy; dy = 0; }
	copy_width = std::min(copy_width, src.symbian_width - sx);
	copy_height = std::min(copy_height, src.symbian_height - sy);
	copy_width = std::min(copy_width, symbian_width - dx);
	copy_height = std::min(copy_height, symbian_height - dy);
	const Rect* paint_clip = SymbianRenderDamage::PaintClipRect();
	if (paint_clip) {
		const int left = std::max(dx, paint_clip->x);
		const int top = std::max(dy, paint_clip->y);
		const int right = std::min(dx + copy_width, paint_clip->x + paint_clip->width);
		const int bottom = std::min(dy + copy_height, paint_clip->y + paint_clip->height);
		sx += left - dx;
		sy += top - dy;
		dx = left;
		dy = top;
		copy_width = right - left;
		copy_height = bottom - top;
	}
	if (copy_width <= 0 || copy_height <= 0)
		return true;

	uint8_t* destination_base = static_cast<uint8_t*>(symbian_pixels);
	const uint8_t* source_base = static_cast<const uint8_t*>(src.symbian_pixels);
	const int source_right = sx + copy_width;
	bool has_written = false;
	for (int row = 0; row < copy_height; ++row) {
		const int source_y = sy + row;
		const int first = src.symbian_span_offsets[static_cast<size_t>(source_y)];
		const int last = src.symbian_span_offsets[static_cast<size_t>(source_y + 1)];
		for (int span = first; span < last; span += 2) {
			const int start = std::max(sx, src.symbian_spans[static_cast<size_t>(span)]);
			const int end = std::min(source_right, src.symbian_spans[static_cast<size_t>(span + 1)]);
			if (start >= end)
				continue;

			uint16_t* destination = reinterpret_cast<uint16_t*>(
				destination_base + (dy + row) * symbian_pitch) + dx + start - sx;
			const uint16_t* source = reinterpret_cast<const uint16_t*>(
				source_base + source_y * src.symbian_pitch) + start;
			int count = end - start;
			if ((reinterpret_cast<size_t>(destination) & 3u) != 0 && count > 0) {
				*destination++ = SymbianA1555To565(*source++);
				--count;
			}
			uint32_t* destination32 = reinterpret_cast<uint32_t*>(destination);
			while (count >= 2) {
				const uint32_t pixels = (reinterpret_cast<size_t>(source) & 3u) == 0
					? *reinterpret_cast<const uint32_t*>(source)
					: static_cast<uint32_t>(source[0]) | (static_cast<uint32_t>(source[1]) << 16);
				*destination32++ = SymbianA1555PairTo565(pixels);
				source += 2;
				count -= 2;
			}
			destination = reinterpret_cast<uint16_t*>(destination32);
			if (count > 0)
				*destination = SymbianA1555To565(*source);
			has_written = true;
		}
	}
	if (has_written)
		written.Set(dx, dy, copy_width, copy_height);
	return true;
}

void Bitmap::SetClipRects(const Rect* aRects, int aCount) {
	if (!bitmap || !aRects || aCount <= 0) {
		pixman_image_set_clip_region32(bitmap, nullptr);
		return;
	}

	pixman_region32_t region;
	if (aCount == 1) {
		const Rect& rect = aRects[0];
		pixman_region32_init_rect(&region, rect.x, rect.y, rect.width, rect.height);
	} else {
		pixman_region32_init(&region);
		for (int index = 0; index < aCount; ++index) {
			const Rect& rect = aRects[index];
			if (!rect.IsEmpty())
				pixman_region32_union_rect(&region, &region, rect.x, rect.y, rect.width, rect.height);
		}
	}
	pixman_image_set_clip_region32(bitmap, &region);
	pixman_region32_fini(&region);
}

void Bitmap::ClearClipRects() {
	if (bitmap)
		pixman_image_set_clip_region32(bitmap, nullptr);
}
#endif

uint32_t Bitmap::rmask() const {
	return pixel_format.r.mask;
}

uint32_t Bitmap::gmask() const {
	return pixel_format.g.mask;
}

uint32_t Bitmap::bmask() const {
	return pixel_format.b.mask;
}

uint32_t Bitmap::amask() const {
	return pixel_format.a.mask;
}

namespace {
	pixman_image_t *CreateMask(Opacity const& opacity, Rect const& src_rect, pixman_transform_t const* pxform = NULL) {
		if (opacity.IsOpaque())
			return (pixman_image_t*) NULL;

		if (!opacity.IsSplit()) {
			static pixman_image_t* masks[256] = {};
			const int value = std::max(0, std::min(255, opacity.Value()));
			if (!masks[value]) {
				pixman_color_t tcolor = {0, 0, 0, static_cast<uint16_t>(value << 8)};
				masks[value] = pixman_image_create_solid_fill(&tcolor);
			}
			return masks[value] ? pixman_image_ref(masks[value]) : NULL;
		}

		pixman_image_t *mask = pixman_image_create_bits(PIXMAN_a8, 1, 2, (uint32_t*) NULL, 4);
		uint32_t* pixels = pixman_image_get_data(mask);
		*reinterpret_cast<uint8_t*>(&pixels[0]) = (opacity.top & 0xFF);
		*reinterpret_cast<uint8_t*>(&pixels[1]) = (opacity.bottom & 0xFF);

		pixman_transform_t xform;
		pixman_transform_init_identity(&xform);

		pixman_transform_scale((pixman_transform_t*) NULL, &xform, 
								pixman_int_to_fixed(src_rect.width),
								pixman_int_to_fixed(src_rect.height));

		pixman_transform_translate((pixman_transform_t*) NULL, &xform,
									   pixman_int_to_fixed(0),
									   pixman_int_to_fixed(-opacity.split));
		if (pxform)
			pixman_transform_multiply(&xform, &xform, pxform);

		pixman_image_set_transform(mask, &xform);

		return mask;
	}
} // anonymous namespace

void Bitmap::Blit(int x, int y, Bitmap const& src, Rect const& src_rect, Opacity const& opacity) {
	if (opacity.IsTransparent())
		return;
#ifdef EASYRPG_SYMBIAN
	unsigned int signature = SymbianDrawSignature(ESymbianBlit);
	signature = SymbianHashBitmap(signature, src);
	signature = SymbianHashRect(signature, src_rect);
	signature = SymbianHashOpacity(signature, opacity);
	if (SymbianRecordDraw(this, signature, Rect(x, y, src_rect.width, src_rect.height)))
		return;
	if (opacity.IsSplit()) {
		const int split = std::max(0, std::min(src_rect.height, opacity.split));
		if (split > 0 && opacity.top > 0)
			Blit(x, y, src, Rect(src_rect.x, src_rect.y, src_rect.width, split), Opacity(opacity.top));
		if (split < src_rect.height && opacity.bottom > 0)
			Blit(x, y + split, src,
				Rect(src_rect.x, src_rect.y + split, src_rect.width, src_rect.height - split),
				Opacity(opacity.bottom));
		return;
	}
	if (this != &src && pixman_format == PIXMAN_r5g6b5) {
		Rect written;
		if (opacity.Value() == 255 && src.pixman_format == PIXMAN_a1r5g5b5 &&
			SymbianBlitA1555Spans565(x, y, src, src_rect, written)) {
			if (!written.IsEmpty())
				Touch(written);
			return;
		}
		if (SymbianBlitTo565(pixels(), pitch(), width(), height(),
			src.pixels(), src.pitch(), src.width(), src.height(), src.pixman_format,
			x, y, src_rect, opacity.Value(), written)) {
			if (!written.IsEmpty())
				Touch(written);
			return;
		}
	}
#endif

	pixman_image_t* mask = CreateMask(opacity, src_rect);

	pixman_image_composite32(src.GetOperator(mask),
							 src.bitmap,
							 mask, bitmap,
							 src_rect.x, src_rect.y,
							 0, 0,
							 x, y,
							 src_rect.width, src_rect.height);

	if (mask != NULL)
		pixman_image_unref(mask);

#ifdef EASYRPG_SYMBIAN
	Touch(Rect(x, y, src_rect.width, src_rect.height));
#else
	RefreshCallback();
#endif
}

void Bitmap::BlitTileFast565(int x, int y, Bitmap const& src, int src_x, int src_y) {
#ifdef EASYRPG_SYMBIAN
	if (this != &src && pixman_format == PIXMAN_r5g6b5 &&
		(src.pixman_format == PIXMAN_r5g6b5 || src.pixman_format == PIXMAN_a1r5g5b5)) {
		int dx = x;
		int dy = y;
		int sx = src_x;
		int sy = src_y;
		int copy_width = TILE_SIZE;
		int copy_height = TILE_SIZE;
		const int source_width = src.width();
		const int source_height = src.height();
		const int destination_width = width();
		const int destination_height = height();
		if (sx < 0) { dx -= sx; copy_width += sx; sx = 0; }
		if (sy < 0) { dy -= sy; copy_height += sy; sy = 0; }
		if (dx < 0) { sx -= dx; copy_width += dx; dx = 0; }
		if (dy < 0) { sy -= dy; copy_height += dy; dy = 0; }
		copy_width = std::min(copy_width, source_width - sx);
		copy_height = std::min(copy_height, source_height - sy);
		copy_width = std::min(copy_width, destination_width - dx);
		copy_height = std::min(copy_height, destination_height - dy);
		const Rect* paint_clip = SymbianRenderDamage::PaintClipRect();
		if (paint_clip) {
			const Rect& dirty = *paint_clip;
			const int left = std::max(dx, dirty.x);
			const int top = std::max(dy, dirty.y);
			const int right = std::min(dx + copy_width, dirty.x + dirty.width);
			const int bottom = std::min(dy + copy_height, dirty.y + dirty.height);
			sx += left - dx;
			sy += top - dy;
			dx = left;
			dy = top;
			copy_width = right - left;
			copy_height = bottom - top;
		}
		if (copy_width <= 0 || copy_height <= 0)
			return;

		uint8_t* destination_base = static_cast<uint8_t*>(pixels());
		const uint8_t* source_base = static_cast<const uint8_t*>(src.pixels());
		const int destination_pitch = pitch();
		const int source_pitch = src.pitch();
		if (src.pixman_format == PIXMAN_r5g6b5) {
			if (copy_width == TILE_SIZE) {
				for (int row = 0; row < copy_height; ++row)
					SymbianCopyTileRow565(destination_base + (dy + row) * destination_pitch + dx * 2,
						source_base + (sy + row) * source_pitch + sx * 2);
			} else {
				const size_t row_bytes = static_cast<size_t>(copy_width) * 2u;
				for (int row = 0; row < copy_height; ++row)
					std::memcpy(destination_base + (dy + row) * destination_pitch + dx * 2,
						source_base + (sy + row) * source_pitch + sx * 2,
						row_bytes);
			}
		} else {
			for (int row = 0; row < copy_height; ++row) {
				uint16_t* destination = reinterpret_cast<uint16_t*>(destination_base + (dy + row) * destination_pitch) + dx;
				const uint16_t* source = reinterpret_cast<const uint16_t*>(source_base + (sy + row) * source_pitch) + sx;
				int column = 0;
				if (((reinterpret_cast<size_t>(destination) | reinterpret_cast<size_t>(source)) & 3u) == 0) {
					uint32_t* destination32 = reinterpret_cast<uint32_t*>(destination);
					const uint32_t* source32 = reinterpret_cast<const uint32_t*>(source);
					const int pairs = copy_width >> 1;
					for (int pair = 0; pair < pairs; ++pair)
						destination32[pair] = SymbianA1555PairTo565(source32[pair]);
					column = pairs << 1;
				}
				for (; column < copy_width; ++column)
					destination[column] = SymbianA1555To565(source[column]);
			}
		}
		return;
	}
#endif
	BlitFast(x, y, src, Rect(src_x, src_y, TILE_SIZE, TILE_SIZE), Opacity::opaque);
}

void Bitmap::BlitFast(int x, int y, Bitmap const & src, Rect const & src_rect, Opacity const & opacity) {
	if (opacity.IsTransparent())
		return;
#ifdef EASYRPG_SYMBIAN
	unsigned int signature = SymbianDrawSignature(ESymbianBlitFast);
	signature = SymbianHashBitmap(signature, src);
	signature = SymbianHashRect(signature, src_rect);
	signature = SymbianHashOpacity(signature, opacity);
	if (SymbianRecordDraw(this, signature, Rect(x, y, src_rect.width, src_rect.height)))
		return;
	if (this != &src && !opacity.IsSplit() && pixman_format == PIXMAN_r5g6b5) {
		Rect written;
		if (opacity.Value() == 255 && src.pixman_format == PIXMAN_a1r5g5b5 &&
			SymbianBlitA1555Spans565(x, y, src, src_rect, written)) {
			if (!written.IsEmpty())
				Touch(written);
			return;
		}
		if (SymbianBlitTo565(pixels(), pitch(), width(), height(),
			src.pixels(), src.pitch(), src.width(), src.height(), src.pixman_format,
			x, y, src_rect, opacity.Value(), written)) {
			if (!written.IsEmpty())
				Touch(written);
			return;
		}
	}
#endif

	pixman_image_composite32(src.GetOperator(),
		src.bitmap,
		nullptr, bitmap,
		src_rect.x, src_rect.y,
		0, 0,
		x, y,
		src_rect.width, src_rect.height);

	RefreshCallback();
}

pixman_image_t* Bitmap::GetSubimage(Bitmap const& src, const Rect& src_rect) {
	uint8_t* pixels = (uint8_t*) src.pixels() + src_rect.x * src.bpp() + src_rect.y * src.pitch();
	pixman_image_t* subimage = pixman_image_create_bits(src.pixman_format,
		src_rect.width,
		src_rect.height,
		(uint32_t*) pixels,
		src.pitch());
	return subimage;
}

void Bitmap::TiledBlit(Rect const& src_rect, Bitmap const& src, Rect const& dst_rect, Opacity const& opacity) {
	TiledBlit(0, 0, src_rect, src, dst_rect, opacity);
}

void Bitmap::TiledBlit(int ox, int oy, Rect const& src_rect, Bitmap const& src, Rect const& dst_rect, Opacity const& opacity) {
	if (opacity.IsTransparent())
		return;
#ifdef EASYRPG_SYMBIAN
	unsigned int signature = SymbianDrawSignature(ESymbianTiledBlit);
	signature = SymbianHashBitmap(signature, src);
	signature = SymbianHashRect(signature, src_rect);
	signature = SymbianHashRect(signature, dst_rect);
	signature = SymbianHashInt(signature, ox);
	signature = SymbianHashInt(signature, oy);
	signature = SymbianHashOpacity(signature, opacity);
	if (SymbianRecordDraw(this, signature, dst_rect))
		return;
	if (this != &src && !opacity.IsSplit() && opacity.Value() == 255 &&
		pixman_format == PIXMAN_r5g6b5 && src.pixman_format == PIXMAN_r5g6b5 &&
		src_rect.width > 0 && src_rect.height > 0 &&
		src_rect.x >= 0 && src_rect.y >= 0 &&
		src_rect.x + src_rect.width <= src.width() &&
		src_rect.y + src_rect.height <= src.height()) {
		Rect destination = dst_rect;
		destination.Adjust(GetRect());
		const Rect* paint_clip = SymbianRenderDamage::PaintClipRect();
		if (paint_clip)
			destination.Adjust(*paint_clip);
		if (!destination.IsEmpty()) {
			int source_y_offset = (destination.y - dst_rect.y + oy) % src_rect.height;
			if (source_y_offset < 0)
				source_y_offset += src_rect.height;
			uint8_t* destination_base = static_cast<uint8_t*>(pixels());
			const uint8_t* source_base = static_cast<const uint8_t*>(src.pixels());
			for (int row = 0; row < destination.height; ++row) {
				const int source_y = src_rect.y + source_y_offset;
				uint8_t* destination_row = destination_base +
					(destination.y + row) * pitch() + destination.x * 2;
				const uint8_t* source_row = source_base + source_y * src.pitch();
				int source_x_offset = (destination.x - dst_rect.x + ox) % src_rect.width;
				if (source_x_offset < 0)
					source_x_offset += src_rect.width;
				int remaining = destination.width;
				while (remaining > 0) {
					const int pixels_to_copy = std::min(remaining, src_rect.width - source_x_offset);
					std::memcpy(destination_row,
						source_row + (src_rect.x + source_x_offset) * 2,
						static_cast<size_t>(pixels_to_copy) * 2u);
					destination_row += pixels_to_copy * 2;
					remaining -= pixels_to_copy;
					source_x_offset = 0;
				}
				if (++source_y_offset == src_rect.height)
					source_y_offset = 0;
			}
			Touch(destination);
		}
		return;
	}
#endif

	if (ox >= src_rect.width)	ox %= src_rect.width;
	if (oy >= src_rect.height)	ox %= src_rect.height;
	if (ox < 0) ox += src_rect.width  * ((-ox + src_rect.width  - 1) / src_rect.width);
	if (oy < 0) oy += src_rect.height * ((-oy + src_rect.height - 1) / src_rect.height);

	pixman_image_t* src_bm = GetSubimage(src, src_rect);

	pixman_image_set_repeat(src_bm, PIXMAN_REPEAT_NORMAL);

	pixman_transform_t xform;
	pixman_transform_init_translate(&xform,
									pixman_int_to_fixed(ox),
									pixman_int_to_fixed(oy));

	pixman_image_set_transform(src_bm, &xform);

	pixman_image_t* mask = CreateMask(opacity, src_rect, &xform);

	pixman_image_composite32(src.GetOperator(mask),
							 src_bm, mask, bitmap,
							 0, 0,
							 0, 0,
							 dst_rect.x, dst_rect.y,
							 dst_rect.width, dst_rect.height);

	pixman_image_unref(src_bm);

	if (mask != NULL)
		pixman_image_unref(mask);

	RefreshCallback();
}

void Bitmap::StretchBlit(Bitmap const&  src, Rect const& src_rect, Opacity const& opacity) {
	StretchBlit(GetRect(), src, src_rect, opacity);
}

void Bitmap::StretchBlit(Rect const& dst_rect, Bitmap const& src, Rect const& src_rect, Opacity const& opacity) {
	if (opacity.IsTransparent())
		return;
#ifdef EASYRPG_SYMBIAN
	unsigned int signature = SymbianDrawSignature(ESymbianStretchBlit);
	signature = SymbianHashBitmap(signature, src);
	signature = SymbianHashRect(signature, src_rect);
	signature = SymbianHashRect(signature, dst_rect);
	signature = SymbianHashOpacity(signature, opacity);
	if (SymbianRecordDraw(this, signature, dst_rect))
		return;
	if (!opacity.IsSplit() && this != &src && pixman_format == PIXMAN_r5g6b5) {
		Rect written;
		if (SymbianScaleTo565(pixels(), pitch(), width(), height(),
			src.pixels(), src.pitch(), src.width(), src.height(), src.pixman_format,
			dst_rect, src_rect, opacity.Value(), false, false, written)) {
			if (!written.IsEmpty())
				Touch(written);
			return;
		}
	}
#endif

	double zoom_x = (double)src_rect.width  / dst_rect.width;
	double zoom_y = (double)src_rect.height / dst_rect.height;

	pixman_transform_t xform;

	pixman_transform_init_scale(&xform,
								pixman_double_to_fixed(zoom_x),
								pixman_double_to_fixed(zoom_y));

	pixman_transform_translate((pixman_transform_t *) NULL, &xform, src_rect.x, src_rect.y);

	pixman_image_set_transform(src.bitmap, &xform);

	pixman_image_t* mask = CreateMask(opacity, src_rect, &xform);

	pixman_image_composite32(src.GetOperator(mask),
							 src.bitmap, mask, bitmap,
							 src_rect.x / zoom_x, src_rect.y / zoom_y,
							 0, 0,
							 dst_rect.x, dst_rect.y,
							 dst_rect.width, dst_rect.height);

	pixman_transform_init_identity(&xform);
	pixman_image_set_transform(src.bitmap, &xform);

	if (mask != NULL)
		pixman_image_unref(mask);

	RefreshCallback();
}

void Bitmap::TransformBlit(Rect const& dst_rect, Bitmap const& src, Rect const& src_rect, const Matrix& inv, Opacity const& opacity) {
	if (opacity.IsTransparent())
		return;
#ifdef EASYRPG_SYMBIAN
	unsigned int signature = SymbianDrawSignature(ESymbianTransformBlit);
	signature = SymbianHashBitmap(signature, src);
	signature = SymbianHashRect(signature, src_rect);
	signature = SymbianHashRect(signature, dst_rect);
	signature = SymbianHashDouble(signature, inv.xx);
	signature = SymbianHashDouble(signature, inv.xy);
	signature = SymbianHashDouble(signature, inv.x0);
	signature = SymbianHashDouble(signature, inv.yx);
	signature = SymbianHashDouble(signature, inv.yy);
	signature = SymbianHashDouble(signature, inv.y0);
	signature = SymbianHashOpacity(signature, opacity);
	if (SymbianRecordDraw(this, signature, dst_rect))
		return;
#endif

	pixman_transform_t xform = {{
		{ pixman_double_to_fixed(inv.xx), pixman_double_to_fixed(inv.xy), pixman_double_to_fixed(inv.x0) },
		{ pixman_double_to_fixed(inv.yx), pixman_double_to_fixed(inv.yy), pixman_double_to_fixed(inv.y0) },
		{ pixman_double_to_fixed(0.0),    pixman_double_to_fixed(0.0),    pixman_double_to_fixed(1.0) }
		}};

	pixman_image_set_transform(src.bitmap, &xform);

	pixman_image_t* mask = CreateMask(opacity, src.GetRect(), &xform);

	pixman_image_composite32(src.GetOperator(mask),
							 src.bitmap, mask, bitmap,
							 dst_rect.x, dst_rect.y,
							 dst_rect.x, dst_rect.y,
							 dst_rect.x, dst_rect.y,
							 dst_rect.width, dst_rect.height);

	pixman_transform_init_identity(&xform);
	pixman_image_set_transform(src.bitmap, &xform);

	if (mask != NULL)
		pixman_image_unref(mask);

	RefreshCallback();
}

void Bitmap::WaverBlit(int x, int y, double zoom_x, double zoom_y, Bitmap const& src, Rect const& src_rect, int depth, double phase, Opacity const& opacity) {
	if (opacity.IsTransparent())
		return;
#ifdef EASYRPG_SYMBIAN
	const int outputWidth = static_cast<int>(std::floor(src_rect.width * zoom_x));
	const int outputHeight = static_cast<int>(std::floor(src_rect.height * zoom_y));
	const int margin = static_cast<int>(std::ceil(std::fabs(2.0 * zoom_x * depth)));
	unsigned int signature = SymbianDrawSignature(ESymbianWaverBlit);
	signature = SymbianHashBitmap(signature, src);
	signature = SymbianHashRect(signature, src_rect);
	signature = SymbianHashDouble(signature, zoom_x);
	signature = SymbianHashDouble(signature, zoom_y);
	signature = SymbianHashInt(signature, depth);
	signature = SymbianHashDouble(signature, phase);
	signature = SymbianHashOpacity(signature, opacity);
	if (SymbianRecordDraw(this, signature, Rect(x - margin, y, outputWidth + margin * 2, outputHeight)))
		return;
#endif

	pixman_transform_t xform;
	pixman_transform_init_scale(&xform,
								pixman_double_to_fixed(1.0 / zoom_x),
								pixman_double_to_fixed(1.0 / zoom_y));

	pixman_image_set_transform(src.bitmap, &xform);

	pixman_image_t* mask = CreateMask(opacity, src_rect, &xform);

	int height = static_cast<int>(std::floor(src_rect.height * zoom_y));
	int width  = static_cast<int>(std::floor(src_rect.width * zoom_x));
	for (int i = 0; i < height; i++) {
		int dy = y + i;
		if (dy < 0)
			continue;
		if (dy >= this->height())
			break;
		int sy = static_cast<int>(std::floor((i+0.5) / zoom_y));
		int offset = (int) (2 * zoom_x * depth * sin((phase + (src_rect.y + sy) * 11.2) * 3.14159 / 180));

		pixman_image_composite32(src.GetOperator(mask),
								 src.bitmap, mask, bitmap,
								 src_rect.x, i,
								 src_rect.x, i,
								 x + offset, dy,
								 width, 1);
	}

	pixman_transform_init_identity(&xform);
	pixman_image_set_transform(src.bitmap, &xform);

	if (mask != NULL)
		pixman_image_unref(mask);

	RefreshCallback();
}

static pixman_color_t PixmanColor(const Color &color) {
	pixman_color_t pcolor;
	pcolor.red = color.red * color.alpha;
	pcolor.green = color.green * color.alpha;
	pcolor.blue = color.blue * color.alpha;
	pcolor.alpha = color.alpha << 8;
	return pcolor;
}

void Bitmap::Fill(const Color &color) {
#ifdef EASYRPG_SYMBIAN
	unsigned int signature = SymbianDrawSignature(ESymbianFill);
	signature = SymbianHashColor(signature, color);
	if (SymbianRecordDraw(this, signature, GetRect()))
		return;
#endif
#ifdef EASYRPG_SYMBIAN
	if (bpp() == 2 && color.alpha >= 255) {
		SymbianFill565(*this, GetRect(), SymbianColor565(color));
		Touch(GetRect());
		return;
	}
#endif
	pixman_color_t pcolor = PixmanColor(color);
	Rect src_rect(0, 0, static_cast<uint16_t>(width()), static_cast<uint16_t>(height()));

	pixman_image_t* timage = pixman_image_create_solid_fill(&pcolor);

	pixman_image_composite32(PIXMAN_OP_SRC,
		timage, (pixman_image_t*)NULL, bitmap,
		src_rect.x, src_rect.y,
		0, 0,
		0, 0,
		src_rect.width, src_rect.height);

	pixman_image_unref(timage);

#ifdef EASYRPG_SYMBIAN
	Touch(GetRect());
#else
	RefreshCallback();
#endif
}

void Bitmap::FillRect(Rect const& dst_rect, const Color &color) {
#ifdef EASYRPG_SYMBIAN
	unsigned int signature = SymbianDrawSignature(ESymbianFillRect);
	signature = SymbianHashRect(signature, dst_rect);
	signature = SymbianHashColor(signature, color);
	if (SymbianRecordDraw(this, signature, dst_rect))
		return;
#endif
#ifdef EASYRPG_SYMBIAN
	if (bpp() == 2 && color.alpha >= 255) {
		SymbianFill565(*this, dst_rect, SymbianColor565(color));
		Touch(dst_rect);
		return;
	}
#endif
	pixman_color_t pcolor = PixmanColor(color);
	pixman_rectangle16_t rect = {
	static_cast<int16_t>(dst_rect.x),
	static_cast<int16_t>(dst_rect.y),
	static_cast<uint16_t>(dst_rect.width),
	static_cast<uint16_t>(dst_rect.height)};

	pixman_image_fill_rectangles(PIXMAN_OP_OVER, bitmap, &pcolor, 1, &rect);

#ifdef EASYRPG_SYMBIAN
	Touch(dst_rect);
#else
	RefreshCallback();
#endif
}

void Bitmap::Clear() {
#ifdef EASYRPG_SYMBIAN
	unsigned int signature = SymbianDrawSignature(ESymbianClear);
	if (SymbianRecordDraw(this, signature, GetRect())) {
		SymbianRenderDamage::RequireFullPaint();
		return;
	}
	if (bpp() == 2) {
		SymbianFill565(*this, GetRect(), 0);
		Touch(GetRect());
		return;
	}
#endif
	memset(pixels(), '\0', height() * pitch());

#ifdef EASYRPG_SYMBIAN
	Touch(GetRect());
#else
	RefreshCallback();
#endif
}

void Bitmap::ClearRect(Rect const& dst_rect) {
#ifdef EASYRPG_SYMBIAN
	unsigned int signature = SymbianDrawSignature(ESymbianClearRect);
	signature = SymbianHashRect(signature, dst_rect);
	if (SymbianRecordDraw(this, signature, dst_rect))
		return;
	if (bpp() == 2) {
		SymbianFill565(*this, dst_rect, 0);
		Touch(dst_rect);
		return;
	}
#endif
	pixman_color_t pcolor = {0, 0, 0, 0};
	pixman_rectangle16_t rect = {
		static_cast<int16_t>(dst_rect.x),
		static_cast<int16_t>(dst_rect.y),
		static_cast<uint16_t>(dst_rect.width),
		static_cast<uint16_t>(dst_rect.height)
	};

	pixman_image_fill_rectangles(PIXMAN_OP_CLEAR, bitmap, &pcolor, 1, &rect);

#ifdef EASYRPG_SYMBIAN
	Touch(dst_rect);
#else
	RefreshCallback();
#endif
}

static uint8_t HardLightValue(int aSource, int aDestination) {
	int result = aSource <= 128
		? (2 * aSource * aDestination) / 255
		: 255 - (2 * (255 - aSource) * (255 - aDestination)) / 255;
	if (result < 0)
		result = 0;
	else if (result > 255)
		result = 255;
	return static_cast<uint8_t>(result);
}

void Bitmap::ToneBlit(int x, int y, Bitmap const& src, Rect const& src_rect, const Tone &tone, Opacity const& opacity) {
	if (tone == Tone(128,128,128,128)) {
		if (&src != this) {
			Blit(x, y, src, src_rect, opacity);
		}
		return;
	}
#ifdef EASYRPG_SYMBIAN
	unsigned int signature = SymbianDrawSignature(ESymbianToneBlit);
	signature = SymbianHashBitmap(signature, src);
	signature = SymbianHashRect(signature, src_rect);
	signature = SymbianHashTone(signature, tone);
	signature = SymbianHashOpacity(signature, opacity);
	if (SymbianRecordDraw(this, signature, Rect(x, y, src_rect.width, src_rect.height))) {
		if (&src == this)
			SymbianRenderDamage::RequireFullPaint();
		return;
	}
#endif

	if (&src != this) {
#ifdef EASYRPG_SYMBIAN
		if (pixman_format == PIXMAN_r5g6b5) {
			Rect written;
			if (!SymbianBlitTo565(pixels(), pitch(), width(), height(),
				src.pixels(), src.pitch(), src.width(), src.height(), src.pixman_format,
				x, y, src_rect, 255, written)) {
				pixman_image_composite32(src.GetOperator(), src.bitmap, (pixman_image_t*)NULL, bitmap,
					src_rect.x, src_rect.y, 0, 0, x, y, src_rect.width, src_rect.height);
			}
		} else
#endif
		{
			pixman_image_composite32(src.GetOperator(), src.bitmap, (pixman_image_t*)NULL, bitmap,
				src_rect.x, src_rect.y, 0, 0, x, y, src_rect.width, src_rect.height);
		}
	}

	const bool apply_color = tone.red != 128 || tone.green != 128 || tone.blue != 128;
	static uint8_t red_lookup[256];
	static uint8_t green_lookup[256];
	static uint8_t blue_lookup[256];
	static int cached_red = -1;
	static int cached_green = -1;
	static int cached_blue = -1;
	if (apply_color &&
		(cached_red != tone.red || cached_green != tone.green || cached_blue != tone.blue)) {
		for (int value = 0; value < 256; ++value) {
			red_lookup[value] = HardLightValue(tone.red, value);
			green_lookup[value] = HardLightValue(tone.green, value);
			blue_lookup[value] = HardLightValue(tone.blue, value);
		}
		cached_red = tone.red;
		cached_green = tone.green;
		cached_blue = tone.blue;
	}

#ifdef EASYRPG_SYMBIAN
	if (bpp() == 2) {
		int sat = 1024;
		if (tone.gray > 128)
			sat = 1024 + (tone.gray - 128) * 16;
		else if (tone.gray < 128)
			sat = tone.gray * 8;

		Rect effect_rect(x, y, src_rect.width, src_rect.height);
		effect_rect.Adjust(GetRect());
		const Rect* paint_clip = SymbianRenderDamage::PaintClipRect();
		if (paint_clip)
			effect_rect.Adjust(*paint_clip);
		uint8_t* base = static_cast<uint8_t*>(pixels());
		const int destination_pitch = pitch();
		for (int py = effect_rect.y; py < effect_rect.y + effect_rect.height; ++py) {
			uint16_t* row = reinterpret_cast<uint16_t*>(base + py * destination_pitch);
			for (int px = effect_rect.x; px < effect_rect.x + effect_rect.width; ++px) {
				const uint16_t pixel = row[px];
				int red = (pixel >> 11) & 31;
				int green = (pixel >> 5) & 63;
				int blue = pixel & 31;
				red = (red << 3) | (red >> 2);
				green = (green << 2) | (green >> 4);
				blue = (blue << 3) | (blue >> 2);

				if (tone.gray != 128) {
					const int lum = (7471 * blue + 38470 * green + 19595 * red) >> 16;
					red = (lum * 1024 + (red - lum) * sat) >> 10;
					green = (lum * 1024 + (green - lum) * sat) >> 10;
					blue = (lum * 1024 + (blue - lum) * sat) >> 10;
					red = red > 255 ? 255 : red < 0 ? 0 : red;
					green = green > 255 ? 255 : green < 0 ? 0 : green;
					blue = blue > 255 ? 255 : blue < 0 ? 0 : blue;
				}

				if (apply_color) {
					red = red_lookup[red];
					green = green_lookup[green];
					blue = blue_lookup[blue];
				}

				row[px] = static_cast<uint16_t>(((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3));
			}
		}
		Touch(effect_rect);
		return;
	}
#endif

	if (tone.gray != 128) {
		uint32_t* pixels = (uint32_t*)this->pixels();

		int sat;
		if (tone.gray > 128) {
			sat = 1024 + (tone.gray - 128) * 16;
		}
		else {
			sat = tone.gray * 8;
		}

		int as = pixel_format.a.shift;
		int rs = pixel_format.r.shift;
		int gs = pixel_format.g.shift;
		int bs = pixel_format.b.shift;

		// Move according to x/y value
		pixels = pixels + (y * pitch() / sizeof(uint32_t) + x) - (pitch() / sizeof(uint32_t));

		// Algorithm from OpenPDN (MIT license)
		// Transformation in Y'CbCr color space
		for (int i = 0; i < src_rect.height; ++i) {
			// Advance one pixel row
			pixels += pitch() / sizeof(uint32_t);

			for (int j = 0; j < src_rect.width; ++j) {
				uint32_t pixel = pixels[j];
				uint8_t a = (pixel >> as) & 0xFF;
				// &src != this works around a corner case with opacity split (character in a bush)
				// in that case a == 0 and the effect is not applied
				if (a == 0 && &src != this) {
					continue;
				}
				uint8_t r = (pixel >> rs) & 0xFF;
				uint8_t g = (pixel >> gs) & 0xFF;
				uint8_t b = (pixel >> bs) & 0xFF;
				// Y' = 0.299 R' + 0.587 G' + 0.114 B'
				uint8_t lum = (7471 * b + 38470 * g + 19595 * r) >> 16;
				// Scale Cb/Cr by scale factor "sat"
				int red = ((lum * 1024 + (r - lum) * sat) >> 10);
				red = red > 255 ? 255 : red < 0 ? 0 : red;
				int green = ((lum * 1024 + (g - lum) * sat) >> 10);
				green = green > 255 ? 255 : green < 0 ? 0 : green;
				int blue = ((lum * 1024 + (b - lum) * sat) >> 10);
				blue = blue > 255 ? 255 : blue < 0 ? 0 : blue;
				pixels[j] = ((uint32_t) red << rs) | ((uint32_t) green << gs) | ((uint32_t) blue << bs) |
							((uint32_t) a << as);
			}
		}
	}

	if (apply_color) {
		int as = pixel_format.a.shift;
		int rs = pixel_format.r.shift;
		int gs = pixel_format.g.shift;
		int bs = pixel_format.b.shift;

		uint32_t* pixels = (uint32_t*)this->pixels();
		// Move according to x/y value
		pixels = pixels + (y * pitch() / sizeof(uint32_t) + x) - (pitch() / sizeof(uint32_t));

		for (int i = 0; i < src_rect.height; ++i) {
			// Advance one pixel row
			pixels += pitch() / sizeof(uint32_t);

			for (int j = 0; j < src_rect.width; ++j) {
				uint32_t pixel = pixels[j];
				uint8_t a = (pixel >> as) & 0xFF;
				if (a == 0 && &src != this) {
					continue;
				}
				uint8_t r = (pixel >> rs) & 0xFF;
				uint8_t g = (pixel >> gs) & 0xFF;
				uint8_t b = (pixel >> bs) & 0xFF;

				int red = red_lookup[r];
				int green = green_lookup[g];
				int blue = blue_lookup[b];
				pixels[j] = ((uint32_t) red << rs) | ((uint32_t) green << gs) | ((uint32_t) blue << bs) |
							((uint32_t) a << as);
			}
		}
	}

	RefreshCallback();
}

void Bitmap::BlendBlit(int x, int y, Bitmap const& src, Rect const& src_rect, const Color& color, Opacity const& opacity) {
	if (color.alpha == 0) {
		if (&src != this)
			Blit(x, y, src, src_rect, opacity);
		return;
	}
#ifdef EASYRPG_SYMBIAN
	unsigned int signature = SymbianDrawSignature(ESymbianBlendBlit);
	signature = SymbianHashBitmap(signature, src);
	signature = SymbianHashRect(signature, src_rect);
	signature = SymbianHashColor(signature, color);
	signature = SymbianHashOpacity(signature, opacity);
	if (SymbianRecordDraw(this, signature, Rect(x, y, src_rect.width, src_rect.height))) {
		if (&src == this)
			SymbianRenderDamage::RequireFullPaint();
		return;
	}
	if (&src != this && opacity.IsOpaque() && bpp() == 4 && src.pixman_format == PIXMAN_a1r5g5b5) {
		int sx = src_rect.x;
		int sy = src_rect.y;
		int dx = x;
		int dy = y;
		int copy_width = src_rect.width;
		int copy_height = src_rect.height;
		if (sx < 0) { dx -= sx; copy_width += sx; sx = 0; }
		if (sy < 0) { dy -= sy; copy_height += sy; sy = 0; }
		if (dx < 0) { sx -= dx; copy_width += dx; dx = 0; }
		if (dy < 0) { sy -= dy; copy_height += dy; dy = 0; }
		copy_width = std::min(copy_width, src.width() - sx);
		copy_height = std::min(copy_height, src.height() - sy);
		copy_width = std::min(copy_width, width() - dx);
		copy_height = std::min(copy_height, height() - dy);
		if (copy_width > 0 && copy_height > 0) {
			const unsigned int alpha = static_cast<unsigned int>(color.alpha);
			const unsigned int inverse = 255u - alpha;
			uint8_t* destination_base = static_cast<uint8_t*>(pixels());
			const uint8_t* source_base = static_cast<const uint8_t*>(src.pixels());
			for (int row = 0; row < copy_height; ++row) {
				uint32_t* destination = reinterpret_cast<uint32_t*>(destination_base + (dy + row) * pitch()) + dx;
				const uint16_t* source = reinterpret_cast<const uint16_t*>(source_base + (sy + row) * src.pitch()) + sx;
				for (int column = 0; column < copy_width; ++column) {
					const uint16_t pixel = source[column];
					if ((pixel & 0x8000u) == 0)
						continue;
					unsigned int red = (pixel >> 10) & 31u;
					unsigned int green = (pixel >> 5) & 31u;
					unsigned int blue = pixel & 31u;
					red = (red << 3) | (red >> 2);
					green = (green << 3) | (green >> 2);
					blue = (blue << 3) | (blue >> 2);
					red = SymbianMul255(red, inverse) + SymbianMul255(static_cast<unsigned int>(color.red), alpha);
					green = SymbianMul255(green, inverse) + SymbianMul255(static_cast<unsigned int>(color.green), alpha);
					blue = SymbianMul255(blue, inverse) + SymbianMul255(static_cast<unsigned int>(color.blue), alpha);
					destination[column] = format.rgba_to_uint32_t(static_cast<uint8_t>(red), static_cast<uint8_t>(green), static_cast<uint8_t>(blue), 255);
				}
			}
			Touch(Rect(dx, dy, copy_width, copy_height));
		}
		return;
	}
#endif

	if (&src != this)
		pixman_image_composite32(src.GetOperator(),
								 src.bitmap, (pixman_image_t*) NULL, bitmap,
								 src_rect.x, src_rect.y,
								 0, 0,
								 x, y,
								 src_rect.width, src_rect.height);

	pixman_color_t tcolor = PixmanColor(color);
	pixman_image_t* timage = pixman_image_create_solid_fill(&tcolor);

	pixman_image_composite32(src.GetOperator(),
							 timage, src.bitmap, bitmap,
							 0, 0,
							 src_rect.x, src_rect.y,
							 x, y,
							 src_rect.width, src_rect.height);

	pixman_image_unref(timage);

	RefreshCallback();
}

void Bitmap::FlipBlit(int x, int y, Bitmap const& src, Rect const& src_rect, bool horizontal, bool vertical, Opacity const& opacity) {
	if (!horizontal && !vertical) {
		Blit(x, y, src, src_rect, opacity);
		return;
	}
#ifdef EASYRPG_SYMBIAN
	unsigned int signature = SymbianDrawSignature(ESymbianFlipBlit);
	signature = SymbianHashBitmap(signature, src);
	signature = SymbianHashRect(signature, src_rect);
	signature = SymbianHashInt(signature, horizontal ? 1 : 0);
	signature = SymbianHashInt(signature, vertical ? 1 : 0);
	signature = SymbianHashOpacity(signature, opacity);
	if (SymbianRecordDraw(this, signature, Rect(x, y, src_rect.width, src_rect.height)))
		return;
	if (!opacity.IsSplit() && this != &src && pixman_format == PIXMAN_r5g6b5) {
		Rect written;
		if (SymbianScaleTo565(pixels(), pitch(), width(), height(),
			src.pixels(), src.pitch(), src.width(), src.height(), src.pixman_format,
			Rect(x, y, src_rect.width, src_rect.height), src_rect, opacity.Value(),
			horizontal, vertical, written)) {
			if (!written.IsEmpty())
				Touch(written);
			return;
		}
	}
#endif

	pixman_transform_t xform;
	pixman_transform_init_scale(&xform,
								pixman_int_to_fixed(horizontal ? -1 : 1),
								pixman_int_to_fixed(vertical ? -1 : 1));

	pixman_transform_translate((pixman_transform_t*) NULL, &xform,
							   pixman_int_to_fixed(horizontal ? src.GetWidth() : 0),
							   pixman_int_to_fixed(vertical ? src.GetHeight() : 0));

	pixman_image_set_transform(src.bitmap, &xform);

	pixman_image_composite32(src.GetOperator(),
							 src.bitmap, (pixman_image_t*) NULL, bitmap,
							 horizontal ? src.GetWidth() - src_rect.x - src_rect.width : src_rect.x,
							 vertical ? src.GetHeight() - src_rect.y - src_rect.height : src_rect.y,
							 0, 0,
							 x, y,
							 src_rect.width, src_rect.height);

	pixman_transform_init_identity(&xform);
	pixman_image_set_transform(src.bitmap, &xform);

	RefreshCallback();
}

void Bitmap::Flip(const Rect& dst_rect, bool horizontal, bool vertical) {
	if (!horizontal && !vertical)
		return;

	BitmapRef resampled(new Bitmap(dst_rect.width, dst_rect.height, GetTransparent()));

	resampled->FlipBlit(0, 0, *this, dst_rect, horizontal, vertical, Opacity::opaque);

	pixman_image_composite32(GetOperator(),
							 resampled->bitmap, (pixman_image_t*) NULL, bitmap,
							 0, 0,
							 0, 0,
							 dst_rect.x, dst_rect.y,
							 dst_rect.width, dst_rect.height);

	RefreshCallback();
}

void Bitmap::MaskedBlit(Rect const& dst_rect, Bitmap const& mask, int mx, int my, Color const& color) {
#ifdef EASYRPG_SYMBIAN
	unsigned int signature = SymbianDrawSignature(ESymbianMaskedColor);
	signature = SymbianHashBitmap(signature, mask);
	signature = SymbianHashRect(signature, dst_rect);
	signature = SymbianHashInt(signature, mx);
	signature = SymbianHashInt(signature, my);
	signature = SymbianHashColor(signature, color);
	if (SymbianRecordDraw(this, signature, dst_rect))
		return;
#endif
	pixman_color_t tcolor = {
		static_cast<uint16_t>(color.red << 8),
		static_cast<uint16_t>(color.green << 8),
		static_cast<uint16_t>(color.blue << 8),
		static_cast<uint16_t>(color.alpha << 8)};

	pixman_image_t* source = pixman_image_create_solid_fill(&tcolor);

	pixman_image_composite32(PIXMAN_OP_OVER,
							 source, mask.bitmap, bitmap,
							 0, 0,
							 mx, my,
							 dst_rect.x, dst_rect.y,
							 dst_rect.width, dst_rect.height);

	pixman_image_unref(source);

	RefreshCallback();
}

void Bitmap::MaskedBlit(Rect const& dst_rect, Bitmap const& mask, int mx, int my, Bitmap const& src, int sx, int sy) {
#ifdef EASYRPG_SYMBIAN
	unsigned int signature = SymbianDrawSignature(ESymbianMaskedBitmap);
	signature = SymbianHashBitmap(signature, mask);
	signature = SymbianHashBitmap(signature, src);
	signature = SymbianHashRect(signature, dst_rect);
	signature = SymbianHashInt(signature, mx);
	signature = SymbianHashInt(signature, my);
	signature = SymbianHashInt(signature, sx);
	signature = SymbianHashInt(signature, sy);
	if (SymbianRecordDraw(this, signature, dst_rect))
		return;
#endif
	pixman_image_composite32(PIXMAN_OP_OVER,
							 src.bitmap, mask.bitmap, bitmap,
							 sx, sy,
							 mx, my,
							 dst_rect.x, dst_rect.y,
							 dst_rect.width, dst_rect.height);

	RefreshCallback();
}

void Bitmap::Blit2x(Rect const& dst_rect, Bitmap const& src, Rect const& src_rect) {
#ifdef EASYRPG_SYMBIAN
	unsigned int signature = SymbianDrawSignature(ESymbianBlit2x);
	signature = SymbianHashBitmap(signature, src);
	signature = SymbianHashRect(signature, src_rect);
	signature = SymbianHashRect(signature, dst_rect);
	if (SymbianRecordDraw(this, signature, dst_rect))
		return;
#endif
	pixman_transform_t xform;
	pixman_transform_init_scale(&xform,
								pixman_double_to_fixed(0.5),
								pixman_double_to_fixed(0.5));

	pixman_image_set_transform(src.bitmap, &xform);

	pixman_image_composite32(PIXMAN_OP_SRC,
							 src.bitmap, (pixman_image_t*) NULL, bitmap,
							 src_rect.x, src_rect.y,
							 0, 0,
							 dst_rect.x, dst_rect.y,
							 dst_rect.width, dst_rect.height);

	pixman_transform_init_identity(&xform);
	pixman_image_set_transform(src.bitmap, &xform);

	RefreshCallback();
}

Color Bitmap::GetColor(uint32_t uint32_color) const {
	uint8_t r, g, b, a;
	GetColorComponents(uint32_color, r, g, b, a);
	return Color(r, g, b, a);
}

uint32_t Bitmap::GetUint32Color(const Color &color) const {
	return GetUint32Color(color.red, color.green, color.blue, color.alpha);
}

uint32_t Bitmap::GetUint32Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) const {
	MultiplyAlpha(r, g, b, a);
#ifdef EASYRPG_SYMBIAN
	return format.rgba_to_uint32_t(r, g, b, a);
#else
	return pixel_format.rgba_to_uint32_t(r, g, b, a);
#endif
}

void Bitmap::GetColorComponents(uint32_t color, uint8_t &r, uint8_t &g, uint8_t &b, uint8_t &a) const {
#ifdef EASYRPG_SYMBIAN
	format.uint32_to_rgba(color, r, g, b, a);
#else
	pixel_format.uint32_to_rgba(color, r, g, b, a);
#endif
	DivideAlpha(r, g, b, a);
}

pixman_op_t Bitmap::GetOperator(pixman_image_t* mask) const {
	if (!mask && (!GetTransparent() || GetOpacity() == Opaque)) {
		return PIXMAN_OP_SRC;
	}

	return PIXMAN_OP_OVER;
}
