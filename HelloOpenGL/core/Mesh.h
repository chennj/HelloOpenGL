#pragma once

#include "Base.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <string>
#include <vector>

struct AssimpVertex
{
	// ����λ��
	glm::vec3 Position;
	// ÿһ����ķ�����
	glm::vec3 Normal;
	// ��������
	glm::vec2 TexCoords;
	// ����
	glm::vec3 Tangent;
	// ˫����
	glm::vec3 Bitangent;
};

struct AssimpTexture
{
	unsigned int Id;
	std::string Type;
	std::string Path;
};

class Mesh
{
public:
	/*  ��Ⱦ����  */
	unsigned int VAO, VBO, IBO;

public:
	Mesh(float vertexs[], unsigned int indices[], std::vector<AssimpTexture> textures);
	Mesh(std::vector<AssimpVertex> vertices, std::vector<unsigned int> indices, std::vector<AssimpTexture> textures);
	~Mesh();

public:
	/*  ��������  */
	std::vector<AssimpVertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	std::vector<AssimpTexture> m_Textures;

private:
	/*  ����  */
	void SetupMesh();

public:
	/*  ����  */
	void Draw(Ref<Shader>& shader);
	void Draw1(Ref<Shader>& shader);
};