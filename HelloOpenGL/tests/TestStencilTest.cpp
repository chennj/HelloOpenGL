#include "TestStencilTest.h"

#include "../imgui/imgui.h"

namespace tests
{
	TestStencilTest::TestStencilTest()
	{
		// --------- ���� --------------------------------------------------
		float cubePositions[] = {

			-0.5f,	-0.5f,	 0.5f,	/*���� 0-0*/		0.0f, 0.0f,	/*����*/
			 0.5f,	-0.5f,	 0.5f,	/*���� 1-1*/		1.0f, 0.0f,	/*����*/
			 0.5f,	 0.5f,	 0.5f,	/*���� 2-2*/		1.0f, 1.0f,	/*����*/
			-0.5f,	 0.5f,	 0.5f,	/*���� 3-3*/		0.0f, 1.0f,	/*����*/

			-0.5f,	-0.5f,	-0.5f,	/*���� 4-4*/		0.0f, 0.0f,	/*����*/
			-0.5f,	-0.5f,	 0.5f,	/*���� 5-0*/		1.0f, 0.0f,	/*����*/
			-0.5f,	 0.5f,	 0.5f,	/*���� 6-3*/		1.0f, 1.0f,	/*����*/
			-0.5f,	 0.5f,	-0.5f,	/*���� 7-7*/		0.0f, 1.0f,	/*����*/

			 0.5f,	-0.5f,	-0.5f,	/*���� 8-5*/		0.0f, 0.0f,	/*����*/
			-0.5f,	-0.5f,	-0.5f,	/*���� 9-4*/		1.0f, 0.0f,	/*����*/
			-0.5f,	 0.5f,	-0.5f,	/*���� 10-7*/	1.0f, 1.0f,	/*����*/
			 0.5f,	 0.5f,	-0.5f,	/*���� 11-6*/	0.0f, 1.0f,	/*����*/

			 0.5f,	-0.5f,	 0.5f,	/*���� 12-1*/	0.0f, 0.0f,	/*����*/
			 0.5f,	-0.5f,	-0.5f,	/*���� 13-5*/	1.0f, 0.0f,	/*����*/
			 0.5f,	 0.5f,	-0.5f,	/*���� 14-6*/	1.0f, 1.0f,	/*����*/
			 0.5f,	 0.5f,	 0.5f,	/*���� 15-2*/	0.0f, 1.0f,	/*����*/

			-0.5f,	 0.5f,	 0.5f,	/*���� 16-3*/	0.0f, 0.0f,	/*����*/
			 0.5f,	 0.5f,	 0.5f,	/*���� 17-2*/	1.0f, 0.0f,	/*����*/
			 0.5f,	 0.5f,	-0.5f,	/*���� 18-6*/	1.0f, 1.0f,	/*����*/
			-0.5f,	 0.5f,	-0.5f,	/*���� 19-7*/	0.0f, 1.0f,	/*����*/

			-0.5f,	-0.5f,	-0.5f,	/*���� 20-4*/	0.0f, 0.0f,	/*����*/
			 0.5f,	-0.5f,	-0.5f,	/*���� 21-5*/	1.0f, 0.0f,	/*����*/
			 0.5f,	-0.5f,	 0.5f,	/*���� 22-1*/	1.0f, 1.0f,	/*����*/
			-0.5f,	-0.5f,	 0.5f,	/*���� 23-0*/	0.0f, 1.0f,	/*����*/
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
		cubeLayout.Push<float>(2);	// ��������(s,t)

		m_CubeVBO = CreateScope<VertexBuffer>(cubePositions, sizeof(cubePositions));
		m_CubeVAO->AddBuffer(*m_CubeVBO, cubeLayout);
		m_CubeIBO = CreateScope<IndexBuffer>(cubeIndices, sizeof(cubeIndices) / sizeof(unsigned int));

		// --------- ���� --------------------------------------------------
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
		planeLayout.Push<float>(3);	// ��������(x,y,z)
		planeLayout.Push<float>(2);	// ��������(s,t)

		m_PlaneVBO = CreateScope<VertexBuffer>(planePositions, sizeof(planePositions));
		m_PlaneVAO->AddBuffer(*m_PlaneVBO, planeLayout);
		m_PlaneIBO = CreateScope<IndexBuffer>(planeIndices, sizeof(planeIndices) / sizeof(unsigned int));

		// --------- shader --------------------------------------------------
		m_Shader = CreateRef<Shader>("shaders/Stencil.shader");

		m_CubeTexture = CreateRef<Texture>("../res/texture/container2.png");
		m_FloorTexture = CreateRef<Texture>("../res/texture/cnchess/WHITE.GIF");

		m_CameraPosition = glm::vec3(0.0f, 2.0f, 13.0f);
		m_CameraPitch = glm::radians(0.0f);
		m_CameraYaw = glm::radians(180.0f);
		m_CameraWorldup = glm::vec3(0.0f, 1.0f, 0.0f);
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
		GLCall(glDepthFunc(GL_LESS));

		/**
		*
		* ����ģ�����
		*/
		GLCall(glEnable(GL_STENCIL_TEST));
		GLCall(glStencilFunc(GL_NOTEQUAL, 1, 0xFF));
		GLCall(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));
	}

