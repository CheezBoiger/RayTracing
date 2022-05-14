// Raytracer.

#include "RayTracer.hpp"
#include "Interaction.hpp"
#include "Light.hpp"
#include "Material.hpp"
#include "common/Threading.hpp"

#include "framebuffer/Image.hpp"
#include "framebuffer/RenderTarget.hpp"

#include "scene/Camera.hpp"
#include "scene/Scene.hpp"

#include "math/CommonMath.hpp"

#define IMDEBUGGING 1
#if IMDEBUGGING
    #include <iostream>
    #include <stdio.h>
    #define DEBUG(s, ...) printf(s##,__VA_ARGS__)

    // Try to print this crap, in c++
    template<class F, typename... Args> void doFor(F fun, Args... args) {
        int x[] = { (fun(args), 0)... };
    }

    template<typename... Args> void printDebug(Args... args) { 
        using namespace std;
        doFor([&](auto arg) -> void { std::cout << arg; }, args...);
    }
#else
    #define DEBUG(s, ...)
    template<typename... Args> void printDebug(Args... args) { }
#endif

namespace rt {


B32 isBlack(const Float3& color)
{
   // printDebug(" ", 3, " ", 4, " cat", "\n");
    return color[0] == 0.f && color[1] == 0.f && color[2] == 0.f;
}

struct Sample {
    F32 x;
    F32 y;
};

static Sample sample4[] {
    { 0.25f, 0.25 },
    { -0.25, -0.25 },
    { 0.25, -0.25 },
    {-0.25, 0.25 }
};

void Integrator::render(Scene* pScene)
{
    checkFrameBuffer();

    checkCamera();

    U64 frameWidth = m_framebuffer.rt0->getWidth();
    U64 frameHeight = m_framebuffer.rt0->getHeight();

    dispatch({[=] (const ThreadID& id) -> void {
        Float3 accumColor;
        U32 x = id.global.x;
        U32 y = id.global.y;

        if (x >= frameWidth || y >= frameHeight)
            return;

        for (U32 sample = 0; sample < m_samples; ++sample) {
            F32 posX = (F32)x + sample4[sample].x;
            F32 posY = (F32)y + sample4[sample].y;
            Ray camRay = m_pCamera->generateRay(posX, posY);
            Float3 sceneColor = li(camRay, pScene, 1);
            // Tonemap. Since this is optional, we need to check if there is a function to use. Otherwise,
            // just store the raw color.
            Float3 rgb = (m_tonemap.evaluate) ? m_tonemap.evaluate(sceneColor) : sceneColor;
            accumColor += rgb;
        }

        accumColor = accumColor / m_samples;
        accumColor.x = RT_CLAMP(accumColor.x, 0.f, 1.f);
        accumColor.y = RT_CLAMP(accumColor.y, 0.f, 1.f);
        accumColor.z = RT_CLAMP(accumColor.z, 0.f, 1.f);
        // Write to image.
        m_framebuffer.rt0->storeColor(x, y, accumColor);
    }, 64, 64, 1 }, m_framebuffer.rt0->getWidth() / 64 + 1, m_framebuffer.rt0->getHeight() / 64 + 1, 1);

    //
    m_output = createPNG("Test.png");
    if (m_output)
    {
        m_output->saveBuffer(m_framebuffer.rt0->getBuffer(), 
                            m_framebuffer.rt0->getWidth(), m_framebuffer.rt0->getHeight(), 3);
    }
    destroyImage(m_output);
    m_output = nullptr;
}

Float3 Integrator::li(Ray& ray, Scene* pScene, I32 depth)
{
    // Calculate radiance along the camera ray.
    Float3 radiance = Float3(0.0f, 0.0f, 0.0f);

    SurfaceInteraction si = { };
    si.time = INFINITY;

    std::vector<Light*>& lights = pScene->getLights();
 
    if (pScene->intersects(ray, si))
    {
        //radiance += si.pMaterial->color;
        for (U32 i = 0; i < lights.size(); ++i)
        {
            Light* light = lights[i];
            Float3 wi = light->getLightDirection();
            Float3 li = light->sampleLi(si, wi);
            Float3 wo = si.wo; // Outgoing direction, usually represents the eye
            // Obtain the local space for the bsdf.
            Float3 wis = worldToLightLocal(wi, si);
            Float3 wos = worldToLightLocal(wo, si);
            Float3 f = si.pMaterial->distributionF(wis, wos);
            
            F32 shadow = 1.f;
            if (light->isShadowing())
            {
                // Spawn shadow ray from point to direction of light source.
                Ray shadowRay = light->emitShadowRay(si);
                // check if shadow ray intersect an object in the scene.
                // Actually a pretty shitty way to do it, especially because it will
                // fail on glossy surfaces. Need to find another way.
                SurfaceInteraction shadowSI = { };
                if (pScene->intersects(shadowRay, shadowSI))
                {
                    // Determine the material, otherwise, assume it is opaque. No
                    // radiance applied to this point.
                    shadow = 0.f;
                }
            }
            // Light contribution factored by the BSDF distribution.
            
            F32 kD = dot(wi, si.vNormal);
            if (!isBlack(f) && kD > 0.f) 
                radiance += (f * li * kD) * shadow;
        }

        // Compute scattering, reflection and transmission.
        if (depth <= m_maxDepth)
        {
            radiance += specularReflect(ray, pScene, si, depth + 1);
            radiance += specularTransmit(ray, pScene, si, depth + 1);
        }
    }
        
    return radiance;
}


Float3 Integrator::specularReflect(const Ray& ray, Scene* pScene, const SurfaceInteraction& si, I32 depth)
{
    Float3 woW = si.wo;
    Float3 wiW;

    // Convert to light space.
    Float3 wo = worldToLightLocal(woW, si);
    Float3 wi = worldToLightLocal(wiW, si);
    F32 pdf = 0.f;
    Float3 f = Float3(0.3f, 0.3f, 0.3f); //si.pMaterial->sampleDistributionF(wo, wi, Float2(0, 0), pdf);

    if (!isBlack(f))
    {
        wiW = -reflect(ray.dir, si.vNormal);
        // Compute ray from reflection. Increment depth in the process.
        // Compute the error, since we are using float values, they might 
        // overshoot into the interacted surface, which will cause ray to reflect onto
        // the same surface.
        Float3 err = si.vNormal * 0.001f;
        Ray reflectR =  { si.vPosition + err, wiW };
        return f * li(reflectR, pScene, depth + 1);
    }
        
    return f;
}

Float3 Integrator::specularTransmit(const Ray& ray, Scene* pScene, const SurfaceInteraction& si, I32 depth)
{
    return Float3(0.f);
}

void Integrator::checkCamera()
{
}

void Integrator::checkFrameBuffer()
{
}
} // rt