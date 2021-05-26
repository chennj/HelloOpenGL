#pragma once

#include "Test.h"

#include "../core/Camera.h"

#include "../core/VertexBuffer.h"
#include "../core/VertexBufferLayout.h"
#include "../core/Texture.h"

namespace tests
{
	class TestCamera : public Test
	{
	private:
		Scope<VertexArray> _VAO;
		Scope<VertexBuffer> _VBO;
		Scope<IndexBuffer> _IndexBuffer;
		Scope<Shader> _Shader;
		Scope<Texture> _Texture;

		glm::vec3 _MPosition;
		glm::mat4 _MRotation;
		glm::vec3 _MRotationDirection;
		float _MRotationAngle;
		glm::vec3 _MScale;

		glm::mat4 _Proj, _View, _Model;
		float m_fov;

		glm::vec3* _CubePositions;

		Scope<Camera> m_camera;
		GLFWwindow* m_window;
		glm::vec3 m_camera_position;
		glm::vec3 m_camera_worldup;
		float m_camera_pitch;
		float m_camera_yaw;

	public:
		TestCamera();
		~TestCamera();

	public:
		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

		void InitCallback();
	};
}

