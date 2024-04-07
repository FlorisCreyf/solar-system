#include "Ship.h"

namespace Solar {

    Ship::Ship()
    {
    }

    std::vector<Vertex> Ship::getVertices() const
    {
        float z = -0.9f;
        std::vector<Vertex> vertices;
        Vector4 color{1.0f, 1.0f, 1.0f, 1.0f};
        vertices.push_back(Vertex(Vector4{0.03, -0.03f, z, 1.0f}, color)); // right
        vertices.push_back(Vertex(Vector4{-0.03, -0.03f, z, 1.0f}, color)); // left
        vertices.push_back(Vertex(Vector4{0.0f, 0.03f, z, 1.0f}, color)); // top
        vertices.push_back(Vertex(Vector4{0.0f, -0.02f, z, 1.0f}, color)); // bottom
        return vertices;
    }

    std::vector<unsigned int> Ship::getIndices() const
    {
        std::vector<unsigned> indices;
        indices.push_back(3);
        indices.push_back(2);
        indices.push_back(1);

        indices.push_back(3);
        indices.push_back(0);
        indices.push_back(2);
        return indices;
    }

    void Ship::getTransformation(float t[9]) const
    {
        Object::getTransformation(t);
        t[0] = std::cos(angle);
        t[1] = -std::sin(angle);
        t[3] = std::sin(angle);
        t[4] = std::cos(angle);
        t[6] = location.x;
        t[7] = location.y;
    }

}