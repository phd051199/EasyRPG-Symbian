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
#include <algorithm>
#include <cstring>
#include "tilemap_layer.h"
#include "graphics.h"
#include "output.h"
#include "player.h"
#include "map_data.h"
#include "bitmap.h"
#ifdef EASYRPG_SYMBIAN
#include "RenderDamage.h"
#endif

// Blocks subtiles IDs
// Mess with this code and you will die in 3 days...
// [tile-id][row][col]
static const int8_t BlockA_Subtiles_IDS[47][2][2] = {
#define N -1
	{{N, N}, {N, N}},
	{{3, N}, {N, N}},
	{{N, 3}, {N, N}},
	{{3, 3}, {N, N}},
	{{N, N}, {N, 3}},
	{{3, N}, {N, 3}},
	{{N, 3}, {N, 3}},
	{{3, 3}, {N, 3}},
	{{N, N}, {3, N}},
	{{3, N}, {3, N}},
	{{N, 3}, {3, N}},
	{{3, 3}, {3, N}},
	{{N, N}, {3, 3}},
	{{3, N}, {3, 3}},
	{{N, 3}, {3, 3}},
	{{3, 3}, {3, 3}},
	{{1, N}, {1, N}},
	{{1, 3}, {1, N}},
	{{1, N}, {1, 3}},
	{{1, 3}, {1, 3}},
	{{2, 2}, {N, N}},
	{{2, 2}, {N, 3}},
	{{2, 2}, {3, N}},
	{{2, 2}, {3, 3}},
	{{N, 1}, {N, 1}},
	{{N, 1}, {3, 1}},
	{{3, 1}, {N, 1}},
	{{3, 1}, {3, 1}},
	{{N, N}, {2, 2}},
	{{3, N}, {2, 2}},
	{{N, 3}, {2, 2}},
	{{3, 3}, {2, 2}},
	{{1, 1}, {1, 1}},
	{{2, 2}, {2, 2}},
	{{0, 2}, {1, N}},
	{{0, 2}, {1, 3}},
	{{2, 0}, {N, 1}},
	{{2, 0}, {3, 1}},
	{{N, 1}, {2, 0}},
	{{3, 1}, {2, 0}},
	{{1, N}, {0, 2}},
	{{1, 3}, {0, 2}},
	{{0, 0}, {1, 1}},
	{{0, 2}, {0, 2}},
	{{1, 1}, {0, 0}},
	{{2, 0}, {2, 0}},
	{{0, 0}, {0, 0}}
#undef N
};

