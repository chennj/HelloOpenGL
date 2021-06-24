#pragma once

#include "Test.h"

#include "../core/Camera.h"

#include "../core/VertexBuffer.h"
#include "../core/VertexBufferLayout.h"
#include "../core/Texture.h"

#include "../core/Mesh.h"
#include "../core/Model.h"

namespace tests
{
	// 实例化 - 一次渲染大量的实例
	class TestInstancing : public Test
	{
	private:
		Ref<Shader>		m_ShaderPlanet,m_ShaderAsteriods;

		glm::mat4 m_Proj, m_View, m_Model;
		float m_FOV = 45.0f;

		float		m_ModelRotationAngle = 15.0f;
		glm::vec3	m_ModelRotationDirection = { 1.0f, 0.3f, 0.5f };
		glm::vec3	m_ModelPosition = { 0.0f, 0.0f, 0.0f };
		glm::vec3	m_ModelScale = { 4.0f, 4.0f, 4.0f };

		Scope<Camera>	m_Camera;
		glm::vec3		m_CameraPosition;
		glm::vec3		m_CameraWorldup;
		float			m_CameraPitch;
		float			m_CameraYaw;

		GLFWwindow*		m_Window;

		Scope<Model>	m_ModelPlanet, m_ModelAsteriods;

		bool m_EnableMouseCallback = false;

		glm::mat4* m_ModelMatrices;
		unsigned int m_AsteriodAmount = 10000;

	public:
		float m_ScreenWidth = 1200.0f;
		float m_ScreenHeight = 600.0f;

	public:
		TestInstancing();
		~TestInstancing();

	public:
		void OnUpdate(float deltaTime) override;
		void OnRender()override;
		void OnImGuiRender()override;

	public:
		void InitCallback();
		void ProcessInput(float deltaTime);
	};
}