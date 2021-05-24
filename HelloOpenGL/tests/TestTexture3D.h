#pragma once

#include "Test.h"

#include <random>

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

		glm::vec3* _CubePositions;
		std::default_random_engine _UEngine;
		std::uniform_real_distribution<float> _UInt{ 0.1f,0.9f };
	public:
		TestTexture3D();
		~TestTexture3D();

	public:
		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}