// [tile-id][row][col][x/y]
static const uint8_t BlockD_Subtiles_IDS[50][2][2][2] = {
//     T-L     T-R       B-L     B-R
    {{{1, 2}, {1, 2}}, {{1, 2}, {1, 2}}},
    {{{2, 0}, {1, 2}}, {{1, 2}, {1, 2}}},
    {{{1, 2}, {2, 0}}, {{1, 2}, {1, 2}}},
    {{{2, 0}, {2, 0}}, {{1, 2}, {1, 2}}},
    {{{1, 2}, {1, 2}}, {{1, 2}, {2, 0}}},
    {{{2, 0}, {1, 2}}, {{1, 2}, {2, 0}}},
    {{{1, 2}, {2, 0}}, {{1, 2}, {2, 0}}},
    {{{2, 0}, {2, 0}}, {{1, 2}, {2, 0}}},
    {{{1, 2}, {1, 2}}, {{2, 0}, {1, 2}}},
    {{{2, 0}, {1, 2}}, {{2, 0}, {1, 2}}},
    {{{1, 2}, {2, 0}}, {{2, 0}, {1, 2}}},
    {{{2, 0}, {2, 0}}, {{2, 0}, {1, 2}}},
    {{{1, 2}, {1, 2}}, {{2, 0}, {2, 0}}},
    {{{2, 0}, {1, 2}}, {{2, 0}, {2, 0}}},
    {{{1, 2}, {2, 0}}, {{2, 0}, {2, 0}}},
    {{{2, 0}, {2, 0}}, {{2, 0}, {2, 0}}},
    {{{0, 2}, {0, 2}}, {{0, 2}, {0, 2}}},
    {{{0, 2}, {2, 0}}, {{0, 2}, {0, 2}}},
    {{{0, 2}, {0, 2}}, {{0, 2}, {2, 0}}},
    {{{0, 2}, {2, 0}}, {{0, 2}, {2, 0}}},
    {{{1, 1}, {1, 1}}, {{1, 1}, {1, 1}}},
    {{{1, 1}, {1, 1}}, {{1, 1}, {2, 0}}},
    {{{1, 1}, {1, 1}}, {{2, 0}, {1, 1}}},
    {{{1, 1}, {1, 1}}, {{2, 0}, {2, 0}}},
    {{{2, 2}, {2, 2}}, {{2, 2}, {2, 2}}},
    {{{2, 2}, {2, 2}}, {{2, 0}, {2, 2}}},
    {{{2, 0}, {2, 2}}, {{2, 2}, {2, 2}}},
    {{{2, 0}, {2, 2}}, {{2, 0}, {2, 2}}},
    {{{1, 3}, {1, 3}}, {{1, 3}, {1, 3}}},
    {{{2, 0}, {1, 3}}, {{1, 3}, {1, 3}}},
    {{{1, 3}, {2, 0}}, {{1, 3}, {1, 3}}},
    {{{2, 0}, {2, 0}}, {{1, 3}, {1, 3}}},
    {{{0, 2}, {2, 2}}, {{0, 2}, {2, 2}}},
    {{{1, 1}, {1, 1}}, {{1, 3}, {1, 3}}},
    {{{0, 1}, {0, 1}}, {{0, 1}, {0, 1}}},
    {{{0, 1}, {0, 1}}, {{0, 1}, {2, 0}}},
    {{{2, 1}, {2, 1}}, {{2, 1}, {2, 1}}},
    {{{2, 1}, {2, 1}}, {{2, 0}, {2, 1}}},
    {{{2, 3}, {2, 3}}, {{2, 3}, {2, 3}}},
    {{{2, 0}, {2, 3}}, {{2, 3}, {2, 3}}},
    {{{0, 3}, {0, 3}}, {{0, 3}, {0, 3}}},
    {{{0, 3}, {2, 0}}, {{0, 3}, {0, 3}}},
    {{{0, 1}, {2, 1}}, {{0, 1}, {2, 1}}},
    {{{0, 1}, {0, 1}}, {{0, 3}, {0, 3}}},
    {{{0, 3}, {2, 3}}, {{0, 3}, {2, 3}}},
    {{{2, 1}, {2, 1}}, {{2, 3}, {2, 3}}},
    {{{0, 1}, {2, 1}}, {{0, 3}, {2, 3}}},
    {{{1, 2}, {1, 2}}, {{1, 2}, {1, 2}}},
    {{{1, 2}, {1, 2}}, {{1, 2}, {1, 2}}},
    {{{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}}
};

TilemapLayer::TilemapLayer(int ilayer) :
	visible(true),
	ox(0),
	oy(0),
	width(0),
	height(0),
	animation_frame(0),
	animation_step_ab(0),
	animation_step_c(0),
	animation_speed(24),
	animation_type(0),
	layer(ilayer) {

	memset(autotiles_ab, 0, sizeof(autotiles_ab));
	memset(autotiles_d, 0, sizeof(autotiles_d));

	// SubLayer for the tiles with Wall or Above passability
	// Its z-value should be between the z of the events in the upper layer and the hero
	sublayers.push_back(std::make_shared<TilemapSubLayer>(this, 98+layer));
	// SubLayer for the tiles without Wall or Above passability
	// Its z-value should be under z of the events in the lower layer
	sublayers.push_back(std::make_shared<TilemapSubLayer>(this, -2+layer));
}

#ifdef EASYRPG_SYMBIAN
void TilemapLayer::SymbianTouch() {
	++symbian_revision;
	if (symbian_revision == 0)
		symbian_revision = 1;
	SymbianRenderDamage::RequestFullFrame();
}

unsigned int TilemapLayer::SymbianSignature(int z_order) const {
	unsigned int hash = SymbianRenderDamage::HashSeed(static_cast<unsigned int>(reinterpret_cast<size_t>(this)));
	hash = SymbianRenderDamage::HashValue(hash, symbian_revision);
	hash = SymbianRenderDamage::HashValue(hash, static_cast<unsigned int>(z_order));
	if (chipset) {
		hash = SymbianRenderDamage::HashValue(hash, static_cast<unsigned int>(reinterpret_cast<size_t>(chipset.get())));
		hash = SymbianRenderDamage::HashValue(hash, chipset->Revision());
	}
	return hash;
}
#endif

static int WrapTileIndex(int value, int size) {
	if (value >= 0 && value < size)
		return value;
	if (value < 0) {
		value += size;
		if (value >= 0)
			return value;
	} else {
		value -= size;
		if (value < size)
			return value;
	}
	value %= size;
	return value < 0 ? value + size : value;
}

static Bitmap& ToneTile(BitmapRef& aEffect,
	std::vector<uint8_t>& aCachedTiles,
	const BitmapRef& aSource,
	int aRow,
	int aCol,
	const Tone& aTone) {
	if (aTone == Tone())
		return *aSource;
	if (!aEffect) {
		aEffect = Bitmap::Create(aSource->width(), aSource->height());
		aCachedTiles.assign((aSource->width() / TILE_SIZE) * (aSource->height() / TILE_SIZE), 0);
	}
	const int columns = aSource->width() / TILE_SIZE;
	const int index = aRow * columns + aCol;
	if (!aCachedTiles[index]) {
		Rect rect(aCol * TILE_SIZE, aRow * TILE_SIZE, TILE_SIZE, TILE_SIZE);
		aEffect->ClearRect(rect);
		aEffect->ToneBlit(rect.x, rect.y, *aSource, rect, aTone, Opacity::opaque);
		aCachedTiles[index] = 1;
	}
	return *aEffect;
}

void TilemapLayer::DrawTile(Bitmap& destination, Bitmap& screen, int x, int y, int row, int col, int opacity) {
	const Bitmap::TileOpacity op = static_cast<Bitmap::TileOpacity>(opacity);
	if (!fast_blit && op == Bitmap::Transparent)
		return;
	const int source_x = col * TILE_SIZE;
	const int source_y = row * TILE_SIZE;
	if (op == Bitmap::Opaque)
		destination.BlitTileFast565(x, y, screen, source_x, source_y);
	else {
		Rect rect(source_x, source_y, TILE_SIZE, TILE_SIZE);
		if (fast_blit)
			destination.BlitFast(x, y, screen, rect, 255);
		else
			destination.Blit(x, y, screen, rect, 255);
	}
}

void TilemapLayer::Draw(int z_order) {
	if (!visible || width <= 0 || height <= 0)
		return;

	const int display_width = DisplayUi->GetWidth();
	const int display_height = DisplayUi->GetHeight();
	const int offset_x = ox % TILE_SIZE;
	const int offset_y = oy % TILE_SIZE;
	int tiles_x = (display_width + TILE_SIZE - 1) / TILE_SIZE;
	int tiles_y = (display_height + TILE_SIZE - 1) / TILE_SIZE;
	if (offset_x != 0)
		++tiles_x;
	if (offset_y != 0)
		++tiles_y;
#ifdef EASYRPG_SYMBIAN
	if (SymbianRenderDamage::IsFullPaintRecording() && layer == 0) {
		symbian_recorded_animation_step_ab = animation_step_ab;
		symbian_recorded_animation_step_c = animation_step_c;
	}
	if (SymbianRenderDamage::IsRecording()) {
		SymbianRenderDamage::Record(SymbianSignature(z_order), Rect(0, 0, display_width, display_height));
		const bool dirty_ab = layer == 0 && symbian_has_animated_ab &&
			symbian_recorded_animation_step_ab != animation_step_ab;
		const bool dirty_c = layer == 0 && symbian_has_animated_c &&
			symbian_recorded_animation_step_c != animation_step_c;
		if (dirty_ab || dirty_c) {
			const int first_map_y = WrapTileIndex(oy / TILE_SIZE, height);
			int map_x = WrapTileIndex(ox / TILE_SIZE, width);
			int map_draw_x = -offset_x;
			for (int x = 0; x < tiles_x; ++x) {
				int map_y = first_map_y;
				int map_draw_y = -offset_y;
				for (int y = 0; y < tiles_y; ++y) {
					const TileData& tile = data_cache[map_x][map_y];
					if (tile.z == z_order &&
						((dirty_ab && tile.source == SourceAutotileAB) ||
						 (dirty_c && tile.source == SourceChipsetAnimatedC)))
						SymbianRenderDamage::MarkDirty(Rect(map_draw_x, map_draw_y, TILE_SIZE, TILE_SIZE));
					++map_y;
					if (map_y == height)
						map_y = 0;
					map_draw_y += TILE_SIZE;
				}
				++map_x;
				if (map_x == width)
					map_x = 0;
				map_draw_x += TILE_SIZE;
			}
			symbian_recorded_animation_step_ab = animation_step_ab;
			symbian_recorded_animation_step_c = animation_step_c;
		}
		return;
	}
#endif

	BitmapRef const& destination_ref = DisplayUi->GetDisplaySurface();
	if (!destination_ref)
		return;
	Bitmap& destination = *destination_ref;
	int first_tile_x = 0;
	int first_tile_y = 0;
	int last_tile_x = tiles_x;
	int last_tile_y = tiles_y;
#ifdef EASYRPG_SYMBIAN
	if (SymbianRenderDamage::IsPainting()) {
		const Rect& dirty = SymbianRenderDamage::CurrentPaintRect();
		const int base_x = -offset_x;
		const int base_y = -offset_y;
		while (first_tile_x < last_tile_x && base_x + (first_tile_x + 1) * TILE_SIZE <= dirty.x)
			++first_tile_x;
		while (first_tile_y < last_tile_y && base_y + (first_tile_y + 1) * TILE_SIZE <= dirty.y)
			++first_tile_y;
		while (last_tile_x > first_tile_x && base_x + (last_tile_x - 1) * TILE_SIZE >= dirty.x + dirty.width)
			--last_tile_x;
		while (last_tile_y > first_tile_y && base_y + (last_tile_y - 1) * TILE_SIZE >= dirty.y + dirty.height)
			--last_tile_y;
	}
#endif
	const bool neutral_tone = tone == Tone();
	const int first_map_y = WrapTileIndex(oy / TILE_SIZE, height);
	int map_x = WrapTileIndex(ox / TILE_SIZE, width);
	map_x += first_tile_x;
	while (map_x >= width)
		map_x -= width;
	int map_draw_x = -offset_x + first_tile_x * TILE_SIZE;

	for (int x = first_tile_x; x < last_tile_x; ++x) {
		int map_y = first_map_y + first_tile_y;
		while (map_y >= height)
			map_y -= height;
		int map_draw_y = -offset_y + first_tile_y * TILE_SIZE;
		for (int y = first_tile_y; y < last_tile_y; ++y) {
			TileData& tile = data_cache[map_x][map_y];
			if (z_order == tile.z) {
				int frame = 0;
				const BitmapRef* source = NULL;
				switch (tile.source) {
				case SourceChipset:
					source = &chipset;
					break;
				case SourceChipsetAnimatedC:
					source = &chipset;
					frame = animation_step_c;
					break;
				case SourceAutotileAB:
					source = &autotiles_ab_screen;
					frame = animation_step_ab;
					break;
				case SourceAutotileD:
					source = &autotiles_d_screen;
					break;
				default:
					break;
				}
				if (source && *source) {
					const int col = tile.source_x[frame];
					const int row = tile.source_y[frame];
					if (neutral_tone) {
						DrawTile(destination, **source, map_draw_x, map_draw_y, row, col, tile.opacity[frame]);
					} else {
						BitmapRef* effect = &chipset_effect;
						std::vector<uint8_t>* tone_tiles = &chipset_tone_tiles;
						if (tile.source == SourceAutotileAB) {
							effect = &autotiles_ab_screen_effect;
							tone_tiles = &autotiles_ab_screen_tone_tiles;
						} else if (tile.source == SourceAutotileD) {
							effect = &autotiles_d_screen_effect;
							tone_tiles = &autotiles_d_screen_tone_tiles;
						}
						Bitmap& tile_screen = ToneTile(*effect, *tone_tiles, *source, row, col, tone);
						DrawTile(destination, tile_screen, map_draw_x, map_draw_y, row, col, tile.opacity[frame]);
					}
				}
			}
			++map_y;
			if (map_y == height)
				map_y = 0;
			map_draw_y += TILE_SIZE;
		}
		++map_x;
		if (map_x == width)
			map_x = 0;
		map_draw_x += TILE_SIZE;
	}
}

TilemapLayer::TileXY TilemapLayer::GetCachedAutotileAB(short ID, short animID) {
	short block = ID / 1000;
	short b_subtile = (ID - block * 1000) / 50;
	short a_subtile = ID - block * 1000 - b_subtile * 50;
	return autotiles_ab[animID][block][b_subtile][a_subtile];
}

TilemapLayer::TileXY TilemapLayer::GetCachedAutotileD(short ID) {
	short block = (ID - 4000) / 50;
	short subtile = ID - 4000 - block * 50;
	return autotiles_d[block][subtile];
}

void TilemapLayer::CreateTileCache(const std::vector<short>& nmap_data) {
	data_cache.resize(width);
	for (int x = 0; x < width; x++) {
		data_cache[x].resize(height);
		for (int y = 0; y < height; y++) {
			TileData tile;
			tile.ID = nmap_data[x + y * width];
			tile.z = 0;
			tile.source = SourceNone;
			memset(tile.source_x, 0, sizeof(tile.source_x));
			memset(tile.source_y, 0, sizeof(tile.source_y));
			memset(tile.opacity, Bitmap::Partial, sizeof(tile.opacity));

			// Calculate the tile Z
			if (!passable.empty()) {
				if (tile.ID >= BLOCK_F) { // Upper layer
					if ((passable[substitutions[tile.ID - BLOCK_F]] & Passable::Above) != 0)
						tile.z = 99; // Upper sublayer
					else
						tile.z = -1; // Lower sublayer

				} else { // Lower layer
					int chip_index =
						tile.ID >= BLOCK_E ? substitutions[tile.ID - BLOCK_E] + 18 :
						tile.ID >= BLOCK_D ? (tile.ID - BLOCK_D) / 50 + 6 :
						tile.ID >= BLOCK_C ? (tile.ID - BLOCK_C) / 50 + 3 :
						tile.ID / 1000;
					if ((passable[chip_index] & (Passable::Wall | Passable::Above)) != 0)
						tile.z = 98; // Upper sublayer
					else
						tile.z = -2; // Lower sublayer

				}
			}
			data_cache[x][y] = tile;
		}
	}
}

void TilemapLayer::PrepareTileDrawCache() {
	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {
			TileData& tile = data_cache[x][y];
			tile.source = SourceNone;
			memset(tile.source_x, 0, sizeof(tile.source_x));
			memset(tile.source_y, 0, sizeof(tile.source_y));
			memset(tile.opacity, Bitmap::Partial, sizeof(tile.opacity));

			if (layer == 0) {
				if (tile.ID >= BLOCK_E && tile.ID < BLOCK_E + BLOCK_E_TILES) {
					const int index = tile.ID - BLOCK_E;
					const int id = index < static_cast<int>(substitutions.size()) ? substitutions[index] : index;
					tile.source = SourceChipset;
					if (id < 96) {
						tile.source_x[0] = static_cast<uint8_t>(12 + id % 6);
						tile.source_y[0] = static_cast<uint8_t>(id / 6);
					} else {
						tile.source_x[0] = static_cast<uint8_t>(18 + (id - 96) % 6);
						tile.source_y[0] = static_cast<uint8_t>((id - 96) / 6);
					}
				} else if (tile.ID >= BLOCK_C && tile.ID < BLOCK_D) {
					tile.source = SourceChipsetAnimatedC;
					const uint8_t col = static_cast<uint8_t>(3 + (tile.ID - BLOCK_C) / 50);
					for (int frame = 0; frame < 4; ++frame) {
						tile.source_x[frame] = col;
						tile.source_y[frame] = static_cast<uint8_t>(4 + frame);
					}
				} else if (tile.ID < BLOCK_C) {
					tile.source = SourceAutotileAB;
					for (int frame = 0; frame < 3; ++frame) {
						const TileXY pos = GetCachedAutotileAB(tile.ID, frame);
						tile.source_x[frame] = pos.x;
						tile.source_y[frame] = pos.y;
					}
				} else if (tile.ID >= BLOCK_D && tile.ID < BLOCK_E) {
					tile.source = SourceAutotileD;
					const TileXY pos = GetCachedAutotileD(tile.ID);
					tile.source_x[0] = pos.x;
					tile.source_y[0] = pos.y;
				}
			} else if (tile.ID >= BLOCK_F && tile.ID < BLOCK_F + BLOCK_F_TILES) {
				const int index = tile.ID - BLOCK_F;
				const int id = index < static_cast<int>(substitutions.size()) ? substitutions[index] : index;
				tile.source = SourceChipset;
				if (id < 48) {
					tile.source_x[0] = static_cast<uint8_t>(18 + id % 6);
					tile.source_y[0] = static_cast<uint8_t>(8 + id / 6);
				} else {
					tile.source_x[0] = static_cast<uint8_t>(24 + (id - 48) % 6);
					tile.source_y[0] = static_cast<uint8_t>((id - 48) / 6);
				}
			}

			const BitmapRef* source = NULL;
			int frames = 1;
			switch (tile.source) {
			case SourceChipset:
				source = &chipset;
				break;
			case SourceChipsetAnimatedC:
				source = &chipset;
				frames = 4;
				break;
			case SourceAutotileAB:
				source = &autotiles_ab_screen;
				frames = 3;
				break;
			case SourceAutotileD:
				source = &autotiles_d_screen;
				break;
			default:
				break;
			}
			if (source && *source) {
				for (int frame = 0; frame < frames; ++frame)
					tile.opacity[frame] = static_cast<uint8_t>((*source)->GetTileOpacity(tile.source_y[frame], tile.source_x[frame]));
			}
		}
	}
}

