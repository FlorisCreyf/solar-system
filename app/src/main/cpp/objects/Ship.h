#ifndef SOLARSYSTEM_SHIP_H
#define SOLARSYSTEM_SHIP_H

#include "Object.h"

namespace Solar {

    class Ship : public Object {
    public:
        Ship();
        std::vector<Vertex> getVertices() const;
        std::vector<unsigned> getIndices() const;
        void getTransformation(float transform[9]) const;

        float mass = 1.0f;
        Vector2 force = {0.0f, 0.0f};
        Vector2 velocity = {0.0, 0.0};
        Vector2 acceleration = {};

        float angle = 0.0f;
        Vector2 location = {};
    };

}

#endif //SOLARSYSTEM_SHIP_H