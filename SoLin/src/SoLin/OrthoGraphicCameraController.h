#pragma once

#include "SoLin/Core/Timestep.h"

#include"SoLin/Events/MouseEvent.h"
#include"SoLin/Events/ApplicationEvent.h"

#include"SoLin/Renderer/OrthoGraphicCamera.h"

namespace SoLin {

	class OrthoGraphicCameraController {
	public:
		//@param aspectRation 宽高比(x/y)
		//@param rotation 旋转标记(默认F)
		OrthoGraphicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep& ts);
		void OnEvent(Event& e);

		OrthoGraphicCamera& GetCamera() { return m_Camera; }
		const OrthoGraphicCamera& GetCamera()const { return m_Camera; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent e);
		bool OnWindowResized(WindowResizeEvent e);
	private:
		float m_AspectRatio;						//宽高比
		float m_ZoomLevel = 1.0f;					//缩放(默认1)
		OrthoGraphicCamera m_Camera;				//正交相机

		bool m_Rotation;							//旋转标志

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };			//相机位置
		float m_CameraRotation = 0.0f;								//角度
		float m_CameraTranslationSpeed = 5.0f;						//移动速度
		float m_CameraRotationSpeed = 180.0f;						//旋转速度
	};
}