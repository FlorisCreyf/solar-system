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

    struct Color {
        float r = 0.0f;
        float g = 0.0f;
        float b = 0.0f;
    };

}

#endif //SOLARSYSTEM_VERTEX_H
