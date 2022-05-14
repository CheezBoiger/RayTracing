// Raytracer.

#include <math.h>

#include "Float.hpp"

namespace rt {

Float3 cross(const Float3& lh, const Float3& rh)
{
    return {
            lh.y * rh.z - lh.z * rh.y, 
            lh.z * rh.x - lh.x * rh.z,
            lh.x * rh.y - lh.y * rh.x
    };
}

F32 dot(const Float2& lh, const Float2& rh)
{
    return (lh[0] * rh[0]) + (lh[1] * rh[1]);
}

F32 dot(const Float3& lh, const Float3& rh)
{
    return (lh[0] * rh[0]) + (lh[1] * rh[1]) + (lh[2] * rh[2]);
}

F32 dot(const Float4& lh, const Float4& rh)
{
    return (lh[0] * rh[0]) + (lh[1] * rh[1]) + (lh[2] * rh[2]) + (lh[3] * rh[3]);
}

Float2 sqrt(const Float2& lh)
{
    return { sqrtf(lh.x), sqrtf(lh.y) };
}

Float3 sqrt(const Float3& lh)
{
    return { sqrtf(lh.x), sqrtf(lh.y), sqrtf(lh.z) };
}

Float4 sqrt(const Float4& lh)
{
    return { sqrtf(lh.x), sqrtf(lh.y), sqrtf(lh.z), sqrtf(lh.w) };
}

F32 length(const Float2& v)
{
    return sqrtf(length2(v));
}

F32 length(const Float3& v)
{
    return sqrtf(length2(v));
}

F32 length(const Float4& v)
{
    return sqrtf(length2(v));
}

F32 length2(const Float2& v)
{
    return dot(v, v);
}

F32 length2(const Float3& v)
{
    return dot(v, v);
}

F32 length2(const Float4& v)
{
    return dot(v, v);
}

Float2 normalize(const Float2& v)
{
    return v / length(v);
}

Float3 normalize(const Float3& v)
{
    return v / length(v);
}

Float4 normalize(const Float4& v)
{
    return v / length(v);
}

Float3 reflect(const Float3& wo, const Float3& n)
{
    // Incident light wo outgoing must be inverted to get the reflection light
    // directed to the source, wi.
    return -wo + 2.0f * dot(n, wo) * n;
}

Float3 refract(const Float3& vI, const Float3& vN, F32 eta)
{
    F32 dotNI = dot(vN, vI);
    F32 k = 1.0f - eta * eta * (1.0f - dotNI * dotNI);
    Float3 vR;
    if (k >= 0.0f)
        vR = eta * vI - (eta * dotNI + sqrtf(k)) * vN;
    return vR;
}     

Float2 pow(const Float2& lh, F32 exp)
{
    return Float2(powf(lh.x, exp), powf(lh.y, exp));
}  

Float3 pow(const Float3& lh, F32 exp)
{
    return Float3(powf(lh.x, exp), powf(lh.y, exp), powf(lh.z, exp));
}

Float4 pow(const Float4& lh, F32 exp)
{
    return Float4(powf(lh.x, exp), powf(lh.y, exp), powf(lh.z, exp), powf(lh.w, exp));
}

F32 cosTheta(const Float3& w) 
{
    return w.z;
}

F32 cos2Theta(const Float3& w)
{
    return w.z * w.z;
}

F32 absCosTheta(const Float3& w)
{
    return fabsf(w.z);
}

F32 sin2Theta(const Float3& w)
{
    return fmaxf(0.f, 1.f - cos2Theta(w));
}

F32 sinTheta(const Float3& w)
{
    return sqrtf(sin2Theta(w));
}

F32 tanTheta(const Float3& w)
{
    return sinTheta(w) / cosTheta(w);
}

F32 tan2Theta(const Float3& w)
{
    return sin2Theta(w) / cos2Theta(w);
}
} // rt