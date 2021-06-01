/**
*
* 冯氏光照模型:
* ----------------------------------------------------------------
* 这是一种经验模型 = 环境光（ambinet） + 漫反射（diffuse） + 镜面反射（specular）
*/
#include "TestLight.h"

#include "../imgui/imgui.h"
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

		-0.5f,	-0.5f,	 0.5f,	/*顶点 0-0*/		 0.0f,	 0.0f,	  1.0f,	/*法向量*/	0.0f, 0.0f,	/*坐标*/
		 0.5f,	-0.5f,	 0.5f,	/*顶点 1-1*/		 0.0f,	 0.0f,	  1.0f,	/*法向量*/	1.0f, 0.0f,	/*坐标*/
		 0.5f,	 0.5f,	 0.5f,	/*顶点 2-2*/		 0.0f,	 0.0f,	  1.0f,	/*法向量*/	1.0f, 1.0f,	/*坐标*/
		-0.5f,	 0.5f,	 0.5f,	/*顶点 3-3*/		 0.0f,	 0.0f,	  1.0f,	/*法向量*/	0.0f, 1.0f,	/*坐标*/
												
		-0.5f,	-0.5f,	-0.5f,	/*顶点 4-4*/		-1.0f,	 0.0f,	  0.0f,	/*法向量*/	0.0f, 0.0f,	/*坐标*/
		-0.5f,	-0.5f,	 0.5f,	/*顶点 5-0*/		-1.0f,	 0.0f,	  0.0f,	/*法向量*/	1.0f, 0.0f,	/*坐标*/
		-0.5f,	 0.5f,	 0.5f,	/*顶点 6-3*/		-1.0f,	 0.0f,	  0.0f,	/*法向量*/	1.0f, 1.0f,	/*坐标*/
		-0.5f,	 0.5f,	-0.5f,	/*顶点 7-7*/		-1.0f,	 0.0f,	  0.0f,	/*法向量*/	0.0f, 1.0f,	/*坐标*/
																  
		 0.5f,	-0.5f,	-0.5f,	/*顶点 8-5*/		 0.0f,	 0.0f,	 -1.0f,	/*法向量*/	0.0f, 0.0f,	/*坐标*/
		-0.5f,	-0.5f,	-0.5f,	/*顶点 9-4*/		 0.0f,	 0.0f,	 -1.0f,	/*法向量*/	1.0f, 0.0f,	/*坐标*/
		-0.5f,	 0.5f,	-0.5f,	/*顶点 10-7*/	 0.0f,	 0.0f,	 -1.0f,	/*法向量*/	1.0f, 1.0f,	/*坐标*/
		 0.5f,	 0.5f,	-0.5f,	/*顶点 11-6*/	 0.0f,	 0.0f,	 -1.0f,	/*法向量*/	0.0f, 1.0f,	/*坐标*/
												 				  
		 0.5f,	-0.5f,	 0.5f,	/*顶点 12-1*/	 1.0f,	 0.0f,	  0.0f,	/*法向量*/	0.0f, 0.0f,	/*坐标*/
		 0.5f,	-0.5f,	-0.5f,	/*顶点 13-5*/	 1.0f,	 0.0f,	  0.0f,	/*法向量*/	1.0f, 0.0f,	/*坐标*/
		 0.5f,	 0.5f,	-0.5f,	/*顶点 14-6*/	 1.0f,	 0.0f,	  0.0f,	/*法向量*/	1.0f, 1.0f,	/*坐标*/
		 0.5f,	 0.5f,	 0.5f,	/*顶点 15-2*/	 1.0f,	 0.0f,	  0.0f,	/*法向量*/	0.0f, 1.0f,	/*坐标*/
												 				  
		-0.5f,	 0.5f,	 0.5f,	/*顶点 16-3*/	 0.0f,	 1.0f,	  0.0f,	/*法向量*/	0.0f, 0.0f,	/*坐标*/
		 0.5f,	 0.5f,	 0.5f,	/*顶点 17-2*/	 0.0f,	 1.0f,	  0.0f,	/*法向量*/	1.0f, 0.0f,	/*坐标*/
		 0.5f,	 0.5f,	-0.5f,	/*顶点 18-6*/	 0.0f,	 1.0f,	  0.0f,	/*法向量*/	1.0f, 1.0f,	/*坐标*/
		-0.5f,	 0.5f,	-0.5f,	/*顶点 19-7*/	 0.0f,	 1.0f,	  0.0f,	/*法向量*/	0.0f, 1.0f,	/*坐标*/
												 				  
		-0.5f,	-0.5f,	-0.5f,	/*顶点 20-4*/	 0.0f,	-1.0f,	  0.0f,	/*法向量*/	0.0f, 0.0f,	/*坐标*/
		 0.5f,	-0.5f,	-0.5f,	/*顶点 21-5*/	 0.0f,	-1.0f,	  0.0f,	/*法向量*/	1.0f, 0.0f,	/*坐标*/
		 0.5f,	-0.5f,	 0.5f,	/*顶点 22-1*/	 0.0f,	-1.0f,	  0.0f,	/*法向量*/	1.0f, 1.0f,	/*坐标*/
		-0.5f,	-0.5f,	 0.5f,	/*顶点 23-0*/	 0.0f,	-1.0f,	  0.0f,	/*法向量*/	0.0f, 1.0f,	/*坐标*/
		};

		unsigned int indices[] = {
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

		m_VAO = CreateScope<VertexArray>();

		VertexBufferLayout layout;
		layout.Push<float>(3);	// 顶点坐标(x,y,z)
		layout.Push<float>(3);	// 法向量(x,y,z)
		layout.Push<float>(2);	// 纹理坐标(s,t)

		m_VBO = CreateScope<VertexBuffer>(positions, sizeof(positions));
		m_VAO->AddBuffer(*m_VBO, layout);

		m_IBO = CreateScope<IndexBuffer>(indices, sizeof(indices) / sizeof(unsigned int));

		m_Shader = CreateRef<Shader>("shaders/Material.shader");

		m_Material = CreateScope<Material>(m_Shader, "material");
		m_Light = CreateScope<Material>(m_Shader, "light");

		m_Background	= CreateScope<Texture>();
		m_Texture		= CreateScope<Texture>("../res/texture/matrix.jpg");

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
		// 欧拉角法 
		m_Camera			= CreateScope<Camera>(
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

	TestLight::~TestLight()
	{
		delete[] m_CubePositions;

		glfwSetWindowUserPointer(m_Window, nullptr);
		glfwSetScrollCallback(m_Window, nullptr);
		glfwSetCharCallback(m_Window, nullptr);
	}

	void TestLight::OnUpdate(float deltaTime)
	{
		//m_Camera->UpdateVector(m_CameraPosition, m_CameraPitch, m_CameraYaw, m_CameraWorldup);
		//m_Camera->UpdatePosition();

		//m_LightColor.x = sin(glfwGetTime() * 2.0f);
		//m_LightColor.y = sin(glfwGetTime() * 0.7f);
		//m_LightColor.z = sin(glfwGetTime() * 1.3f);

	}

	void TestLight::OnRender()
	{
		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Renderer renderer;

		for (unsigned int i = 0; i < 10; i++)
		{
			float angle = m_ModelRotationAngle;
			if (i % 3 == 0)
				angle = glfwGetTime() * 25.0f;
			m_Proj = glm::perspective(glm::radians(m_FOV), 1200.0f / 600.0f/*Viewport的width/height*/, 0.1f, 100.0f);
			m_Model = glm::translate(glm::mat4(1.0f), m_CubePositions[i])
				* glm::rotate(glm::mat4(1.0f), glm::radians(angle), m_ModelRotationDirection)
				* glm::scale(glm::mat4(1.0f), m_ModelScale);
			m_View = m_Camera->GetViewMatrix();

			m_Shader->Bind();

			m_Background->Bind(0);
			m_Shader->SetUniform1i("u_Background", 0);
			m_Texture->Bind(1);
			m_Shader->SetUniform1i("u_Texture", 1);

			m_Shader->SetUniformMat4f("u_Model",	m_Model);
			m_Shader->SetUniformMat4f("u_View",		m_View);
			m_Shader->SetUniformMat4f("u_Proj",		m_Proj);

			m_Shader->SetUniform3f("viewPosition",		m_Camera->GetPosition());

			m_Light->SetPosition(m_LightPosition);
			m_Light->SetAmbient(glm::vec3(0.2f, 0.2f, 0.2f));
			m_Light->SetDiffuse(glm::vec3(0.5f, 0.5f, 0.5f));
			m_Light->SetSpecular(glm::vec3(1.0f, 1.0f, 1.0f));

			m_Material->SetDiffuse(m_Diffuse, 3);
			m_Material->SetSpecular(m_Specular, 4);
			m_Material->SetShininess(64.0f);

			renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
		}
	}

	void TestLight::OnImGuiRender()
	{
		ImGui::DragFloat3("Light Color",	&m_LightColor.x,	0.01f);
		ImGui::DragFloat3("Light Pos",		&m_LightPosition.x,	0.01f);
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