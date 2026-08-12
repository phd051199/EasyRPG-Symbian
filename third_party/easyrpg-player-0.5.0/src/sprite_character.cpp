#include "sprite_character.h"
#include "cache.h"
#include "game_map.h"
#include "bitmap.h"

Sprite_Character::Sprite_Character(Game_Character* character) :
	character(character),
	tile_id(0),
	character_index(0),
	chara_width(24*(TILE_SIZE/16)),
	chara_height(32*(TILE_SIZE/16)) {
	Update();
}

void Sprite_Character::Update() {
	Sprite::Update();
	Rect r;
	if (tile_id != character->GetTileId() ||
		character_name != character->GetSpriteName() ||
		character_index != character->GetSpriteIndex()) {
		tile_id = character->GetTileId();
		character_name = character->GetSpriteName();
		character_index = character->GetSpriteIndex();
		if (tile_id > 0) {
			FileRequestAsync* tile_request = AsyncHandler::RequestFile("ChipSet", Game_Map::GetChipsetName());
			request_id = tile_request->Bind(&Sprite_Character::OnTileSpriteReady, this);
			tile_request->Start();
		} else if (character_name.empty()) {
			SetBitmap(BitmapRef());
		} else {
			FileRequestAsync* char_request = AsyncHandler::RequestFile("CharSet", character_name);
			request_id = char_request->Bind(&Sprite_Character::OnCharSpriteReady, this);
			char_request->Start();
		}
	}

	if (tile_id == 0) {
		int row = character->GetSpriteDirection();
		r.Set(character->GetPattern() * chara_width, row * chara_height, chara_width, chara_height);
		SetSrcRect(r);
	}

	int flash_time = character->GetFlashTimeLeft();
	if (flash_time > 0) {
		double level = character->GetFlashLevel();
		if (level <= 0.0)
			level = 128.0;
		Color color = character->GetFlashColor();
		int alpha = static_cast<int>(level);
		if (alpha < 0)
			alpha = 0;
		else if (alpha > 255)
			alpha = 255;
		color.alpha = alpha;
		SetFlashEffect(color);
		level -= level / flash_time;
		--flash_time;
		character->SetFlashLevel(flash_time > 0 ? level : 0.0);
		character->SetFlashTimeLeft(flash_time);
	} else {
		SetFlashEffect(Color());
	}

	SetVisible(character->GetVisible());
	if (GetVisible())
		SetOpacity(character->GetOpacity());

	SetX(character->GetScreenX());
	SetY(character->GetScreenY());
	SetZ(character->GetScreenZ());

	int bush_split = 4 - character->GetBushDepth();
	SetBushDepth(bush_split > 3 ? 0 : GetHeight() / bush_split);
}

Game_Character* Sprite_Character::GetCharacter() {
	return character;
}

void Sprite_Character::SetCharacter(Game_Character* new_character) {
	character = new_character;
}

void Sprite_Character::OnTileSpriteReady(FileRequestResult*) {
	std::string chipset = Game_Map::GetChipsetName();
	BitmapRef tile;
	if (!chipset.empty())
		tile = Cache::Tile(Game_Map::GetChipsetName(), tile_id);
	else
		tile = Bitmap::Create(16, 16, Color());
	SetBitmap(tile);
	Rect r;
	r.Set(0, 0, TILE_SIZE, TILE_SIZE);
	SetSrcRect(r);
	SetOx(8);
	SetOy(16);
	Update();
}

void Sprite_Character::OnCharSpriteReady(FileRequestResult*) {
	SetBitmap(Cache::Charset(character_name));
	SetOx(chara_width / 2);
	SetOy(chara_height);
	int sx = (character_index % 4) * chara_width * 3;
	int sy = (character_index / 4) * chara_height * 4;
	Rect r;
	r.Set(sx, sy, chara_width * 3, chara_height * 4);
	SetSpriteRect(r);
	Update();
}
