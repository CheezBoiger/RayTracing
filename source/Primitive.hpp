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


struct Shape 
{

    virtual Matrix44 getLocalToWorld() const { return m_localToWorld; }
    virtual Matrix44 getWorldToLocal() const { return m_worldToLocal; }

    virtual B32 intersects(const Ray& ray, SurfaceInteraction& si) = 0;

protected:
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

    Material* getMat() { return m_pMaterial; }
    Shape* getShape() { return m_pShape; }

    Bounds3& getLocalBounds() { return Bounds3(); }
    Bounds3& getWorldBounds() { return Bounds3(); }

    void setShape(Shape* pShape) { m_pShape = pShape; }
    void setMat(Material* pMat) { m_pMaterial = pMat; }
    
private:
    Shape*      m_pShape;
    Material*   m_pMaterial;
    Bounds3     m_localBounds;
    Bounds3     m_worldBounds;
};
} // rt