#include "Star.h"
#include <cmath>

namespace Solar {

    Star::Star() : N(100)
    {
        transform[0] = 0.5f;
        transform[4] = 0.5f;
    }

    std::vector<Vertex> Star::getVertices() const
    {
        std::vector<Vertex> vertices;
        Vector4 color{0.4f, 0.5f, 0.6f, 1.0f};
        float angle = 0.0f;
        float z = -0.1f;
        float delta = 2.0f * M_PI / static_cast<float>(N);
        vertices.push_back(Vertex(Vector4{0.0f, 0.0f, z, 1.0f}, color));
        for (int i = 0; i < N; i++) {
            Vector4 position{std::cosf(angle), std::sinf(angle), z, 1.0f};
            vertices.push_back(Vertex(position, color));
            angle += delta;
        }
        return vertices;
    }

    std::vector<unsigned int> Star::getIndices() const
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

}