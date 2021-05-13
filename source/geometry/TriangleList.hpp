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


struct Triangle : public Shape
{

    B32 intersects(const Ray& ray, SurfaceInteraction& si) override
    {   
        
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
    TriangleList* m_pTriangleList;
    const U32* m_index;
};
} // namespace rt