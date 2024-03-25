#ifndef SOLARSYSTEM_GAME_H
#define SOLARSYSTEM_GAME_H

#include <game-activity/native_app_glue/android_native_app_glue.h>

#include "AndroidOut.h"
#include "Renderer.h"
#include "Scene.h"

namespace Solar {

    class Game {
    public:
        Game(android_app *app);
        ~Game();
        void execute();
        void processInput();
        void init(android_app *app);
        void release();

        JNIEnv *jniEnv;

    private:
        android_app *app;
        Renderer *renderer;
        Scene scene;
        float xDown;
        float yDown;
    };

}

#endif //SOLARSYSTEM_GAME_H
