#pragma once

#include "SoLin/Renderer/Camera.h"

namespace SoLin {
    class SceneCamera :public Camera {
    public:
        SceneCamera();
        virtual ~SceneCamera() = default;

        void SetOrthographic(float orthographicSize, float orthographicNear, float orthographicFar);

        void SetOrthographicSize(float size) { m_OrthographicSize = size;UpdateProjection(); }
        float GetOrthographicSize()const { return m_OrthographicSize; }

        void ViewportResize(uint32_t width, uint32_t height);
    private:
        void UpdateProjection();
    private:
        float m_AspectRatio = 0.0f;//宽高比

        float m_OrthographicSize = 10.0f;//正交尺寸
        float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;
    };
}
