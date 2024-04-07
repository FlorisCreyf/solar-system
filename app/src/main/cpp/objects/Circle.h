#ifndef SOLARSYSTEM_CIRCLE_H
#define SOLARSYSTEM_CIRCLE_H

#include "Object.h"

namespace Solar {

    class Circle : public Object {
    public:
        Circle();
        std::vector<Vertex> getVertices() const;
        std::vector<unsigned> getIndices() const;
        void getTransformation(float transform[9]) const;

        float mass = 1.0f;
        float radius = 1.0f;
        float distance = 0.0f;
        float angle = 0.0f;
        Vector2 location = {};
        Vector4 color = {};

    private:
        const int N = 100;
    };

}

#endif //SOLARSYSTEM_CIRCLE_H
