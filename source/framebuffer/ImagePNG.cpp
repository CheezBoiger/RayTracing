// Raytracer.
#include "Image.hpp"

#include "RenderTarget.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION 1
#include "stb_image_write.h"

namespace rt 
{

class ImagePNG : public Image
{
public:
    ImagePNG(const std::string& filename = "")
        : Image(filename) { }

    void saveBuffer(const ImageBuffer* pBuf, U32 width, U32 height, U32 channels) override
    {
        int err = stbi_write_png(getFilename().c_str(), width, height, channels, pBuf->getRaw(), width * channels);
    }
};

Image* createPNG(const std::string& filename)
{
    return new ImagePNG(filename);
}
}