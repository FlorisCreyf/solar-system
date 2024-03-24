#include "Shader.h"
#include "AndroidOut.h"
#include "Buffer.h"
#include <cassert>

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

    Shader::Shader() : program(0)
    {
        out << "Creating shader" << std::endl;

        const std::string vertexSource(vertex);
        const std::string fragmentSource(fragment);

        GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
        if (!vertexShader) {
            return;
        }

        GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
        if (!fragmentShader) {
            glDeleteShader(vertexShader);
            return;
        }

        program = glCreateProgram();
        if (program) {
            glAttachShader(program, vertexShader);
            glAttachShader(program, fragmentShader);
            glLinkProgram(program);
            GLint linkStatus = GL_FALSE;
            glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
            if (linkStatus != GL_TRUE) {
                GLint logLength = 0;
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
                if (logLength) {
                    GLchar *log = new GLchar[logLength];
                    glGetProgramInfoLog(program, logLength, nullptr, log);
                    out << "Failed to link program with:\n" << log << std::endl;
                    delete[] log;
                }
                glDeleteProgram(program);
                program = 0;
            }
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    Shader::~Shader()
    {
        out << "Deleting shader" << std::endl;
        if (program) {
            glDeleteProgram(program);
            program = 0;
        }
    }

    GLuint Shader::compileShader(GLenum shaderType, const std::string &shaderSource)
    {
        GLuint shader = glCreateShader(shaderType);
        if (shader) {
            auto *shaderRawString = (GLchar *) shaderSource.c_str();
            GLint shaderLength = shaderSource.length();
            glShaderSource(shader, 1, &shaderRawString, &shaderLength);
            glCompileShader(shader);

            GLint shaderCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderCompiled);

            if (!shaderCompiled) {
                GLint infoLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);

                if (infoLength) {
                    auto *infoLog = new GLchar[infoLength];
                    glGetShaderInfoLog(shader, infoLength, nullptr, infoLog);
                    out << "Failed to compile with:\n" << infoLog << std::endl;
                    delete[] infoLog;
                }

                glDeleteShader(shader);
                shader = 0;
            }
        }
        return shader;
    }

    void Shader::activate() const
    {
        glUseProgram(program);
    }

    void Shader::deactivate() const
    {
        glUseProgram(0);
    }

    void Shader::draw(const Buffer &buffer, Allocation alloc, const float transform[9]) const
    {
        GLuint u = glGetUniformLocation(program, "transform");
        glUniformMatrix3fv(u, 1, false, transform);
        GLvoid *start = (GLvoid *)(alloc.indexStart * sizeof(unsigned));
        glDrawElements(GL_TRIANGLES, alloc.indexCount, GL_UNSIGNED_INT, start);
    }

}