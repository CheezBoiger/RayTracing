// Raytracer.

#include "RayTracer.hpp"
#include "Interaction.hpp"
#include "Light.hpp"
#include "Material.hpp"

#include "framebuffer/Image.hpp"
#include "framebuffer/RenderTarget.hpp"

#include "scene/Camera.hpp"
#include "scene/Scene.hpp"

#include "math/CommonMath.hpp"

namespace rt {


B32 isBlack(const Float3& color)
{
    return color[0] == 0.f && color[1] == 0.f && color[2] == 0.f;
}


void Integrator::render(Scene* pScene)
{
    checkFrameBuffer();

    checkCamera();

    for (U32 y = 0; y < m_framebuffer.rt0->getHeight(); ++y) 
    {
        for (U32 x = 0; x < m_framebuffer.rt0->getWidth(); ++x)
        {
            Ray camRay = m_pCamera->generateRay((F32)x, (F32)y);
            Float3 rgb = li(camRay, pScene, 1);
            rgb.x = RT_CLAMP(rgb.x, 0.f, 1.f);
            rgb.y = RT_CLAMP(rgb.y, 0.f, 1.f);
            rgb.z = RT_CLAMP(rgb.z, 0.f, 1.f);
            // Write to image.
            m_framebuffer.rt0->storeColor(x, y, rgb);
        }
    }

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
            Float3 wis = toLocalSpace(wi, si);
            Float3 wos = toLocalSpace(wo, si);
            Float3 f = si.pMaterial->sampleDistributionF(wis, wos);
            
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
            if (!isBlack(f)) 
                radiance += (f * li * RT_ABS(dot(wi, si.vNormal))) * shadow;
        }
#if 1
        // Compute scattering, reflection.
        Float3 r = -reflect(ray.dir, si.vNormal);
        if (depth <= m_maxDepth)
        {
            // Compute ray from reflection. Increment depth in the process.
            // Compute the error, since we are using float values, they might 
            // overshoot into the interacted surface, which will cause ray to reflect onto
            // the same surface.
            Float3 err = si.vNormal * 0.001f;
            Ray reflectR =  { si.vPosition + err, r };
            radiance += li(reflectR, pScene, depth + 1) * 0.5;
        }
#endif
    }
        
    return radiance;
}

void Integrator::checkCamera()
{
}

void Integrator::checkFrameBuffer()
{
}
} // rt