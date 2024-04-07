#include "../AndroidOut.h"
#include "ObjectShader.h"

static const char *vertex = R"vertex(#version 300 es
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
uniform mat3 transform;
out vec3 vertexColor;
void main() {
    vec3 pos = transform * vec3(position.xy, 1.0f);
    gl_Position = vec4(pos.xy, position.z, 1.0f);
    vertexColor = color.rgb;
}
)vertex";

static const char *fragment = R"fragment(#version 300 es
precision mediump float;
in vec3 vertexColor;
out vec3 color;
void main() {
    color = vertexColor;
}
)fragment";

namespace Solar {

    ObjectShader::ObjectShader() : Shader(vertex, fragment)
    {
    }

    void ObjectShader::drawElements(const Buffer &buffer, Allocation alloc,
                                    const float transform[9]) const
    {
        GLuint u = glGetUniformLocation(getProgram(), "transform");
        glUniformMatrix3fv(u, 1, false, transform);
        GLvoid *start = (GLvoid *)(alloc.indexStart * sizeof(unsigned));
        glDrawElements(GL_TRIANGLES, alloc.indexCount, GL_UNSIGNED_INT, start);
    }

    void ObjectShader::drawLines(const Solar::Buffer &buffer, Solar::Allocation alloc,
                                 const float *transform) const
    {
        GLuint u = glGetUniformLocation(getProgram(), "transform");
        glUniformMatrix3fv(u, 1, false, transform);
        GLvoid *start = (GLvoid *)(alloc.vertexStart * sizeof(Vertex));
        glDrawArrays(GL_LINES, alloc.vertexStart, alloc.vertexCount);
    }

}