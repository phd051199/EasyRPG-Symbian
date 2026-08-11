#include <memory>
#include <string>

#include "baseui.h"
#include "bitmap.h"
#include "color.h"
#include "graphics.h"
#include "input.h"
#include "output.h"
#include "player.h"
#include "scene.h"
#include "scene_logo.h"
#include "easyrpgsymbian/SymbianAudio.h"
#include "easyrpgsymbian/UpstreamPlayerBridge.h"

namespace {
unsigned int gTicks = 0;
bool gPlayerStarted = false;
bool gRenderFrame = true;
int gLastSceneType = -1;
bool gLogSceneFrame = false;

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
#ifdef SUPPORT_AUDIO
    AudioInterface& GetAudio() override { return EasyRpgSymbianAudio(); }
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
        for (unsigned int bit = 0; bit < 32; ++bit) {
            const unsigned int index = word * 32 + bit;
            if (index >= Input::Keys::KEYS_COUNT)
                return;
            if ((words[word] & (1u << bit)) != 0)
                state.set(index);
        }
    }
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
    return std::shared_ptr<BaseUi>(new SymbianUi(width, height));
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
    Output::Debug("Symbian bridge player init returned");
    if (!DisplayUi)
        return -3;
    if (DisplayUi->GetWidth() != width || DisplayUi->GetHeight() != height)
        DisplayUi->Resize(width, height);

    Graphics::Init();
    Output::Debug("Symbian bridge graphics init done");
    Input::Init();
    Output::Debug("Symbian bridge input init done");
    Scene::Push(std::make_shared<Scene>());
    Scene::Push(std::shared_ptr<Scene>(new Scene_Logo()));
    Output::Debug("Symbian bridge scene stack ready");
    Player::reset_flag = false;
    Player::FrameReset();
    gLastSceneType = -1;
    gLogSceneFrame = false;
    gPlayerStarted = true;
    return 0;
}

extern "C" void EasyRpgUpstreamPlayerShutdown(void) {
    if (!gPlayerStarted)
        return;
    gPlayerStarted = false;
    if (DisplayUi)
        Player::Exit();
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
    static bool first_tick = true;
    if (first_tick)
        Output::Debug("Symbian first tick enter render=%d", gRenderFrame ? 1 : 0);
    Player::MainLoop();
    if (first_tick) {
        Output::Debug("Symbian first tick leave");
        first_tick = false;
    }
    if (DisplayUi && Scene::instance) {
        const int scene_type = static_cast<int>(Scene::instance->type);
        if (scene_type != gLastSceneType) {
            gLastSceneType = scene_type;
            gLogSceneFrame = true;
            if (scene_type >= 0 && scene_type < Scene::SceneMax)
                Output::Debug("Symbian scene %s", Scene::scene_names[scene_type]);
        }
        if (gRenderFrame && gLogSceneFrame && DisplayUi->GetDisplaySurface()) {
            BitmapRef surface = DisplayUi->GetDisplaySurface();
            const unsigned int* pixels = reinterpret_cast<const unsigned int*>(surface->pixels());
            if (pixels) {
                const int width = surface->width();
                const int height = surface->height();
                const int pitch = surface->pitch() / static_cast<int>(sizeof(unsigned int));
                unsigned int hash = 2166136261u;
                for (int y = 0; y < height; ++y) {
                    const unsigned int* row = pixels + y * pitch;
                    for (int x = 0; x < width; ++x) {
                        hash ^= row[x];
                        hash *= 16777619u;
                    }
                }
                const unsigned int center = pixels[(height / 2) * pitch + width / 2];
                Output::Debug("Symbian frame scene=%d hash=%08x center=%08x", scene_type, hash, center);
                gLogSceneFrame = false;
            }
        }
    }
    if (!DisplayUi) {
        gPlayerStarted = false;
        return 1;
    }
    return 0;
}

extern "C" int EasyRpgUpstreamPlayerShouldRender(void) {
    return gRenderFrame ? 1 : 0;
}

extern "C" const unsigned int* EasyRpgUpstreamPlayerPixels(void) {
    if (!DisplayUi || !DisplayUi->GetDisplaySurface())
        return 0;
    return reinterpret_cast<const unsigned int*>(DisplayUi->GetDisplaySurface()->pixels());
}

extern "C" int EasyRpgUpstreamPlayerWidth(void) {
    return DisplayUi ? static_cast<int>(DisplayUi->GetWidth()) : 0;
}

extern "C" int EasyRpgUpstreamPlayerHeight(void) {
    return DisplayUi ? static_cast<int>(DisplayUi->GetHeight()) : 0;
}

extern "C" int EasyRpgUpstreamPlayerPitchPixels(void) {
    if (!DisplayUi || !DisplayUi->GetDisplaySurface())
        return 0;
    return DisplayUi->GetDisplaySurface()->pitch() / static_cast<int>(sizeof(unsigned int));
}
