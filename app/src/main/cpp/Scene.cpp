#include "Scene.h"
#include "Buffer.h"
#include "AndroidOut.h"
#include <cmath>
#include <assert.h>

namespace Solar {

    Scene::Scene()
    {
        star.mass = 5.0f;
        star.radius = 1.0f;
        ship.mass = 1.0f;
        ship.location.x = location.x = 0.0f;
        ship.location.y = location.y = 0.0f;
        planets.resize(5);
    }

    Scene::~Scene()
    {
    }

    void Scene::load()
    {
        if (objects.empty()) {
            buffer.add(ship);
            buffer.add(star);
            objects.push_back(&ship);
            objects.push_back(&star);

            float i = star.radius;
            for (auto &planet : planets) {
                i += 1.0f;
                planet.radius = 0.5f / i;
                planet.distance = static_cast<float>(i);
                planet.angle = static_cast<float>(i);
                planet.color = Vector4{0.9f, 0.5f, 0.6f, 1.0f};
                planet.mass = 0.4f;
                objects.push_back(&planet);
                buffer.add(planet);
            }

            buffer.upload();
        }
    }

    void Scene::unload()
    {
        objects.clear();
        buffer.clear();
    }

    void Scene::update(Vector2 loc)
    {
        location = loc;
    }

    void Scene::update(Ray ray)
    {
        Vector2 v = ray.to - ray.from;
        ship.force = v * 4.0f;
        float m = v.magnitude();
        if (m > 0.1f) {
            v = v / v.magnitude();
            ship.angle = std::atan(v.x / v.y);
            if (v.y < 0.0f)
                ship.angle = ship.angle + M_PI;
        }
    }

    void addForce(Vector2 &force, Vector2 p1, Vector2 p2, float m1, float m2, float radius)
    {
        float fx = 0.0f;
        float fy = 0.0f;
        float dx = p1.x - p2.x;
        float dy = p1.y - p2.y;
        float r = std::sqrt(dx*dx + dy*dy);
        fx -= m1 * m2 / (r*r);
        fy -= m1 * m2 / (r*r);
        fx *= dx / r;
        fy *= dy / r;
        if (r < radius) {
            fx = 0.0f;
            fy = 0.0f;
        }
        force.x += fx;
        force.y += fy;
    }

    void Scene::update(double duration)
    {
        float i = 0.0f;
        Vector2 force = ship.force;
        Vector2 pos = ship.location;

        for (auto &planet : planets) {
            i += 1.0f;
            planet.angle += duration;
            planet.location.x = std::cos(planet.angle/i) * planet.distance;
            planet.location.y = std::sin(planet.angle/i) * planet.distance;
            addForce(force, ship.location, planet.location, ship.mass, planet.mass, planet.radius);
        }

        addForce(force, ship.location, star.location, ship.mass, star.mass, star.radius);
        ship.acceleration.x = force.x / ship.mass;
        ship.acceleration.y = force.y / ship.mass;
        ship.velocity.x += ship.acceleration.x * duration;
        ship.velocity.y += ship.acceleration.y * duration;
        pos.x += ship.velocity.x*duration;
        pos.y += ship.velocity.y*duration;

        ship.location = pos;
        location = pos;
    }

    Vector2 Scene::getLocation() const
    {
        return location;
    }

    std::vector<const Object *> Scene::getObjects() const
    {
        return objects;
    }

    const Ship Scene::getShip() const
    {
        return ship;
    }

    const Buffer &Scene::getBuffer() const
    {
        return buffer;
    }

}