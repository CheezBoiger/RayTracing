// Raytracer.
#include "RenderTarget.hpp"

namespace rt {


void RenderTarget::storeColor(U32 x, U32 y, const Float3& color)
{
    if (!surface)
        return;
    (*surface)[(U64(width) * U64(y) + U64(x)) * 3 + 0] = U8(color.x * 255.f);
    (*surface)[(U64(width) * U64(y) + U64(x)) * 3 + 1] = U8(color.y * 255.f);
    (*surface)[(U64(width) * U64(y) + U64(x)) * 3 + 2] = U8(color.z * 255.f);
    
}
} // rt