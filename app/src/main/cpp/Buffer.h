#ifndef MODEL_H
#define MODEL_H

#include "objects/Object.h"
#include <GLES3/gl3.h>
#include <vector>

namespace Solar {

    class Buffer {
    public:
        Buffer();
        ~Buffer();
        void add(Object &object);
        void upload();
        void update(std::vector<Vector2> vertices, size_t index);
        void clear();
        void bind() const;
        void unbind() const;

    private:
        void deleteVAO();

        GLuint vao;
        GLuint buffers[2];

        std::vector<Vector2> vertices;
        std::vector<unsigned> indices;
    };

}

#endif