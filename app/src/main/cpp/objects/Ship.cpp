#include "Ship.h"

namespace Solar {

    Ship::Ship()
    {
        color = Color{1.0f, 1.0f, 1.0f};
    }

    std::vector<Vector2> Ship::getVertices() const
    {
        std::vector<Vector2> vertices;
        vertices.push_back(Vector2{0.03, -0.03f}); // right
        vertices.push_back(Vector2{-0.03, -0.03f}); // left
        vertices.push_back(Vector2{0.0f, 0.03f}); // top
        vertices.push_back(Vector2{0.0f, -0.02f}); // bottom
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

    void Ship::getCorners(Vector2 &top, Vector2 &bottomRight, Vector2 &bottomLeft) const
    {
        float d = 0.03f;
        float sn = std::sin(angle) * 0.03f;
        float cn = std::cos(angle) * 0.03f;
        top = Vector2{sn, cn} + location;
        bottomRight = Vector2{cn-sn, -cn-sn} + location;
        bottomLeft = Vector2{-cn-sn, -cn+sn} + location;
        // Without rotation:
        // top = Vector2{0.0f, d} + location;
        // bottomRight = Vector2{d, -d} + location;
        // bottomLeft = Vector2{-d, -d} + location;
    }

}