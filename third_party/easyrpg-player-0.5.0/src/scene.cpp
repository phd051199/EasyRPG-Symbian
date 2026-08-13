#include <cassert>

#include "async_handler.h"
#include "graphics.h"
#include "output.h"
#include "player.h"
#include "scene.h"
#ifdef EASYRPG_SYMBIAN
#include "UpstreamPlayerBridge.h"
#endif

std::shared_ptr<Scene> Scene::instance;
std::vector<std::shared_ptr<Scene> > Scene::old_instances;
std::vector<std::shared_ptr<Scene> > Scene::instances;

const char Scene::scene_names[SceneMax][12] = {
    "Null",
    "Title",
    "Map",
    "Menu",
    "Item",
    "Skill",
    "Equip",
    "ActorTarget",
    "Status",
    "File",
    "Save",
    "Load",
    "End",
    "Battle",
    "Shop",
    "Name",
    "Gameover",
    "Debug",
    "Logo",
    "Order",
    "GameBrowser",
    "Teleport"
};

enum PushPopOperation {
    ScenePushed = 1,
    ScenePopped
};

int Scene::push_pop_operation = 0;

Scene::Scene() {
    type = Scene::Null;
}

void Scene::MainFunction() {
    static bool init = false;

    if (AsyncHandler::IsImportantFilePending() || Graphics::IsTransitionPending()) {
        Player::Update(false);
    } else if (!init) {
        switch (push_pop_operation) {
        case ScenePushed:
            Start();
            initialized = true;
            break;
        case ScenePopped:
            if (!initialized) {
                Start();
                initialized = true;
            } else {
                Continue();
            }
            break;
        default:
            break;
        }

        push_pop_operation = 0;
        TransitionIn();
        Resume();
        init = true;
        return;
    } else {
        Player::Update();
    }

    if (Scene::instance.get() != this) {
        assert(Scene::instance == instances.back());
#ifdef EASYRPG_SYMBIAN
        Graphics::Update(EasyRpgUpstreamPlayerShouldRender() != 0);
#else
        Graphics::Update(true);
#endif
        Suspend();
        TransitionOut();

        if (push_pop_operation == ScenePushed)
            Graphics::Push(Scene::instance->DrawBackground());

        init = false;
    }
}

void Scene::Start() {
}

void Scene::Continue() {
}

void Scene::Resume() {
}

void Scene::Suspend() {
}

void Scene::TransitionIn() {
    Graphics::Transition(Graphics::TransitionFadeIn, 6);
}

void Scene::TransitionOut() {
    Graphics::Transition(Graphics::TransitionFadeOut, 6, true);
}

void Scene::Update() {
}

void Scene::Push(std::shared_ptr<Scene> const& new_scene, bool pop_stack_top) {
    if (pop_stack_top && !instances.empty()) {
        old_instances.push_back(instances.back());
        instances.pop_back();
    }

    instances.push_back(new_scene);
    instance = new_scene;
    push_pop_operation = ScenePushed;
}

void Scene::Pop() {
    old_instances.push_back(instances.back());
    instances.pop_back();

    if (instances.empty()) {
        Push(std::make_shared<Scene>());
    } else {
        instance = instances.back();
    }

    push_pop_operation = ScenePopped;
}

void Scene::PopUntil(SceneType scene_type) {
    int count = 0;

    for (int i = static_cast<int>(instances.size()) - 1; i >= 0; --i) {
        if (instances[i]->type == scene_type) {
            for (i = 0; i < count; ++i) {
                old_instances.push_back(instances.back());
                instances.pop_back();
            }
            instance = instances.back();
            push_pop_operation = ScenePopped;
            return;
        }
        ++count;
    }

    Output::Warning("The requested scene %s was not on the stack", scene_names[scene_type]);
}

std::shared_ptr<Scene> Scene::Find(SceneType scene_type) {
    std::vector<std::shared_ptr<Scene> >::const_reverse_iterator it;
    for (it = instances.rbegin(); it != instances.rend(); ++it) {
        if ((*it)->type == scene_type)
            return *it;
    }
    return std::shared_ptr<Scene>();
}

bool Scene::DrawBackground() {
    return true;
}
