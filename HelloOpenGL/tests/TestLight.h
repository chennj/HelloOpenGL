#pragma once

#include "Test.h"

#include "../core/Camera.h"

#include "../core/VertexBuffer.h"
#include "../core/VertexBufferLayout.h"
#include "../core/Texture.h"

#include "../core/Material.h"

namespace tests
{
	class TestLight : public Test
	{
	private:
		Scope<VertexArray>	m_VAO;
		Scope<VertexBuffer> m_VBO;
		Scope<IndexBuffer>	m_IBO;
		Ref<Shader>			m_Shader;
		Scope<Material>		m_Material;
		Scope<Texture>		m_Texture;
		Scope<Texture>		m_Background;

		glm::mat4 m_Proj, m_View, m_Model;
		float m_FOV = 45.0f;

		float		m_ModelRotationAngle		= 0.0f;
		glm::vec3	m_ModelRotationDirection	= { 1.0f, 0.3f, 0.5f };
		glm::vec3	m_ModelPosition				= { 0.0f, 0.0f, 0.0f };
		glm::vec3	m_ModelScale				= { 1.0f, 1.0f, 1.0f };

		glm::vec3* m_CubePositions;

		GLFWwindow* m_Window;

		Scope<Camera>	m_Camera;
		glm::vec3		m_CameraPosition;
		glm::vec3		m_CameraWorldup;
		float			m_CameraPitch;
		float			m_CameraYaw;

		glm::vec3		m_LightPosition = { 1.9f,0.8f,2.0f };
		glm::vec3		m_ViewPosition;
		glm::vec3		m_LightColor = { 1.0f,1.0f,1.0f };
		glm::vec3		m_ObjectColor = { 1.0f,0.74f,0.31f };

	public:
		TestLight();
		~TestLight();

	public:
		void OnUpdate(float deltaTime) override;
		void OnRender()override;
		void OnImGuiRender()override;

	public:
		void InitCallback();
	};
}