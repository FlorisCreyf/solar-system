#ifndef SOLARSYSTEM_SHIP_H
#define SOLARSYSTEM_SHIP_H

#include "Object.h"

namespace Solar {

    class Ship : public Object {
    public:
        Ship();
        std::vector<Vector2> getVertices() const;
        std::vector<unsigned> getIndices() const;
        void getTransformation(float transform[9]) const;
        void getCorners(Vector2 &top, Vector2 &bottomRight, Vector2 &bottomLeft) const;

        float mass = 1.0f;
        Vector2 force = {};
        Vector2 velocity = {};
        Vector2 refVelocity = {};
        Vector2 acceleration = {};
        float angle = 0.0f;
        Vector2 location = {};
    };

}

#endif //SOLARSYSTEM_SHIP_H