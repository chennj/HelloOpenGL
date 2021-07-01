#include "TestShadowMapping.h"

#include "../core/Renderer.h"
#include "../imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <math.h>

using namespace tests;

const unsigned int SHADOW_WIDTH = 1400, SHADOW_HEIGHT = 1400;

TestShadowMapping::TestShadowMapping()
{
	// ������ԣ����㣬���࣬��������ȣ�����VAO, VBO, IBO
	// ------------------------------------------------------------------

	// 1. �ذ�
	float floorPositions[] = {
		// positions            // normals         // texcoords
         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
         25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
	};

	unsigned int floorIndices[] = {
		0,1,2,
		3,4,5,
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

	// 2. ����
	// �����Ż���
	float cubeVertices[] = {
		// back face
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
		 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
		// front face
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
		 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
		 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
		-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		// left face
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
		-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
		// right face
		 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
		 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
		 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
		 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
		 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
		 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
		// bottom face
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
		 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
		 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
		 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
		-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
		// top face
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
		 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
		 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
		 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
		-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
	};

	unsigned int cubeIndices[] = {
		0,	1,	2,			// ��һ������
		3,	4,	5,			//

		6,	7,	8,			// �ڶ�������
		9,	10,	11,			//

		12,	13,	14,			// ����������
		15,	16,	17,			// 

		18, 19,	20,			// ���ĸ�����
		21, 22, 23,			//

		24, 25, 26,			// ���������
		27, 28, 29,			//

		30, 31, 32,			// ����������
		33, 34, 35,			//
	};
	m_CubeVAO = CreateScope<VertexArray>();
	m_CubeVBO = CreateScope<VertexBuffer>(cubeVertices, sizeof(cubeVertices));
	VertexBufferLayout cubeLayout;
	cubeLayout.Push<float>(3);	// ��������(x,y)
	cubeLayout.Push<float>(3);	// ������
	cubeLayout.Push<float>(2);	// ��������(s,t)

	m_CubeVAO->AddBuffer(*m_CubeVBO, cubeLayout);
	m_CubeIBO = CreateScope<IndexBuffer>(cubeIndices, sizeof(cubeIndices) / sizeof(unsigned int));

	// 3. ������Ӱ
	float quadVertices[] = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};
	unsigned int quadIndices[] = {
		0, 1, 2, 3
	};

	m_QuadVAO = CreateScope<VertexArray>();
	m_QuadVBO = CreateScope<VertexBuffer>(quadVertices, sizeof(quadVertices));
	VertexBufferLayout quadLayout;
	quadLayout.Push<float>(3);	// ��������(x,y)
	quadLayout.Push<float>(2);	// ��������(s,t)

	m_QuadVAO->AddBuffer(*m_QuadVBO, quadLayout);
	m_QuadIBO = CreateScope<IndexBuffer>(quadIndices, sizeof(quadIndices) / sizeof(unsigned int));

	// ������Ӱ����
	glGenTextures(1, &m_DepthMap);
	glBindTexture(GL_TEXTURE_2D, m_DepthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// ------------------------------------------------------------------

	// FBO��֡����
	// ------------------------------------------------------------------
	glGenFramebuffers(1, &m_DepthMapFBO);
	// �������������ΪFBO����Ȼ�����
	glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthMap, 0);
	// ������Ҫ��ֻ���ڴӹ��͸��ͼ����Ⱦ������ʱ�������Ϣ��������ɫ����û���á�Ȼ������������ɫ�����֡��������ǲ������ģ�
	// ����������Ҫ��ʽ����OpenGL���ǲ������κ���ɫ���ݽ�����Ⱦ��
	// ����ͨ��������glDrawBuffer��glReadBuffer�Ѷ��ͻ��ƻ�������ΪGL_NONE��������¡�
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	// ȡ����
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// ------------------------------------------------------------------

	// ��ɫ��
	// ------------------------------------------------------------------
	// ������ɫ��
	m_Shader = CreateScope<Shader>("shaders/ShadowMapping.vs.glsl", "shaders/ShadowMapping.fs.glsl");
	// �����ɫ��
	m_SimpleDepthShader = CreateScope<Shader>("shaders/ShadowMapping_Depth.vs.glsl", "shaders/ShadowMapping_Depth.fs.glsl");
	// ������ɫ��
	m_DebugDepthQuad	= CreateScope<Shader>("shaders/ShadowMapping_Debug_Quad.vs.glsl", "shaders/ShadowMapping_Debug_Quad.fs.glsl");
	// ------------------------------------------------------------------

	// ƽ�й�Դ
	// -------------
	m_LightPos = { -2.0f, 4.0f, -1.0f };

	// ���
	// ------------------------------------------------------------------
	m_CameraPosition = glm::vec3(0.0f, 2.0f, 10.0f);
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

TestShadowMapping::~TestShadowMapping()
{
	glfwSetWindowUserPointer(m_Window, nullptr);
	glfwSetScrollCallback(m_Window, nullptr);
	glfwSetCursorPosCallback(m_Window, nullptr);
	glfwSetFramebufferSizeCallback(m_Window, nullptr);
}

void TestShadowMapping::OnUpdate(float deltaTime)
{
	ProcessInput(deltaTime);
}

void TestShadowMapping::OnRender()
{
	GLCall(glClearColor(0, 0, 0, 1));
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Renderer renderer;

	// 1. render depth of scene to texture (from light's perspective)
	// --------------------------------------------------------------
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = 1.0f, far_plane = 7.5f;
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	lightView = glm::lookAt(m_LightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;
	// render scene from light's point of view
	m_SimpleDepthShader->Bind();
	m_SimpleDepthShader->SetUniformMat4("lightSpaceMatrix", lightSpaceMatrix);

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFBO);
	{
		glClear(GL_DEPTH_BUFFER_BIT);
		m_FloorTexture->Bind();
		RenderScene(m_SimpleDepthShader);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// reset viewport
	glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 2. render scene as normal using the generated depth/shadow map  
	// --------------------------------------------------------------
	m_Shader->Bind();
	glm::mat4 projection = glm::perspective(glm::radians(m_FOV), (float)m_ScreenWidth / (float)m_ScreenHeight, 0.1f, 100.0f);
	glm::mat4 view = m_Camera->GetViewMatrix();
	m_Shader->SetUniformMat4("projection", projection);
	m_Shader->SetUniformMat4("view", view);
	// set light uniforms
	m_Shader->SetUniform1i("diffuseTexture", 0);
	m_Shader->SetUniform1i("shadowMap", 1);
	m_Shader->SetUniform3f("viewPos", m_Camera->GetPosition());
	m_Shader->SetUniform3f("lightPos", m_LightPos);
	m_Shader->SetUniformMat4("lightSpaceMatrix", lightSpaceMatrix);
	m_FloorTexture->Bind();
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_DepthMap);
	RenderScene(m_Shader);

	// 3. render Depth map to quad for visual debugging
	// ---------------------------------------------
	m_DebugDepthQuad->Bind();
	m_DebugDepthQuad->SetUniform1i("depthMap", 0);
	m_DebugDepthQuad->SetUniform1f("near_plane", near_plane);
	m_DebugDepthQuad->SetUniform1f("far_plane", far_plane);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_DepthMap);
	// ���ڵ�����Ӱ
	//renderer.Draw(*m_QuadVAO, *m_QuadIBO, *m_DebugDepthQuad);

}

