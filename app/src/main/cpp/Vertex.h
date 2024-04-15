#ifndef SOLARSYSTEM_VERTEX_H
#define SOLARSYSTEM_VERTEX_H

#include <cmath>
#include <ostream>

namespace Solar {

    struct Vector2 {
        float x = 0.0f;
        float y = 0.0f;

        Vector2 operator*(float b)
        {
            return Vector2{x * b, y * b};
        }

        Vector2 &operator*=(float b)
        {
            x *= b;
            y *= b;
            return *this;
        }

        Vector2 operator/(float b)
        {
            return Vector2{x / b, y / b};
        }

        Vector2 &operator/=(float b)
        {
            x /= b;
            y /= b;
            return *this;
        }

        Vector2 operator+(const Vector2 &b)
        {
            return Vector2{x + b.x, y + b.y};
        }

        Vector2 &operator+=(const Vector2 &b)
        {
            x += b.x;
            y += b.y;
            return *this;
        }

        Vector2 operator-(const Vector2 &b)
        {
            return Vector2{x - b.x, y - b.y};
        }

        Vector2 &operator-=(const Vector2 &b)
        {
            x -= b.x;
            y -= b.y;
            return *this;
        }

        float magnitude()
        {
            return std::sqrt(x*x + y*y);
        }

        Vector2 normalize()
        {
            float m = magnitude();
            return Vector2{x/m, y/m};
        }
    };

    inline std::ostream &operator<<(std::ostream &os, Vector2 v) {
        os << "(" << v.x << " " << v.y << ")";
        return os;
    }

    inline float dot(Vector2 a, Vector2 b)
    {
        return a.x*b.x + a.y*b.y;
    }

    struct Color {
        float r = 0.0f;
        float g = 0.0f;
        float b = 0.0f;
    };

}

#endif //SOLARSYSTEM_VERTEX_H
