#include"slpch.h"
#include"OrthoGraphicCameraController.h"

#include"SoLin/Core/Input.h"
#include"SoLin/Core/KeyCodes.h"

namespace SoLin {
	OrthoGraphicCameraController::OrthoGraphicCameraController(float aspectRatio, bool rotation)
		:m_AspectRatio(aspectRatio),m_Rotation(rotation),
		m_Camera(-m_AspectRatio*m_ZoomLevel,m_AspectRatio*m_ZoomLevel,-m_ZoomLevel,m_ZoomLevel)
	{}

	void OrthoGraphicCameraController::OnUpdate(Timestep ts)
	{
        SL_PROFILE_FUNCTION();

		if (m_Rotation) {
			// 相机朝向是z的负方向，OpenGL中是右手坐标系，旋转是绕z轴转，加角度是四指弯曲方向
			// z正方向指向屏幕外，加角度就是逆时针转相机，物体是顺时针
			if (Input::IsKeyPressed(SL_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * ts;
			else if (Input::IsKeyPressed(SL_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * ts;

			if (m_CameraRotation > 180.0f)
				m_CameraRotation -= 360.0f;
			else if (m_CameraRotation < -180.0f)
				m_CameraRotation += 360.0f;

			m_Camera.SetRotation(m_CameraRotation);
		}

		if (Input::IsKeyPressed(SL_KEY_A))
		{
			m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(SL_KEY_D))
		{
			m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		if (Input::IsKeyPressed(SL_KEY_W))
		{
			m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(SL_KEY_S))
		{
			m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
		}
		m_Camera.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel;
	}
	void OrthoGraphicCameraController::OnEvent(Event& e)
	{
        SL_PROFILE_FUNCTION();

		EventDispatcher dispathcer(e);
		dispathcer.Dispatch<MouseScrolledEvent>(SOLIN_BIND_EVENT_FN(OrthoGraphicCameraController::OnMouseScrolled));
		dispathcer.Dispatch<WindowResizeEvent>(SOLIN_BIND_EVENT_FN(OrthoGraphicCameraController::OnWindowResized));

	}
	bool OrthoGraphicCameraController::OnMouseScrolled(MouseScrolledEvent e)
	{
        SL_PROFILE_FUNCTION();

		m_ZoomLevel -= e.GetYOffset() * 0.5f;							//向前滚动通常为负
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);			//保证缩放比例最低为0.25
		m_Camera.SetProjectionMatrix(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
	bool OrthoGraphicCameraController::OnWindowResized(WindowResizeEvent e)
	{
        SL_PROFILE_FUNCTION();

		// 更新宽高比
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjectionMatrix(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}