void TilemapLayer::GenerateAutotileAB(short ID, short animID) {
	// Calculate the block to use
	//	1: A1 + Upper B (Grass + Coast)
	//	2: A2 + Upper B (Snow + Coast)
	//	3: A1 + Lower B (Grass + Ocean/Deep water)
	short block = ID / 1000;

	// Calculate the B block combination
	short b_subtile = (ID - block * 1000) / 50;
	if (b_subtile >= TILE_SIZE) {
		Output::Warning("Invalid AB autotile ID: %d (b_subtile = %d)",
						ID, b_subtile);
		return;
	}

	// Calculate the A block combination
	short a_subtile = ID - block * 1000 - b_subtile * 50;
	if (a_subtile >= 47) {
		Output::Warning("Invalid AB autotile ID: %d (a_subtile = %d)",
						ID, a_subtile);
		return;
	}

	if (autotiles_ab[animID][block][b_subtile][a_subtile].valid)
		return;

	uint8_t quarters[2][2][2];

	// Determine block B subtiles
	for (int j = 0; j < 2; j++) {
		for (int i = 0; i < 2; i++) {
			// Skip the subtile if it will be used one from A block instead
			if (BlockA_Subtiles_IDS[a_subtile][j][i] != -1) continue;

			// Get the block B subtiles ids and get their coordinates on the chipset
			int t = (b_subtile >> (j * 2 + i)) & 1;
			if (block == 2) t ^= 3;

			quarters[j][i][0] = animID;
			quarters[j][i][1] = 4 + t;
		}
	}

	// Determine block A subtiles
	for (int j = 0; j < 2; j++) {
		for (int i = 0; i < 2; i++) {
			// Skip the subtile if it was used one from B block
			if (BlockA_Subtiles_IDS[a_subtile][j][i] == -1) continue;

			// Get the block A subtiles ids and get their coordinates on the chipset
			quarters[j][i][0] = animID + (block == 1 ? 3 : 0);
			quarters[j][i][1] = BlockA_Subtiles_IDS[a_subtile][j][i];
		}
	}

	// Determine block B subtiles when combining A and B
	if (b_subtile != 0 && a_subtile != 0) {
		for (int j = 0; j < 2; j++) {
			for (int i = 0; i < 2; i++) {
				// calculate tile (row 0..3)
				int t = (b_subtile >> (j * 2 + i)) & 1;
				if (block == 2) t *= 2;

				// Skip the subtile if not used
				if (t == 0) continue;

				// Get the coordinates on the chipset
				quarters[j][i][0] = animID;
				quarters[j][i][1] = 4 + t;
			}
		}
	}

	// pack the quarters data into a word
	uint32_t quarters_hash = 0;
	for (int j = 0; j < 2; j++)
		for (int i = 0; i < 2; i++)
			for (int k = 0; k < 2; k++) {
				quarters_hash <<= 4;
				quarters_hash |= quarters[j][i][k];
			}

	// check whether we have already generated this tile
	std::map<uint32_t, TileXY>::iterator it;
	it = autotiles_ab_map.find(quarters_hash);
	if (it != autotiles_ab_map.end()) {
		autotiles_ab[animID][block][b_subtile][a_subtile] = it->second;
		return;
	}

	int id = autotiles_ab_next++;
	int dst_x = id % TILES_PER_ROW;
	int dst_y = id / TILES_PER_ROW;

	TileXY tile_xy(dst_x, dst_y);
	autotiles_ab_map[quarters_hash] = tile_xy;
	autotiles_ab[animID][block][b_subtile][a_subtile] = tile_xy;
}

