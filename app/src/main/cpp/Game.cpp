#include <assert.h>
#include "Game.h"
#include "Arrow.h"

#include "swappy/swappyGL.h"
#include "swappy/swappyGL_extra.h"

namespace Solar {

    Game::Game(android_app *app) :
        app(app),
        renderer(nullptr),
        scene(),
        jniEnv(nullptr)
    {
    }

    Game::~Game()
    {
        delete renderer;
    }

    void Game::init(android_app *new_app)
    {
        if (!renderer) {
            out << "Creating game" << std::endl;
            app = new_app;
            app->activity->vm->AttachCurrentThread(&jniEnv, NULL);
            SwappyGL_init(jniEnv, app->activity->javaGameActivity);
            SwappyGL_setSwapIntervalNS(SWAPPY_SWAP_60FPS);
            SwappyGL_setWindow(app->window);
            renderer = new Renderer(app);
            scene.load();
        }
    }

    void Game::release()
    {
        out << "Deleting game" << std::endl;
        scene.unload();
        delete renderer;
        renderer = nullptr;
        jniEnv = nullptr;
        SwappyGL_destroy();
    }

    void Game::execute()
    {
        if (renderer) {
            processInput();
            renderer->render(scene);
        }
    }

    void Game::processInput()
    {
        auto *inputBuffer = android_app_swap_input_buffers(app);
        if (!inputBuffer)
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
                        xDown = x;
                        yDown = y;
                        scene.initUpdate();
                    }
                    break;
                case AMOTION_EVENT_ACTION_UP:
                case AMOTION_EVENT_ACTION_POINTER_UP:
                    break;
                case AMOTION_EVENT_ACTION_MOVE:
                    if (pointerIndex == 0) {
                        float height = renderer->getHeight() / 2;
                        float width = renderer->getWidth() / 2;
                        if (width < height) {
                            height *= renderer->getAspect();
                        } else {
                            width *= renderer->getAspect();
                        }
                        Arrow arrow{
                            xDown/width,
                            (height-yDown)/height,
                            x/width,
                            (height-y)/height};
                        scene.update(arrow);
                    }
                    break;
            }
        }
        android_app_clear_motion_events(inputBuffer);
        android_app_clear_key_events(inputBuffer);
    }

}