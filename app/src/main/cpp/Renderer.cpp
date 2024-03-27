#include "AndroidOut.h"
#include "Renderer.h"
#include "swappy/swappyGL.h"
#include "swappy/swappyGL_extra.h"
#include <game-activity/native_app_glue/android_native_app_glue.h>
#include <GLES3/gl3.h>
#include <cassert>
#include <vector>

namespace Solar {

    Renderer::Renderer(android_app *app) :
        app(app),
        display(EGL_NO_DISPLAY),
        surface(EGL_NO_SURFACE),
        context(EGL_NO_CONTEXT),
        width(0),
        height(0)
    {
        display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        assert(eglInitialize(display, nullptr, nullptr) == EGL_TRUE);
        EGLConfig config = getConfig();
        surface = eglCreateWindowSurface(display, config, app->window, nullptr);
        EGLint attributes[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};
        context = eglCreateContext(display, config, nullptr, &attributes[0]);
        assert(eglMakeCurrent(display, surface, surface, context));

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LEQUAL);
        assert(glGetError() == GL_NO_ERROR);

        shader = new Shader;
    }

    Renderer::~Renderer()
    {
        delete shader;
        if (display != EGL_NO_DISPLAY) {
            assert(eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT) == EGL_TRUE);
            if (context != EGL_NO_CONTEXT)
                assert(eglDestroyContext(display, context) == EGL_TRUE);
            if (surface != EGL_NO_SURFACE)
                assert(eglDestroySurface(display, surface) == EGL_TRUE);
            assert(eglTerminate(display) == EGL_TRUE);
        }
    }

    EGLConfig Renderer::getConfig()
    {
        constexpr EGLint attributes[] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_DEPTH_SIZE, 24,
            EGL_NONE
        };
        EGLint numConfigs;
        assert(eglChooseConfig(display, attributes, nullptr, 0, &numConfigs) == EGL_TRUE);
        std::vector<EGLConfig> configs(numConfigs);
        assert(eglChooseConfig(display, attributes, configs.data(), numConfigs, &numConfigs) == EGL_TRUE);
        EGLConfig config = configs[numConfigs - 1];
        for (size_t i = 0; i < numConfigs - 1; i++) {
            EGLint red;
            EGLint green;
            EGLint blue;
            EGLint depth;
            assert(eglGetConfigAttrib(display, configs[i], EGL_RED_SIZE, &red) == EGL_TRUE);
            assert(eglGetConfigAttrib(display, configs[i], EGL_BLUE_SIZE, &green) == EGL_TRUE);
            assert(eglGetConfigAttrib(display, configs[i], EGL_BLUE_SIZE, &blue) == EGL_TRUE);
            assert(eglGetConfigAttrib(display, configs[i], EGL_DEPTH_SIZE, &depth) == EGL_TRUE);
            if (red == 8 && green == 8 && blue == 8 && depth == 24)
                config = configs[i];
        }
        return config;
    }

    void Renderer::render(const Scene &scene)
    {
        assert(glGetError() == GL_NO_ERROR);
        assert(eglQuerySurface(display, surface, EGL_WIDTH, &width) == EGL_TRUE);
        assert(eglQuerySurface(display, surface, EGL_HEIGHT, &height) == EGL_TRUE);

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        assert(glGetError() == GL_NO_ERROR);

        auto objects = scene.getObjects();
        shader->activate();
        scene.getBuffer().bind();
        Vector2 location = scene.getLocation();
        for (const Object *object : objects) {
            float transform[9];
            memcpy(transform, object->transform, sizeof(float) * 9);
            float aspect = getAspect();
            if (width < height) {
                transform[4] *= aspect;
                transform[7] *= aspect;
                transform[6] += location.x;
                transform[7] += location.y * aspect;
            } else {
                transform[0] *= aspect;
                transform[6] *= aspect;
                transform[6] += location.x * aspect;
                transform[7] += location.y;
            }
            shader->draw(scene.getBuffer(), object->getAllocation(), transform);
        }
        scene.getBuffer().unbind();
        shader->deactivate();
        if (SwappyGL_isEnabled())
            SwappyGL_swap(display, surface);
        else
            eglSwapBuffers(display, surface);
    }

    float Renderer::getAspect() const
    {
        if (width < height)
            return static_cast<float>(width) / static_cast<float>(height);
        else
            return static_cast<float>(height) / static_cast<float>(width);
    }

    int Renderer::getWidth() const
    {
        return width;
    }

    int Renderer::getHeight() const
    {
        return height;
    }

}
