#ifndef SOLARSYSTEM_STAR_H
#define SOLARSYSTEM_STAR_H

#include "Object.h"

namespace Solar {

    class Star : public Object {
    public:
        Star();
        std::vector<Vertex> getVertices() const;
        std::vector<unsigned> getIndices() const;

        float mass = 1.0f;
        float radius = 1.0f;

    private:
        const int N = 100;
    };

}

#endif //SOLARSYSTEM_STAR_H
