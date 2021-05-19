#include "TestTexture3D.h"

#include "../core/Renderer.h"
#include "../imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace tests;

TestTexture3D::TestTexture3D()
{
	_MPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	_MRotationAngle = 0.0f;
	_MScale = glm::vec3(1.0f, 1.0f, 1.0f);
	_MRotationDirection = glm::vec3(1.0f, 0.0f, 0.0f);
	// �������Z���ƶ�-150��λ,��Ȼ����������,��Ϊ����positions��z��������0.0
	_View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -150.0f));
	_FOV = 45.0f;
	//_Proj = glm::ortho(-120.0f, 120.0f, -60.0f, 60.0f, -1.0f, 1.0f);

	//GLCall(glViewport(0.0f, 0.0f, 960.0f, 540.0f));

	float positions[] = {
			-50.0f,	-50.0f,	0.0f,/*���½Ƕ�������*/	0.0f, 0.0f,		/*��Ӧ����������*/
			 50.0f,	-50.0f,	0.0f,/*���½Ƕ�������*/	1.0f, 0.0f,		/*��Ӧ����������*/
			 50.0f,	 50.0f,	0.0f,/*���ϽǶ�������*/	1.0f, 1.0f,		/*��Ӧ����������*/
							
			-50.0f,	 50.0f,	0.0f,/*���ϽǶ�������*/	0.0f, 1.0f		/*��Ӧ����������*/
	};

	unsigned int indices[] = {
		0, 1, 2,		//��һ�������ε����������������index��
		2, 3, 0			//�ڶ��������ε��������������
	};

	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	_VAO = CreateScope<VertexArray>();
	_VBO = CreateScope<VertexBuffer>(positions, 5 * 4 * sizeof(float));
	VertexBufferLayout layout;
	layout.Push<float>(3);	// ��������(x,y)
	layout.Push<float>(2);	// ��������(s,t)

	_VAO->AddBuffer(*_VBO, layout);
	_IndexBuffer = CreateScope<IndexBuffer>(indices, 6);

	_Shader = CreateScope<Shader>("shaders/Basic3D.shader");
	_Shader->Bind();
	_Shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

	_Texture = CreateScope<Texture>("../res/texture/cnchess/WHITE.GIF");
	_Shader->SetUniform1i("u_Texture", 0/*��Ϊ���ǵ��������0���ۣ���Texture::Bind()*/);
}

TestTexture3D::~TestTexture3D()
{
}

void TestTexture3D::OnUpdate(float deltaTime)
{
}

void TestTexture3D::OnRender()
{
	GLCall(glClearColor(0, 0, 0, 1));
	GLCall(glClear(GL_COLOR_BUFFER_BIT));

	Renderer renderer;
	_Texture->Bind();

	/**
	* model ģ�ͱ任��������Ӿֲ�����ϵ�任����������ϵ�������������ڵĳ�����
	*				����������һ��ɳ����ͨ��ģ�ͱ任���������ɳ�����������Ϻ�����������ɼ��
	* _View ��ͼ�任���������Ϊ�۲�㣬�����������λ�õ�λ�úͷ��򣬼���������ϵ�任���۲�����ϵ
	*				���������������������ߣ��൱���������ұ��ƶ���
	* _Proj ͶӰ�任��ѡ��۲�ķ�ʽ������ͶӰ��͸��ͶӰ��������ͶӰ���ü��ռ䣬
	*				����������һ��ɳ�����㵽���Ǵ���߿����������ұߡ�ʹ�����뿴��������Զ�˿���
	*				�Ǵ��ϵ��¿������Ǵ������Ͽ���������̫�������ֻ���õ�ɳ����һ���֡�
	*/
	{
		_Proj = glm::perspective(glm::radians(_FOV), 1200.0f/600.0f/*Viewport��width/height*/, 0.1f, 1000.0f);
		_Model = glm::translate(glm::mat4(1.0f), _MPosition)
			* glm::rotate(glm::mat4(1.0f), glm::radians(_MRotationAngle), _MRotationDirection)
			* glm::scale(glm::mat4(1.0f), _MScale);
		glm::mat4 mvp = _Proj * _View * _Model;
		_Shader->Bind();
		_Shader->SetUniformMat4f("u_MVP", mvp);
		renderer.Draw(*_VAO, *_IndexBuffer, *_Shader);
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
