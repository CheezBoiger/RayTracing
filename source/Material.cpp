// Raytracer.
#include "Material.hpp"
#include "Interaction.hpp"

#include "math/CommonMath.hpp"

namespace rt {

const std::string CustomMaterial::Common::kNormal = "normal";
const std::string CustomMaterial::Common::kAlbedo = "albedo";
const std::string CustomMaterial::Common::kRoughness = "roughness";
const std::string CustomMaterial::Common::kSpecular = "specular";
const std::string CustomMaterial::Common::kGloss = "gloss";
const std::string CustomMaterial::Common::kDiffuse = "diffuse";

// Reflectance 
Float3 lambertDiffuse(const Float3& r)
{
    return r * RT_INV_PI;
}

Float3 MatteMaterial::distributionF(const Float3& wi, const Float3& wo)
{
    return lambertDiffuse(color);
}

F32 MicrofacetMaterial::roughnessToAlpha(F32 roughness)
{
    roughness = fmaxf(roughness, (F32)1e-3);
    F32 x = logf(roughness);
    return 1.62142f + 0.819955f * x + 0.1734f * x * x + 0.0171201f * x * x * x +
               0.000640711f * x * x * x * x;
}

F32 cosPhi(const Float3& w)
{
    F32 sinT = sinTheta(w);
    return (sinT == 0.f) ? 1.f : RT_CLAMP(w.x / sinT, -1.f, 1.f); 
}

F32 sinPhi(const Float3& w)
{
    F32 sinT = sinTheta(w);
    return (sinT == 0.f) ? 0.f : RT_CLAMP(w.y / sinT, -1.f, 1.f);
}

F32 cos2Phi(const Float3& w)
{
    return cosPhi(w) * cosPhi(w);
}

F32 sin2Phi(const Float3& w)
{
    return sinPhi(w) * sinPhi(w);
}

B32 sameHemisphere(const Float3& w, const Float3& wp)
{
    return w.z * wp.z > 0.f;
}

Float3 fresnelDielectric(F32 cosThetaI, F32 etaI, F32 etaT)
{
    F32 fS;
    cosThetaI = RT_CLAMP(cosThetaI, -1.f, 1.f);
    B32 entering = cosThetaI > 0.f;
    if (!entering)
    {
        // Doing a really lame swap. This is probably what is already happening internally.
        *(U32*)&etaI ^= *(U32*)&etaT;
        *(U32*)&etaT ^= *(U32*)&etaI;
        *(U32*)&etaI ^= *(U32*)&etaT;
        cosThetaI = fabsf(cosThetaI);
    }
       F32 sinThetaI = sqrtf(fmaxf((F32)0,
                             1 - cosThetaI * cosThetaI));
       F32 sinThetaT = etaI / etaT * sinThetaI;
          if (sinThetaT >= 1)
              return Float3(1, 1, 1);
       F32 cosThetaT = sqrtf(fmaxf((F32)0,
                                   1 - sinThetaT * sinThetaT));
    F32 Rparl = ((etaT * cosThetaI) - (etaI * cosThetaT)) /
                  ((etaT * cosThetaI) + (etaI * cosThetaT));
    F32 Rperp = ((etaI * cosThetaI) - (etaT * cosThetaT)) /
                  ((etaI * cosThetaI) + (etaT * cosThetaT));
    fS = (Rparl * Rparl + Rperp * Rperp) / 2;
    return Float3(fS, fS, fS);
}

F32 MicrofacetMaterial::lambda(F32 alphaX, F32 alphaY, const Float3& w) const
{
    F32 absTanTheta = fabsf(tanTheta(w));
    if (isinf(absTanTheta)) return 0.f;
    F32 alpha = sqrtf(cos2Phi(w) * alphaX * alphaX + 
                      sin2Phi(w) * alphaY * alphaY);
    F32 alpha2Tan2Theta = (alpha * absTanTheta) * (alpha * absTanTheta);
    return (-1.f + sqrtf(1.f + alpha2Tan2Theta)) / 2.f;
}

F32 MicrofacetMaterial::g(const Float3& wo, const Float3& wi, F32 alphaX, F32 alphaY)
{
    return 1.f / (1.f + lambda(alphaX, alphaY, wo) + lambda(alphaX, alphaY, wi));
}

F32 MicrofacetMaterial::d(F32 alphaX, F32 alphaY, const Float3& wh)
{
    F32 tan2T = tan2Theta(wh);
    if (isinf(tan2T)) return 0.f;
    F32 cos4Theta = cos2Theta(wh) * cos2Theta(wh);
    F32 e = (cos2Phi(wh) / (alphaX * alphaX) +
             sin2Phi(wh) / (alphaY * alphaY)) * tan2T;
    return 1.f / (RT_PI * alphaX * alphaY * cos4Theta * (1.f + e) * (1.f + e));
}

Float3 MicrofacetMaterial::distributionF(const Float3& wi, const Float3& wo)
{
    if (wo.z == 0.f) return Float3();
    // We must transfer our rays to local space, in order to use microfacet equations.
    F32 cosThetaO = absCosTheta(wo);
    F32 cosThetaI = absCosTheta(wi);
    Float3 wh = wi + wo; // half vector.
    if (cosThetaI == 0.f || cosThetaO == 0.f) return Float3();
    if (wh.x == 0.f && wh.y == 0.f && wh.z == 0.f) return Float3();
    wh = normalize(wh);
    Float3 fresnel = fresnelDielectric(dot(wi, wh), 0.5f, 0.1f);
    F32 alphaX = roughnessToAlpha(kD);      
    F32 alphaY = roughnessToAlpha(kD);
    return color * (d(alphaX, alphaY, wh) * g(wo, wi, alphaX, alphaY) * fresnel / (4 * cosThetaI * cosThetaO));
}

Float3 worldToLightLocal(const Float3& v, const SurfaceInteraction& si)
{
    // We are using Gram�Schmidt to calculate our TBN.
    Float3 ns = si.vNormal;
    Float3 ss = normalize(si.dpdu);
    Float3 ts = cross(ns, ss);
    return Float3(dot(v, ss), dot(v, ts), dot(v, ns));
}

Float3 lightLocalToWorld(const Float3& v, const SurfaceInteraction& si)
{
    // Using Gram-Schmidt to calculate TBN.
    Float3 ns = si.vNormal;
    Float3 ss = normalize(si.dpdu);
    Float3 ts = cross(ns, ss);
    return Float3(
        ss.x * v.x + ts.x * v.y + ns.x * v.z,
        ss.y * v.x + ts.y * v.y + ns.y * v.z,
        ss.z * v.x + ts.z * v.y + ns.z * v.z
    );
}

Float3 IMaterial::sampleDistributionF(const Float3& wo, Float3& wi, const Float2& u, F32& pdf)
{
    wi = Float3(-wo.x, -wo.y, wo.z);
    pdf = 1.f;
    Float3 wh = Float3();
    // TODO: We are going to need to make a separate BSDF struct for these formulas.
    Float3 r;
    return fresnelDielectric(cosTheta(wi), 0.5f, 0.1f) * r / absCosTheta(wi);
}

Float3 TrowbridgeReitzDistribution::sampleWh(const Float3& wo, const Float2& u)
{
    return Float3();
}

bool CustomMaterial::getTexture(const std::string& texName, Texture** pTextureOut)
{
    if (texName.empty())
        return false;

    auto& it = m_textureMap.find(texName);
    
    if ((it = m_textureMap.find(texName)) == m_textureMap.end())
        return false;

    *pTextureOut = it->second;
    return true;
}
} // rt