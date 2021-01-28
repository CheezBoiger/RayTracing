// Raytracer.
#pragma once

#include "Float.hpp"

#define MAT4_INDEX(row, col) (row * 4 + col)

namespace rt {

struct Matrix44 {
    union {
        struct {
            Float4 row0, row1, row2, row3;
        };
        F32 d[16];
    };
    
    Matrix44(F32 a00 = 1, F32 a01 = 0, F32 a02 = 0, F32 a03 = 0,
             F32 a10 = 0, F32 a11 = 1, F32 a12 = 0, F32 a13 = 0,
             F32 a20 = 0, F32 a21 = 0, F32 a22 = 1, F32 a23 = 0,
             F32 a30 = 0, F32 a31 = 0, F32 a32 = 0, F32 a33 = 1);

    Matrix44(const Float4& row0,
             const Float4& row1,
             const Float4& row2,
             const Float4& row3)
        : row0(row0), row1(row1), row2(row2), row3(row3) { }

    F32     operator[](U32 i) const { return d[i]; }
    F32&    operator[](U32 i) { return d[i]; }

    F32     get(U32 row, U32 col) const { return d[MAT4_INDEX(row, col)]; }
    F32&    get(U32 row, U32 col) { return d[MAT4_INDEX(row, col)]; }

    Matrix44 operator*(const Matrix44& rh) const;
    Matrix44 operator*(F32 scalar) const;    

    Matrix44 operator+(const Matrix44& rh) const;
    Matrix44 operator+(F32 scalar) const;

    Matrix44 operator-(const Matrix44& rh) const;    
    Matrix44 operator-(F32 scalar) const;
    Matrix44 operator-() const;

    Matrix44 operator/(F32 scalar) const;

    void operator+=(F32 scalar);
    void operator*=(F32 scalar);
    void operator-=(F32 scalar);
    void operator/=(F32 scalar); 

    void operator+=(const Matrix44& rh);
    void operator-=(const Matrix44& rh);
    void operator*=(const Matrix44& rh);

    friend Float4   operator*(const Float4& lh, const Matrix44& rh);
    friend Matrix44 operator+(F32 lh, const Matrix44& rh);
    friend Matrix44 operator-(F32 lh, const Matrix44& rh);
    friend Matrix44 operator/(F32 lh, const Matrix44& rh);
};

Matrix44    translate(const Matrix44& lh, const Float3& rh);
Matrix44    rotate(const Matrix44& lh, const Float3& axis, F32 radians);
Matrix44    scale(const Matrix44& lh, const Float4& factor);
Matrix44    transpose(const Matrix44& lh);
Matrix44    inverse(const Matrix44& lh);
Matrix44    adjugate(const Matrix44& lh);

F32         determinant(const Matrix44& lh);
Matrix44    identity();
Matrix44    perspective(F32 fov, F32 aspect, F32 ne, F32 fa);
Matrix44    lookAt(const Float3& position, const Float3& target, const Float3& up);
} // rt