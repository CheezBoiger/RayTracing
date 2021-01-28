// Raytracer.
#pragma once

#include "math/Matrix44.hpp"
#include "math/Float.hpp"
#include "math/Ray.hpp"

#include "scene/Camera.hpp"


namespace rt {


class Camera {
public:
    void adjustScreenToRaster(Float2 resolution)
    {
        static Float2 ndcMin = { -1, -1 };
        static Float2 ndcMax = {  1,  1 };
        m_screenToRaster = 

                translate(identity(), Float4(-ndcMin.x, -ndcMax.y, 0.0f, 1.0f)) *
                scale(identity(), Float4(1.f / (ndcMax.x - ndcMin.x), 
                                         1.f / (ndcMin.y - ndcMax.y), 1.0f, 1.0f)) *
                scale(identity(), Float4(resolution, 1.0f, 1.0f));
        m_rasterToScreen = inverse(m_screenToRaster);
    }

    void updateProjection(F32 fov, F32 aspect, F32 ne, F32 fa) {
        m_cameraToScreen = perspective(fov, aspect, ne, fa);
        m_screenToCamera = inverse(m_cameraToScreen);

        m_rasterToCamera = m_rasterToScreen * m_screenToCamera;
        m_cameraToRaster = inverse(m_rasterToCamera);
    }

    void update(Matrix44 cameraToWorld) 
    {
        m_cameraToWorld = cameraToWorld;
        m_worldToCamera = inverse(m_cameraToWorld);
    }
    
    Ray generateRay(F32 pixelX, F32 pixelY) {
        Float3 film = Float3(F32(pixelX) + 0.5f, F32(pixelY) + 0.5f, 0.f);
        Float3 camDir = Float4(film, 1.0f) * m_rasterToCamera;
        Ray camRay = Ray(Float3(), normalize(camDir));
        camRay = camRay * m_cameraToWorld;
        return camRay;
    }

    const Matrix44& getRasterToScreen() const { return m_rasterToScreen; }
    const Matrix44& getScreenToRaster() const { return m_screenToRaster; }

private:
    Matrix44 m_proj;
    Matrix44 m_rasterToScreen;
    Matrix44 m_screenToRaster;

    Matrix44 m_worldToCamera;
    Matrix44 m_cameraToWorld;

    Matrix44 m_cameraToScreen;
    Matrix44 m_screenToCamera;

    Matrix44 m_rasterToCamera;
    Matrix44 m_cameraToRaster;
};
} // rt