void TilemapLayer::GenerateAutotileD(short ID) {
	// Calculate the D block id
	short block = (ID - 4000) / 50;

	// Calculate the D block combination
	short subtile = ID - 4000 - block * 50;

	if (block >= 12 || subtile >= 50 || block < 0 || subtile < 0) {
		Output::Warning("Tilemap index out of range: %d %d", block, subtile);
		return;
	}

	if (autotiles_d[block][subtile].valid)
		return;

	uint8_t quarters[2][2][2];

	// Get Block chipset coords
	short block_x, block_y;
	if (block < 4) {
		// If from first column
		block_x = (block % 2) * 3;
		block_y = 8 + (block / 2) * 4;
	} else {
		// If from second column
		block_x = 6 + (block % 2) * 3;
		block_y = ((block - 4) / 2) * 4;
	}

	// Calculate D block subtiles
	for (int j = 0; j < 2; j++) {
		for (int i = 0; i < 2; i++) {
			// Get the block D subtiles ids and get their coordinates on the chipset
			quarters[j][i][0] = block_x + BlockD_Subtiles_IDS[subtile][j][i][0];
			quarters[j][i][1] = block_y + BlockD_Subtiles_IDS[subtile][j][i][1];
		}
	}

	// pack the quarters data into a word
	uint32_t quarters_hash = 0;
	for (int j = 0; j < 2; j++)
		for (int i = 0; i < 2; i++)
			for (int k = 0; k < 2; k++) {
				quarters_hash <<= 4;//multiply 16
				quarters_hash |= quarters[j][i][k];
			}



	// check whether we have already generated this tile
	std::map<uint32_t, TileXY>::iterator it;
	it = autotiles_d_map.find(quarters_hash);
	if (it != autotiles_d_map.end()) {
		autotiles_d[block][subtile] = it->second;
		return;
	}

	int id = autotiles_d_next++;
	int dst_x = id % TILES_PER_ROW;
	int dst_y = id / TILES_PER_ROW;

	TileXY tile_xy(dst_x, dst_y);
	autotiles_d_map[quarters_hash] = tile_xy;
	autotiles_d[block][subtile] = tile_xy;
}

