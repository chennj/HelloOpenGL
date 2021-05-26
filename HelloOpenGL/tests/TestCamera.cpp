#include "TestCamera.h"

#include "../imgui/imgui.h"

#include <iostream>

using namespace tests;

#define BB_KEY_A                  97
#define BB_KEY_D                  100
#define BB_KEY_S                  119
#define BB_KEY_W                  115

TestCamera::TestCamera()
{
	_MPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	_MRotationAngle = 30.0f;
	_MScale = glm::vec3(1.0f, 1.0f, 1.0f);
	_MRotationDirection = glm::vec3(1.0f, 0.0f, 0.0f);
	// �������Z���ƶ�-150��λ,��Ȼ����������,��Ϊ����positions��z��������0.0
	m_fov = 45.0f;

	//GLCall(glViewport(0.0f, 0.0f, 960.0f, 540.0f));

	// ������˸���������-�����ο���ע�⣺���������ڲ�ͬ������Ҫ����
	//��������ʱ�룬��Ĵָָ��ķ��߷���Ϊ���棩
	//-0.5f,	-0.5f,	 0.5f,/*���½Ƕ������� 0*/	0.0f, 0.0f,		/*��Ӧ����������*/
	// 0.5f,	-0.5f,	 0.5f,/*���½Ƕ������� 1*/	1.0f, 0.0f,		/*��Ӧ����������*/
	// 0.5f,	 0.5f,	 0.5f,/*���ϽǶ������� 2*/	1.0f, 1.0f,		/*��Ӧ����������*/							
	//-0.5f,	 0.5f,	 0.5f,/*���ϽǶ������� 3*/	0.0f, 1.0f,		/*��Ӧ����������*/

	//-0.5f,	-0.5f,	-0.5f,/*���½Ƕ������� 4*/	0.0f, 1.0f,		/*��Ӧ����������*/
	// 0.5f,	-0.5f,	-0.5f,/*���½Ƕ������� 5*/	1.0f, 1.0f,		/*��Ӧ����������*/
	// 0.5f,	 0.5f,	-0.5f,/*���ϽǶ������� 6*/	1.0f, 0.0f,		/*��Ӧ����������*/
	//-0.5f,	 0.5f,	-0.5f,/*���ϽǶ������� 7*/	0.0f, 0.0f,		/*��Ӧ����������*/
	float positions[] = {

		-0.5f,	-0.5f,	 0.5f,/*���½Ƕ������� 0-0*/	0.0f, 0.0f,	/*��Ӧ����������*/
		 0.5f,	-0.5f,	 0.5f,/*���½Ƕ������� 1-1*/	1.0f, 0.0f,	/*��Ӧ����������*/
		 0.5f,	 0.5f,	 0.5f,/*���ϽǶ������� 2-2*/	1.0f, 1.0f,	/*��Ӧ����������*/
		-0.5f,	 0.5f,	 0.5f,/*���ϽǶ������� 3-3*/	0.0f, 1.0f,	/*��Ӧ����������*/

		-0.5f,	-0.5f,	-0.5f,/*���½Ƕ������� 4-4*/	0.0f, 0.0f,	/*��Ӧ����������*/
		-0.5f,	-0.5f,	 0.5f,/*���½Ƕ������� 5-0*/	1.0f, 0.0f,	/*��Ӧ����������*/
		-0.5f,	 0.5f,	 0.5f,/*���ϽǶ������� 6-3*/	1.0f, 1.0f,	/*��Ӧ����������*/
		-0.5f,	 0.5f,	-0.5f,/*���ϽǶ������� 7-7*/	0.0f, 1.0f,	/*��Ӧ����������*/

		 0.5f,	-0.5f,	-0.5f,/*���½Ƕ������� 8-5*/	0.0f, 0.0f,	/*��Ӧ����������*/
		-0.5f,	-0.5f,	-0.5f,/*���½Ƕ������� 9-4*/	1.0f, 0.0f,	/*��Ӧ����������*/
		-0.5f,	 0.5f,	-0.5f,/*���ϽǶ������� 10-7*/	1.0f, 1.0f,	/*��Ӧ����������*/
		 0.5f,	 0.5f,	-0.5f,/*���ϽǶ������� 11-6*/	0.0f, 1.0f,	/*��Ӧ����������*/

		 0.5f,	-0.5f,	 0.5f,/*���½Ƕ������� 12-1*/	0.0f, 0.0f,	/*��Ӧ����������*/
		 0.5f,	-0.5f,	-0.5f,/*���½Ƕ������� 13-5*/	1.0f, 0.0f,	/*��Ӧ����������*/
		 0.5f,	 0.5f,	-0.5f,/*���ϽǶ������� 14-6*/	1.0f, 1.0f,	/*��Ӧ����������*/
		 0.5f,	 0.5f,	 0.5f,/*���ϽǶ������� 15-2*/	0.0f, 1.0f,	/*��Ӧ����������*/

		-0.5f,	 0.5f,	 0.5f,/*���½Ƕ������� 16-3*/	0.0f, 0.0f,	/*��Ӧ����������*/
		 0.5f,	 0.5f,	 0.5f,/*���½Ƕ������� 17-2*/	1.0f, 0.0f,	/*��Ӧ����������*/
		 0.5f,	 0.5f,	-0.5f,/*���ϽǶ������� 18-6*/	1.0f, 1.0f,	/*��Ӧ����������*/
		-0.5f,	 0.5f,	-0.5f,/*���ϽǶ������� 19-7*/	0.0f, 1.0f,	/*��Ӧ����������*/

		-0.5f,	-0.5f,	-0.5f,/*���½Ƕ������� 20-4*/	0.0f, 0.0f,	/*��Ӧ����������*/
		 0.5f,	-0.5f,	-0.5f,/*���½Ƕ������� 21-5*/	1.0f, 0.0f,	/*��Ӧ����������*/
		 0.5f,	-0.5f,	 0.5f,/*���ϽǶ������� 22-1*/	1.0f, 1.0f,	/*��Ӧ����������*/
		-0.5f,	-0.5f,	 0.5f,/*���ϽǶ������� 23-0*/	0.0f, 1.0f,	/*��Ӧ����������*/
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

	_VAO = CreateScope<VertexArray>();
	_VBO = CreateScope<VertexBuffer>(positions, sizeof(positions)/*5 * 24 * sizeof(float)*/);
	VertexBufferLayout layout;
	layout.Push<float>(3);	// ��������(x,y)
	layout.Push<float>(2);	// ��������(s,t)

	_VAO->AddBuffer(*_VBO, layout);
	_IndexBuffer = CreateScope<IndexBuffer>(indices, sizeof(indices) / sizeof(unsigned int));

	_Shader = CreateScope<Shader>("shaders/Basic3D.shader");
	_Shader->Bind();

	_Texture = CreateScope<Texture>("../res/texture/texture-02-1.png");
	_Shader->SetUniform1i("u_Texture", 0/*��Ϊ���ǵ��������0���ۣ���Texture::Bind()*/);


	_CubePositions = new glm::vec3[10];
	_CubePositions[0] = glm::vec3(0.0f, 0.0f, 0.0f);
	_CubePositions[1] = glm::vec3(2.0f, 5.0f, -15.0f);
	_CubePositions[2] = glm::vec3(-1.5f, -2.2f, -2.5f);
	_CubePositions[3] = glm::vec3(-3.8f, -2.0f, -12.3f);
	_CubePositions[4] = glm::vec3(2.4f, -0.4f, -3.5f);
	_CubePositions[5] = glm::vec3(-1.7f, 3.0f, -7.5f);
	_CubePositions[6] = glm::vec3(1.3f, -2.0f, -2.5f);
	_CubePositions[7] = glm::vec3(1.5f, 2.0f, -2.5f);
	_CubePositions[8] = glm::vec3(1.5f, 0.2f, -1.5f);
	_CubePositions[9] = glm::vec3(-1.3f, 1.0f, -1.5f);

	m_camera_position = glm::vec3(0.0f, 0.0f, 6.0f);
	m_camera_pitch = glm::radians(0.0f);
	m_camera_yaw = glm::radians(180.0f);
	m_camera_worldup = glm::vec3(0.0f, 1.0f, 0.0f);

	// ������
	//m_camera = CreateScope<Camera>(
	//	glm::vec3(0.0f, 0.0f, 3.0f),
	//	glm::vec3(0.0f, 0.0f, 0.0f),
	//	glm::vec3(0.0f, 1.0f, 0.0f)
	//	);

	// ŷ���Ƿ� 
	m_camera = CreateScope<Camera>(
		m_camera_position,	// ���λ��
		m_camera_pitch,		// ���������ת��0��
		m_camera_yaw,		// �������ת��0������ϵ-������ϵ
		m_camera_worldup	// ������������
		);

	m_window = glfwGetCurrentContext();
	ASSERT(m_window);

	InitCallback();
}

TestCamera::~TestCamera()
{
	delete[] _CubePositions;

	glfwSetWindowUserPointer(m_window, nullptr);
	glfwSetCursorPosCallback(m_window, nullptr);
	glfwSetScrollCallback(m_window, nullptr);
}

void TestCamera::OnUpdate(float deltaTime)
{
	m_camera->Update(m_camera_position, m_camera_pitch, m_camera_yaw, m_camera_worldup);
}

void TestCamera::OnRender()
{
	GLCall(glClearColor(0, 0, 0, 1));
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Renderer renderer;
	_Texture->Bind();

	/**
	* model ģ�ͱ任��������Ӿֲ�����ϵ�任����������ϵ�������������ڵĳ�����
	*				����������һ��ɳ����ͨ��ģ�ͱ任���������ɳ�����������Ϻ�����������ɼ��
	* _View ��ͼ�任���������Ϊ�۲�㣬�����������λ�õ�λ�úͷ��򣬼���������ϵ�任���۲�����ϵ
	*				���������������������ߣ��൱���������ұ��ƶ���
	* _Proj ͶӰ�任������ͶӰ��͸��ͶӰ����׶�壬����ԶС��FOV�ӽǴ�С����������ͶӰ���ü��ռ䣨��ƽ�棩��
	*				����������һ��ɳ�����㵽���Ǵ���߿����������ұߡ�ʹ�����뿴��������Զ�˿���
	*				�Ǵ��ϵ��¿������Ǵ������Ͽ���������̫�������ֻ���õ�ɳ����һ���֡�
	*/
#if 0
	{
		_Proj = glm::perspective(glm::radians(_FOV), 1200.0f / 600.0f/*Viewport��width/height*/, 0.1f, 1000.0f);
		_Model = glm::translate(glm::mat4(1.0f), _MPosition)
			* glm::rotate(glm::mat4(1.0f), glm::radians(_MRotationAngle), _MRotationDirection)
			* glm::scale(glm::mat4(1.0f), _MScale);
		_View = m_camera->GetViewMatrix();
		_Shader->Bind();
		_Shader->SetUniformMat4f("u_Model", _Model);
		_Shader->SetUniformMat4f("u_View", _View);
		_Shader->SetUniformMat4f("u_Proj", _Proj);
		renderer.Draw(*_VAO, *_IndexBuffer, *_Shader);
	}
#endif
#if 1
	// �����������
	{
		for (unsigned int i = 0; i < 10; i++)
		{
			float angle = _MRotationAngle;
			if (i % 3 == 0)
				angle = glfwGetTime() * 25.0f;
			_Proj = glm::perspective(glm::radians(m_fov), 1200.0f / 600.0f/*Viewport��width/height*/, 0.1f, 1000.0f);
			_Model = glm::translate(glm::mat4(1.0f), _CubePositions[i])
				* glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f))
				* glm::scale(glm::mat4(1.0f), _MScale);
			_View = m_camera->GetViewMatrix();
			_Shader->Bind();
			_Shader->SetUniformMat4f("u_Model", _Model);
			_Shader->SetUniformMat4f("u_View", _View);
			_Shader->SetUniformMat4f("u_Proj", _Proj);
			renderer.Draw(*_VAO, *_IndexBuffer, *_Shader);
		}
	}
