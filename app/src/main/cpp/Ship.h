#ifndef SOLARSYSTEM_SHIP_H
#define SOLARSYSTEM_SHIP_H

#include "Object.h"

namespace Solar {

    class Ship : public Object {
    public:
        Ship();
        void setLocation(float x, float y);
        std::vector<Vertex> getVertices() const;
        std::vector<unsigned> getIndices() const;

        float mass = 1.0f;
    };

}

#endif //SOLARSYSTEM_SHIP_H