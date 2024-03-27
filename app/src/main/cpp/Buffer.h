#ifndef MODEL_H
#define MODEL_H

#include "Object.h"
#include <GLES3/gl3.h>
#include <vector>

namespace Solar {

    class Buffer {
    public:
        Buffer();
        ~Buffer();
        void add(Object &object);
        void upload();
        void clear();
        void bind() const;
        void unbind() const;

    private:
        void deleteVAO();

        GLuint vao;
        GLuint buffers[2];

        std::vector<Vertex> vertices;
        std::vector<unsigned> indices;
    };

}

#endif