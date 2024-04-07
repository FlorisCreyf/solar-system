# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

cmake_minimum_required(VERSION 3.22.1)

project("solarsystem")

find_package(game-activity REQUIRED CONFIG)
find_package(games-frame-pacing REQUIRED CONFIG)

add_definitions("-DIMGUI_IMPL_OPENGL_ES3")

set(IMGUI_DIR "../../../../thirdparty/imgui")
add_library(imgui STATIC
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PRIVATE ${IMGUI_DIR})

add_library(solarsystem SHARED
        main.cpp
        AndroidOut.cpp
        Buffer.cpp
        Game.cpp
        Renderer.cpp
        Scene.cpp
        shaders/Shader.cpp
        shaders/ObjectShader.cpp
        shaders/BackgroundShader.cpp
        objects/Ship.cpp
        objects/Circle.cpp
        objects/Background.cpp
        objects/Path.cpp
)
target_include_directories(solarsystem PRIVATE
        ${IMGUI_DIR}
)

target_link_libraries(solarsystem
        game-activity::game-activity
        games-frame-pacing::swappy_static
        EGL
        GLESv3
        jnigraphics
        android
        log
        imgui
)