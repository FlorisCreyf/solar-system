#include "Game.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"
#include "swappy/swappyGL.h"
#include "swappy/swappyGL_extra.h"
#include <cassert>

namespace Solar {

    Game::Game(android_app *app) :
        app(app),
        renderer(nullptr),
        scene(nullptr),
        jniEnv(nullptr)
    {
        app->activity->vm->AttachCurrentThread(&jniEnv, NULL);
        assert(SwappyGL_init(jniEnv, app->activity->javaGameActivity));
        SwappyGL_setSwapIntervalNS(SWAPPY_SWAP_60FPS);

        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();

        AAssetManager *mgr = app->activity->assetManager;
        AAssetDir *assetDir = AAssetManager_openDir(mgr, "");
        const char *filename = nullptr;
        while ((filename = AAssetDir_getNextFileName(assetDir)) != nullptr) {
            if (strcmp(filename, "RobotoMono-Light.ttf") == 0) {
                AAsset *asset = AAssetManager_open(mgr, filename, AASSET_MODE_STREAMING);
                size_t size = AAsset_getLength(asset);
                char *data = new char[size];
                memmove(data, AAsset_getBuffer(asset), size);
                io.Fonts->AddFontFromMemoryTTF(data, size, 40);
                AAsset_close(asset);
            }
        }
    }

    Game::~Game()
    {
        if (scene)
            delete scene;
        ImGui::DestroyContext();
        SwappyGL_destroy();
        app->activity->vm->AttachCurrentThread(&jniEnv, NULL);
        release();
    }

    void Game::init(android_app *new_app)
    {
        app = new_app;
        if (!renderer) {
            assert(SwappyGL_setWindow(app->window));
            renderer = new Renderer(app);
            if (!scene)
                scene = new Scene(1.0f/std::min(renderer->getWidth(), renderer->getHeight()));
            scene->load();
        }
        currentTime = std::chrono::high_resolution_clock::now();
    }

    void Game::release()
    {
        if (scene)
            scene->unload();
        if (renderer) {
            delete renderer;
            renderer = nullptr;
        }
    }

    void Game::execute()
    {
        if (!renderer)
            return;

        auto now = std::chrono::high_resolution_clock::now();
        auto past = currentTime;
        auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(now - past);

        if (duration.count() > 1.f/120.f) {
            currentTime = now;
            update(duration);
        }

        processInput();
        renderer->render(*scene);
    }

    void Game::update(std::chrono::duration<double> duration)
    {
        scene->update(duration.count());
    }

    void Game::processInput()
    {
        auto *inputBuffer = android_app_swap_input_buffers(app);
        if (!inputBuffer || !renderer)
            return;

        for (auto i = 0; i < inputBuffer->motionEventsCount; i++) {
            auto &motionEvent = inputBuffer->motionEvents[i];
            auto action = motionEvent.action;
            auto pointerIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
            auto &pointer = motionEvent.pointers[pointerIndex];
            auto x = GameActivityPointerAxes_getX(&pointer);
            auto y = GameActivityPointerAxes_getY(&pointer);
            switch (action & AMOTION_EVENT_ACTION_MASK) {
                case AMOTION_EVENT_ACTION_DOWN:
                case AMOTION_EVENT_ACTION_POINTER_DOWN:
                    if (pointerIndex == 0) {
                        originalLocation = scene->getLocation();
                        originalPointLocation = Vector2{x, y};
                    }
                    break;
                case AMOTION_EVENT_ACTION_UP:
                case AMOTION_EVENT_ACTION_POINTER_UP:
                    scene->update(Ray{Vector2{}, Vector2{}});
                    break;
                case AMOTION_EVENT_ACTION_MOVE:
                    if (pointerIndex == 0) {
                        float height = renderer->getHeight() / 2;
                        float width = renderer->getWidth() / 2;
                        if (width < height)
                            height *= renderer->getAspect();
                        else
                            width *= renderer->getAspect();
                        float x1 = originalPointLocation.x/width;
                        float y1 = (height-originalPointLocation.y)/height;
                        float x2 = x/width;
                        float y2 = (height-y)/height;
                        Vector2 location{
                            originalLocation.x + x2 - x1,
                            originalLocation.y + y2 - y1
                        };
                        scene->update(Ray{originalLocation, location});
                    }
                    break;
            }
        }
        android_app_clear_motion_events(inputBuffer);
        android_app_clear_key_events(inputBuffer);
    }

}