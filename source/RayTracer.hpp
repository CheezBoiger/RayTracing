// Raytracer.
#pragma once
#include "common/Types.hpp"

#include "scene/Camera.hpp"
#include "scene/Scene.hpp"

#include "math/Float.hpp"
#include "math/Ray.hpp"
#include <vector>

namespace rt {

struct Light;
struct RenderTarget;
struct SurfaceInteraction;

class Image;
class Scene;

class Integrator {
public:
    Integrator()
        : m_maxDepth(2)
        , m_samples(1)
        , m_output(nullptr)
    {
        m_framebuffer.rt0 = nullptr;
    }

    // Render the scene.
    //
    void render(Scene* pScene);

    // Calculate incidence radiance along the camera ray.
    // This function handles the light contributions to this given ray.
    Float3 li(Ray& ray, Scene* pScene, I32 depth = 0);

    Float3 specularReflect(const Ray& ray, Scene* pScene, const SurfaceInteraction& si, I32 depth);
    Float3 specularTransmit(const Ray& ray, Scene* pScene, const SurfaceInteraction& si, I32 depth);

    void setCamera(Camera* cam) {
        m_pCamera = cam;
    }

    void setRenderTarget(RenderTarget* rt) 
    {
        m_framebuffer.rt0 = rt;    
    }

    void setSamples(U32 samples) { m_samples = samples > 0 ? samples : 1; }

private:

    void checkCamera();
    void checkFrameBuffer();

    Camera* m_pCamera;

    struct {
        RenderTarget* rt0;
    } m_framebuffer;

    Image*              m_output;
    U32                 m_maxDepth;
    U32                 m_samples;
};
} // rt
