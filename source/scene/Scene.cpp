// Raytracer.
#include "Scene.hpp"

#include "math/Ray.hpp"
#include "acceleration/Aggregate.hpp"

namespace rt {


B32 Scene::intersects(const Ray& ray, SurfaceInteraction& si)
{
    if (!m_pAggregate)
        return false;
    return m_pAggregate->intersects(ray, si);
}

void Scene::addPrimitive(U32 primitiveCount, Primitive** ppPrimitives)
{
    if (m_pAggregate)
        m_pAggregate->addPrimitives(primitiveCount, ppPrimitives);
}
} // rt