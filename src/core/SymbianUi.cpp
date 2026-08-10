#include <cstdlib>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

#include "baseui.h"
#include "bitmap.h"
#include "color.h"
#include "data.h"
#include "graphics.h"
#include "lmu_reader.h"
#include "sprite.h"
#include "tilemap.h"
#include "easyrpgsymbian/UpstreamGraphicsBridge.h"

namespace ImagePNG {
bool ReadPNGBytes(const uint8_t* data,
                  unsigned len,
                  bool transparent,
                  int& width,
                  int& height,
                  void*& pixels);
}

namespace {
unsigned int gTicks = 0;
std::unique_ptr<Tilemap> gTilemap;
std::unique_ptr<Sprite> gPlayerSprite;
std::unique_ptr<Sprite> gPictureSprite;

struct PictureTransition {
    int remaining;
    int total;
    int start_x;
    int start_y;
    int start_zoom;
    int start_opacity;
    int target_x;
    int target_y;
    int target_zoom;
    int target_opacity;
};

PictureTransition gPictureTransition = {0, 0, 0, 0, 1000, 255, 0, 0, 1000, 255};

const RPG::Chipset* FindChipset(int chipset_id) {
    for (std::vector<RPG::Chipset>::const_iterator it = Data::chipsets.begin();
         it != Data::chipsets.end();
         ++it) {
        if (it->ID == chipset_id)
            return &*it;
    }
    return 0;
}

int ClampScroll(int value, int map_pixels, int screen_pixels) {
    if (value < 0)
        return 0;
    const int maximum = map_pixels > screen_pixels ? map_pixels - screen_pixels : 0;
    return value > maximum ? maximum : value;
}

int ClampPercent(int value) {
    if (value < 0)
        return 0;
    return value > 100 ? 100 : value;
}

int PercentToOpacity(int value) {
    return (ClampPercent(value) * 255 + 50) / 100;
}

void ResetPictureTransition() {
    gPictureTransition.remaining = 0;
    gPictureTransition.total = 0;
}

void UpdatePictureTransition() {
    if (!gPictureSprite || gPictureTransition.remaining <= 0 || gPictureTransition.total <= 0)
        return;

    const int elapsed = gPictureTransition.total - gPictureTransition.remaining + 1;
    const int total = gPictureTransition.total;
    const int x = gPictureTransition.start_x +
                  (gPictureTransition.target_x - gPictureTransition.start_x) * elapsed / total;
    const int y = gPictureTransition.start_y +
                  (gPictureTransition.target_y - gPictureTransition.start_y) * elapsed / total;
    const int zoom = gPictureTransition.start_zoom +
                     (gPictureTransition.target_zoom - gPictureTransition.start_zoom) * elapsed / total;
    const int opacity = gPictureTransition.start_opacity +
                        (gPictureTransition.target_opacity - gPictureTransition.start_opacity) * elapsed / total;
    gPictureSprite->SetX(x);
    gPictureSprite->SetY(y);
    gPictureSprite->SetZoomX(zoom / 1000.0);
    gPictureSprite->SetZoomY(zoom / 1000.0);
    gPictureSprite->SetOpacity(opacity, opacity);
    --gPictureTransition.remaining;
}

BitmapRef CreateResourceBitmap(const unsigned char* data, unsigned int size, uint32_t flags) {
    if (!data || size == 0)
        return BitmapRef();

    static const unsigned char png_signature[8] = {0x89, 'P', 'N', 'G', 0x0d, 0x0a, 0x1a, 0x0a};
    if (size >= 8 && memcmp(data, png_signature, 8) == 0) {
        int width = 0;
        int height = 0;
        void* pixels = 0;
        if (!ImagePNG::ReadPNGBytes(data, size, true, width, height, pixels) ||
            !pixels || width <= 0 || height <= 0)
            return BitmapRef();

        BitmapRef source = Bitmap::Create(pixels,
                                           width,
                                           height,
                                           width * 4,
                                           format_R8G8B8A8_a().format());
        BitmapRef result;
        if (source) {
            result = Bitmap::Create(width, height, true);
            if (result) {
                result->Clear();
                result->Blit(0, 0, *source, source->GetRect(), Opacity::opaque);
                result->CheckPixels(flags);
            }
        }
        source.reset();
        free(pixels);
        return result;
    }

    return Bitmap::Create(data,
                          size,
                          true,
                          flags);
}

class SymbianUi : public BaseUi {
public:
    SymbianUi(long width, long height) {
        Bitmap::SetFormat(format_B8G8R8A8_a().format());
        Resize(width, height);
        mouse_focus = false;
        mouse_x = 0;
        mouse_y = 0;
        cursor_visible = false;
        back_color = Color(0, 0, 0, 255);
        keys.reset();
    }

