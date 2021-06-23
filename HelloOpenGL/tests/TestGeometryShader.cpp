#include "TestGeometryShader.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"

namespace tests
{
	TestGeometryShader::TestGeometryShader()
	{
		// --------- 点位置 --------------------------------------------------
		float points[] = {
			-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // top-left
			 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // top-right
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
			-0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left
		};

		unsigned int pointIndices[] = {
			0,	1,	2,	3,
		};

		m_PointVAO = CreateScope<VertexArray>();

		VertexBufferLayout pointLayout;
		pointLayout.Push<float>(2);	// 顶点坐标(x,y,z)
		pointLayout.Push<float>(3);	// 法向量

		m_PointVBO = CreateScope<VertexBuffer>(points, sizeof(points));
		m_PointVAO->AddBuffer(*m_PointVBO, pointLayout);
		m_PointIBO = CreateScope<IndexBuffer>(pointIndices, sizeof(pointIndices) / sizeof(unsigned int));

		// --------- shader --------------------------------------------------
		m_Shader = CreateRef<Shader>("shaders/Geometry.vs.glsl", "shaders/Geometry.fs.glsl", "shaders/Geometry.gs.glsl");

		m_CameraPosition = glm::vec3(0.0f, -1.0f, 3.0f);
		m_CameraPitch = glm::radians(0.0f);
		m_CameraYaw = glm::radians(180.0f);
		m_CameraWorldup = glm::vec3(0.0f, 1.0f, 0.0f);
		// 欧拉角法 
		m_Camera = CreateScope<Camera>(
			m_CameraPosition,	// 相机位置
			m_CameraPitch,		// 相机向上下转动角度
			m_CameraYaw,		// 相机左右转动角度,右手系-》左手系 = 转动180
			m_CameraWorldup		// 世界坐标向上方向向量
			);

		m_Window = glfwGetCurrentContext();
		ASSERT(m_Window);

		InitCallback();

		/**
		*
		* blending 混合方式
		* 具体解释：看 /doc/blend-*.jpg
		*/
		//GLCall(glEnable(GL_BLEND));
		//GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		/**
		*
		* 开启深度测试，因为深度信息都存储在Z缓冲中，默认是关闭的，如果不开启，
		* 渲染三维图形会很奇怪
		*/
		GLCall(glEnable(GL_DEPTH_TEST));
	}

	TestGeometryShader::~TestGeometryShader()
	{
		glfwSetWindowUserPointer(m_Window, nullptr);
		glfwSetScrollCallback(m_Window, nullptr);
		glfwSetCursorPosCallback(m_Window, nullptr);
		glfwSetFramebufferSizeCallback(m_Window, nullptr);
	}

	void TestGeometryShader::OnUpdate(float deltaTime)
	{
		ProcessInput(deltaTime);
	}

	void TestGeometryShader::OnRender()
	{
		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Renderer renderer;
		renderer.DrawPoint(*m_PointVAO, *m_PointIBO, *m_Shader);
	}

	void TestGeometryShader::OnImGuiRender()
	{
		//ImGui::Begin("Depth Opition");
		//ImGui::Checkbox("LESS or NO", &m_IsDepthLess);
		//ImGui::Checkbox("Depth Visual", &m_IsDepthVisual);
		//ImGui::End();
	}

	void TestGeometryShader::InitCallback()
	{
		glfwSetWindowUserPointer(m_Window, this);

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) {

			TestGeometryShader* self = (TestGeometryShader*)glfwGetWindowUserPointer(window);

			if (self->m_FOV >= 1.0f && self->m_FOV <= 45.0f)
				self->m_FOV -= yoffset;
			if (self->m_FOV <= 1.0f)
				self->m_FOV = 1.0f;
			if (self->m_FOV >= 45.0f)
				self->m_FOV = 45.0f;
		});

		static bool firstMouse = true;
		static double lastX = m_ScreenWidth / 2;
		static double lastY = m_ScreenHeight / 2;

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) {

			TestGeometryShader* self = (TestGeometryShader*)glfwGetWindowUserPointer(window);

			if (!self->m_EnableMouseCallback)return;

			if (firstMouse)
			{
				lastX = xpos;
				lastY = ypos;
				firstMouse = false;
			}

			float xoffset = xpos - lastX;
			float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

			lastX = xpos;
			lastY = ypos;

			self->m_Camera->MouseMovement(xoffset, yoffset);
		});

		glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {

			TestGeometryShader* self = (TestGeometryShader*)glfwGetWindowUserPointer(window);
			self->m_ScreenWidth = width;
			self->m_ScreenHeight = height;
			glViewport(0, 0, width, height);
		});
	}

	void TestGeometryShader::ProcessInput(float deltaTime)
	{
		if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			m_EnableMouseCallback = false;

		if (glfwGetKey(m_Window, GLFW_KEY_SPACE) == GLFW_PRESS)
			m_EnableMouseCallback = true;

		if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
			m_Camera->Keyboard(FORWARD, deltaTime);

		if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
			m_Camera->Keyboard(BACKWARD, deltaTime);

		if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
			m_Camera->Keyboard(LEFT, deltaTime);

		if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
			m_Camera->Keyboard(RIGHT, deltaTime);

		if (glfwGetKey(m_Window, GLFW_KEY_Q) == GLFW_PRESS)
			m_Camera->Keyboard(UP, deltaTime);

		if (glfwGetKey(m_Window, GLFW_KEY_E) == GLFW_PRESS)
			m_Camera->Keyboard(DOWN, deltaTime);
	}
}