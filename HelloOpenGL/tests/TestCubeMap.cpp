#include "TestCubeMap.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"

namespace tests
{
	TestCubeMap::TestCubeMap()
	{
		// --------- ���� --------------------------------------------------
		float cubePositions[] = {

			-0.5f,	-0.5f,	 0.5f,	/*���� 0-0*/		 0.0f,	 0.0f,	  1.0f,	/*������*/
			 0.5f,	-0.5f,	 0.5f,	/*���� 1-1*/		 0.0f,	 0.0f,	  1.0f,	/*������*/
			 0.5f,	 0.5f,	 0.5f,	/*���� 2-2*/		 0.0f,	 0.0f,	  1.0f,	/*������*/
			-0.5f,	 0.5f,	 0.5f,	/*���� 3-3*/		 0.0f,	 0.0f,	  1.0f,	/*������*/

			-0.5f,	-0.5f,	-0.5f,	/*���� 4-4*/		-1.0f,	 0.0f,	  0.0f,	/*������*/
			-0.5f,	-0.5f,	 0.5f,	/*���� 5-0*/		-1.0f,	 0.0f,	  0.0f,	/*������*/
			-0.5f,	 0.5f,	 0.5f,	/*���� 6-3*/		-1.0f,	 0.0f,	  0.0f,	/*������*/
			-0.5f,	 0.5f,	-0.5f,	/*���� 7-7*/		-1.0f,	 0.0f,	  0.0f,	/*������*/

			 0.5f,	-0.5f,	-0.5f,	/*���� 8-5*/		 0.0f,	 0.0f,	 -1.0f,	/*������*/
			-0.5f,	-0.5f,	-0.5f,	/*���� 9-4*/		 0.0f,	 0.0f,	 -1.0f,	/*������*/
			-0.5f,	 0.5f,	-0.5f,	/*���� 10-7*/	 0.0f,	 0.0f,	 -1.0f,	/*������*/
			 0.5f,	 0.5f,	-0.5f,	/*���� 11-6*/	 0.0f,	 0.0f,	 -1.0f,	/*������*/

			 0.5f,	-0.5f,	 0.5f,	/*���� 12-1*/	 1.0f,	 0.0f,	  0.0f,	/*������*/
			 0.5f,	-0.5f,	-0.5f,	/*���� 13-5*/	 1.0f,	 0.0f,	  0.0f,	/*������*/
			 0.5f,	 0.5f,	-0.5f,	/*���� 14-6*/	 1.0f,	 0.0f,	  0.0f,	/*������*/
			 0.5f,	 0.5f,	 0.5f,	/*���� 15-2*/	 1.0f,	 0.0f,	  0.0f,	/*������*/

			-0.5f,	 0.5f,	 0.5f,	/*���� 16-3*/	 0.0f,	 1.0f,	  0.0f,	/*������*/
			 0.5f,	 0.5f,	 0.5f,	/*���� 17-2*/	 0.0f,	 1.0f,	  0.0f,	/*������*/
			 0.5f,	 0.5f,	-0.5f,	/*���� 18-6*/	 0.0f,	 1.0f,	  0.0f,	/*������*/
			-0.5f,	 0.5f,	-0.5f,	/*���� 19-7*/	 0.0f,	 1.0f,	  0.0f,	/*������*/

			-0.5f,	-0.5f,	-0.5f,	/*���� 20-4*/	 0.0f,	-1.0f,	  0.0f,	/*������*/
			 0.5f,	-0.5f,	-0.5f,	/*���� 21-5*/	 0.0f,	-1.0f,	  0.0f,	/*������*/
			 0.5f,	-0.5f,	 0.5f,	/*���� 22-1*/	 0.0f,	-1.0f,	  0.0f,	/*������*/
			-0.5f,	-0.5f,	 0.5f,	/*���� 23-0*/	 0.0f,	-1.0f,	  0.0f,	/*������*/
		};

		unsigned int cubeIndices[] = {
			0,	1,	2,			// ��һ������
			2,	3,	0,			//

			4,	5,	6,			// �ڶ�������
			6,	7,	4,			//

			8,	9,	10,			// ����������
			10,	11,	8,			// 

			12, 13,	14,			// ���ĸ�����
			14, 15, 12,			//

			16, 17, 18,			// ���������
			18, 19, 16,			//

			20, 21, 22,			// ����������
			22, 23, 20,			//
		};

		m_CubeVAO = CreateScope<VertexArray>();

		VertexBufferLayout cubeLayout;
		cubeLayout.Push<float>(3);	// ��������(x,y,z)
		cubeLayout.Push<float>(3);	// ������

		m_CubeVBO = CreateScope<VertexBuffer>(cubePositions, sizeof(cubePositions));
		m_CubeVAO->AddBuffer(*m_CubeVBO, cubeLayout);
		m_CubeIBO = CreateScope<IndexBuffer>(cubeIndices, sizeof(cubeIndices) / sizeof(unsigned int));

		// --------- ��պ� --------------------------------------------------
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
			0,	1,	2,			// ��һ������
			2,	3,	0,			//

			4,	5,	6,			// �ڶ�������
			6,	7,	4,			//

			8,	9,	10,			// ����������
			10,	11,	8,			// 

			12, 13,	14,			// ���ĸ�����
			14, 15, 12,			//

			16, 17, 18,			// ���������
			18, 19, 16,			//

			20, 21, 22,			// ����������
			22, 23, 20,			//
		};

