#include "TestMesh.h"
#include <fstream>

namespace tests
{
	const float SCR_WIDTH = 1200;
	const float SCR_HEIGHT = 600;

	TestMesh::TestMesh()
	{
		m_Shader = CreateRef<Shader>("shaders/Mesh.shader");

		m_AssimpModel = CreateScope<Model>("../res/model/nanosuit/nanosuit.obj");

		m_Material = CreateScope<Material>(m_Shader, "material");

		m_LightDirectional = CreateScope<LightDirectional>(
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(180.0f, 45.0f, 45.0f),
			glm::vec3(1.5f, 1.5f, 1.5f)
			);
		m_LightPoint0 = CreateScope<LightPoint>(
			glm::vec3(4.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			1.0f, 0.1f, 0.032f
			);
		m_LightPoint1 = CreateScope<LightPoint>(
			glm::vec3(0.0f, 4.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			1.0f, 0.1f, 0.032f
			);
		m_LightPoint2 = CreateScope<LightPoint>(
			glm::vec3(0.0f, 0.0f, 4.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			1.0f, 0.1f, 0.032f
			);
		m_LightPoint3 = CreateScope<LightPoint>(
			glm::vec3(40.0f, 40.0f, 4.0f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			1.0f, 0.1f, 0.032f
			);

		m_LightSpot = CreateScope<LightSpot>(
			glm::vec3(0.0f, 0.0f, 2.0f),
			glm::vec3(180.0f, 0.0f, 0.0f),
			glm::vec3(2.0f, 2.0f, 2.0f),
			0.99f, 0.97f,
			1.0f, 0.1f, 0.032f
			);

		// 欧拉角法 
		m_Camera = CreateScope<Camera>(
			glm::vec3(0.0f, 8.0f, 20.0f),	// 相机位置
			glm::radians(0.0f),				// 相机向上下转动角度
			glm::radians(180.0f),			// 相机左右转动角度,右手系-》左手系 = 转动180
			glm::vec3(0.0f, 1.0f, 0.0f)		// 世界坐标向上方向向量
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

	TestMesh::~TestMesh()
	{
		glfwSetWindowUserPointer(m_Window, nullptr);
		glfwSetScrollCallback(m_Window, nullptr);
		glfwSetCursorPosCallback(m_Window, nullptr);
		glfwSetFramebufferSizeCallback(m_Window, nullptr);
	}

	void TestMesh::OnUpdate(float deltaTime)
	{
		ProcessInput(deltaTime);
	}

	void TestMesh::OnRender()
	{
		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float angle = m_ModelRotationAngle;
		m_Proj = glm::perspective(glm::radians(m_FOV), SCR_WIDTH / SCR_HEIGHT/*Viewport的width/height*/, 0.1f, 100.0f);
		m_Model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))
			* glm::rotate(glm::mat4(1.0f), glm::radians(angle), m_ModelRotationDirection)
			* glm::scale(glm::mat4(1.0f), m_ModelScale);
		m_View = m_Camera->GetViewMatrix();

		m_Shader->Bind();

		m_Shader->SetUniformMat4f("u_Model", m_Model);
		m_Shader->SetUniformMat4f("u_View", m_View);
		m_Shader->SetUniformMat4f("u_Proj", m_Proj);

		m_Material->SetShininess(64.0f);

		m_Shader->SetUniform3f("cameraPosition", m_Camera->GetPosition());

#if 1		// 聚光灯
		m_Shader->SetUniform3f("lightS.position", m_LightSpot->m_Position);
		m_Shader->SetUniform3f("lightS.directionToLight", m_LightSpot->m_Direction);
		m_Shader->SetUniform3f("lightS.color", m_LightSpot->m_Color);
		m_Shader->SetUniform1f("lightS.cutoff", m_LightSpot->m_Cutoff);
		m_Shader->SetUniform1f("lightS.cutoffouter", m_LightSpot->m_CutoffOuter);
		m_Shader->SetUniform1f("lightS.constant", m_LightSpot->m_Attenuation.Constant);
		m_Shader->SetUniform1f("lightS.linear", m_LightSpot->m_Attenuation.Linear);
		m_Shader->SetUniform1f("lightS.quadratic", m_LightSpot->m_Attenuation.Quadratic);

		// 点光源
		m_Shader->SetUniform3f("lightP0.position", m_LightPoint0->m_Position);
		m_Shader->SetUniform3f("lightP0.color", m_LightPoint0->m_Color);
		m_Shader->SetUniform1f("lightP0.constant", m_LightPoint0->m_Attenuation.Constant);
		m_Shader->SetUniform1f("lightP0.linear", m_LightPoint0->m_Attenuation.Linear);
		m_Shader->SetUniform1f("lightP0.quadratic", m_LightPoint0->m_Attenuation.Quadratic);

		m_Shader->SetUniform3f("lightP1.position", m_LightPoint1->m_Position);
		m_Shader->SetUniform3f("lightP1.color", m_LightPoint1->m_Color);
		m_Shader->SetUniform1f("lightP1.constant", m_LightPoint1->m_Attenuation.Constant);
		m_Shader->SetUniform1f("lightP1.linear", m_LightPoint1->m_Attenuation.Linear);
		m_Shader->SetUniform1f("lightP1.quadratic", m_LightPoint1->m_Attenuation.Quadratic);

		m_Shader->SetUniform3f("lightP2.position", m_LightPoint2->m_Position);
		m_Shader->SetUniform3f("lightP2.color", m_LightPoint2->m_Color);
		m_Shader->SetUniform1f("lightP2.constant", m_LightPoint2->m_Attenuation.Constant);
		m_Shader->SetUniform1f("lightP2.linear", m_LightPoint2->m_Attenuation.Linear);
		m_Shader->SetUniform1f("lightP2.quadratic", m_LightPoint2->m_Attenuation.Quadratic);

		m_Shader->SetUniform3f("lightP3.position", m_LightPoint3->m_Position);
		m_Shader->SetUniform3f("lightP3.color", m_LightPoint3->m_Color);
		m_Shader->SetUniform1f("lightP3.constant", m_LightPoint3->m_Attenuation.Constant);
		m_Shader->SetUniform1f("lightP3.linear", m_LightPoint3->m_Attenuation.Linear);
		m_Shader->SetUniform1f("lightP3.quadratic", m_LightPoint3->m_Attenuation.Quadratic);

		//平行光源
		m_Shader->SetUniform3f("lightD.position", m_LightDirectional->m_Position);
		m_Shader->SetUniform3f("lightD.color", m_LightDirectional->m_Color);
		m_Shader->SetUniform3f("lightD.directionToLight", m_LightDirectional->m_Direction);
#endif
		m_AssimpModel->Draw(m_Shader);
	}

	void TestMesh::OnImGuiRender()
	{
	}

	void TestMesh::InitCallback()
	{
		glfwSetWindowUserPointer(m_Window, this);

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) {

			TestMesh* self = (TestMesh*)glfwGetWindowUserPointer(window);

			if (self->m_FOV >= 1.0f && self->m_FOV <= 45.0f)
				self->m_FOV -= yoffset;
			if (self->m_FOV <= 1.0f)
				self->m_FOV = 1.0f;
			if (self->m_FOV >= 45.0f)
				self->m_FOV = 45.0f;
		});

		static bool firstMouse = true;
		static double lastX = SCR_WIDTH;
		static double lastY = SCR_HEIGHT;

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) {

			TestMesh* self = (TestMesh*)glfwGetWindowUserPointer(window);

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
			glViewport(0, 0, width, height);
		});
	}

	void TestMesh::ProcessInput(float deltaTime)
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