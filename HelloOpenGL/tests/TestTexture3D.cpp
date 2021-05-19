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
	// 相机沿着Z轴移动-150单位,不然看不到物体,因为下面positions的z的坐标是0.0
	_View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -150.0f));
	_FOV = 45.0f;
	//_Proj = glm::ortho(-120.0f, 120.0f, -60.0f, 60.0f, -1.0f, 1.0f);

	//GLCall(glViewport(0.0f, 0.0f, 960.0f, 540.0f));

	float positions[] = {
			-50.0f,	-50.0f,	0.0f,/*左下角顶点坐标*/	0.0f, 0.0f,		/*对应的纹理坐标*/
			 50.0f,	-50.0f,	0.0f,/*右下角顶点坐标*/	1.0f, 0.0f,		/*对应的纹理坐标*/
			 50.0f,	 50.0f,	0.0f,/*右上角顶点坐标*/	1.0f, 1.0f,		/*对应的纹理坐标*/
							
			-50.0f,	 50.0f,	0.0f,/*左上角顶点坐标*/	0.0f, 1.0f		/*对应的纹理坐标*/
	};

	unsigned int indices[] = {
		0, 1, 2,		//第一个三角形的三个顶点的索引（index）
		2, 3, 0			//第二个三角形的三个顶点的索引
	};

	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	_VAO = CreateScope<VertexArray>();
	_VBO = CreateScope<VertexBuffer>(positions, 5 * 4 * sizeof(float));
	VertexBufferLayout layout;
	layout.Push<float>(3);	// 顶点坐标(x,y)
	layout.Push<float>(2);	// 纹理坐标(s,t)

	_VAO->AddBuffer(*_VBO, layout);
	_IndexBuffer = CreateScope<IndexBuffer>(indices, 6);

	_Shader = CreateScope<Shader>("shaders/Basic3D.shader");
	_Shader->Bind();
	_Shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

	_Texture = CreateScope<Texture>("../res/texture/cnchess/WHITE.GIF");
	_Shader->SetUniform1i("u_Texture", 0/*因为我们的纹理绑定在0卡槽，看Texture::Bind()*/);
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
	* model 模型变换：将物体从局部坐标系变换到世界坐标系，决定物体所在的场景，
	*				比如物体是一个沙发，通过模型变换，决定这个沙发到底是在上海，还是在洛杉矶。
	* _View 视图变换：将相机作为观察点，并决定相机的位置的位置和方向，即世界坐标系变换到观察坐标系
	*				比如把相机放置在物体的左边（相当于物体向右边移动）
	* _Proj 投影变换：选择观察的方式，正交投影或透视投影，将物体投影到裁剪空间，
	*				比如物体是一个沙发，你到底是从左边看他，还是右边。使近距离看，还是离远了看，
	*				是从上到下看，还是从下往上看，如果里的太近你可能只看得到沙发的一部分。
	*/
	{
		_Proj = glm::perspective(glm::radians(_FOV), 1200.0f/600.0f/*Viewport的width/height*/, 0.1f, 1000.0f);
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