BitmapRef TilemapLayer::GenerateAutotiles(int count, const std::map<uint32_t, TileXY>& map) {
	int rows = (count + TILES_PER_ROW - 1) / TILES_PER_ROW;
	BitmapRef tiles = Bitmap::Create(TILES_PER_ROW * TILE_SIZE, rows * TILE_SIZE, true, 16);
	tiles->Clear();
	Rect rect(0, 0, TILE_SIZE/2, TILE_SIZE/2);

	std::map<uint32_t, TileXY>::const_iterator it;
	for (it = map.begin(); it != map.end(); ++it) {
		uint32_t quarters_hash = it->first;
		TileXY dst = it->second;

		// unpack the quarters data
		for (int j = 0; j < 2; j++) {
			for (int i = 0; i < 2; i++) {
				int x = quarters_hash >> 28;
				quarters_hash <<= 4;
				
				int y = quarters_hash >> 28;
				quarters_hash <<= 4;
				
				rect.x = (x * 2 + i) * (TILE_SIZE/2);
				rect.y = (y * 2 + j) * (TILE_SIZE/2);

				tiles->Blit((dst.x * 2 + i) * (TILE_SIZE / 2), (dst.y * 2 + j) * (TILE_SIZE / 2), *chipset, rect, 255);
			}
		}
	}

	if (rows > 0) {
		tiles->CheckPixels(Bitmap::Flag_Chipset | Bitmap::Flag_ReadOnly);
	}

	return tiles;
}

