#include"slpch.h"

#include"OrthoGraphicCamera.h"

#include<glm/gtc/matrix_transform.hpp>

namespace SoLin {
	OrthoGraphicCamera::OrthoGraphicCamera(float left, float right, float bottom, float top)
		:m_ViewMatrix(1.0f),m_ProjectionMatrix(glm::ortho(left,right,bottom,top,-1.0f,1.0f))
	{
		m_ViewProjectionMatrix = m_ViewMatrix * m_ProjectionMatrix;
	}
	void OrthoGraphicCamera::SetProjectionMatrix(float left, float right, float bottom, float top)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
	void OrthoGraphicCamera::UpdateViewMatrix()
	{
		// 变化矩阵 = 位移矩阵 * 旋转矩阵（TR）
		//OpenGL中是从右向左乘，先旋转再位移
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));

		// 在相机的坐标系中，相机是被观测的，因此其变换与实际的视图矩阵是反的
		m_ViewMatrix = glm::inverse(transform);

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}