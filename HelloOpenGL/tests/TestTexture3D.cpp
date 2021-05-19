#include "TestTexture3D.h"

#include "../core/Renderer.h"
#include "../imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace tests;

TestTexture3D::TestTexture3D()
{
	_Translation = glm::vec3(100.0f, 100.0f, 0.0f);
	_View = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	_Proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);

	float positions[] = {
			-50.0f,	-50.0f,	/*左下角顶点坐标*/	0.0f, 0.0f,		/*对应的纹理坐标*/
			 50.0f,	-50.0f,	/*右下角顶点坐标*/	1.0f, 0.0f,		/*对应的纹理坐标*/
			 50.0f,	 50.0f,	/*右上角顶点坐标*/	1.0f, 1.0f,		/*对应的纹理坐标*/

			-50.0f,	 50.0f,	/*左上角顶点坐标*/	0.0f, 1.0f		/*对应的纹理坐标*/
	};

	unsigned int indices[] = {
		0, 1, 2,		//第一个三角形的三个顶点的索引（index）
		2, 3, 0			//第二个三角形的三个顶点的索引
	};

	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	_VAO = CreateScope<VertexArray>();
	_VBO = CreateScope<VertexBuffer>(positions, 4 * 4 * sizeof(float));
	VertexBufferLayout layout;
	layout.Push<float>(2);	// 顶点坐标(x,y)
	layout.Push<float>(2);	// 纹理坐标(s,t)

	_VAO->AddBuffer(*_VBO, layout);
	_IndexBuffer = CreateScope<IndexBuffer>(indices, 6);

	_Shader = CreateScope<Shader>("shaders/Basic.shader");
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

	{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), _Translation);
		glm::mat4 mvp = _Proj * _View * model;
		_Shader->Bind();
		_Shader->SetUniformMat4f("u_MVP", mvp);
		renderer.Draw(*_VAO, *_IndexBuffer, *_Shader);
	}
}

void TestTexture3D::OnImGuiRender()
{
	ImGui::SliderFloat3("Translation", &_Translation.x, 0.0f, 960.0f);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	uint32_t textureId = _Texture->GetRendererID();
	ImGui::Image((void*)textureId, { 64.0f, 64.0f });
}