    void BeginDisplayModeChange() override {}
    void EndDisplayModeChange() override {}

    void Resize(long width, long height) override {
        current_display_mode.effective = true;
        current_display_mode.zoom = false;
        current_display_mode.width = static_cast<int>(width);
        current_display_mode.height = static_cast<int>(height);
        current_display_mode.bpp = 32;
        current_display_mode.flags = 0;
        main_surface = Bitmap::Create(static_cast<int>(width), static_cast<int>(height), false);
    }

    void ToggleFullscreen() override {}
    void ToggleZoom() override {}
    void ProcessEvents() override {}
    void UpdateDisplay() override {}
    void SetTitle(const std::string&) override {}

    bool ShowCursor(bool flag) override {
        const bool old = cursor_visible;
        cursor_visible = flag;
        return old;
    }

    bool IsFullscreen() override { return true; }
    uint32_t GetTicks() const override { return gTicks; }
    void Sleep(uint32_t) override {}
};
}

std::shared_ptr<BaseUi> DisplayUi;

BaseUi::BaseUi() :
    mouse_focus(false),
    mouse_x(0),
    mouse_y(0),
    cursor_visible(true),
    back_color(0, 0, 0, 255) {
    keys.reset();
}

std::shared_ptr<BaseUi> BaseUi::CreateUi(long width, long height, bool, bool) {
    return std::shared_ptr<BaseUi>(new SymbianUi(width, height));
}

void BaseUi::CleanDisplay() {
    if (main_surface) main_surface->Clear();
}

BitmapRef BaseUi::CaptureScreen() {
    if (!main_surface) return BitmapRef();
    return Bitmap::Create(*main_surface, main_surface->GetRect());
}

long BaseUi::GetWidth() const {
    return current_display_mode.width;
}

long BaseUi::GetHeight() const {
    return current_display_mode.height;
}

bool BaseUi::GetMouseFocus() const {
    return mouse_focus;
}

int BaseUi::GetMousePosX() const {
    return mouse_x;
}

int BaseUi::GetMousePosY() const {
    return mouse_y;
}

Color const& BaseUi::GetBackcolor() const {
    return back_color;
}

void BaseUi::SetBackcolor(const Color& color) {
    back_color = color;
}

void BaseUi::AddBackground() {
    if (main_surface) main_surface->Fill(back_color);
}

BitmapRef const& BaseUi::GetDisplaySurface() const {
    return main_surface;
}

BitmapRef& BaseUi::GetDisplaySurface() {
    return main_surface;
}

BaseUi::KeyStatus& BaseUi::GetKeyStates() {
    return keys;
}

extern "C" int EasyRpgUpstreamGraphicsInit(int width, int height) {
    if (width <= 0 || height <= 0) return -1;
    DisplayUi = BaseUi::CreateUi(width, height, true, false);
    if (!DisplayUi || !DisplayUi->GetDisplaySurface()) return -2;
    Graphics::Init();
    return 0;
}

extern "C" void EasyRpgUpstreamGraphicsShutdown(void) {
    gPictureSprite.reset();
    ResetPictureTransition();
    gPlayerSprite.reset();
    gTilemap.reset();
    if (DisplayUi) Graphics::Quit();
    DisplayUi.reset();
}

extern "C" int EasyRpgUpstreamGraphicsRenderFrame(void) {
    if (!DisplayUi || !DisplayUi->GetDisplaySurface()) return -1;
    if (gTilemap) gTilemap->Update();
    if (gPlayerSprite) gPlayerSprite->Update();
    UpdatePictureTransition();
    if (gPictureSprite) gPictureSprite->Update();
    Graphics::Update(true);
    return 0;
}

