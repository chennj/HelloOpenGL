#include "TestBlinnPhong.h"

#include "../core/Renderer.h"
#include "../imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <math.h>

using namespace tests;

TestBlinnPhong::TestBlinnPhong()
{
	// �ذ�
	// ------------------------------------------------------------------
	float floorPositions[] = {

		// positions            // normals         // texcoords
		10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
	   -10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
	   -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

		10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
	   -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
		10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
	};

	unsigned int floorIndices[] = {
		0,	1,	2,			// ��һ������
		3,	4,	5,			//
	};

	m_FloorVAO = CreateScope<VertexArray>();
	m_FloorVBO = CreateScope<VertexBuffer>(floorPositions, sizeof(floorPositions));
	VertexBufferLayout layout;
	layout.Push<float>(3);	// ��������(x,y)
	layout.Push<float>(3);	// ������
	layout.Push<float>(2);	// ��������(s,t)

	m_FloorVAO->AddBuffer(*m_FloorVBO, layout);
	m_FloorIBO = CreateScope<IndexBuffer>(floorIndices, sizeof(floorIndices) / sizeof(unsigned int));
	m_FloorTexture = CreateScope<Texture>("../res/texture/wood-1.jpg");
	// ------------------------------------------------------------------

	// ��ɫ��
	m_FloorShader = CreateScope<Shader>("shaders/AdvancedLighting.vs.glsl", "shaders/AdvancedLighting.fs.glsl");

	// ���
	// ------------------------------------------------------------------
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
	// ------------------------------------------------------------------

	// ����
	// ------------------------------------------------------------------
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
	// ------------------------------------------------------------------

}

TestBlinnPhong::~TestBlinnPhong()
{
	glfwSetWindowUserPointer(m_Window, nullptr);
	glfwSetScrollCallback(m_Window, nullptr);
	glfwSetCursorPosCallback(m_Window, nullptr);
	glfwSetFramebufferSizeCallback(m_Window, nullptr);
}

void TestBlinnPhong::OnUpdate(float deltaTime)
{
	ProcessInput(deltaTime);
}

void TestBlinnPhong::OnRender()
{
	GLCall(glClearColor(0, 0, 0, 1));
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Renderer renderer;

	// �ذ�
	m_Proj = glm::perspective(glm::radians(m_FOV), m_ScreenWidth / m_ScreenHeight, 0.1f, 100.0f);
	m_Model = glm::translate(glm::mat4(1.0f), m_ModelPosition)
		* glm::rotate(glm::mat4(1.0f), glm::radians(m_ModelRotationAngle), m_ModelRotationDirection)
		* glm::scale(glm::mat4(1.0f), m_ModelScale);
	m_View = m_Camera->GetViewMatrix();

	m_FloorShader->Bind();
	m_FloorTexture->Bind();

	// �ƹ�
	m_FloorShader->SetUniform3f("viewPos", m_Camera->GetPosition());
	m_FloorShader->SetUniform3f("lightPos", m_LightPosition);
	m_FloorShader->SetUniform1i("blinn", m_IsBlinn);

	//m_FloorShader->SetUniformMat4f("model", m_Model);
	m_FloorShader->SetUniformMat4f("view", m_View);
	m_FloorShader->SetUniformMat4f("projection", m_Proj);

	renderer.Draw(*m_FloorVAO, *m_FloorIBO, *m_FloorShader);
}

void TestBlinnPhong::OnImGuiRender()
{
	ImGui::Begin("Phong And Blinn Opition");
	ImGui::Checkbox("Is Blinn", &m_IsBlinn);
	ImGui::End();
}

void TestBlinnPhong::InitCallback()
{
	glfwSetWindowUserPointer(m_Window, this);

	glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) {

		TestBlinnPhong* self = (TestBlinnPhong*)glfwGetWindowUserPointer(window);

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

		TestBlinnPhong* self = (TestBlinnPhong*)glfwGetWindowUserPointer(window);

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

		TestBlinnPhong* self = (TestBlinnPhong*)glfwGetWindowUserPointer(window);
		self->m_ScreenWidth = width;
		self->m_ScreenHeight = height;
		glViewport(0, 0, width, height);
	});
}

void TestBlinnPhong::ProcessInput(float deltaTime)
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