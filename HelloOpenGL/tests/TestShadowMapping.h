#pragma once

#include "Test.h"

#include <random>

#include "../core/Camera.h"

#include "../core/VertexBuffer.h"
#include "../core/VertexBufferLayout.h"
#include "../core/Texture.h"

#include "../core/LightSpot.h"
#include "../core/FrameBuffer.h"

namespace tests
{
	// 高级光照 阴影映射
	class TestShadowMapping : public Test
	{
	private:
		Scope<VertexArray>		m_FloorVAO,m_CubeVAO,m_QuadVAO;
		Scope<VertexBuffer>		m_FloorVBO,m_CubeVBO,m_QuadVBO;
		Scope<IndexBuffer>		m_FloorIBO,m_CubeIBO,m_QuadIBO;

		unsigned int			m_DepthMapFBO;
		unsigned int			m_DepthMap;
		glm::vec3				m_LightPos;

		Ref<Texture>			m_FloorTexture;

		Ref<Shader>				m_Shader;
		Ref<Shader>				m_SimpleDepthShader;
		Ref<Shader>				m_DebugDepthQuad;

		Scope<FrameBuffer>		m_FBO;

		glm::mat4 m_Proj, m_View, m_Model;
		float m_FOV = 45.0f;

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
		TestShadowMapping();
		~TestShadowMapping();

	public:
		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	public:
		void InitCallback();
		void ProcessInput(float deltaTime);

	private:
		void RenderScene(Ref<Shader>& shader);

	};
}