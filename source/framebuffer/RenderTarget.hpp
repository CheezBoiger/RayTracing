// Raytracer.
#pragma once

#include "common/Types.hpp"
#include "math/Float.hpp"

namespace rt {

class ImageBuffer
{
public:
    ImageBuffer(U32 width = 1ULL, U32 height = 1ULL)
        : m_sizeInBytes((U64)width * U64(height) * 3) 
    {
        if (m_sizeInBytes)
        {
            m_raw = new U8[m_sizeInBytes];
        }
    }

    ~ImageBuffer()
    {
        if (m_sizeInBytes)
        {
            delete[] m_raw;
        }
        
        m_sizeInBytes = 0ULL;
    }

    U8& operator[](U64 i) { return m_raw[i]; }

    const U8* getRaw() const { return m_raw; }

private:
    U8* m_raw;
    U64 m_sizeInBytes;
};

struct RenderTarget
{
    // Surface buffer.
    ImageBuffer*    surface;
    U32             width;
    U32             height;

    U32             getWidth() const { return width; }
    U32             getHeight() const { return height; }

    ImageBuffer*    getBuffer() { return surface; }

    void storeColor(U32 x, U32 y, const Float3& color);
};
} // rt