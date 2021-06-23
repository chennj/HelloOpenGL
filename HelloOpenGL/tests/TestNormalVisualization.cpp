#include "TestNormalVisualization.h"

#include "../core/Renderer.h"
#include "../imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <math.h>

using namespace tests;

TestNormalVisualization::TestNormalVisualization()
{
	// -------------------------- 箱子 ---------------------------------------
	float cubePositions[] = {

		-0.5f,	-0.5f,	 0.5f,	/*顶点 0-0*/		 0.0f, 0.0f,	/*坐标*/
		 0.5f,	-0.5f,	 0.5f,	/*顶点 1-1*/		 1.0f, 0.0f,	/*坐标*/
		 0.5f,	 0.5f,	 0.5f,	/*顶点 2-2*/		 1.0f, 1.0f,	/*坐标*/
		-0.5f,	 0.5f,	 0.5f,	/*顶点 3-3*/		 0.0f, 1.0f,	/*坐标*/

		-0.5f,	-0.5f,	-0.5f,	/*顶点 4-4*/		-0.0f, 0.0f,	/*坐标*/
		-0.5f,	-0.5f,	 0.5f,	/*顶点 5-0*/		-1.0f, 0.0f,	/*坐标*/
		-0.5f,	 0.5f,	 0.5f,	/*顶点 6-3*/		-1.0f, 1.0f,	/*坐标*/
		-0.5f,	 0.5f,	-0.5f,	/*顶点 7-7*/		-0.0f, 1.0f,	/*坐标*/

		 0.5f,	-0.5f,	-0.5f,	/*顶点 8-5*/		 0.0f, 0.0f,	/*坐标*/
		-0.5f,	-0.5f,	-0.5f,	/*顶点 9-4*/		 1.0f, 0.0f,	/*坐标*/
		-0.5f,	 0.5f,	-0.5f,	/*顶点 10-7*/	 1.0f, 1.0f,	/*坐标*/
		 0.5f,	 0.5f,	-0.5f,	/*顶点 11-6*/	 0.0f, 1.0f,	/*坐标*/

		 0.5f,	-0.5f,	 0.5f,	/*顶点 12-1*/	 0.0f, 0.0f,	/*坐标*/
		 0.5f,	-0.5f,	-0.5f,	/*顶点 13-5*/	 1.0f, 0.0f,	/*坐标*/
		 0.5f,	 0.5f,	-0.5f,	/*顶点 14-6*/	 1.0f, 1.0f,	/*坐标*/
		 0.5f,	 0.5f,	 0.5f,	/*顶点 15-2*/	 0.0f, 1.0f,	/*坐标*/

		-0.5f,	 0.5f,	 0.5f,	/*顶点 16-3*/	 0.0f, 0.0f,	/*坐标*/
		 0.5f,	 0.5f,	 0.5f,	/*顶点 17-2*/	 1.0f, 0.0f,	/*坐标*/
		 0.5f,	 0.5f,	-0.5f,	/*顶点 18-6*/	 1.0f, 1.0f,	/*坐标*/
		-0.5f,	 0.5f,	-0.5f,	/*顶点 19-7*/	 0.0f, 1.0f,	/*坐标*/

		-0.5f,	-0.5f,	-0.5f,	/*顶点 20-4*/	 0.0f, 0.0f,	/*坐标*/
		 0.5f,	-0.5f,	-0.5f,	/*顶点 21-5*/	 1.0f, 0.0f,	/*坐标*/
		 0.5f,	-0.5f,	 0.5f,	/*顶点 22-1*/	 1.0f, 1.0f,	/*坐标*/
		-0.5f,	-0.5f,	 0.5f,	/*顶点 23-0*/	 0.0f, 1.0f,	/*坐标*/
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
	m_CubeVBO = CreateScope<VertexBuffer>(cubePositions, sizeof(cubePositions));
	VertexBufferLayout layoutCube;
	layoutCube.Push<float>(3);	// 顶点坐标(x,y,z)
	layoutCube.Push<float>(2);	// 纹理坐标(s,t)

	m_CubeVAO->AddBuffer(*m_CubeVBO, layoutCube);
	m_CubeIBO = CreateScope<IndexBuffer>(cubeIndices, sizeof(cubeIndices) / sizeof(unsigned int));

	// -------------------------- 箱子法向量 ---------------------------------------
	float cubeNormalPositions[] = {

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

	unsigned int cubeNormalIndices[] = {
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

	m_CubeNormalVAO = CreateScope<VertexArray>();
	m_CubeNormalVBO = CreateScope<VertexBuffer>(cubeNormalPositions, sizeof(cubeNormalPositions));
	VertexBufferLayout layoutNormal;
	layoutNormal.Push<float>(3);	// 顶点坐标(x,y,z)
	layoutNormal.Push<float>(3);	// 顶点法向量(x,y,z)

	m_CubeNormalVAO->AddBuffer(*m_CubeNormalVBO, layoutNormal);
	m_CubeNormalIBO = CreateScope<IndexBuffer>(cubeNormalIndices, sizeof(cubeNormalIndices) / sizeof(unsigned int));

	// -------------------------- shader -------------------------------------------------------------------------------
	m_ShaderNormal	= CreateScope<Shader>("shaders/NormalVisualization.vs.glsl", "shaders/NormalVisualization.fs.glsl", "shaders/NormalVisualization.gs.glsl");
	m_ShaderDefault = CreateScope<Shader>("shaders/Default.vs.glsl", "shaders/Default.fs.glsl");
	
	m_CubeTexture	= CreateScope<Texture>("../res/texture/container2.png");

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

TestNormalVisualization::~TestNormalVisualization()
{
	delete[] m_CubePositions;

	glfwSetWindowUserPointer(m_Window, nullptr);
	glfwSetScrollCallback(m_Window, nullptr);
	glfwSetCursorPosCallback(m_Window, nullptr);
	glfwSetFramebufferSizeCallback(m_Window, nullptr);
}

void TestNormalVisualization::OnUpdate(float deltaTime)
{
	ProcessInput(deltaTime);
}

void TestNormalVisualization::OnRender()
{
	GLCall(glClearColor(0, 0, 0, 1));
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Renderer renderer;
	m_CubeTexture->Bind();

	// 立方体
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

		// 法向量
		m_ShaderNormal->Bind();
		m_ShaderNormal->SetUniformMat4f("model",		m_Model);
		m_ShaderNormal->SetUniformMat4f("view",			m_View);
		m_ShaderNormal->SetUniformMat4f("projection",	m_Proj);
		renderer.Draw(*m_CubeNormalVAO, *m_CubeNormalIBO, *m_ShaderNormal);

		// 箱子
		m_ShaderDefault->Bind();
		m_CubeTexture->Bind();
		m_ShaderDefault->SetUniform1i("texture_diffuse1", 0);
		m_ShaderDefault->SetUniformMat4f("model",		m_Model);
		m_ShaderDefault->SetUniformMat4f("view",		m_View);
		m_ShaderDefault->SetUniformMat4f("projection",	m_Proj);
		renderer.Draw(*m_CubeVAO, *m_CubeIBO, *m_ShaderDefault);

		break;
	}
}

void TestNormalVisualization::OnImGuiRender()
{

}

void TestNormalVisualization::InitCallback()
{
	glfwSetWindowUserPointer(m_Window, this);

	glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) {

		TestNormalVisualization* self = (TestNormalVisualization*)glfwGetWindowUserPointer(window);

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

		TestNormalVisualization* self = (TestNormalVisualization*)glfwGetWindowUserPointer(window);

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

		TestNormalVisualization* self = (TestNormalVisualization*)glfwGetWindowUserPointer(window);
		self->m_ScreenWidth = width;
		self->m_ScreenHeight = height;
		glViewport(0, 0, width, height);
	});
}

void TestNormalVisualization::ProcessInput(float deltaTime)
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