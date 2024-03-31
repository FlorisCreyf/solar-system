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
        virtual std::vector<Vertex> getVertices() const = 0;
        virtual std::vector<unsigned> getIndices() const = 0;

        virtual void getTransformation(float transform[9]) const
        {
            transform[0] = 1.0f;
            transform[1] = 0.0f;
            transform[2] = 0.0f;
            transform[3] = 0.0f;
            transform[4] = 1.0f;
            transform[5] = 0.0f;
            transform[6] = 0.0f;
            transform[7] = 0.0f;
            transform[8] = 1.0f;
        }

        Allocation getAllocation() const
        {
            return alloc;
        }

    private:
        Allocation alloc;
        float transform[9] = {
                1, 0, 0,
                0, 1, 0,
                0, 0, 1
        };

        friend class Buffer;
    };

}

#endif //SOLARSYSTEM_OBJECT_H
