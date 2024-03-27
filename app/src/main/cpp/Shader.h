#ifndef SHADER_H
#define SHADER_H

#include "Buffer.h"
#include "Object.h"
#include <GLES3/gl3.h>
#include <string>

namespace Solar {

    class Shader {
    public:
        Shader();
        ~Shader();
        void activate() const;
        void deactivate() const;
        void draw(const Buffer &buffer, Allocation alloc, const float transform[9]) const;

    private:
        GLuint compileShader(GLenum shaderType, const std::string &shaderSource);

        GLuint program;
    };

}

#endif