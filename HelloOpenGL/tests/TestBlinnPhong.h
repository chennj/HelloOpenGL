#pragma once

#include "Test.h"

#include <random>

#include "../core/Camera.h"

#include "../core/VertexBuffer.h"
#include "../core/VertexBufferLayout.h"
#include "../core/Texture.h"

#include "../core/LightSpot.h"

namespace tests
{
	// ∏ﬂº∂π‚’’ Blinn-Phong
	class TestBlinnPhong : public Test
	{
	private:
		Scope<VertexArray>		m_FloorVAO;
		Scope<VertexBuffer>		m_FloorVBO;
		Scope<IndexBuffer>		m_FloorIBO;
		Ref<Texture>			m_FloorTexture;
		Ref<Shader>				m_FloorShader;

		Scope<LightSpot>		m_LightSpot;
		glm::vec3				m_LightPosition = { 0.0f, 0.0f, 0.0f };

		glm::mat4 m_Proj, m_View, m_Model;
		float m_FOV = 45.0f;

		float		m_ModelRotationAngle = 0.0f;
		glm::vec3	m_ModelRotationDirection = { 1.0f, 0.3f, 0.5f };
		glm::vec3	m_ModelPosition = { 0.0f, 0.0f, 0.0f };
		glm::vec3	m_ModelScale = { 1.0f, 1.0f, 1.0f };

		glm::vec3* m_CubePositions;

		GLFWwindow* m_Window;

		Scope<Camera>	m_Camera;
		glm::vec3		m_CameraPosition;
		glm::vec3		m_CameraWorldup;
		float			m_CameraPitch;
		float			m_CameraYaw;

		bool m_EnableMouseCallback = false;
		bool m_IsBlinn = true;
	public:
		float m_ScreenWidth = 1200.0f;
		float m_ScreenHeight = 600.0f;

	public:
		TestBlinnPhong();
		~TestBlinnPhong();

	public:
		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	public:
		void InitCallback();
		void ProcessInput(float deltaTime);
	};
}