// Raytracer.
#include "RayTracer.hpp"
#include "Light.hpp"

#include "common/Types.hpp"

#include "math/CommonMath.hpp"
#include "math/Float.hpp"
#include "math/Matrix44.hpp"

#include "framebuffer/RenderTarget.hpp"
#include "scene/Camera.hpp"
#include "scene/Scene.hpp"
#include "acceleration/SimpleContainer.hpp"
#include "geometry/Sphere.hpp"

#include <random>

using namespace rt;

int main(int c, char* argv[])
{
    DirectionLight dirLight;
    dirLight.enableShadowing(true);
    dirLight.wi = Float3(0.0f, 0.9f, 0.0f);
    dirLight.l = Float3(15.0f, 15.0f, 15.0f);
    
    Scene scene;
    SimpleContainer aggregate;
    scene.setAggregate(&aggregate);
    std::vector<Sphere> spheres;
    std::vector<Primitive*> primitives;
    std::vector<Material*> materials;

    std::random_device dev;
    std::mt19937 mt(dev());
    std::uniform_real_distribution<F32> cc(0.f, 1.f);
    std::uniform_real_distribution<F32> xy(-10.f, 10.f);
    std::uniform_real_distribution<F32> z(-15.f, 15.f);
    
    for (U32 i = 0; i < 150; ++i) 
    {
        Sphere sphere;
        sphere.m_localToWorld = translate(identity(), Float3(xy(mt), xy(mt), z(mt)));
        sphere.m_worldToLocal = inverse(sphere.m_localToWorld);
        sphere.m_radius = 1.f;
        spheres.push_back(sphere);
        //MatteMaterial* mat =  new MatteMaterial();
        //mat->color = Float3(cc(mt), cc(mt), cc(mt));
        MicrofacetMaterial* mat = new MicrofacetMaterial();
        mat->color = Float3(cc(mt), cc(mt), cc(mt));
        mat->kD = 0.04f;
        materials.push_back(mat);
    }
    for (U32 i = 0; i < 150; ++i)
    {
        Primitive* prim = new Primitive();
        prim->setShape(&spheres[i]);
        prim->setMat(materials[i]);
        primitives.push_back(prim);
    }
    scene.addPrimitive(primitives.size(), primitives.data());
    scene.addLight(&dirLight);
    Float2 resolution{ 1920, 1080 };
    Camera camera;
    Integrator integrator;

    ImageBuffer renderBuf((U32)resolution.x, (U32)resolution.y);
    RenderTarget rt;
    rt.width = (U32)resolution.x;
    rt.height = (U32)resolution.y;
    rt.surface = &renderBuf;

    camera.update(identity());
    camera.adjustScreenToRaster(resolution);
    camera.updateProjection(RT_RAD(45.0f), resolution.x / resolution.y, 0.001f, 1000.0f);
    Matrix44 worldToCamera =  rotate(identity(), Float3(1.0f, 0.0f, 0.0f), RT_RAD(45.0f));
    worldToCamera = translate(worldToCamera, Float3(0.0, 25.0, -25.0f));
    camera.update(worldToCamera);

    // Setup
    integrator.setCamera(&camera);
    integrator.setRenderTarget(&rt);
    // Trace the scene.
    integrator.render(&scene);

    return 0;
}