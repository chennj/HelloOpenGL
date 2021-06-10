#include "TestFaceCulling.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"

namespace tests
{
	TestFaceCulling::TestFaceCulling()
	{
		// --------- 箱子 --------------------------------------------------
		float cubePositions[] = {

			-0.5f,	-0.5f,	 0.5f,	/*顶点 0-0*/		0.0f, 0.0f,	/*坐标*/
			 0.5f,	-0.5f,	 0.5f,	/*顶点 1-1*/		1.0f, 0.0f,	/*坐标*/
			 0.5f,	 0.5f,	 0.5f,	/*顶点 2-2*/		1.0f, 1.0f,	/*坐标*/
			-0.5f,	 0.5f,	 0.5f,	/*顶点 3-3*/		0.0f, 1.0f,	/*坐标*/

			-0.5f,	-0.5f,	-0.5f,	/*顶点 4-4*/		0.0f, 0.0f,	/*坐标*/
			-0.5f,	-0.5f,	 0.5f,	/*顶点 5-0*/		1.0f, 0.0f,	/*坐标*/
			-0.5f,	 0.5f,	 0.5f,	/*顶点 6-3*/		1.0f, 1.0f,	/*坐标*/
			-0.5f,	 0.5f,	-0.5f,	/*顶点 7-7*/		0.0f, 1.0f,	/*坐标*/

			 0.5f,	-0.5f,	-0.5f,	/*顶点 8-5*/		0.0f, 0.0f,	/*坐标*/
			-0.5f,	-0.5f,	-0.5f,	/*顶点 9-4*/		1.0f, 0.0f,	/*坐标*/
			-0.5f,	 0.5f,	-0.5f,	/*顶点 10-7*/	1.0f, 1.0f,	/*坐标*/
			 0.5f,	 0.5f,	-0.5f,	/*顶点 11-6*/	0.0f, 1.0f,	/*坐标*/

			 0.5f,	-0.5f,	 0.5f,	/*顶点 12-1*/	0.0f, 0.0f,	/*坐标*/
			 0.5f,	-0.5f,	-0.5f,	/*顶点 13-5*/	1.0f, 0.0f,	/*坐标*/
			 0.5f,	 0.5f,	-0.5f,	/*顶点 14-6*/	1.0f, 1.0f,	/*坐标*/
			 0.5f,	 0.5f,	 0.5f,	/*顶点 15-2*/	0.0f, 1.0f,	/*坐标*/

			-0.5f,	 0.5f,	 0.5f,	/*顶点 16-3*/	0.0f, 0.0f,	/*坐标*/
			 0.5f,	 0.5f,	 0.5f,	/*顶点 17-2*/	1.0f, 0.0f,	/*坐标*/
			 0.5f,	 0.5f,	-0.5f,	/*顶点 18-6*/	1.0f, 1.0f,	/*坐标*/
			-0.5f,	 0.5f,	-0.5f,	/*顶点 19-7*/	0.0f, 1.0f,	/*坐标*/

			// 演示面剔除时，底部和地面重合，有干扰，所以y向上提高一点，所以-0.49
			-0.5f,	-0.49f,	-0.5f,	/*顶点 20-4*/	0.0f, 0.0f,	/*坐标*/
			 0.5f,	-0.49f,	-0.5f,	/*顶点 21-5*/	1.0f, 0.0f,	/*坐标*/
			 0.5f,	-0.49f,	 0.5f,	/*顶点 22-1*/	1.0f, 1.0f,	/*坐标*/
			-0.5f,	-0.49f,	 0.5f,	/*顶点 23-0*/	0.0f, 1.0f,	/*坐标*/
		};

		unsigned int cubeIndices[] = {
			0,	1,	2,			// 第一个矩形
			2,	3,	0,			//

			4,	5,	6,			// 第二个矩形
			6,	7,	4,			//

			8,	9,	10,			// 第三个矩形
			10,	11,	8,			// 

			12, 13,	14,			// 第四个矩形
			14, 15, 12,			//

			16, 17, 18,			// 第五个矩形
			18, 19, 16,			//

			20, 21, 22,			// 第六个矩形
			22, 23, 20,			//
		};

		m_CubeVAO = CreateScope<VertexArray>();

		VertexBufferLayout cubeLayout;
		cubeLayout.Push<float>(3);	// 顶点坐标(x,y,z)
		cubeLayout.Push<float>(2);	// 纹理坐标(s,t)

		m_CubeVBO = CreateScope<VertexBuffer>(cubePositions, sizeof(cubePositions));
		m_CubeVAO->AddBuffer(*m_CubeVBO, cubeLayout);
		m_CubeIBO = CreateScope<IndexBuffer>(cubeIndices, sizeof(cubeIndices) / sizeof(unsigned int));

		// --------- 地面 --------------------------------------------------
		float planePositions[] = {
			// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
			-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
			 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
			 5.0f, -0.5f, -5.0f,  2.0f, 2.0f,
			-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		};

		unsigned int planeIndices[] = {
			0,1,2,
			2,3,0,
		};

		m_PlaneVAO = CreateScope<VertexArray>();

		VertexBufferLayout planeLayout;
		planeLayout.Push<float>(3);	// 顶点坐标(x,y,z)
		planeLayout.Push<float>(2);	// 纹理坐标(s,t)

		m_PlaneVBO = CreateScope<VertexBuffer>(planePositions, sizeof(planePositions));
		m_PlaneVAO->AddBuffer(*m_PlaneVBO, planeLayout);
		m_PlaneIBO = CreateScope<IndexBuffer>(planeIndices, sizeof(planeIndices) / sizeof(unsigned int));

		// --------- 草 --------------------------------------------------
		float grassPositions[] = {
			// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
			-0.5f, -0.5f,  0.0f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.0f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		};

		unsigned int grassIndices[] = {
			0,1,2,
			2,3,0,
		};

		m_GrassVAO = CreateScope<VertexArray>();

		VertexBufferLayout grassLayout;
		grassLayout.Push<float>(3);	// 顶点坐标(x,y,z)
		grassLayout.Push<float>(2);	// 纹理坐标(s,t)

		m_GrassVBO = CreateScope<VertexBuffer>(grassPositions, sizeof(grassPositions));
		m_GrassVAO->AddBuffer(*m_GrassVBO, grassLayout);
		m_GrassIBO = CreateScope<IndexBuffer>(grassIndices, sizeof(grassIndices) / sizeof(unsigned int));

		m_Vegetation.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
		m_Vegetation.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
		m_Vegetation.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
		m_Vegetation.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
		m_Vegetation.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

		// --------- shader --------------------------------------------------
		m_Shader = CreateScope<Shader>("shaders/Blending.shader");

		m_CubeTexture = CreateScope<Texture>("../res/texture/container2.png");
		m_FloorTexture = CreateScope<Texture>("../res/texture/cnchess/WHITE.GIF");
		m_GrassTexture = CreateScope<Texture>("../res/texture/grass.png");
		m_WindowTexture = CreateScope<Texture>("../res/texture/blending_transparent_window.png");

		m_CameraPosition = glm::vec3(0.0f, 2.0f, 10.0f);
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
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		/**
		*
		* 开启深度测试，因为深度信息都存储在Z缓冲中，默认是关闭的，如果不开启，
		* 渲染三维图形会很奇怪
		*/
		GLCall(glEnable(GL_DEPTH_TEST));

		/**
		*
		* 面剔除
		*/
		GLCall(glEnable(GL_CULL_FACE));
	}

