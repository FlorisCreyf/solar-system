#include "Path.h"

namespace Solar {

    Path::Path(size_t capacity) : points(capacity*2)
    {
        color = Color{1.0f, 1.0f, 1.0f};
    }

    void Path::addPoint(Vector2 point)
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

    std::vector<Vector2> Path::getVertices() const
    {
        return points;
    }

    std::vector<unsigned int> Path::getIndices() const
    {
        return std::vector<unsigned>();
    }

}