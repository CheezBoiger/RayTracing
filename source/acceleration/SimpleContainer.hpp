// Raytracer.
#pragma once

#include "math/Float.hpp"
#include "acceleration/Aggregate.hpp"

#include "Primitive.hpp"

#include <vector>

namespace rt {

class SimpleContainer : public Aggregate
{
public:
    B32 intersects(const Ray& ray, SurfaceInteraction& si) override {
        B32 intersect = false;
        for (Primitive* prim : m_pPrimitives)
        {
            SurfaceInteraction test;
            if (prim->intersects(ray, test))
            {
                intersect = true;
                if (test.time < si.time)
                {
                    si = test;
                }   
            }
        }

        return intersect;
    }

    B32 addPrimitives(U32 primitiveCount, Primitive** pPrimitives) override 
    {
        for (U32 i = 0; i < primitiveCount; ++i)
        {
            m_pPrimitives.push_back(pPrimitives[i]);
        }
        return true;    
    }

    std::vector<Primitive*> m_pPrimitives;
};
} // rt