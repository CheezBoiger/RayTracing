// Raytracer.
#pragma once

#include "common/Types.hpp"
#include "Light.hpp"

#include <vector>

namespace rt {


struct SurfaceInteraction;
struct Primitive;
struct Light;
class Aggregate;
struct Ray;

class Scene {
public:
    B32 intersects(const Ray& ray, SurfaceInteraction& si);

    void addPrimitive(U32 primitiveCount, Primitive** ppPrimitives);

    // 
    void setAggregate(Aggregate* pAggregate) { m_pAggregate = pAggregate; }

    std::vector<Light*>& getLights() { return m_lights; }

    void addLight(Light* light) { m_lights.push_back(light); }

private:
    // Aggregate contains the structure storing all primitives in the scene.
    // This abstraction provides the interface to determine how to implement
    // an acceleration structure, or container to optimize ray traversal.
    Aggregate* m_pAggregate;

    std::vector<Light*> m_lights;
};
} // rt