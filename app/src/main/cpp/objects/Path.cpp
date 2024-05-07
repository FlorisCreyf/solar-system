#include "Path.h"
#include "../AndroidOut.h"

// Points:    0      1       2      3
//            *------*       *------*
//            |      |       |      |
//            *------*       *------*
// Vertices:  2      4       6      8
// Indices:   0      6       6      12

namespace Solar {

    Path::Path(size_t size, Vector2 firstPoint, float width) :
        vertices(size*4),
        indices(size*6),
        capacity(size*2),
        width(width)
    {
        color = Color{0.3f, 0.6f, 0.9f};
        addPoint(firstPoint);
    }

    void Path::addLine(Solar::Vector2 p0, Solar::Vector2 p1, size_t vi, size_t ii)
    {
        Vector2 d = (p1 - p0).normalize();
        Vector2 n1{-d.y*width, d.x*width};
        Vector2 n2{d.y*width, -d.x*width};
        vertices[vi+0] = p0 + n1;
        vertices[vi+1] = p0 + n2;
        vertices[vi+2] = p1 + n2;
        vertices[vi+3] = p1 + n1;
        indices[ii+0] = vi+0;
        indices[ii+1] = vi+1;
        indices[ii+2] = vi+2;
        indices[ii+3] = vi+2;
        indices[ii+4] = vi+3;
        indices[ii+5] = vi+0;
    }

    Allocation Path::addPoint(Vector2 point)
    {
        Allocation a;
        if (index % 2 == 1) {
            size_t vi = (index-1)*2;
            size_t ii = (index-1)*3;
            addLine(lastPoint, point, vi, ii);
            a.vertexStart = vi;
            a.indexStart = ii;
            a.vertexCount = 4;
            a.indexCount = 6;
        };

        lastPoint = point;
        index = (index + 1) % capacity;
        return a;
    }

    Vector2 Path::getLastPoint() const
    {
        return lastPoint;
    }

    std::vector<Vector2> Path::getVertices() const
    {
        return vertices;
    }

    std::vector<unsigned int> Path::getIndices() const
    {
        return indices;
    }

    const std::vector<Vector2> &Path::getVerticesRef() const
    {
        return vertices;
    }

    const std::vector<unsigned int> &Path::getIndicesRef() const
    {
        return indices;
    }

}