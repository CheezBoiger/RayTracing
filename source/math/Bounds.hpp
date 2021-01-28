// Raytracer.
#pragma once

#include "common/Types.hpp"
#include "math/Ray.hpp"

namespace rt {


struct Bounds3
{
    Float3 min, max;
};

bool        rayBoundsIntersect(const Ray& ray, const Bounds3& bounds);
bool        intersect(const Bounds3& lh, const Bounds3& rh);
F32         volume(const Bounds3& lh);
Float3      center(const Bounds3& lh);
Bounds3     boundsUnion(const Bounds3& lh, const Bounds3& rh);
} // rt