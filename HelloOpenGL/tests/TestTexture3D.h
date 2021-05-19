#pragma once

#include "Test.h"

#include "../core/VertexBuffer.h"
#include "../core/VertexBufferLayout.h"
#include "../core/Texture.h"

namespace tests
{
	class TestTexture3D : public Test
	{
	private:
		Scope<VertexArray> _VAO;
		Scope<VertexBuffer> _VBO;
		Scope<IndexBuffer> _IndexBuffer;
		Scope<Shader> _Shader;
		Scope<Texture> _Texture;

		glm::vec3 _MPosition;
		glm::mat4 _MRotation;
		glm::vec3 _MRotationDirection;
		float _MRotationAngle;
		glm::vec3 _MScale;

		glm::mat4 _Proj, _View, _Model;
		float _FOV;

	public:
		TestTexture3D();
		~TestTexture3D();

	public:
		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}