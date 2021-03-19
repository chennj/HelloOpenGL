#include "TestBatchIndices.h"

#include "../imgui/imgui.h"

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

#include "..\stb_image\stb_image.h"

#include <array>

namespace tests
{
	struct Vec3 {
		float x, y, z;
	};
	struct Vec4 {
		float r, g, b, a;
	};
	struct Vec2{
		float s, t;
	};
	struct Vertex
	{
		Vec3 Position;
		Vec4 Color;
		Vec2 TexCoords;
		float TexID;
	};

	static GLuint LoadTexture(const std::string& path)
	{
		int w, h, bits;

		stbi_set_flip_vertically_on_load(1);

		auto* pixels = stbi_load(path.c_str(), &w, &h, &bits, 4);
		GLuint textureID;
		GLCall(glGenTextures(1, &textureID));
		GLCall(glBindTexture(GL_TEXTURE_2D, textureID));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels));

		if (pixels)
			stbi_image_free(pixels);

		return textureID;
	}

	static std::array<Vertex, 4> CreateQuad(float x, float y, float textureID)
	{
		float size = 150.0f;

		Vertex v0;
		v0.Position = { x, y,  0.0f };
		v0.Color = { 0.18f, 0.6f, 0.96f, 1.0f };
		v0.TexCoords = { 0.0f, 0.0f };
		v0.TexID = textureID;

		Vertex v1;
		v1.Position = { x + size, y,  0.0f };
		v1.Color = { 0.18f, 0.6f, 0.96f, 1.0f };
		v1.TexCoords = { 1.0f, 0.0f };
		v1.TexID = textureID;

		Vertex v2;
		v2.Position = { x + size, y + size,  0.0f };
		v2.Color = { 0.18f, 0.6f, 0.96f, 1.0f };
		v2.TexCoords = { 1.0f, 1.0f };
		v2.TexID = textureID;

		Vertex v3;
		v3.Position = { x, y + size,  0.0f };
		v3.Color = { 0.18f, 0.6f, 0.96f, 1.0f };
		v3.TexCoords = { 0.0f, 1.0f };
		v3.TexID = textureID;
		
		return { v0, v1, v2, v3 };
	}

	static Vertex* CreateQuad(Vertex* target, float x, float y, float textureID)
	{
		float size = 50.0f;

		target->Position = { x, y,  0.0f };
		target->Color = { 0.18f, 0.6f, 0.96f, 1.0f };
		target->TexCoords = { 0.0f, 0.0f };
		target->TexID = textureID;
		target++;

		target->Position = { x + size, y,  0.0f };
		target->Color = { 0.18f, 0.6f, 0.96f, 1.0f };
		target->TexCoords = { 1.0f, 0.0f };
		target->TexID = textureID;
		target++;

		target->Position = { x + size, y + size,  0.0f };
		target->Color = { 0.18f, 0.6f, 0.96f, 1.0f };
		target->TexCoords = { 1.0f, 1.0f };
		target->TexID = textureID;
		target++;

		target->Position = { x, y + size,  0.0f };
		target->Color = { 0.18f, 0.6f, 0.96f, 1.0f };
		target->TexCoords = { 0.0f, 1.0f };
		target->TexID = textureID;
		target++;

		return target;
	}

	TestBatchIndices::TestBatchIndices()
		:_Translation(200, 200, 0),
		_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
		_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
	{
		//float positions[] = {
		//	//��һ�����Σ����������Σ�
		//	-50.0f, -50.0f,  0.0f,	/*pos 0*/	0.18f, 0.6f, 0.96f, 1.0f,	/*pos 0 color*/	0.0f, 0.0f,	/*pos 0 texture*/	0.0f,	/*texture index*/
		//	 100.0f,-50.0f,  0.0f,	/*pos 1*/	0.18f, 0.6f, 0.96f, 1.0f,	/*pos 0 color*/	1.0f, 0.0f,	/*pos 1 texture*/	0.0f,	/*texture index*/
		//	 100.0f, 100.0f, 0.0f,	/*pos 2*/	0.18f, 0.6f, 0.96f, 1.0f,	/*pos 0 color*/	1.0f, 1.0f,	/*pos 2 texture*/	0.0f,	/*texture index*/
		//	-50.0f,  100.0f, 0.0f,	/*pos 3*/	0.18f, 0.6f, 0.96f, 1.0f,	/*pos 0 color*/	0.0f, 1.0f,	/*pos 3 texture*/	0.0f,	/*texture index*/
		//	//�ڶ������Σ����������Σ�
		//	 250.0f, -50.0f, 0.0f,	/*pos 4*/	1.0f, 0.93f, 0.24f, 1.0f,	/*pos 4 color*/	0.0f, 0.0f,	/*pos 4 texture*/	1.0f,	/*texture index*/
		//	 400.0f, -50.0f, 0.0f,	/*pos 5*/	1.0f, 0.93f, 0.24f, 1.0f,	/*pos 4 color*/	1.0f, 0.0f,	/*pos 5 texture*/	1.0f,	/*texture index*/
		//	 400.0f,  100.0f,0.0f,	/*pos 6*/	1.0f, 0.93f, 0.24f, 1.0f,	/*pos 4 color*/	1.0f, 1.0f,	/*pos 6 texture*/	1.0f,	/*texture index*/
		//	 250.0f,  100.0f,0.0f,	/*pos 7*/	1.0f, 0.93f, 0.24f, 1.0f,	/*pos 4 color*/	0.0f, 1.0f,	/*pos 7 texture*/	1.0f,	/*texture index*/
		//};

		const size_t MaxQuadCount = 1000;
		const size_t MaxVertexCount = MaxQuadCount * 4;	// 1������ = 2�������� = 4�����㣨2�������غϣ�
		const size_t MaxIndexCount = MaxQuadCount * 6;

		GLCall(glGenVertexArrays(1, &_VA));
		GLCall(glBindVertexArray(_VA));

		GLCall(glGenBuffers(1, &_VB));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, _VB));
		GLCall(glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW));

		// ��ӦColorAndTexture.shader�� layout(location = 0) in vec3 a_Position;
		// ��_VB��_VA��0λ�ã�����Ϊ��Ч
		// ����0λ�õ�����
		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position)));
		// ��ӦColorAndTexture.shader�� layout(location = 1) in vec4 a_Color;
		// ��_VB��_VA��1λ�ã�����Ϊ��Ч
		// ����1λ�õ�����
		GLCall(glEnableVertexAttribArray(1));
		GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color)));
		// ��ӦColorAndTexture.shader�� layout(location = 1) in vec2	a_TexCoord;
		// ��_VB��_VA��2λ�ã�����Ϊ��Ч
		// ����2λ�õ�����
		GLCall(glEnableVertexAttribArray(2));
		GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexCoords)));
		// ��ӦColorAndTexture.shader�� layout(location = 1) in float a_TexIndex;
		// ��_VB��_VA��2λ�ã�����Ϊ��Ч
		// ����3λ�õ�����
		GLCall(glEnableVertexAttribArray(3));
		GLCall(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexID)));

		//unsigned int indices[] = {
		//	0, 1, 2,			//��1�����εĵ�1�������ε������������������positions[]��λ�ã�
		//	2, 3, 0,			//��1�����εĵ�2�������ε��������������
		//	4, 5, 6,			//��2�����εĵ�1�������ε��������������
		//	6, 7, 4				//��2�����εĵ�2�������ε��������������
		//};
		uint32_t indices[MaxIndexCount];
		uint32_t offset = 0;
		for (size_t i = 0; i < MaxIndexCount; i += 6)
		{
			indices[i + 0] = 0 + offset;
			indices[i + 1] = 1 + offset;
			indices[i + 2] = 2 + offset;

			indices[i + 3] = 2 + offset;
			indices[i + 4] = 3 + offset;
			indices[i + 5] = 0 + offset;

			offset += 4;
		}

		GLCall(glGenBuffers(1, &_IB));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IB));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

		_Tex01 = LoadTexture("../res/texture/texture-01.png");
		_Tex02 = LoadTexture("../res/texture/texture-02.png");

		_Shader = std::make_unique<Shader>("ColorAndTexture.shader");
		GLCall(glUseProgram(_Shader->GetRendererID()));
		GLCall(auto loc = glGetUniformLocation(_Shader->GetRendererID(), "u_Textures"));
		int samples[2] = { 0,1 };
		GLCall(glUniform1iv(loc, 2, samples));

		GLCall(glActiveTexture(GL_TEXTURE0 + 0));
		GLCall(glBindTexture(GL_TEXTURE_2D, _Tex01));
		GLCall(glActiveTexture(GL_TEXTURE0 + 1));
		GLCall(glBindTexture(GL_TEXTURE_2D, _Tex02));
		//opengl version 330 ��֧��
		//glBindTextureUnit(0, _Tex01);
		//glBindTextureUnit(1, _Tex02);
	}

	TestBatchIndices::~TestBatchIndices()
	{
		GLCall(glDeleteTextures(1, &_Tex01));
		GLCall(glDeleteTextures(1, &_Tex02));
	}

	// ���������OnRender����һ��
	void TestBatchIndices::OnUpdate(float deltaTime)
	{
		// Set dynamic vertex buffer
		//float positions[] = {
		//	//��һ�����Σ����������Σ�
		//	-50.0f, -50.0f,  0.0f,	/*pos 0*/	0.18f, 0.6f, 0.96f, 1.0f,	/*pos 0 color*/	0.0f, 0.0f,	/*pos 0 texture*/	0.0f,	/*texture index*/
		//	 100.0f,-50.0f,  0.0f,	/*pos 1*/	0.18f, 0.6f, 0.96f, 1.0f,	/*pos 0 color*/	1.0f, 0.0f,	/*pos 1 texture*/	0.0f,	/*texture index*/
		//	 100.0f, 100.0f, 0.0f,	/*pos 2*/	0.18f, 0.6f, 0.96f, 1.0f,	/*pos 0 color*/	1.0f, 1.0f,	/*pos 2 texture*/	0.0f,	/*texture index*/
		//	-50.0f,  100.0f, 0.0f,	/*pos 3*/	0.18f, 0.6f, 0.96f, 1.0f,	/*pos 0 color*/	0.0f, 1.0f,	/*pos 3 texture*/	0.0f,	/*texture index*/
		//	//�ڶ������Σ����������Σ�
		//	 250.0f, -50.0f, 0.0f,	/*pos 4*/	1.0f, 0.93f, 0.24f, 1.0f,	/*pos 4 color*/	0.0f, 0.0f,	/*pos 4 texture*/	1.0f,	/*texture index*/
		//	 400.0f, -50.0f, 0.0f,	/*pos 5*/	1.0f, 0.93f, 0.24f, 1.0f,	/*pos 4 color*/	1.0f, 0.0f,	/*pos 5 texture*/	1.0f,	/*texture index*/
		//	 400.0f,  100.0f,0.0f,	/*pos 6*/	1.0f, 0.93f, 0.24f, 1.0f,	/*pos 4 color*/	1.0f, 1.0f,	/*pos 6 texture*/	1.0f,	/*texture index*/
		//	 250.0f,  100.0f,0.0f,	/*pos 7*/	1.0f, 0.93f, 0.24f, 1.0f,	/*pos 4 color*/	0.0f, 1.0f,	/*pos 7 texture*/	1.0f,	/*texture index*/
		//};

		//auto q0 = CreateQuad(_QuadPositionA[0], _QuadPositionA[1], 0.0f);
		//auto q1 = CreateQuad(_QuadPositionB[0], _QuadPositionB[1], 1.0f);

		//Vertex vertices[8];
		//memcpy(vertices, q0.data(), q0.size() * sizeof(Vertex));
		//memcpy(vertices + q0.size(), q1.data(), q1.size() * sizeof(Vertex));

		////GLCall(glBindBuffer(GL_ARRAY_BUFFER, _VB));
		//GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));
	}

	void TestBatchIndices::OnRender()
	{
		GLCall(glClearColor(0.25f, 0.25f, 0.25f, 1));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		uint32_t indexCount = 0;

		std::array<Vertex, 1000> vertices;
		Vertex* buffer = vertices.data();
		for (int y = 0; y < 250; y+=50)
		{
			for (int x = 0; x < 250; x+=50)
			{
				buffer = CreateQuad(buffer, x, y, (x/50 + y/50) % 2);
				indexCount += 6;
			}
		}

		buffer = CreateQuad(buffer, _QuadPosition[0], _QuadPosition[1], 0.0f);
		indexCount += 6;

		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data()));

		glm::mat4 model = glm::translate(glm::mat4(1.0f), _Translation);
		glm::mat4 mvp = _Proj * _View * model;
		_Shader->Bind();
		_Shader->SetUniformMat4f("u_MVP", mvp);

		GLCall(glBindVertexArray(_VA));
		GLCall(glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr));
	}

	void TestBatchIndices::OnImGuiRender()
	{
		ImGui::DragFloat2("Quad Position", _QuadPosition, 10.0f);
	}

}