void TilemapLayer::Update() {
	animation_frame += 1;

	// Step to the next animation frame
	if (animation_frame % 6 == 0) {
		animation_step_c = (animation_step_c + 1) % 4;
	}
	if (animation_frame == animation_speed) {
		animation_step_ab = 1;
	} else if (animation_frame == animation_speed * 2) {
		animation_step_ab = 2;
	} else if (animation_frame == animation_speed * 3) {
		if (animation_type == 0) {
			// If animation type is 1-2-3-2
			animation_step_ab = 1;
		} else {
			// If animation type is 1-2-3
			animation_step_ab = 0;
			animation_frame = 0;
		}
	} else if (animation_frame >= animation_speed * 4) {
		animation_step_ab = 0;
		animation_frame = 0;
	}
}

BitmapRef const& TilemapLayer::GetChipset() const {
	return chipset;
}

void TilemapLayer::SetChipset(BitmapRef const& nchipset) {
	chipset = nchipset;
#ifdef EASYRPG_SYMBIAN
	SymbianTouch();
#endif
	chipset_effect.reset();
	chipset_tone_tiles.clear();

	if (autotiles_ab_next != 0 && autotiles_d_screen != nullptr && layer == 0) {
		autotiles_ab_screen = GenerateAutotiles(autotiles_ab_next, autotiles_ab_map);
		autotiles_d_screen = GenerateAutotiles(autotiles_d_next, autotiles_d_map);
		autotiles_ab_screen_effect.reset();
		autotiles_d_screen_effect.reset();
		autotiles_ab_screen_tone_tiles.clear();
		autotiles_d_screen_tone_tiles.clear();
	}
	if (!data_cache.empty())
		PrepareTileDrawCache();
}

