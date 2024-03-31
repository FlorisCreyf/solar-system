#ifndef SOLARSYSTEM_SCENE_H
#define SOLARSYSTEM_SCENE_H

#include "Ray.h"
#include "Buffer.h"
#include "Object.h"
#include "Ship.h"
#include "Circle.h"
#include <chrono>

namespace Solar {

    class Scene {
    public:
        Scene();
        ~Scene();
        void load();
        void unload();
        void update(Vector2 location);
        void update(Ray ray);
        void update(double duration);
        Vector2 getLocation() const;
        const Buffer &getBuffer() const;
        std::vector<const Object *> getObjects() const;
        const Ship getShip() const;

    private:
        Buffer buffer;
        Ship ship;
        Circle star;
        std::vector<Circle> planets;
        std::vector<const Object *> objects;
        Vector2 location = {};
    };

}

#endif //SOLARSYSTEM_SCENE_H
