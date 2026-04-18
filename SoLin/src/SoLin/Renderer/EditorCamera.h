#pragma once
#include"Camera.h"
#include"SoLin/Core/Timestep.h"
#include"SoLin/Events/Event.h"
#include"SoLin/Events/MouseEvent.h"

#include<glm/glm.hpp>

namespace SoLin {
    class EditorCamera :public Camera {
    public:
        EditorCamera() = default;
        EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

        void OnUpdate(Timestep ts);
        void OnEvent(Event& e);

        inline float GetDistance() const { return m_Distance; }
        inline void SetDistance(float distance) { m_Distance = distance; }

        inline void SetViewportSize(float width, float height) { m_ViewportWidth = width;m_ViewportHeight = height; UpdateProjection(); }

        const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
        glm::mat4 GetViewProjection()const { return m_ProjectionMatrix * m_ViewMatrix; }

        // @brief 获取垂直距离
        glm::vec3 GetUpDirection() const;
        // @brief 获取水平距离
        glm::vec3 GetRightDirection() const;
        // @brief 获取前进距离
        glm::vec3 GetForwardDirection() const;
        const glm::vec3& GetPosition() const { return m_Position; }
        // @brief 获取方向
        glm::quat GetOrientation() const;

        // @brief 获取俯仰角
        float GetPitch() const { return m_Pitch; }
        // @brief 获取偏航角
        float GetYaw() const { return m_Yaw; }
    private:
        // @brief 更新投影
        void UpdateProjection();
        // @brief 更新视图
        void UpdateView();

        // @brief 鼠标滚轮
        bool OnMouseScroll(MouseScrolledEvent& e);

        // @brief 鼠标平移
        void MousePan(const glm::vec2& delta);
        // @brief 鼠标旋转
        void MouseRotate(const glm::vec2& delta);
        // @brief 鼠标缩放
        void MouseZoom(float delta);

        // @brief 计算位置
        glm::vec3 CalculatePosition() const;

        // @brief 平移步长
        std::pair<float, float> PanSpeed() const;
        // @brief 旋转步长
        float RotationSpeed() const;
        // @brief 缩放步长
        float ZoomSpeed() const;
    private:
        float m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;

        glm::mat4 m_ViewMatrix;                             // 视口矩阵
        glm::vec3 m_Position = { 0.0f,0.0f,0.0f };          // 位置
        glm::vec3 m_FocalPoint = { 0.0f,0.0f,0.0f };        // 焦点

        glm::vec2 m_InitialMousePosition = { 0.0f,0.0f };   // 初始鼠标位置

        float m_Distance = 10.0f;                           // 距离
        float m_Pitch = 0.0f, m_Yaw = 0.0f;                 // 欧拉角

        float m_ViewportWidth = 1280;// 视口宽
        float m_ViewportHeight = 720;// 视口高

    };
}
