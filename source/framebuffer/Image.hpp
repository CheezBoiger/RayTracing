// Raytracer.

#pragma once

#include "common/Types.hpp"
#include "math/Float.hpp"

#include <string>

namespace rt {

class ImageBuffer;

class Image 
{
public:
    Image(std::string filename = "")
        : m_filename(filename) { }

    virtual ~Image() { }

    virtual void saveBuffer(const ImageBuffer* pBuf, U32 width, U32 height, U32 channels) = 0;

    const std::string& getFilename() const { return m_filename; }
private:
    std::string m_filename;
};


Image* createTGA();
Image* createPNG(const std::string& filename);
Image* createJPG();

static void    destroyImage(Image* image)
{
    if (image)
        delete image;
}
} // rt