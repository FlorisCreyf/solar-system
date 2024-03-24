#ifndef SOLARSYSTEM_SCENE_H
#define SOLARSYSTEM_SCENE_H

#include "Arrow.h"
#include "Buffer.h"
#include "Object.h"
#include "Ship.h"
#include "Star.h"

namespace Solar {

    class Scene {
    public:
        Scene();
        ~Scene();
        void load();
        void unload();
        void initUpdate();
        void update(Arrow arrow);
        Vector2 getLocation() const;
        const Buffer &getBuffer() const;
        std::vector<const Object *> getObjects() const;

    private:
        Buffer buffer;
        Ship ship;
        Star star;
        std::vector<const Object *> objects;
        Vector2 location = {};
        Vector2 startLocation = {};
    };

}

#endif //SOLARSYSTEM_SCENE_H