std::vector<short> TilemapLayer::GetMapData() const {
	return map_data;
}

void TilemapLayer::SetMapData(const std::vector<short>& nmap_data) {
#ifdef EASYRPG_SYMBIAN
	symbian_has_animated_ab = false;
	symbian_has_animated_c = false;
	for (std::vector<short>::const_iterator it = nmap_data.begin(); it != nmap_data.end(); ++it) {
		if (*it < BLOCK_C)
			symbian_has_animated_ab = true;
		else if (*it >= BLOCK_C && *it < BLOCK_D)
			symbian_has_animated_c = true;
	}
	SymbianTouch();
#endif
	// Create the tiles data cache
	CreateTileCache(nmap_data);
	memset(autotiles_ab, 0, sizeof(autotiles_ab));
	memset(autotiles_d, 0, sizeof(autotiles_d));

	if (layer == 0) {
		autotiles_ab_map.clear();
		autotiles_d_map.clear();
		autotiles_ab_next = 0;
		autotiles_d_next = 0;
		std::vector<uint8_t> seen_ab(BLOCK_C, 0);
		std::vector<uint8_t> seen_d(BLOCK_E - BLOCK_D, 0);
		for (std::vector<short>::const_iterator it = nmap_data.begin(); it != nmap_data.end(); ++it) {
			const int id = *it;
			if (id >= 0 && id < BLOCK_C) {
				if (seen_ab[id])
					continue;
				seen_ab[id] = 1;
				GenerateAutotileAB(static_cast<short>(id), 0);
				GenerateAutotileAB(static_cast<short>(id), 1);
				GenerateAutotileAB(static_cast<short>(id), 2);
			} else if (id >= BLOCK_D && id < BLOCK_E) {
				const int index = id - BLOCK_D;
				if (seen_d[index])
					continue;
				seen_d[index] = 1;
				GenerateAutotileD(static_cast<short>(id));
			}
		}
		autotiles_ab_screen = GenerateAutotiles(autotiles_ab_next, autotiles_ab_map);
		autotiles_d_screen = GenerateAutotiles(autotiles_d_next, autotiles_d_map);
		autotiles_ab_screen_effect.reset();
		autotiles_d_screen_effect.reset();
		autotiles_ab_screen_tone_tiles.clear();
		autotiles_d_screen_tone_tiles.clear();
	}

	map_data = nmap_data;
	PrepareTileDrawCache();
}

