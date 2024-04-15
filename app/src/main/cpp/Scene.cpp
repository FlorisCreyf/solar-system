#include "Scene.h"
#include "Buffer.h"
#include "AndroidOut.h"
#include <cmath>
#include <assert.h>

namespace Solar {

    Scene::Scene(float pixelSize) :
        planet("Planet"),
        path(1000, Vector2{0.0f, 1.5f}, pixelSize*4.0f),
        collidedObject(nullptr),
        pixelSize(pixelSize),
        uploaded(false)
    {
        planet.mass = 5.0f;
        planet.radius = 1.0f;
        ship.mass = 1.0f;
        ship.location = {0.0f, 1.5f};
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

    Scene::~Scene()
    {
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

    void movePoint(Vector2 p, Ship &ship, Circle circle)
    {
        Vector2 u = p - circle.location;
        if (ship.velocity.magnitude() > 0.0f) {
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
        }

        Vector2 normal = (ship.location - circle.location).normalize();
        Vector2 d = normal * dot(ship.velocity-circle.velocity, normal);
        ship.velocity -= d * 2.0f;

        if ((ship.velocity - circle.velocity).magnitude() < 1.0f)
            ship.velocity = circle.velocity;
        else
            ship.velocity = ship.velocity + ((circle.velocity - ship.velocity) * 0.1f);
    }

    bool checkCollision(Vector2 p, Ship &ship, Circle circle)
    {
        Vector2 u = p - circle.location;
        float r = u.magnitude();
        if (r <= circle.radius) {
            movePoint(p, ship, circle);
            return true;
        }
        return false;
    }

    bool collide(Ship &ship, Circle circle)
    {
        return checkCollision(ship.location, ship, circle);
        // Vector2 top;
        // Vector2 bottomRight;
        // Vector2 bottomLeft;
        // bool collision;
        // ship.getCorners(top, bottomRight, bottomLeft);
        // collision = collidePoint(top, ship, circle);
        // if (collision)
        //     ship.getCorners(top, bottomRight, bottomLeft);
        // collision = collision || collidePoint(bottomRight, ship, circle);
        // if (collision)
        //     ship.getCorners(top, bottomRight, bottomLeft);
        // collision = collision || collidePoint(bottomLeft, ship, circle);
        // return collision;
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
        Vector2 force = ship.force;

        addForce(force, ship.location, planet.location, ship.mass, planet.mass, planet.radius);
        planet.location += planet.velocity * duration;

        for (auto &moon : moons) {
            moon.angle += duration;
            float m = 1.0f / moon.distance;
            float n = moon.distance - moon.radius;
            moon.velocity = {
                std::cos((moon.angle-n)*m),
                -std::sin((moon.angle-n)*m)
            };
            addForce(force, ship.location, moon.location, ship.mass, moon.mass, moon.radius);
            moon.location += moon.velocity * duration;
        }

        if (collidedObject) {
            Vector2 n = (ship.location - collidedObject->location).normalize();
            force -= n * dot(n, force);
        }

        ship.acceleration = force / ship.mass;
        ship.velocity += ship.acceleration * duration;
        ship.location += ship.velocity * duration;

        if (!collide(ship, planet)) {
            collidedObject = nullptr;
            for (auto &moon: moons)
                if (collide(ship, moon)) {
                    collidedObject = &moon;
                    break;
                }
        } else
            collidedObject = &planet;

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