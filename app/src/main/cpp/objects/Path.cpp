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
        vertices(size*4), indices(size*6), capacity(size*2), width(width)
    {
        color = Color{0.3f, 0.6f, 0.9f};
        addPoint(firstPoint);
    }

    Allocation Path::addPoint(Vector2 point)
    {
        Allocation a;
        if (index % 2 == 1) {
            Vector2 d = point - lastPoint;
            d.normalize();
            Vector2 n1{-d.y*width, d.x*width};
            Vector2 n2{d.y*width, -d.x*width};
            size_t vi = (index-1)*2;
            size_t ii = (index-1)*3;

            vertices[vi+0] = lastPoint + n1;
            vertices[vi+1] = lastPoint + n2;
            vertices[vi+2] = point + n2;
            vertices[vi+3] = point + n1;

            const size_t vs = getAllocation().vertexStart;
            indices[ii+0] = vs+vi+0;
            indices[ii+1] = vs+vi+1;
            indices[ii+2] = vs+vi+2;
            indices[ii+3] = vs+vi+2;
            indices[ii+4] = vs+vi+3;
            indices[ii+5] = vs+vi+0;

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