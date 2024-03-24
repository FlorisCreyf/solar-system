#ifndef SOLARSYSTEM_VERTEX_H
#define SOLARSYSTEM_VERTEX_H

namespace Solar {

    union Vector2 {
        struct {
            float x, y;
        };
        float value[2];
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
