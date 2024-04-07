#ifndef SOLARSYSTEM_PATH_H
#define SOLARSYSTEM_PATH_H

#include "Object.h"

namespace Solar {

    class Path : public Object {
    public:
        Path(size_t capacity);
        std::vector<Vector2> getVertices() const;
        std::vector<unsigned> getIndices() const;
        size_t getIndex() const;
        bool isFull() const;
        void addPoint(Vector2 point);

    private:
        size_t index = 0;
        bool full = false;
        std::vector<Vector2> points;
    };

}

#endif //SOLARSYSTEM_PATH_H
