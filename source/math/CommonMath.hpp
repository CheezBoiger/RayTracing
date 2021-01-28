// Raytracer
#pragma once

#include "common/Types.hpp"

#define RT_PI                  3.141592653589793238462643383279502884197169399375
#define RT_INV_PI              0.31830988618

#define RT_LERP(a, b, t)       ((a) * (1.0f - (t)) + (b) * (t))
#define RT_RAD(deg)            ((deg) * ((rt::F32)RT_PI / 180.f))
#define RT_DEG(rad)            ((rad) * (180.f / (rt::F32)RT_PI))
#define RT_ABS(v)              ((v) < 0.f ? -(v) : (v))
#define RT_CLAMP(v, mi, ma)    ((v) > (ma) ? (ma) : ((v) < (mi) ? (mi) : (v)))