		m_SkyboxVAO = CreateScope<VertexArray>();

		VertexBufferLayout skyboxLayout;
		skyboxLayout.Push<float>(3);	// ��������(x,y,z)

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
		// ŷ���Ƿ� 
		m_Camera = CreateScope<Camera>(
			m_CameraPosition,	// ���λ��
			m_CameraPitch,		// ���������ת���Ƕ�
			m_CameraYaw,		// �������ת���Ƕ�,����ϵ-������ϵ = ת��180
			m_CameraWorldup		// �����������Ϸ�������
			);

		m_Window = glfwGetCurrentContext();
		ASSERT(m_Window);

		InitCallback();

		/**
		*
		* blending ��Ϸ�ʽ
		* ������ͣ��� /doc/blend-*.jpg
		*/
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		/**
		*
		* ������Ȳ��ԣ���Ϊ�����Ϣ���洢��Z�����У�Ĭ���ǹرյģ������������
		* ��Ⱦ��άͼ�λ�����
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

		m_Proj = glm::perspective(glm::radians(m_FOV), m_ScreenWidth / m_ScreenHeight/*Viewport��width/height*/, 0.1f, 100.0f);
		m_View = m_Camera->GetViewMatrix();

		// ����
		m_Model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, -1.0f))
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_ModelRotationAngle), m_ModelRotationDirection)
			* glm::scale(glm::mat4(1.0f), m_ModelScale);
		// ����ģ�͡��۲��ͶӰ����
		// ʹ��2D��������ӽ�����Ⱦ
		//m_CubeTexture->Bind();
		// ʹ��3D�����������Ⱦ
		m_ShaderSkybox->Bind();
		m_Shader->Bind();
		m_Shader->SetUniform1i("textureIndex", 0);
		m_Shader->SetUniformMat4f("u_Proj", m_Proj);
		m_Shader->SetUniformMat4f("u_View", m_View);
		m_Shader->SetUniformMat4f("u_Model", m_Model);
		m_Shader->SetUniform3f("cameraPos", m_Camera->GetPosition());
		renderer.Draw(*m_CubeVAO, *m_CubeIBO, *m_Shader);

		// ��պ�
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		m_ShaderSkybox->Bind();
		m_ShaderSkybox->SetUniform1i("skybox", 0);
		m_SkyboxTexture->Bind();
		// ���ù۲��ͶӰ����
		m_ShaderSkybox->SetUniformMat4f("u_View", glm::mat4(glm::mat3(m_Camera->GetViewMatrix())/*�Ƴ�λ�Ʒ���*/));
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