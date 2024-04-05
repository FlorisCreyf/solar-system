#ifndef SHADER_H
#define SHADER_H

#include "../Buffer.h"
#include "../Object.h"
#include <GLES3/gl3.h>
#include <string>

namespace Solar {

    class Shader {
    public:
    protected:
        Shader(const std::string &vertexSource, const std::string &fragmentSource);
        ~Shader();
        GLuint getProgram() const;

    public:
        void activate() const;
        void deactivate() const;

    private:
        GLuint compileShader(GLenum shaderType, const std::string &shaderSource);

        GLuint program;
    };

}

#endif