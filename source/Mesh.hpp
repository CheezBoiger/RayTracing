// Raytracer.
#pragma once

#include "common/Types.hpp"
#include "geometry/topology.hpp"
#include "math/Matrix44.hpp"

#include "Primitive.hpp"

#include <vector>

namespace rt {


class Mesh 
{
public:
    Matrix44 getWorldToModel() const { return m_worldToModel; }
    Matrix44 getModelToWorld() const { return m_modelToWorld; }
private:
    std::vector<Primitive> m_prims;
    Matrix44 m_modelToWorld;
    Matrix44 m_worldToModel;
};
} // rt