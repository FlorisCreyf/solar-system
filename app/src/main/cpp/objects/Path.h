#ifndef SOLARSYSTEM_PATH_H
#define SOLARSYSTEM_PATH_H

#include "Object.h"

namespace Solar {

    class Path : public Object {
    public:
        Path(size_t capacity);
        std::vector<Vertex> getVertices() const;
        std::vector<unsigned> getIndices() const;
        size_t getIndex() const;
        bool isFull() const;
        void addPoint(Vertex point);

    private:
        size_t index = 0;
        bool full = false;
        std::vector<Vertex> points;
    };

}

#endif //SOLARSYSTEM_PATH_H
