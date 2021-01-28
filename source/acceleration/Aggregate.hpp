// Raytracer.
#pragma once

#include "math/Float.hpp"
#include "math/Ray.hpp"

#include "Primitive.hpp"
#include "Interaction.hpp"

namespace rt {


class Aggregate
{
public:
    virtual ~Aggregate() { }

    virtual B32 intersects(const Ray& ray, SurfaceInteraction& si) = 0;

    virtual B32 update() { return true; }

    virtual B32 addPrimitives(U32 primitiveCount, Primitive** pPrimitives) = 0;
};
} // rt