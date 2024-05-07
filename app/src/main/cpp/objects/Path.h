#ifndef SOLARSYSTEM_PATH_H
#define SOLARSYSTEM_PATH_H

#include "Object.h"

namespace Solar {

    class Path : public Object {
    public:
        Path(size_t capacity, Vector2 firstPoint, float width);
        std::vector<Vector2> getVertices() const;
        std::vector<unsigned> getIndices() const;
        const std::vector<Vector2> &getVerticesRef() const;
        const std::vector<unsigned> &getIndicesRef() const;
        Allocation addPoint(Vector2 point);
        Vector2 getLastPoint() const;

    private:
        void addLine(Vector2 p0, Vector2 p1, size_t vi, size_t ii);

        float width;
        size_t index = 0;
        size_t capacity;
        bool full = false;
        std::vector<Vector2> vertices;
        std::vector<unsigned> indices;
        Vector2 lastPoint;
    };

}

#endif //SOLARSYSTEM_PATH_H