	TestStencilTest::~TestStencilTest()
	{
		glfwSetWindowUserPointer(m_Window, nullptr);
		glfwSetScrollCallback(m_Window, nullptr);
		glfwSetCursorPosCallback(m_Window, nullptr);
		glfwSetFramebufferSizeCallback(m_Window, nullptr);
	}

	void TestStencilTest::OnUpdate(float deltaTime)
	{
		ProcessInput(deltaTime);
	}

	void TestStencilTest::OnRender()
	{
		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);// don't forget to clear the stencil buffer!

		Renderer renderer;

		m_Proj = glm::perspective(glm::radians(m_FOV), m_ScreenWidth / m_ScreenHeight/*Viewport��width/height*/, 0.1f, 100.0f);
		m_View = m_Camera->GetViewMatrix();

		m_Shader->Bind();
		m_Shader->SetUniform1i("textureIndex", 0);

		m_Shader->SetUniformMat4f("u_View", m_View);
		m_Shader->SetUniformMat4f("u_Proj", m_Proj);

		glStencilMask(0x00); // �ǵñ�֤�����ڻ��Ƶذ��ʱ�򲻻����ģ�建��
		m_Shader->SetUniform1i("drawFrame", 0);
		// --------------------------------------------------------------------
		// ����
		m_FloorTexture->Bind();
		m_Shader->SetUniformMat4f("u_Model", glm::mat4(1.0f));
		renderer.Draw(*m_PlaneVAO, *m_PlaneIBO, *m_Shader);

		// 1st. render pass, draw objects as normal, writing to the stencil buffer
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		m_Shader->SetUniform1i("drawFrame", 0);
		m_ModelScale = glm::vec3(1.0f, 1.0f, 1.0f);
		// --------------------------------------------------------------------
		// ����
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

		// 2nd. render pass: now draw slightly scaled versions of the objects, this time disabling stencil writing.
		// Because the stencil buffer is now filled with several 1s. The parts of the buffer that are 1 are not drawn, thus only drawing 
		// the objects' size differences, making it look like borders.
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);
		m_ModelScale = glm::vec3(1.1f, 1.1f, 1.1f);
		m_Shader->SetUniform1i("drawFrame", 1);
		// -----------------------------------------------------------------------------------------------------------------------------
		// �������
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

		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		glEnable(GL_DEPTH_TEST);
	}

	void TestStencilTest::OnImGuiRender()
	{

	}

	void TestStencilTest::InitCallback()
	{
		glfwSetWindowUserPointer(m_Window, this);

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) {

			TestStencilTest* self = (TestStencilTest*)glfwGetWindowUserPointer(window);

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

			TestStencilTest* self = (TestStencilTest*)glfwGetWindowUserPointer(window);

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

			TestStencilTest* self = (TestStencilTest*)glfwGetWindowUserPointer(window);
			self->m_ScreenWidth = width;
			self->m_ScreenHeight = height;
			glViewport(0, 0, width, height);
		});
	}

	void TestStencilTest::ProcessInput(float deltaTime)
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