#ifndef SOLARSYSTEM_RENDERER_H
#define SOLARSYSTEM_RENDERER_H

#include "Buffer.h"
#include "shaders/BackgroundShader.h"
#include "shaders/ObjectShader.h"
#include "Scene.h"
#include <EGL/egl.h>
#include <memory>

extern "C" struct android_app;

namespace Solar {

    class Renderer {
    public:
        Renderer(android_app *app);
        ~Renderer();
        void render(const Scene &scene);
        float getAspect() const;
        int getWidth() const;
        int getHeight() const;

    private:
        android_app *app;
        EGLDisplay display;
        EGLSurface surface;
        EGLContext context;
        EGLint width;
        EGLint height;
        ObjectShader *objectShader;
        BackgroundShader *backgroundShader;

        EGLConfig getConfig();
        void getTransform(float transform[9], const Scene &scene, const Object *object);
        void drawText(const Scene &scene);
    };

}

#endif