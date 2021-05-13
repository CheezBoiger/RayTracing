// Raytracer.
#pragma once

#include "math/Float.hpp"
#include "math/Ray.hpp"

#include "Interaction.hpp"

namespace rt {

struct SurfaceInteraction;

struct Light 
{
    // Light radiance (luminance)
    virtual Float3 sampleLi(const SurfaceInteraction& si, Float3& wi) = 0;
    
    virtual Float3 getLightDirection() = 0;

    B32 isShadowing() const { return m_shadowing; }

    void enableShadowing(B32 enable) { m_shadowing = enable; }

    virtual Ray emitShadowRay(SurfaceInteraction& si) = 0;

private:
    B32 m_shadowing;
};

struct PointLight : public Light
{
    Float3 position;
    Float3 i;

    Float3 sampleLi(const SurfaceInteraction& si, Float3& wi) override {
        wi = normalize(position - si.vPosition);
        return i / length2(position - si.vPosition);
    }
};

struct DirectionLight : public Light 
{
    Float3 wi; // light direction.
    Float3 l;  // light radiance.

    Float3 sampleLi(const SurfaceInteraction& si, Float3& wi) override {
        wi = this->wi;
        return l;
    }

    Float3 getLightDirection() override
    {
        return wi;
    }

    Ray emitShadowRay(SurfaceInteraction& si) override
    {
        Float3 err = si.vNormal * 0.0005f;
        return { si.vPosition + err, wi };
    }
};


struct AreaLight : public Light
{
    AreaLight(const Matrix44& lightToWorld, I32 nSamples);
    virtual Float3 l(const SurfaceInteraction& si, const Float3& w) const = 0;

};

struct DiffuseAreaLight : public AreaLight
{
    virtual Float3 l(const SurfaceInteraction& si, const Float3& w) const override { return Float3(); }
};
} // rt