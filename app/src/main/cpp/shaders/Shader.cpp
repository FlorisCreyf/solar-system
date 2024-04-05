#include "Shader.h"
#include "../AndroidOut.h"
#include "../Buffer.h"
#include <cassert>

namespace Solar {

    Shader::Shader(const std::string &vertexSource, const std::string &fragmentSource) : program(0)
    {
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

    GLuint Shader::getProgram() const
    {
        return program;
    }

}