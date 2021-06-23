#include "TestExplode.h"

#include "../core/Renderer.h"
#include "../imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <math.h>

using namespace tests;

TestExplode::TestExplode()
{
	// -------------------------- ���� ---------------------------------------
	float cubePositions[] = {

		-0.5f,	-0.5f,	 0.5f,	/*���� 0-0*/		 0.0f, 0.0f,	/*����*/
		 0.5f,	-0.5f,	 0.5f,	/*���� 1-1*/		 1.0f, 0.0f,	/*����*/
		 0.5f,	 0.5f,	 0.5f,	/*���� 2-2*/		 1.0f, 1.0f,	/*����*/
		-0.5f,	 0.5f,	 0.5f,	/*���� 3-3*/		 0.0f, 1.0f,	/*����*/

		-0.5f,	-0.5f,	-0.5f,	/*���� 4-4*/		-0.0f, 0.0f,	/*����*/
		-0.5f,	-0.5f,	 0.5f,	/*���� 5-0*/		-1.0f, 0.0f,	/*����*/
		-0.5f,	 0.5f,	 0.5f,	/*���� 6-3*/		-1.0f, 1.0f,	/*����*/
		-0.5f,	 0.5f,	-0.5f,	/*���� 7-7*/		-0.0f, 1.0f,	/*����*/

		 0.5f,	-0.5f,	-0.5f,	/*���� 8-5*/		 0.0f, 0.0f,	/*����*/
		-0.5f,	-0.5f,	-0.5f,	/*���� 9-4*/		 1.0f, 0.0f,	/*����*/
		-0.5f,	 0.5f,	-0.5f,	/*���� 10-7*/	 1.0f, 1.0f,	/*����*/
		 0.5f,	 0.5f,	-0.5f,	/*���� 11-6*/	 0.0f, 1.0f,	/*����*/

		 0.5f,	-0.5f,	 0.5f,	/*���� 12-1*/	 0.0f, 0.0f,	/*����*/
		 0.5f,	-0.5f,	-0.5f,	/*���� 13-5*/	 1.0f, 0.0f,	/*����*/
		 0.5f,	 0.5f,	-0.5f,	/*���� 14-6*/	 1.0f, 1.0f,	/*����*/
		 0.5f,	 0.5f,	 0.5f,	/*���� 15-2*/	 0.0f, 1.0f,	/*����*/

		-0.5f,	 0.5f,	 0.5f,	/*���� 16-3*/	 0.0f, 0.0f,	/*����*/
		 0.5f,	 0.5f,	 0.5f,	/*���� 17-2*/	 1.0f, 0.0f,	/*����*/
		 0.5f,	 0.5f,	-0.5f,	/*���� 18-6*/	 1.0f, 1.0f,	/*����*/
		-0.5f,	 0.5f,	-0.5f,	/*���� 19-7*/	 0.0f, 1.0f,	/*����*/

		-0.5f,	-0.5f,	-0.5f,	/*���� 20-4*/	 0.0f, 0.0f,	/*����*/
		 0.5f,	-0.5f,	-0.5f,	/*���� 21-5*/	 1.0f, 0.0f,	/*����*/
		 0.5f,	-0.5f,	 0.5f,	/*���� 22-1*/	 1.0f, 1.0f,	/*����*/
		-0.5f,	-0.5f,	 0.5f,	/*���� 23-0*/	 0.0f, 1.0f,	/*����*/
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
	m_CubeVBO = CreateScope<VertexBuffer>(cubePositions, sizeof(cubePositions));
	VertexBufferLayout layout;
	layout.Push<float>(3);	// ��������(x,y)
	layout.Push<float>(2);	// ��������(s,t)

	m_CubeVAO->AddBuffer(*m_CubeVBO, layout);
	m_CubeIBO = CreateScope<IndexBuffer>(cubeIndices, sizeof(cubeIndices) / sizeof(unsigned int));

	m_Shader = CreateScope<Shader>("shaders/Explode.vs.glsl", "shaders/Explode.fs.glsl", "shaders/Explode.gs.glsl");
	
	m_CubeTexture = CreateScope<Texture>("../res/texture/container2.png");

	m_CubePositions = new glm::vec3[10];
	m_CubePositions[0] = glm::vec3(0.0f, 0.0f, 0.0f);
	m_CubePositions[1] = glm::vec3(2.0f, 5.0f, -15.0f);
	m_CubePositions[2] = glm::vec3(-1.5f, -2.2f, -2.5f);
	m_CubePositions[3] = glm::vec3(-3.8f, -2.0f, -12.3f);
	m_CubePositions[4] = glm::vec3(2.4f, -0.4f, -3.5f);
	m_CubePositions[5] = glm::vec3(-1.7f, 3.0f, -7.5f);
	m_CubePositions[6] = glm::vec3(1.3f, -2.0f, -2.5f);
	m_CubePositions[7] = glm::vec3(1.5f, 2.0f, -2.5f);
	m_CubePositions[8] = glm::vec3(1.5f, 0.2f, -1.5f);
	m_CubePositions[9] = glm::vec3(-1.3f, 1.0f, -1.5f);

	m_CameraPosition = glm::vec3(0.0f, 0.0f, 6.0f);
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

}

TestExplode::~TestExplode()
{
	delete[] m_CubePositions;

	glfwSetWindowUserPointer(m_Window, nullptr);
	glfwSetScrollCallback(m_Window, nullptr);
	glfwSetCursorPosCallback(m_Window, nullptr);
	glfwSetFramebufferSizeCallback(m_Window, nullptr);
}

void TestExplode::OnUpdate(float deltaTime)
{
	ProcessInput(deltaTime);
}

void TestExplode::OnRender()
{
	GLCall(glClearColor(0, 0, 0, 1));
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Renderer renderer;
	m_CubeTexture->Bind();

	// ������
	for (unsigned int i = 0; i < 10; i++)
	{
		float angle = m_ModelRotationAngle;
		if (i % 3 == 0)
			angle = glfwGetTime() * 25.0f;

		m_Proj = glm::perspective(glm::radians(m_FOV), m_ScreenWidth / m_ScreenHeight, 0.1f, 100.0f);
		m_Model = glm::translate(glm::mat4(1.0f), m_CubePositions[i])
			* glm::rotate(glm::mat4(1.0f), glm::radians(angle), m_ModelRotationDirection)
			* glm::scale(glm::mat4(1.0f), m_ModelScale);
		m_View = m_Camera->GetViewMatrix();

		m_Shader->Bind();
		m_CubeTexture->Bind();

		m_Shader->SetUniform1f("time", glfwGetTime());
		m_Shader->SetUniform1i("texture_diffuse1", 0);
		m_Shader->SetUniformMat4f("model",		m_Model);
		m_Shader->SetUniformMat4f("view",		m_View);
		m_Shader->SetUniformMat4f("projection",	m_Proj);

		renderer.Draw(*m_CubeVAO, *m_CubeIBO, *m_Shader);
		break;
	}
}

void TestExplode::OnImGuiRender()
{

}

void TestExplode::InitCallback()
{
	glfwSetWindowUserPointer(m_Window, this);

	glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) {

		TestExplode* self = (TestExplode*)glfwGetWindowUserPointer(window);

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

		TestExplode* self = (TestExplode*)glfwGetWindowUserPointer(window);

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

		TestExplode* self = (TestExplode*)glfwGetWindowUserPointer(window);
		self->m_ScreenWidth = width;
		self->m_ScreenHeight = height;
		glViewport(0, 0, width, height);
	});
}

void TestExplode::ProcessInput(float deltaTime)
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