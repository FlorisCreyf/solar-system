#ifndef SOLARSYSTEM_VERTEX_H
#define SOLARSYSTEM_VERTEX_H

#include <cmath>

namespace Solar {

    union Vector2 {
        struct {
            float x, y;
        };
        float value[2];

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

    union Vector4 {
        struct {
            float x, y, z, w;
        };
        struct {
            float r, g, b, a;
        };
        float value[4];
    };

    struct Vertex {
        Vector4 position;
        Vector4 color;

        constexpr Vertex(const Vector4 &position, const Vector4 &color) :
            position(position),
            color(color)
        {
        }
    };

}

#endif //SOLARSYSTEM_VERTEX_H
