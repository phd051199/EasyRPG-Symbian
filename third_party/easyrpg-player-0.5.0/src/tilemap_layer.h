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

#ifndef _TILEMAP_LAYER_H_
#define _TILEMAP_LAYER_H_

// Headers
#include <vector>
#include <map>
#include "system.h"
#include "drawable.h"
#include "tone.h"

class Rect;
class TilemapLayer;

/**
 * TilemapSubLayer class.
 */
class TilemapSubLayer : public Drawable {
public:
	TilemapSubLayer(TilemapLayer* tilemap, int z);
	~TilemapSubLayer() override;

	void Draw() override;

	int GetZ() const override;

	DrawableType GetType() const override;

private:
	DrawableType type;
	TilemapLayer* tilemap;
	int z;
};

/**
 * TilemapLayer class.
 */
class TilemapLayer {
public:
	TilemapLayer(int ilayer);

	void DrawTile(Bitmap& destination, Bitmap& screen, int x, int y, int row, int col, int opacity);
	void Draw(int z_order);

	void Update();

	BitmapRef const& GetChipset() const;
	void SetChipset(BitmapRef const& nchipset, BitmapRef const& nopaque = BitmapRef());
	std::vector<short> GetMapData() const;
	void SetMapData(const std::vector<short>& nmap_data);
	std::vector<unsigned char> GetPassable() const;
	void SetPassable(const std::vector<unsigned char>& npassable);
	bool GetVisible() const;
	void SetVisible(bool nvisible);
	int GetOx() const;
	void SetOx(int nox);
	int GetOy() const;
	void SetOy(int noy);
	int GetWidth() const;
	void SetWidth(int nwidth);
	int GetHeight() const;
	void SetHeight(int nheight);
	int GetAnimationSpeed() const;
	void SetAnimationSpeed(int speed);
	int GetAnimationType() const;
	void SetAnimationType(int type);
	void Substitute(int old_id, int new_id);
	/**
	 * Influences how tiles of the tilemap are blitted.
	 * When enabled the opacity information of the tile is ignored and a opaque
	 * tile is assumed (Faster).
	 *
	 * @param fast true: enable fast blit (ignores alpha)
	 */
	void SetFastBlit(bool fast);

	void SetTone(Tone tone);

private:
	BitmapRef chipset;
#ifdef EASYRPG_SYMBIAN
	BitmapRef chipset_opaque;
#endif
	BitmapRef chipset_effect;
	std::vector<uint8_t> chipset_tone_tiles;

	std::vector<short> map_data;
	std::vector<uint8_t> passable;
	std::vector<uint8_t> substitutions;
	bool visible;
	int ox;
	int oy;
	int width;
	int height;
	char animation_frame;
	char animation_step_ab;
	char animation_step_c;
	int animation_speed;
	int animation_type;
	int layer;
	bool fast_blit = false;

	void CreateTileCache(const std::vector<short>& nmap_data);
	void PrepareTileDrawCache();
	void DrawTiles(Bitmap& destination, int z_order, const Rect* paint_clip);
	void DrawTilesView(Bitmap& destination, int z_order, const Rect* paint_clip, int view_ox, int view_oy, int view_width, int view_height);
	void GenerateAutotileAB(short ID, short animID);
	void GenerateAutotileD(short ID);

	static const int TILES_PER_ROW = 64;

	struct TileXY {
		uint8_t x;
		uint8_t y;
		bool valid;
		TileXY() : valid(false) {}
		TileXY(uint8_t x, uint8_t y) : x(x), y(y), valid(true) {}
	};

	BitmapRef GenerateAutotiles(int count, const std::map<uint32_t, TileXY>& map);

	TileXY GetCachedAutotileAB(short ID, short animID);
	TileXY GetCachedAutotileD(short ID);
	BitmapRef autotiles_ab_screen;
#ifdef EASYRPG_SYMBIAN
	BitmapRef autotiles_ab_screen_opaque;
#endif
	BitmapRef autotiles_ab_screen_effect;
	std::vector<uint8_t> autotiles_ab_screen_tone_tiles;
	BitmapRef autotiles_d_screen;
#ifdef EASYRPG_SYMBIAN
	BitmapRef autotiles_d_screen_opaque;
#endif
	BitmapRef autotiles_d_screen_effect;
	std::vector<uint8_t> autotiles_d_screen_tone_tiles;

	int autotiles_ab_next = -1;
	int autotiles_d_next = -1;

	TileXY autotiles_ab[3][3][16][47];
	TileXY autotiles_d[12][50];

	std::map<uint32_t, TileXY> autotiles_ab_map;
	std::map<uint32_t, TileXY> autotiles_d_map;

	enum TileSource {
		SourceNone,
		SourceChipset,
		SourceChipsetAnimatedC,
		SourceAutotileAB,
		SourceAutotileD
	};

	struct TileData {
		short ID;
		int z;
		uint8_t source;
		uint8_t source_x[4];
		uint8_t source_y[4];
		uint8_t opacity[4];
	};
	std::vector<std::vector<TileData> > data_cache;
	std::vector<std::shared_ptr<TilemapSubLayer> > sublayers;

	Tone tone;
#ifdef EASYRPG_SYMBIAN
	uint32_t symbian_revision = 1;
	bool symbian_has_animated_ab[2] = { false, false };
	bool symbian_has_animated_c[2] = { false, false };
	struct SymbianScrollCacheState {
		BitmapRef bitmap;
		uint32_t revision = 0;
		int ox = 0;
		int oy = 0;
		int animation_step_ab = -1;
		int animation_step_c = -1;
		int recorded_animation_step_ab = -1;
		int recorded_animation_step_c = -1;
	};
	SymbianScrollCacheState symbian_scroll_caches[2];
	void SymbianTouch();
	void SymbianScrollTouch(int shift_x, int shift_y);
	int SymbianScrollCacheIndex(int z_order) const;
	bool SymbianUseScrollCache(int z_order) const;
	bool SymbianUseAnchoredScrollCache(int z_order) const;
	bool SymbianScrollCacheValid(int z_order, int display_width, int display_height) const;
	bool SymbianPrepareScrollCache(int z_order, int display_width, int display_height);
	void SymbianShiftScrollCache(int z_order, int shift_x, int shift_y);
	void SymbianRefreshScrollCacheRect(int z_order, const Rect& rect);
	unsigned int SymbianSignature(int z_order) const;
#endif
};

#endif
