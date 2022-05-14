// Raytracer.
#pragma once

#include "common/Types.hpp"

#if defined SIMD_ENABLE
#include <xmmintrin.h>
#include <immintrin.h>
#endif

namespace rt {

struct Float3;
struct Float4;

struct Float2 {
    union { struct { F32 x, y; };
            struct { F32 s, t; };
            struct { F32 r, g; }; };

    Float2(F32 x = 0.f, F32 y = 0.f)
        : x(x), y(y) { }

    F32 operator[](I32 i) const { return (&x)[i]; }
    F32& operator[](I32 i) { return (&x)[i]; }

    Float2 operator+(const Float2& rh) const {
        return { x + rh.x, y + rh.y };
    }

    Float2 operator+(F32 scalar) const {
        return { x + scalar, y + scalar };    
    }

    Float2 operator-(const Float2& rh) const {
        return { x - rh.x, y - rh.y };
    }

    Float2 operator-(F32 scalar) const {
        return { x - scalar, y - scalar };
    }

    Float2 operator-() const {
        return { -x, -y };
    }

    Float2 operator*(const Float2& rh) const {
        return { x * rh.x, y * rh.y };
    }

    Float2 operator*(F32 scalar) const {
        return { x * scalar, y * scalar };
    }

    Float2 operator/(const Float2& rh) const {
        return { x / rh.x, y / rh.y };
    }

    Float2 operator/(F32 scalar) const {
        return { x / scalar, y / scalar };
    }

    friend Float2 operator+(F32 lh, const Float2& rh) {
        return rh + lh;
    }

    friend Float2 operator-(F32 lh, const Float2& rh) {
        return -rh + lh;
    }
};


struct Float3 {
    union { struct { F32 x, y, z; };
            struct { F32 s, t, q; };
            struct { F32 r, g, b; }; };

    Float3(F32 x = 0.f, F32 y = 0.f, F32 z = 0.f)
        : x(x), y(y), z(z) { }    
    Float3(const Float2& a, F32 z = 0.f)
        : x(a.x), y(a.y), z(z) { }

    F32 operator[](I32 i) const { return (&x)[i]; }
    F32& operator[](I32 i) { return (&x)[i]; }

    Float3 operator*(const Float3& rh) const {
        return { x * rh.x, y * rh.y, z * rh.z };
    }

    Float3 operator+(F32 scalar) const {
        return { x + scalar, y + scalar, z + scalar };
    }

    Float3 operator-(F32 scalar) const {
        return { x - scalar, y - scalar, z - scalar };
    }

    Float3 operator*(F32 scalar) const {
        return { x * scalar, y * scalar, z * scalar };
    }

    Float3 operator/(F32 scalar) const {
        return { x / scalar, y / scalar, z / scalar };
    }

    Float3 operator/(const Float3& rh) const {
        return { x / rh.x, y / rh.y, z / rh.z };
    }

    Float3 operator+(const Float3& rh) const {
        return { x + rh.x, y + rh.y, z + rh.z };
    }

    Float3 operator-(const Float3& rh) const {
        return { x - rh.x, y - rh.y, z - rh.z };
    }

    Float3 operator-() const { 
        return { -x, -y, -z };
    }

    void operator+=(const Float3& rh) {
        x += rh[0];
        y += rh[1];
        z += rh[2];
    }

    void operator-=(const Float3& rh) {
        x -= rh[0];
        y -= rh[1];
        z -= rh[2];
    }

    void operator*=(const Float3& rh) { 
        x *= rh[0];
        y *= rh[1];
        z *= rh[2];
    }

    operator Float2 () const {
        return Float2(x, y);
    }

    friend Float3 operator+(F32 lh, const Float3& rh) {
        return rh + lh;
    }

    friend Float3 operator-(F32 lh, const Float3& rh) {
        return -rh + lh;
    }

    friend Float3 operator*(F32 lh, const Float3& rh) {
        return rh * lh;
    }

    friend Float3 operator/(F32 lh, const Float3& rh) {
        return Float3(lh / rh.x, lh / rh.y, lh / rh.z);
    }

    friend B32 operator<(const Float3& lh, F32 rh) {
        return (lh.x < rh) && (lh.y < rh) && (lh.z < rh);
    }

    friend B32 operator>(const Float3& lh, F32 rh)
    {
        return (lh.x > rh) && (lh.y > rh) && (lh.z > rh);
    }
};


struct Float4 {
    union { struct { F32 x, y, z, w; };
            struct { F32 s, t, q, p; };
            struct { F32 r, g, b, a; }; 
#if defined SIMD_ENABLE
            __m128 xmm;
#endif
                };