#endif
}

void TestCamera::OnImGuiRender()
{
	ImGui::DragFloat3("Camera Position", &m_camera_position.x, 0.01f);
	ImGui::DragFloat("Camera pitch", &m_camera_pitch, 0.01f);
	ImGui::DragFloat("Camera yaw", &m_camera_yaw, 0.01f);
}

void TestCamera::InitCallback()
{
	// ȡ���α�
	//GLCall(glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED));

	static bool firstMouse = true;
	static double lastX = 0.0f;
	static double lastY = 0.0f;

	glfwSetWindowUserPointer(m_window, this);

	glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {

		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		TestCamera* self = (TestCamera*)glfwGetWindowUserPointer(window);

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		float speed = 0.01;
		xoffset *= speed;
		yoffset *= speed;

		self->m_camera_yaw += xoffset;
		self->m_camera_pitch += yoffset;
		if (self->m_camera_pitch > 89.0f)
			self->m_camera_pitch = 89.0f;
		if (self->m_camera_pitch < -89.0f)
			self->m_camera_pitch = -89.0f;

		//std::cout << "glfwSetCursorPosCallback() pitch=" << self->m_camera_pitch << ", yaw=" << self->m_camera_yaw << std::endl;

	});

	glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset){

		TestCamera* self = (TestCamera*)glfwGetWindowUserPointer(window);

		if (self->m_fov >= 1.0f && self->m_fov <= 45.0f)
			self->m_fov -= yoffset;
		if (self->m_fov <= 1.0f)
			self->m_fov = 1.0f;
		if (self->m_fov >= 45.0f)
			self->m_fov = 45.0f;
	});

	glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int keycode)
	{
		TestCamera* self = (TestCamera*)glfwGetWindowUserPointer(window);

		//std::cout << keycode << std::endl;

		float speed = 0.05f;

		switch (keycode)
		{
		case BB_KEY_A:
		case GLFW_KEY_A:
			self->m_camera_position.x -= speed;
			break;
		case BB_KEY_D:
		case GLFW_KEY_D:
			self->m_camera_position.x += speed;
			break;
		case BB_KEY_W:
		case GLFW_KEY_W :
			self->m_camera_position.z -= speed;
			break;
		case BB_KEY_S:
		case GLFW_KEY_S:
			self->m_camera_position.z += speed;
			break;
		case GLFW_KEY_UP:
			self->m_camera_position.y -= speed;
			break;
		case GLFW_KEY_DOWN:
			self->m_camera_position.y += speed;
			break;
		}
	});
}

