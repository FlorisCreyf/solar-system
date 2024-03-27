#ifndef SOLARSYSTEM_OBJECT_H
#define SOLARSYSTEM_OBJECT_H

#include "Vertex.h"
#include <vector>

namespace Solar {

    struct Allocation {
        size_t vertexStart = 0;
        size_t vertexCount = 0;
        size_t indexStart = 0;
        size_t indexCount = 0;
    };

    class Object {
    public:
        float transform[9] = {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f};

        virtual std::vector<Vertex> getVertices() const = 0;
        virtual std::vector<unsigned> getIndices() const = 0;
        Allocation getAllocation() const
        {
            return alloc;
        }

    private:
        Allocation alloc;

        friend class Buffer;
    };

}

#endif //SOLARSYSTEM_OBJECT_H