    Float4(F32 x = 0.f, F32 y = 0.f, F32 z = 0.f, F32 w = 0.f)
        : x(x), y(y), z(z), w(w) { }
    Float4(const Float3& vf3, F32 w = 0.f)
        : x(vf3[0]), y(vf3[1]), z(vf3[2]), w(w) { }
    Float4(const Float2& a0, const Float2& a1)
        : x(a0.x), y(a0.y), z(a1.x), w(a1.y) { }
    Float4(const Float2& a, F32 z = 0.f, F32 w = 0.f)
        : x(a.x), y(a.y), z(z), w(w) { }

    F32 operator[](I32 i) const { return (&x)[i]; }
    F32& operator[](I32 i) { return (&x)[i]; }

    Float4 operator+(const Float4& rh) const {
        return { x + rh[0], y + rh[1], z + rh[2], w + rh[3] };
    }

    Float4 operator+(F32 scalar) const {
        return { x + scalar, y + scalar, z + scalar, w + scalar };
    }

    Float4 operator*(const Float4& rh) const {
        return { x * rh.x, y * rh.y, z * rh.z, w * rh.w };
    }

    Float4 operator*(F32 scalar) const {
        return { x * scalar, y * scalar, z * scalar, w * scalar };
    }

    Float4 operator/(const Float4& rh) const {
        return { x / rh.x, y / rh.y, z / rh.z, w / rh.w };
    }

    Float4 operator/(F32 scalar) const {
        return { x / scalar, y / scalar, z / scalar, w / scalar };
    }

    Float4 operator-(const Float4& rh) const {
        return { x - rh.x, y - rh.y, z - rh.z, w - rh.w };
    }

    Float4 operator-(F32 scalar) const {
        return { x - scalar, y - scalar, z - scalar, w - scalar };
    }

    Float4 operator-() const {
        return { -x, -y, -z, -w };
    }

    void operator+=(const Float4& rh) {
        x += rh[0];
        y += rh[1];
        z += rh[2];
        w += rh[3];
    }

    void operator-=(const Float4& rh) {
        x -= rh[0];
        y -= rh[1];
        z -= rh[2];
        w -= rh[3];
    }

    void operator*=(const Float4& rh) {
        x *= rh[0];
        y *= rh[1];
        z *= rh[2];
        w *= rh[3];
    }

    operator Float3() const {
        return Float3(x, y, z);
    } 

    operator Float2() const {
        return Float2(x, y);
    }

    friend Float4 operator+(F32 lh, const Float4& rh) {
        return rh + lh;
    }

    friend Float4 operator-(F32 lh, const Float4& rh) {
        return -rh + lh;
    }

    friend Float4 operator*(F32 lh, const Float4& rh) {
        return rh * lh;
    }

    friend Float4 operator/(F32 lh, const Float4& rh) {
        return { lh / rh.x, lh / rh.y, lh / rh.z, lh / rh.w };
    }
};

Float3 cross(const Float3& lh, const Float3& rh);

F32     dot(const Float2& lh, const Float2& rh);
F32     dot(const Float3& lh, const Float3& rh);
F32     dot(const Float4& lh, const Float4& rh);

Float2  sqrt(const Float2& lh);
Float3  sqrt(const Float3& lh);
Float4  sqrt(const Float4& lh);

Float2  pow(const Float2& lh, F32 exp);
Float3  pow(const Float3& lh, F32 exp);
Float4  pow(const Float4& lh, F32 exp);

F32     length(const Float2& v);
F32     length(const Float3& v);
F32     length(const Float4& v);

F32     length2(const Float2& v);
F32     length2(const Float3& v);
F32     length2(const Float4& v);

Float2  normalize(const Float2& v);
Float3  normalize(const Float3& v);
Float4  normalize(const Float4& v);

// Reflection calculation.
Float3  reflect(const Float3& vI, const Float3& vN);

// Refraction calculation.
Float3  refract(const Float3& vI, const Float3& vN, F32 eta);

F32 cosTheta(const Float3& w);
F32 cos2Theta(const Float3& w);
F32 absCosTheta(const Float3& w);
F32 sin2Theta(const Float3& w);
F32 sinTheta(const Float3& w);
F32 tanTheta(const Float3& w);
F32 tan2Theta(const Float3& w);
} // rt