#pragma once

#include "SoLin/Renderer/Camera.h"

namespace SoLin {
    class SceneCamera :public Camera {
    public:
        // @brief 投影类别
        enum class ProjectionType { Perspective = 0, Orthographic = 1 };

        ProjectionType GetProjectionType() { return m_ProjectionType; }
        void SetProjectionType(ProjectionType type) { m_ProjectionType = type; }
        //--------------------------------------------------------------------------
    public:
        SceneCamera();
        virtual ~SceneCamera() = default;

        void SetOrthographic(float orthographicSize, float orthographicNear, float orthographicFar);
        void SetPerspective(float perspectiveVerticalFOV, float perspectiveNear, float persepctiveFar);

        void SetOrthographicSize(float size) { m_OrthographicSize = size;UpdateProjection(); }
        float GetOrthographicSize()const { return m_OrthographicSize; }
        void  SetOrthographicNearClip(float nearClip) { m_OrthographicNear = nearClip; UpdateProjection(); }
        float GetOrthographicNearClip() const { return m_OrthographicNear; }
        void  SetOrthographicFarClip(float farClip) { m_OrthographicFar = farClip; UpdateProjection(); }
        float GetOrthographicFarClip() const { return m_OrthographicFar; }

        void  SetPerspectiveVerticalFOV(float verticalFov) { m_PerspectiveFOV = verticalFov;  UpdateProjection(); }		// "FOV" is in radians
        float GetPerspectiveVerticalFOV() const { return m_PerspectiveFOV; }
        void  SetPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; UpdateProjection(); }
        float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
        void  SetPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; UpdateProjection(); }
        float GetPerspectiveFarClip() const { return m_PerspectiveFar; }

        void ViewportResize(uint32_t width, uint32_t height);
    private:
        void UpdateProjection();
    private:
        // @brief 投影类型
        ProjectionType m_ProjectionType = ProjectionType::Orthographic;
    private:// Parameter for orthographic
        float m_AspectRatio = 0.0f;//宽高比

        float m_OrthographicSize = 10.0f;//正交尺寸
        float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;
    private:// Parameter for perspective
        float m_PerspectiveFOV = glm::radians(45.0f);
        float m_PerspectiveNear = 0.01f;
        float m_PerspectiveFar = 1000.0f;
    };
}
