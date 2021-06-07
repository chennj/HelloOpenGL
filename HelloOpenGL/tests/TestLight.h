#pragma once

#include "Test.h"

#include "../core/Camera.h"

#include "../core/VertexBuffer.h"
#include "../core/VertexBufferLayout.h"
#include "../core/Texture.h"

#include "../core/Material.h"
#include "../core/LightDirectional.h"
#include "../core/LightPoint.h"
#include "../core/LightSpot.h"

namespace tests
{
	class TestLight : public Test
	{
	private:
		Scope<VertexArray>		m_VAO;
		Scope<VertexBuffer>		m_VBO;
		Scope<IndexBuffer>		m_IBO;
		Ref<Shader>				m_Shader;
		Scope<Material>			m_Material;
		Scope<LightDirectional>	m_LightDirectional;
		Scope<LightPoint>		m_LightPoint0, m_LightPoint1, m_LightPoint2, m_LightPoint3;
		Scope<LightSpot>		m_LightSpot;
		Ref<Texture>			m_Diffuse;
		Ref<Texture>			m_Specular;

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

		glm::vec3		m_LightPosition = { 1.2f, 1.0f, 8.0f };
		glm::vec3		m_ViewPosition;
		glm::vec3		m_LightColor = { 1.0f,1.0f,1.0f };
		glm::vec3		m_ObjectColor = { 1.0f,0.74f,0.31f };

		bool m_EnableMouseCallback = false;

	public:
		TestLight();
		~TestLight();

	public:
		void OnUpdate(float deltaTime) override;
		void OnRender()override;
		void OnImGuiRender()override;

	public:
		void InitCallback();
		void ProcessInput(float deltaTime);
	};
}