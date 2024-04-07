#ifndef SOLARSYSTEM_BACKGROUND_H
#define SOLARSYSTEM_BACKGROUND_H

#include "Object.h"

namespace Solar {

    class Background : public Object {
    public:
        std::vector<Vector2> getVertices() const;
        std::vector<unsigned> getIndices() const;
    };

}

#endif //SOLARSYSTEM_BACKGROUND_H
