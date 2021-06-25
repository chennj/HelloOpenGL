#include "TestFrameBuffer.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"

namespace tests
{
	TestFrameBuffer::TestFrameBuffer()
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

			// ��ʾ���޳�ʱ���ײ��͵����غϣ��и��ţ�����y�������һ�㣬����-0.49
			-0.5f,	-0.49f,	-0.5f,	/*���� 20-4*/	0.0f, 0.0f,	/*����*/
			 0.5f,	-0.49f,	-0.5f,	/*���� 21-5*/	1.0f, 0.0f,	/*����*/
			 0.5f,	-0.49f,	 0.5f,	/*���� 22-1*/	1.0f, 1.0f,	/*����*/
			-0.5f,	-0.49f,	 0.5f,	/*���� 23-0*/	0.0f, 1.0f,	/*����*/
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

		// --------- �� --------------------------------------------------
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
		grassLayout.Push<float>(3);	// ��������(x,y,z)
		grassLayout.Push<float>(2);	// ��������(s,t)

		m_GrassVBO = CreateScope<VertexBuffer>(grassPositions, sizeof(grassPositions));
		m_GrassVAO->AddBuffer(*m_GrassVBO, grassLayout);
		m_GrassIBO = CreateScope<IndexBuffer>(grassIndices, sizeof(grassIndices) / sizeof(unsigned int));

		m_Vegetation.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
		m_Vegetation.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
		m_Vegetation.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
		m_Vegetation.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
		m_Vegetation.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

		// --------- ֡���� --------------------------------------------------
		float framePositions[] = {
			// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
			-1.0f, -1.0f,  0.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  0.0f,  1.0f, 1.0f,
			-1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		};

		unsigned int frameIndices[] = {
			0,1,2,
			2,3,0,
		};

		m_FrameVAO = CreateScope<VertexArray>();

		VertexBufferLayout frameLayout;
		frameLayout.Push<float>(3);	// ��������(x,y,z)
		frameLayout.Push<float>(2);	// ��������(s,t)

		m_FrameVBO = CreateScope<VertexBuffer>(framePositions, sizeof(framePositions));
		m_FrameVAO->AddBuffer(*m_FrameVBO, frameLayout);
		m_FrameIBO = CreateScope<IndexBuffer>(frameIndices, sizeof(frameIndices) / sizeof(unsigned int));

		//FrameBufferSpecification spec = { m_ScreenWidth / 2,m_ScreenHeight / 2 , 1, false };
		FrameBufferSpecification spec = { m_ScreenWidth,m_ScreenHeight , 1, false };
		m_FBO = CreateScope<FrameBuffer>(spec);

		// --------- shader --------------------------------------------------
		m_Shader = CreateScope<Shader>("shaders/FrameBuffer.shader");
		m_ShaderFrame = CreateScope<Shader>("shaders/FrameBuffer_Screen.shader");

		// --------- texture --------------------------------------------------
		m_CubeTexture	= CreateScope<Texture>("../res/texture/container2.png");
		m_FloorTexture	= CreateScope<Texture>("../res/texture/white-stone-floor-texture.jpg");
		m_GrassTexture	= CreateScope<Texture>("../res/texture/grass.png");
		m_WindowTexture = CreateScope<Texture>("../res/texture/blending_transparent_window.png");

		// --------- camera --------------------------------------------------
		m_CameraPosition	= glm::vec3(0.0f, 2.0f, 10.0f);
		m_CameraPitch		= glm::radians(0.0f);
		m_CameraYaw			= glm::radians(180.0f);
		m_CameraWorldup		= glm::vec3(0.0f, 1.0f, 0.0f);
		// ŷ���Ƿ� 
		m_Camera = CreateScope<Camera>(
			m_CameraPosition,	// ���λ��
			m_CameraPitch,		// ���������ת���Ƕ�
			m_CameraYaw,		// �������ת���Ƕ�,����ϵ-������ϵ = ת��180
			m_CameraWorldup		// �����������Ϸ�������
			);

		// --------- other --------------------------------------------------
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

		/**
		*
		* ���޳�
		*/
		GLCall(glEnable(GL_CULL_FACE));
	}

	TestFrameBuffer::~TestFrameBuffer()
	{
		glfwSetWindowUserPointer(m_Window, nullptr);
		glfwSetScrollCallback(m_Window, nullptr);
		glfwSetCursorPosCallback(m_Window, nullptr);
		glfwSetFramebufferSizeCallback(m_Window, nullptr);
	}

	void TestFrameBuffer::DrawScene()
	{
		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Renderer renderer;

		m_Proj = glm::perspective(glm::radians(m_FOV), m_ScreenWidth / m_ScreenHeight/*Viewport��width/height*/, 0.1f, 100.0f);
		m_View = m_Camera->GetViewMatrix();

		m_Shader->Bind();
		m_Shader->SetUniform1i("textureIndex", 0);

		m_Shader->SetUniformMat4f("u_View", m_View);
		m_Shader->SetUniformMat4f("u_Proj", m_Proj);

		// ����
		// ���Ƶ���ʱ�������޳�
		GLCall(glDisable(GL_CULL_FACE));
		m_FloorTexture->Bind();
		m_Shader->SetUniformMat4f("u_Model", glm::mat4(1.0f));
		renderer.Draw(*m_PlaneVAO, *m_PlaneIBO, *m_Shader);
		GLCall(glEnable(GL_CULL_FACE));

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

		// ��
		// ���Ʋ�ʱ�������޳�
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

	void TestFrameBuffer::OnUpdate(float deltaTime)
	{
		ProcessInput(deltaTime);
	}

	void TestFrameBuffer::OnRender()
	{
		// ��һ�����׶Σ����������Ƶ������Լ�֡�����ϣ�����������������Ⱦ����֡�����һ��������
		m_FBO->Bind();
		glEnable(GL_DEPTH_TEST);
		DrawScene();

		// �ڶ������׶Σ���֡�������¶�λ����Ļ��
		// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
		// clear all relevant buffers
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
		glClear(GL_COLOR_BUFFER_BIT);

		// ���������׶Σ���֡�����������Ļ��
		Renderer renderer;
		m_ShaderFrame->Bind();
		glBindTexture(GL_TEXTURE_2D, m_FBO->GetColorAttachmentRendererID());	// use the color attachment texture as the texture of the quad plane
		renderer.Draw(*m_FrameVAO, *m_FrameIBO, *m_ShaderFrame);
	}

	void TestFrameBuffer::OnImGuiRender()
	{
		//ImGui::Checkbox("Front or Back", &m_FrontOrBack);
	}

	void TestFrameBuffer::InitCallback()
	{
		glfwSetWindowUserPointer(m_Window, this);

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) {

			TestFrameBuffer* self = (TestFrameBuffer*)glfwGetWindowUserPointer(window);

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

			TestFrameBuffer* self = (TestFrameBuffer*)glfwGetWindowUserPointer(window);

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

			TestFrameBuffer* self = (TestFrameBuffer*)glfwGetWindowUserPointer(window);
			self->m_ScreenWidth = width;
			self->m_ScreenHeight = height;
			glViewport(0, 0, width, height);
		});
	}

	void TestFrameBuffer::ProcessInput(float deltaTime)
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