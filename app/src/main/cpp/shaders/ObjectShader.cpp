#include "../AndroidOut.h"
#include "ObjectShader.h"

static const char *vertex = R"vertex(#version 300 es
layout(location = 0) in vec2 position;
uniform mat3 transform;
void main() {
    vec3 pos = transform * vec3(position, 1.0f);
    gl_Position = vec4(pos.xy, 0.0f, 1.0f);
}
)vertex";

static const char *fragment = R"fragment(#version 300 es
precision mediump float;
uniform vec3 color;
out vec3 fragColor;
void main() {
    fragColor = color;
}
)fragment";

namespace Solar {

    ObjectShader::ObjectShader() : Shader(vertex, fragment)
    {
    }

    void ObjectShader::drawElements(const Buffer &buffer, Allocation alloc, Color color,
                                    const float transform[9]) const
    {
        GLuint u1 = glGetUniformLocation(getProgram(), "transform");
        GLuint u2 = glGetUniformLocation(getProgram(), "color");
        glUniformMatrix3fv(u1, 1, false, transform);
        glUniform3f(u2, color.r, color.g, color.b);
        GLvoid *start = (GLvoid *)(alloc.indexStart * sizeof(unsigned));
        glDrawElements(GL_TRIANGLES, alloc.indexCount, GL_UNSIGNED_INT, start);
    }

    void ObjectShader::drawLines(const Solar::Buffer &buffer, Solar::Allocation alloc, Color color,
                                 const float *transform) const
    {
        GLuint u1 = glGetUniformLocation(getProgram(), "transform");
        GLuint u2 = glGetUniformLocation(getProgram(), "color");
        glUniformMatrix3fv(u1, 1, false, transform);
        glUniform3f(u2, color.r, color.g, color.b);
        glDrawArrays(GL_LINES, alloc.vertexStart, alloc.vertexCount);
    }

}
