#include "BackgroundShader.h"

static const char *vertex = R"vertex(#version 300 es
layout(location = 0) in vec2 position;
void main() {
    gl_Position = vec4(position, 0.0f, 1.0f);
}
)vertex";

static const char *fragment = R"fragment(#version 300 es
precision mediump float;
uniform ivec2 offset;
uniform int width;
out vec3 color;
void main() {
    int x = int(gl_FragCoord.x) + offset.x;
    int y = int(gl_FragCoord.y) + offset.y;
    if (x % width == 0 || y % width == 0)
        color = vec3(0.3, 0.3, 0.3);
    else
        color = vec3(0.1, 0.15, 0.2);
}
)fragment";

namespace Solar {

    BackgroundShader::BackgroundShader(int width, int height) :
        Shader(vertex, fragment), width(width), height(height)
    {
    }

    void BackgroundShader::draw(const Solar::Buffer &buffer, Solar::Allocation alloc, int x, int y) const
    {
        GLuint u1 = glGetUniformLocation(getProgram(), "offset");
        GLuint u3 = glGetUniformLocation(getProgram(), "width");
        if (width < height) {
            glUniform1i(u3, width);
            glUniform2i(u1, x, y+(width-height)/2);
        } else {
            glUniform1i(u3, height);
            glUniform2i(u1, x+(height-width)/2, y);
        }
        GLvoid *start = (GLvoid *)(alloc.indexStart * sizeof(unsigned));
        glDrawElements(GL_TRIANGLES, alloc.indexCount, GL_UNSIGNED_INT, start);
    }

}