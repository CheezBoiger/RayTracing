// Raytracer.


#include "common/Types.hpp"
#include "math/Float.hpp"

#include <math.h>

namespace rt {

class Texture;
class TextureSampler;

struct SurfaceInteraction;

static Float3 schlickFresnel(Float3 kS, F32 cosTheta)
{
    return kS + (1 - kS) * powf(1 - cosTheta, 5.f);
}

Float3 toLocalSpace(const Float3& v, const SurfaceInteraction& si);

struct Material
{
    virtual ~Material() { }

    // Sample the distribution, but also be sure to convert wi and wo to 
    // shading space before passing to this function.
    virtual Float3 sampleDistributionF(const Float3& wi, const Float3& wo) = 0;
};

struct MatteMaterial : public Material
{
    Float4              color;
    // Sample the distribution function.
    Float3 sampleDistributionF(const Float3& wi, const Float3& wo) override;    
};

struct MicrofacetMaterial : public Material
{  
    Float3              color;
    Texture*            albedo;
    // Normal * 0.5 + 0.5 to obtain [0, 1] range.
    Texture*            normal;
    Texture*            metallicRoughness;
    Texture*            ao;
    TextureSampler*     surfaceSampler;
    // Roughness parameter [0.04, 1]
    F32                 kD;
    // Metallic parameter [0, 1]
    F32                 kS;

    Float3 sampleDistributionF(const Float3& wi, const Float3& wo) override;

    F32 d(F32 alphaX, F32 alphaY, const Float3& wh);
    F32 g(const Float3& wo, const Float3& wi, F32 alphaX, F32 alphaY);
    F32 roughnessToAlpha(F32 roughness);
    F32 lambda(F32 alphaX, F32 alphaY, const Float3& w) const;
};
} // rt