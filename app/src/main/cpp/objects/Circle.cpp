#include "Circle.h"
#include <cmath>

namespace Solar {

    Circle::Circle() : N(100), color{0.4f, 0.5f, 0.6f, 1.0f}
    {
    }

    std::vector<Vertex> Circle::getVertices() const
    {
        std::vector<Vertex> vertices;
        float theta = 0.0f;
        float z = -0.1f;
        float delta = 2.0f * M_PI / static_cast<float>(N);
        vertices.push_back(Vertex(Vector4{0.0f, 0.0f, z, 1.0f}, color));
        for (int i = 0; i < N; i++) {
            float x = std::cosf(theta) * radius;
            float y = std::sinf(theta) * radius;
            Vector4 position{x, y, z, 1.0f};
            vertices.push_back(Vertex(position, color));
            theta += delta;
        }
        return vertices;
    }

    std::vector<unsigned int> Circle::getIndices() const
    {
        std::vector<unsigned> indices;
        unsigned j = 1;
        for (int i = 0; i < N-1; i++) {
            indices.push_back(j + 0);
            indices.push_back(j + 1);
            indices.push_back(0);
            j++;
        }
        indices.push_back(N);
        indices.push_back(1);
        indices.push_back(0);
        return indices;
    }

    void Circle::getTransformation(float t[9]) const
    {
        Object::getTransformation(t);
        t[6] = location.x;
        t[7] = location.y;
    }

}