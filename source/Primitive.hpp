// Raytracer
#pragma once

#include "common/Types.hpp"

#include "math/Bounds.hpp"
#include "math/Ray.hpp"
#include "math/Matrix44.hpp"

#include "geometry/topology.hpp"

#include "Material.hpp"
#include "Interaction.hpp"

#include <vector>

namespace rt {


// Shape structure.
struct Shape 
{

    virtual Matrix44 getLocalToWorld() const { return m_localToWorld; }
    virtual Matrix44 getWorldToLocal() const { return m_worldToLocal; }

    // Check for intersection of the ray, and fill the interaction table
    // if such an intersection is made.
    virtual B32 intersects(const Ray& ray, SurfaceInteraction& si) = 0;

    // Get the area of our shape.
    virtual F32 area() const { return 0.f; }
    
    // Calculate the probability density function (PDF) for the shape.
    // This is usually 1 / area. 
    virtual F32 pdf(const SurfaceInteraction& si) const
    {
        return 1.f / area();
    }

protected:
    // Shape Transformations.
    Matrix44 m_localToWorld;
    Matrix44 m_worldToLocal;
};

struct Primitive 
{
    B32 intersects(const Ray& ray, SurfaceInteraction& si)
    {
        B32 intersect = m_pShape->intersects(ray, si); 
        if (intersect)
            si.pMaterial = m_pMaterial; 
        return intersect;
    }

    IMaterial* getMat() { return m_pMaterial; }
    Shape* getShape() { return m_pShape; }

    Bounds3& getLocalBounds() { return Bounds3(); }
    Bounds3& getWorldBounds() { return Bounds3(); }

    void setShape(Shape* pShape) { m_pShape = pShape; }
    void setMat(IMaterial* pMat) { m_pMaterial = pMat; }
    
private:
    Shape*      m_pShape;
    IMaterial*  m_pMaterial;
    Bounds3     m_localBounds;
    Bounds3     m_worldBounds;
};
} // rt