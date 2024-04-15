#ifndef SOLARSYSTEM_CIRCLE_H
#define SOLARSYSTEM_CIRCLE_H

#include "Object.h"
#include <string>

namespace Solar {

    class Circle : public Object {
    public:
        Circle(std::string name);
        std::vector<Vector2> getVertices() const;
        std::vector<unsigned> getIndices() const;
        void getTransformation(float transform[9]) const;

        float mass = 1.0f;
        float radius = 1.0f;
        float distance = 0.0f;
        float angle = 0.0f;
        Vector2 location;
        Vector2 velocity;
        const std::string name;

    private:
        const int N = 100;
    };

}

#endif //SOLARSYSTEM_CIRCLE_H
