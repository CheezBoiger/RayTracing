// Raytracer
#pragma once

#include "common/Types.hpp"

#include "math/Float.hpp"
#include "math/Matrix44.hpp"

#include "geometry/topology.hpp"

#include "Interaction.hpp"
#include "Primitive.hpp"


namespace rt {


class TriangleList 
{
public:
    void loadVertices();
    void cleanUp();

    Triangle getTriangle(U32 index) 
    {
        if (index > m_nTriangles)
            return Triangle(nullptr, nullptr, 0);
        return Triangle(this, m_indices, index);
    }

    // Getters for vertex values.
    inline const Float3& getPosition(U32 index) const   { return m_vertices.m_positions[index]; }
    inline const Float3& getNormal(U32 index) const     { return m_vertices.m_normals[index]; }
    inline const Float3& getTangents(U32 index) const   { return m_vertices.m_tangents[index]; }
    inline const Float2& getUVs(U32 index) const        { return m_vertices.m_uvs[index]; }

private:
    U32     m_nTriangles;
    U32     m_nVertices;
    U32*    m_indices;

    struct {
        Float3* m_positions;
        Float3* m_normals;
        Float3* m_tangents;
        Float2* m_uvs;
    } m_vertices;   
};


// Triangle representation from a given triangle list. This will ultimately define how each 
// triangle from a list must be processed.
struct Triangle : public Shape
{
    Triangle(TriangleList* pList, U32* indices, U32 index)
        : m_pTriangleList(pList)
        , m_index(indices + index * 3u)
    {
        
    }

    B32 intersects(const Ray& ray, SurfaceInteraction& si) override
    {   
        // Use Moller-Trumbore intersection algorithm.
        const F32 kEpsilon = 0.0000001f;
        Float3 p0 = m_pTriangleList->getPosition(m_index[0]);
        Float3 p1 = m_pTriangleList->getPosition(m_index[1]);
        Float3 p2 = m_pTriangleList->getPosition(m_index[2]);

        Float3 edge1 = p1 - p0;
        Float3 edge2 = p2 - p0;
        Float3 s;
        Float3 q;

        Float3 h    = cross(ray.dir, edge2);
        F32 a       = dot(edge1, h);
        F32 f       = 0.f;

        // U and V are calculated with barycentrics.
        F32 u, v    = -1.0f;

        if (a > -kEpsilon && a < kEpsilon)
            return false;

        f = 1.0f / a;
        s = ray.o - p0;
        u = f * dot(s, h);

        if (u < 0.0f || u > 1.0f)
            return false;

        q = cross(s, edge1);
        v = f * dot(ray.dir, q);

        if (v < 0.0f | (u + v) > 1.0f)
            return false;

        F32 t = f * dot(edge2, q);

        // We have intersected this ray.
        if (t > kEpsilon)
        {
            si.time         = t;
            si.vPosition    = ray.o + ray.dir * t;
            si.vNormal      = cross(edge1, edge2);
            si.vTexCoord    = Float2(u, v);
            si.wo           = -ray.dir;
            return true;
        }

        return false;
    }

    // Calculate the area of a triangle by obtaining the surface area of a parallelogram,
    // and taking the half of it.
    F32 area() const override
    {
        const Float3& p0 = m_pTriangleList->getPosition(m_index[0]);
        const Float3& p1 = m_pTriangleList->getPosition(m_index[1]);
        const Float3& p2 = m_pTriangleList->getPosition(m_index[2]);
        return 0.5f * length(cross(p1 - p0, p2 - p0));
    }
private:
    TriangleList*   m_pTriangleList;
    const U32*      m_index;
};
} // namespace rt