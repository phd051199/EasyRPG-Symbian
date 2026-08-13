#include <memory>
#include <sstream>
#include <string>

#include "audio.h"
#include "baseui.h"
#include "bitmap.h"
#include "color.h"
#include "filefinder.h"
#include "graphics.h"
#include "input.h"
#include "main_data.h"
#include "output.h"
#include "player.h"
#include "scene.h"
#include "scene_gamebrowser.h"
#include "scene_map.h"
#include "scene_title.h"
#include "RenderDamage.h"
#include "UpstreamPlayerBridge.h"

namespace {
unsigned int gTicks = 0;
bool gPlayerStarted = false;
bool gRenderFrame = true;
void* gFramebufferPixels = 0;
int gFramebufferWidth = 0;
int gFramebufferHeight = 0;
int gFramebufferPitchBytes = 0;

DynamicFormat FramebufferFormat() {
    return DynamicFormat(16, 5, 11, 6, 5, 5, 0, 0, 0, PF::NoAlpha);
}

BitmapRef CreateMainSurface(int width, int height) {
    if (gFramebufferPixels && gFramebufferWidth == width &&
        gFramebufferHeight == height && gFramebufferPitchBytes >= width * 2) {
        return Bitmap::Create(gFramebufferPixels,
                              width,
                              height,
                              gFramebufferPitchBytes,
                              FramebufferFormat());
    }
    return Bitmap::Create(width, height, false);
}

class EasyRpgUi : public BaseUi {
public:
    EasyRpgUi(long width, long height) {
        Bitmap::SetFormat(FramebufferFormat());
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
        current_display_mode.bpp = 16;
        current_display_mode.flags = 0;
        main_surface = CreateMainSurface(static_cast<int>(width), static_cast<int>(height));
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
#ifdef SUPPORT_AUDIO
    AudioInterface& GetAudio() override { return EasyRpgAudio(); }
#endif
};

void SetKeys(unsigned int keys_0,
             unsigned int keys_1,
             unsigned int keys_2,
             unsigned int keys_3) {
    if (!DisplayUi)
        return;
    BaseUi::KeyStatus& state = DisplayUi->GetKeyStates();
    state.reset();
    const unsigned int words[4] = {keys_0, keys_1, keys_2, keys_3};
    for (unsigned int word = 0; word < 4; ++word) {
        unsigned int value = words[word];
        while (value != 0) {
            const unsigned int bit = static_cast<unsigned int>(__builtin_ctz(value));
            const unsigned int index = word * 32 + bit;
            if (index >= Input::Keys::KEYS_COUNT)
                return;
            state.set(index);
            value &= value - 1;
        }
    }
}

void PushInitialGameScene() {
    std::shared_ptr<FileFinder::DirectoryTree> tree =
        FileFinder::CreateDirectoryTree(Main_Data::GetProjectPath());
    if (!tree)
        Output::Error("%s is not a valid path", Main_Data::GetProjectPath().c_str());

    if (!tree || !FileFinder::IsValidProject(*tree)) {
        Scene::Push(std::make_shared<Scene_GameBrowser>());
        return;
    }

    FileFinder::SetDirectoryTree(tree);
    Player::CreateGameObjects();
    Scene::Push(std::make_shared<Scene_Title>());

    if (Player::load_game_id <= 0)
        return;

    std::stringstream ss;
    ss << "Save" << (Player::load_game_id <= 9 ? "0" : "") << Player::load_game_id << ".lsd";
    Player::LoadSavegame(FileFinder::FindDefault(ss.str()));
    Scene::Push(std::make_shared<Scene_Map>(true));
}
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
    return std::shared_ptr<BaseUi>(new EasyRpgUi(width, height));
}

void BaseUi::CleanDisplay() {
    if (main_surface)
        main_surface->Clear();
}

BitmapRef BaseUi::CaptureScreen() {
    if (!main_surface)
        return BitmapRef();
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
    if (main_surface)
        main_surface->Fill(back_color);
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

extern "C" int EasyRpgUpstreamPlayerBindFramebuffer(void* pixels,
                                                      int width,
                                                      int height,
                                                      int pitch_bytes) {
    if (!pixels || width <= 0 || height <= 0 || pitch_bytes < width * 2)
        return -1;

    const bool changed = gFramebufferPixels != pixels ||
        gFramebufferWidth != width ||
        gFramebufferHeight != height ||
        gFramebufferPitchBytes != pitch_bytes;
    gFramebufferPixels = pixels;
    gFramebufferWidth = width;
    gFramebufferHeight = height;
    gFramebufferPitchBytes = pitch_bytes;

    if (!DisplayUi || !changed)
        return 0;

    BitmapRef surface = Bitmap::Create(pixels,
                                       width,
                                       height,
                                       pitch_bytes,
                                       FramebufferFormat());
    if (!surface || !surface->pixels())
        return -2;
    DisplayUi->GetDisplaySurface() = surface;
    return 0;
}

extern "C" int EasyRpgUpstreamPlayerInit(const char* project_path,
                                           const char* save_path,
                                           int width,
                                           int height) {
    if (!project_path || !save_path || width <= 0 || height <= 0)
        return -1;
    if (gPlayerStarted)
        return -2;

    char arg0[] = "EasyRPG";
    char project_option[] = "--project-path";
    char save_option[] = "--save-path";
    char* argv[] = {
        arg0,
        project_option,
        const_cast<char*>(project_path),
        save_option,
        const_cast<char*>(save_path),
        0
    };

    Player::Init(5, argv);
    if (!DisplayUi)
        return -3;
    if (DisplayUi->GetWidth() != width || DisplayUi->GetHeight() != height)
        DisplayUi->Resize(width, height);

    Graphics::Init();
    Input::Init();
    Scene::Push(std::make_shared<Scene>());
    PushInitialGameScene();
    Player::reset_flag = false;
    Player::FrameReset();
    gPlayerStarted = true;
    return 0;
}

extern "C" void EasyRpgUpstreamPlayerShutdown(void) {
    if (!gPlayerStarted)
        return;
    gPlayerStarted = false;
    if (DisplayUi)
        Player::Exit();
    gFramebufferPixels = 0;
    gFramebufferWidth = 0;
    gFramebufferHeight = 0;
    gFramebufferPitchBytes = 0;
}

extern "C" void EasyRpgUpstreamPlayerPause(void) {
    if (gPlayerStarted && DisplayUi)
        Player::Pause();
}

extern "C" void EasyRpgUpstreamPlayerResume(void) {
    if (gPlayerStarted && DisplayUi)
        Player::Resume();
}

extern "C" int EasyRpgUpstreamPlayerTick(unsigned int ticks,
                                           unsigned int keys_0,
                                           unsigned int keys_1,
                                           unsigned int keys_2,
                                           unsigned int keys_3,
                                           int render_frame) {
    if (!gPlayerStarted || !DisplayUi || !Scene::instance)
        return -1;

    gTicks = ticks;
    gRenderFrame = render_frame != 0;
    SetKeys(keys_0, keys_1, keys_2, keys_3);
    Player::MainLoop();
    if (!DisplayUi) {
        gPlayerStarted = false;
        return 1;
    }
    return 0;
}

extern "C" int EasyRpgUpstreamPlayerShouldRender(void) {
    return gRenderFrame ? 1 : 0;
}

extern "C" int EasyRpgUpstreamPlayerDirtyRegionCount(void) {
    return SymbianRenderDamage::DamageValid()
        ? SymbianRenderDamage::DirtyCount()
        : -1;
}

extern "C" int EasyRpgUpstreamPlayerDirtyRegion(int index,
                                                   int* x,
                                                   int* y,
                                                   int* width,
                                                   int* height) {
    if (!SymbianRenderDamage::DamageValid() || index < 0 ||
        index >= SymbianRenderDamage::DirtyCount() || !x || !y || !width || !height)
        return 0;
    const Rect& rect = SymbianRenderDamage::DirtyRect(index);
    *x = rect.x;
    *y = rect.y;
    *width = rect.width;
    *height = rect.height;
    return 1;
}

extern "C" void EasyRpgUpstreamPlayerConsumeDirtyRegions(void) {
    SymbianRenderDamage::ConsumeDamage();
}
