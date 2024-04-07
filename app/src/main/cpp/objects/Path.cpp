#include "Path.h"

namespace Solar {

    Path::Path(size_t capacity) : points(capacity*2)
    {
    }

    void Path::addPoint(Solar::Vertex point)
    {
        points[index] = point;
        index = (index + 1) % points.size();
        if (index == 0)
            full = true;
    }

    bool Path::isFull() const
    {
        return full;
    }

    size_t Path::getIndex() const
    {
        return index;
    }

    std::vector<Vertex> Path::getVertices() const
    {
        return points;
    }

    std::vector<unsigned int> Path::getIndices() const
    {
        return std::vector<unsigned>();
    }

}