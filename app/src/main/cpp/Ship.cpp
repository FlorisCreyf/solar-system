#include "Ship.h"

namespace Solar {

    Ship::Ship()
    {
        transform[0] = 0.05f; // scale x
        transform[4] = 0.1f; // scale y
        transform[6] = 0.0f; // translate x
        transform[7] = 0.0f; // translate y
    }

    void Ship::setLocation(float x, float y)
    {
        transform[6] = x;
        transform[7] = y;
    }

    std::vector<Vertex> Ship::getVertices() const
    {
        std::vector<Vertex> vertices;
        vertices.push_back(Vertex(Vector4{1.0f, 0.0f, -0.2f, 1.0f}, Vector4{1.0f, 1.0f, 1.0f, 1.0f}));
        vertices.push_back(Vertex(Vector4{-1.0f, 0.0f, -0.2f, 1.0f}, Vector4{1.0f, 1.0f, 1.0f, 1.0f}));
        vertices.push_back(Vertex(Vector4{0.0f, 1.0f, -0.2f, 1.0f}, Vector4{1.0f, 1.0f, 1.0f, 1.0f}));
        return vertices;
    }

    std::vector<unsigned int> Ship::getIndices() const
    {
        std::vector<unsigned> indices;
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        return indices;
    }

}