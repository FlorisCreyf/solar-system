#include "Background.h"

namespace Solar {

    std::vector<Vector2> Background::getVertices() const
    {
        std::vector<Vector2> vertices;
        vertices.push_back(Vector2{3.0f, -1.0f});
        vertices.push_back(Vector2{-1.0f, 3.0f});
        vertices.push_back(Vector2{-1.0f, -1.0f});
        return vertices;
    }

    std::vector<unsigned int> Background::getIndices() const
    {
        std::vector<unsigned> indices;
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        return indices;
    }

}