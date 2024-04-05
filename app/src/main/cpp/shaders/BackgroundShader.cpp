#include "BackgroundShader.h"

static const char *vertex = R"vertex(#version 300 es
layout(location = 0) in vec4 position;
void main() {
    gl_Position = position;
}
)vertex";

static const char *fragment = R"fragment(#version 300 es
precision mediump float;
uniform ivec2 offset;
uniform ivec2 screen;
uniform int width;
out vec3 color;
void main() {
    int x = int(gl_FragCoord.x) + offset.x + screen.x;
    int y = int(gl_FragCoord.y) + offset.y + screen.y;
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
        GLuint u2 = glGetUniformLocation(getProgram(), "screen");
        GLuint u3 = glGetUniformLocation(getProgram(), "width");
        glUniform2i(u1, x, y);
        if (width < height) {
            glUniform1i(u3, width );
            glUniform2i(u2, 0, (width-height)/2);
        } else {
            glUniform1i(u3, height);
            glUniform2i(u2, (height-width)/2, 0);
        }
        GLvoid *start = (GLvoid *)(alloc.indexStart * sizeof(unsigned));
        glDrawElements(GL_TRIANGLES, alloc.indexCount, GL_UNSIGNED_INT, start);
    }

}