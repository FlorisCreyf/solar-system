#include "Scene.h"
#include "Buffer.h"
#include "AndroidOut.h"
#include <cmath>
#include <assert.h>

namespace Solar {

    Scene::Scene(float pixelSize) :
        planet("Planet"),
        path(1000, Vector2{0.0f, 3.5f}, pixelSize*4.0f),
        collidedObject(nullptr),
        pixelSize(pixelSize),
        uploaded(false)
    {
        planet.mass = 5.0f;
        planet.radius = 1.0f;
        planet.velocity = {0.0f, 0.1f};
        ship.mass = 1.0f;
        ship.location = {0.0f, 3.5f};
        ship.velocity = {0.0f, 0.1f};
        float r = planet.radius;
        for (size_t i = 1; i < 5; i++) {
            Circle moon("Moon-" + std::to_string(i));
            r += 1.0f;
            moon.radius = 0.5f / r;
            moon.distance = static_cast<float>(r) + moon.radius;
            moon.angle = static_cast<float>(r);
            moon.color = Color{0.9f, 0.5f, 0.6f};
            moon.mass = M_PI * moon.radius * moon.radius;
            moon.location = {0.0f, moon.distance};
            moons.push_back(moon);
        }
        for (auto &moon : moons)
            objects.push_back(&moon);
        objects.push_back(&planet);
        objects.push_back(&path);
        objects.push_back(&ship);
    }

    void Scene::load()
    {
        if (!uploaded) {
            buffer.add(path);
            buffer.add(background);
            buffer.add(ship);
            buffer.add(planet);
            for (auto &moon : moons)
                buffer.add(moon);
            buffer.upload();
        }
    }

    void Scene::unload()
    {
        buffer.clear();
        uploaded = false;
    }

    void Scene::update(Vector2 loc)
    {
        ship.location = loc;
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

    /* TODO: What happens if the ship passed all the way through the circle? */
    void Scene::movePoint(Vector2 p, Circle circle, float duration)
    {
        Vector2 u = p - circle.location;
        if (ship.velocity.magnitude() > 0.0f) {
            // Move ship to surface along velocity vector
            Vector2 v = ship.velocity.normalize();
            float b = dot(u, v);
            float u_norm = u.magnitude();
            float t1 = u_norm * u_norm - b * b;
            float a = std::sqrt(std::abs(t1));
            float t2 = circle.radius * circle.radius - a * a;
            float l = std::sqrt(std::abs(t2));
            if (b < 0.0f)
                ship.location -= v * (b + l);
            else
                ship.location += v * (l - b);

            // Project velocity vector onto surface tangent
            Vector2 normal = (ship.location - circle.location).normalize();
            float angle = std::atan2(normal.y, normal.x);
            Vector2 tangent{-std::sin(angle), std::cos(angle)};
            ship.velocity = tangent * dot(ship.velocity, tangent);
            ship.location += ship.velocity * duration;
        }
    }

    bool Scene::collide(Vector2 p, Circle &circle, float duration)
    {
        Vector2 u = p - circle.location;
        float r = u.magnitude();
        if (r <= circle.radius) {
            movePoint(p, circle, duration);
            collidedObject = &circle;
            ship.refVelocity = circle.velocity;
            return true;
        } else {
            collidedObject = nullptr;
            return false;
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
        force.x += fx;
        force.y += fy;
    }

    void Scene::update(double duration)
    {
        Vector2 force = ship.force;
        ship.location += ship.refVelocity * duration;

        planet.location += planet.velocity * duration;
        addForce(force, ship.location, planet.location, ship.mass, planet.mass, planet.radius);

        for (auto &moon : moons) {
            moon.angle += duration;
            float d = 0.5f;
            float m = 1.0f / moon.distance * d;
            float n = moon.distance - moon.radius;
            moon.velocity = {
                std::cos((moon.angle-n)*m)*d,
                -std::sin((moon.angle-n)*m)*d
            };
            moon.velocity += planet.velocity;
            moon.location += moon.velocity * duration;
            addForce(force, ship.location, moon.location, ship.mass, moon.mass, moon.radius);
        }

        if (collidedObject) {
            Vector2 normal = (ship.location - collidedObject->location).normalize();
            force -= normal * dot(force, normal);
        }

        ship.acceleration = force / ship.mass;
        ship.velocity += ship.acceleration * duration;
        ship.location += ship.velocity * duration;

        if (!collide(ship.location, planet, duration))
            for (auto &moon : moons)
                if (collide(ship.location, moon, duration))
                    break;

        updatePath();
    }

    void Scene::updatePath()
    {
        if ((path.getLastPoint() - ship.location).magnitude() > pixelSize * 20.0f) {
            Allocation alloc = path.addPoint(ship.location);
            if (alloc.indexCount) {
                const size_t vs = path.getAllocation().vertexStart;
                const size_t is = path.getAllocation().indexStart;
                const Vector2 *vb = &path.getVerticesRef().data()[alloc.vertexStart];
                const unsigned *ib = &path.getIndicesRef().data()[alloc.indexStart];
                buffer.update(vb, alloc.vertexStart+vs, alloc.vertexCount);
                buffer.update(ib, alloc.indexStart+is, alloc.indexCount);
            }
        }
    }

    Vector2 Scene::getLocation() const
    {
        return ship.location;
    }

    std::vector<const Object *> Scene::getObjects() const
    {
        return objects;
    }

    const Ship Scene::getShip() const
    {
        return ship;
    }

    const Circle *Scene::getCollidedObject() const
    {
        return collidedObject;
    }

    const Background Scene::getBackground() const
    {
        return background;
    }

    const Buffer &Scene::getBuffer() const
    {
        return buffer;
    }

}