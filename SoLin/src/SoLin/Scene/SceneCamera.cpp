#include "slpch.h"
#include"SceneCamera.h"

#include<glm/gtc/matrix_transform.hpp>

namespace SoLin {

    SceneCamera::SceneCamera() {
        UpdateProjection();
    }
    void SceneCamera::SetOrthographic(float orthographicSize, float orthographicNear, float orthographicFar)
    {
        m_OrthographicSize = orthographicSize;
        m_OrthographicNear = orthographicNear;
        m_OrthographicFar = orthographicFar;

        UpdateProjection();
    }
    void SceneCamera::ViewportResize(uint32_t width, uint32_t height)
    {
        m_AspectRatio = (float)width / (float)height;

        UpdateProjection();
    }
    void SceneCamera::UpdateProjection()
    {
        float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
        float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
        float orthoBottom = -m_OrthographicSize * 0.5f;
        float orthoTop = m_OrthographicSize * 0.5f;

        m_ProjectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
    }
}