extern "C" int EasyRpgUpstreamGraphicsLoadMap(const char* lmu_path,
                                                const unsigned char* chipset_data,
                                                unsigned int chipset_size,
                                                int chipset_id,
                                                int start_x,
                                                int start_y) {
    if (!DisplayUi || !lmu_path || !chipset_data || chipset_size == 0)
        return -1;

    std::unique_ptr<RPG::Map> map = LMU_Reader::Load(lmu_path, std::string());
    if (!map || map->width <= 0 || map->height <= 0)
        return -2;

    const int selected_chipset_id = chipset_id > 0 ? chipset_id : map->chipset_id;
    const RPG::Chipset* chipset = FindChipset(selected_chipset_id);
    if (!chipset)
        return -3;

    BitmapRef chipset_bitmap = CreateResourceBitmap(chipset_data,
                                                     chipset_size,
                                                     Bitmap::Flag_Chipset | Bitmap::Flag_ReadOnly);
    if (!chipset_bitmap || chipset_bitmap->width() <= 0 || chipset_bitmap->height() <= 0)
        return -4;

    std::unique_ptr<Tilemap> tilemap(new Tilemap());
    tilemap->SetWidth(map->width);
    tilemap->SetHeight(map->height);
    tilemap->SetChipset(chipset_bitmap);
    tilemap->SetMapDataDown(map->lower_layer);
    tilemap->SetMapDataUp(map->upper_layer);

    std::vector<unsigned char> passages_down = chipset->passable_data_lower;
    std::vector<unsigned char> passages_up = chipset->passable_data_upper;
    if (passages_down.size() < 162)
        passages_down.resize(162, 0x0f);
    if (passages_up.size() < 144)
        passages_up.resize(144, 0x0f);
    tilemap->SetPassableDown(passages_down);
    tilemap->SetPassableUp(passages_up);
    tilemap->SetAnimationType(chipset->animation_type);
    tilemap->SetAnimationSpeed(chipset->animation_speed != 0 ? 12 : 24);

    const int ox = ClampScroll(start_x * 16 - 152,
                               map->width * 16,
                               static_cast<int>(DisplayUi->GetWidth()));
    const int oy = ClampScroll(start_y * 16 - 112,
                               map->height * 16,
                               static_cast<int>(DisplayUi->GetHeight()));
    tilemap->SetOx(ox);
    tilemap->SetOy(oy);
    gTilemap.swap(tilemap);
    return 0;
}

extern "C" int EasyRpgUpstreamGraphicsLoadPlayer(const unsigned char* character_data,
                                                   unsigned int character_size,
                                                   int character_index,
                                                   int transparent,
                                                   int start_x,
                                                   int start_y) {
    if (!DisplayUi || !gTilemap)
        return -1;

    gPlayerSprite.reset();
    if (transparent != 0)
        return 0;
    if (!character_data || character_size == 0 || character_index < 0 || character_index >= 8)
        return -2;

    BitmapRef character_bitmap = CreateResourceBitmap(character_data,
                                                       character_size,
                                                       Bitmap::Flag_ReadOnly);
    if (!character_bitmap || character_bitmap->width() <= 0 || character_bitmap->height() <= 0)
        return -3;
    if (character_bitmap->width() % 12 != 0 || character_bitmap->height() % 8 != 0)
        return -4;

    const int frame_width = character_bitmap->width() / 12;
    const int frame_height = character_bitmap->height() / 8;
    const int group_x = character_index % 4;
    const int group_y = character_index / 4;
    const int source_x = (group_x * 3 + 1) * frame_width;
    const int source_y = group_y * 4 * frame_height;

    std::unique_ptr<Sprite> sprite(new Sprite());
    sprite->SetBitmap(character_bitmap);
    sprite->SetSrcRect(Rect(source_x, source_y, frame_width, frame_height));
    sprite->SetOx(frame_width / 2);
    sprite->SetOy(frame_height);
    sprite->SetX(start_x * 16 - gTilemap->GetOx() + 8);
    sprite->SetY(start_y * 16 - gTilemap->GetOy() + 16);
    sprite->SetZ(100);
    gPlayerSprite.swap(sprite);
    return 0;
}

