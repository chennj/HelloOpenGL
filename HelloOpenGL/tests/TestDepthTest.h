#pragma once

#include "Test.h"

#include "../core/Camera.h"

#include "../core/VertexBuffer.h"
#include "../core/VertexBufferLayout.h"
#include "../core/Texture.h"

namespace tests
{
	class TestDepthTest : public Test
	{
	private:
		Scope<VertexArray>		m_CubeVAO,m_PlaneVAO;
		Scope<VertexBuffer>		m_CubeVBO,m_PlaneVBO;
		Scope<IndexBuffer>		m_CubeIBO,m_PlaneIBO;
		Ref<Shader>				m_Shader;
		Ref<Texture>			m_CubeTexture;
		Ref<Texture>			m_FloorTexture;

		glm::mat4 m_Proj, m_View, m_Model;
		float m_FOV = 45.0f;

		float		m_ModelRotationAngle = 0.0f;
		glm::vec3	m_ModelRotationDirection = { 1.0f, 0.3f, 0.5f };
		glm::vec3	m_ModelPosition = { 0.0f, 0.0f, 0.0f };
		glm::vec3	m_ModelScale = { 1.0f, 1.0f, 1.0f };

		GLFWwindow* m_Window;

		Scope<Camera>	m_Camera;
		glm::vec3		m_CameraPosition;
		glm::vec3		m_CameraWorldup;
		float			m_CameraPitch;
		float			m_CameraYaw;

		bool m_EnableMouseCallback = false;
		bool m_IsDepthLess = true;
		bool m_IsDepthVisual = false;

	public:
		float m_ScreenWidth = 1200.0f;
		float m_ScreenHeight = 600.0f;

	public:
		TestDepthTest();
		~TestDepthTest();

	public:
		void OnUpdate(float deltaTime) override;
		void OnRender()override;
		void OnImGuiRender()override;

	public:
		void InitCallback();
		void ProcessInput(float deltaTime);

	};
}