void TestShadowMapping::OnImGuiRender()
{
	//ImGui::Begin("Phong And Blinn Opition");
	//ImGui::Checkbox("Is Blinn", &m_IsBlinn);
	//ImGui::End();
}

void TestShadowMapping::InitCallback()
{
	glfwSetWindowUserPointer(m_Window, this);

	glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) {

		TestShadowMapping* self = (TestShadowMapping*)glfwGetWindowUserPointer(window);

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

		TestShadowMapping* self = (TestShadowMapping*)glfwGetWindowUserPointer(window);

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

		TestShadowMapping* self = (TestShadowMapping*)glfwGetWindowUserPointer(window);
		self->m_ScreenWidth = width;
		self->m_ScreenHeight = height;
		glViewport(0, 0, width, height);
	});
}

void TestShadowMapping::ProcessInput(float deltaTime)
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

void tests::TestShadowMapping::RenderScene(Ref<Shader>& shader)
{
	Renderer renderer;

	// floor
	glm::mat4 model = glm::mat4(1.0f);
	shader->SetUniformMat4f("model", model);
	renderer.Draw(*m_FloorVAO, *m_FloorIBO, *shader);
	// cubes
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader->SetUniformMat4f("model", model);
	renderer.Draw(*m_CubeVAO, *m_CubeIBO, *shader);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader->SetUniformMat4f("model", model);
	renderer.Draw(*m_CubeVAO, *m_CubeIBO, *shader);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::scale(model, glm::vec3(0.25));
	shader->SetUniformMat4f("model", model);
	renderer.Draw(*m_CubeVAO, *m_CubeIBO, *shader);
}
