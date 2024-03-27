#ifndef SOLARSYSTEM_RENDERER_H
#define SOLARSYSTEM_RENDERER_H

#include "Buffer.h"
#include "Shader.h"
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
        Shader *shader;

        EGLConfig getConfig();
    };

}

#endif