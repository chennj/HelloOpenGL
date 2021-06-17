#pragma once

#include "Test.h"

#include "../core/Camera.h"

#include "../core/VertexBuffer.h"
#include "../core/VertexBufferLayout.h"
#include "../core/Texture.h"
#include "../core/Texture3D.h"

namespace tests
{
	class TestCubeMap : public Test
	{
	private:
		Scope<VertexArray>		m_CubeVAO, m_SkyboxVAO;
		Scope<VertexBuffer>		m_CubeVBO, m_SkyboxVBO;
		Scope<IndexBuffer>		m_CubeIBO, m_SkyboxIBO;
		Ref<Shader>				m_Shader,m_ShaderSkybox;
		Ref<Texture>			m_CubeTexture;
		Ref<Texture3D>			m_SkyboxTexture;

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
		TestCubeMap();
		~TestCubeMap();

	public:
		void OnUpdate(float deltaTime) override;
		void OnRender()override;
		void OnImGuiRender()override;

	public:
		void InitCallback();
		void ProcessInput(float deltaTime);

	};
}