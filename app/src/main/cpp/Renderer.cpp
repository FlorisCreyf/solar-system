#include "AndroidOut.h"
#include "Matrix.h"
#include "Renderer.h"
#include "swappy/swappyGL.h"
#include "swappy/swappyGL_extra.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"
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
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LEQUAL);
        glLineWidth(2);
        assert(glGetError() == GL_NO_ERROR);

        assert(eglQuerySurface(display, surface, EGL_WIDTH, &width) == EGL_TRUE);
        assert(eglQuerySurface(display, surface, EGL_HEIGHT, &height) == EGL_TRUE);

        objectShader = new ObjectShader();
        backgroundShader = new BackgroundShader(width, height);

        ImGui::StyleColorsDark();
        ImGui_ImplOpenGL3_Init(NULL);
        ImGuiIO &io = ImGui::GetIO();
        io.DisplaySize = ImVec2(width, height);
    }

    Renderer::~Renderer()
    {
        ImGui_ImplOpenGL3_Shutdown();
        delete objectShader;
        delete backgroundShader;
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
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        assert(glGetError() == GL_NO_ERROR);

        scene.getBuffer().bind();

        backgroundShader->activate();
        Background background = scene.getBackground();
        Vector2 location = scene.getLocation();
        float x = location.x/2.0f * width;
        float y = location.y/2.0f * height;
        if (width < height)
            y *= getAspect();
        else
            x *= getAspect();
        backgroundShader->draw(scene.getBuffer(), background.getAllocation(), x, y);

        objectShader->activate();
        auto objects = scene.getObjects();
        for (const Object *object : objects) {
            float transform[9];
            getTransform(transform, scene, object);
            objectShader->drawElements(scene.getBuffer(), object->getAllocation(), object->color, transform);
        }
        {
            Path path = scene.getPath();
            float transform[9];
            getTransform(transform, scene, &path);
            Allocation alloc;
            alloc = path.getAllocation();
            alloc.vertexCount = path.getIndex();
            objectShader->drawLines(scene.getBuffer(), alloc, path.color, transform);
            if (path.isFull()) {
                alloc = path.getAllocation();
                alloc.vertexStart += path.getIndex();
                alloc.vertexCount -= path.getIndex();
                objectShader->drawLines(scene.getBuffer(), alloc, path.color, transform);
            }
        }
        scene.getBuffer().unbind();

        drawText(scene);

        if (SwappyGL_isEnabled())
            SwappyGL_swap(display, surface);
        else
            eglSwapBuffers(display, surface);
    }

    void Renderer::getTransform(float transform[9], const Scene &scene, const Solar::Object *object)
    {
        Vector2 location = scene.getLocation();
        float t1[9] = {};
        float t2[9] = {};
        object->getTransformation(t1);
        float aspect = getAspect();
        t2[0] = 1.0f;
        t2[4] = 1.0f;
        t2[8] = 1.0f;
        if (width < height) {
            t2[4] = aspect;
            t2[6] = -location.x;
            t2[7] = -location.y * aspect;
        } else {
            t2[0] = aspect;
            t2[6] = -location.x * aspect;
            t2[7] = -location.y;
        }
        multiply_mat3(transform, t2, t1);
    }

    void Renderer::drawText(const Solar::Scene &scene)
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();
        ImGuiIO& io = ImGui::GetIO();
        ImVec2 windowPosition(0, 0);
        ImVec2 windowSize(io.DisplaySize.x, io.DisplaySize.y);
        ImGui::SetNextWindowPos(windowPosition);
        ImGui::SetNextWindowSizeConstraints(windowSize, windowSize, NULL, NULL);
        ImGuiWindowFlags windowFlags =
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground |
            ImGuiWindowFlags_NoTitleBar;
        ImGui::Begin("coords", nullptr, windowFlags);
        ImGui::Text("Position: (%.3f %.3f)", scene.getLocation().x, scene.getLocation().y);
        Vector2 velocity = scene.getShip().velocity;
        ImGui::Text("Velocity: %.3f", sqrt(velocity.x*velocity.x + velocity.y*velocity.y));
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