	TestFaceCulling::~TestFaceCulling()
	{
		glfwSetWindowUserPointer(m_Window, nullptr);
		glfwSetScrollCallback(m_Window, nullptr);
		glfwSetCursorPosCallback(m_Window, nullptr);
		glfwSetFramebufferSizeCallback(m_Window, nullptr);
	}

	void TestFaceCulling::OnUpdate(float deltaTime)
	{
		ProcessInput(deltaTime);

		if (m_FrontOrBack)
			glCullFace(GL_FRONT);
		else
			glCullFace(GL_BACK);
	}

	void TestFaceCulling::OnRender()
	{
		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Renderer renderer;

		m_Proj = glm::perspective(glm::radians(m_FOV), m_ScreenWidth / m_ScreenHeight/*Viewport的width/height*/, 0.1f, 100.0f);
		m_View = m_Camera->GetViewMatrix();

		m_Shader->Bind();
		m_Shader->SetUniform1i("textureIndex", 0);

		m_Shader->SetUniformMat4f("u_View", m_View);
		m_Shader->SetUniformMat4f("u_Proj", m_Proj);

		// 地面
		// 绘制地面时禁用面剔除
		GLCall(glDisable(GL_CULL_FACE));
		m_FloorTexture->Bind();
		m_Shader->SetUniformMat4f("u_Model", glm::mat4(1.0f));
		renderer.Draw(*m_PlaneVAO, *m_PlaneIBO, *m_Shader);
		GLCall(glEnable(GL_CULL_FACE));

		// 箱子
		m_Model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, -1.0f))
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_ModelRotationAngle), m_ModelRotationDirection)
			* glm::scale(glm::mat4(1.0f), m_ModelScale);
		m_CubeTexture->Bind();
		m_Shader->SetUniformMat4f("u_Model", m_Model);
		renderer.Draw(*m_CubeVAO, *m_CubeIBO, *m_Shader);

		m_Model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f))
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_ModelRotationAngle), m_ModelRotationDirection)
			* glm::scale(glm::mat4(1.0f), m_ModelScale);
		m_CubeTexture->Bind();
		m_Shader->SetUniformMat4f("u_Model", m_Model);
		renderer.Draw(*m_CubeVAO, *m_CubeIBO, *m_Shader);

		// 草
		// 绘制草时禁用面剔除
		GLCall(glDisable(GL_CULL_FACE));
		m_GrassTexture->Bind();
		for (unsigned int i = 0; i < m_Vegetation.size(); i++)
		{
			m_Model = glm::mat4(1.0f);
			m_Model = glm::translate(m_Model, m_Vegetation[i]);
			m_Shader->SetUniformMat4f("u_Model", m_Model);
			renderer.Draw(*m_GrassVAO, *m_GrassIBO, *m_Shader);
		}
		GLCall(glEnable(GL_CULL_FACE));
	}

	void TestFaceCulling::OnImGuiRender()
	{
		ImGui::Checkbox("Front or Back", &m_FrontOrBack);
	}

	void TestFaceCulling::InitCallback()
	{
		glfwSetWindowUserPointer(m_Window, this);

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) {

			TestFaceCulling* self = (TestFaceCulling*)glfwGetWindowUserPointer(window);

			if (self->m_FOV >= 1.0f && self->m_FOV <= 45.0f)
				self->m_FOV -= yoffset;
			if (self->m_FOV <= 1.0f)
				self->m_FOV = 1.0f;
			if (self->m_FOV >= 45.0f)
				self->m_FOV = 45.0f;
		});

		static bool firstMouse = true;
		static double lastX = m_ScreenWidth;
		static double lastY = m_ScreenHeight;

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) {

			TestFaceCulling* self = (TestFaceCulling*)glfwGetWindowUserPointer(window);

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

			TestFaceCulling* self = (TestFaceCulling*)glfwGetWindowUserPointer(window);
			self->m_ScreenWidth = width;
			self->m_ScreenHeight = height;
			glViewport(0, 0, width, height);
		});
	}

	void TestFaceCulling::ProcessInput(float deltaTime)
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