extern "C" int EasyRpgUpstreamGraphicsShowPicture(const unsigned char* picture_data,
                                                    unsigned int picture_size,
                                                    int x,
                                                    int y,
                                                    int magnify_percent,
                                                    int opacity_percent) {
    if (!DisplayUi || !picture_data || picture_size == 0)
        return -1;

    BitmapRef picture_bitmap = CreateResourceBitmap(picture_data,
                                                     picture_size,
                                                     Bitmap::Flag_ReadOnly);
    if (!picture_bitmap || picture_bitmap->width() <= 0 || picture_bitmap->height() <= 0)
        return -2;

    std::unique_ptr<Sprite> sprite(new Sprite());
    sprite->SetBitmap(picture_bitmap);
    sprite->SetOx(picture_bitmap->width() / 2);
    sprite->SetOy(picture_bitmap->height() / 2);
    sprite->SetX(x);
    sprite->SetY(y);
    sprite->SetZoomX(ClampPercent(magnify_percent) / 100.0);
    sprite->SetZoomY(ClampPercent(magnify_percent) / 100.0);
    const int opacity = PercentToOpacity(opacity_percent);
    sprite->SetOpacity(opacity, opacity);
    sprite->SetZ(1000);
    gPictureSprite.swap(sprite);
    ResetPictureTransition();
    return 0;
}

extern "C" int EasyRpgUpstreamGraphicsMovePicture(int x,
                                                    int y,
                                                    int magnify_percent,
                                                    int opacity_percent,
                                                    int duration_frames) {
    if (!gPictureSprite)
        return -1;

    const int target_zoom = ClampPercent(magnify_percent) * 10;
    const int target_opacity = PercentToOpacity(opacity_percent);
    if (duration_frames <= 0) {
        gPictureSprite->SetX(x);
        gPictureSprite->SetY(y);
        gPictureSprite->SetZoomX(target_zoom / 1000.0);
        gPictureSprite->SetZoomY(target_zoom / 1000.0);
        gPictureSprite->SetOpacity(target_opacity, target_opacity);
        ResetPictureTransition();
        return 0;
    }

    gPictureTransition.remaining = duration_frames;
    gPictureTransition.total = duration_frames;
    gPictureTransition.start_x = gPictureSprite->GetX();
    gPictureTransition.start_y = gPictureSprite->GetY();
    gPictureTransition.start_zoom = static_cast<int>(gPictureSprite->GetZoomX() * 1000.0 + 0.5);
    gPictureTransition.start_opacity = gPictureSprite->GetOpacity();
    gPictureTransition.target_x = x;
    gPictureTransition.target_y = y;
    gPictureTransition.target_zoom = target_zoom;
    gPictureTransition.target_opacity = target_opacity;
    return 0;
}

extern "C" void EasyRpgUpstreamGraphicsErasePicture(void) {
    gPictureSprite.reset();
    ResetPictureTransition();
}

extern "C" void EasyRpgUpstreamGraphicsSetTicks(unsigned int ticks) {
    gTicks = ticks;
}

extern "C" void EasyRpgUpstreamGraphicsSetKeys(unsigned int keys_lo, unsigned int keys_hi) {
    if (!DisplayUi) return;
    BaseUi::KeyStatus& state = DisplayUi->GetKeyStates();
    state.reset();
    for (unsigned int i = 0; i < 32; ++i) {
        if ((keys_lo & (1u << i)) != 0) state.set(i);
        if ((keys_hi & (1u << i)) != 0) state.set(i + 32);
    }
}

extern "C" const unsigned int* EasyRpgUpstreamGraphicsPixels(void) {
    if (!DisplayUi || !DisplayUi->GetDisplaySurface()) return 0;
    return reinterpret_cast<const unsigned int*>(DisplayUi->GetDisplaySurface()->pixels());
}

extern "C" int EasyRpgUpstreamGraphicsWidth(void) {
    return DisplayUi ? static_cast<int>(DisplayUi->GetWidth()) : 0;
}

extern "C" int EasyRpgUpstreamGraphicsHeight(void) {
    return DisplayUi ? static_cast<int>(DisplayUi->GetHeight()) : 0;
}

extern "C" int EasyRpgUpstreamGraphicsPitchPixels(void) {
    if (!DisplayUi || !DisplayUi->GetDisplaySurface()) return 0;
    return DisplayUi->GetDisplaySurface()->pitch() / static_cast<int>(sizeof(unsigned int));
}
