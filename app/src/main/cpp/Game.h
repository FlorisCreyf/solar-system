#ifndef SOLARSYSTEM_GAME_H
#define SOLARSYSTEM_GAME_H

#include "AndroidOut.h"
#include "Ray.h"
#include "Renderer.h"
#include "Scene.h"
#include <game-activity/native_app_glue/android_native_app_glue.h>
#include <chrono>

namespace Solar {

    class Game {
    public:
        Game(android_app *app);
        ~Game();
        void execute();
        void processInput();
        void init(android_app *app);
        void release();

    private:
        void update(std::chrono::duration<double> duration);

        JNIEnv *jniEnv;
        android_app *app;
        Renderer *renderer;
        Scene *scene;
        Vector2 originalLocation;
        Vector2 originalPointLocation;
        std::chrono::high_resolution_clock::time_point currentTime;
    };

}

#endif //SOLARSYSTEM_GAME_H
