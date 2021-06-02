/**
*
* ���Ϲ���ģ��:
* ----------------------------------------------------------------
* ����һ�־���ģ�� = �����⣨ambinet�� + �����䣨diffuse�� + ���淴�䣨specular��
*/
#include "TestLight.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include <iostream>

#define BB_KEY_A                  97
#define BB_KEY_D                  100
#define BB_KEY_S                  119
#define BB_KEY_W                  115
#define BB_KEY_Q                  113
#define BB_KEY_E                  101

namespace tests
{
	TestLight::TestLight()
	{
		float positions[] = {

		-0.5f,	-0.5f,	 0.5f,	/*���� 0-0*/		 0.0f,	 0.0f,	  1.0f,	/*������*/	0.0f, 0.0f,	/*����*/
		 0.5f,	-0.5f,	 0.5f,	/*���� 1-1*/		 0.0f,	 0.0f,	  1.0f,	/*������*/	1.0f, 0.0f,	/*����*/
		 0.5f,	 0.5f,	 0.5f,	/*���� 2-2*/		 0.0f,	 0.0f,	  1.0f,	/*������*/	1.0f, 1.0f,	/*����*/
		-0.5f,	 0.5f,	 0.5f,	/*���� 3-3*/		 0.0f,	 0.0f,	  1.0f,	/*������*/	0.0f, 1.0f,	/*����*/
												
		-0.5f,	-0.5f,	-0.5f,	/*���� 4-4*/		-1.0f,	 0.0f,	  0.0f,	/*������*/	0.0f, 0.0f,	/*����*/
		-0.5f,	-0.5f,	 0.5f,	/*���� 5-0*/		-1.0f,	 0.0f,	  0.0f,	/*������*/	1.0f, 0.0f,	/*����*/
		-0.5f,	 0.5f,	 0.5f,	/*���� 6-3*/		-1.0f,	 0.0f,	  0.0f,	/*������*/	1.0f, 1.0f,	/*����*/
		-0.5f,	 0.5f,	-0.5f,	/*���� 7-7*/		-1.0f,	 0.0f,	  0.0f,	/*������*/	0.0f, 1.0f,	/*����*/
																  
		 0.5f,	-0.5f,	-0.5f,	/*���� 8-5*/		 0.0f,	 0.0f,	 -1.0f,	/*������*/	0.0f, 0.0f,	/*����*/
		-0.5f,	-0.5f,	-0.5f,	/*���� 9-4*/		 0.0f,	 0.0f,	 -1.0f,	/*������*/	1.0f, 0.0f,	/*����*/
		-0.5f,	 0.5f,	-0.5f,	/*���� 10-7*/	 0.0f,	 0.0f,	 -1.0f,	/*������*/	1.0f, 1.0f,	/*����*/
		 0.5f,	 0.5f,	-0.5f,	/*���� 11-6*/	 0.0f,	 0.0f,	 -1.0f,	/*������*/	0.0f, 1.0f,	/*����*/
												 				  
		 0.5f,	-0.5f,	 0.5f,	/*���� 12-1*/	 1.0f,	 0.0f,	  0.0f,	/*������*/	0.0f, 0.0f,	/*����*/
		 0.5f,	-0.5f,	-0.5f,	/*���� 13-5*/	 1.0f,	 0.0f,	  0.0f,	/*������*/	1.0f, 0.0f,	/*����*/
		 0.5f,	 0.5f,	-0.5f,	/*���� 14-6*/	 1.0f,	 0.0f,	  0.0f,	/*������*/	1.0f, 1.0f,	/*����*/
		 0.5f,	 0.5f,	 0.5f,	/*���� 15-2*/	 1.0f,	 0.0f,	  0.0f,	/*������*/	0.0f, 1.0f,	/*����*/
												 				  
		-0.5f,	 0.5f,	 0.5f,	/*���� 16-3*/	 0.0f,	 1.0f,	  0.0f,	/*������*/	0.0f, 0.0f,	/*����*/
		 0.5f,	 0.5f,	 0.5f,	/*���� 17-2*/	 0.0f,	 1.0f,	  0.0f,	/*������*/	1.0f, 0.0f,	/*����*/
		 0.5f,	 0.5f,	-0.5f,	/*���� 18-6*/	 0.0f,	 1.0f,	  0.0f,	/*������*/	1.0f, 1.0f,	/*����*/
		-0.5f,	 0.5f,	-0.5f,	/*���� 19-7*/	 0.0f,	 1.0f,	  0.0f,	/*������*/	0.0f, 1.0f,	/*����*/
												 				  
		-0.5f,	-0.5f,	-0.5f,	/*���� 20-4*/	 0.0f,	-1.0f,	  0.0f,	/*������*/	0.0f, 0.0f,	/*����*/
		 0.5f,	-0.5f,	-0.5f,	/*���� 21-5*/	 0.0f,	-1.0f,	  0.0f,	/*������*/	1.0f, 0.0f,	/*����*/
		 0.5f,	-0.5f,	 0.5f,	/*���� 22-1*/	 0.0f,	-1.0f,	  0.0f,	/*������*/	1.0f, 1.0f,	/*����*/
		-0.5f,	-0.5f,	 0.5f,	/*���� 23-0*/	 0.0f,	-1.0f,	  0.0f,	/*������*/	0.0f, 1.0f,	/*����*/
		};

		unsigned int indices[] = {
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

		m_VAO = CreateScope<VertexArray>();

		VertexBufferLayout layout;
		layout.Push<float>(3);	// ��������(x,y,z)
		layout.Push<float>(3);	// ������(x,y,z)
		layout.Push<float>(2);	// ��������(s,t)

		m_VBO = CreateScope<VertexBuffer>(positions, sizeof(positions));
		m_VAO->AddBuffer(*m_VBO, layout);

		m_IBO = CreateScope<IndexBuffer>(indices, sizeof(indices) / sizeof(unsigned int));

		m_Shader = CreateRef<Shader>("shaders/MultiLight.shader");

		m_Material = CreateScope<Material>(m_Shader, "material");

		m_LightDirectional = CreateScope<LightDirectional>(
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(90.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 1.0f, 1.0f)
			);
		m_LightPoint0 = CreateScope<LightPoint>(
			glm::vec3(1.0f, 0.0f, 0.0f), 
			glm::vec3(1.0f, 0.0f, 0.0f),
			1.0f, 0.1f, 0.032f
			);
		m_LightPoint1 = CreateScope<LightPoint>(
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			1.0f, 0.1f, 0.032f
			);
		m_LightPoint2 = CreateScope<LightPoint>(
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			1.0f, 0.1f, 0.032f
			);
		m_LightPoint3 = CreateScope<LightPoint>(
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			1.0f, 0.1f, 0.032f
			);

		m_LightSpot = CreateScope<LightSpot>(
			glm::vec3(0.0f, 0.0f, 2.0f), 
			glm::vec3(180.0f, 0.0f, 0.0f), 
			glm::vec3(2.0f, 2.0f, 2.0f),
			0.99f,0.97f,
			1.0f, 0.1f, 0.032f
			);

		m_Diffuse		= CreateRef<Texture>("../res/texture/container2.png");
		m_Specular		= CreateRef<Texture>("../res/texture/container2_specular.png");

		m_CubePositions		= new glm::vec3[10];
		m_CubePositions[0]	= glm::vec3(0.0f, 0.0f, 0.0f);
		m_CubePositions[1]	= glm::vec3(2.0f, 5.0f, -15.0f);
		m_CubePositions[2]	= glm::vec3(-1.5f, -2.2f, -2.5f);
		m_CubePositions[3]	= glm::vec3(-3.8f, -2.0f, -12.3f);
		m_CubePositions[4]	= glm::vec3(2.4f, -0.4f, -3.5f);
		m_CubePositions[5]	= glm::vec3(-1.7f, 3.0f, -7.5f);
		m_CubePositions[6]	= glm::vec3(1.3f, -2.0f, -2.5f);
		m_CubePositions[7]	= glm::vec3(1.5f, 2.0f, -2.5f);
		m_CubePositions[8]	= glm::vec3(1.5f, 0.2f, -1.5f);
		m_CubePositions[9]	= glm::vec3(-1.3f, 1.0f, -1.5f);

		m_CameraPosition	= glm::vec3(0.0f, 0.0f, 6.0f);
		m_CameraPitch		= glm::radians(0.0f);
		m_CameraYaw			= glm::radians(180.0f);
		m_CameraWorldup		= glm::vec3(0.0f, 1.0f, 0.0f);
		// ŷ���Ƿ� 
		m_Camera			= CreateScope<Camera>(
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

	TestLight::~TestLight()
	{
		delete[] m_CubePositions;

		glfwSetWindowUserPointer(m_Window, nullptr);
		glfwSetScrollCallback(m_Window, nullptr);
		glfwSetCharCallback(m_Window, nullptr);
	}

	void TestLight::OnUpdate(float deltaTime)
	{
		m_LightSpot->UpdateDirection();
	}

	void TestLight::OnRender()
	{
		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Renderer renderer;

		for (unsigned int i = 0; i < 10; i++)
		{
			float angle = m_ModelRotationAngle;
			//if (i % 3 == 0)
			//	angle = glfwGetTime() * 25.0f;
			m_Proj = glm::perspective(glm::radians(m_FOV), 1200.0f / 600.0f/*Viewport��width/height*/, 0.1f, 100.0f);
			m_Model = glm::translate(glm::mat4(1.0f), m_CubePositions[i])
				* glm::rotate(glm::mat4(1.0f), glm::radians(angle), m_ModelRotationDirection)
				* glm::scale(glm::mat4(1.0f), m_ModelScale);
			m_View = m_Camera->GetViewMatrix();

			m_Shader->Bind();

			m_Material->SetDiffuse(m_Diffuse, 0);
			m_Material->SetSpecular(m_Specular, 1);
			m_Material->SetShininess(64.0f);

			m_Shader->SetUniformMat4f("u_Model",	m_Model);
			m_Shader->SetUniformMat4f("u_View",		m_View);
			m_Shader->SetUniformMat4f("u_Proj",		m_Proj);

			m_Shader->SetUniform3f("cameraPosition",	m_Camera->GetPosition());

#if 1		// �۹��
			m_Shader->SetUniform3f("lightS.position",			m_LightSpot->m_Position);
			m_Shader->SetUniform3f("lightS.directionToLight",	m_LightSpot->m_Direction);
			m_Shader->SetUniform3f("lightS.color",				m_LightSpot->m_Color);
			m_Shader->SetUniform1f("lightS.cutoff",				m_LightSpot->m_Cutoff);
			m_Shader->SetUniform1f("lightS.cutoffouter",		m_LightSpot->m_CutoffOuter);
			m_Shader->SetUniform1f("lightS.constant",			m_LightSpot->m_Attenuation.Constant);
			m_Shader->SetUniform1f("lightS.linear",				m_LightSpot->m_Attenuation.Linear);
			m_Shader->SetUniform1f("lightS.quadratic",			m_LightSpot->m_Attenuation.Quadratic);
#endif

#if 1		// ���Դ
			m_Shader->SetUniform3f("lightP0.position",	m_LightPoint0->m_Position);
			m_Shader->SetUniform3f("lightP0.color",		m_LightPoint0->m_Color);
			m_Shader->SetUniform1f("lightP0.constant",	m_LightPoint0->m_Attenuation.Constant);
			m_Shader->SetUniform1f("lightP0.linear",	m_LightPoint0->m_Attenuation.Linear);
			m_Shader->SetUniform1f("lightP0.quadratic",	m_LightPoint0->m_Attenuation.Quadratic);

			m_Shader->SetUniform3f("lightP1.position",	m_LightPoint1->m_Position);
			m_Shader->SetUniform3f("lightP1.color",		m_LightPoint1->m_Color);
			m_Shader->SetUniform1f("lightP1.constant",	m_LightPoint1->m_Attenuation.Constant);
			m_Shader->SetUniform1f("lightP1.linear",	m_LightPoint1->m_Attenuation.Linear);
			m_Shader->SetUniform1f("lightP1.quadratic", m_LightPoint1->m_Attenuation.Quadratic);

			m_Shader->SetUniform3f("lightP2.position",	m_LightPoint2->m_Position);
			m_Shader->SetUniform3f("lightP2.color",		m_LightPoint2->m_Color);
			m_Shader->SetUniform1f("lightP2.constant",	m_LightPoint2->m_Attenuation.Constant);
			m_Shader->SetUniform1f("lightP2.linear",	m_LightPoint2->m_Attenuation.Linear);
			m_Shader->SetUniform1f("lightP2.quadratic", m_LightPoint2->m_Attenuation.Quadratic);

			m_Shader->SetUniform3f("lightP3.position",	m_LightPoint3->m_Position);
			m_Shader->SetUniform3f("lightP3.color",		m_LightPoint3->m_Color);
			m_Shader->SetUniform1f("lightP3.constant",	m_LightPoint3->m_Attenuation.Constant);
			m_Shader->SetUniform1f("lightP3.linear",	m_LightPoint3->m_Attenuation.Linear);
			m_Shader->SetUniform1f("lightP3.quadratic", m_LightPoint3->m_Attenuation.Quadratic);
#endif

#if 1		//ƽ�й�Դ
			m_Shader->SetUniform3f("lightD.position",			m_LightDirectional->m_Position);
			m_Shader->SetUniform3f("lightD.color",				m_LightDirectional->m_Color);
			m_Shader->SetUniform3f("lightD.directionToLight",	m_LightDirectional->m_Direction);
#endif
			//m_Shader->SetUniform3f("light.ambient",		glm::vec3(0.1f, 0.1f, 0.1f));
			//m_Shader->SetUniform3f("light.diffuse",		glm::vec3(0.5f, 0.5f, 0.5f));
			//m_Shader->SetUniform3f("light.specular",	glm::vec3(1.0f, 1.0f, 1.0f));


			renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
		}
	}

	void TestLight::OnImGuiRender()
	{
		ImGui::Separator();
		ImGui::Text("%s", "Spot");
		ImGui::DragFloat3("Color",		&m_LightSpot->m_Color.x, 0.01f);
		ImGui::DragFloat3("Angle",		&m_LightSpot->m_Angle.x, 0.5f);
		ImGui::DragFloat3("Position",	&m_LightSpot->m_Position.x, 0.01f);
		ImGui::DragFloat("Cutoff",		&m_LightSpot->m_Cutoff, 0.01f);
		ImGui::DragFloat("CutoffOuter", &m_LightSpot->m_CutoffOuter, 0.01f);
	}

	void TestLight::InitCallback()
	{
		glfwSetWindowUserPointer(m_Window, this);

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) {

			TestLight* self = (TestLight*)glfwGetWindowUserPointer(window);

			if (self->m_FOV >= 1.0f && self->m_FOV <= 45.0f)
				self->m_FOV -= yoffset;
			if (self->m_FOV <= 1.0f)
				self->m_FOV = 1.0f;
			if (self->m_FOV >= 45.0f)
				self->m_FOV = 45.0f;
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
		{
			TestLight* self = (TestLight*)glfwGetWindowUserPointer(window);

			//std::cout << keycode << std::endl;

			float speed = 0.1f;

			switch (keycode)
			{
			case BB_KEY_A:
			case GLFW_KEY_A:
				self->m_CameraPosition.x -= speed;
				break;
			case BB_KEY_D:
			case GLFW_KEY_D:
				self->m_CameraPosition.x += speed;
				break;
			case BB_KEY_W:
			case GLFW_KEY_W:
				self->m_CameraPosition.z += speed;
				break;
			case BB_KEY_S:
			case GLFW_KEY_S:
				self->m_CameraPosition.z -= speed;
				break;
			case BB_KEY_Q:
			case GLFW_KEY_Q:
				self->m_CameraPosition.y += speed;
				break;
			case BB_KEY_E:
			case GLFW_KEY_E:
				self->m_CameraPosition.y -= speed;
				break;
			}
			self->m_Camera->UpdateVector(
				self->m_CameraPosition, 
				self->m_CameraPitch, 
				self->m_CameraYaw, 
				self->m_CameraWorldup);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			TestLight* self = (TestLight*)glfwGetWindowUserPointer(window);

			//std::cout << "----" << std::endl;
			//std::cout << "key=" << key << ","
			//	<< "scancode=" << scancode << ","
			//	<< "action=" << action << ","
			//	<< "mods=" << mods << std::endl;

			float speed = 0.005f;

			switch (action)
			{
				case GLFW_RELEASE:
				{
					break;
				}
				case GLFW_PRESS:
				case GLFW_REPEAT:
				{
					switch (key)
					{
					case GLFW_KEY_LEFT:
						self->m_CameraYaw += speed * 2;
						break;
					case GLFW_KEY_RIGHT:
						self->m_CameraYaw -= speed * 2;
						break;
					case GLFW_KEY_UP:
						self->m_CameraPosition.z -= speed * 10;
						break;
					case GLFW_KEY_DOWN:
						self->m_CameraPosition.z += speed * 10;
						break;
					case GLFW_KEY_PAGE_UP:
						self->m_CameraPitch += speed;
						break;
					case GLFW_KEY_PAGE_DOWN:
						self->m_CameraPitch -= speed;
						break;
					}
					break;
				}
			}
			self->m_Camera->UpdateVector(
				self->m_CameraPosition,
				self->m_CameraPitch,
				self->m_CameraYaw,
				self->m_CameraWorldup);
		});
	}
}