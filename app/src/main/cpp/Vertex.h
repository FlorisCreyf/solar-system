#ifndef SOLARSYSTEM_VERTEX_H
#define SOLARSYSTEM_VERTEX_H

#include <cmath>

namespace Solar {

    struct Vector2 {
        float x = 0.0f;
        float y = 0.0f;

        Vector2 operator*(float b)
        {
            return Vector2{x*b, y*b};
        }

        Vector2 operator/(float b)
        {
            return Vector2{x/b, y/b};
        }

        Vector2 operator+(const Vector2 &b)
        {
            return Vector2{x+b.x, y+b.y};
        }

        Vector2 operator-(const Vector2 &b)
        {
            return Vector2{x-b.x, y-b.y};
        }

        float magnitude()
        {
            return std::sqrt(x*x + y*y);
        }
    };

    struct Vector4 {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        float w = 0.0f;
    };

    struct Vertex {
        Vector4 position;
        Vector4 color;

        Vertex()
        {
        }

        constexpr Vertex(const Vector4 &position, const Vector4 &color) :
            position(position),
            color(color)
        {
        }
    };

}

#endif //SOLARSYSTEM_VERTEX_H
