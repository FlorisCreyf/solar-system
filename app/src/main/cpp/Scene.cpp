#include "Scene.h"
#include "Buffer.h"
#include "AndroidOut.h"
#include <cmath>

namespace Solar {

    Scene::Scene()
    {
    }

    Scene::~Scene()
    {
    }

    void Scene::load()
    {
        if (objects.empty()) {
            out << "Creating scene" << std::endl;
            buffer.add(ship);
            buffer.add(star);
            objects.push_back(&ship);
            objects.push_back(&star);
            buffer.upload();
        }
    }

    void Scene::unload()
    {
        out << "Unloading scene" << std::endl;
        objects.clear();
        buffer.clear();
    }

    void Scene::update(Solar::Arrow arrow)
    {
        float dx = arrow.x2 - arrow.x1;
        float dy = arrow.y2 - arrow.y1;
        location.x = startLocation.x + dx;
        location.y = startLocation.y + dy;
        ship.setLocation(-location.x, -location.y);
    }

    void Scene::initUpdate()
    {
        startLocation = location;
    }

    Vector2 Scene::getLocation() const
    {
        return location;
    }

    std::vector<const Object *> Scene::getObjects() const
    {
        return objects;
    }

    const Buffer &Scene::getBuffer() const
    {
        return buffer;
    }

}