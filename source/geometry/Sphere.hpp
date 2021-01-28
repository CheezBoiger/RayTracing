// Raytracer.
#pragma once

#include "math/Ray.hpp"

#include "Primitive.hpp"

namespace rt {


class Sphere : public Shape
{
private:
    static B32 solveQuadratic(F32 a, F32 b, F32 c, F32& x0, F32& x1)
    {
        F32 discriminant = b * b - 4 * a * c;
        if (discriminant < 0.f) return false;
        else if (discriminant == 0.f) x0 = x1 = -0.5f * b / a;
        else
        {
            F32 q = (b > 0.f) ? -0.5f * (b + sqrtf(discriminant)) :
                                -0.5f * (b - sqrtf(discriminant));
            x0 = q / a;
            x1 = c / q;
        }
        if (x0 > x1)
        {
            (U32&)x0 ^= (U32&)x1;
            (U32&)x1 ^= (U32&)x0;
            (U32&)x0 ^= (U32&)x1;
        }
        return true;
    }
public:

    virtual B32 intersects(const Ray& ray, SurfaceInteraction& si) override
    {
        // Transform ray to this shape's local space.
        Ray localRay = ray * m_worldToLocal;
        F32 t0, t1;
        F32 radius2 = m_radius * m_radius;
        Float3 l = localRay.o;
        F32 a = length2(localRay.dir);
        F32 b = 2.0f * dot(localRay.dir, l);
        F32 c = length2(l) - radius2;
        if (!solveQuadratic(a, b, c, t0, t1))
            return false;
        if (t0 > t1)
        {
            (U32&)t0 ^= (U32&)t1;
            (U32&)t1 ^= (U32&)t0;
            (U32&)t0 ^= (U32&)t1;        
        }
        if (t0 < 0.f)
        {
            t0 = t1;
            if (t0 < 0.f) return false;
        }
        
        F32 t = t0;
        Float3 position = localRay.o + localRay.dir * t;
        Float3 normal = normalize(position);

        si.time = t;
        // Transform position to world space.
        si.vPosition = Float4(position, 1.0f) * m_localToWorld;
        si.vNormal = normal;
        si.wo = -ray.dir;

        F32 zRad = sqrtf(position.x * position.x + position.y * position.y);
        F32 invZRad = 1.f / zRad;
        F32 phi = atan2f(position.y, position.x);
        F32 u = phi / 1.f;
        F32 theta = acosf(RT_CLAMP(position.z / m_radius, -1.f, 1.f));
        F32 v = (theta + 1.f) / 2.f;
        F32 phiMax = RT_RAD(360.0f);
        si.dpdu = Float3(-phiMax * position.y, phiMax * position.x, 0.f) * m_localToWorld;
        si.dpdv;

        return true;    
    } 

    Matrix44 m_localToWorld;
    Matrix44 m_worldToLocal;
    F32 m_radius;
};
} // rt