#pragma once

#include "Test.h"

#include "../core/Camera.h"

#include "../core/VertexBuffer.h"
#include "../core/VertexBufferLayout.h"
#include "../core/Texture.h"
#include "../core/FrameBuffer.h"

#include <map>

namespace tests
{
	class TestFrameBuffer : public Test
	{
	private:
		Scope<VertexArray>		m_CubeVAO, m_PlaneVAO, m_GrassVAO, m_FrameVAO;
		Scope<VertexBuffer>		m_CubeVBO, m_PlaneVBO, m_GrassVBO, m_FrameVBO;
		Scope<IndexBuffer>		m_CubeIBO, m_PlaneIBO, m_GrassIBO, m_FrameIBO;
		Scope<FrameBuffer>		m_FBO;
		Scope<Shader>			m_Shader;
		Scope<Shader>			m_ShaderFrame;
		Scope<Texture>			m_CubeTexture;
		Scope<Texture>			m_FloorTexture;
		Scope<Texture>			m_GrassTexture;
		Scope<Texture>			m_WindowTexture;

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

		std::vector<glm::vec3> m_Vegetation;
	public:
		float m_ScreenWidth = 1200.0f;
		float m_ScreenHeight = 600.0f;

	public:
		TestFrameBuffer();
		~TestFrameBuffer();

	private:
		void DrawScene();

	public:
		void OnUpdate(float deltaTime) override;
		void OnRender()override;
		void OnImGuiRender()override;

	public:
		void InitCallback();
		void ProcessInput(float deltaTime);

	};
}

