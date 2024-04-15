#ifndef SOLARSYSTEM_SCENE_H
#define SOLARSYSTEM_SCENE_H

#include "Ray.h"
#include "Buffer.h"
#include "objects/Background.h"
#include "objects/Object.h"
#include "objects/Ship.h"
#include "objects/Circle.h"
#include "objects/Path.h"
#include <chrono>

namespace Solar {

    class Scene {
    public:
        Scene(float pixelSize);
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
        const Circle *getCollidedObject() const;
        const Background getBackground() const;

    private:
        void updatePath();

        Buffer buffer;
        Ship ship;
        Path path;
        Circle planet;
        Background background;
        std::vector<Circle> moons;
        std::vector<const Object *> objects;
        Circle *collidedObject;
        const float pixelSize;
        bool uploaded;
    };

}

#endif //SOLARSYSTEM_SCENE_H
