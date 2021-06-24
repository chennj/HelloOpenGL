#include "TestInstancing.h"

#include "../core/Renderer.h"
#include "../imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <math.h>

using namespace tests;

TestInstancing::TestInstancing()
{
	/**
	* shader
	* 行星与星带
	*/
	m_ShaderPlanet		= CreateScope<Shader>("shaders/Planet.vs.glsl", "shaders/Planet.fs.glsl");
	m_ShaderAsteriods	= CreateScope<Shader>("shaders/Asteroids.vs.glsl", "shaders/Asteroids.fs.glsl");
	// ------------------------------------------------------------------

	/**
	* model mesh
	*/
	m_ModelPlanet		= CreateScope<Model>("../res/model/planet/planet.obj");
	m_ModelAsteriods	= CreateScope<Model>("../res/model/rock/rock.obj");
	// ------------------------------------------------------------------


	/**
	* generate a large list of semi-random model transformation matrices
	*/
	m_ModelMatrices = new glm::mat4[m_AsteriodAmount];
	srand(glfwGetTime()); // initialize random seed	
	float radius = 150.0;
	float offset = 25.0f;
	for (unsigned int i = 0; i < m_AsteriodAmount; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		// 1. translation: displace along circle with 'radius' in range [-offset, offset]
		float angle = (float)i / (float)m_AsteriodAmount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.3f; // keep height of asteroid field smaller compared to width of x and z
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		// 2. scale: Scale between 0.05 and 0.25f
		float scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));

		// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. now add to list of matrices
		m_ModelMatrices[i] = model;
	}
	// ------------------------------------------------------------------

	/**
	* configure instanced array
	*/
	// -------------------------
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, m_AsteriodAmount * sizeof(glm::mat4), &m_ModelMatrices[0], GL_STATIC_DRAW);

	// set transformation matrices as an instance vertex attribute (with divisor 1)
	// note: we're cheating a little by taking the, now publicly declared, VAO of the model's mesh(es) and adding new vertexAttribPointers
	// normally you'd want to do this in a more organized fashion, but for learning purposes this will do.
	// -----------------------------------------------------------------------------------------------------------------------------------
	for (unsigned int i = 0; i < m_ModelAsteriods->m_Meshes.size(); i++)
	{
		unsigned int VAO = m_ModelAsteriods->m_Meshes[i].VAO;
		glBindVertexArray(VAO);
		// set attribute pointers for matrix (4 times vec4)
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(2, 1);
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);

		glBindVertexArray(0);
	}
	// ------------------------------------------------------------------

	/**
	* 相机
	*/
	m_CameraPosition = glm::vec3(0.0f, 26.0f, 250.0f);
	m_CameraPitch = glm::radians(-5.0f);
	m_CameraYaw = glm::radians(180.0f);
	m_CameraWorldup = glm::vec3(0.0f, 1.0f, 0.0f);
	// 欧拉角法 
	m_Camera = CreateScope<Camera>(
		m_CameraPosition,	// 相机位置
		m_CameraPitch,		// 相机向上下转动角度
		m_CameraYaw,		// 相机左右转动角度,右手系-》左手系 = 转动180
		m_CameraWorldup		// 世界坐标向上方向向量
		);
	// ---------------------------------------------------

	/**
	* 其他
	*/
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

TestInstancing::~TestInstancing()
{
	delete[] m_ModelMatrices;

	glfwSetWindowUserPointer(m_Window, nullptr);
	glfwSetScrollCallback(m_Window, nullptr);
	glfwSetCursorPosCallback(m_Window, nullptr);
	glfwSetFramebufferSizeCallback(m_Window, nullptr);
}

void TestInstancing::OnUpdate(float deltaTime)
{
	ProcessInput(deltaTime);
}

void TestInstancing::OnRender()
{
	GLCall(glClearColor(0, 0, 0, 1));
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_Proj = glm::perspective(glm::radians(m_FOV), m_ScreenWidth / m_ScreenHeight, 0.1f, 1000.0f);
	m_View = m_Camera->GetViewMatrix();
	m_Model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -3.0f, 0.0f))
		* glm::rotate(glm::mat4(1.0f), glm::radians(m_ModelRotationAngle), m_ModelRotationDirection)
		* glm::scale(glm::mat4(1.0f), m_ModelScale);

	// draw planet
	m_ShaderPlanet->Bind();
	m_ShaderPlanet->SetUniformMat4f("model", m_Model);
	m_ShaderPlanet->SetUniformMat4f("view", m_View);
	m_ShaderPlanet->SetUniformMat4f("projection", m_Proj);
	m_ModelPlanet->Draw1(m_ShaderPlanet);

	// draw asteriods
	m_ShaderAsteriods->Bind();
	m_ShaderAsteriods->SetUniformMat4f("view", m_View);
	m_ShaderAsteriods->SetUniformMat4f("projection", m_Proj);
	m_ShaderAsteriods->SetUniform1i("texture_diffuse1", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_ModelAsteriods->m_TexturesLoaded[0].Id); // note: we also made the textures_loaded vector public (instead of private) from the model class.
	for (unsigned int i = 0; i < m_ModelAsteriods->m_Meshes.size(); i++)
	{
		glBindVertexArray(m_ModelAsteriods->m_Meshes[i].VAO);
		glDrawElementsInstanced(GL_TRIANGLES, m_ModelAsteriods->m_Meshes[i].m_Indices.size(), GL_UNSIGNED_INT, 0, m_AsteriodAmount);
		glBindVertexArray(0);
	}
}

void TestInstancing::OnImGuiRender()
{

}

void TestInstancing::InitCallback()
{
	glfwSetWindowUserPointer(m_Window, this);

	glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) {

		TestInstancing* self = (TestInstancing*)glfwGetWindowUserPointer(window);

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

		TestInstancing* self = (TestInstancing*)glfwGetWindowUserPointer(window);

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

		TestInstancing* self = (TestInstancing*)glfwGetWindowUserPointer(window);
		self->m_ScreenWidth = width;
		self->m_ScreenHeight = height;
		glViewport(0, 0, width, height);
	});
}

void TestInstancing::ProcessInput(float deltaTime)
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