std::vector<unsigned char> TilemapLayer::GetPassable() const {
	return passable;
}

void TilemapLayer::SetPassable(const std::vector<unsigned char>& npassable) {
	passable = npassable;
#ifdef EASYRPG_SYMBIAN
	SymbianTouch();
#endif

	if (substitutions.size() < passable.size())
	{
		substitutions.resize(passable.size());
		for (uint8_t i = 0; i < substitutions.size(); i++)
			substitutions[i] = i;
	}

	// Recalculate z values of all tiles
	CreateTileCache(map_data);
	PrepareTileDrawCache();
}

bool TilemapLayer::GetVisible() const {
	return visible;
}

void TilemapLayer::SetVisible(bool nvisible) {
	visible = nvisible;
}

int TilemapLayer::GetOx() const {
	return ox;
}

void TilemapLayer::SetOx(int nox) {
#ifdef EASYRPG_SYMBIAN
	if (ox != nox)
		SymbianTouch();
#endif
	ox = nox;
}

int TilemapLayer::GetOy() const {
	return oy;
}

void TilemapLayer::SetOy(int noy) {
#ifdef EASYRPG_SYMBIAN
	if (oy != noy)
		SymbianTouch();
#endif
	oy = noy;
}

int TilemapLayer::GetWidth() const {
	return width;
}

void TilemapLayer::SetWidth(int nwidth) {
#ifdef EASYRPG_SYMBIAN
	if (width != nwidth)
		SymbianTouch();
#endif
	width = nwidth;
}

int TilemapLayer::GetHeight() const {
	return height;
}

void TilemapLayer::SetHeight(int nheight) {
#ifdef EASYRPG_SYMBIAN
	if (height != nheight)
		SymbianTouch();
#endif
	height = nheight;
}

int TilemapLayer::GetAnimationSpeed() const {
	return animation_speed;
}

void TilemapLayer::SetAnimationSpeed(int speed) {
	animation_speed = speed;
}

int TilemapLayer::GetAnimationType() const {
	return animation_type;
}

void TilemapLayer::SetAnimationType(int type) {
	animation_type = type;
}

void TilemapLayer::Substitute(int old_id, int new_id) {
	int subst_count = 0;

	for (size_t i = 0; i < substitutions.size(); ++i) {
		if (substitutions[i] == old_id) {
			++subst_count;
			substitutions[i] = (uint8_t) new_id;
		}
	}

	if (subst_count > 0) {
#ifdef EASYRPG_SYMBIAN
		SymbianTouch();
#endif
		// Recalculate z values of all tiles
		CreateTileCache(map_data);
		PrepareTileDrawCache();
	}
}

void TilemapLayer::SetFastBlit(bool fast) {
	fast_blit = fast;
}

TilemapSubLayer::TilemapSubLayer(TilemapLayer* tilemap, int z) :
	type(TypeTilemap),
	tilemap(tilemap),
	z(z)
{
	Graphics::RegisterDrawable(this);
}

TilemapSubLayer::~TilemapSubLayer() {
	Graphics::RemoveDrawable(this);
}

void TilemapSubLayer::Draw() {
	if (!tilemap->GetChipset()) {
		return;
	}

	tilemap->Draw(GetZ());
}

int TilemapSubLayer::GetZ() const {
	return z;
}

DrawableType TilemapSubLayer::GetType() const {
	return type;
}

void TilemapLayer::SetTone(Tone tone) {
	if (tone == this->tone) {
		return;
	}

	this->tone = tone;
#ifdef EASYRPG_SYMBIAN
	SymbianTouch();
	if (tone == Tone()) {
		autotiles_d_screen_effect.reset();
		autotiles_ab_screen_effect.reset();
		chipset_effect.reset();
		autotiles_d_screen_tone_tiles.clear();
		autotiles_ab_screen_tone_tiles.clear();
		chipset_tone_tiles.clear();
		return;
	}
#endif

	if (autotiles_d_screen_effect)
		std::fill(autotiles_d_screen_tone_tiles.begin(), autotiles_d_screen_tone_tiles.end(), 0);
	if (autotiles_ab_screen_effect)
		std::fill(autotiles_ab_screen_tone_tiles.begin(), autotiles_ab_screen_tone_tiles.end(), 0);
	if (chipset_effect)
		std::fill(chipset_tone_tiles.begin(), chipset_tone_tiles.end(), 0);
}