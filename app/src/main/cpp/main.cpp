#include <jni.h>

#include "AndroidOut.h"
#include "Renderer.h"
#include "Scene.h"
#include "Game.h"

#include <game-activity/GameActivity.cpp>
#include <game-text-input/gametextinput.cpp>

extern "C" {

#include <game-activity/native_app_glue/android_native_app_glue.c>

static Solar::Game *game = nullptr;

void handle_cmd(android_app *app, int32_t cmd)
{
    switch (cmd) {
        case APP_CMD_INIT_WINDOW:
            game->init(app);
            break;
        case APP_CMD_TERM_WINDOW:
            game->release();
            break;
        default:
            break;
    }
}

bool motion_event_filter_func(const GameActivityMotionEvent *motionEvent)
{
    auto sourceClass = motionEvent->source & AINPUT_SOURCE_CLASS_MASK;
    return (sourceClass == AINPUT_SOURCE_CLASS_POINTER ||
            sourceClass == AINPUT_SOURCE_CLASS_JOYSTICK);
}

void android_main(struct android_app *app)
{
    Solar::out << "Calling main" << std::endl;

    if (!game)
        game = new Solar::Game(app);

    app->onAppCmd = &handle_cmd;
    android_app_set_motion_event_filter(app, motion_event_filter_func);

    int events;
    android_poll_source *source;
    while (!app->destroyRequested) {
        if (ALooper_pollAll(0, nullptr, &events, (void **)&source) >= 0)
            if (source)
                source->process(app, source);
        game->execute();
    }
}

}
