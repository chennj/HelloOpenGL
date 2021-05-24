#include "TestTexture3D.h"

#include "../core/Renderer.h"
#include "../imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <math.h>

using namespace tests;

TestTexture3D::TestTexture3D()
{
	_MPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	_MRotationAngle = 30.0f;
	_MScale = glm::vec3(1.0f, 1.0f, 1.0f);
	_MRotationDirection = glm::vec3(1.0f, 0.0f, 0.0f);
	// �������Z���ƶ�-150��λ,��Ȼ����������,��Ϊ����positions��z��������0.0
	_View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
	_FOV = 45.0f;

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


	_CubePositions= new glm::vec3[10]; 
	_CubePositions[0] = glm::vec3(0.0f, 0.0f, 0.0f);
	_CubePositions[1] = glm::vec3(2.0f,  5.0f, -15.0f);
	_CubePositions[2] = glm::vec3(-1.5f, -2.2f, -2.5f);
	_CubePositions[3] = glm::vec3(-3.8f, -2.0f, -12.3f);
	_CubePositions[4] = glm::vec3(2.4f, -0.4f, -3.5f);
	_CubePositions[5] = glm::vec3(-1.7f,  3.0f, -7.5f);
	_CubePositions[6] = glm::vec3(1.3f, -2.0f, -2.5f);
	_CubePositions[7] = glm::vec3(1.5f,  2.0f, -2.5f);
	_CubePositions[8] = glm::vec3(1.5f,  0.2f, -1.5f);
	_CubePositions[9] = glm::vec3(-1.3f,  1.0f, -1.5f);

}

TestTexture3D::~TestTexture3D()
{
	delete[] _CubePositions;
}

void TestTexture3D::OnUpdate(float deltaTime)
{
}

void TestTexture3D::OnRender()
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
	{
		_Proj = glm::perspective(glm::radians(_FOV), 1200.0f/600.0f/*Viewport��width/height*/, 0.1f, 1000.0f);
		_Model = glm::translate(glm::mat4(1.0f), _MPosition)
			* glm::rotate(glm::mat4(1.0f), glm::radians(_MRotationAngle), _MRotationDirection)
			* glm::scale(glm::mat4(1.0f), _MScale);
		_Shader->Bind();
		_Shader->SetUniformMat4f("u_Model", _Model);
		_Shader->SetUniformMat4f("u_View", _View);
		_Shader->SetUniformMat4f("u_Proj", _Proj);
		renderer.Draw(*_VAO, *_IndexBuffer, *_Shader);
	}

	// �����������
	{
		for (unsigned int i = 0; i < 10; i++)
		{
			//_MRotationDirection.x = (float)std::fmod(_MRotationDirection.x + _UInt(_UEngine), 1);
			//_MRotationDirection.y = (float)std::fmod(_MRotationDirection.y + _UInt(_UEngine), 1);
			//_MRotationDirection.z = (float)std::fmod(_MRotationDirection.z + _UInt(_UEngine), 1);
			float angle = _MRotationAngle;
			if (i % 3 == 0) 
				angle = glfwGetTime() * 25.0f;
			_Model = glm::translate(glm::mat4(1.0f), _CubePositions[i])
				* glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f))
				* glm::scale(glm::mat4(1.0f), _MScale);
			_Shader->Bind();
			_Shader->SetUniformMat4f("u_Model", _Model);
			_Shader->SetUniformMat4f("u_View", _View);
			_Shader->SetUniformMat4f("u_Proj", _Proj);
			renderer.Draw(*_VAO, *_IndexBuffer, *_Shader);
		}
	}
}

void TestTexture3D::OnImGuiRender()
{
	ImGui::DragFloat3("Model Position", &_MPosition.x);
	ImGui::DragFloat("Model Rotation", &_MRotationAngle); 
	ImGui::DragFloat("Proj FOV", &_FOV);
	ImGui::DragFloat3("Model Direction", &_MRotationDirection.x);
	ImGui::DragFloat3("Model Scale", &_MScale.x);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	uint32_t textureId = _Texture->GetRendererID();
	ImGui::Image((void*)textureId, { 64.0f, 64.0f });
}
