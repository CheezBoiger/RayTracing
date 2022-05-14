// Raytracer.
#pragma once

#include "math/Bounds.hpp"
#include "Primitive.hpp"
#include "Interaction.hpp"
#include "acceleration/Aggregate.hpp"


namespace rt {


class BoundingVolumeHierarchy : public Aggregate {
public:

    struct BVHNode {
        Bounds3     bounds;
        BVHNode*    children[2];
        I32         splitAxis;
        I32         offsetPrimitives;
        I32         numPrimitives;
    };

    virtual B32 intersects(const Ray& ray, SurfaceInteraction& si) override;

    virtual B32 addPrimitives(U32 primitiveCount, Primitive** ppPrimitives) override;

    virtual B32 update() override { return true; }

private:
};
} // rt
