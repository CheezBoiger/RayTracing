// Raytracer.
#pragma once

#include "common/Types.hpp"
#include "math/Float.hpp"

namespace rt {

struct IMaterial;


// A surface interaction made by a given ray, will need to be stored as data to be used 
// for lighting.
struct SurfaceInteraction
{
    Float3      vNormal;
    Float3      vPosition;
    Float3      vTexCoord;

    // Direction of outgoing light, which is usually being taken in by the viewer, or eye.
    Float3      wo;
    Float3      dpdu; // tangent u.
    Float3      dpdv; // tangent v.

    IMaterial*  pMaterial;
    F32         time;
};
} // rt