#include "Background.h"

namespace Solar {

    std::vector<Vertex> Background::getVertices() const
    {
        std::vector<Vertex> vertices;
        Vector4 color{1.0f, 1.0f, 1.0f, 1.0f};
        vertices.push_back(Vertex(Vector4{3.0f, -1.0f, 0.0f, 1.0f}, color));
        vertices.push_back(Vertex(Vector4{-1.0f, 3.0f, 0.0f, 1.0f}, color));
        vertices.push_back(Vertex(Vector4{-1.0f, -1.0f, 0.0f, 1.0f}, color));
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