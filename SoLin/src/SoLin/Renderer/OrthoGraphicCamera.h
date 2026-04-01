#pragma once
#include<glm/glm.hpp>

namespace SoLin {

	// @brief 正交相机
	class OrthoGraphicCamera {
	public:
		//@param left -x
		//@param right +x
		//@param bottom -y
		//@param top +y
		OrthoGraphicCamera(float left, float right, float bottom, float top);

		void SetProjectionMatrix(float left, float right, float bottom, float top);

		void SetPosition(const glm::vec3& position) { m_Position = position; UpdateViewMatrix(); }
		void SetRotation(const float& rotation) { m_Rotation = rotation; UpdateViewMatrix(); }

		const glm::vec3& GetPosition() const { return m_Position; }
		const float& GetRotation()const { return m_Rotation; }

		const glm::mat4& GetViewMatrix()const { return m_ViewMatrix; }
		const glm::mat4& GetProjectionMatrix()const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewProjectionMatrix()const { return m_ViewProjectionMatrix; }

	private:
		// @brief 用于更新视图矩阵与视图投影矩阵
		void UpdateViewMatrix();
	private:
		glm::vec3 m_Position = { 0.0f,0.0f,0.0f };	//相机位置(自身坐标系)
		float m_Rotation = 0.0f;					//旋转角度(正交相机是二维的，只会围绕z轴旋转)

		glm::mat4 m_ViewMatrix;						//视图矩阵
		glm::mat4 m_ProjectionMatrix;				//投影矩阵
		glm::mat4 m_ViewProjectionMatrix;			//视图投影矩阵	逻辑上先V再P
	};
}