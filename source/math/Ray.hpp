// Raytracer.
#pragma once

#include "common/Types.hpp"
#include "math/Float.hpp"
#include "math/Matrix44.hpp"

namespace rt {


struct Ray {
    Float3 o, dir;

    Ray(const Float3& origin = Float3(), const Float3& dir = Float3())
        : o(origin), dir(dir) { }

    Ray invert() const {
        return { o, -dir };
    }

    Ray operator-() const {
        return invert();
    }

    Ray operator+(const Ray& rh) const
    {
        return { o + rh.o, dir + rh.dir };
    }

    Ray operator-(const Ray& rh) const
    {
        return { o - rh.o, dir - rh.dir };
    }

    Ray operator*(const Ray& rh) const
    {
        return { o * rh.o, dir * rh.dir };
    }

    Ray operator*(const Matrix44& lh) const
    {
        Ray ans = *this;
        ans.o = Float4(ans.o, 1.0f) * lh;
        // We don't need the translation for direction vector.
        ans.dir = Float3(
            dir[0] * lh[0] + dir[1] * lh[4] + dir[2] * lh[8],
            dir[0] * lh[1] + dir[1] * lh[5] + dir[2] * lh[9],
            dir[0] * lh[2] + dir[1] * lh[6] + dir[2] * lh[10]
        );
        return ans;
    }
};
} // rt