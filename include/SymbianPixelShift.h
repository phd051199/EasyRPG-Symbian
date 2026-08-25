#ifndef EASYRPG_SYMBIAN_PIXEL_SHIFT_H
#define EASYRPG_SYMBIAN_PIXEL_SHIFT_H

#include <cstring>
#include <stdint.h>

// Shift a packed pixel rectangle in place. Returns false when the shift
// cannot be performed (no pixels, bad depth, or shift larger than surface).
static bool SymbianShiftPixels(uint8_t* pixels,
		int width,
		int height,
		int pitch,
		int bytes,
		int shift_x,
		int shift_y) {
	if (shift_x == 0 && shift_y == 0)
		return true;
	if (!pixels || bytes <= 0)
		return false;

	const int abs_x = shift_x < 0 ? -shift_x : shift_x;
	const int abs_y = shift_y < 0 ? -shift_y : shift_y;
	if (abs_x >= width || abs_y >= height)
		return false;

	const int source_x = shift_x < 0 ? -shift_x : 0;
	const int destination_x = shift_x > 0 ? shift_x : 0;
	const int source_y = shift_y < 0 ? -shift_y : 0;
	const int destination_y = shift_y > 0 ? shift_y : 0;
	const int copy_width = width - abs_x;
	const int copy_height = height - abs_y;
	const size_t row_bytes = static_cast<size_t>(copy_width) * static_cast<size_t>(bytes);
	const size_t packed_row_bytes = static_cast<size_t>(width) * static_cast<size_t>(bytes);

	if (shift_y == 0 && static_cast<size_t>(pitch) == packed_row_bytes) {
		const size_t shift_bytes = static_cast<size_t>(abs_x) * static_cast<size_t>(bytes);
		const size_t move_bytes = packed_row_bytes * static_cast<size_t>(height) - shift_bytes;
		std::memmove(pixels + (shift_x > 0 ? shift_bytes : 0),
			pixels + (shift_x < 0 ? shift_bytes : 0),
			move_bytes);
		return true;
	}

	if (shift_x == 0) {
		std::memmove(pixels + destination_y * pitch,
			pixels + source_y * pitch,
			static_cast<size_t>(copy_height) * static_cast<size_t>(pitch));
		return true;
	}

	if (destination_y > source_y) {
		for (int row = copy_height - 1; row >= 0; --row) {
			std::memmove(pixels + (destination_y + row) * pitch + destination_x * bytes,
				pixels + (source_y + row) * pitch + source_x * bytes,
				row_bytes);
		}
	} else {
		for (int row = 0; row < copy_height; ++row) {
			std::memmove(pixels + (destination_y + row) * pitch + destination_x * bytes,
				pixels + (source_y + row) * pitch + source_x * bytes,
				row_bytes);
		}
	}
	return true;
}

#endif
