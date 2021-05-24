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
	// 相机沿着Z轴移动-150单位,不然看不到物体,因为下面positions的z的坐标是0.0
	_View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
	_FOV = 45.0f;

	//GLCall(glViewport(0.0f, 0.0f, 960.0f, 540.0f));

	// 立方体八个顶点坐标-用作参考，注意：纹理坐标在不同的面需要调整
	//（右手逆时针，大拇指指向的法线方向为外面）
	//-0.5f,	-0.5f,	 0.5f,/*左下角顶点坐标 0*/	0.0f, 0.0f,		/*对应的纹理坐标*/
	// 0.5f,	-0.5f,	 0.5f,/*右下角顶点坐标 1*/	1.0f, 0.0f,		/*对应的纹理坐标*/
	// 0.5f,	 0.5f,	 0.5f,/*右上角顶点坐标 2*/	1.0f, 1.0f,		/*对应的纹理坐标*/							
	//-0.5f,	 0.5f,	 0.5f,/*左上角顶点坐标 3*/	0.0f, 1.0f,		/*对应的纹理坐标*/

	//-0.5f,	-0.5f,	-0.5f,/*左下角顶点坐标 4*/	0.0f, 1.0f,		/*对应的纹理坐标*/
	// 0.5f,	-0.5f,	-0.5f,/*右下角顶点坐标 5*/	1.0f, 1.0f,		/*对应的纹理坐标*/
	// 0.5f,	 0.5f,	-0.5f,/*右上角顶点坐标 6*/	1.0f, 0.0f,		/*对应的纹理坐标*/
	//-0.5f,	 0.5f,	-0.5f,/*左上角顶点坐标 7*/	0.0f, 0.0f,		/*对应的纹理坐标*/
	float positions[] = {

		-0.5f,	-0.5f,	 0.5f,/*左下角顶点坐标 0-0*/	0.0f, 0.0f,	/*对应的纹理坐标*/
		 0.5f,	-0.5f,	 0.5f,/*右下角顶点坐标 1-1*/	1.0f, 0.0f,	/*对应的纹理坐标*/
		 0.5f,	 0.5f,	 0.5f,/*右上角顶点坐标 2-2*/	1.0f, 1.0f,	/*对应的纹理坐标*/
		-0.5f,	 0.5f,	 0.5f,/*左上角顶点坐标 3-3*/	0.0f, 1.0f,	/*对应的纹理坐标*/
	
		-0.5f,	-0.5f,	-0.5f,/*左下角顶点坐标 4-4*/	0.0f, 0.0f,	/*对应的纹理坐标*/
		-0.5f,	-0.5f,	 0.5f,/*右下角顶点坐标 5-0*/	1.0f, 0.0f,	/*对应的纹理坐标*/
		-0.5f,	 0.5f,	 0.5f,/*右上角顶点坐标 6-3*/	1.0f, 1.0f,	/*对应的纹理坐标*/
		-0.5f,	 0.5f,	-0.5f,/*左上角顶点坐标 7-7*/	0.0f, 1.0f,	/*对应的纹理坐标*/

		 0.5f,	-0.5f,	-0.5f,/*左下角顶点坐标 8-5*/	0.0f, 0.0f,	/*对应的纹理坐标*/
		-0.5f,	-0.5f,	-0.5f,/*右下角顶点坐标 9-4*/	1.0f, 0.0f,	/*对应的纹理坐标*/
		-0.5f,	 0.5f,	-0.5f,/*右上角顶点坐标 10-7*/	1.0f, 1.0f,	/*对应的纹理坐标*/
		 0.5f,	 0.5f,	-0.5f,/*左上角顶点坐标 11-6*/	0.0f, 1.0f,	/*对应的纹理坐标*/

		 0.5f,	-0.5f,	 0.5f,/*左下角顶点坐标 12-1*/	0.0f, 0.0f,	/*对应的纹理坐标*/
		 0.5f,	-0.5f,	-0.5f,/*右下角顶点坐标 13-5*/	1.0f, 0.0f,	/*对应的纹理坐标*/
		 0.5f,	 0.5f,	-0.5f,/*右上角顶点坐标 14-6*/	1.0f, 1.0f,	/*对应的纹理坐标*/
		 0.5f,	 0.5f,	 0.5f,/*左上角顶点坐标 15-2*/	0.0f, 1.0f,	/*对应的纹理坐标*/

		-0.5f,	 0.5f,	 0.5f,/*左下角顶点坐标 16-3*/	0.0f, 0.0f,	/*对应的纹理坐标*/
		 0.5f,	 0.5f,	 0.5f,/*右下角顶点坐标 17-2*/	1.0f, 0.0f,	/*对应的纹理坐标*/
		 0.5f,	 0.5f,	-0.5f,/*右上角顶点坐标 18-6*/	1.0f, 1.0f,	/*对应的纹理坐标*/
		-0.5f,	 0.5f,	-0.5f,/*左上角顶点坐标 19-7*/	0.0f, 1.0f,	/*对应的纹理坐标*/

		-0.5f,	-0.5f,	-0.5f,/*左下角顶点坐标 20-4*/	0.0f, 0.0f,	/*对应的纹理坐标*/
		 0.5f,	-0.5f,	-0.5f,/*右下角顶点坐标 21-5*/	1.0f, 0.0f,	/*对应的纹理坐标*/
		 0.5f,	-0.5f,	 0.5f,/*右上角顶点坐标 22-1*/	1.0f, 1.0f,	/*对应的纹理坐标*/
		-0.5f,	-0.5f,	 0.5f,/*左上角顶点坐标 23-0*/	0.0f, 1.0f,	/*对应的纹理坐标*/
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

	_VAO = CreateScope<VertexArray>();
	_VBO = CreateScope<VertexBuffer>(positions, sizeof(positions)/*5 * 24 * sizeof(float)*/);
	VertexBufferLayout layout;
	layout.Push<float>(3);	// 顶点坐标(x,y)
	layout.Push<float>(2);	// 纹理坐标(s,t)

	_VAO->AddBuffer(*_VBO, layout);
	_IndexBuffer = CreateScope<IndexBuffer>(indices, sizeof(indices) / sizeof(unsigned int));

	_Shader = CreateScope<Shader>("shaders/Basic3D.shader");
	_Shader->Bind();

	_Texture = CreateScope<Texture>("../res/texture/texture-02-1.png");
	_Shader->SetUniform1i("u_Texture", 0/*因为我们的纹理绑定在0卡槽，看Texture::Bind()*/);


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
	* model 模型变换：将物体从局部坐标系变换到世界坐标系，决定物体所在的场景，
	*				比如物体是一个沙发，通过模型变换，决定这个沙发到底是在上海，还是在洛杉矶。
	* _View 视图变换：将相机作为观察点，并决定相机的位置的位置和方向，即世界坐标系变换到观察坐标系
	*				比如把相机放置在物体的左边（相当于物体向右边移动）
	* _Proj 投影变换：正交投影或透视投影（视锥体，近大远小，FOV视角大小），将物体投影到裁剪空间（近平面），
	*				比如物体是一个沙发，你到底是从左边看他，还是右边。使近距离看，还是离远了看，
	*				是从上到下看，还是从下往上看，如果里的太近你可能只看得到沙发的一部分。
	*/
	{
		_Proj = glm::perspective(glm::radians(_FOV), 1200.0f/600.0f/*Viewport的width/height*/, 0.1f, 1000.0f);
		_Model = glm::translate(glm::mat4(1.0f), _MPosition)
			* glm::rotate(glm::mat4(1.0f), glm::radians(_MRotationAngle), _MRotationDirection)
			* glm::scale(glm::mat4(1.0f), _MScale);
		_Shader->Bind();
		_Shader->SetUniformMat4f("u_Model", _Model);
		_Shader->SetUniformMat4f("u_View", _View);
		_Shader->SetUniformMat4f("u_Proj", _Proj);
		renderer.Draw(*_VAO, *_IndexBuffer, *_Shader);
	}

	// 更多的立方体
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
