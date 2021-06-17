#include "TestCubeMap.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"

namespace tests
{
	TestCubeMap::TestCubeMap()
	{
		// --------- 箱子 --------------------------------------------------
		float cubePositions[] = {

			-0.5f,	-0.5f,	 0.5f,	/*顶点 0-0*/		 0.0f,	 0.0f,	  1.0f,	/*法向量*/
			 0.5f,	-0.5f,	 0.5f,	/*顶点 1-1*/		 0.0f,	 0.0f,	  1.0f,	/*法向量*/
			 0.5f,	 0.5f,	 0.5f,	/*顶点 2-2*/		 0.0f,	 0.0f,	  1.0f,	/*法向量*/
			-0.5f,	 0.5f,	 0.5f,	/*顶点 3-3*/		 0.0f,	 0.0f,	  1.0f,	/*法向量*/

			-0.5f,	-0.5f,	-0.5f,	/*顶点 4-4*/		-1.0f,	 0.0f,	  0.0f,	/*法向量*/
			-0.5f,	-0.5f,	 0.5f,	/*顶点 5-0*/		-1.0f,	 0.0f,	  0.0f,	/*法向量*/
			-0.5f,	 0.5f,	 0.5f,	/*顶点 6-3*/		-1.0f,	 0.0f,	  0.0f,	/*法向量*/
			-0.5f,	 0.5f,	-0.5f,	/*顶点 7-7*/		-1.0f,	 0.0f,	  0.0f,	/*法向量*/

			 0.5f,	-0.5f,	-0.5f,	/*顶点 8-5*/		 0.0f,	 0.0f,	 -1.0f,	/*法向量*/
			-0.5f,	-0.5f,	-0.5f,	/*顶点 9-4*/		 0.0f,	 0.0f,	 -1.0f,	/*法向量*/
			-0.5f,	 0.5f,	-0.5f,	/*顶点 10-7*/	 0.0f,	 0.0f,	 -1.0f,	/*法向量*/
			 0.5f,	 0.5f,	-0.5f,	/*顶点 11-6*/	 0.0f,	 0.0f,	 -1.0f,	/*法向量*/

			 0.5f,	-0.5f,	 0.5f,	/*顶点 12-1*/	 1.0f,	 0.0f,	  0.0f,	/*法向量*/
			 0.5f,	-0.5f,	-0.5f,	/*顶点 13-5*/	 1.0f,	 0.0f,	  0.0f,	/*法向量*/
			 0.5f,	 0.5f,	-0.5f,	/*顶点 14-6*/	 1.0f,	 0.0f,	  0.0f,	/*法向量*/
			 0.5f,	 0.5f,	 0.5f,	/*顶点 15-2*/	 1.0f,	 0.0f,	  0.0f,	/*法向量*/

			-0.5f,	 0.5f,	 0.5f,	/*顶点 16-3*/	 0.0f,	 1.0f,	  0.0f,	/*法向量*/
			 0.5f,	 0.5f,	 0.5f,	/*顶点 17-2*/	 0.0f,	 1.0f,	  0.0f,	/*法向量*/
			 0.5f,	 0.5f,	-0.5f,	/*顶点 18-6*/	 0.0f,	 1.0f,	  0.0f,	/*法向量*/
			-0.5f,	 0.5f,	-0.5f,	/*顶点 19-7*/	 0.0f,	 1.0f,	  0.0f,	/*法向量*/

			-0.5f,	-0.5f,	-0.5f,	/*顶点 20-4*/	 0.0f,	-1.0f,	  0.0f,	/*法向量*/
			 0.5f,	-0.5f,	-0.5f,	/*顶点 21-5*/	 0.0f,	-1.0f,	  0.0f,	/*法向量*/
			 0.5f,	-0.5f,	 0.5f,	/*顶点 22-1*/	 0.0f,	-1.0f,	  0.0f,	/*法向量*/
			-0.5f,	-0.5f,	 0.5f,	/*顶点 23-0*/	 0.0f,	-1.0f,	  0.0f,	/*法向量*/
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
		cubeLayout.Push<float>(3);	// 法向量

		m_CubeVBO = CreateScope<VertexBuffer>(cubePositions, sizeof(cubePositions));
		m_CubeVAO->AddBuffer(*m_CubeVBO, cubeLayout);
		m_CubeIBO = CreateScope<IndexBuffer>(cubeIndices, sizeof(cubeIndices) / sizeof(unsigned int));

		// --------- 天空盒 --------------------------------------------------
		float skyboxPositions[] = {
			-1.0f,	-1.0f,	 1.0f,
			 1.0f,	-1.0f,	 1.0f,
			 1.0f,	 1.0f,	 1.0f,
			-1.0f,	 1.0f,	 1.0f,
			
			-1.0f,	-1.0f,	-1.0f,
			-1.0f,	-1.0f,	 1.0f,
			-1.0f,	 1.0f,	 1.0f,
			-1.0f,	 1.0f,	-1.0f,
		
			 1.0f,	-1.0f,	-1.0f,
			-1.0f,	-1.0f,	-1.0f,
			-1.0f,	 1.0f,	-1.0f,
			 1.0f,	 1.0f,	-1.0f,
		
			 1.0f,	-1.0f,	 1.0f,
			 1.0f,	-1.0f,	-1.0f,
			 1.0f,	 1.0f,	-1.0f,
			 1.0f,	 1.0f,	 1.0f,
		
			-1.0f,	 1.0f,	 1.0f,
			 1.0f,	 1.0f,	 1.0f,
			 1.0f,	 1.0f,	-1.0f,
			-1.0f,	 1.0f,	-1.0f,
		
			-1.0f,	-1.0f,	-1.0f,
			 1.0f,	-1.0f,	-1.0f,
			 1.0f,	-1.0f,	 1.0f,
			-1.0f,	-1.0f,	 1.0f,
		};

		unsigned int skyboxIndices[] = {
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

		m_SkyboxVAO = CreateScope<VertexArray>();

		VertexBufferLayout skyboxLayout;
		skyboxLayout.Push<float>(3);	// 顶点坐标(x,y,z)

		m_SkyboxVBO = CreateScope<VertexBuffer>(skyboxPositions, sizeof(skyboxPositions));
		m_SkyboxVAO->AddBuffer(*m_SkyboxVBO, skyboxLayout);
		m_SkyboxIBO = CreateScope<IndexBuffer>(skyboxIndices, sizeof(skyboxIndices) / sizeof(unsigned int));

		// --------- shader --------------------------------------------------
		m_Shader = CreateRef<Shader>("shaders/CubeMap.shader");
		m_ShaderSkybox = CreateRef<Shader>("shaders/CubeMap_Skybox.shader");

		m_CubeTexture = CreateRef<Texture>("../res/texture/container2.png");
		std::string dir = "../res/texture/skybox/";
		m_SkyboxTexture = CreateRef<Texture3D>(
			dir + "left.jpg",
			dir + "right.jpg", 
			dir + "top.jpg", 
			dir + "bottom.jpg", 
			dir + "front.jpg", 
			dir + "back.jpg" );

		m_CameraPosition = glm::vec3(0.0f, -1.0f, 5.0f);
		m_CameraPitch = glm::radians(0.0f);
		m_CameraYaw = glm::radians(180.0f);
		m_CameraWorldup = glm::vec3(0.0f, -1.0f, 0.0f);
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
	}

	TestCubeMap::~TestCubeMap()
	{
		glfwSetWindowUserPointer(m_Window, nullptr);
		glfwSetScrollCallback(m_Window, nullptr);
		glfwSetCursorPosCallback(m_Window, nullptr);
		glfwSetFramebufferSizeCallback(m_Window, nullptr);
	}

	void TestCubeMap::OnUpdate(float deltaTime)
	{
		ProcessInput(deltaTime);
	}

	void TestCubeMap::OnRender()
	{
		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Renderer renderer;

		m_Proj = glm::perspective(glm::radians(m_FOV), m_ScreenWidth / m_ScreenHeight/*Viewport的width/height*/, 0.1f, 100.0f);
		m_View = m_Camera->GetViewMatrix();

		// 箱子
		m_Model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, -1.0f))
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_ModelRotationAngle), m_ModelRotationDirection)
			* glm::scale(glm::mat4(1.0f), m_ModelScale);
		// 设置模型、观察和投影矩阵
		// 使用2D纹理对箱子进行渲染
		//m_CubeTexture->Bind();
		// 使用3D纹理对箱子渲染
		m_ShaderSkybox->Bind();
		m_Shader->Bind();
		m_Shader->SetUniform1i("textureIndex", 0);
		m_Shader->SetUniformMat4f("u_Proj", m_Proj);
		m_Shader->SetUniformMat4f("u_View", m_View);
		m_Shader->SetUniformMat4f("u_Model", m_Model);
		m_Shader->SetUniform3f("cameraPos", m_Camera->GetPosition());
		renderer.Draw(*m_CubeVAO, *m_CubeIBO, *m_Shader);

		// 天空盒
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		m_ShaderSkybox->Bind();
		m_ShaderSkybox->SetUniform1i("skybox", 0);
		m_SkyboxTexture->Bind();
		// 设置观察和投影矩阵
		m_ShaderSkybox->SetUniformMat4f("u_View", glm::mat4(glm::mat3(m_Camera->GetViewMatrix())/*移除位移分量*/));
		m_ShaderSkybox->SetUniformMat4f("u_Proj", m_Proj);
		renderer.Draw(*m_SkyboxVAO, *m_SkyboxIBO, *m_ShaderSkybox);
		glDepthFunc(GL_LESS); // set depth function back to default	
	}

	void TestCubeMap::OnImGuiRender()
	{
		//ImGui::Begin("Depth Opition");
		//ImGui::Checkbox("LESS or NO", &m_IsDepthLess);
		//ImGui::Checkbox("Depth Visual", &m_IsDepthVisual);
		//ImGui::End();
	}

	void TestCubeMap::InitCallback()
	{
		glfwSetWindowUserPointer(m_Window, this);

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) {

			TestCubeMap* self = (TestCubeMap*)glfwGetWindowUserPointer(window);

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

			TestCubeMap* self = (TestCubeMap*)glfwGetWindowUserPointer(window);

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

			TestCubeMap* self = (TestCubeMap*)glfwGetWindowUserPointer(window);
			self->m_ScreenWidth = width;
			self->m_ScreenHeight = height;
			glViewport(0, 0, width, height);
		});
	}

	void TestCubeMap::ProcessInput(float deltaTime)
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